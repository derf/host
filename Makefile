CFLAGS = -Wall -Wextra -pedantic -O2
prefix = /usr/local

all: bin/host

bin/%: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

install: bin/host
	mkdir -p $(prefix)/bin
	cp bin/host $(prefix)/bin/host
	chmod 755 $(prefix)/bin/host

uninstall:
	rm -f $(prefix)/bin/host

clean:
	rm -f bin/host

.PHONY: all install uninstall clean
