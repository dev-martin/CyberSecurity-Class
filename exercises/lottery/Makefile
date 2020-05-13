CFILES := $(wildcard *.c)
PROGS := $(patsubst %.c,%,$(CFILES))
CFLAGS := -fno-stack-protector -z execstack -w -ggdb
CC := gcc

all: $(PROGS)
	@echo $(PROGS)

clean:
	rm $(PROGS)

stack: stack.c
	$(CC) -o $@ $(CFLAGS) $<
	sudo chmod 4755 stack

