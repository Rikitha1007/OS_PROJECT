CC = gcc
CFLAGS = -Wall -Wextra
OBJS = custom_shell.o custom_commands.o
EXEC = custom_shell

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

custom_shell.o: custom_shell.c custom_commands.h
	$(CC) $(CFLAGS) -c custom_shell.c

custom_commands.o: custom_commands.c custom_commands.h
	$(CC) $(CFLAGS) -c custom_commands.c

clean:
	rm -f *.o $(EXEC)

