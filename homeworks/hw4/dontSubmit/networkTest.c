/* tcp-server-iterative.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXBUFFER 16

void appLayerProtocol(int listener) {
	while ( 1 )
  	{
	struct sockaddr_in remote_client;
	int addrlen = sizeof( remote_client );

	printf( "SERVER: Blocked on accept()\n" );
	int newsd = accept( listener, (struct sockaddr *)&remote_client,
	                    (socklen_t *)&addrlen );
	if ( newsd == -1 ) { perror( "accept() failed" ); continue; }

	printf( "SERVER: Accepted new client connection on newsd %d\n", newsd );
	/* newsd is a newly assigned socket (file) descriptor that is tied to
	 *  the new incoming TCP connection that has been established
	 */

	int n;
	do
	{
	  char buffer[MAXBUFFER+1];
	  /* recv() is a BLOCKING call... */
	  n = recv( newsd, buffer, MAXBUFFER, 0 );   /* or we can use read() */

	  if ( n == -1 )
	  {
	    perror( "recv() failed" );
	    return EXIT_FAILURE;  /* TO DO: can we instead loop back to accept() here? */
	  }
	  else if ( n == 0 )
	  {
	    /* the remote client shut down or the socket connection was closed */
	    printf( "SERVER: Rcvd 0 from recv(); closing descriptor %d...\n", newsd );
	  }
	  else /* n > 0 */
	  {
	    buffer[n] = '\0';
	    printf( "SERVER: Rcvd message (%d bytes) from %s: \"%s\"\n", n,
	            inet_ntoa( (struct in_addr)remote_client.sin_addr ), buffer );

	    printf( "SERVER: Sending acknowledgement to client...\n" );
	    n = send( newsd, "ACK\n", 4, 0 );   /* or we can use write() */
	    if ( n == -1 ) { perror( "write() failed" ); /* return EXIT_FAILURE; */ }
	  }
	}
	while ( n > 0 );   /* keep looping since we have received at least 1 byte */

	close( newsd );
  }
}

int main()
{
  /* Create the listener socket as TCP socket (SOCK_STREAM) */
  int listener = socket( AF_INET, SOCK_STREAM, 0 );
    /* here, the listener is a socket descriptor (part of the fd table) */

  if ( listener == -1 ) { perror( "socket() failed" ); return EXIT_FAILURE; }

  /* populate the socket structure for bind() */
  struct sockaddr_in tcp_server;
  tcp_server.sin_family = AF_INET;  /* IPv4 */

  tcp_server.sin_addr.s_addr = htonl( INADDR_ANY );
    /* allow any IP address to connect */

  unsigned short port = 8123;   /* TO DO: have this given as a command-line arg */

  tcp_server.sin_port = htons( port );

  if ( bind( listener, (struct sockaddr *)&tcp_server, sizeof( tcp_server ) ) == -1 )
  {
    perror( "bind() failed" );
    return EXIT_FAILURE;
  }

  /* identify our port number (socket) as a TCP listener */
  if ( listen( listener, 5 ) == -1 )
  {
    perror( "listen() failed" );
    return EXIT_FAILURE;
  }

  printf( "SERVER: TCP listener socket (fd %d) bound to port %d\n", listener, port );


  /************** APPLICATION-LAYER PROTOCOL starts here... *******************/
  appLayerProtocol(listener);

  close( listener );

  return EXIT_SUCCESS;
}