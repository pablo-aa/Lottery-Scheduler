#ifndef PROCESS_H
#define PROCESS_H

// Process struct
typedef struct {
    char name[50];
    int priority;
} Process;

// Process functions
void execute_process(Process *process);
void print_process(Process *process);

#endif