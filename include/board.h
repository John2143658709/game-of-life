#ifndef board_H
#define board_H

#include <stdlib.h>
#include <curses.h>

#include "chunk.h"

#define DEFAULTWIDTH 8
#define BOARDNAMELENGTH 64

struct board{
	scp first;
	scp curChunk;
	int size;
	int iterations;
	int untilAutoGC;
	char name[BOARDNAMELENGTH];
};

#define forEachChunk(b, p) for(scp p = b->first; p != NULL; p = p->next)

//Mutate the board by one time unit.
//TODO: Make memory more efficient by calculating a better order
//TODO: Decide how often to run the GC
void iterateBoard(struct board *board);
void iterateBoardTimes(struct board *board, int times);
//This will generate new chunks if they will be needed in the next iteration
//Returns the number of chunks generated.
int generateNewChunks(struct board *b);
//Adds a new chunk to the board
void addChunk(struct board *b, int x, int y, const bval *mem);
//Removes a chunk from the board
void removeChunk(struct board *b, scp c);
//Creates a new empty board. Must call freeBoard() to release the pointer.
struct board *createBoard(const char *name);
//Creates the empty board data
//You must use this method if you do not load the board data from io.
void initializeBoard(struct board *board);
void freeBoard(struct board *board);
//Set the name of a board
void setBoardName(struct board *b, const char *name);

//Move the board by x, y units right/down. Negative for left/up
//returns 0 for ok, -1 for failure
int moveBoard(struct board *b, int x, int y);
//Move the board to the position of x, y. O(getChunkAtPos())
//returns 0 for ok, -1 for failure
int setBoard(struct board *b, int x, int y);
//Get the chunk at the specified point. NULL on fail
scp getChunkAtPos(const struct board *b, int x, int y);
//Resize the board to the new provided size.
//Returns -1 if it failed TODO: Exit program on -1
int resizeBoard(struct board *b, int new);
//Same as resizeBoard, but will round up to the next power of two
int resizeBoardMin(struct board *b, int min);
//Draws the current chunk as selected by the board. Exits if there is no chunk selected
//chunk.h: drawChunk does most of the heavy lifting, with drawboard just displaying info
//TODO: return or handle error
void drawBoard(const struct board *b);

//Remove all chunks that are 0, possibly compressing the buffer
//Returns the number of chunks removed
//This method may be referenced as "The GC"
void collectGarbage(struct board *b);

//DO NOT USE THIS FUNCTION PLEASE, THE VALUE IS NOW STORED IN THE CHUNK STRUCT
//Returns the chunks position from the end of the board
//Undefined behavior if c is not of b
//b and c are pointers
#define chunkPos(b, c) ((int) (((c) - (b)->chunks) / sizeof(struct chunk)))

#endif
