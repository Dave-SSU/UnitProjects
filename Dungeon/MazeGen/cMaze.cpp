/*******************************************************************************************
* Name:             cMaze.cpp                                                              *
* Description:      Class that wraps a randomly generated a 'perfect' maze created         *
*                   using depth-first search algorithm                                     *
* Date:             02/11/2010                                                             *
* Data Structures:  Double array of structs for cells, stack for back-tracking             *
* Others:           fstream is used to save maze into a file                               *
*                                                                                          *
* Note: Original code and maze generation algorithm written by Jay Seong and transcribed   *
*   by Stephen Pendley from YouTube video https://www.youtube.com/watch?v=EvAzVhAii_o.     *
*   cMaze class with additional functionality and comments written by Dave Cobb.           * 
* All credit for this version of the code and it's original goes to Jay Seong.             * 
*******************************************************************************************/

#include <iostream>
#include <stack>
#include <random>
#include <time.h>
#include "cMaze.h"

#ifdef _DEBUG

#include <windows.h>
#include <conio.h>
#define _CONSOLE_OUTPUT 1

#else

#define print()

#endif

const unsigned int SIGNED_MASK = 0x7FFFFFF;

//-----------------------------------------------------------------------------
cMaze::cMaze()
{
}

//-----------------------------------------------------------------------------
cMaze::~cMaze()
{
    reset();
}

//-----------------------------------------------------------------------------
void    cMaze::reset()
{
    if (nullptr != p_Cells)
    {   
        delete[] p_Cells[0];
        delete[] p_Cells;
        p_Cells = nullptr;
    }
    if (nullptr != p_chars)
    {
        delete[] p_chars[0];
        delete[] p_chars;
        p_chars = nullptr;
    }
    
    for (auto stringItr = m_strings.begin(); stringItr != m_strings.end(); stringItr++)
        stringItr->erase();
    m_strings.erase(m_strings.begin(), m_strings.end());
	m_endpoints.erase(m_endpoints.begin(), m_endpoints.end());

    m_start.m_x = m_start.m_y = m_end.m_x = m_end.m_y = m_dimensions.m_x = m_dimensions.m_y = 0;
}

//-----------------------------------------------------------------------------
bool    cMaze::create(int seed, int width, int height)
{
    // validate input: dimensions must be odd numbers. Shrink by 1 if even
    if (MINIMUM_SIZE >= width || MINIMUM_SIZE >= height)
        return false;

    // if re-creating the maze, set key members back to clean starting point
    if (nullptr != p_chars)
        reset();

	// store and apply the random seed passed in
    m_seed = seed;
	m_seed = (unsigned int)time(0);
	m_mtRand.seed(m_seed);

    m_dimensions.m_x = width;
    m_dimensions.m_y = height;

    // Create the temporary data structures and generate the actual maze
    initialize();
    generateMaze();

    if (m_printGeneration)
	    print();

    // now create a 2D character array; don't always need/want the Cells after generating the maze
    p_chars = new char*[height];
    p_chars[0] = new char[width*height];
    for (int r = 1; r < height; r++)
        p_chars[r] = p_chars[r - 1] + width;

    // and copy the display characters from the Cell array to the char array and the vector of strings
    for (int r = 0; r < m_dimensions.m_y; r++)
    {
        for (int c = 0; c < m_dimensions.m_x; c++)
            p_chars[r][c] = p_Cells[r][c].display;
        std::string s(p_chars[r], m_dimensions.m_x);
        m_strings.push_back(s);
    }

#ifdef _DEBUG
    // put a pause in so maze can be inspected visually
    std::cout << std::endl << "press a key to continue" << std::endl;
    _getch();
#endif

    return true;
}

//-----------------------------------------------------------------------------
void    cMaze::initialize()
{
    // Allocate a 2D matrix of Cells, using a sneaky approach that allows use of [][] notation.
    //  This code actually creates an array of Cell pointers, and then a block of Cells.
    //  The array of pointers is then initialized to point into the block, so each pointer 
    //  accesses a "row" from 0 to Height.
    p_Cells = new Cell*[m_dimensions.m_y];
    p_Cells[0] = new Cell[m_dimensions.m_x*m_dimensions.m_y];
    for (int r = 1; r < m_dimensions.m_y; r++)
        p_Cells[r] = p_Cells[r - 1] + m_dimensions.m_x;

    for (int r = 0; r < m_dimensions.m_y; r++)
    {
        for (int c = 0; c < m_dimensions.m_x; c++)
        {
            p_Cells[r][c].display = m_wallChar;
            p_Cells[r][c].visited = false;
            p_Cells[r][c].top_wall = true;
            p_Cells[r][c].bot_wall = true;
            p_Cells[r][c].left_wall = true;
            p_Cells[r][c].right_wall = true;
        }
    }
    for (int r = 1; r < m_dimensions.m_y - 1; r++)
    {
        for (int c = 1; c < m_dimensions.m_x-1; c++)
        {
            // Border Cells have fewer accessible walls
            p_Cells[1][c].top_wall = false;
            p_Cells[m_dimensions.m_y - 2][c].bot_wall = false;
            p_Cells[r][1].left_wall = false;
            p_Cells[r][m_dimensions.m_x - 2].right_wall = false;
        }
    }
}

