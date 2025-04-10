#include <stdio.h>
#include <unistd.h>

#include "network_functions.c"

#define BUF_SIZE 4096
#define PROXY_PORT "3303"


int main(int argc, char const* argv[]) {
	int host_socket;
	struct addrinfo* host_info;
	

	host_info = get_sock_info(PROXY_PORT);
	host_socket = socket(
		host_info->ai_family,
		host_info->ai_socktype,
		host_info->ai_protocol
	);


	connect(host_socket, host_info->ai_addr, host_info->ai_addrlen);

	char buffer[BUF_SIZE];
	snprintf(buffer, sizeof(buffer),
		"GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", "127.0.0.1");
	printf("%s", buffer);
	send(host_socket, buffer, sizeof(buffer), 0);


	int received;
	while ( (received=recv(host_socket, buffer, sizeof(buffer), 0)) > 0 ) {
		buffer[received] = '\0';
		printf("%s", buffer);
	}



	printf("\n");
	freeaddrinfo(host_info);
	close(host_socket);
	return 0;
}


