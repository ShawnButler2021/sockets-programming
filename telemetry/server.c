#include "network_functions.c"
#include <stdlib.h>
#include <errno.h>
#include <signal.h>


#define PORT "5000"
#define BUF_SIZE 64


void *get_in_addr(struct sockaddr *sa);

int main() {

	struct addrinfo* sock_info;
	int host_socket;
	{
		struct socket_information results = socket_setup(PORT);
		sock_info = results.info;
		host_socket = results.socket_fd;

		if (host_socket == -1) { perror("host_socket: "); exit(errno); }
	}

	// bind socket
	{
		int error = bind(host_socket, sock_info->ai_addr, sock_info->ai_addrlen);
		if (error == -1) {
			close(host_socket);
			perror("bind fail: ");
			exit(error);
		}
	}




	int bytes = 0;
	struct sockaddr_storage other_addr;
	socklen_t addr_len = sizeof(other_addr);
	char buffer[BUF_SIZE];

	while ( 1 ) {
		memset(buffer, '\0', BUF_SIZE);

		bytes = recvfrom( host_socket, buffer, BUF_SIZE-1, 
			0, get_in_addr((struct sockaddr*)&other_addr), &addr_len);
		printf("%s\n", buffer);

	}
	

	close(host_socket);
	return 0;
}



void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}