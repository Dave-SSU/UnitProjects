#pragma once
#include <istream>
#include <string>
#include <vector>
#include <random>

#include "cVector2.h"

const int MINIMUM_SIZE = 9;
const int DEFAULT_MAZE_WIDTH = 19;
const int DEFAULT_MAZE_HEIGHT = 19;


class cMaze
{
public:  //  cMaze public interface methods
    // use create() to make a new maze, giving it appropriate dimensions and optional random seed
    bool    create(int seed = rand(), int width = DEFAULT_MAZE_WIDTH, int height = DEFAULT_MAZE_HEIGHT);
      
    // getStrings() returns a vector of strings; one string for each horizontal row of the maze
    const std::vector<std::string>&	getStrings();

	// getEndpoints returns a vector of potential start and endpoints
	const std::vector<cVector2>& getEndpoints();

    // getWidth() and getHeight() returns the dimensions of the current maze, if any 
    int     getWidth()      { return m_dimensions.m_x; }
    int     getHeight()     { return m_dimensions.m_y; }

    // getSeed() returns the random seed used to create the maze
    int     getSeed()	    { return m_seed; }

    // getStart() sets the rol and column of the player start position
    bool    getStart(int& col, int& row);

    // getExit() sets the rol and column of the player end/exit position
    bool    getExit(int& col, int& row);

	// print() tells the maze to draw itself in current form to the console, if printing is on (m_printGeneration == true)
    void    print();

	// turn printing the maze with each change and at the end on
    void    printOn()       { m_printGeneration = true; }

	// turn printing the maze with each change and at the end off
    void    printOff()      { m_printGeneration = false; }
    
	// get the character used by the maze to indicate a wall space
	char    getWallChar()   { return m_wallChar; }
	
	// get the character used by the maze to indicate a floor space
    char    getFloorChar()  { return m_floorChar;  }

	// get the character used by the maze to indicate the player starting location
	char    getStartChar()  { return m_startChar; }
	
	// get the character used by the maze to indicate the player exit location
	char    getExitChar()   { return m_exitChar; }

	// default constructor
    cMaze();
    virtual ~cMaze();

protected:
	// call reset() to wipe any existing maze data, ready to create a new one
	void    reset();
	
	// repeated looks for random cells that contain floor cells
    bool    getRandomEmptyPosition(int& col, int& row, int nAttempts = 0);

	// getPositionValue() returns the character at a particular row/y, col/x location
	char    getPositionValue(int col, int row);
	
	// setPositionValue() set the character at a particular row/y, col/x location
	char    setPositionValue(int col, int row, char value);

    cVector2    m_start{ 0,0 };         // maze starting point
    cVector2    m_end{ 0,0 };           // designated end/finish point
    cVector2    m_dimensions{ 0,0 };    // size of the maze: x=width/columns, y=height/rows
    int         m_seed{ 0 };            // seed used to generate the maze
	std::mt19937 m_mtRand;				// mersenne-twister object
    char**      p_chars{ nullptr };     // pointer to an array of characters - printable maze
    char        m_floorChar {' '};      // empty space character
    char        m_wallChar{ (char)178 }; // this is a block in the extended ascii table
    char        m_startChar{ 'S' };     // starting space character
    char        m_exitChar{ 'E' };      // exit space character
    char        m_errorChar{ 0 };       // used when querying the maze to report an error
    bool        m_printGeneration { false };  // if true, prints progress while map is generated
    std::vector<std::string>  m_strings; // vector of strings instead that comprise the map
    std::vector<cVector2>	m_endpoints; // vector of endpoints: potential start and exit

private:
    void        initialize();
    void        generateMaze();

    // The Cell struct is used internally by the maze generation algorithm. 
    //  It is only used during maze creation and not part of the public interface.
    //  Only the resulting character array is stored and usable.
    struct Cell
    {
        bool visited;
        bool top_wall;
        bool bot_wall;
        bool left_wall;
        bool right_wall;
        char display;
    };
    Cell**      p_Cells;
};

