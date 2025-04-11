// network includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>



#include <stdio.h> // printf
#include <string.h> // memset
#include <unistd.h> // close
#include <stdlib.h> // malloc

int NUM_OF_SOCKETS = 0;
int **sockets;

struct socket_information {
	struct addrinfo* info;
	int socket_fd;
};


struct addrinfo* get_sock_info(char *ip, char port[5]) {
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	int addr_error = getaddrinfo(ip, port, &hints, &res);
	if (addr_error != 0) {
		const char *error_string;
		error_string = gai_strerror(addr_error);
		printf("%s", error_string);
	}

	return res;
}
struct socket_information socket_setup(char *ip, char port[5]) {
	struct socket_information si;

	si.info = get_sock_info(ip, port);
	si.socket_fd = socket(
		si.info->ai_family,
		si.info->ai_socktype,
		si.info->ai_protocol
	);

	return si;
}
int accept_client_connection(int host_socket) {
	struct sockaddr_storage addr;
	socklen_t size;

	return accept(
		host_socket,
		(struct sockaddr*) &addr,
		&size
	);
}
void resize_num_of_sockets(int amount) {
	NUM_OF_SOCKETS = amount;
	sockets = (int **)malloc(amount * sizeof(int));
}
