/**
 * fifteen.c
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

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

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
    FILE *file = fopen("log.txt", "w");
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
        int tile = get_int();

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
        usleep(500000);
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
    // declare integer n to use for descending the #s
    int n = (d * d) - 1;
    // access the row then the column of the board
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // set tiles values
            board[i][j] = n;
            n--;
        }
        // set blank tile to zero
        board[d][d] = 0;

        // check for even # of tiles, if so, swap the place of 1 and 2
        if(d % 2 == 0)
            {
                int swap = board[d-1][d-2];
                board[d-1][d-2] = board[d-1][d-3];
                board[d-1][d-3] = swap;
            }
    }
}


/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // access row of the board
    for(int i = 0; i < d; i++)
    {
        // access column of board
        for(int j = 0; j < d; j++)
        {
            // print values and add a space if single-digit #s
            if(board[i][j] > 9)
            {
                printf("%i ", board[i][j]);
            }
            else if(board[i][j] < 10 && board[i][j] >= 1)
            {
                printf("%2i ", board[i][j]);
            }
            // print an underspace for the blank tile
            else if(board[i][j] < 1)
            {
                printf(" _");
            }
            else
            {
               break;
            }


        }
        printf("\n");
    }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
   int swap;
    // define two variables for the location of user's tile and the blank tile
    // access the row of the board
    for(int i = 0; i < d; i++)
    {
        // access column of the board
        for(int j = 0; j < d; j++)
        {
            // find the tile the user wants to move then swap
            if(board[i][j] == tile)
            {
                if(i > 0)
                {
                    if(board[i-1][j-0] == 0)
                    {
                        swap = tile;
                        board[i][j] = board[i-1][j-0];
                        board[i-1][j-0] = swap;
                        return true;
                    }

                }
                if(j > 0)
                {
                    if(board[i-0][j-1] == 0)
                    {
                        swap = tile;
                        board[i][j] = board[i-0][j-1];
                        board[i-0][j-1] = swap;
                        return true;
                    }

                }

                if(i < d - 1)
                {
                    if(board[i+1][j+0] == 0)
                    {
                        swap = tile;
                        board[i][j] = board[i+1][j+0];
                        board[i+1][j+0] = swap;
                        return true;
                    }

                }

                if(j < d - 1)
                {
                    if(board[i+0][j+1] == 0)
                    {
                        swap = tile;
                        board[i][j] = board[i+0][j+1];
                        board[i+0][j+1] = swap;
                        return true;
                    }
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
    // declare integer v to use for ascending the #s
    int v = 1;
    // access the row of the board
    for(int i = 0; i < d; i++)
    {
        // access the column of the board
        for(int j = 0; j < d; j++)
        {
            // check to see if board is in ascending order
            if(board[i][j] == v)
            {
                v++;
            }
            // check for blank space and finish game
            else if((v == d*d) && (board[d][d] == 0))
            {
                return true;
            }

            else
            {
                return false;
            }
        }
    }
    return true;
}

