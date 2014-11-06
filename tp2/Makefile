CC = gcc
CFLAGS = -std=c99 -pthread -Wall -I.
TARGET = main 
DEPS = io.h fs.h helpers.h shell.h
OBJ = io.o fs.o helpers.o shell.o

all: $(TARGET)

$(TARGET): $(TARGET:%=%.o) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
