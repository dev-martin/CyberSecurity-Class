CFILES := $(wildcard *.c)
PROGS := $(patsubst %.c,%,$(CFILES))
CFLAGS := -ggdb -static -fno-stack-protector -z execstack -Wformat=0
CC := gcc

all: $(PROGS)
	@echo $(PROGS)

clean:
	rm -f $(PROGS)

stack: stack.c
	sudo $(CC) -o $@ $(CFLAGS) $<
	sudo chmod 4755 stack

