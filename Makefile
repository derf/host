CFLAGS = -Wall -Wextra -pedantic -O2
prefix = /usr/local

all: bin/host

bin/%: src/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

install: bin/host
	mkdir -p $(prefix)/bin $(prefix)/share/man/man1
	cp bin/host $(prefix)/bin/host
	cp man/man1/host.1 $(prefix)/share/man/man1
	chmod 755 $(prefix)/bin/host
	chmod 644 $(prefix)/share/man/man1/host.1

uninstall:
	rm -f $(prefix)/bin/host
	rm -f $(prefix)/share/man/man1/host.1

clean:
	rm -f bin/host

.PHONY: all install uninstall clean
