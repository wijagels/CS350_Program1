# Taylor Foxhall
# William Jagels
# Together we are: Sarude Dandstorm
# Please grade whilst the following plays in the background:
# https://www.youtube.com/watch?v=OOkKX841wsw
CFLAGS=-g -Wall -Wextra -pedantic -std=c++11 -DNDEBUG
SRCEXT=cpp
HEADEREXT=hpp
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
	$(CXX) $(LDFLAGS) $^ -o $@

%.o:%.$(SRCEXT)
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN)

.PHONY: clean
