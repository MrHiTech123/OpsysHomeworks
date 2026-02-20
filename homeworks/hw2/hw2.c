#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#endif

void rlimitSettings() {
	#ifndef _WIN32
		struct rlimit rl;
		getrlimit(RLIMIT_NPROC, &rl);
		rl.rlim_cur = 32;
		setrlimit(RLIMIT_NPROC, &rl);
	#endif
	
}

typedef enum {
	MOVE_NONE = 0,
	MOVE_LEFT = 1,
	MOVE_RIGHT = 2,
	MOVE_UP = 4,
	MOVE_DOWN = 8
	
} Moves;

typedef struct {
	bool** visited;
	int numRows;
	int numCols;
} Board;

Board* Board_createAndAllocate(int numRows, int numCols) {
	Board* toReturn = calloc(1, sizeof(Board));
	toReturn->numRows = numRows;
	toReturn->numCols = numCols;
	
	toReturn->visited = calloc(toReturn->numRows, sizeof(bool*));
	for (int i = 0; i < toReturn->numRows; ++i) {
		*(toReturn->visited + i) = calloc(toReturn->numCols, sizeof(bool));
	}
	
	return toReturn;
}

void Board_free(Board* board) {
	for (int i = 0; i < board->numRows; ++i) {
		free(*(board->visited + i));
	}
	
	free(board->visited);
	free(board);
}


int numValidMoves(Moves moves) {
	int toReturn = 0;
	while (moves) {
		if (moves & 1) {
			++toReturn;
		}
		moves >>= 1;
	}
	return toReturn;
}

Moves validMoves(int numRows, int numCols, int row, int column) {
	Moves toReturn = MOVE_NONE;
	
	if (row > 0) toReturn &= MOVE_UP;
	if (column > 0) toReturn &= MOVE_LEFT;
	if (row < (numRows - 1)) toReturn &= MOVE_DOWN;
	if (column < (numCols - 1)) toReturn &= MOVE_RIGHT;
	
	return toReturn;
	
}


int actualProgram(int numRows, int numCols, int row, int col) {
	Board* board = Board_createAndAllocate(numRows, numCols);
	
	
	Board_free(board);
}

int main(int argc, const char** argv)
{
	if (argc != 5) {
		fprintf(stderr, "Error: usage hw2 m n r c\n");
		return EXIT_FAILURE;
	}
	
	int numRows = atoi(*(argv + 1));
	int numCols = atoi(*(argv + 2));
	int startingRow = atoi(*(argv + 3));
	int startingColumn = atoi(*(argv + 4));
	
	if (numRows <= 0 || numCols <= 0) {
		fprintf(stderr, "Error: m or n is <= 0\n");
		return EXIT_FAILURE;
	}
	if (numRows * numCols > 255) {
		fprintf(stderr, "Error, m * n is too big\n");
		return EXIT_FAILURE;
	}
	
	if (startingRow < 0 || startingRow >= numRows || startingColumn < 0 || startingColumn >= numCols) {
		fprintf(stderr, "Error: starting coords not in bounds\n");
		return EXIT_FAILURE;
	}
	
	#ifdef DEBUG
	printf("%d %d %d %d %d\n", m, n, r, c, numValidMoves(10));
	#endif
	
	actualProgram(numRows, numCols, startingRow, startingColumn);
	
	return 0;
}



