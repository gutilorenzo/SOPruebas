/*
 * puntero.c
 *
 *  Created on: 17/7/2017
 *      Author: utnso
 */

#include <stdio.h>
#include <stdlib.h>



int main(){
	int* puntero = malloc(sizeof(int));

	printf("Se guardo memoria en el espacio: %p\n",&puntero);

	printf("Tiene el valor de: %d\n",puntero);

	//Le asignamos un nuevo valor
	*puntero = 25;

	printf("Ahora tiene el valor de: %d\n", *puntero);

	//int prueba = socket(PF_INET,SOCK_STREAM,0);

	//printf("Valor del Socket: %s\n",prueba);



	return 0;

}
