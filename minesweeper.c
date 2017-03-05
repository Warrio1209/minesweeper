/****************************************************************************
* COSC2138/CPT 220 - Programming Principles 2A
* Study Period 2  2015 Assignment #1 - minesweeper program
* Full Name        : EDIT HERE
* Student Number   : EDIT HERE
* Course Code      : EDIT HERE
* Start up code provided by the CTeach Team
****************************************************************************/

#include "minesweeper.h"

/****************************************************************************
* Function main() is the entry point for the program.
****************************************************************************/
int main(void)
{
   /* Stores all hidden data about the minefield. */
   char minefield[MAX_GRID][MAX_GRID];
   /* A version of the minefield that only stores known information. */
   char displayGrid[MAX_GRID][MAX_GRID];
   /* Number of cells in each row and column used. */
   unsigned size = 0;
   char type;
   char row;
   int col;
   int victoryCheck = 0;
   init(minefield,displayGrid);
   size = getSize();
   placeMines(minefield,size);
   int i = 0;
   do{
      displayMinefield(displayGrid,size);
      type = guessType();
      guessSquare(&row,&col,size);
   }while(processGuess(minefield,displayGrid,size,type,row,col) != FINISHED);
   i = 0;
   while(i<size && victoryCheck == 0)
   {
      int j = 0;
      while(j<size && victoryCheck == 0)
      {
         if(minefield[i][j] == MINE)
         {
            if(displayGrid[i][j]!= FLAG)
            {
               victoryCheck = 1;
            }
         }
         j++;
      }
      i++;
   }
   if(victoryCheck == 0)
   {
      printf("%s\n","Congratulations, you WIN!");
   }
   else
   {
       printf("%s\n%s\n","BANG!","You're DEAD.");
   }
   i = 0;
   while(i<size)
   {
      printf("%s%i","   ",++i);
   }
   i = 0;
   while(i<size)
   {
      int j = 0;
      printHorizontalDivider(size);
      printf("%c%s",('a'+i)," |");
      while(j<size)
      {
         if(displayGrid[i][j] == FLAG)
         {
            if(minefield[i][j] != MINE)
            {
               printf("%c%c%s",' ',BAD_MINE," |");
            }
            else
            {
               printf("%c%c%s",' ',MINE," |");
            }
         }
         else
         {
            printf("%c%c%s",' ',minefield[i][j]," |");
         }
         j++;
      }
      i++;
   }
   printHorizontalDivider(size);
   getchar();
   return EXIT_SUCCESS;
}


/****************************************************************************
* Function init() initialises the "minefield" array with BLANK characters,
* and the "displayGrid" array with UNKNOWN characters. These constants (and
* others) are obtained from the header file.
****************************************************************************/
void init(char minefield[MAX_GRID][MAX_GRID], 
          char displayGrid[MAX_GRID][MAX_GRID])
{
   int i = 0;
   int j = 0;
   while(i<MAX_GRID)
   {
      j = 0;
      while(j<MAX_GRID)
      {
         minefield[i][j] = BLANK;
         displayGrid[i][j] = UNKNOWN;
         j++;
      }
      i++;
   }
}


/****************************************************************************
* Function getSize() prompts the user for the size of the minefield in the
* range MIN_GRID to MAX_GRID. Example:
* ------------------------------------------------------
* Enter minefield size (2-16): 6
* You have chosen a minefield with 6 rows and 6 columns.
* ------------------------------------------------------
****************************************************************************/
unsigned getSize()
{
   unsigned size;
   do{  
   printf("%s%i-%i%s","Enter minefield size (",MIN_GRID,MAX_GRID,"): ");
   scanf("%u",&size);
   readRestOfLine();
   }while(size>MAX_GRID || size<MIN_GRID);
   printf("%s%i%s%i%s\n","You have chosen a minefield with ",size," rows and ", size," columns.");
   return size;
}


