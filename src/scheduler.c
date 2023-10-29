#include "scheduler.h"
#include <string.h>

// Scheduler implementations
void init_scheduler(Scheduler *scheduler, char *name, int quantum, int time, Process *processes) {
    strcpy(scheduler->name, name);
    scheduler->quantum = quantum;
    scheduler->time = time;
    scheduler->processes = processes;
}
