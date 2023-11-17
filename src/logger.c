#include "logger.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

/**
    * This file contains the logic to log the processes.
    * 
    * @file logger.c
*/

// Logger implementations
void create_logger(Logger **logger) {
    // allocate memory for the logger
    *logger = malloc(sizeof(Logger));
    if (*logger == NULL) {
        perror("Failed to allocate memory for logger");
        exit(EXIT_FAILURE);
    }
    (*logger)->processes = NULL;
}

void add_process_to_logger(Logger *logger, Process *process) {
  add_process(&(logger->processes), process);
}

void print_all_processes(Logger *logger) {
  Process *current = logger->processes;
  // general logger
  printf("\n=============LOGGER=============\n");
  while (current != NULL) {
    printf("Process: %s\n", current->name);
    printf("ready_time: %ld\n", current->ready_time);
    printf("start_time: %ld\n", current->start_time);
    printf("end_time: %ld\n\n", current->end_time);
    current = current->next;
  }
  printf("\n================================\n");

  // makespan
  current = logger->processes;
  printf("\n============MAKESPAN============\n");
  while (current != NULL) {
    printf("Process: %s\n", current->name);
    printf("makespan: %ld\n\n", current->end_time - current->ready_time);
    current = current->next;
  }
  printf("\n================================\n");

  // turnaround time
  current = logger->processes;
  printf("\n==========TURNAROUND TIME=======\n");
  while (current != NULL) {
    printf("Process: %s\n", current->name);
    printf("turnaround time: %ld\n\n", current->end_time - current->start_time);
    current = current->next;
  }
  printf("\n================================\n");
}
