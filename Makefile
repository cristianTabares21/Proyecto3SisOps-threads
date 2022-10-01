CC=gcc

%.o: %.c
	$(CC) -c -o $@ $<

socket_client: socket_client.o
	gcc -o socket_client socket_client.o

socket_server: socket_server.o
	gcc -o socket_server socket_server.o
	
clean:
	rm -f *.o socket_client socket_server
