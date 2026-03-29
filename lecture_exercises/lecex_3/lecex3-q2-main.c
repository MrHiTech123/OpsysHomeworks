/* lecex3-q2-main.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>

/* You will write the lecex3_q2_child() function in lecex3-q2.c */
void * lecex3_q2_child( void * arg );

int main( int argc, char ** argv )
{
  if ( argc != 3 )
  {
    fprintf( stderr, "ERROR: invalid arguments\n" );
    fprintf( stderr, "USAGE: ./a.out <input-file> <#-of-bytes>\n" );
    return EXIT_FAILURE;
  }

  int fd = open( *(argv + 1), O_RDONLY );
  if ( fd == -1 ) { perror( "open() failed" ); return EXIT_FAILURE; }

  int bytes = atoi( *(argv + 2) );

  char * buffer = malloc( bytes + 1 );
  int rc = read( fd, buffer, bytes );
  if ( rc == -1 ) { perror( "read() failed" ); return EXIT_FAILURE; }
  *(buffer + rc) = '\0';
  close( fd );

  pthread_t tid;
  rc = pthread_create( &tid, NULL, lecex3_q2_child, buffer );
  if ( rc != 0 ) { fprintf( stderr, "pthread_create() failed (%d)\n", rc ); return EXIT_FAILURE; }

  int * result;
  rc = pthread_join( tid, (void **)&result );
  if ( rc != 0 ) { fprintf( stderr, "pthread_join() failed (%d)\n", rc ); return EXIT_FAILURE; }

  if ( *result != -1 ) printf( "%cbytes: %d%c \"%s\"\n", 0x5b, *result, 0x5d, buffer );
  free( result );
  free( buffer );
  return EXIT_SUCCESS;
}
