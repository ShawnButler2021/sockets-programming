all: server client

server: network.o 
	gcc server.c -o server.exe

client: network.o
	gcc client.c -o client.exe

network.o:
	gcc network_functions.c -c -o network.o

clean:
	rm *.o *.exe