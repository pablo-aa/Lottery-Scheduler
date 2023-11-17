#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "message.h"
#include "process.h"
#include "logger.h"
#include "common.h"

/**
    * This file is responsible to execute the lottery scheduler algorithm.
    * 
    * @file scheduler.c
*/

volatile sig_atomic_t alarm_fired = 0;

void handle_sigalrm(int sig) {
    alarm_fired = 1;
}

int main() {
    // inicializar a fila de mensagens
    key_t key = MSG_QUEUE_KEY;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Error in creating message queue");
        return -1;
    }
    // inicializar variáveis envolvidas no escalonamento
    // criando uma lista de processos
    Process *process_list = NULL;

    Process *gantt_list = NULL;

    // Inicializando o logger
    Logger *logger = NULL;
    create_logger(&logger);

    // Configurar signal para detectar final do timer
    struct sigaction sa;
    sa.sa_handler = &handle_sigalrm;
    sigaction(SIGALRM, &sa, NULL);

    // criar o while responsável por fazer a leitura da fila de mensagens
    // e depois fazer o escalonamento
    int finished_message_queue = 0;
    for(;;){
        // realizar a leitura da fila de mensagens
        struct message mensagem;
        while (!finished_message_queue) {
            ssize_t result = msgrcv(msgid, &mensagem, sizeof(struct message) - sizeof(long), 0, IPC_NOWAIT);
            
            if (result != -1) {
                // adiciona o processo na process_list se não for mensagem de término
                if (strcmp(mensagem.process_name, "end") != 0) {
                    if(VERBOSE){ printf("Received message: %s\n", mensagem.process_name); }
                    Process *process = create_process(mensagem.process_name, mensagem.priority + 1, 1);
                    process->ready_time = mensagem.ready_time;
                    add_process(&process_list, process);
                } else {
                    finished_message_queue = 1;
                    if(VERBOSE){ printf("Mensagem de término recebida\n"); }
                }
            } else if (errno == ENOMSG) {
                // Se não houver mensagem, saia do loop
                break;
            }
        }

        // Se a fila de mensagens estiver vazia e a lista de processos também, sair do loop
        if(finished_message_queue && process_list == NULL){
            if(VERBOSE){ printf("Fila de mensagens vazia e lista de processos vazia\n"); }
            break;
        }

        // if (errno != ENOMSG) {
        //     perror("msgrcv");
        // }

        // se a lista de processos não estiver vazia, executar arquivo padrão com busy waiting
        if(process_list != NULL){
            // selecionar o processo a ser executado
            Process *process = select_process_lottery(process_list);

            // executar o processo
            if(VERBOSE){ printf("Executing process: %s\n", process->name); }

            Process *process_copy_gantt = malloc(sizeof(Process));
            if (process_copy_gantt == NULL) {
                perror("Failed to allocate memory for process copy");
                exit(EXIT_FAILURE);
            }
            *process_copy_gantt = *process;

            add_process(&gantt_list, process_copy_gantt);

            if (process->is_new) {
                if(VERBOSE){ printf("Processo novo\n"); }
                process->start_time = time(NULL);
                // O processo é novo, então tem que ser criado
                process->pid = fork();
                if (process->pid == 0) {
                    execl("./busy_wait", "./busy_wait", process->name, NULL);
                } else if (process->pid > 0) {
                    if(VERBOSE){ printf("PID of child process: %d\n", process->pid); }
                } else { // falha
                    perror("fork");
                }
            } else {
                if(VERBOSE){ printf("Processo já criado, continuando...\n"); }
                // processo já criado, retomando a execução
                kill(process->pid, SIGCONT);
            }

            // inicializar o timer do quantum
            if(VERBOSE){ printf("\n QUANTUM \n"); }
            alarm(6);

            // esperar o processo terminar ou o quantum ser acionado
            int status;
            while (!alarm_fired && waitpid(process->pid, &status, WNOHANG) == 0) {
                // O processo filho ainda está em execução
                sleep(1);  // Espere um pouco antes de verificar novamente
            }

            if (alarm_fired) {
                // O quantum foi acionado
                if(VERBOSE){ printf("Quantum expired for process %d\n", process->pid); }
                kill(process->pid, SIGSTOP);
                // process is not new anymore
                process->is_new = 0;
                alarm_fired = 0;  // Reset the alarm flag
            } else if (WIFEXITED(status)) {
                // O processo filho terminou
                if(VERBOSE){ printf("Child process exited with status %d\n", WEXITSTATUS(status)); }
                process->end_time = time(NULL);
                
                Process *process_copy = malloc(sizeof(Process));
                if (process_copy == NULL) {
                    perror("Failed to allocate memory for process copy");
                    exit(EXIT_FAILURE);
                }
                *process_copy = *process;
                // Adicionar processo ao logger
                add_process_to_logger(logger, process_copy);

                remove_process(&process_list, process);
            } else if (WIFSIGNALED(status)) {
                if(VERBOSE){ printf("Child process was killed by signal %d\n", WTERMSIG(status)); }
            }
        }

    }

    /*************************
    * LOGGER INFO GENERATION *
    **************************/
    print_all_processes(logger);

    /*************************
    * GANTT CHART GENERATION *
    **************************/

    printf("\n=============GANTT=============\n");
    char* gantt[500];
    // Inicializa o array de strings
    for (int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++) {
        gantt[i] = malloc(sizeof(char) * 500);
        if (gantt[i] == NULL) {
            perror("Failed to allocate memory for gantt");
            exit(EXIT_FAILURE);
        }
        gantt[i][0] = '\0';
    }
    Process *current = gantt_list;

    int n_exists = 0;
    while (current != NULL) {
        // verifica se o current->name já está no gantt
        int exists = 0;
        for(int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++){
            if(gantt[i] != NULL && strcmp(gantt[i], current->name) == 0){
                exists = 1;
                break;
            }
        }

        if(!exists){
            n_exists++;
            for(int i = 0; i < MAX_NUMBER_OF_PROCESSES; i++){
                if(gantt[i][0] == '\0'){
                    strcpy(gantt[i], current->name);
                    break;
                }
            }
        }

        current = current->next;
    }
    // Itera sobre a lista ligada
    for (Process* gl = gantt_list; gl != NULL; gl = gl->next) {
        char* name = gl->name;

        // Verifica se o nome já está presente em gantt e adiciona execução do processo
        for (int i = 0; i < n_exists; i++) {
            if (strncmp(gantt[i], name, strlen(name)) == 0) {
                strcat(gantt[i], "---");
            } else {
                strcat(gantt[i], "   ");
            }
        }
    }
    // Imprime os resultados
    for (int i = 0; i < n_exists; i++) {
        printf("%s\n", gantt[i]);
    }

    // Libera a memória alocada para as strings em gantt
    for (int i = 0; i < n_exists; i++) {
        free(gantt[i]);
    }

    printf("\n================================\n");

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}