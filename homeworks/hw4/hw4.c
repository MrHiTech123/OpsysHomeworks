#ifndef _MYSTRING
#define _MYSTRING

#include <string.h>

typedef char* str;

#define equals_str(s1, s2) !strcmp(s1, s2)
#define pluralize(amount) (amount == 1)? "" : "s"

#endif

#ifndef _LINKEDLIST
#define _LINKEDLIST

#include <stdlib.h>
#include <stdbool.h>

#define LINKEDLIST_NODETYPE_NAME(type) LinkedListNode_##type
#define LINKEDLIST_TYPE_NAME(type) LinkedList_##type

#define MACRO_VAR(name) macroVar_ ## name ## __LINE__
#define LinkedList__foreach(type, varname, list) type varname; if (list.first) {varname = list.first->value;}; for (LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(currentList) = list.first; MACRO_VAR(currentList); ({MACRO_VAR(currentList) = MACRO_VAR(currentList)->next; if (MACRO_VAR(currentList)) {varname = MACRO_VAR(currentList)->value;}}))
#define LinkedList__create {.first = NULL}
#define LinkedList__createNodePtr(type, v) ({LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(toReturn) = (LINKEDLIST_NODETYPE_NAME(type)*)calloc(1, sizeof(LINKEDLIST_NODETYPE_NAME(type))); MACRO_VAR(toReturn)->value = v; MACRO_VAR(toReturn);})
#define LinkedList__append(type, list, v) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current) && MACRO_VAR(current)->next) {MACRO_VAR(current) = MACRO_VAR(current)->next;}; LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = LinkedList__createNodePtr(type, v); if (list.first) {MACRO_VAR(current)->next = MACRO_VAR(next);} else {list.first = MACRO_VAR(next);};}
#define __LINKED_LIST_FREE(type, list, freeFunction) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(current) = list.first; while (MACRO_VAR(current)) {LINKEDLIST_NODETYPE_NAME(type)* MACRO_VAR(next) = MACRO_VAR(current)->next; freeFunction(MACRO_VAR(current)); MACRO_VAR(current) = MACRO_VAR(next);}}
#define LinkedList__length(type, list) ({int MACRO_VAR(toReturn) = 0; LinkedList__foreach(type, MACRO_VAR(unused), list) {MACRO_VAR(unused) = MACRO_VAR(unused); ++MACRO_VAR(toReturn);}; MACRO_VAR(toReturn);})
#define LinkedList__contains(type, haystack, needle, equalityFunction) ({bool MACRO_VAR(toReturn) = false; LinkedList__foreach(type, MACRO_VAR(current), haystack) {if (equalityFunction(MACRO_VAR(current), needle)) {MACRO_VAR(toReturn) = true; break;}}; MACRO_VAR(toReturn);})
#define LinkedList__copy(type, list) ({LINKEDLIST_TYPE_NAME(type) MACRO_VAR(toReturn__copy) = LinkedList__create; LinkedList__foreach(type, MACRO_VAR(current__copy), list) {LinkedList__append(type, MACRO_VAR(toReturn__copy), MACRO_VAR(current__copy));}; MACRO_VAR(toReturn__copy);})
#define LinkedList__getLast(type, list) ({type MACRO_VAR(toReturn); LinkedList__foreach(type, MACRO_VAR(current), list) {MACRO_VAR(toReturn) = MACRO_VAR(current);}; MACRO_VAR(toReturn);})

#define LinkedList__free(type, list) __LINKED_LIST_FREE(type, list, free)
#define LinkedList__freeFree(type, list) __LINKED_LIST_FREE(type, list, LinkedList__free)
#define LinkedList__freeFreeFree(type, list) __LINKED_LIST_FREE(type, list LinkedList__freeFre ## e)

#define DECLARE_LINKEDLIST_TYPE(type) typedef struct LINKEDLIST_NODETYPE_NAME(type) {struct LINKEDLIST_NODETYPE_NAME(type)* next; type value;} LINKEDLIST_NODETYPE_NAME(type); typedef struct {LINKEDLIST_NODETYPE_NAME(type)* first;} LINKEDLIST_TYPE_NAME(type)

#endif

#ifndef _LINKEDMAP
#define _LINKEDMAP

