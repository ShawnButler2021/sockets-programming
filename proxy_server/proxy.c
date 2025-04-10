// network includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <stdio.h>
#include <unistd.h>
#include <string.h> // memset

#include "network_functions.c"


#define BUF_SIZE 4096
#define PROXY_PORT "3303"


int main(int argc, char const* argv[]) {
	int host_socket, input_socket;
	int server_socket;


	// setting up host socket
	struct addrinfo* host_info;
	struct sockaddr_storage input_addr;
	socklen_t addr_size;
	host_info = get_sock_info(PROXY_PORT);
	host_socket = socket(
		host_info->ai_family,
		host_info->ai_socktype,
		host_info->ai_protocol
	);
	bind(host_socket, host_info->ai_addr, host_info->ai_addrlen);
	listen(host_socket, 1);

	// setting up input socket from 
	// host socket
	addr_size = sizeof(input_addr);
	input_socket = accept(
		host_socket,
		(struct sockaddr*)&input_addr, 
		&addr_size
	);


	// setting up connection to server
	struct addrinfo* server_info;
	server_info = get_sock_info("5000");	// change to http or https port later
	server_socket = socket(
		server_info->ai_family,
		server_info->ai_socktype,
		server_info->ai_protocol
	);
	connect(
		server_socket,
		server_info->ai_addr,
		server_info->ai_addrlen
	);





	// receiving request to pass 
	// along
	char buffer[BUF_SIZE];
	recv(input_socket, buffer, BUF_SIZE, 0);


	// forwarding request to server
	send(server_socket, buffer, sizeof(buffer), 0);


	// receiving data from the server
	int received = 1;
	int sent;
	char server_buffer[BUF_SIZE];
	while ( (received=recv(server_socket, server_buffer, sizeof(server_buffer), 0)) > 0 ) {
		server_buffer[received] = '\0';

		//printf("%s", server_buffer);

		sent = send(input_socket, server_buffer, sizeof(server_buffer), 0);
	}








	printf("\n");
	close(input_socket);
	close(host_socket);
	freeaddrinfo(host_info);
	freeaddrinfo(server_info);
	return 0;
}



