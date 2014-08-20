CC := gcc
CFLAGS := -Wall -g
SRC := $(wildcard *.c)
TARGET := $(SRC:%.c=bin/%)

all: | bin $(TARGET)

bin/% : %.c
	$(CC) $(CFLAGS) $< -o $@ 

bin:
	mkdir bin

clean:
	rm -fr bin/
