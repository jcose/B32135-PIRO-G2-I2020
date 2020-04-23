#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "Semaphore.h"

int main() {
    Semaphore s;
    //char * texto = NULL;
    int i;
    pid_t pid;  // process id is used to check if process was correctly created

    if ( (pid = fork()) >= 0 ) {    //fork successfully created
        if(pid > 0){    // parent process
            for (int i = 0; i < 10; i++ )
                printf( "Esperando para activar el semaforo %d\n", i);
            //     printf( "%s\n", texto );
            scanf( "%i", &i );
            s.Signal();
        }
        else {  //child process
            printf( "Esperando que el semaforo se active  ...\n" );
            s.Wait();
            printf( "Semaforo activado.\n" );
        }
    }
    else {  // fork error message
        perror("fork");
        exit(1);
    }
}
/*
   Salida esperada:
     Esperando para activar el semaforo ... (puede aparecer despues)
     Esperando que el semaforo se active
     Esperando para activar el semaforo ...
     Esperando para activar el semaforo ...
     ...
     [Debe escribir un numero para lectura por scanf ...]
     Semaforo activado
*/
