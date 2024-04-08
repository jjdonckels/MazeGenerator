/*******************************************************
 * AUTHOR: James Donckels
 * LAST MODIFIED: 11/1/23
 ******************************************************/

#include <iostream>
#include <vector>

/***************************************************************
 * This program recursively generates a random maze and
 * displays it to the console.
 **************************************************************/

using namespace std;

// MAZE DIMENSIONS
const int ROW_NUM = 17, COL_NUM = 25;

// prints the maze to the console
void printMaze(int m[ROW_NUM][COL_NUM]);

// fills in the maze
void createMaze(int m[ROW_NUM][COL_NUM], int currCol, int currRow, int startCol, int startRow);

// return a list of directions to go where new stones can be knocked down
vector<int> validNewPaths(int m[ROW_NUM][COL_NUM], int currCol, int currRow);

// knocks down two stones in a valid direction
void makeNewPath(int m[ROW_NUM][COL_NUM], vector<int> paths, int &currCol, int &currRow);

// moves position back towards the start by backtracking on the least traveled path
void backtrack(int m[ROW_NUM][COL_NUM], int &currCol, int &currRow);

// returns the direction to backtrack along
int findBacktrackPath(int m[ROW_NUM][COL_NUM], int currCol, int currRow);

// randomly puts an entrance and exit on the maze
void createOpenings(int m[ROW_NUM][COL_NUM]);

int main()
{
    // seed the random number generator or more random maze generation
    srand(time(NULL));


    // set up maze with all stones standing
    int maze[ROW_NUM][COL_NUM];
    for (int row = 0; row < ROW_NUM; row++)
    {
        for (int col = 0; col < COL_NUM; col++)
        {
            maze[row][col] = 0;
        }
    }


    cout << endl << endl;

    // get random start position
    // get random starting column, will be odd index
    int startCol;
    startCol = (rand() % (COL_NUM - 2)) + 1;
    while (startCol % 2 != 1)
        startCol = (rand() % (COL_NUM - 2)) + 1;

    // get random starting row, will be odd index
    int startRow;
    startRow = (rand() % (ROW_NUM - 2)) + 1;
    while (startRow % 2 != 1)
        startRow = (rand() % (ROW_NUM - 2)) + 1;

    // create the maze interior
    createMaze(maze, startCol, startRow, startCol, startRow);

    // create entrance and exit
    createOpenings(maze);

    // print maze to console
    cout << "\nMaximize window for proper scaling\n\n";
    printMaze(maze);

    return 0;
}

/**************************************************************
 * printMaze()
 *
 * This function prints the maze to the console by filling
 * walls in with boxes and leaving spaces blank.
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *
 *  RETURNS:
 *      prints the maze walls to the console
 *************************************************************/
void printMaze(int m[ROW_NUM][COL_NUM])
{
    for (int row = 0; row < ROW_NUM; row++)
    {
        for (int col = 0; col < COL_NUM; col++)
        {
            if (m[row][col] == 0)
                cout << "\u25A0  ";
            else
                cout << "   ";
        }
        cout << endl;
    }
}

/**************************************************************
 * createMaze()
 *
 * This function creates the maze interior by knocking down
 * walls and making a path through them around the whole grid
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *  currCol - the current column
 *  currRow - the current row
 *  startCol - starting column
 *  startRow - starting row
 *
 *  RETURNS:
 *      this function updates the values in the 2d maze array
 *      to show which entries are walls and which ones
 *      are paths
 *************************************************************/
void createMaze(int m[ROW_NUM][COL_NUM], int currCol, int currRow, const int startCol, const int startRow)
{
    // update current position in maze
    m[currRow][currCol]++;

    // base case: after backtracking, if no walls left to knock down, return out
    if (m[startRow][startCol] > 1)
        return;
    else
    {

        vector<int> validPaths = validNewPaths(m, currCol, currRow);
        // branch for knocking down new stones
        if (!validPaths.empty())
        {
            makeNewPath(m, validPaths, currCol, currRow);
            createMaze(m, currCol, currRow, startCol, startRow);
        }
            // branch for backtracking
        else if (validPaths.empty())
        {
            backtrack(m, currCol, currRow);
            createMaze(m, currCol, currRow, startCol, startRow);
        }
        else
        {
            cout << "\nError in createMaze, shouldn't be reached\n";
        }
    }
}

