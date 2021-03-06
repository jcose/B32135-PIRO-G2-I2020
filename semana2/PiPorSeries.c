/*
  Calcula el numero PI utilizando una serie "infinita"
  Debe recibir la cantidad de iteraciones como parametro
  pi = (-1)^i x 4/(2xi + 1)
 * Semana 02, deben crear dos programas:
 *    Agregar Semaforos y un segmento de memoria compartida para lograr la solución correcta
 *    Agregar Paso de mensajes para lograr la solución correcta
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>


/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/
double calcularSumaParcialPi( double *valorDeRetorno, int proceso, long inicial, long terminos) {
    double casiPi = 0;
    double alterna = 4;
    long divisor = 0;
    long termino;

    for ( termino = inicial; termino < terminos; termino++ ) {
        divisor = termino + termino + 1;		// 2 x termino + 1
        casiPi += alterna/divisor;		// 4 / (2xi + 1)
        alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
    }
    *valorDeRetorno += casiPi;			// Guarda el resultado en el vector y finaliza
    //printf("Devuelto: \033[91m %g \033[0m. Calculado: %.15f\n", *valorDeRetorno, casiPi);
    exit( 0 );
}


int main( int argc, char ** argv ) {
    long terminos, inicio, fin;
    int proceso;
    int pid;
    int id;
    //double casiPi = 0;

    terminos = 1000000;
    if ( argc > 1 ) {
        terminos = atol( argv[ 1 ] );
    }

    id = shmget( 32135, sizeof(double), 0700 | IPC_CREAT );

    if(id < 0){
        perror("shmget");
                exit(1);
    }

    double *suma = (double*) shmat( id, NULL, 0 );//area de memoria compartida

    if(*suma < 0){
        perror("shmat");
                exit(1);
    }

    for ( proceso = 0; proceso < 10; proceso++ ) {
        inicio = proceso * terminos/10;
        fin = (proceso + 1) * terminos/10;
        pid = fork();
        if ( ! pid ) {
            calcularSumaParcialPi( suma, proceso, inicio, fin);
        } else {
            printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
        }
    }

    for ( proceso = 0; proceso < 10; proceso++ ) {
        int status;
        pid_t pid = wait( &status );

        if(pid < 0){
            perror("wait");
                    exit(1);
        }
    }

    /*for ( proceso = 1; proceso < 10; proceso++ ) {
      casiPi[ 0 ] += casiPi[ proceso ];
   }*/

    printf( "Valor calculado de Pi es \033[91m %g \033[0m con %ld terminos\n", *suma, terminos );
    shmdt( suma );
    shmctl( id, IPC_RMID, NULL );
}
