#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "message.h"
#include "process.h"

volatile sig_atomic_t alarm_fired = 0;

void handle_sigalrm(int sig) {
    alarm_fired = 1;
}

int main() {
    // inicializar a fila de mensagens
    key_t key = 1234;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Error in creating message queue");
        return -1;
    }
    // inicializar variáveis envolvidas no escalonamento
    // criando uma lista de processos
    Process *process_list = NULL;

    // Configurar signal para detectar final do timer
    struct sigaction sa;
    sa.sa_handler = &handle_sigalrm;
    sigaction(SIGALRM, &sa, NULL);

    // criar o while responsável por fazer a leitura da fila de mensagens
    // e depois fazer o escalonamento
    int finished_message_queue = 0;
    while(1){
        // realizar a leitura da fila de mensagens
        struct message mensagem;
        while (!finished_message_queue) {
            ssize_t result = msgrcv(msgid, &mensagem, sizeof(struct message) - sizeof(long), 0, IPC_NOWAIT);
            
            if (result != -1) {
                // adiciona o processo na process_list se não for mensagem de término
                if (strcmp(mensagem.process_name, "end") != 0) {
                    printf("Received message: %s\n", mensagem.process_name);
                    Process *process = create_process(mensagem.process_name, mensagem.priority + 1, 1);
                    add_process(&process_list, process);
                } else {
                    finished_message_queue = 1;
                    printf("Mensagem de término recebida\n");
                }
            } else if (errno == ENOMSG) {
                // Se não houver mensagem, saia do loop
                break;
            }
        }

        // Se a fila de mensagens estiver vazia e a lista de processos também, sair do loop
        if(finished_message_queue && process_list == NULL){
            printf("Fila de mensagens vazia e lista de processos vazia\n");
            break;
        }

        // if (errno != ENOMSG) {
        //     perror("msgrcv");
        // }

        // se a lista de processos não estiver vazia, executar arquivo padrão com busy waiting
        if(process_list != NULL){
            // selecionar o processo a ser executado
            Process *process = select_process(process_list);

            // executar o processo
            printf("Executing process: %s\n", process->name);

            if (process->is_new) {
                printf("Processo novo\n");
                // O processo é novo, então tem que ser criado
                process->pid = fork();
                if (process->pid == 0) {
                    execl("./busy_wait", "./busy_wait", process->name, NULL);
                } else if (process->pid > 0) {
                    printf("PID of child process: %d\n", process->pid);
                } else { // falha
                    perror("fork");
                }
            } else {
                printf("Processo já criado, continuando...\n");
                // processo já criado, retomando a execução
                kill(process->pid, SIGCONT);
            }

            // inicializar o timer do quantum
            printf("\n QUANTUM \n");
            alarm(6);

            // esperar o processo terminar ou o quantum ser acionado
            int status;
            while (!alarm_fired && waitpid(process->pid, &status, WNOHANG) == 0) {
                // O processo filho ainda está em execução
                sleep(1);  // Espere um pouco antes de verificar novamente
            }

            if (alarm_fired) {
                // O quantum foi acionado
                printf("Quantum expired for process %d\n", process->pid);
                kill(process->pid, SIGSTOP);
                // process is not new anymore
                process->is_new = 0;
                alarm_fired = 0;  // Reset the alarm flag
            } else if (WIFEXITED(status)) {
                // O processo filho terminou
                printf("Child process exited with status %d\n", WEXITSTATUS(status));
                remove_process(&process_list, process);
            } else if (WIFSIGNALED(status)) {
                printf("Child process was killed by signal %d\n", WTERMSIG(status));
            }
        }

    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}