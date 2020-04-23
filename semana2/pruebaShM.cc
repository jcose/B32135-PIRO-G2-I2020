#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
//#include <wait.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "Semaphore.h"

int main() {

    int i;
    Semaphore s;
    int id = shmget( 32135, 1024, 0700 | IPC_CREAT );
    char * area = (char *) shmat( id, NULL, 0 );
    char * var = (char *)("Area de memoria compartida 2020");
    char newvar[100];
    pid_t pid;

    if ( (pid = fork()) > 1 ) {
        strcpy( area, var );
        printf( "Escriba un numero y <ENTER> para continuar ...\n" );
        scanf( "%i", &i );
        printf( "Variable es %s \n", area );
        s.Signal();
    }
    else if(pid == 0){
        s.Wait();
        strcpy( newvar, area );
        printf( "La variable compartida es: %s\n", newvar);
        exit( 0 );  //closes child proccess
    }
    else
    {
        perror("fork");
                exit(1);
    }
    wait(&i);
    printf( "Fin del programa ...\n");
    shmdt( area );
    shmctl( id, IPC_RMID, NULL );
}

