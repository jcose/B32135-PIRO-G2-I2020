#include <errno.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "Socket.h"

/* 
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( char tipo, bool ipv6 ): type( tipo ), ipv6( ipv6 ) {
    //int socket(int domain, int type, int protocol);
    //domain AF_INET or AF_UNIX
    //type SOCK_STREAM or SOCK_DGRAM
    //protocol 0 para que la eleccion la haga el sistema
    //return director de fichero o -1 en caso de error. errno
    
    idSocket = 0;
    if( type == 's' ){
    	if(ipv6){
    	    	idSocket = socket( AF_INET6, SOCK_STREAM, 0 );
    	}
    	else {
    		idSocket = socket( AF_INET, SOCK_STREAM, 0 );
    	}
    }
    else if( type == 'd' ){
    	if(ipv6){
    	    	idSocket = socket( AF_INET6, SOCK_DGRAM, 0 );
    	}
    	else {
    		idSocket = socket( AF_INET, SOCK_DGRAM, 0 );
    	}
    }
    else{
	printf("Socket constructor failed. Type parameter must be 's' or 'd', but '%c' was provided\n", type);
    }
    
    if( idSocket == -1 ){
    	printf( "Socket failed: %d\n", errno );
    }
}


Socket::~Socket(){
    Close();
}


void Socket::Close(){
	if( close(idSocket) == -1 )
		printf( "Socket close: %d\n", errno );
}

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   int port: ubicacion del proceso, por ejemplo 80
 */
int Socket::Connect( char * hostip, int port ) {
	struct sockaddr_in address;
	if( type == 's' ) {
		address.sin_addr.s_addr = inet_addr( hostip );
		address.sin_family = AF_INET;
		address.sin_port = htons( port );
		if( connect( idSocket, (struct sockaddr*) &address, sizeof(address) ) == -1 ) {
			printf("Connect failed: %d\n", errno);
			return -1;
		}
	}
   	return 0;
}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( char *host, char *service ) {
	struct sockaddr_in6 s_address;
	if( type == 's' ) {
		s_address.sin6_family = AF_INET6;
		inet_pton(AF_INET6, /*host*/"::1", &s_address.sin6_addr);
		//convertir service a int
		
		s_address.sin6_port = htons(80);
		if( connect( idSocket, (struct sockaddr*) &s_address, sizeof(s_address) ) == -1 ) {
			printf("Connect failed: %d\n", errno);
			return -1;
		}
	}	
   return 0;
}


int Socket::Read( char *text, int len ) {
	int value;
	if( ( value = read( idSocket, ( void * )text, len ) ) == -1 ) {
		printf( "Socket Read: %d\n", errno );
	}
   	return value;
}


int Socket::Write( char *text , int len) {
	int value;
	if( (value = write( idSocket, ( const void *) text, len ) ) == -1 ) {
		printf( "Socket Write: %d\n", errno );
	}
   	return value;
}

int Socket::Write( char *text ) {
	int value;
	if( (value = write( idSocket, ( const void *) text, strlen(text) ) ) == -1 ) {
		printf( "Socket Write: %d\n", errno );
	}
   	return value;
}


int Socket::Listen( int queue ) {

    return -1;

}


int Socket::Bind( int port ) {
	struct sockaddr_un su;
   	if(idSocket == -1){
    		su.sun_family = AF_INET;
    		strcpy( su.sun_path, "/semana3/mysocketinet");
    	}
	return -1;

}


//Socket * Socket::Accept(){

  //  return -1;

//}


int Socket::Shutdown( int mode ) {

    return -1;

}


void Socket::SetIDSocket(int id){

    idSocket = id;

}

