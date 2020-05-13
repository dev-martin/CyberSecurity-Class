CFILES := $(wildcard *.c)
PROGS := $(patsubst %.c,chroot/%,$(CFILES))
CFLAGS := -static
CC := gcc

all: $(PROGS)

clean:
	rm $(PROGS)

chroot/% : %.c
	$(CC) $(CFLAGS) -o $@ $<

