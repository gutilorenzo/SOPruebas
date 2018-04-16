#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main(void){

	fd_set master;	//Conjunto maestro de descriptores de fichero
	fd_set read_fds;	//Conjunto temporal de descriptores de fichero para select()
	struct sockaddr_in direccionServidor;	//Direccion de Servidor
	struct sockaddr_in direccionCliente; 	//Direccion de Cliente

	int fdmax;	//Numero maximo de descriptores de fichero

	int newFd; 		//descriptor de socket de la nueva conexion aceptada
	char buf[256];	//Buffer para los datos del cliente

	int nbytes;
	int yes = 1;
	int addrlen;
	int i,j;
	FD_ZERO(&master);	//Borra los conjuntos maestro y temporal
	FD_ZERO(&read_fds);

	char mensajeBienvenida[256] = "Te has conectado con el Servidor, envia tu mensaje\n";
	int cliente = socket(AF_INET, SOCK_STREAM,0);

	if(cliente == -1){
		perror("Error en la creacion del Socket");
		exit(1);
	}
	// obviar el mensaje "address already in use" (la dirección ya se está usando)

	if (setsockopt(cliente, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {

		perror("En uso: setsockopt");
		exit(1);
		}
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionServidor.sin_port = htons(PORT);
	memset(&(direccionServidor.sin_zero),'\0',8);

	if(bind(cliente, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0){

		perror("Error en la creacion del Bind");
		close(cliente);	//Cerramos el Socket
		exit(1);
	}

	printf("Aceptando conexiones en el puerto %d\n", PORT);

	if(listen(cliente, 10) < 0){
		perror("Error en el Listen");
		exit(1);

	}

	puts("Esperando conexiones.....");

	//Añadimos cliente al conjunto maestro
	FD_SET(cliente, &master);	// seguir la pista del descriptor de fichero mayor
	fdmax = cliente; // por ahora es éste
	        // bucle principal

	for(;;){
		read_fds = master;

		if(select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1){
			perror("Error en el Select");
			exit(1);
		}

		// explorar conexiones existentes en busca de datos que leer
		for(i = 0; i <= fdmax; i++){
			if(FD_ISSET(i, &read_fds)){ 	//Tenemos DATOS!!
				if(i == cliente){
					//Gestionar nuevas conexiones
					addrlen = sizeof(direccionCliente);
					newFd = accept(cliente, (struct sockaddr *)&direccionCliente,&addrlen);
					if(newFd == -1){
						perror("Error en el accept en la nueva conexion");

					}else{
						FD_SET(newFd, &master); // Añadimos la conexion nueva al conjunto maestro
						if(newFd > fdmax){	//Actualizar el maximo
							fdmax = newFd;
						}
						printf("Select Server: nueva conexion desde: %s, con el puerto: %d"
								" en el socket %d\n",
								inet_ntoa(direccionCliente.sin_addr), ntohs(direccionCliente.sin_port),
								newFd);

								send(newFd,mensajeBienvenida,sizeof(mensajeBienvenida),0);
					}
				}else{
						//Gestionar datos de un cliente
					if((nbytes = recv(i,buf,sizeof(buf),0)) <= 0){
							//Error o Conexion cerrada por el cliente
						if(nbytes == 0){
								// CONEXION CERRADA
							printf("Select Server: se ha desconectado el socket: %d \n",i);

						}else{
							perror("Error en el recv");

						}
						close(i);
						FD_CLR(i, &master);		// Eliminar del conjunto maestro

					}else{

						//Tenemos datos de algun cliente

						for(j = 0; j <= fdmax; j++){
								//Enviar a todo el mundo

							if(FD_ISSET(j, &master)){
									//Excepto al Cliente y nosotros mismos
								if(j != cliente && j != i){
									if(send(j,buf,nbytes,0) == -1){
									perror("Error en send");
									}
								}
							}
						}
					}
				}
			}
		}
	}
return 0;
}

