#include<stdio.h>
#include<stdlib.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h>
#include<sys/wait.h>

char *finalCommand[100];  // Variable global que va a almacenar el comando que digita el usuario para que sea usado por los hilos

void cadenaTokens(char commands[], char *salida[]){  // Funcion que se encarga de separar el comando por espacios
    char *puntero;
    int i = 0;
    puntero = strtok(commands, " ");  // Se separa el comando por espacios

    while(puntero != NULL){
        salida[i] = puntero;  // Se almacena el token en el nuevo arreglo
        puntero = strtok(NULL, " ");  // Se recorre todo el comando
        i++;  // Se aumenta la posicion
    }
	salida[i] = NULL;
}

void *createContainer(void *params){  // Funcion del hilo que crea los contenedores
	char *args[] = {"sudo", "docker", "run", "-di", "--name", finalCommand[1], finalCommand[2], NULL};  // Una lista con un comando default para crear contenedores
	pid_t pid;
	pid = fork();  // Se crea un proceso para poder ejecutar el execvp

	if(pid == 0){ // Proceso hijo
		if(execvp(args[0], args) < 0){  // Se ejecuta el execvp con los argumentos pasados
			puts("Error.");
		}
	}
	else{
		wait(NULL);  // El padre espera al hijo
	}
}

void *listContainer(void *params){  // Funcion del hilo que lista los contenedores
	char *args[] = {"sudo", "docker", "ps", "-a", NULL}; // Una lista con un comando default para listar los contenedores.
	pid_t pid;
	pid = fork();  // Se crea un proceso para poder ejecutar el execvp

	if(pid == 0){  // Proceso hijo
		if(execvp(args[0], args) < 0){
			puts("Error.");
		}
	}
	else{
		wait(NULL);  // El padre espera al hijo
	}
}

void *stopContainer(void *params){  // Funcion del hilo que detiene los contenedores
	char *args[] = {"sudo", "docker", "stop", finalCommand[1], NULL};  // Detiene un contenedor dado su nombre
	pid_t pid;
	pid = fork();  // Se crea el proceso

	if(pid == 0){ // Proceso hijo
		if(execvp(args[0], args) < 0){
			puts("Error.");
		}
	}
	else{ 
		wait(NULL);  // El padre espera al hijo
	}
}

void *deleteContainer(void *params){  // Funcion del hilo que elimina los contenedores
	char *args[] = {"sudo", "docker", "rm", finalCommand[1], NULL};  // Elimina un contenedor dado su nombre
	pid_t pid;
	pid = fork();  // Se crea el proceso

	if(pid == 0){  // Proceso hijo
		if(execvp(args[0], args) < 0){
			puts("Error.");
		}
	}
	else{
		wait(NULL);  // El padre espera al hijo
	}
}


void *executeCommandBasic(void *params){  // Funcion que ejecuta un comando digitado por el usuario
	pid_t pid;
	pid = fork();  // Se crea el proceso

	if(pid == 0){  // Proceso hijo
		if(execvp(finalCommand[0], finalCommand) < 0){  // Se ejecuta el comando que digito el usuario
			puts("Error.");
		}
	}
	else{
		wait(NULL);  // El padre espera al hijo
	}
}

int main(int argc , char *argv[]) {
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;  // https://github.com/torvalds/linux/blob/master/tools/include/uapi/linux/in.h
	char client_message[2000];  // Mensaje del cliente
	char *listCommands[100];  // Funcion que contendra el comando separado por espacios
	pthread_t tid1, tid2, tid3, tid4, tid5;  // Creacion de 5 identificadores para 5 hilos
	pthread_attr_t attr;  // Atributos para los hilos
	
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	
	//Bind the socket to the address and port number specified
	if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(client_sock < 0) {
		perror("accept failed");
	}
	puts("Connection accepted");

	pthread_attr_init(&attr);  // inicializacion de los atributos por defecto para los hilos

	while(1) {
		//accept connection from an incoming client
		memset(client_message, 0, 2000);
		int typeCommand = 0;  // Variable para saber que hilo llamar
		//Receive a message from client
		if(recv(client_sock, client_message, 2000, 0) < 0) {
			printf("Error al recibir la peticion.\n");
			//Send the message back to client
			send(client_sock, "Error.", 6, 0);
			return 1;
		}
		cadenaTokens(client_message, listCommands);
		if(strcmp("create", listCommands[0]) == 0){  // Para crear un contenedor por default.
			typeCommand = 1;
			memcpy(finalCommand, listCommands, 100);  // Se pasa el comando a la variable global para ser usada por los hilos
		}
		else if(strcmp("list", listCommands[0]) == 0){  // Para listar contenedores.
			typeCommand = 2;
			memcpy(finalCommand, listCommands, 100);
		}
		else if(strcmp("stop", listCommands[0]) == 0){  // Para detener un contenedor por default.
			typeCommand = 3;
			memcpy(finalCommand, listCommands, 100);
		}
		else if(strcmp("delete", listCommands[0]) == 0){  // Para eliminar un contenedor por default.
			typeCommand = 4;
			memcpy(finalCommand, listCommands, 100);
		}
		else if(strcmp("sudo", listCommands[0]) == 0){  // Para ejecutar un comando digitado por el usuario.
			typeCommand = 5;
			memcpy(finalCommand, listCommands, 100);
		}

		switch (typeCommand)  // Un switch que escoge que hilo llamar dependiendo una entrada
		{
		case 1: // Hilo crear
			pthread_create(&tid1, &attr, createContainer, NULL);
			pthread_join(tid1, NULL);
			break;
		
		case 2: // Hilo listar
			pthread_create(&tid2, &attr, listContainer, NULL);
			pthread_join(tid2, NULL);
			break;
		
		case 3:  // Hilo detener
			pthread_create(&tid3, &attr, stopContainer, NULL);
			pthread_join(tid3, NULL);
			break;

		case 4: // Hilo eliminar
			pthread_create(&tid4, &attr, deleteContainer, NULL);
			pthread_join(tid4, NULL);
			break;

		case 5:  // Hilo ejecutar
			pthread_create(&tid5, &attr, executeCommandBasic, NULL);
			pthread_join(tid5, NULL);
			break;
		}
		
		send(client_sock, "Peticion recibida.", 18, 0); // Se le envia un mensaje al cliente cuando se recibe una peticion.
    }
	return 0;
}