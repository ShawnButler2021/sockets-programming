#include "network_functions.c"

#include <stdlib.h>
#include <errno.h>
#include <signal.h>

// TODO SIGINT TO CLOSE SOCKETS

#define BUF_SIZE 64
#define PORT "5000"
#define BACKLOG 1


int host_socket, input_socket;
FILE* fptr;


void close_server(int sig);


int main() {
	// catching signals
	signal(SIGINT, close_server);

	struct socket_information results;
	struct addrinfo* sock_info;
	int errors, size;


	char file_request_buffer[BUF_SIZE];
	unsigned char file_buffer[BUF_SIZE];


	// host socket set up
	results = socket_setup(PORT);
	sock_info = results.info;
	host_socket = results.socket_fd;
	if (host_socket == -1) {
		perror("host_socket: ");
		exit(errno);
	}

	errors = bind(host_socket, sock_info->ai_addr, sock_info->ai_addrlen);
	if (errors < 0) { perror("bind socket: "); exit(errors); }
	errors = listen(host_socket, BACKLOG);
	if (errors < 0) { perror("listen socket: "); exit(errors); }


	while ( 1 ) {
		// getting new connection
		input_socket = accept_client_connection(host_socket);
		if (input_socket < 0) { perror("input socket: "); exit(input_socket); }


		// getting file request
		recv(input_socket, file_request_buffer, sizeof(file_request_buffer), 0);
		fptr = fopen(file_request_buffer, "r");
		if (fptr == NULL) {
			printf("The file has not been opened\n");
			exit(1);
		}


		// reading file
		while ( (size = fread(file_buffer, sizeof(unsigned char), BUF_SIZE, fptr)) > 0 ) {
			file_buffer[ size * sizeof(unsigned char) ] = '\0';  // trimming tail of file read
			send(input_socket, file_buffer, sizeof(file_buffer), 0);
			memset(file_buffer, '.', sizeof(unsigned char)*BUF_SIZE);
		}
		printf("Served %s\n", file_request_buffer);
		

		fclose(fptr); fptr = NULL;
		close(input_socket);
	}


	return 0;
}



void close_server(int sig) {
	if (host_socket != -1) {
		close(host_socket);
	}
	if (input_socket != -1) {
		close(input_socket);
	}
	if (fptr != NULL) {
		fclose(fptr);
	}


	printf("\nServer closing down.\n");

	exit(0);
}