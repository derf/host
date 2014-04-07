/*
 * Copyright © 2009 by Daniel Friesel <derf@finalrewind.org>
 * License: WTFPL <http://sam.zoy.org/wtfpl>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PRINT_USAGE \
	fprintf(stderr, "Usage: %s [-46r] <hostname>\n", argv[0])

static inline int addr_to_ip(const struct addrinfo *address, char *ip, int length) {
	void *ptr;
	switch (address->ai_family) {
		case AF_INET:
			ptr = &((struct sockaddr_in *) address->ai_addr)->sin_addr;
			break;
		case AF_INET6:
			ptr = &((struct sockaddr_in6 *) address->ai_addr)->sin6_addr;
			break;
		default:
			fprintf(stderr, "Unknown address family: %u\n", address->ai_family);
			return 0;
	}
	if (inet_ntop(address->ai_family, ptr, ip, length) == NULL) {
		perror("inet_ntop");
		return 0;
	}
	return 1;
}

int main(int argc, char **argv) {
	struct addrinfo hints = { 0, AF_UNSPEC, SOCK_DGRAM, 0, 0, NULL, NULL, NULL };
	struct addrinfo *result, *address;
	char hostname[NI_MAXHOST];
	char ip_address[INET6_ADDRSTRLEN];
	int ret;
	int show_rdns = 0;
	enum {
		INPUT_NONE,
		INPUT_IP,
		INPUT_HOST
	} input_type = INPUT_NONE;
	signed char option;

	while ((option = getopt(argc, argv, "46r")) != -1) {
		switch (option) {
			case '4':
				hints.ai_family = AF_INET;
				break;
			case '6':
				hints.ai_family = AF_INET6;
				break;
			case 'r':
				show_rdns = 1;
				break;
			default:
				PRINT_USAGE;
				return EXIT_FAILURE;
		}
	}

	if (optind >= argc) {
		PRINT_USAGE;
		return EXIT_FAILURE;
	}

	ret = getaddrinfo(argv[optind], NULL, &hints, &result);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		return EXIT_FAILURE;
	}

	for (address = result; address != NULL; address = address->ai_next) {
		if (addr_to_ip(address, ip_address, sizeof(ip_address)) == 0)
			continue;
		if (input_type == INPUT_NONE) {
			if (strcmp(ip_address, argv[optind]) == 0)
				input_type = INPUT_IP;
			else
				input_type = INPUT_HOST;
		}
		if (input_type == INPUT_HOST)
			puts(ip_address);
		if ((input_type == INPUT_IP) || (show_rdns == 1)) {
			ret = getnameinfo(address->ai_addr, address->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
			if (ret != 0) {
				fprintf(stderr, "getnameinfo: %s\n", gai_strerror(ret));
				continue;
			}
			puts(hostname);
		}
	}
	freeaddrinfo(address);
	return(EXIT_SUCCESS);
}
