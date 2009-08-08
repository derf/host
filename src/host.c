/*
 * Copyright © 2009 by Daniel Friesel <derf@derf.homelinux.org>
 * License: WTFPL <http://sam.zoy.org/wtfpl>
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv) {
	struct addrinfo hints = { 0, AF_UNSPEC, SOCK_DGRAM, 0, 0, NULL, NULL, NULL };
	struct addrinfo *result, *address;
	char hostname[NI_MAXHOST];
	char ip_address[INET6_ADDRSTRLEN];
	int ret;
	struct in_addr addr;
	void *ptr;

	ret = getaddrinfo(argv[1], NULL, &hints, &result);
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		exit(EXIT_FAILURE);
	}

	for (address = result; address != NULL; address = address->ai_next) {
		switch (address->ai_family) {
			case AF_INET:
				ptr = &((struct sockaddr_in *) address->ai_addr)->sin_addr;
				break;
			case AF_INET6:
				ptr = &((struct sockaddr_in6 *) address->ai_addr)->sin6_addr;
				break;
			default:
				fprintf(stderr, "Unknown address family: %u\n", address->ai_family);
				goto loop;
		}
		if (inet_ntop(address->ai_family, ptr, ip_address, sizeof(ip_address)) == NULL) {
			perror("inet_ntop");
			continue;
		}
		puts(ip_address);
		ret = getnameinfo(address->ai_addr, address->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0);
		if (ret != 0) {
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(ret));
			continue;
		}
		puts(hostname);
loop:;
	}
	freeaddrinfo(address);
	return(EXIT_SUCCESS);
}