/****************************************************************************
* Function placeMines() places a number of mines in the minefield. The 
* number of mines placed is equal to the number of squares in the minefield
* multiplied by the MINE_DENSITY constant.
* For example, a grid with size 6 has 36 squares.
* 36 x 0.16 = 5.76 or approximately 6 mines. (Result is always rounded up.)
* The mines are placed at random positions and each mine must be in a 
* different square. Use the MINE constant to mark mine squares. Use numbers
* from 1-8 to mark other squares that have that number of mines adjacent to
* them. Squares that don't have adjacent mines are marked as BLANK.
* Here's an example:
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 1]
* a | M | M | 1 |   |   |   |
*   +---+---+---+---+---+---+
* b | 2 | 2 | 1 |   |   |   |
*   +---+---+---+---+---+---+
* c |   |   | 1 | 1 | 1 |   |
*   +---+---+---+---+---+---+
* d |   |   | 2 | M | 2 |   |
*   +---+---+---+---+---+---+
* e |   |   | 3 | M | 4 | 1 |
*   +---+---+---+---+---+---+
* f |   |   | 2 | M | M | 1 |
*   +---+---+---+---+---+---+
* ---------------------------
****************************************************************************/
void placeMines(char minefield[MAX_GRID][MAX_GRID], unsigned size)
{
   unsigned numMines = (unsigned)(size*size*MINE_DENSITY)+1;
   int i = 0;
   int xPosition;
   int yPosition;
   while(i<numMines)
   {
      do{
         srand(time(NULL));
         xPosition = rand()%size;
         srand(time(NULL));
         yPosition = rand()%size;
      }while(minefield[xPosition][yPosition] == MINE);
      minefield[xPosition][yPosition] = MINE;
      setProximityIndicators(minefield,xPosition+1,yPosition+1);
      setProximityIndicators(minefield,xPosition+1,yPosition);
      setProximityIndicators(minefield,xPosition+1,yPosition-1);
      setProximityIndicators(minefield,xPosition,yPosition+1);
      setProximityIndicators(minefield,xPosition,yPosition-1);
      setProximityIndicators(minefield,xPosition-1,yPosition+1);
      setProximityIndicators(minefield,xPosition-1,yPosition);
      setProximityIndicators(minefield,xPosition-1,yPosition-1);
      i++;
   }
}


/****************************************************************************
* Function displayMinefield() shows the user an ascii-art drawing of the
* minefield with only known information displayed. Initially, nothing is
* known about the minefield, so all squares are hidden. Example:
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 2]
* a | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* b | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* c | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* d | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* e | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* f | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* You have flagged 0/6 mines.
* ---------------------------
****************************************************************************/
void displayMinefield(char displayGrid[MAX_GRID][MAX_GRID], unsigned size)
{
     int i = 0;
     int j = 0;
     int flagCount = 0;
     printf("  %s%i","  ",++i);
     while(i<size)
     {
        printf("%s%i","   ",++i);
     }
     i = 0;
     while(i<size)
     {
        printHorizontalDivider(size);
        printf("%c%s",('a'+i)," |");
        j = 0;
        while(j<size)
        {
           printf("%c%c%s",' ',displayGrid[i][j]," |");
           if(displayGrid[i][j] == FLAG)
           {
              flagCount++;
           }
           j++;
        }
        i++;
     }
     printHorizontalDivider(size);
     printf("%s%i%c%i%s","You have flagged ", flagCount, '/',(int)(size*size*MINE_DENSITY+1)," mines.");
}


/****************************************************************************
* Function guessType() prompts the user for a single character representing
* one of three options. These options are used in the processGuess() 
* function. Example:
* --------------------------------------------------
* Enter guess type ((f)lag, (u)ncover or (s)weep): u
* You have selected "uncover".
* --------------------------------------------------
****************************************************************************/
char guessType()
{
   while(1)
   {
      char guessType;
      printf("\n%s","Enter guess type ((f)lag, (u)ncover or (s)weep): ");
      guessType = fgetc(stdin);
      readRestOfLine();
      if(tolower(guessType) == FLAG_SQUARE)
      {
         printf("You have selected \"flag\".");
         return FLAG_SQUARE;
      }
      else if(tolower(guessType) == UNCOVER_SQUARE)
      {
         printf("You have selected \"uncover\".");
         return UNCOVER_SQUARE;
      }
      else if(tolower(guessType) == SWEEP_SQUARE)
      {
         printf("You have selected \"sweep\".");
         return SWEEP_SQUARE;
      }
   }
}


/****************************************************************************
* Function guessSquare() prompts the user for the reference of a square 
* in the minefield. The row and column components are extracted and made
* available to the calling function separately. Example:
* ------------------------------------------
* Enter square (a1-f6): f1
* Your have selected row "f" and column "1".
* ------------------------------------------
****************************************************************************/
void guessSquare(char* row, unsigned* col, unsigned size)
{
     char temp[3];
     printf("%s%c%i%s","Enter square (a1-",'a'+size-1,size,"): ");
     fgets(temp,3,stdin);
     readRestOfLine();
     *row = temp[0];
     *col = temp[1]-'0'-1;
}

