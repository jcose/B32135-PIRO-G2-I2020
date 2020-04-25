#include "Buzon.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


Buzon::Buzon() {
    id = msgget( 0xB32135, IPC_CREAT | 0600 );
    if ( -1 == id ) {
        perror( "Constructor de Buzon" );
        exit( 1 );
    }
}

Buzon::~Buzon() {
    //msgctl( id, IPC_RMID, NULL );
    //printf( "destruyendo bozon %d\n", id );
}

int Buzon::Enviar( const char * mensaje, int veces, long tipo ) {// Envia la tira de caracteres como un mensaje tipo
    int size;
    int st = -1;
    if ( ( size = strlen( mensaje ) ) > 0 ) {
        my_message.mtype = tipo;
        my_message.veces = veces;
        strcpy( my_message.etiqueta, mensaje );
        st= msgsnd( id, (const void *) & my_message, sizeof( my_message ), IPC_NOWAIT );
    }
    return st;
}

int Buzon::Enviar( void * mensaje, int len, int veces, long tipo ) {
    int st = -1;
    if ( len > 0 ) {
        my_message.mtype = tipo;
        my_message.veces = veces;
        strcpy( my_message.etiqueta, (const char*)mensaje);
        st= msgsnd( id, (const void *) & my_message, sizeof( my_message ), IPC_NOWAIT );
    }
    return st;
}

int Buzon::Recibir( char * mensaje, int * veces, long tipo ) {// len es el tamaño máximo que soporte la variable mensaje
    struct msgbuf {
        long mtype;
        int veces;
        char etiqueta[ 32 ];
    } temp_message;

    int st;
    
    if((st = msgrcv(id,(void *)&temp_message,sizeof( temp_message ),tipo,IPC_NOWAIT))>0){
    	strcpy(mensaje, temp_message.etiqueta);
    }
    return st;

}

/*int Buzon::Recibir( void * mensaje, int len, long tipo ) {// mensaje puede ser de tipo msgbuf
    printf( "recibir segundo\n");

    return 0;
}*/
