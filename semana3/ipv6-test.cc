#include <stdio.h>
#include <string.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {

   Socket s( 's', true );
   char a[512];

   memset( a, 0, 512 );
   //"fe80::ffb4:ea86:1aa4:19c%eno1" prof
   //en0 "fe80::10a1:c47f:9de2:291f%en0"
   s.Connect( (char *) "::1", (char *) "http" );
   s.Write( ( char* )"GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n", 36 );
   s.Read( a, 512 );
   printf( "%s\n", a);
}

