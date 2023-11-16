#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include "message.h"


int main() {
    // Fake scheduler only to test main.c
    // Obtendo o identificador da fila de mensagens criada pelo main
    key_t key = 1234;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Error in creating message queue");
        return -1;
    }

    printf("Starting scheduler.c\n");
    printf("Message queue id: %d\n", msgid);

    struct message message;
    while (1) {
        // Esperando por uma mensagem
        if (msgrcv(msgid, &message, sizeof(struct message) - sizeof(long), 0, 0) == -1) {
            if (errno == ENOMSG) {
                break;
            } else {
                perror("msgrcv");
                break;
            }
        }
        
        // Imprime a mensagem recebida
        printf("Message received: %s %d\n", message.process_name, message.priority);

        // Se a mensagem for de término, sai do loop
        if (strcmp(message.process_name, "end") == 0) {
            break;
        }
    }

    // Removendo a fila de mensagens
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}