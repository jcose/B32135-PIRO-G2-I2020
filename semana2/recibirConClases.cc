#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Buzon.h"
#include <iostream>
using namespace std;

int main( int argc, char ** argv ) {
    int id, st, veces;
    char label[ 32 ];
    //cout << "Hola"<<endl;
    Buzon m;
   // printf("recibir con clases \n");
    while ( (st = m.Recibir( label, & veces, 2020 ) ) > 0 ) {
	//printf("while recibirConClases");
        printf( "Label: %s, status %d \n", label, st );
    }
	//printf("fuera del while \n");
    exit(0);
}

