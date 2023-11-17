#include "process.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Process implementations
Process* create_process(char *name, int number_of_tickets) {
    Process *process = malloc(sizeof(Process));
    strcpy(process->name, name);
    process->number_of_tickets = number_of_tickets;
    process->next = NULL;
    return process;
}

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

// fake select process function
Process* select_process(Process *process_list) {
    Process *process = process_list;
    return process;
}