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
    msgctl( id, IPC_RMID, NULL );
    printf( "destruyendo bozon %d\n", id );
}

int Buzon::Enviar( const char * mensaje, int veces, long tipo ) {// Envia la tira de caracteres como un mensaje tipo
    int size;
    int st = -1;
    if ( ( size = strlen( mensaje ) ) > 0 ) {
        my_message.mtype = tipo;
        my_message.veces = veces;
        strncpy( my_message.etiqueta, mensaje, size );
        return msgsnd( id, (const void *) & my_message, sizeof( my_message ), IPC_NOWAIT );
    }
    return st;
}

int Buzon::Enviar( void * mensaje, int len, int veces, long tipo ) {
    int st = -1;
    if ( len > 0 ) {
        my_message.mtype = tipo;
        my_message.veces = veces;
        strncpy( my_message.etiqueta, (const char*)mensaje, len );
        return msgsnd( id, (const void *) & my_message, sizeof( my_message ), IPC_NOWAIT );
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
    if ((st = msgrcv( id, (void *) & temp_message, sizeof( temp_message ), tipo, 0 ) ) > 0 ) {
        //strncpy(mensaje, temp_message.etiqueta, strlen(temp_message.etiqueta));
        strcpy(mensaje, temp_message.etiqueta);
    }
    //printf("mensaje: [%s] tamano: %lu size: %lu\n", mensaje, strlen(mensaje), sizeof (mensaje));
    //printf("temp_msg: [%s] tamano: %lu size: %lu\n", temp_message.etiqueta, strlen(temp_message.etiqueta), sizeof (temp_message));
    printf("st: %d\n",st);
    return st;

}

int Buzon::Recibir( void * mensaje, int len, long tipo ) {// mensaje puede ser de tipo msgbuf

    return 0;
}
