CC := gcc
CFLAGS := -Wall -g
SRC := $(wildcard *.c)
TARGET := $(addprefix bin/, $(SRC:.c=))

all: | bin $(TARGET)

bin/% : %.c
	$(CC) $(CFLAGS) $< -o $@ 

bin:
	mkdir bin

clean:
	rm -fr bin/
