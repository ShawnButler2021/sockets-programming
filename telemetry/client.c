#include "network_functions.c"

#include <stdlib.h>
#include <errno.h>


#define BUF_SIZE 64
#define PORT "5000"



int main() {

	

	struct addrinfo* sock_info;
	int server_socket;
	{
		struct socket_information results = socket_setup(PORT);
		sock_info = results.info;
		server_socket = results.socket_fd;

		if (server_socket == -1) { perror("server_socket: "); exit(errno); }
	}



	char buffer[BUF_SIZE] = " womp";
	int bytes = sendto(server_socket, buffer, 64, 0, sock_info->ai_addr, sock_info->ai_addrlen);





	close(server_socket);

	return 0;
}