/**************************************************************
 * validNewPaths()
 *
 * This function searches for the directions to make new
 * paths in and break down more walls
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *  currCol - the current column
 *  currRow - the current row the
 *
 *  RETURNS:
 *      this function returns a vector of ints storing the
 *      valid directions to make new paths in
 *
 *      if the vector is empty, there are no new paths and
 *      the path must backtrack
 *
 *      the directions are 0, 1, 2, 3 for east, north,
 *      west, and south, respectively
 *************************************************************/
vector<int> validNewPaths(int m[ROW_NUM][COL_NUM], int currCol, int currRow)
{
    vector<int> paths;
    // check east
    if (m[currRow][currCol + 1] == 0 && m[currRow][currCol + 2] == 0 && currCol < COL_NUM - 3)
        paths.push_back(0);
    // check north
    if (m[currRow - 1][currCol] == 0 && m[currRow - 2][currCol] == 0 && currRow >= 3)
        paths.push_back(1);
    // check west
    if (m[currRow][currCol - 1] == 0 && m[currRow][currCol - 2] == 0 && currCol >= 3)
        paths.push_back(2);
    // check south
    if (m[currRow + 1][currCol] == 0 && m[currRow + 2][currCol] == 0 && currRow < ROW_NUM - 3)
        paths.push_back(3);

    return paths;
}

/**************************************************************
 * makeNewPath()
 *
 * This function simulates knocking down stones
 * along a new path and updates the maze grid accordingly
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *  paths - the vector that holds the valid directions
 *      to create a new path and knock down new stones
 *  currCol - the current column, passed by reference
 *      because it will be updated in the function
 *  currRow - the current row, passed by reference
 *      because it will be updated in the function
 *
 *  RETURNS:
 *      this function updates the maze data to simulate
 *      a new path being made and new stones being knocked
 *      down; also updates the current column
 *      and row positions according to the direction of the
 *      new path
 *************************************************************/
void makeNewPath(int m[ROW_NUM][COL_NUM], vector<int> paths, int &currCol, int &currRow)
{
    int randPathIndex = rand() % paths.size();
    int direction = paths.at(randPathIndex);

    if (direction == 0) // go east
    {
        m[currRow][currCol + 1]++; // knock down stone in front
        currCol = currCol + 2; // move to new position 2 units east
    }
    else if (direction == 1) // go north
    {
        m[currRow - 1][currCol]++; // knock down stone in front
        currRow = currRow - 2; // move to new position 2 units north
    }
    else if (direction == 2) // go west
    {
        m[currRow][currCol - 1]++; // knock down stone in front
        currCol = currCol - 2; // move to new position 2 units west
    }
    else if (direction == 3) // go south
    {
        m[currRow + 1][currCol]++; // knock down stone in front
        currRow = currRow + 2; // move to new position 2 units south
    }
    else
    {
        cout << "\n\nError in makeNewPath, shouldn't be reached\n\n";
    }
}

/**************************************************************
 * backtrack()
 *
 * This function simulates backtracking along a path
 * that was already made since there are no new paths
 * to make from the current position; backtracking
 * will either reveal another direction a new path can be
 * made in, or it will go all the way back to the starting
 * position in the maze meaning the maze interior is completely
 * generated and the recursive function will end
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *  currCol - the current column, passed by reference
 *      because it will be updated in the function
 *  currRow - the current row, passed by reference
 *      because it will be updated in the function
 *
 *  RETURNS:
 *      this function updates the maze data according to
 *      which direction was chosen; even though
 *      no new stones were knocked down, each entry in the
 *      maze is updated after being visited to keep track
 *      of how many times that path has been covered
 *
 *      also updates current column and row positions in the maze
 *************************************************************/
