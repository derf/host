include config.mk

all: bin/host

bin/host: src/host.c
	mkdir -p bin
	${CC} ${CFLAGS} -o $@ $<

install: bin/host
	@echo installing bin to ${bin_dir} and man to ${man_dir}
	@mkdir -p ${bin_dir} ${man_dir}/man1
	@cp bin/host ${bin_dir}/host
	@cp man/1/host ${man_dir}/man1/host.1
	@chmod 755 ${bin_dir}/host
	@chmod 644 ${man_dir}/man1/host.1

uninstall:
	rm -f ${bin_dir}/host
	rm -f ${man_dir}/man1/host.1

clean:
	rm -f bin/host

.PHONY: all install uninstall clean
