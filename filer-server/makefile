all: network.o
	gcc server.c -o server.exe
	gcc client.c -o client.exe

client: network.o 
	gcc client.c -o client.exe

server: network.o
	gcc server.c -o server.exe

network.o:
	gcc network_functions.c -c -o network.o

clean:
	rm *.o *.exe