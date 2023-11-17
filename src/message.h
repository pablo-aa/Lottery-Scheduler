// message.h
#ifndef MESSAGE_H
#define MESSAGE_H
#include <time.h>

struct message {
  char process_name[256];
  int priority;
  int pid;
  time_t ready_time;
};

#endif // MESSAGE_H