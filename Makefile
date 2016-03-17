# Taylor Foxhall
# Willaim Jagels

CC=gcc
CFLAGS=-c -g -Wall -Wextra -pedantic -std=c11
LDFLAGS=
SOURCES=lab4.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab4
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

ccred:=$(shell echo "\033[0;31m")
ccgreen:=$(shell echo "\033[0;92m")
ccyellow:=$(echo -e "\033[0;33m")
ccend:=$(shell echo "\033[0m")


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN)
