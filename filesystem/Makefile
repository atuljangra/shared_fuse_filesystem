CC = g++ -g
CFLAGS = -c -Wall -std=c++11 -DFUSE_USE_VERSION=26 $(shell pkg-config fuse --cflags)
#LINKFLAGS = -Wall `pkg-config fuse --libs`
LINKFLAGS = -Wall $(shell pkg-config fuse --libs) 

EXECUTABLE = fileSystem

OBJECTS=$(SOURCES:.cpp=.o)
	SOURCES= ../Message.cpp $(wildcard *.cpp) 

.PHONY: all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
			$(CC) $(OBJECTS) -o $@ $(LINKFLAGS)

.cpp.o:
			$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
		rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY:run
run:
	mkdir -p mount 
	./$(EXECUTABLE) -f mount

umount:
	fusermount -u b
