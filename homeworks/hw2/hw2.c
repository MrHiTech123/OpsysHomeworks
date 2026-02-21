#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define arraySet(arr, index, value) *(arr + index) = value
#define arrayGet(arr, index) *(arr + index)
#define matrixSet(mat, row, col, value) *(*(mat + row) + col) = value
#define matrixGet(mat, row, col) *(*(mat + row) + col)


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
	MOVE_UP = 1,
	MOVE_RIGHT = 2,
	MOVE_DOWN = 4,
	MOVE_LEFT = 8
} Move;

#define forEachMove(currentMove) for (Move currentMove = MOVE_UP; currentMove <= MOVE_LEFT; currentMove <<= 1)


void Move_getAdditionsToRowCol(const Move move, int* rowModResult, int* colModResult) {
	switch (move)
	{
	case MOVE_LEFT:
		*rowModResult = 0;
		*colModResult = -1;
		break;
	case MOVE_RIGHT:
		*rowModResult = 0;
		*colModResult = 1;
		break;
	case MOVE_UP:
		*rowModResult = -1;
		*colModResult = 0;
		break;
	case MOVE_DOWN:
		*rowModResult = 1;
		*colModResult = 0;
		break;
	
		
		
	default:
		break;
	}
}


typedef struct {
	bool** visited;
	int numRows;
	int numCols;
} BoardData;

typedef BoardData* Board;


Board Board_createAndAllocate(const int numRows, const int numCols) {
	Board toReturn = calloc(1, sizeof(BoardData));
	toReturn->numRows = numRows;
	toReturn->numCols = numCols;
	
	toReturn->visited = calloc(toReturn->numRows, sizeof(bool*));
	for (int i = 0; i < toReturn->numRows; ++i) {
		arraySet(toReturn->visited, i, calloc(toReturn->numCols, sizeof(bool)));
	}
	
	return toReturn;
}

void Board_visit(Board board, int row, int col) {
	matrixSet(board->visited, row, col, 1);
}

bool Board_wasVisitedAt(Board board, int row, int col) {
	if (row < 0 || row > (board->numRows - 1) || col < 0 || col > (board->numCols - 1)) {
		return false;
	}
	
	return matrixGet(board->visited, row, col);
}

int Board_spacesVisited(Board board) {
	int toReturn = 0;
	for (int row = 0; row < board->numRows; ++row) {
		for (int col = 0; col < board->numCols; ++col) {
			toReturn += Board_wasVisitedAt(board, row, col);
		}
	}
	return toReturn;
}

int Board_totalSpaces(Board board) {
	return board->numRows * board->numCols;
}

bool Board_allVisited(Board board) {
	return Board_spacesVisited(board) == Board_totalSpaces(board);
}

void Board_free(Board board) {
	for (int i = 0; i < board->numRows; ++i) {
		free(arrayGet(board->visited, i));
	}
	
	free(board->visited);
	free(board);
}



int numMoves(Move moves) {
	int toReturn = 0;
	while (moves) {
		if (moves & 1) {
			++toReturn;
		}
		moves >>= 1;
	}
	return toReturn;
}

Move validMoves(Board board, int row, int column) {
	Move toReturn = MOVE_NONE;
	
	if (row > 0 && !Board_wasVisitedAt(board, row - 1, column)) {
		toReturn &= MOVE_UP;
	}
	if (column > 0 && !Board_wasVisitedAt(board, row, column - 1)) {
		toReturn &= MOVE_LEFT;
	}
	if (row < (board->numRows - 1) && !Board_wasVisitedAt(board, row + 1, column)) {
		toReturn &= MOVE_DOWN;
	}
	if (column < (board->numCols - 1) && !Board_wasVisitedAt(board, row, column + 1)) {
		toReturn &= MOVE_RIGHT;
	}
	
	return toReturn;
	
}

int logBase2(int n) {
	int toReturn = -1;
	while (n) {
		n >>= 1;
		++toReturn;
	}
	return toReturn;
}

// readEndOfPipe is -1 if it's closed
int oneRecursiveLayer(Board board, int row, int col, int writeEndOfPipe, int readEndOfPipe) {
	Move validMovesHere = validMoves(board, row, col);
	int amountValid = numMoves(validMovesHere);
	int rowModFromMove, colModFromMove;
	
	
	switch (amountValid) {
		case 0: 
			exit(Board_spacesVisited(board));
			break;
		case 1:
			Move_getAdditionsToRowCol(validMovesHere, &rowModFromMove, &colModFromMove);
			int result = oneRecursiveLayer(board, row + rowModFromMove, col + colModFromMove, writeEndOfPipe, -1);
			exit(result);
		default:
			pid_t* pids = calloc(4, sizeof(pid_t));
			
			forEachMove(currentMove) {	
				if (validMovesHere & currentMove) {
					pid_t p = fork();
					arraySet(pids, logBase2(currentMove), p);
					if (p == 0) {
						if (readEndOfPipe >= 0) {
							close(readEndOfPipe);
						}
						
						Move_getAdditionsToRowCol(validMovesHere, &rowModFromMove, &colModFromMove);
						int result = oneRecursiveLayer(board, row + rowModFromMove, col + colModFromMove, writeEndOfPipe, -1);
						exit(result);
					}
				}
			}
			forEachMove(currentMove) {
				if (arrayGet(pids, logBase2(currentMove))) {
					int status;
					
					
					
					waitpid(arrayGet(pids, logBase2(currentMove)), &status, WNOHANG);
				}
			}
		
	}
	
	if (!amountValid) {
		exit(Board_spacesVisited(board));
	}
	
	
	
	// int rowMod, colMod;
	
	
	// Move_getAdditionsToRowCol()
	
}


int actualProgram(int numRows, int numCols, int row, int col) {
	Board board = Board_createAndAllocate(numRows, numCols);
	int* mainPipe = calloc(2, sizeof(int));
	
	Board_visit(board, row, col);
	
	pipe(mainPipe);
	int readEnd = arrayGet(mainPipe, 0);
	int writeEnd = arrayGet(mainPipe, 1);
	
	Move possibleMovesFromStart = validMoves(board, row, col);
	
	 {
		
	}
	
	
	
	free(mainPipe);
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
	
	if (numRows <= 1 || numCols <= 1) {
		fprintf(stderr, "Error: m or n is <= 1\n");
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
	printf("%d %d %d %d %d\n", numRows, numCols, startingRow, startingColumn, numMoves(10));
	
	forEachMove(currentMove) {
		printf("%d\n", currentMove);
	}
	
	int i[3];
	arraySet(i, 2, 3);
	printf("%d\n", arrayGet(i, 2));
	
	printf("%d %d %d %d\n", logBase2(MOVE_UP), logBase2(MOVE_RIGHT), logBase2(MOVE_DOWN), logBase2(MOVE_LEFT));
	
	
	#endif
	
	actualProgram(numRows, numCols, startingRow, startingColumn);
	
	return 0;
}



