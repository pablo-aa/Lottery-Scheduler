#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "message.h"
#include "process.h"


void timer_handler(int signum);
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
    // signal(SIGALRM, timer_handler);

    // criar o while responsável por fazer a leitura da fila de mensagens
    // e depois fazer o escalonamento
    int finished_message_queue = 0;
    while(1){
        // realizar a leitura da fila de mensagens
        struct message message;
        while (!finished_message_queue) {
            ssize_t result = msgrcv(msgid, &message, sizeof(struct message) - sizeof(long), 0, IPC_NOWAIT);
            
            if (result != -1) {
                // adiciona o processo na process_list se não for mensagem de término
                if (strcmp(message.process_name, "end") != 0) {
                    printf("Received message: %s\n", message.process_name);
                    Process *process = create_process(message.process_name, message.priority + 1);
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

        // selecionar o processo a ser executado
        Process *process = select_process(process_list);
        printf("Executing process %s\n", process->name);
        remove_process(&process_list, process);

        printf("Sleeping for 6 seconds\n");
        sleep(6);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}