# Taylor Foxhall
# William Jagels

CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic -std=gnu99
SRCEXT=c
HEADEREXT=h
LDFLAGS=
SOURCES=$(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS=$(patsubst %.$(SRCEXT),%.o,$(SOURCES))
HEADERS=$(shell find . -type f -name "*.$(HEADEREXT)")
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
	$(CC) $(LDFLAGS) $^ -o $@

%.o:%.$(SRCEXT)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN)

.PHONY: clean
