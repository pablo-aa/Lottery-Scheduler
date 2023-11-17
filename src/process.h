#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>

typedef struct Process Process;
// Process struct
struct Process{
    char name[256];
    int number_of_tickets; // priority + 1
    int received_tickets[4];
    int pid;
    int is_new; // flag if process is new (1 - new, 0 - old)
    time_t ready_time; // time when process was added to "ready"
    time_t start_time; // time when process was added to "running" for the first time
    time_t end_time; // time when process terminated

    struct Process *next; // linked list
};

// Create new process
Process *create_process(char *name, int number_of_tickets, int is_new);
// Add process to the end of the list
void add_process(Process **head, Process *process);
// Remove process from the list
void remove_process(Process **head, Process *process);
// Select the first process in the list
Process* select_process(Process *head);
// Select the process folowing the lottery algorithm
Process* select_process_lottery(Process *process_list);

#endif