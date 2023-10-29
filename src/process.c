#include "process.h"
#include <stdio.h>
#include <string.h>

// Process implementations
void execute_process(Process *process) {
    printf("Process %s + UNIX call.\n", process->name);
}

void print_process(Process *process) {
    printf("Process: %s, priority: %d\n", process->name, process->priority);
}