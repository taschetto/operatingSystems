CC = gcc
CFLAGS = -Wall -pthread -I. -std=c99 -D_POSIX_C_SOURCE=199309L
TARGET = simple dining
DEPS = lock.h
OBJ = lock.o

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
