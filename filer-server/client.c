#include <stdlib.h>


#include "network_functions.c"


#define BUF_SIZE 64
#define PORT "5000"


int main(int argc, char *argv[]) {
	if (argc < 2) { printf("too few arguments\n"); exit(1); }


	struct socket_information results;
	struct addrinfo* sock_info;
	int server_socket;


	// setting up socket
	results = socket_setup(PORT);
	sock_info = results.info;
	server_socket = results.socket_fd;
	connect(server_socket, sock_info->ai_addr, sock_info->ai_addrlen);


	// setting up buffer variables
	char request_buffer[BUF_SIZE];
	unsigned char server_buffer[BUF_SIZE];
	int bytes, flag;

	// requesting file
	FILE* fptr;
	fptr = fopen(argv[1], "wb");
	if (fptr == NULL) {
		printf("The file was not opened\n");
		exit(1);
	}
	strcpy(request_buffer, "./work/white.jpeg");
	send(server_socket, request_buffer, sizeof(request_buffer), 0);


	// getting file data
	while ( (bytes = recv(server_socket, server_buffer, sizeof(server_buffer), 0)) > 0 ) {
		server_buffer[bytes] = '\0';  // trimming tail of file read
		

		flag = fwrite(server_buffer, sizeof(unsigned char), BUF_SIZE, fptr);
		if (!flag) {
			printf("Error writing\n");
			exit(1);
		}

		memset(server_buffer, '.', sizeof(unsigned char)*BUF_SIZE);
	}


	printf("\n");
	close(server_socket);
	fclose(fptr);
	return 0;
}