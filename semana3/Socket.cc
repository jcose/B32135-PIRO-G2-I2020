#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
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
    idSocket = 0;
    if( type == 's' ){	// 's' para "stream"
    	if(ipv6){	// ipv6: si queremos un socket para IPv6
    	    	idSocket = socket( AF_INET6, SOCK_STREAM, 0 );
    	}
    	else {
    		idSocket = socket( AF_INET, SOCK_STREAM, 0 );
    	}
    }
    else if( type == 'd' ){	// 'd' para "datagram"
    	if(ipv6){	// ipv6: si queremos un socket para IPv6
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
	int status;
	char * host = hostip;
	struct hostent *hostbyname;
	
	hostbyname = gethostbyname( hostip );
	if( hostbyname != nullptr ) {
		host = hostbyname->h_name;
	}

	if( ipv6 ) {
		struct sockaddr_in6 s_address;
		s_address.sin6_family = AF_INET6;
		inet_pton( AF_INET6, host, &s_address.sin6_addr );	
		s_address.sin6_port = htons( port );
		status = connect( idSocket, ( struct sockaddr* ) &s_address, sizeof( s_address ) );
	}
	else {
		struct sockaddr_in address;
		address.sin_addr.s_addr = inet_addr( host );
		address.sin_family = AF_INET;
		address.sin_port = htons( port );
		status = connect( idSocket, ( struct sockaddr* ) &address, sizeof( address ) );
	}

	if(status == -1) {
		printf("Connect failed: %d\n", errno);
	}
   	return status;
   	
}

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( char *host, char *service ) {
   	int status, port;
	struct protoent *proto;
	proto = getprotobyname(service);
	if(proto != nullptr) {
		port = proto->p_proto;
	}
	else if ( strcmp( service, "http" ) == 0 ){
		port = 7002;
	}
	else {
		port = 12321;
	}
   	
	if( ipv6 ) {
		struct sockaddr_in6 s_address;
		s_address.sin6_family = AF_INET6;
		inet_pton( AF_INET6, host, &s_address.sin6_addr );	
		s_address.sin6_port = htons( port );
		status = connect( idSocket, ( struct sockaddr* ) &s_address, sizeof( s_address ) );
	}
	else {
		struct sockaddr_in address;
		address.sin_addr.s_addr = inet_addr( host );
		address.sin_family = AF_INET;
		address.sin_port = htons( port );
		status = connect( idSocket, ( struct sockaddr* ) &address, sizeof( address ) );
	}

	if(status == -1) {
		printf("Connect failed: %d\n", errno);
	}
   	return status;
}

int Socket::Read( char *text, int len ) {
	int value = read( idSocket, ( void * )text, len );
	if( value == -1 ) {
		printf( "Socket Read: %d\n", errno );
	}
   	return value;
}

int Socket::Write( char *text , int len) {
	int value = write( idSocket, ( const void *) text, len );
	if( value == -1 ) {
		printf( "Socket Write: %d\n", errno );
	}
   	return value;
}

int Socket::Write( char *text ) {
	int value = write( idSocket, ( const void *) text, strlen(text) );
	if( value == -1 ) {
		printf( "Socket Write: %d\n", errno );
	}
   	return value;
}

int Socket::Listen( int queue ) {
	return -1;
}

int Socket::Bind( int port ) {
	/*struct sockaddr_un su;
   	if(idSocket == -1){
    		su.sun_family = AF_INET;
    		strcpy( su.sun_path, "/semana3/mysocketinet");
    	}*/
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

