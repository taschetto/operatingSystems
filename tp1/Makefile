CC = gcc
CFLAGS = -Wall -pthread -I. -std=c99
TARGET = simple dining my_dining
DEPS = gtwait.h
OBJ = gtwait.o

all: $(TARGET)

simple: simple.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

dining: dining.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

my_dining: my_dining.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -fr *.o
	rm -fr $(TARGET)