//-----------------------------------------------------------------------------
void    cMaze::generateMaze()
{
    // Generate a random odd position between 1 and width - 2, and 1 and height - 1.
    // Use SIGNED_MASK to force the random number to be positive when converted from unsigned int to signed int.
    cVector2 randPos(SIGNED_MASK & (2 * m_mtRand() + 1) % (m_dimensions.m_x - 1), SIGNED_MASK & (2 * m_mtRand() + 1) % (m_dimensions.m_y - 1));
    
	// Save this position as the player starting point
    m_start = randPos;
	m_endpoints.push_back(randPos);

	// counter for all the cells visited while generating the maze. done when all have been visited.
    int visitedCells = 1;
    int totalCells = ((m_dimensions.m_x - 1) / 2)*((m_dimensions.m_y - 1) / 2);
    
    // Stack is used to trace the reverse path
    std::stack<int> back_trackX, back_trackY; 		
    std::stack<cVector2> back_track;

    // Set the start cell
	p_Cells[randPos.m_y][randPos.m_x].display = getStartChar();
    // Set start cell as visited;
    p_Cells[randPos.m_y][randPos.m_x].visited = true;

    bool just_moved = false;
    char endpointChar = 'a';

    // Now - randomly wander through the cells adjacent to the current cell, until all cells are visited. 
    //  This guarantees there is a path from the starting cell to a finishing cell.
    while (visitedCells < totalCells)
    {
        // test to make sure the adjacent cells are valid
        if ((randPos.m_y >=2 && (p_Cells[randPos.m_y - 2][randPos.m_x].visited == false) && (p_Cells[randPos.m_y][randPos.m_x].top_wall == true && p_Cells[randPos.m_y - 2][randPos.m_x].bot_wall == true)) ||
            (randPos.m_y <= m_dimensions.m_y-3 && (p_Cells[randPos.m_y + 2][randPos.m_x].visited == false) && (p_Cells[randPos.m_y][randPos.m_x].bot_wall == true && p_Cells[randPos.m_y + 2][randPos.m_x].top_wall == true)) ||
            (randPos.m_x >= 2 && (p_Cells[randPos.m_y][randPos.m_x - 2].visited == false) && (p_Cells[randPos.m_y][randPos.m_x].left_wall == true && p_Cells[randPos.m_y][randPos.m_x - 2].right_wall == true)) ||
            (randPos.m_x <= m_dimensions.m_x - 3 && (p_Cells[randPos.m_y][randPos.m_x + 2].visited == false) && (p_Cells[randPos.m_y][randPos.m_x].right_wall == true && p_Cells[randPos.m_y][randPos.m_x + 2].left_wall == true)))
        {
            int random = (m_mtRand() % 4);		// Pick a random wall 0-3 to knock down

            // GO UP
            if ((random == 0) && (randPos.m_y > 1)) {
                if (p_Cells[randPos.m_y - 2][randPos.m_x].visited == false) {	// If not already visited
                    p_Cells[randPos.m_y - 1][randPos.m_x].display = m_floorChar; // Delete display
                    p_Cells[randPos.m_y - 1][randPos.m_x].visited = true;		// Mark cell as visited
                    p_Cells[randPos.m_y][randPos.m_x].top_wall = false;			// Knock down wall

                    back_track.push(randPos);									// Push position for back tracking
                    just_moved = true;

                    randPos.m_y -= 2;											// Move to next cell
                    p_Cells[randPos.m_y][randPos.m_x].visited = true;			// Mark cell moved to as visited
                    p_Cells[randPos.m_y][randPos.m_x].display = m_floorChar;	// Update path
                    p_Cells[randPos.m_y][randPos.m_x].bot_wall = false;			// Knock down wall
                    visitedCells++;												// Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO DOWN
            else if ((random == 1) && (randPos.m_y < m_dimensions.m_y - 2)) {
                if (p_Cells[randPos.m_y + 2][randPos.m_x].visited == false) {	// If not visited
                    p_Cells[randPos.m_y + 1][randPos.m_x].display = m_floorChar;// Delete display
                    p_Cells[randPos.m_y + 1][randPos.m_x].visited = true;		// Mark cell as visited
                    p_Cells[randPos.m_y][randPos.m_x].bot_wall = false;			// Knock down wall

                    back_track.push(randPos);									// Push position for back tracking
                    just_moved = true;

                    randPos.m_y += 2;											// Move to next cell
                    p_Cells[randPos.m_y][randPos.m_x].visited = true;			// Mark cell moved to as visited
                    p_Cells[randPos.m_y][randPos.m_x].display = m_floorChar;	// Update path
                    p_Cells[randPos.m_y][randPos.m_x].top_wall = false;			// Knock down wall
                    visitedCells++;												// Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO LEFT
            else if ((random == 2) && (randPos.m_x > 1)) {
                if (p_Cells[randPos.m_y][randPos.m_x - 2].visited == false) {	// If not visited
                    p_Cells[randPos.m_y][randPos.m_x - 1].display = m_floorChar;// Delete display
                    p_Cells[randPos.m_y][randPos.m_x - 1].visited = true;		// Mark cell as visited
                    p_Cells[randPos.m_y][randPos.m_x].left_wall = false;		// Knock down wall

                    back_track.push(randPos);									// Push position for back tracking
                    just_moved = true;

                    randPos.m_x -= 2;											// Move to next cell
                    p_Cells[randPos.m_y][randPos.m_x].visited = true;			// Mark cell moved to as visited
                    p_Cells[randPos.m_y][randPos.m_x].display = m_floorChar;	// Update path
                    p_Cells[randPos.m_y][randPos.m_x].right_wall = false;		// Knock down wall
                    visitedCells++;												// Increase visitedCells counter
                }
                else
                    continue;
            }

            // GO RIGHT
            else if ((random == 3) && (randPos.m_x < m_dimensions.m_x - 2)) {
                if (p_Cells[randPos.m_y][randPos.m_x + 2].visited == false) {	// If not visited
                    p_Cells[randPos.m_y][randPos.m_x + 1].display = m_floorChar;// Delete display
                    p_Cells[randPos.m_y][randPos.m_x + 1].visited = true;		// Mark cell as visited
                    p_Cells[randPos.m_y][randPos.m_x].right_wall = false;		// Knock down wall

                    back_track.push(randPos);									// Push position for back tracking
                    just_moved = true;

                    randPos.m_x += 2;											// Move to next cell
                    p_Cells[randPos.m_y][randPos.m_x].visited = true;			// Mark cell moved to as visited
                    p_Cells[randPos.m_y][randPos.m_x].display = m_floorChar;	// Update path
                    p_Cells[randPos.m_y][randPos.m_x].left_wall = false;		// Knock down wall
                    visitedCells++;												// Increase visitedCells counter
                }
                else
                    continue;
            }
        }
        else 
        {
            if (just_moved)
            {
                p_Cells[randPos.m_y][randPos.m_x].display = endpointChar++;
				m_endpoints.push_back(randPos);
                just_moved = false;
            }
            randPos = back_track.top();
            back_track.pop();
        }
        if (m_printGeneration)
            print();
    }

    m_end = randPos;
	m_endpoints.push_back(randPos);
    p_Cells[m_end.m_y][m_end.m_x].display = getExitChar();
    if (m_printGeneration)
        print();
}

//-----------------------------------------------------------------------------
const std::vector<std::string>&	cMaze::getStrings()
{
    return const_cast<const std::vector<std::string>&>(m_strings);
}

const std::vector<cVector2>&	cMaze::getEndpoints()
{
	return const_cast<const std::vector<cVector2>&>(m_endpoints); 
}

#if _CONSOLE_OUTPUT
//-----------------------------------------------------------------------------
void        cMaze::print()
{
    HANDLE hOut;
    COORD position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    position.X = 0;
    position.Y = 0;
    SetConsoleCursorPosition(hOut, position);

    for (int r = 0; r < m_dimensions.m_y; r++) 
    {
        std::cout << std::endl;
        for (int c = 0; c < m_dimensions.m_x; c++)
            std::cout << p_Cells[r][c].display;
    }
}
#endif

//-----------------------------------------------------------------------------
bool    cMaze::getStart(int& col, int& row)
{
    if (nullptr == p_chars)
        return false;
    col = m_start.m_x;
    row = m_start.m_y;
    return true;
}

//-----------------------------------------------------------------------------
bool    cMaze::getExit(int& col, int& row)
{
    if (nullptr == p_chars)
        return false;
    col = m_end.m_x;
    row = m_end.m_y;
    return true;
}

//-----------------------------------------------------------------------------
bool    cMaze::getRandomEmptyPosition(int & col, int & row, int nAttempts)
{
    if (0 == nAttempts)
        nAttempts = m_dimensions.m_x * m_dimensions.m_y;
    do
    {
        int x = m_mtRand() % (m_dimensions.m_x - 2) + 1;
        int y = m_mtRand() % (m_dimensions.m_y - 2) + 1;

        // is position is empty
        if (m_floorChar == p_chars[y][x])
        {
            col = x;
            row = y;
            return true;
        }
    } while (--nAttempts >= 0);
    return false;
}

//-----------------------------------------------------------------------------
char    cMaze::getPositionValue(int col, int row)
{
    if (0 > col || 0 > row || m_dimensions.m_y <= row || m_dimensions.m_x <= col)
        return m_errorChar;
    return p_chars[col][row];
}

//-----------------------------------------------------------------------------
char    cMaze::setPositionValue(int col, int row, char value)
{
    char    old = getPositionValue(col, row);
    if (m_errorChar == old)
        return m_errorChar;
    p_chars[col][row] = value;
    return old;
}
