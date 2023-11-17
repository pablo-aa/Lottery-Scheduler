#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>

typedef struct Process Process;
// Process struct
struct Process{
    char name[256]; // process name
    int number_of_tickets; // number of tickets based on priority
    int received_tickets[4]; // received tickets in lottery
    int pid;
    time_t ready_time; // time when process is ready
    time_t start_time; // time when process starts
    time_t end_time; // time when process ends

    struct Process *next; // next process
};

// Process functions
// Create new process
Process *create_process(char *name, int number_of_tickets);
// Add process to the end of the list
void add_process(Process **head, Process *process);
// Remove process from the list
void remove_process(Process **head, Process *process);

Process* select_process(Process *head);

#endif