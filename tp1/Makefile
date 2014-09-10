TARGET = simpleThreads
CC = gcc
CFLAGS = -Wall -pthread -I.
DEPS = gtwait.h
OBJ = $(TARGET:%=%.o) gtwait.o

%.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -fr *.o
	rm -fr $(TARGET)
