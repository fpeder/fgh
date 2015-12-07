CC = gcc
CFLAGS = -Wall -O2

all: gh p

gh: guohall.c rw.c rw.h
	$(CC) $(CFLAGS) -o $@ $^

p: pede_guohall.c rw.c rw.h
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: clean
clean:
	rm gh p
