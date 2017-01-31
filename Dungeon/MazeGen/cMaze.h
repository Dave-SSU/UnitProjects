#pragma once
#include <istream>
#include <string>
#include <vector>

const int MINIMUM_SIZE = 3;
const int DEFAULT_MAZE_WIDTH = 19;
const int DEFAULT_MAZE_HEIGHT = 19;

class cMaze
{
public:

    cMaze();
    virtual ~cMaze();
    void    reset();
    bool    create(int seed = rand(), int width = DEFAULT_MAZE_WIDTH, int height = DEFAULT_MAZE_HEIGHT);
    std::vector<std::string>     getStrings();    // add this method to use strings?
    //char**  getCharsAs2DArray() { return p_chars; } // this can only be used after getting width and height; less than ideal way to supply
    void    print();
    int     getWidth()  { return m_dimensions.m_x; }
    int     getHeight() { return m_dimensions.m_y; }
    int     getSeed() { return m_seed; }
    bool    getStart(int& col, int& row);
    bool    getEnd(int& col, int& row);
    // remove these methods? providing too much?
    bool    getRandomEmptyPosition(int& col, int& row, int nAttempts=0);
    char    getPositionValue(int col, int row);
    char    setPositionValue(int col, int row, char value);
    void    printOn() { m_printGeneration = true; }
    void    printOff() { m_printGeneration = false; }
    char    getWallChar() { return m_wallChar; }
    char    getFloorChar() { return m_floorChar;  }

private:
    void        initialize();
    void        generateMaze();

    // The Cell struct is used internally by the maze generation algorithm. 
    //  It is only used durin maze creation and not part of the public interface;
    //  only the resulting character array is stored and usable.
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

protected:

    // Internal Vector2 class is used to manage map coordinates. Always keeping the 
    //  row and col values together just makes life easier and shows a simple utility class
    //  in action.
    class Vector2
    {
    public:
        Vector2();
        Vector2(int x, int y);
        Vector2(Vector2& v);
        Vector2& operator=(Vector2& v);
        bool operator==(const Vector2 & v) const;
        // it's ok to expose the members here since the 
        // class is internal to the Maze class  
        // and very little within the Vector so minimal risk
        int     m_x, m_y;
    };

    Vector2     m_start;                // maze starting point
    Vector2     m_end;                  // designated end/finish point
    Vector2     m_dimensions;           // size of the maze: x=width/columns, y=height/rows
    int         m_seed;                 // seed used to generate the maze
    char**      p_chars;                // pointer to an array of characters - printable maze
    std::vector<std::string>  m_strings;  // vector of strings instead of char arrays
    char        m_wallChar = (char)178;       // this is a block in the extended ascii table
    char        m_floorChar = ' ';      // empty space character
    char        m_errorChar = 0;        // used when querying the maze to report an error
    bool        m_printGeneration = true;   // if true, prints progress while map is generated
};