/****************************************************************************
* Function processGuess() manipulates the displayGrid variable and determines
* if the Minesweeper game is over or not. It processes the three types of
* actions called "(f)lag square", "(u)ncover square", and "(s)weep square".
* Examples:
*
* Consider the following minefield ("minefield" variable):
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 3]
* a | M | M | 1 |   |   |   |
*   +---+---+---+---+---+---+
* b | 2 | 2 | 1 |   |   |   |
*   +---+---+---+---+---+---+
* c |   |   | 1 | 1 | 1 |   |
*   +---+---+---+---+---+---+
* d |   |   | 2 | M | 2 |   |
*   +---+---+---+---+---+---+
* e |   |   | 3 | M | 4 | 1 |
*   +---+---+---+---+---+---+
* f |   |   | 2 | M | M | 1 |
*   +---+---+---+---+---+---+
* ---------------------------
* Initially, the minefield is entirely hidden ("displayGrid" variable):
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 4]
* a | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* b | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* c | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* d | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* e | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* f | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* ---------------------------
* If the user decides to "uncover square f1", the following can happen:
* - If a mine is accidentally uncovered, the game is lost.
* - If a numbered square is uncovered, this is marked in the "displayGrid"
*   variable.
* - If a blank square is uncovered, this is marked in the "displayGrid" 
*   variable and all adjacent blank squares above, below, left, or right
*   are also marked.
* In this example, square f1 is blank, so the "displayGrid" variable is
* updated to look like this:
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 5]
* a | ? | ? | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* b | 2 | 2 | 1 | ? | ? | ? |
*   +---+---+---+---+---+---+
* c |   |   | 1 | ? | ? | ? |
*   +---+---+---+---+---+---+
* d |   |   | 2 | ? | ? | ? |
*   +---+---+---+---+---+---+
* e |   |   | 3 | ? | ? | ? |
*   +---+---+---+---+---+---+
* f |   |   | 2 | ? | ? | ? |
*   +---+---+---+---+---+---+
* ---------------------------
* From the above information, the user can already deduce where five of the
* six mines are located. They are in squares a1, a2, d4, e4, and f4. So, the
* user would next want to "flag square a1" and the other four squares. 
* The user can only flag squares on unknown squares. So, our "displayGrid"
* variable now looks like this:
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 6]
* a | F | F | ? | ? | ? | ? |
*   +---+---+---+---+---+---+
* b | 2 | 2 | 1 | ? | ? | ? |
*   +---+---+---+---+---+---+
* c |   |   | 1 | ? | ? | ? |
*   +---+---+---+---+---+---+
* d |   |   | 2 | F | ? | ? |
*   +---+---+---+---+---+---+
* e |   |   | 3 | F | ? | ? |
*   +---+---+---+---+---+---+
* f |   |   | 2 | F | ? | ? |
*   +---+---+---+---+---+---+
* ---------------------------
* The user can now deduce that there are no mines in squares a3, a4, b4, and
* c4. The user can now uncover these squares one at a time if he or she
* wishes. However, this is somewhat inefficient. Alternatively, the user may
* want to "sweep" around a numbered square that is considered safe because 
* the adjacent mines have been (hopefully) correctly flagged.
* So the user can "sweep square b3". This effectively executes the "uncover"
* command on the surrounding 8 squares (but only the ones that are marked as
* UNKNOWN). After performing the "sweep square b3" command, the "displayGrid"
* variable looks like this:
* ---------------------------
*     1   2   3   4   5   6
*   +---+---+---+---+---+---+ [ EXAMPLE 7]
* a | F | F | 1 |   |   |   |
*   +---+---+---+---+---+---+
* b | 2 | 2 | 1 |   |   |   |
*   +---+---+---+---+---+---+
* c |   |   | 1 | 1 | 1 |   |
*   +---+---+---+---+---+---+
* d |   |   | 2 | F | 2 |   |
*   +---+---+---+---+---+---+
* e |   |   | 3 | F | 4 | 1 |
*   +---+---+---+---+---+---+
* f |   |   | 2 | F | ? | ? |
*   +---+---+---+---+---+---+
* ---------------------------
* The user will need to guess whether the final mine is in square f5 or f6
* and flag the square. When six squares are flagged, the user guesses are 
* verified. If any guess is incorrect, then the user loses and the game is
* over. Otherwise the user wins the game and the game is over. The 
* displayMinefield() function is called one last time with bad guesses marked
* as BAD_MINE.
*
* This function is a large one, and it is recommended that you further
* modularise it. Leave this one until you have implemented the other six
* major functions. Additionally, you should implement the three commands in
* this order: flag, uncover, sweep.
****************************************************************************/
int processGuess(char minefield[MAX_GRID][MAX_GRID], 
                 char displayGrid[MAX_GRID][MAX_GRID],
                 unsigned size, char type, char row, unsigned col)
{
   int rowNum = row-'0'-'0'-1;
   if(type == FLAG_SQUARE)
   {
      int i = 0;
      int flagCount = 0;
      flagSquare(displayGrid,rowNum,col);
      while(i<size)
      {
         int j = 0;
         while(j<size)
         {
            if(displayGrid[i][j] == FLAG)
            flagCount++;
            j++;
         }
         i++;
      }
      if(flagCount<(unsigned)(size*size*MINE_DENSITY)+1)
      {
         return CONTINUE;
      }
      else
      {
         return FINISHED;
      }
   }
   else if(type == UNCOVER_SQUARE)
   {
      if(uncoverSquare(minefield,displayGrid,rowNum,col) == FINISHED)
      {
         
         return FINISHED;
      }
      else
      {
         return CONTINUE;
      }
   }
   else if(type == SWEEP_SQUARE)
   {
      if(sweepSquare(minefield,displayGrid,rowNum,col) == FINISHED)
      {
         return FINISHED;
      }
      else
      {
         return CONTINUE;
      }
   }
   return CONTINUE;
}


