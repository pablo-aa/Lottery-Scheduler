#include "logger.h"
#include "common.h"
#include "message.h"
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

  // Creating a message queue to communicate with the scheduler
  key_t key = 1234;
  int msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    perror("Error in creating message queue");
    return -1;
  }

  printf("Message queue created with id %d\n", msgid);

  // Creating read process with fork
  pid_t pid = fork();

  if(pid == 0){
    // Reading processes.txt file
    FILE *file = fopen("processes.txt", "r");

    // Read each line every 2 seconds and add to message queue
    char line[256];
    while (fgets(line, sizeof(line), file)) {
      // Remove newline character
      line[strcspn(line, "\n")] = 0;

      // split line into process name and priority
      char *process_name = strtok(line, " ");
      char *priority = strtok(NULL, " ");

      // Create message struct
      struct message message;
      message.type = 0;
      strcpy(message.process_name, process_name);
      message.priority = atoi(priority);

      printf("Sending message to queue: %s %d\n", message.process_name, message.priority);

      // Send message to the queue
      msgsnd(msgid, &message, sizeof(struct message) - sizeof(long), 0);

      // Wait 2 seconds
      sleep(2);
    }

    // Send end of message to the queue
    printf("End process\n");
    struct message end_message;
    end_message.type = 1;
    end_message.priority = 0;
    strcpy(end_message.process_name, "end");
    msgsnd(msgid, &end_message, sizeof(struct message) - sizeof(long), 0);
    exit(0);
  }
  
  // Creating execl process for scheduler
  execl("./scheduler", "scheduler", NULL);

  return 0;
}