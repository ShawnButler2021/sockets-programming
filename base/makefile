
all: network.o
	gcc port.c -o port.exe
	gcc client.c -o client.exe

network.o:
	gcc network_functions.c -c -o network.o

port: network.o
	gcc port.c -o port.exe

client: network.o
	gcc client.c -o client.exe



make clean:
	rm *.exe
