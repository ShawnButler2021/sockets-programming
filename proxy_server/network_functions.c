// network includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <string.h> // memset



struct addrinfo* get_sock_info(char port[5]) {
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int addr_error = getaddrinfo("127.0.0.1", port, &hints, &res);
	if (addr_error != 0) {
		const char *error_string;
		error_string = gai_strerror(addr_error);
		printf("%s", error_string);
	}

	return res;
}