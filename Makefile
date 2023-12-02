# compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# source files
SRCS = main.c house.c logger.c utils.c room.c evidence.c hunter.c ghost.c
OBJS = $(SRCS:.c=.o)

# gcc -o fp main.c house.c logger.c utils.c room.c evidence.c hunter.c ghost.c -pthread
# executable
TARGET = fp

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
