CC = gcc
CFLAGS = -Wall -std=c99
TARGETS = server client

all: $(TARGETS)
%: %.c
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean
clean:
	-rm -rf $(TARGETS)