#include <stdbool.h>





#define LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LinkedMapDataStruct_ ## keyType ## _ ## valueType
#define LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDLIST_TYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_NODE_NAME(keyType, valueType) LINKEDLIST_NODETYPE_NAME(LinkedMapDataStruct_ ## keyType ## _ ## valueType)
#define LINKEDMAP_TYPE_NAME(keyType, valueType) LinkedMap_ ## keyType ## _ ## valueType
#define DECLARE_LINKEDMAP_TYPE(keyType, valueType) typedef struct {keyType key; valueType value;} LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType); DECLARE_LINKEDLIST_TYPE(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType)); typedef LINKEDMAP_DATA_STRUCT_LINKED_LIST_TYPE_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)
// typedef LinkedList__ ## LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) LINKEDMAP_TYPE_NAME(keyType, valueType)

// LinkedList_LinkedMapDataStruct_int_str
#define LinkedMap__create LinkedList__create

#define equals(x, y) x == y


#define LinkedMap__containsKey(keyType, valueType, map, k, equalityFunction) ({bool toReturn = false; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = true;}}; toReturn;})
#define LinkedMap__insert(keyType, valueType, map, k, v) {LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType) MACRO_VAR(toAppend) = {.key = k, .value = v}; LinkedList__append(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), map, MACRO_VAR(toAppend))}
#define LinkedMap__free(keyType, valueType, map) LinkedList__free(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), map)
#define LINKEDMAP_FIND_NODE(keyType, valueType, map, k, equalityFunction) ({LINKEDMAP_NODE_NAME(keyType, valueType)* toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current); break;}}; toReturn;})
#define LinkedMap__find(keyType, valueType, map, k, equalityFunction) ({valueType toReturn; LinkedList__foreach(LINKEDMAP_DATA_STRUCT_NAME(keyType, valueType), MACRO_VAR(current), map) {if (equalityFunction(MACRO_VAR(current).key, k)) {toReturn = MACRO_VAR(current).value; break;}}; toReturn;})
#define LinkedMap__set(keyType, valueType, map, k, v, equalityFunction) if (LinkedMap__containsKey(keyType, valueType, map, k, equalityFunction)) {for (LINKEDMAP_NODE_NAME(keyType, valueType)* MACRO_VAR(current) = map.first; MACRO_VAR(current); MACRO_VAR(current) = MACRO_VAR(current)->next) {if (equalityFunction(MACRO_VAR(current)->value.key, k)) {MACRO_VAR(current)->value.value = v; break;}}} else {LinkedMap__insert(keyType, valueType, map, k, v)}

#endif

#ifndef _BIGRAM
#define _BIGRAM
#include <stdio.h>

DECLARE_LINKEDLIST_TYPE(str);
DECLARE_LINKEDLIST_TYPE(LinkedList_str);
DECLARE_LINKEDMAP_TYPE(str, int);
DECLARE_LINKEDMAP_TYPE(str, LinkedMap_str_int);
typedef LinkedMap_str_LinkedMap_str_int* BiGramHolder;

#define BIGRAMHOLDER_CONTAINS(holder, key) LinkedMap__containsKey(str, LinkedMap_str_int, (*holder), key, equals_str)
#define BIGRAMHOLDER_FIND(holder, key) LinkedMap__find(str, LinkedMap_str_int, (*holder), key, equals_str)
#define BIGRAMHOLDER_FOREACH(varName, holder) LinkedList__foreach(LinkedMapDataStruct_str_LinkedMap_str_int, varName, (*holder))
#define BIGRAMHOLDER_SET(holder, firstWord, value) LinkedMap__set(str, LinkedMap_str_int, (*holder), firstWord, value, equals_str);

void BiGramHolder__display(BiGramHolder holder) {
	printf("Displaying bigram holder!\n");
	BIGRAMHOLDER_FOREACH(s, holder) {
		printf("%s\n", s.key);
		LinkedList__foreach(LinkedMapDataStruct_str_int, i, s.value) {
			printf("\t%s %d\n", i.key, i.value);
		}
	}
}

