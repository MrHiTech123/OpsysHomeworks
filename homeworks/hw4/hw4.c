#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>



#include "mystring.h"
#include "linkedlist.h"
#include "linkedmap.h"
#include "bigram.h"
DECLARE_LINKEDLIST_TYPE(pthread_t);
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



char* LinkedList_char__toString(LinkedList_char list) {
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
	
	str toReturn = LinkedList_char__toString(wordSoFar);
	
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

LinkedList_str readAllWordsFromString(str content) {
	int* filePipe = calloc(2, sizeof(int));
	pipe(filePipe);
	int readingEnd = at(filePipe, 0);
	int writingEnd = at(filePipe, 1);
	free(filePipe);
	
	write(writingEnd, content, strlen(content) + 1);	
	
	LinkedList_str toReturn = readAllWordsFromFile(readingEnd);
	
	
	close(readingEnd);
	close(writingEnd);
	
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

pthread_mutex_t BIGRAM_HOLDER_MUTEX = PTHREAD_MUTEX_INITIALIZER;

bool shutDownFlag = false;

void ensureBytesReadOverNetwork(int fd, int bytesToRead, void* buffer) {
	int totalBytesRead = 0;
	int n;
	while (totalBytesRead < bytesToRead) {
		n = recv(fd, buffer + totalBytesRead, bytesToRead - totalBytesRead, 0);
		totalBytesRead += n;
	}
}

void addDataToBigrams(char* data, BiGramHolder bigrams) {
	LinkedList_str words = readAllWordsFromString(data);
	printf("THREAD: extracted %d bigrams\n", LinkedList__length(str, words));
	
	pthread_mutex_lock(&BIGRAM_HOLDER_MUTEX);
		BiGramHolder__addAll(bigrams, words);
	pthread_mutex_unlock(&BIGRAM_HOLDER_MUTEX);
	
	LinkedList__free(str, words);
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

void sendSentenceCollection(int newsd, LinkedList_LinkedList_str sentences) {
	LinkedList__foreach (LinkedList_str, currentSentenceLinkedList, sentences) {
		str sentence = joinAll(currentSentenceLinkedList);
		sendStringMessage(newsd, sentence);
		sendStringMessage(newsd, "\n");
		free(sentence);
	}
	
	sendStringMessage(newsd, "\n");
}

void sendOKResponse(int newsd) {
	sendStringMessage(newsd, "OK\n\n");
	printf("THREAD: sent OK response\n");
}

typedef struct {int listener; int newsd; BiGramHolder bigrams;} appLayerProtocolThreadArgs;


void* appLayerProtocolThread(void* argsPtr) {
	appLayerProtocolThreadArgs* args = (appLayerProtocolThreadArgs*)argsPtr;
	int listener = args->listener;
	int newsd = args->newsd;
	BiGramHolder bigrams = args->bigrams;
	
	free(argsPtr);
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
				addDataToBigrams(readData, bigrams);
				
				sendOKResponse(newsd);
				
				free(readData);
				break;
			case INSTRUCTION_TYPE_GENERATE:
				short breadth, depth;
				
				ensureBytesReadOverNetwork(newsd, sizeof(short), &breadth);
				breadth = ntohs(breadth);
				ensureBytesReadOverNetwork(newsd, sizeof(short), &depth);
				depth = ntohs(depth);
				
				char* word = readWordFromFile(newsd, NULL);
				
				
				printf("THREAD: rcvd 'G' GENERATE request with depth %d and breadth %d\n", breadth, depth);
				
				LinkedList_LinkedList_str sentences = BiGramHolder__generateSentences(bigrams, word, breadth, depth);
				
				
				sendSentenceCollection(newsd, sentences);
				
				printf("THREAD: Sent response with %d phrases.\n", LinkedList__length(LinkedList_str, sentences));
				
				LinkedList__foreach(LinkedList_str, toBeFreed, sentences) {
					LinkedList__free(str, toBeFreed);
				}
				
				LinkedList__free(LinkedList_str, sentences);
				
				
				
				break;
			case INSTRUCTION_TYPE_CLOSE:
				printf("THREAD: rcvd 'C' CLOSE request\n");
				sendOKResponse(newsd);
				return NULL;
				
			case INSTRUCTION_TYPE_SHUTDOWN:
				printf("THREAD: rcvd 'X' SHUTDOWN request\n");
				shutdown(listener, SHUT_RD);
				shutDownFlag = true;
				sendOKResponse(newsd);
				return NULL;
				
		}
		
	}
	return NULL;
}

void appLayerProtocol(int listener, BiGramHolder bigrams) {
	LinkedList_pthread_t threadIds = LinkedList__create;
	
	while (!shutDownFlag) {
		struct sockaddr_in remote_client;
		int addrlen = sizeof( remote_client );

		printf( "MAIN: Blocked on accept()\n" );
		int newsd = accept( listener, (struct sockaddr *)&remote_client, (socklen_t *)&addrlen );
		
		if (shutDownFlag) {
			break;
		}
		
		if ( newsd == -1 ) { perror( "accept() failed" ); continue; }
		
		printf( "MAIN: Accepted new client connection on newsd %d\n", newsd );
		
		pthread_t threadId;
		
		appLayerProtocolThreadArgs* threadArgs = calloc(1, sizeof(appLayerProtocolThreadArgs));
		threadArgs->listener = listener;
		threadArgs->newsd = newsd;
		threadArgs->bigrams = bigrams;
		
		pthread_create(&threadId, NULL, appLayerProtocolThread, threadArgs);
		
		LinkedList__append(pthread_t, threadIds, threadId);
		
		// appLayerProtocolThread(&newsd);
	}
	
	LinkedList__foreach(pthread_t, threadId, threadIds) {
		pthread_join(threadId, NULL);
	}
	
	LinkedList__free(pthread_t, threadIds);
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
	
	// return 0;
	appLayerProtocol(listener, bigrams);
		
	
	LinkedList__free(str, words);
	BiGramHolder__free(bigrams);
	freeAllStrings();
	return 0;
}
