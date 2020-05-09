#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "Socket.h"

Socket::Socket( bool ipv6_entered ): ipv6( ipv6_entered ) {
	if( this->ipv6 ){	// ipv6: si queremos un socket para IPv6
    	    	this->idSocket = socket( AF_INET6, SOCK_STREAM, 0 );
    	}
    	else {
    		this->idSocket = socket( AF_INET, SOCK_STREAM, 0 );
    	}
    	
    	if( this->idSocket == -1 ){
    		printf( "Socket Constructor: error %s\n", hstrerror(errno) );
    		exit(1);
    	}
}

/* 
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( char tipo, bool ipv6 ): type( tipo ), ipv6( ipv6 ) {
    	this->idSocket = 0;
    	if( this->type == 's' ) {	// 's' para "stream"
    		if( this->ipv6 ) {	// ipv6: si queremos un socket para IPv6
 	    		this->idSocket = socket( AF_INET6, SOCK_STREAM, 0 );
    		}
    		else {
    			this->idSocket = socket( AF_INET, SOCK_STREAM, 0 );
    		}
   	 }
   	else if( this->type == 'd' ){	// 'd' para "datagram"
    		if( this->ipv6 ){	// ipv6: si queremos un socket para IPv6
    	    		this->idSocket = socket( AF_INET6, SOCK_DGRAM, 0 );
    		}
    		else {
    			this->idSocket = socket( AF_INET, SOCK_DGRAM, 0 );
    		}
    	}
    	else{	
		printf("Socket constructor failed. Type parameter must be 's' or 'd', but '%c' was provided\n", this->type);
		exit(1);
    	}
    
	if( this->idSocket == -1 ){
		printf( "Socket Constructor: error %s\n", hstrerror(errno) );
		exit(1);
	}
}

Socket::~Socket(){
	if ( this->sock != NULL ) {
		delete this->sock;
	}
	Close();
}

void Socket::Close(){
	if( close(this->idSocket) == -1 ) {
		printf( "Socket Close: error %s\n", hstrerror(errno) );
	}
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
	
	if( hostbyname != NULL ) {
		host = hostbyname->h_name;
	}

	if( this->ipv6 ) {
		struct sockaddr_in6 s_address;
		s_address.sin6_family = AF_INET6;
		inet_pton( AF_INET6, host, &s_address.sin6_addr );	
		s_address.sin6_port = htons( port );
		status = connect( this->idSocket, ( struct sockaddr* ) &s_address, sizeof( s_address ) );
	}
	else {
		struct sockaddr_in address;
		address.sin_addr.s_addr = inet_addr( host );
		address.sin_family = AF_INET;
		address.sin_port = htons( port );
		status = connect( this->idSocket, ( struct sockaddr* ) &address, sizeof( address ) );
	}

	if( status == -1 ) {
		printf( "Socket Connect: error %s\n", hstrerror(errno) );
		exit(1);
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
	
	if(proto != NULL) {
		port = proto->p_proto;
	}
	else if ( strcmp( service, "http" ) == 0 ){
		port = 7002;
	}
	else {
		port = 9876;
	}
   	
	if( this->ipv6 ) {
		struct sockaddr_in6 s_address;
		s_address.sin6_family = AF_INET6;
		inet_pton( AF_INET6, host, &s_address.sin6_addr );	
		s_address.sin6_port = htons( port );
		status = connect( this->idSocket, ( struct sockaddr* ) &s_address, sizeof( s_address ) );
	}
	else {
		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr( host );
		address.sin_port = htons( port );
		status = connect( this->idSocket, ( struct sockaddr* ) &address, sizeof( address ) );
	}

	if(status == -1) {
		printf( "Socket Connect: error %s\n", hstrerror(errno) );
		exit(1);
	}
   	return status;
}

int Socket::Read( char *text, int len ) {
	int value = read( this->idSocket, ( void * )text, len );
	if( value == -1 ) {
		printf( "Socket Read: error %s\n", hstrerror(errno) );
		exit(1);
	}
   	return value;
}

int Socket::Write( char *text , int len) {
	memset( (void *)text, '0', len);
	int value = write( this->idSocket, ( const void *) text, len );
	if( value == -1 ) {
		printf( "Socket Write: error %s\n", hstrerror(errno) );
		exit(1);
	}
   	return value;
}

int Socket::Write( char *text ) {
	
	int value = 0;
	if ( text != NULL ) {
		value = write( this->idSocket, ( const void *) text, strlen(text) );
		if( value == -1 ) {
			printf( "Socket Write: error %s\n", hstrerror(errno) );
			exit(1);
		}
	}
	else {
		printf( "Usage: program 'some text'\n" );
		exit(0);
	}
   	return value;
}

int Socket::Listen( int queue ) {
//int listen(int sockfd, int backlog);
	int ret = listen( this->idSocket, queue );
	if( ret == -1 ) {
		printf( "Socket Listen: error %s\n", hstrerror(errno) );
		exit(1);
	}	
	return 0;
}

int Socket::Bind( int port ) {
	
	int ret;
	if( this->ipv6 ) {
		struct in6_addr any_addr;// = IN6ADDR_ANY_INIT;
		struct sockaddr_in6 my_address6;
		my_address6.sin6_family = AF_INET6;
		my_address6.sin6_port = htons( port );
		my_address6.sin6_addr = any_addr;
		ret = bind( this->idSocket, ( struct sockaddr * ) &my_address6, sizeof( struct sockaddr_in6 ) );
	}
	else {
		struct sockaddr_in my_address;
		my_address.sin_family = AF_INET;
		my_address.sin_port = htons( port );
		my_address.sin_addr.s_addr = htonl( INADDR_ANY );
		ret = bind( this->idSocket, ( struct sockaddr * ) &my_address, sizeof( struct sockaddr_in ) );
	}
	
	if( ret == -1 ) {
		printf( "Socket Bind: error %s\n", hstrerror(errno) );
		exit(0);
	}
	return ret;
}

Socket * Socket::Accept(){
	struct sockaddr peer_addr;
	
	socklen_t len = sizeof(sockaddr);
	
	int nfd = accept( this->idSocket,  &peer_addr, &len );

	this->sock = new Socket( this->ipv6 );

	
	if( nfd == -1 ){
		printf( "Socket Accept: error %s\n", hstrerror(errno) );
		exit(1);
	}

    	if ( this->sock == NULL) {
    		printf( "Socket Accept error: socket construction failed\n ");
    		exit(1);
    	}
    	this->sock->SetIDSocket(nfd);
    	return this->sock;
}

int Socket::Shutdown( int mode ) {
	int status = -1;
	if( mode >= 0 && mode < 3 ) {
		status = shutdown( this->idSocket, mode);
	}
	else {
		printf( "Shutdown: values accepted\n SHUT_RD or 0 Further receives are disallowed\n SHUT_WR or 1 Further sends are disallowed\n SHUT_RDWR or 2 Further sends and receives are disallowed\n ");
	}
	if( status == -1 ) {
		printf( "Socket Shutdown: error %s\n", hstrerror(errno) );
		exit(1);
	}
	return status;
}

void Socket::SetIDSocket(int id){
    	this->idSocket = id;
}