void BiGramHolder__add(BiGramHolder holder, str key, str value) {
	if (!BIGRAMHOLDER_CONTAINS(holder, key)) {
		LinkedMap_str_int toAdd = LinkedMap__create;
		BIGRAMHOLDER_SET(holder, key, toAdd);
	}
		
	LinkedMap_str_int locationToAdd = BIGRAMHOLDER_FIND(holder, key);
	
	int valueToAdd = 1;
	
	if (LinkedMap__containsKey(str, int, locationToAdd, value, equals_str)) {
		valueToAdd = LinkedMap__find(str, int, locationToAdd, value, equals_str) + 1;
	}
	
	LinkedMap__set(str, int, locationToAdd, value, valueToAdd, equals_str);
	BIGRAMHOLDER_SET(holder, key, locationToAdd);
	
}

int BiGramHolder__getTimes(BiGramHolder holder, str firstWord, str secondWord) {
	LinkedMap_str_int location = BIGRAMHOLDER_FIND(holder, firstWord);
	if (!LinkedMap__containsKey(str, int, location, secondWord, equals_str)) {
		return 0;
	}
	
	return LinkedMap__find(str, int, location, secondWord, equals_str);
}

str joinAll(LinkedList_str toJoin, char toJoinWith) {
	int listLen = LinkedList__length(str, toJoin);
	int stringLen = listLen;
	LinkedList__foreach(str, word, toJoin) {
		stringLen += strlen(word);
	}
		
	str toReturn = calloc(stringLen, sizeof(char));
	int currentIndex = 0;
	
	LinkedList__foreach(str, word2, toJoin) {
		strcpy(toReturn + currentIndex, word2);
		currentIndex += strlen(word2);
		*(toReturn + currentIndex) = toJoinWith;
		++currentIndex;
	}
	
	*(toReturn + stringLen - 1) = '\0';
		
	return toReturn;
}

bool alphabeticallyBefore(str first, str second) {
	for (
		char *currentCharLocationFirst = first, *currentCharLocationSecond = second;;
		++currentCharLocationFirst, ++currentCharLocationSecond) {
		if (!*currentCharLocationFirst) {
			return true;
		}
		
		if (!*currentCharLocationSecond) {
			return false;
		}
		
		if (*currentCharLocationFirst < *currentCharLocationSecond) {
			return true;
		}
		
		if (*currentCharLocationFirst > *currentCharLocationSecond) {
			return false;
		}
	}
	
	return false;
}

LinkedList_str BiGramHolder__rankTopNextWords(BiGramHolder holder, str currentWord, int wordsToRank) {	
	LinkedList_str toReturn = LinkedList__create;
	
	if (!BIGRAMHOLDER_CONTAINS(holder, currentWord)) {
		return toReturn;
	}
	
	LinkedMap_str_int location = BIGRAMHOLDER_FIND(holder, currentWord);
	int locationLen = LinkedList__length(LinkedMapDataStruct_str_int, location);
	
	
	
	for (int i = 0; i < wordsToRank && i < locationLen; ++i) {
		str toAdd = NULL;
		int mostAmounts = 0;
		
		LinkedList__foreach(LinkedMapDataStruct_str_int, wordAndFrequency, location) {
			if (
				(
					wordAndFrequency.value > mostAmounts || 
					(wordAndFrequency.value == mostAmounts && alphabeticallyBefore(wordAndFrequency.key, toAdd))
				)
				&& !LinkedList__contains(str, toReturn, wordAndFrequency.key, equals_str)) {
				toAdd = wordAndFrequency.key;
				mostAmounts = wordAndFrequency.value;
			}
		}
		
		LinkedList__append(str, toReturn, toAdd);
	}
	
	return toReturn;
}



