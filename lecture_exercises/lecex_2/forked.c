/* forked.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* implement these functions separately in lecex2-q1.c */
int lecex2_child( const char * filename );
int lecex2_parent();
int main()
{
const char * filename = "input.txt";
int rc;
/* create a new (child) process */
pid_t p = fork();
if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }
if ( p == 0 ) rc = lecex2_child( filename );
else /* p > 0 */ rc = lecex2_parent();
return rc;
}