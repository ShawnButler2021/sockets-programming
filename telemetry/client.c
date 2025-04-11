#include "network_functions.c"


#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>


#define IP "127.0.0.1"
#define PORT "5000"
#define BUF_SIZE 64
#define NUM_OF_SOCKETS 1


struct telemetryData {
	unsigned long timestamp;
	float temperature;
	float voltage;
};

struct telemetryData generate_packet(int verbose);
void close_sockets(int sig);

int main() {
	srand(time(NULL));
	resize_num_of_sockets(1);
	signal(SIGINT, close_sockets);


	
	struct addrinfo* sock_info;
	int server_socket;
	sockets[0] = &server_socket;
	{
		struct socket_information results = socket_setup(IP, PORT);
		sock_info = results.info;
		server_socket = results.socket_fd;

		if (server_socket == -1) { perror("server_socket: "); exit(errno); }
	}


	struct telemetryData temp;
	while ( 1 ) {
		temp = generate_packet(0);

		int bytes = sendto(server_socket, &temp, sizeof(temp), 0, sock_info->ai_addr, sock_info->ai_addrlen);

		// waiting
		#ifdef _WIN32
		Sleep(500);
		#else
		sleep(0.5);
		#endif
	}



	return 0;
}



struct telemetryData generate_packet(int verbose) {
	struct telemetryData packet;


	packet.timestamp = (unsigned long) time(NULL);
	packet.temperature = 40.0 + (rand()%50) / 100.0;
	packet.voltage = 3.7 + (rand() % 50) / 100.0;

	if (verbose) {
		printf("Packet\n");
		printf("Time: %ld\n", packet.timestamp);
		printf("Temperature: %f\n", packet.temperature);
		printf("Volts: %f\n\n", packet.voltage);
	}


	return packet;
}
void close_sockets(int sig) {
	for (int i=0; i<NUM_OF_SOCKETS; ++i) {
		if ( *sockets[i] != -1) { close( *sockets[i] ); printf("\nSocket %i closed\n", i);}
	}
	exit(sig);
}