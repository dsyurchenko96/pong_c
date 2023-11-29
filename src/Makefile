CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = main
DESTINATION = ../build/

all: $(TARGET)

$(TARGET): $(TARGET).o pong.o
	$(CC) $(CFLAGS) -lncurses $(TARGET).o pong.o -o $(DESTINATION)$(TARGET)

$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c

pong.o: pong.c pong.h 
	$(CC) $(CFLAGS) -c pong.c

clean:
	rm -rf *.o $(DESTINATION)*

rebuild: clean all

cpp: 
	cppcheck --enable=all --suppress=missingIncludeSystem .

clang:
	clang-format -i *.c

.PHONY: all clean rebuild cpp build clang