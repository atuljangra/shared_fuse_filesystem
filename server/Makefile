CC=g++ -g
CFLAGS=-c -Wall -std=c++11 -DFUSE_USE_VERSION=26 $(shell pkg-config fuse --cflags)
LDFLAGS=-pthread $(shell pkg-config fuse --libs)
OBJECTS=$(SOURCES:.cpp=.o)
	SOURCES= $(wildcard *.cpp) 

EXECUTABLE=serverExecutable
.PHONY: all
all: $(SOURCES) $(EXECUTABLE)
			
$(EXECUTABLE): $(OBJECTS) 
			$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
			$(CC) $(CFLAGS) $< -o $@

.PHONY: run
run:
		./$(EXECUTABLE)

.PHONY: clean
clean:
		rm -f $(EXECUTABLE) $(OBJECTS)

gdb:
		gdb $(EXECUTABLE) 


