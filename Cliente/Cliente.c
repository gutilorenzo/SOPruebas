/*
 * Cliente.c
 *
 *  Created on: 11 abr. 2018
 *      Author: lorenzo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void){

	struct sockaddr_in direccionServidor;
	char* buffer = malloc(1000);
	char mensaje[256];

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(8080);

	int conexion_servidor = socket(AF_INET, SOCK_STREAM,0);

	if(connect(conexion_servidor, (void*)&direccionServidor, sizeof(direccionServidor)) != 0){
		perror("No se pudo conectar con el servidor");
		return 1;
	}

	recv(conexion_servidor,mensaje,sizeof(mensaje),0);

	printf("Llego del Servidor: %s", mensaje);

/*	int bytesRecibidos = recv(cliente, buffer, 1000, 0);

	if(bytesRecibidos <= 0){

		perror("No se recibio mensaje");
		return 1;
		}


	printf("Me llegaron %d bytes con %s", bytesRecibidos, buffer);
*/

	while(1){

		//printf("Mensaje: ");
		char mensaje[1000];

		scanf("%s", mensaje);

		send(conexion_servidor, mensaje, strlen(mensaje),0);

		int bytesRecibidos = recv(conexion_servidor,buffer,1000,0);
		if(bytesRecibidos <= 0){
			perror("El chabon se deconecto o bla");
			return 1;

		}
		buffer[bytesRecibidos] = '\0';
		printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer);

	}
	close(conexion_servidor);
	free(buffer);
	return 0;
}
