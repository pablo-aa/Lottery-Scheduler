// logger.h
#ifndef LOGGER_H
#define LOGGER_H
#include "process.h"

typedef struct Logger Logger;
struct Logger{
    Process *processes;
};

// Create new logger
void create_logger(Logger **logger);
// Add a new process to the logger
void add_process_to_logger(Logger *logger, Process *process);
// Print all processes in the logger
void print_all_processes(Logger *logger);

#endif // LOGGER_H
