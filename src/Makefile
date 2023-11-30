CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -fsanitize=address

TARGET = main
DESTINATION = ../build/

all: $(TARGET)

$(TARGET): $(TARGET).o pong.o
	$(CC) $(CFLAGS) -lncurses $(DESTINATION)$(TARGET).o $(DESTINATION)pong.o -o $(DESTINATION)$(TARGET)

$(TARGET).o: $(TARGET).c 
	$(CC) $(CFLAGS) -c $(TARGET).c -o $(DESTINATION)$(TARGET).o

pong.o: pong.c pong.h 
	$(CC) $(CFLAGS) -c pong.c -o $(DESTINATION)pong.o

clean:
	rm -rf *.o $(DESTINATION)*

rebuild: clean all

start: $(TARGET)
	$(DESTINATION)$(TARGET)

restart: rebuild start

cpp: 
	cppcheck --enable=all --suppress=missingIncludeSystem .

clang:
	clang-format -i *.c

.PHONY: all clean rebuild cpp clang start restart