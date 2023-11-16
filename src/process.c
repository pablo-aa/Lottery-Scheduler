#include "process.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Process implementations
// Implementations to interact with the process struct and linked list
void add_process(Process **head, Process *process) {
    process->next = NULL;

    if (*head == NULL) {
        *head = process;
    } else {
        Process *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = process;
    }
}

void remove_process(Process **head, Process *process) {
    if (*head == NULL) {
        return;
    }

    if (*head == process) {
        *head = process->next;
        free(process);
        return;
    }

    Process *current = *head;
    while (current->next != NULL && current->next != process) {
        current = current->next;
    }

    if (current->next == NULL) {
        return;
    }

    current->next = process->next;
    free(process);
}

// Lottery scheduler implementations
// Implementations to interact with the lottery scheduler, generating tickets and selecting a winner