CC=gcc
CFLAGS=-Wall -I include

all: prog scheduler

prog: src/logger.c src/main.c src/process.c
	$(CC) $(CFLAGS) -o prog src/logger.c src/main.c src/process.c

scheduler: src/scheduler.c
	$(CC) $(CFLAGS) -o scheduler src/scheduler.c

clean:
	rm -f prog scheduler