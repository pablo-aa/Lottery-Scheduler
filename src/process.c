#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "common.h"

/**
    * This file contains the implementation of the process struct and its functions.
    * The functions includes the lottery scheduler algorithm.
    * 
    * @file process.c
*/



Process* create_process(char *name, int number_of_tickets, int is_new) {
    Process *process = malloc(sizeof(Process));
    strcpy(process->name, name);
    process->number_of_tickets = number_of_tickets;
    process->is_new = is_new;
    process->next = NULL;
    return process;
}

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

Process* select_process(Process *process_list) {
    return process_list;
}

// Lottery scheduler implementations

// Selects a process based on the lottery algorithm
Process* select_process_lottery(Process *process_list) {
    int total_tickets = 0;
    Process *current = process_list;
    while (current != NULL) {
        total_tickets += current->number_of_tickets;
        current = current->next;
    }
    // generate a list of tickets
    int *tickets = malloc(sizeof(int) * total_tickets);

    // populate tickets array from 0 to total_tickets
    for (int i = 0; i < total_tickets; i++) {
        tickets[i] = i;
    }

    // shuffle tickets array
    for (int i = 0; i < total_tickets; i++) {
        int random_index = rand() % total_tickets;
        int temp = tickets[i];
        tickets[i] = tickets[random_index];
        tickets[random_index] = temp;
    }

    // distribute tickets to processes
    current = process_list;
    int ticket_index = 0;
    while (current != NULL) {
        int number_of_tickets = current->number_of_tickets;
        for (int i = 0; i < number_of_tickets; i++) {
            current->received_tickets[i] = tickets[ticket_index];
            ticket_index++;
        }
        current = current->next;
    }

    // select a random ticket
    int random_ticket = rand() % total_tickets;

    // find the process that has the ticket
    current = process_list;
    while (current != NULL) {
        int number_of_tickets = current->number_of_tickets;
        for (int i = 0; i < number_of_tickets; i++) {
            if (current->received_tickets[i] == random_ticket) {
                // free tickets array
                free(tickets);

                // print the summary of the lottery
                if(VERBOSE){ 
                    printf("\n=============LOTTERY=============\n");
                    printf("Total tickets: %d\n", total_tickets);

                    // print the tickets of each process
                    Process *current2 = process_list;
                    while (current2 != NULL) {
                        printf("Process: %s\n", current2->name);
                        printf("Tickets: ");
                        for (int i = 0; i < current2->number_of_tickets; i++) {
                            printf("%d ", current2->received_tickets[i]);
                        }
                        printf("\n");
                        current2 = current2->next;
                    }

                    printf("Random ticket: %d\n", random_ticket);
                    printf("Process selected: %s\n", current->name);
                    printf("\n================================\n");
                }
    

                return current;
            }
        }
        current = current->next;
    }

    return NULL;
}