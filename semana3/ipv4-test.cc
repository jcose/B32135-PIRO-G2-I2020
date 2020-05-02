#include <stdio.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {

   Socket s( 's' );
   char a[512];
//"163.178.104.81"
   s.Connect( ( char *)"163.178.104.81", 80 );
   s.Write( ( char* )"GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n", 36 );
   s.Read( a, 512 );
   printf( "%s\n", a);
}