/****************************************************************************
* Function readRestOfLine() is used for buffer clearing. Source: 
* https://inside.cs.rmit.edu.au/~sdb/teaching/C-Prog/CourseDocuments/
* FrequentlyAskedQuestions/
****************************************************************************/
void readRestOfLine()
{
   int c;

   /* Read until the end of the line or end-of-file. */   
   while ((c = fgetc(stdin)) != '\n' && c != EOF)
      ;

   /* Clear the error and end-of-file flags. */
   clearerr(stdin);
}

void setProximityIndicators(char minefield[MAX_GRID][MAX_GRID],int xPosition,int yPosition)
{
   if(xPosition<MAX_GRID && yPosition<MAX_GRID && xPosition>=0 && yPosition>=0)
   {
      if(minefield[xPosition][yPosition]!= MINE)
      {
         if(minefield[xPosition][yPosition] == BLANK)
         {
            minefield[xPosition][yPosition] = '0';
         }
         minefield[xPosition][yPosition]++;                                
      }
   }
}
void printHorizontalDivider(int size)
{
   int i = 0;
   printf("\n%s","  ");
   while(i<size)
   {
      printf("+---");
      i++;
   }
   printf("%s\n","+");
}
void flagSquare(char displayGrid[MAX_GRID][MAX_GRID],int row,int col)
{
   displayGrid[row][col] = FLAG;
}

int uncoverSquare(char minefield[MAX_GRID][MAX_GRID],char displayGrid[MAX_GRID][MAX_GRID],int row,int col)
{  
 
   if(row>=0 && row<MAX_GRID && col>=0 && col<MAX_GRID)
   {
      if(displayGrid[row][col]!= UNKNOWN)
      {
         return CONTINUE;
      }
      else if(minefield[row][col] == BLANK)
      {
         displayGrid[row][col] = BLANK;
         sweepSquare(minefield,displayGrid,row,col);   
      }
      else if(minefield[row][col] == MINE)
      {
           return FINISHED;
      }
      else
      {
         displayGrid[row][col] = minefield[row][col];
         return CONTINUE;
      }
   }
   return CONTINUE;   
}
int sweepSquare(char minefield[MAX_GRID][MAX_GRID],char displayGrid[MAX_GRID][MAX_GRID],int row,int col)
{
   if(uncoverSquare(minefield,displayGrid,row+1,col+1)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row+1,col)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row+1,col-1)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row,col+1)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row,col-1)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row-1,col+1)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row-1,col)== FINISHED)
   {
      return FINISHED;
   }
   if(uncoverSquare(minefield,displayGrid,row-1,col-1)== FINISHED)
   {
      return FINISHED;
   }
   return CONTINUE;
}
