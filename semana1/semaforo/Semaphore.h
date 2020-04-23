/*
   C++ class to encapsulate Unix semaphore intrinsic structures and
   system calls
*/

#define KEY 0xA12345	// Valor de la llave del recurso
#include <sys/sem.h>

class Semaphore {
public:
    Semaphore(int ValorIn = 0 );
    ~Semaphore();
    int Signal();	// Puede llamarse V
    int Wait();         // Puede llamarse P
private:
    int id;     // Identificador del semaforo
    sembuf sbuf[1]; //is a structure that specifies an operation to be performed on a single semaphore.
};
