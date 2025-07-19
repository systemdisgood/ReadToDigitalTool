#CC=clang
CC=gcc
CFLAGS=-c -Wall -pedantic -Werror -std=c99
LDFLAGS=
SOURCES=ReadToDigitalTool.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ReadToDigitalTool.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm ./*.o

.c.o:
	$(CC) $(CFLAGS) $< -o $@

