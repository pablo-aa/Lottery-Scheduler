#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.h"

// Scheduler struct
typedef struct {
    char name[100];
    int quantum;
    int time;
    Process *processes;
} Scheduler;
// Scheduler functions
void init_scheduler(Scheduler *scheduler, char *name, int quantum, int time, Process *processes);

#endif