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

void sendAddRequest(int sd, char* msg) {
	int lengthOfMessage = strlen(msg) + 5;
	char* toSend = calloc(lengthOfMessage + 1, sizeof(char));
	toSend[0] = '+';
	int * p = (int *)(toSend + 1);
	  *p = htonl( strlen(msg) );  /* data_length */
	           /* ^^ */
	
	strcpy(toSend + 5, msg);
			   

	  printf( "CLIENT: sending ADD request to server...\n" );
	  int n = send( sd, toSend, lengthOfMessage, 0 );
	                  /* ^^ */
	  if ( n == -1 ) { perror( "send() failed" ); return; }

	  n = recv( sd, toSend, MAXBUFFER, 0 );
	  if ( n == -1 ) { perror( "recv() failed" ); return; }
	  if ( n == 0 )
	  {
	    printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
	  }
	  else /* n > 0 */
	  {
	    *(toSend + n) = '\0';
	    printf( "CLIENT: rcvd from server: \n%s\n", toSend );  /* OK\n\n */
	  }
	  
	  free(toSend);
}

void sendGenerateRequest(int sd, short breadth, short depth, char* msg) {
	int lengthOfMessage = strlen(msg) + 6;
	char* toSend = calloc(lengthOfMessage + 1, sizeof(char));
	toSend[0] = 'G';
	
	/* The "...." substring above is a placeholder, which we fill in next... */
	short * ptr = (short *)(toSend + 1);
	*ptr = htons( breadth );  /* breadth */

	ptr = (short *)(toSend + 3);
	*ptr = htons( depth );  /* depth */
	
	strcpy(toSend + 5, msg);
	toSend[lengthOfMessage - 1] = '\n';
	

	  printf( "CLIENT: sending GENERATE request to server...\n" );
	  int n = send( sd, toSend, lengthOfMessage, 0 );
	                  /* ^ */
	  if ( n == -1 ) { perror( "send() failed" ); return; }

	  do
	  {
	    n = recv( sd, toSend, MAXBUFFER, 0 );
	    if ( n == -1 ) { perror( "recv() failed" ); return; }
	    if ( n == 0 )
	    {
	      printf( "CLIENT: rcvd no data; TCP server socket was closed\n" );
	    }
	    else /* n > 0 */
	    {
	      *(toSend + n) = '\0';
	      printf( "CLIENT: rcvd from server: \n%s\n", toSend );

	      /* response must end in "\n\n" -- assuming we receive this in one recv() call */
	      if ( n > 1 && *(toSend + n - 1) == '\n' && *(toSend + n - 2) == '\n' ) break;
	    }
	  }
	  while ( n > 0 );
}

void closeRequest(int sd) {
	char* msg = calloc(MAXBUFFER + 1, sizeof(char));
	printf( "CLIENT: sending CLOSE request to server...\n" );

	int n = send( sd, "C", 1, 0 );
	if ( n == -1 ) { perror( "send() failed" ); return; }
	n = recv( sd, msg, MAXBUFFER, 0 );
	if ( n == -1 ) { perror( "recv() failed" ); return; }
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
	free(msg);
}

void sendShutdownRequest(int sd) {
	char* msg = calloc(MAXBUFFER + 1, sizeof(char));
	  printf( "CLIENT: sending SHUTDOWN request to server...\n" );

	  int n = send( sd, "X", 1, 0 );
	  if ( n == -1 ) { perror( "send() failed" ); return; }
	  n = recv( sd, msg, MAXBUFFER, 0 );
	  if ( n == -1 ) { perror( "recv() failed" ); return; }
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
	  free(msg);
}

int main(int argc, char** argv)
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
  unsigned short server_port = (unsigned short)atoi(argv[1]);
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
  
  printf("Sending requests!\n");
  sendAddRequest(sd, "Aesop's Fables may carry you forward in life!");
  printf("Generating...\n");
  sendGenerateRequest(sd, 2, 7, "may");

  return EXIT_SUCCESS;
}
