// network includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <stdio.h>
#include <string.h> // memset

#include "network_functions.c"


#define BUF_SIZE 4096
#define PORT_PORT "3303"


int main(int argc, char const* argv[]) {
	int host_socket, input_socket;
	struct addrinfo* host_info;
	struct sockaddr_storage input_addr;
	socklen_t addr_size;


	host_info = get_sock_info(PORT_PORT);
	host_socket = socket(
		host_info->ai_family,
		host_info->ai_socktype,
		host_info->ai_protocol
	);


	bind(host_socket, host_info->ai_addr, host_info->ai_addrlen);
	listen(host_socket, 1);

	addr_size = sizeof(input_addr);
	input_socket = accept(
		host_socket,
		(struct sockaddr*)&input_addr, 
		&addr_size
	);

	char recv_buffer[BUF_SIZE];
	recv(input_socket, recv_buffer, BUF_SIZE, 0);

	printf("%s", recv_buffer);




	close(input_socket);
	close(host_socket);
	freeaddrinfo(host_info);
	return 0;
}



