#include "network_functions.c"

#include <stdlib.h>

// TODO SIGINT TO CLOSE SOCKETS

#define BUF_SIZE 64
#define PORT "5000"
#define BACKLOG 3


int main() {
	struct socket_information results;
	struct addrinfo* sock_info;
	int host_socket, input_socket;


	char file_request_buffer[BUF_SIZE];
	char file_buffer[BUF_SIZE * sizeof(char)];


	// host socket set up
	results = socket_setup(PORT);
	sock_info = results.info;
	host_socket = results.socket_fd;
	bind(host_socket, sock_info->ai_addr, sock_info->ai_addrlen);
	listen(host_socket, BACKLOG);


	// file work
	FILE* fptr;
	int size;



	while ( 1 ) {
		input_socket = accept_client_connection(host_socket);


		// getting file request
		recv(input_socket, file_request_buffer, sizeof(file_request_buffer), 0);
		fptr = fopen(file_request_buffer, "r");
		if (fptr == NULL) {
			printf("The file has not been opened\n");
			exit(1);
		}


		// reading file
		while ( (size = fread(file_buffer, sizeof(char), BUF_SIZE, fptr)) > 0 ) {
			file_buffer[ size * sizeof(char) ] = '\0';  // trimming tail of file read
			send(input_socket, file_buffer, sizeof(file_buffer), 0);
			memset(file_buffer, '.', sizeof(char)*BUF_SIZE);
		}
		



		break;

	}


	fclose(fptr);
	close(host_socket);
	close(input_socket);

	return 0;
}