/* hw4-client.c */

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define MAXBUFFER 1024

int main()
{
  /* create TCP client socket (endpoint) */
  int sd = socket( AF_INET, SOCK_STREAM, 0 );
  if ( sd == -1 ) { perror( "socket() failed" ); exit( EXIT_FAILURE ); }

  /* whoops --- this gethostbyname() function is deprecated... */
  struct hostent * hp = gethostbyname( "localhost" );

#if 0
  /* other options... */
  struct hostent * hp = gethostbyname( "linux.cs.rpi.edu" );
  struct hostent * hp = gethostbyname( "127.0.0.1" );  /* localhost */
  struct hostent * hp = gethostbyname( "128.113.126.73" );
#endif

  if ( hp == NULL )
  {
    fprintf( stderr, "ERROR: gethostbyname() failed\n" );
    return EXIT_FAILURE;
  }

  /* replace (above) the deprecated gethostbyname() call with getaddrinfo() */

  struct sockaddr_in tcp_server;
  tcp_server.sin_family = AF_INET;  /* IPv4 */
  memcpy( (void *)&tcp_server.sin_addr, (void *)hp->h_addr, hp->h_length );
  unsigned short server_port = 8123;
  tcp_server.sin_port = htons( server_port );

  printf( "CLIENT: connecting to server...\n" );

  if ( connect( sd, (struct sockaddr *)&tcp_server, sizeof( tcp_server ) ) == -1 )
  {
    perror( "connect() failed" );
    return EXIT_FAILURE;
  }


  /* The implementation of the application protocol is below... */

  char * msg = calloc( MAXBUFFER + 1, sizeof( char ) );
  if ( msg == NULL ) { perror( "calloc() failed" ); return EXIT_FAILURE; }

  int n = 0;

#if 1
  /* Send an ADD request */
  sprintf( msg, "+....The quick dog jumps over the moon." );
                   /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
                   /*              34 bytes              */

  /* The "...." substring above is a placeholder, which we fill in next... */
  int * p = (int *)(msg + 1);
  *p = htonl( 34 );  /* data_length */
           /* ^^ */

  printf( "CLIENT: sending ADD request to server...\n" );
  n = send( sd, msg, 39, 0 );
                  /* ^^ */
  if ( n == -1 ) { perror( "send() failed" ); return EXIT_FAILURE; }

  n = recv( sd, msg, MAXBUFFER, 0 );
  if ( n == -1 ) { perror( "recv() failed" ); return EXIT_FAILURE; }
  if ( n == 0 )
  {
    printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
  }
  else /* n > 0 */
  {
    *(msg + n) = '\0';
    printf( "CLIENT: rcvd from server: \n%s\n", msg );  /* OK\n\n */
  }
#endif

#if 1
  /* Send a GENERATE request */
  sprintf( msg, "G....the\n" );
                   /* ^^^ */

  /* The "...." substring above is a placeholder, which we fill in next... */
  short * ptr = (short *)(msg + 1);
  *ptr = htons( 3 );  /* breadth */

  ptr = (short *)(msg + 3);
  *ptr = htons( 4 );  /* depth */

  printf( "CLIENT: sending GENERATE request to server...\n" );
  n = send( sd, msg, 9, 0 );
                  /* ^ */
  if ( n == -1 ) { perror( "send() failed" ); return EXIT_FAILURE; }

  do
  {
    n = recv( sd, msg, MAXBUFFER, 0 );
    if ( n == -1 ) { perror( "recv() failed" ); return EXIT_FAILURE; }
    if ( n == 0 )
    {
      printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
    }
    else /* n > 0 */
    {
      *(msg + n) = '\0';
      printf( "CLIENT: rcvd from server: \n%s\n", msg );

      /* response must end in "\n\n" -- assuming we receive this in one recv() call */
      if ( n > 1 && *(msg + n - 1) == '\n' && *(msg + n - 2) == '\n' ) break;
    }
  }
  while ( n > 0 );
#endif

#if 0
  printf( "CLIENT: sending CLOSE request to server...\n" );

  n = send( sd, "C", 1, 0 );
  if ( n == -1 ) { perror( "send() failed" ); return EXIT_FAILURE; }
  n = recv( sd, msg, MAXBUFFER, 0 );
  if ( n == -1 ) { perror( "recv() failed" ); return EXIT_FAILURE; }
  if ( n == 0 )
  {
    printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
  }
  else /* n > 0 */
  {
    *(msg + n) = '\0';
    /* assuming we receive the response in one recv() call... */
    printf( "CLIENT: rcvd from server: \n%s\n", msg );  /* OK\n\n */
  }
  close( sd );
#endif

#if 1
  printf( "CLIENT: sending SHUTDOWN request to server...\n" );

  n = send( sd, "X", 1, 0 );
  if ( n == -1 ) { perror( "send() failed" ); return EXIT_FAILURE; }
  n = recv( sd, msg, MAXBUFFER, 0 );
  if ( n == -1 ) { perror( "recv() failed" ); return EXIT_FAILURE; }
  if ( n == 0 )
  {
    printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
  }
  else /* n > 0 */
  {
    *(msg + n) = '\0';
    /* assuming we receive the response in one recv() call... */
    printf( "CLIENT: rcvd from server: \n%s\n", msg );  /* OK\n\n */
  }
  close( sd );
#endif

  return EXIT_SUCCESS;
}
