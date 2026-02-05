/* fork-with-waitpid.c */

#include <stdio.h>
#include <stdlib.h>

// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
typedef size_t pid_t;
#define O_RDONLY _A_RDONLY
#define access _access

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#endif


int main()
{

  pid_t p;     /* pid_t is essentially an unsigned int */

  p = fork();  /* attempt to create a new (child) process */
  if ( p == -1 ) { perror( "fork() failed" ); return EXIT_FAILURE; }

  if ( p == 0 )     /* CHILD PROCESS */
  {
    printf( "CHILD: Happy birthday to me!\n" );
    printf( "CHILD: My PID is %d\n", getpid() );
    return -7;     /* -7 in base 2 (two's complement): 11111001 (249 or -7) */
  }
  else /* p > 0 */  /* PARENT PROCESS */
  {
    printf( "PARENT: My new child process has PID %d\n", p );
    printf( "PARENT: My PID is %d\n", getpid() );

    /* TO DO: what is the EXACT terminal output for this code?
     * --- the waitpid() call below will not return until the
     *      child process has terminated...
     */

    /* wait (BLOCK indefinitely) for my child process to end... */
    int status;
    pid_t child_pid = waitpid( p, &status, 0 );
      /* child_pid == p */

    printf( "PARENT: child process %d terminated...\n", child_pid );

    if ( WIFSIGNALED( status ) )
    {
      int term_signal = WTERMSIG( status );
      printf( "PARENT: ...abnormally (killed by signal %d)\n", term_signal );
    }
    else if ( WIFEXITED( status ) )
    {
      int exit_status = WEXITSTATUS( status );
      printf( "PARENT: ...normally with exit status %d (int)\n", exit_status );

      /* using "hh" in front of "d" below causes printf() to
       *  treat the least significant 8 bits as a char value...
       */
      printf( "PARENT: ...normally with exit status %hhd (char)\n", exit_status );
    }
  }

  return EXIT_SUCCESS;
}

#if 0
                .
                .
                .
             +------+
             | bash |  bash$ a.out
             +------+
                  \
                   \
                  +-------+
                  | a.out | (parent)   return EXIT_SUCCESS (0)
                  +-------+              to its parent (bash)
                       \
                        \
                       +-------+
                       | a.out | (child)    return 13 to its parent
                       +-------+             (or terminated by SIGSEGV)

#endif