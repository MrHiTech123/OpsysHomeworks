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
#define arraySetMove(arr, move, value) arraySet(arr, logBase2(move), value)
#define arrayGetMove(arr, move) arrayGet(arr, logBase2(move))
#define matrixSet(mat, row, col, value) *(*(mat + row) + col) = value
#define matrixGet(mat, row, col) *(*(mat + row) + col)
#define AMOUNT_DIRECTIONS 4


#ifdef PARALLEL
#define PARALLEL_OR_NO_PARALLEL_MODE "PARALLEL"
#else
#define PARALLEL_OR_NO_PARALLEL_MODE "NO_PARALLEL"
#endif


void settings() {
	// struct rlimit rl;
	// getrlimit(RLIMIT_NPROC, &rl);
	// rl.rlim_cur = 32;
	// setrlimit(RLIMIT_NPROC, &rl);
	setvbuf(stdout, NULL, _IONBF, 0);
}

typedef enum {
	MOVE_UP = 1,
	MOVE_RIGHT = 2,
	MOVE_DOWN = 4,
	MOVE_LEFT = 8
} Move;

typedef enum {
	TOURTYPE_OPEN,
	TOURTYPE_CLOSED
} TourType;

const char* TourType_getName(TourType tourType) {
	switch (tourType) {
		case TOURTYPE_OPEN:
			return "n open";
		case TOURTYPE_CLOSED:
			return " closed";
	}
	return NULL;
}


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
	int length;
	int endX;
	int endY;
} Tour;

Tour* Tour_createAndAllocate(int length, int endX, int endY) {
	Tour* toReturn = calloc(1, sizeof(Tour));
	toReturn->length = length;
	toReturn->endX = endX;
	toReturn->endY = endY;
	return toReturn;
}

typedef struct {
	bool** visited;
	int numRows;
	int numCols;
	int numVisited;
} BoardData;

typedef BoardData* Board;


Board Board_createAndAllocate(const int numRows, const int numCols) {
	Board toReturn = calloc(1, sizeof(BoardData));
	toReturn->numRows = numRows;
	toReturn->numCols = numCols;
	toReturn->numVisited = 0;
	
	toReturn->visited = calloc(toReturn->numRows, sizeof(bool*));
	for (int i = 0; i < toReturn->numRows; ++i) {
		arraySet(toReturn->visited, i, calloc(toReturn->numCols, sizeof(bool)));
	}
	
	return toReturn;
}

void Board_visit(Board board, int row, int col) {
	matrixSet(board->visited, row, col, 1);
	++board->numVisited;
}

bool Board_wasVisitedAt(Board board, int row, int col) {
	if (row < 0 || row > (board->numRows - 1) || col < 0 || col > (board->numCols - 1)) {
		return false;
	}
	
	return matrixGet(board->visited, row, col);
}

int Board_totalSpaces(Board board) {
	return board->numRows * board->numCols;
}

bool Board_allVisited(Board board) {
	return board->numVisited == Board_totalSpaces(board);
}

