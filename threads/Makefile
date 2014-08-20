CC := gcc
CFLAGS := -Wall -g -pthread
SRC := $(wildcard *.c)
TARGET := $(SRC:%.c=bin/%)

all: | bin $(TARGET)

bin/% : %.c
	$(CC) $(CFLAGS) $< -o $@ 

bin:
	mkdir bin

clean:
	rm -fr bin/
