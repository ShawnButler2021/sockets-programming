#include "network_functions.c"
#include <stdlib.h>
#include <errno.h>
#include <signal.h>


#define IP "127.0.0.1"
#define PORT "5000"
#define FILENAME "client_data.csv"
#define BUF_SIZE 64


void *get_in_addr(struct sockaddr *sa);
void close_sockets(int sig);


struct telemetryData {
	unsigned long timestamp;
	float temperature;
	float voltage;
};


FILE* fptr = NULL; // global to close with SIGINT


int main() {
	resize_num_of_sockets(1);
	signal(SIGINT, close_sockets);



	struct addrinfo* sock_info;
	int host_socket;
	sockets[0] = &host_socket;
	{
		struct socket_information results = socket_setup(IP, PORT);
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


	// file set up
	{
		fptr = fopen(FILENAME, "a");
		while (fptr == NULL) {
			printf("File error: %d\n", errno);

		}
	}


	int bytes;
	struct sockaddr_storage other_addr;
	socklen_t addr_len = sizeof(other_addr);
	int counter = 1;
	struct telemetryData data;

	while ( 1 ) {
		bytes = recvfrom( host_socket, &data, sizeof(data), 
			0, get_in_addr((struct sockaddr*)&other_addr), &addr_len);

		// printing information
		{
			printf("Packet %i - %i\n", counter, bytes);
			printf("Time: %ld\n", data.timestamp);
			printf("Temperature: %f\n", data.temperature);
			printf("Volts: %f\n\n", data.voltage);
		}

		// writing to file
		{
			char string[BUF_SIZE];

			sprintf(string, "%i,%li,%f,%f", 
				counter, data.timestamp, data.temperature, data.voltage);

			int flag = fprintf(fptr, "%s\n", string);
			if (flag <= 0) { printf("Error writing: %d\n", flag); }
		}

		counter++;
	}
	

	return 0;
}



void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void close_sockets(int sig) {
	for (int i=0; i<NUM_OF_SOCKETS; ++i) {
		if ( *sockets[i] != -1) { close( *sockets[i] ); printf("\nSocket %i closed\n", i);}
	}
	fclose(fptr);
	exit(sig);
}