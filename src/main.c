#include <stdio.h>
#include "process.h"
#include "scheduler.h"
#include "logger.h"
#include "common.h"

int main() {

  FILE *fp;
  fp = fopen("processes.txt", "r");

  if (fp == NULL) {
    printf("Error opening file.\n");
    return 1;
  }

  // Create process size based on file size
  Process process_array[10000];
  int i = 0;
  while (fscanf(fp, "%s %d", process_array[i].name, &process_array[i].priority) != EOF) i++;

  fclose(fp);

  printf("Printing process array from file:\n");
  // Print process array
  for (int j = 0; j < i; j++) print_process(&process_array[j]);

  printf("\nPrinting fake execute process\n");
  // Execute process array
  for (int j = 0; j < i; j++) execute_process(&process_array[j]);

  Scheduler scheduler;

  // Initialize scheduler
  init_scheduler(&scheduler, "lottery scheduler", DEFAULT_QUANTUM, TIME_TO_READY, process_array);

  printf("\nPrinting scheduler info\n");
  // Print scheduler
  printf("Scheduler: %s, quantum: %d, time to ready: %d \n", scheduler.name, scheduler.quantum, scheduler.time);

  printf("\nPrinting processes inside scheduler \n");

  // Print processes inside scheduler
  for (int j = 0; j < i; j++) print_process(&scheduler.processes[j]);

  return 0;
}