LinkedList_LinkedList_str BiGramHolder__generateSentences(BiGramHolder holder, str rootWord, int breadth, int depth) {
	--depth;
	
	LinkedList_LinkedList_str toReturn = LinkedList__create;
	LinkedList_str rootList = LinkedList__create;
	LinkedList__append(str, rootList, rootWord);
	LinkedList__append(LinkedList_str, toReturn, rootList);
	
	for (int currentDepth = 0; currentDepth < depth; ++currentDepth) {
				
		LinkedList_LinkedList_str toReplaceToReturn = LinkedList__create;
		LinkedList__foreach(LinkedList_str, subList, toReturn) {
						
			str lastWord = LinkedList__getLast(str, subList);
			LinkedList_str possibleNextWords = BiGramHolder__rankTopNextWords(holder, lastWord, breadth);
			int lastNextWordForListCopy = LinkedList__length(str, possibleNextWords) - 1;
			
			if (lastNextWordForListCopy < 0) {
				LinkedList__append(LinkedList_str, toReplaceToReturn, subList);
				continue;
			}
			
			
			int currentBreadth = 0;
			LinkedList__foreach(str, currentNextWord, possibleNextWords) {
								
				if (currentBreadth < lastNextWordForListCopy) {
					LinkedList_str currentCopy = LinkedList__copy(str, subList);
										
					LinkedList__append(str, currentCopy, currentNextWord);
					LinkedList__append(LinkedList_str, toReplaceToReturn, currentCopy);
				}
				++currentBreadth;
			}
			
			LinkedList__append(str, subList, LinkedList__getLast(str, possibleNextWords));
			LinkedList__append(LinkedList_str, toReplaceToReturn, subList);
			
			LinkedList__free(str, possibleNextWords);
		}
		
		LinkedList__free(LinkedList_str, toReturn);
		toReturn = toReplaceToReturn;
		
	}
	
	
	return toReturn;
	
}

void printSentenceCollection(LinkedList_LinkedList_str sentences) {
	LinkedList__foreach (LinkedList_str, currentSentenceLinkedList, sentences) {
		str sentence = joinAll(currentSentenceLinkedList, ' ');
		printf("%s\n", sentence);
		free(sentence);
	}
}


void BiGramHolder__addAll(BiGramHolder holder, LinkedList_str words) {
	str prev = NULL;
	LinkedList__foreach(str, word, words) {
		if (prev != NULL) {
			BiGramHolder__add(holder, prev, word);
		}
		prev = word;
		continue;
	}
}

BiGramHolder BiGramHolder__create(LinkedList_str words) {
	BiGramHolder toReturn = calloc(1, sizeof(LinkedMap_str_LinkedMap_str_int));
	toReturn->first = NULL;
	BiGramHolder__addAll(toReturn, words);
	return toReturn;
}



// LinkedList_str BiGramHolder__(Get)

void BiGramHolder__free(BiGramHolder holder) {
	BIGRAMHOLDER_FOREACH(dataStruct, holder) {
		LinkedMap__free(str, int, dataStruct.value);
	}
	LinkedMap__free(str, LinkedMap_str_int, (*holder));
	free(holder);
}


#endif

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>


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
	int bigramsAmount = LinkedList__length(str, words) - 1;
	printf("THREAD: extracted %d bigram%s\n", bigramsAmount, pluralize(bigramsAmount));
	
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

void* timerThread() {
	for (int time = 0; time < 15; ++time) {
		printf("Time = %d\n", time);
		sleep(1);
	}
	return NULL;
}

pthread_t startTimerThread() {
	pthread_t toReturn;
	pthread_create(&toReturn, NULL, timerThread, NULL);
	return toReturn;
}

str addTwoNewlinesToEnd(str s) {
	int len = strlen(s);
	str toReturn = calloc(len + 3, sizeof(char));
	strcpy(toReturn, s);
	at(toReturn, len - 3) = '\n';
	at(toReturn, len - 2) = '\n';
	return toReturn;
}

str sentenceCollectionToString(LinkedList_LinkedList_str sentencesList) {
	LinkedList_str sentences = LinkedList__create;
	
	LinkedList__foreach(LinkedList_str, sentenceList, sentencesList) {
		str sentence = joinAll(sentenceList, ' ');
		LinkedList__append(str, sentences, sentence);
	}
	
	str toBeNewlineTailed = joinAll(sentences, '\n');
	
	printf("%s\n", toBeNewlineTailed);
	
	LinkedList__foreach(str, sentence, sentences) {
		free(sentence);
	}
	
	LinkedList__free(str, sentences);
	
	
	str toReturn = addTwoNewlinesToEnd(toBeNewlineTailed);
	
	free(toBeNewlineTailed);
	
	return toReturn;
	
}

