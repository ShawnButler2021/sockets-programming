
all: network.o
	gcc proxy.c -o proxy.exe
	gcc client.c -o client.exe

network.o:
	gcc network_functions.c -c -o network.o

proxy: network.o
	gcc proxy.c -o proxy.exe

client: network.o
	gcc client.c -o client.exe



make clean:
	rm *.exe
