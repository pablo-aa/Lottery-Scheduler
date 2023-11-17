// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

struct message {
  char process_name[256];
  int priority;
  int pid; // default 0 but could use to send other types of messages like end of message
};

#endif // MESSAGE_H