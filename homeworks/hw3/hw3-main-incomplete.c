/* hw3-main.c */

/* bash$ gcc -Wall -Werror -o hw3.out hw3-main.c hw3.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int * numbers = NULL;
int n = 0;
int numthreads = 0;
int numcomparisons = 0;

int hw3();  /* write the hw3() function and place your code in hw3.c */

/* The assign_thread_label() function populates the allocated
 *  memory pointed to by label with a unique four-character string.
 *   Note that this function assumes memory is already allocated
 *    for label; further, this function is not synchronized.
 */
void assign_thread_label( char * label )
{
  /* varies from test case to test case */
}

void usage()
{
  fprintf( stderr, "ERROR: Invalid arguments\n" );
  fprintf( stderr, "USAGE: hw3.out <n> <seed> <max>\n" );
}

int main( int argc, char ** argv )
{
  if ( argc != 4 ) { usage(); return EXIT_FAILURE; }

  n = atoi( *( argv + 1 ) );
  int seed = atoi( *( argv + 2 ) );
  int max = atoi( *( argv + 3 ) );
  if ( n < 1 || seed < 0 || max < 1 ) { usage(); return EXIT_FAILURE; }

  numbers = calloc( n, sizeof( int ) );
  if ( numbers == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }
  srand( seed );
  for ( int i = 0 ; i < n ; i++ ) *(numbers + i) = rand() % max + 1;

  int rc = hw3();

  if ( rc != EXIT_FAILURE )
  {
    printf( "MAIN: sorted using %d threads and %d comparisons\n",
            numthreads, numcomparisons );

    /* on Submitty, there will be more code here that validates
     *  the global variables when your hw3() function returns...
     */
  }

  free( numbers );
  return rc;
}
