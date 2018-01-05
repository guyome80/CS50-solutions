/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// empty tile informations
int emptyTile;
int emptyTileHor;
int emptyTileVer;

// tile informations
int tileNum;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swap(int*, int*);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        // usleep(500000);
        usleep(20000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // filling array with each tile number.
    int tileNum = d * d - 1;
    while (tileNum >= 0)
    {
        for (int i = 0; i < d; i++)
        {
          for (int j = 0; j < d; j++)
            {
                board[i][j] = tileNum;
                tileNum--;
            }
        }
    }
    
    //the variable = emptyTile.
    emptyTile = 0;
    board[d - 1][d - 1] = emptyTile;
    emptyTileHor = d - 1;
    emptyTileVer = d - 1;
    
    //swaps the 1 and 2 tile with a temporary integer if size of board is even.
    if (d == 4 || d == 6 || d == 8)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] < 10)
            {
                printf(" %i ", board[i][j]);
            }
            else
                printf("%i ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // search the tile number in the board
    int tileHorPos;
    int tileVerPos;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // search the position on the board of the tile
            if (board[i][j] == tile)
            { 
                tileHorPos = i;
                tileVerPos = j;
                
                // checks if emptyTile is next to the tile and so can move horizontally
                if ( emptyTileHor == tileHorPos && tileVerPos - 1 >= -1 && tileVerPos + 1 <= d && (tileVerPos == emptyTileVer - 1 ||  tileVerPos == emptyTileVer + 1))
                {
                    // swap empty tile and chosen tile to move
                    
                    int temp = board[emptyTileHor][emptyTileVer];
                    board[emptyTileHor][emptyTileVer] = board[tileHorPos][tileVerPos];
                    board[tileHorPos][tileVerPos] = temp;
                    swap(&emptyTileVer, &tileVerPos);
                    swap(&emptyTileHor, &tileHorPos);
                    return true;
                }
                //checks if emptyTile is next to the tile and so can move vertically
                else if ( emptyTileVer == tileVerPos && tileHorPos - 1 >= -1 && tileHorPos + 1 <= d && (tileHorPos == emptyTileHor - 1 ||  tileHorPos == emptyTileHor + 1))
                {
                    // swap empty tile and chosen tile to move
                    int temp = board[emptyTileHor][emptyTileVer];
                    board[emptyTileHor][emptyTileVer] = board[tileHorPos][tileVerPos];
                    board[tileHorPos][tileVerPos] = temp;
                    swap(&emptyTileHor, &tileHorPos);
                    swap(&emptyTileVer, &tileVerPos);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //checks whether the order of the tiles
    int boardPrevious = board[0][0];
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        { 
            if ((i != d - 1) || (j != d - 1))
            {
                if (board[i][j] < boardPrevious)
                {
                    return false;
                }
                boardPrevious = board[i][j];
            }
            else if (board[d - 1][d - 1] == 0)
            {
                return true;
            }
        }
    }
return true;
}
/**
 * Description of the swap function of two integers with temporary int.
 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}