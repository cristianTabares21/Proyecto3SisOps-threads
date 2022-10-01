#include <stdio.h>	//printf
#include <stdlib.h>
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int main(int argc, char *argv[]) {
	int sock;
	struct sockaddr_in server;
	char message[1000], server_reply[18];  // Donde se va a almacenar el mensaje y la respuesta del servidor.
	
	//Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Connected");

	while(1){
		puts("\nPuede enviar su peticion de estas dos formas:");
		puts("1. Enviando el comando que quiere ejecutar");
		puts("2. Enviando el tipo de comando que quiere y el nombre del contenedor si lo requiere.\n");
		// Se imprimen unos mensajes 

		puts("Enter message:");
		fflush(stdin);
		if(fgets(message, 1000, stdin)){
			message[strcspn(message, "\n")] = 0;  // Se lee el mensaje que digito el usuario, y le quita el salto de linea al comando recibido
		};

		
		//Send some data
		if (send(sock, message, strlen(message)+1, 0) < 0) {
			puts("Send failed");
			return 1;
		}
		//Receive a reply from the server
		memset(server_reply, 0, 18);
		if (recv(sock, server_reply, 18, 0) < 0) {  // Se recibe la respuesta del servidor.
			puts("recv failed");
			//break;
		}
		printf("Respuesta servidor: %s\n", server_reply);  // Se imprime la respuesta del servidor.
	}
	
	close(sock);
	return 0;
}