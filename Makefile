CC = /usr/bin/gcc
CFLAGS = -Wall -std=c99
TARGETS = adm_server adm_client server client echo

all: $(TARGETS)

%.c%:
	${ CC} $^ ${CFLAGS} -o $@

%.c%.o:
	${ CC} $^ ${CFLAGS} -c $@

.PHONY: clean
clean:
	-rm -f adm_server adm_client server client
