CC=gcc
CFLAGS=-Wall -I include

all: prog scheduler busy_wait

prog: src/logger.c src/main.c src/process.c
	$(CC) $(CFLAGS) -o prog src/logger.c src/main.c src/process.c

scheduler: src/scheduler.c src/process.c
	$(CC) $(CFLAGS) -o scheduler src/scheduler.c src/process.c

busy_wait: src/busy_wait.c
	$(CC) $(CFLAGS) -o busy_wait src/busy_wait.c

clean:
	rm -f prog scheduler