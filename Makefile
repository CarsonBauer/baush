CC = gcc
CFLAGS = -Wall -g

TARGET = baush
SOURCES = src/main.c src/parser.c src/executor.c src/builtInCommands.c

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)