void Board_print(Board board) {
	printf("START BOARD\n");
	for (int row = 0; row < board->numRows; ++row) {
		for (int col = 0; col < board->numCols; ++col) {
			printf("%d ", Board_wasVisitedAt(board, row, col));
		}
		printf("\n");
	}
	printf("END BOARD\n");
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

int forkAndFlush() {
	fflush(stdout);
	return fork();
}

Move validMoves(Board board, int row, int column) {
	Move toReturn = 0;
	
	if (row > 0 && !Board_wasVisitedAt(board, row - 1, column)) {
		toReturn |= MOVE_UP;
	}
	if (column > 0 && !Board_wasVisitedAt(board, row, column - 1)) {
		toReturn |= MOVE_LEFT;
	}
	if (row < (board->numRows - 1) && !Board_wasVisitedAt(board, row + 1, column)) {
		toReturn |= MOVE_DOWN;
	}
	if (column < (board->numCols - 1) && !Board_wasVisitedAt(board, row, column + 1)) {
		toReturn |= MOVE_RIGHT;
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

int manhattanDistance(int x1, int y1, int x2, int y2) {
	return abs(x2 - x1) + abs(y2 - y1);
}

TourType getTypeOfTour(int startX, int startY, int endX, int endY) {
	if (manhattanDistance(startX, startY, endX, endY) <= 1) {
		return TOURTYPE_CLOSED;
	}
	else {
		return TOURTYPE_OPEN;
	}
}


// Define a templated macro here just cuz
#define MAKE_HAS_NON_ZEROES_FUNCTION(name, class) bool name(class* arr, int length) {for (int i = 0; i < length; ++i) {if (arrayGet(arr, i)) {return true;}} return false;}

MAKE_HAS_NON_ZEROES_FUNCTION(hasNonZeroes__pid_t, pid_t);


// readEndOfPipe is -1 if it's closed
void oneRecursiveLayer(Board board, int row, int col, int writeEndOfPipe, int readEndOfPipe, int startRow, int startCol, int depthAllowed) {
	#ifdef DEBUG
	printf("oneRecuriveLayer(%p %d %d %d %d %d %d)\n", board, row, col, writeEndOfPipe, readEndOfPipe, startRow, startCol);
	#endif
	
	if (!depthAllowed) {
		return;
	}
	
	Move validMovesHere = validMoves(board, row, col);
	int amountValid = numMoves(validMovesHere);
	int rowModFromMove, colModFromMove;
	bool isFirstLayer = readEndOfPipe >= 0;
	
	
	
	Board_visit(board, row, col);
	int length;
	
	switch (amountValid) {
		case 0:
			length = board->numVisited - 1;
			TourType type = getTypeOfTour(startRow, startCol, row, col);
			if (length == Board_totalSpaces(board)) {
				printf("*** Found a%s Wazir tour at move #%d; notifying top-level parent\n", TourType_getName(type), length);
				write(writeEndOfPipe, &type, sizeof(TourType));
			}
			else {
				printf("*** Dead end at move #%d\n", length);
			}
			close(writeEndOfPipe);
			exit(length);
		case 1:
			Move_getAdditionsToRowCol(validMovesHere, &rowModFromMove, &colModFromMove);
			oneRecursiveLayer(board, row + rowModFromMove, col + colModFromMove, writeEndOfPipe, -1, startRow, startCol, depthAllowed - 1);
		default:
			pid_t* pids = calloc(AMOUNT_DIRECTIONS, sizeof(pid_t));
			
			
			printf("*** Detected %d possible moves after move #%d; creating %d child processes\n", amountValid, board->numVisited - 1, amountValid);
			
			#ifndef PARALLEL
			int* exitStatuses = calloc(AMOUNT_DIRECTIONS, sizeof(int));
			#endif
			
			forEachMove(currentMove) {	
				if (validMovesHere & currentMove) {
					pid_t p = forkAndFlush();
					arraySetMove(pids, currentMove, p);
					if (p == 0) {
						if (isFirstLayer) {
							close(readEndOfPipe);
						}
						
						
						Move_getAdditionsToRowCol(currentMove, &rowModFromMove, &colModFromMove);
						oneRecursiveLayer(board, row + rowModFromMove, col + colModFromMove, writeEndOfPipe, -1, startRow, startCol, depthAllowed - 1);
					}
					int nothing;
					#ifndef PARALLEL
					waitpid(p, &nothing, 0);
					arraySetMove(exitStatuses, currentMove, nothing);
					#endif
				
				}
			}
			
			close(writeEndOfPipe);
			
			int longestRouteLength = 0;
			TourType typeBuffer;
			while (hasNonZeroes__pid_t(pids, AMOUNT_DIRECTIONS)) {
				forEachMove(currentMove) {
					pid_t p = arrayGetMove(pids, currentMove);
					if (p) {
						int status;
						
						pid_t childPid = waitpid(p, &status, WNOHANG);
						
						#ifndef PARALLEL
						status = arrayGetMove(exitStatuses, currentMove);
						#endif
						
						if (childPid && WIFEXITED(status)) {
							status = WEXITSTATUS(status);
							if (status > longestRouteLength) {
								longestRouteLength = status;
							}
							arraySetMove(pids, currentMove, 0);
						}
						else if (childPid && WIFSIGNALED(status)) {
							printf("SIGNALED: %d\n", status);
						}
						
					}
					
				}
			}
			
			if (isFirstLayer) {
				TourType typeBuffer;
				int* amountOfToursOfType = calloc(2, sizeof(int));
				while (read(readEndOfPipe, &typeBuffer, sizeof(TourType))) {
					++arrayGet(amountOfToursOfType, typeBuffer);
				}
				
				
				if (longestRouteLength == Board_totalSpaces(board)) {
					printf("*** Search complete; found %d open tours and %d closed tours\n", arrayGet(amountOfToursOfType, TOURTYPE_OPEN), arrayGet(amountOfToursOfType, TOURTYPE_CLOSED));
				}
				else {
					printf("*** Search complete; best solution(s) visited %d squares out of %d\n", longestRouteLength, Board_totalSpaces(board));
				}
				
				free(amountOfToursOfType);
			}
			
			#ifndef PARALLEL
			free(exitStatuses);
			#endif
			
			free(pids);
			exit(longestRouteLength);
	}
	
	if (!amountValid) {
		exit(board->numVisited);
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
	
	printf("*** Solving the Wazir tour problem (%dx%d board)\n", numRows, numCols);
	printf("*** %s mode\n", PARALLEL_OR_NO_PARALLEL_MODE);
	printf("*** Start at row %d and column %d (move #1)\n", row, col);
	
	
	oneRecursiveLayer(board, row, col, writeEnd, readEnd, row, col, Board_totalSpaces(board));
	
	
	
	free(mainPipe);
	Board_free(board);
	return 0;
}

int main(int argc, const char** argv)
{
	settings();
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
	
	int a,b;
	Move_getAdditionsToRowCol(MOVE_UP, &a, &b);
	printf("%d %d\n", a, b);
	
	#endif
	
	actualProgram(numRows, numCols, startingRow, startingColumn);
	
	return 0;
}



