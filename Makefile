# Executable
TARGET = prog

# Compilator
CC = gcc

# Compilator flags
CFLAGS = -Wall

# Files directories
SRC_DIR = src
INC_DIR = include

# Main files list
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ $^

# clean executable
clean:
	rm -f $(TARGET)

.PHONY: clean