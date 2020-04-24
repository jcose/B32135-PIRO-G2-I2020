#include "Semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

Semaphore::Semaphore(int ValorIn){
    id = semget(KEY, ValorIn, 0666 | IPC_CREAT);
    if(id < 0)
    {
        errno = 0;
        fprintf(stderr, "Unable to obtain semaphore. errno:%d\n", errno);
        exit(0);
    }
}

Semaphore::~Semaphore(){
    if((semctl(id, 0, GETNCNT)) > 1) //GETNCNT return the number of processes waiting for the value of this semaphore
    {
        if((semctl(id, 0, IPC_RMID)) < 0)//Removes the semaphores
        {
            perror("semctl");
        }
    }
}

int Semaphore::Signal(){ //increments (unlocks) the semaphore pointed to by sem.
    int retval;
    /* Set up the sembuf structure. */
    sbuf[0].sem_num = 0;    // Which semaphore in the semaphore array : */
    sbuf[0].sem_op = 1;     // Which operation? Add 1 to semaphore value : */
    sbuf[0].sem_flg = 0;    // Set the flag so we will wait : */
    /* So do the operation! */
    retval = semop(id, sbuf, 1);

    if(retval != 0)
    {
        printf("sema: V-operation did not succeed.\n");
        perror("REASON");
    }
    return retval;
}

int Semaphore::Wait(){ //decrements (locks) the semaphore pointed to by sem
    int retval;
    /* Set up the sembuf structure. */
    sbuf[0].sem_num = 0;    // Which semaphore in the semaphore array : */
    sbuf[0].sem_op = -1;     // Which operation? Subtract 1 to semaphore value : */
    sbuf[0].sem_flg = 0;    // Set the flag so we will wait : */
    /* So do the operation! */
    retval = semop(id, sbuf, 1);

    if(retval != 0)
    {
        printf("sema: P-operation did not succeed.\n");
        perror("REASON");
    }
    return retval;
}