void backtrack(int m[ROW_NUM][COL_NUM], int &currCol, int &currRow)
{
    int direction = findBacktrackPath(m, currCol, currRow);
    if (direction == 0) // go east
    {
        m[currRow][currCol + 1]++; // update grid value
        currCol = currCol + 2; // move to new position 2 units east
    }
    else if (direction == 1) // go north
    {
        m[currRow - 1][currCol]++; // update grid value
        currRow = currRow - 2; // move to new position 2 units north
    }
    else if (direction == 2) // go west
    {
        m[currRow][currCol - 1]++; // update grid value
        currCol = currCol - 2; // move to new position 2 units west
    }
    else if (direction == 3) // go south
    {
        m[currRow + 1][currCol]++; // update grid value
        currRow = currRow + 2; // move to new position 2 units south
    }
    else
    {
        cout << "\n\nError in backtrack, shouldn't be reached\n\n";
    }
}

/**************************************************************
 * findBacktrackPath()
 *
 * This function is called when backtracking is necessary
 * and there are no new paths to take from the current
 * position; it will find the least traveled path and
 * return the int that corresponds to that direction:
 * 0 for east, 1 for north, 2 for west, and 3 for south
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *  currCol - the current column
 *  currRow - the current row
 *
 *  RETURNS:
 *      this function returns an int from 0 to 3 inclusive
 *      that corresponds to the designated direction the
 *      to backtrack in
 *
 *      returns 0 for east, 1 for north, 2 for west,
 *      and 3 for south
 *************************************************************/
int findBacktrackPath(int m[ROW_NUM][COL_NUM], int currCol, int currRow)
{
    // array holding the grid values directly adjacent to current position
    int testPaths[4];
    testPaths[0] = m[currRow][currCol + 1];
    testPaths[1] = m[currRow - 1][currCol];
    testPaths[2] = m[currRow][currCol - 1];
    testPaths[3] = m[currRow + 1][currCol];

    int minDirectionIndex = 0;
    int minVal = 1;

    // loop through and find the index of the element in the array
    // that is the smallest nonzero positive number, that will be the
    // desired direction
    for (int i = 1; i < 4; i++)
    {
        if (testPaths[i] != 0 && testPaths[i] <= minVal)
        {
            minVal = testPaths[i];
            minDirectionIndex = i;
        }
    }

    // now we have the index of the direction to backtrack along
    return minDirectionIndex;
}

/**************************************************************
 * createOpenings()
 *
 * This function is called after the maze interior has been
 * fully created and adds an entrance and exit to the maze
 * on opposite walls
 *
 * PARAMETERS:
 *  m - the 2D array containing the maze data
 *
 *  RETURNS:
 *      this function updates the maze data to reflect
 *      an opening being made on each of the east/west walls
 *      or north/south walls as determined by a random number
 *      that chooses the orientation of the entrance/exit
 *
 *      POSTCONDITION: the entrance and exit will be adjacent
 *      to an open path, i.e. the entrance and exit will never
 *      open up to a wall meaning there would be no way out of
 *      the maze
 *************************************************************/
void createOpenings(int m[ROW_NUM][COL_NUM])
{
    int openingDirection = rand() % 2;
    if (openingDirection == 0) // opening will be on east side
    {
        int randIndex = rand() % (ROW_NUM - 2) + 1;
        // validate index so opening will connect to another opening and not a wall
        while (m[randIndex][COL_NUM - 2] == 0)
            randIndex = rand() % (ROW_NUM - 2) + 1;
        m[randIndex][COL_NUM - 1]++;

        // make exit on west side
        randIndex = rand() % (ROW_NUM - 2) + 1;
        // validate index so exit will connect to another opening and not a wall
        while (m[randIndex][1] == 0)
            randIndex = rand() % (ROW_NUM - 2) + 1;
        m[randIndex][0]++;
    }
    else
        // opening will be on north side
    {
        int randIndex = rand() % (COL_NUM - 2) + 1;
        // validate index so opening will connect to another opening and not a wall
        while (m[1][randIndex] == 0)
            randIndex = rand() % (COL_NUM - 2) + 1;
        m[0][randIndex]++;

        // make exit on south side
        randIndex = rand() % (COL_NUM - 2) + 1;
        // validate index so opening will connect to another opening and not a wall
        while (m[ROW_NUM - 2][randIndex] == 0)
            randIndex = rand() % (COL_NUM - 2) + 1;
        m[ROW_NUM - 1][randIndex]++;
    }
}
