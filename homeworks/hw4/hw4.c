#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>



#include "mystring.h"
#include "linkedlist.h"
#include "linkedmap.h"
#include "bigram.h"
DECLARE_LINKEDLIST_TYPE(char);




#define at(arr, i) *(arr + i)
#define APOSTROPHE '\''

LinkedList_str allStringsMarkedForFreeing = LinkedList__create;


void settings() {
    // struct rlimit rl;
    // getrlimit(RLIMIT_NPROC, &rl);
    // rl.rlim_cur = 32;
    // setrlimit(RLIMIT_NPROC, &rl);
    setvbuf(stdout, NULL, _IONBF, 0);
}


char readOneChar(int fd) {
	char toReturn;
	int charactersRead = read(fd, &toReturn, 1);
	if (charactersRead == 0) {
		return '\0';
	}
	toReturn = tolower(toReturn);
	return toReturn;
}

char* linkedList_char__toString(LinkedList_char list) {
	str toReturn = calloc(LinkedList__length(char, list) + 1, sizeof(char));
	int writingIndex = 0;
	
	LinkedList__foreach(char, currentChar, list) {
		at(toReturn, writingIndex) = currentChar;
		++writingIndex;
	}
	
	LinkedList__append(str, allStringsMarkedForFreeing, toReturn);
	
	return toReturn;
}


char* readWordFromFile(int fd, bool* endOfFileFlag) {
	LinkedList_char wordSoFar = LinkedList__create;
	
	int apostrophesRead = 0;
	while (1) {
		char currentCharBeingRead = readOneChar(fd);
		
		if (isalpha(currentCharBeingRead)) {
			LinkedList__append(char, wordSoFar, currentCharBeingRead);
		}
		else if (currentCharBeingRead == APOSTROPHE) {
			++apostrophesRead;
			if (apostrophesRead > 1) {
				break;
			}
			LinkedList__append(char, wordSoFar, currentCharBeingRead);
		}
		else {
			if (endOfFileFlag && !currentCharBeingRead) {
				*endOfFileFlag = true;
			}
			break;
		}
		
	}
	
	str toReturn = linkedList_char__toString(wordSoFar);
	
	LinkedList__free(char, wordSoFar);
	
	return toReturn;
	
}

LinkedList_str readAllWordsFromFile(int fd) {
	LinkedList_str toReturn = LinkedList__create;
	
	bool reachedEndOfFile = false;
	while (!reachedEndOfFile) {
		str currentWord = readWordFromFile(fd, &reachedEndOfFile);
		if (strlen(currentWord)) {
			LinkedList__append(str, toReturn, currentWord);
		}
	}
	
	return toReturn;
}

void freeAllStrings() {
	LinkedList__foreach(str, toDelete, allStringsMarkedForFreeing) {
		free(toDelete);
	}
	LinkedList__free(str, allStringsMarkedForFreeing);
}


#define INSTRUCTION_TYPE_ADD '+'
#define INSTRUCTION_TYPE_GENERATE 'G'
#define INSTRUCTION_TYPE_CLOSE 'C'
#define INSTRUCTION_TYPE_SHUTDOWN 'X'

bool shutDownFlag = false;

void ensureBytesReadOverNetwork(int fd, int bytesToRead, void* buffer) {
	int totalBytesRead = 0;
	int n;
	while (totalBytesRead < bytesToRead) {
		n = recv(fd, buffer + totalBytesRead, bytesToRead - totalBytesRead, 0);
		totalBytesRead += n;
	}
}

void addDataToBigrams(char* data) {
	printf("TODO: Add data %s to bigrams\n", data);
}

int appLayerProtocolInit(unsigned short port) {
	int listener = socket( AF_INET, SOCK_STREAM, 0 );
    /* here, the listener is a socket descriptor (part of the fd table) */

	if ( listener == -1 ) { perror( "socket() failed" ); return EXIT_FAILURE; }

	/* populate the socket structure for bind() */
	struct sockaddr_in tcp_server;
	tcp_server.sin_family = AF_INET;  /* IPv4 */

	tcp_server.sin_addr.s_addr = htonl( INADDR_ANY );
	/* allow any IP address to connect */

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
	
	return listener;
}

void sendStringMessage(int newsd, str toSend) {				
	send(newsd, toSend, strlen(toSend), 0);
}

void sendOKResponse(int newsd) {
	sendStringMessage(newsd, "OK\n\n");
	printf("THREAD: sent OK response\n");
}

void* appLayerProtocolThread(void* newsdPtr) {
	int newsd = *(int*)newsdPtr;
	int n = 1;
	while (n > 0) {
		char instructionType;
		n = recv(newsd, &instructionType, sizeof(char), 0);
		switch (instructionType) {
			case INSTRUCTION_TYPE_ADD:
				int length;
				ensureBytesReadOverNetwork(newsd, sizeof(int), &length);
				
				length = ntohl(length);
				printf("THREAD: rcvd '+' ADD request of length %d bytes\n", length);
				char* readData = calloc(length + 1, sizeof(char));
				
				ensureBytesReadOverNetwork(newsd, length, readData);
				addDataToBigrams(readData);
				
				sendOKResponse(newsd);
				
				free(readData);
				break;
			case INSTRUCTION_TYPE_GENERATE:
				short breadth, depth;
				
				ensureBytesReadOverNetwork(newsd, sizeof(short), &breadth);
				breadth = ntohs(breadth);
				ensureBytesReadOverNetwork(newsd, sizeof(short), &depth);
				depth = ntohs(depth);
				
				printf("THREAD: rcvd 'G' GENERATE request with depth %d and breadth %d\n", breadth, depth);
				
				str toSend = "Phrases delineated by \n chars\n\n";
				sendStringMessage(newsd, toSend);
				
				
				printf("Generate\n");
				break;
			case INSTRUCTION_TYPE_CLOSE:
				printf("THREAD: rcvd 'C' CLOSE request\n");
				printf("%c char\n", instructionType);
				sendOKResponse(newsd);
				return NULL;
				
			case INSTRUCTION_TYPE_SHUTDOWN:
				printf("THREAD: rcvd 'X' SHUTDOWN request\n");
				printf("%c char\n", instructionType);
				shutDownFlag = true;
				sendOKResponse(newsd);
				return NULL;
				
		}
		
	}
	return NULL;
}

void appLayerProtocol(int listener) {
	while (!shutDownFlag) {
		struct sockaddr_in remote_client;
		int addrlen = sizeof( remote_client );

		printf( "MAIN: Blocked on accept()\n" );
		int newsd = accept( listener, (struct sockaddr *)&remote_client, (socklen_t *)&addrlen );
		if ( newsd == -1 ) { perror( "accept() failed" ); continue; }
		
		printf( "MAIN: Accepted new client connection on newsd %d\n", newsd );
		/* newsd is a newly assigned socket (file) descriptor that is tied to
		 *  the new incoming TCP connection that has been established
		 */
		appLayerProtocolThread(&newsd);
	}
	
}






int main(int argc, char** argv)
{
	settings();
	str inputFileName = at(argv, 1);
	unsigned short port = (unsigned short)atoi(at(argv, 2));
	int inputFile = open(inputFileName, O_RDONLY);
	
	LinkedList_str words = readAllWordsFromFile(inputFile);
	
	BiGramHolder bigrams = BiGramHolder__create(words);
	
	
	int listener = appLayerProtocolInit(port);
	printf("%d\n", listener);
	
	// return 0;
	appLayerProtocol(listener);
	
	
	
	
	LinkedList__free(str, words);
	BiGramHolder__free(bigrams);
	freeAllStrings();
	return 0;
}
