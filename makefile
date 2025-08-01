#CC=clang
CC=gcc
CFLAGS=-c -Wall -pedantic -std=c99
LDFLAGS= -lm
SOURCES=ReadToDigitalTool.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ReadToDigitalTool.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm ./*.o

.c.o:
	$(CC) $(CFLAGS) $< -o $@

