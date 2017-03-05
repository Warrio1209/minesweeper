/****************************************************************************
* COSC2138/CPT 220 - Programming Principles 2A
* Study Period 2  2015 Assignment #1 - minesweeper program
* Full Name        : EDIT HERE
* Student Number   : EDIT HERE
* Course Code      : EDIT HERE
* Start up code provided by the CTeach Team
****************************************************************************/

/* Header files. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Constants. */
#define MINE_DENSITY 0.16
#define MIN_GRID 2
#define MAX_GRID 16
#define FLAG 'F'
#define MINE 'M'
#define BAD_MINE 'X'
#define UNKNOWN '?'
#define BLANK ' '
#define FLAG_SQUARE 'f'
#define UNCOVER_SQUARE 'u'
#define SWEEP_SQUARE 's'
#define FINISHED 0
#define CONTINUE 1
/* Function prototypes. */
void init(char minefield[MAX_GRID][MAX_GRID], 
          char displayGrid[MAX_GRID][MAX_GRID]);
unsigned getSize();
void placeMines(char minefield[MAX_GRID][MAX_GRID], unsigned size);
void displayMinefield(char displayGrid[MAX_GRID][MAX_GRID], unsigned size);
char guessType();
void guessSquare(char* row, unsigned* col, unsigned size);
int processGuess(char minefield[MAX_GRID][MAX_GRID], 
                 char displayGrid[MAX_GRID][MAX_GRID],
                 unsigned size, char type, char row, unsigned col);
void readRestOfLine();
void setProximityIndicators(char minefield[MAX_GRID][MAX_GRID],int xPosition,int yPosition);
void printHorizontalDivider(int size);
void flagSquare(char displayGrid[MAX_GRID][MAX_GRID],int row,int col);
int uncoverSquare(char minefield[MAX_GRID][MAX_GRID],char displayGrid[MAX_GRID][MAX_GRID],int row,int col);
int sweepSquare(char minefield[MAX_GRID][MAX_GRID],char displayGrid[MAX_GRID][MAX_GRID],int row,int col);
