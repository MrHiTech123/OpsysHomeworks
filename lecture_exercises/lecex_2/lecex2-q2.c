#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
typedef size_t pid_t;
#define O_RDONLY _A_RDONLY
#define access _access

// Windows doesn't have these so I put dummy functions to stop it from complaining
int WIFEXITED(int i) {return 0;}
int WIFSIGNALED(int i) {return 0;}
int WTERMSIG(int i) {return 0;}
int WEXITSTATUS(int i) {return 0;}


#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#endif



typedef struct __charLinkedListNode {
	char value;
	struct __charLinkedListNode* next;
} CharLinkedListNode;

// typedef struct __charLinkedListNode CharLinkedListNode;

typedef struct {
	CharLinkedListNode* top;
} CharStack;

CharStack* allocNewCharStack() {
	CharStack* toReturn = malloc(sizeof(CharStack));
	toReturn->top = 0;
	return toReturn;
}

char CharStack_peek(const CharStack* stack) {
	if (!stack->top) {
		return '\0';
	}
	return stack->top->value;
}

void CharStack_push(CharStack* stack, char toPush) {
	CharLinkedListNode* newTop = (CharLinkedListNode*)malloc(sizeof(CharLinkedListNode));
	newTop->value = toPush;
	newTop->next = stack->top;
	stack->top = newTop;
}

char CharStack_pop(CharStack* stack) {
	if (!stack->top) {
		fprintf(stderr, "Error: Charstack was empty, tried to pop\n");
		exit(1);
	}
	
	char toReturn = CharStack_peek(stack);
	CharLinkedListNode* newTop = stack->top->next;
	free(stack->top);
	stack->top = newTop;
	return toReturn;
}

void CharStack_freeAll(CharStack* stack) {
	while (stack->top) {
		CharStack_pop(stack);
	}
	free(stack);
}

int CharStack_isEmpty(const CharStack* stack) {
	return !stack->top;
}

const char* OPEN_BRACKETS = NULL;
const char* CLOSE_BRACKETS = NULL;
void initializeBracketStrings() {
	char* toBeOpenBrackets = malloc(2 * sizeof(char));
	char* toBeCloseBrackets = malloc(2 * sizeof(char));
	strcpy(toBeOpenBrackets, "(");
	strcpy(toBeCloseBrackets, ")");
	// *(toBeOpenBrackets + 1) = 91;
	// *(toBeCloseBrackets + 1) = 93;
	OPEN_BRACKETS = toBeOpenBrackets;
	CLOSE_BRACKETS = toBeCloseBrackets;
}

void freeBracketStrings() {
	free((void*)OPEN_BRACKETS);
	free((void*)CLOSE_BRACKETS);
}


int isOpenBracket(char toTest) {
	return strchr(OPEN_BRACKETS, toTest) != NULL;
}
int isCloseBracket(char toTest) {
	return strchr(CLOSE_BRACKETS, toTest) != NULL;
}

int bracketsMatch(char openChar, char closeChar) {
	return 
		strchr(OPEN_BRACKETS, openChar) - OPEN_BRACKETS == 
		strchr(CLOSE_BRACKETS, closeChar) - CLOSE_BRACKETS;
}

int howManyValidPairsOfBrackets(const char* fileName) {
	CharStack* stack = allocNewCharStack();
	int toReturn = 0;
	
	int fileHandle = open(fileName, O_RDONLY);
	if (fileHandle < 0) {
		fprintf(stderr, "GRANDCHILD: File %s not found\n", fileName);
		abort();
	}
	
	
	char currentChar;
	while (read(fileHandle, &currentChar, 1)) {
		if (isOpenBracket(currentChar)) {
			CharStack_push(stack, currentChar);
		}
		else if (isCloseBracket(currentChar)) {
			if (bracketsMatch(CharStack_peek(stack), currentChar)) {
				CharStack_pop(stack);
				++toReturn;
			}
		}
	}
	
	close(fileHandle);
	
	CharStack_freeAll(stack);
	
	return toReturn;
	
}


#define CHILD_PROCESS 0
typedef int (*IntSupplier)(int, const char**);

pid_t runAsThread(IntSupplier toRun, int argc, const char** argv) {
	pid_t p = fork();
	if (p == CHILD_PROCESS) {
		int status = toRun(argc, argv);
		exit(status);
	}
	return p;
}

char* pluralLetterEss(int amount) {
	if (amount == 1) {
		return "";
	}
	return "s";
}

int grandchildProcess(int argc, const char** argv) {
	
	if (argc < 2) {
		fprintf(stderr, "GRANDCHILD: usage ./a.out fileName\n");
		abort();
	}
	int pairs = howManyValidPairsOfBrackets(*(argv + 1));
	if (pairs < 0) {
		// No need to print error here, 
		abort();
	}
	
	printf("GRANDCHILD: counted %d properly nested pair%s of parentheses\n", pairs, pluralLetterEss(pairs));
	
	return pairs;
	
}

int childProcess(int argc, const char** argv) {
	pid_t grandchildProcessPid = runAsThread(&grandchildProcess, argc, argv);
	int status;
	waitpid(grandchildProcessPid, &status, 0);
	
	if (WIFSIGNALED(status)) {
		int termSignal = WTERMSIG(status);
		fprintf(stderr, "CHILD: Error, recieved %d\n", termSignal);
		abort();
	}
	else if (WIFEXITED(status)) {
		int exitStatus = WEXITSTATUS(status);
		int toReturn = exitStatus * 2;
		printf("CHILD: doubled %d; returning %d\n", exitStatus, toReturn);
		return toReturn;
	}
	
	fprintf(stderr, "CHILD: Error here, undefined behavior\n");
	abort();
}

void parentProcess(int argc, const char** argv) {
	pid_t childProcessPid = runAsThread(&childProcess, argc, argv);
	
	int status;
	waitpid(childProcessPid, &status, 0);
	
	if (WIFSIGNALED(status)) {
		int termSignal = WTERMSIG(status);
		fprintf(stderr, "PARENT: Error, recieved %d\n", termSignal);
		abort();
	}
	else if (WIFEXITED(status)) {
		int exitStatus = WEXITSTATUS(status);
		printf("PARENT: there are %d parentheses involved in %d properly nested parentheses\n", exitStatus, exitStatus / 2);
	}
	
	
}




int main(int argc, const char** argv)
{
	initializeBracketStrings();	
	
	parentProcess(argc, argv);
	
	freeBracketStrings();
	return 0;
}





