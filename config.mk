CFLAGS ?= -Wall -Wextra -pedantic -O2
PREFIX ?= /usr/local

main_dir = ${DESTDIR}${PREFIX}

man_dir = ${main_dir}/share/man
bin_dir = ${main_dir}/bin
