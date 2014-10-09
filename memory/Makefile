CC := gcc
CFLAGS := -g
SRC := $(wildcard *.c)
TARGET := $(SRC:%.c=%)

all: $(TARGET)

% : %.c
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -fr $(TARGET)
