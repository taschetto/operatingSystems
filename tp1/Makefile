CC = gcc
CFLAGS = -Wall -pthread -I.
TARGET = simple dining
DEPS = gtwait.h
OBJ = gtwait.o

all: $(TARGET)

simple: simple.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

dining: dining.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
