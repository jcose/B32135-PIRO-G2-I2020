Implementación de la clase Semaphore

Se ha implementado la clase Semaphore.h para encapsular los llamados del sistema linux al sistema para manipular los 
semáforos. Cada método implementado utiliza diferentes llamados al sistema:
 
Semaforo: semget
~Semaforo: semctl
Signal, Wait: semop

La compilación del programa se puede hacer desde la terminal. Se agregó un Makefile por lo que solo es necesario compilar haciendo 
$ make
y luego se corre
$ ./pruebaSum
obteniendo la salida esperada. 