void sendSentenceCollection(int newsd, LinkedList_LinkedList_str sentences) {
	str toSend = sentenceCollectionToString(sentences);
	printf("START_SENTENCE_COLLECTION\n%s\nEND_SENTENCE_COLLECTION\n", toSend);
	sendStringMessage(newsd, toSend);
	free(toSend);
	
	// LinkedList__foreach (LinkedList_str, currentSentenceLinkedList, sentences) {
	// 	str sentence = joinAll(currentSentenceLinkedList, ' ');
	// 	sendStringMessage(newsd, sentence);
	// 	printf("%s\n", sentence);
	// 	sendStringMessage(newsd, "\n");
	// 	free(sentence);
	// }
	
	// sendStringMessage(newsd, "\n");
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
				printf("THREAD: rcvd '+' ADD request of length %d byte%s\n", length, pluralize(length));
				
				char* readData = calloc(length + 1, sizeof(char));
				
				ensureBytesReadOverNetwork(newsd, length, readData);
				addDataToBigrams(readData, bigrams);
				
				// fprintf(stderr, "Message recieved was %s\n", readData);
				
				sendOKResponse(newsd);
				
				free(readData);
				break;
			case INSTRUCTION_TYPE_GENERATE:
				short breadth, depth;
				
				ensureBytesReadOverNetwork(newsd, sizeof(short), &breadth);
				breadth = ntohs(breadth);
				ensureBytesReadOverNetwork(newsd, sizeof(short), &depth);
				depth = ntohs(depth);
				
				#ifdef DISPLAY_BIGRAMS
					BiGramHolder__display(bigrams);
				#endif
				
				char* word = readWordFromFile(newsd, NULL);
				
				
				printf("THREAD: rcvd 'G' GENERATE request with depth %d and breadth %d\n", depth, breadth);
				
				pthread_mutex_lock(&BIGRAM_HOLDER_MUTEX);
					LinkedList_LinkedList_str sentences = 
						BiGramHolder__generateSentences(bigrams, word, breadth, depth);
				pthread_mutex_unlock(&BIGRAM_HOLDER_MUTEX);
				
				sendSentenceCollection(newsd, sentences);
				
				int phrases = LinkedList__length(LinkedList_str, sentences);
				printf("THREAD: sent response with %d phrase%s\n", phrases, pluralize(phrases));
				
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
	
	#ifdef _TIMER
		pthread_t timerThreadId = startTimerThread();
	#endif
	
	LinkedList_pthread_t threadIds = LinkedList__create;
	
	while (!shutDownFlag) {
		struct sockaddr_in remote_client;
		int addrlen = sizeof( remote_client );

		printf( "MAIN: blocked on accept()\n" );
		int newsd = accept( listener, (struct sockaddr *)&remote_client, (socklen_t *)&addrlen );
		
		if (shutDownFlag) {
			break;
		}
		
		if ( newsd == -1 ) { perror( "accept() failed" ); continue; }
		
		printf( "MAIN: new connection established\n");
		
		pthread_t threadId;
		
		appLayerProtocolThreadArgs* threadArgs = calloc(1, sizeof(appLayerProtocolThreadArgs));
		threadArgs->listener = listener;
		threadArgs->newsd = newsd;
		threadArgs->bigrams = bigrams;
		
		pthread_create(&threadId, NULL, appLayerProtocolThread, threadArgs);
		
		LinkedList__append(pthread_t, threadIds, threadId);
	}
	
	LinkedList__foreach(pthread_t, threadId, threadIds) {
		pthread_join(threadId, NULL);
	}
	
	#ifdef _TIMER
		pthread_join(timerThreadId, NULL);
	#endif
	
	printf("MAIN: shutting down after confirming 0 child threads running\n");
	
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
	
	
	int wordsLen = LinkedList__length(str, words);
	printf("MAIN: extracted %d word%s (%d bigram%s) from %s\n", wordsLen, pluralize(wordsLen), wordsLen - 1, pluralize(wordsLen - 1), inputFileName);
	
	int listener = appLayerProtocolInit(port);
	
	appLayerProtocol(listener, bigrams);
		
	
	LinkedList__free(str, words);
	BiGramHolder__free(bigrams);
	freeAllStrings();
	return 0;
}
