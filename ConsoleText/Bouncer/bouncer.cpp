#include <Windows.h>
#include <conio.h>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include "ConsoleAttributes.h"


// Reference sites
//  http://cecilsunkure.blogspot.co.uk/2011/11/windows-console-game-writing-to-console.html
//  http://www.teamliquid.net/blogs/350149-game-programming-ascii-games


using namespace std;

const int	INPUT_BUFFER_SIZE = 16;
const char	BLANK_CHAR = ' ';
const string INTRO_MESSAGE = "Right-Click to create a Bouncer, Left-Click on one to to destroy it";
const int BOUNCERS_RESERVED = 32;

struct _Bouncer
{
    char    letter;			// letter./symbol to be drawn
	WORD	attributes;		// colour attributes
	COORD   position;		// current position
	COORD	movement;		// "velocity": x and y change to apply on each update
};


// ----------------------------------------------------------------------------
bool	setupConsole(HANDLE& hIn, HANDLE& hOut, SMALL_RECT& rect);
void	fillScreen(HANDLE hOut, char fillChar, WORD attribute);
void	writeChar(HANDLE hOutput, const COORD& pos, char writeChar, WORD attribute =(WORD)WHITE_ON_BLACK);
void	writeMessage(HANDLE hOutput, const SMALL_RECT& rect, const string& message);
int     getEvents(HANDLE hInput, INPUT_RECORD inputEventArray[]);
_Bouncer	newBouncer(COORD mousePos);

// ----------------------------------------------------------------------------
int main(void)
{
    //srand(0);	// using the better mersenne-twister randomizer 
	mt19937 mt_rand((unsigned int)time(0));

	HANDLE hConsoleOutput = 0, hConsoleInput = 0;
	SMALL_RECT	windowRect;
	
	setupConsole(hConsoleInput, hConsoleOutput, windowRect);

	writeMessage(hConsoleOutput, windowRect, INTRO_MESSAGE);

	// create the vector of bouncers and reserve space for 32 of them
	std::vector<_Bouncer> bouncerVector;
	bouncerVector.reserve(BOUNCERS_RESERVED);

	bool keepLooping = true;
	do
	{
		// Clear all old positions
		for (auto bouncer : bouncerVector)
		{
			writeChar(hConsoleOutput, bouncer.position, BLANK_CHAR);
		}
		// Grab all events
		INPUT_RECORD inputEventArray[INPUT_BUFFER_SIZE];
		int numberOfEvents = getEvents(hConsoleInput, inputEventArray);
		for (int i = 0; i < numberOfEvents; ++i)
		{
			// check for mouse events:
			//  if R-mouse button
			if ((inputEventArray[i].EventType == MOUSE_EVENT) &&
				(inputEventArray[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED))
			{
				bouncerVector.push_back(newBouncer(inputEventArray[i].Event.MouseEvent.dwMousePosition));
			}
			//  if L-mouse button
			else if ((inputEventArray[i].EventType == MOUSE_EVENT) &&
				(inputEventArray[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
			{
				// go through all the bouncers and see if one is at the mouse position
				for (int b = 0; b < bouncerVector.size(); ++b)
				{
					if ((inputEventArray[i].Event.MouseEvent.dwMousePosition.X == bouncerVector[b].position.X)
						&& (inputEventArray[i].Event.MouseEvent.dwMousePosition.Y == bouncerVector[b].position.Y))
					{
						writeChar(hConsoleOutput, bouncerVector[b].position, BLANK_CHAR);
						bouncerVector.erase(bouncerVector.begin() + b);
					}
				}
			}
			// check for Esc key to quit
			else if (inputEventArray[i].EventType == KEY_EVENT)
			{
				if (inputEventArray[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
				{
					keepLooping = false;
					break; // early exit from for-loop
				}
			}
		}
		// update all to new positions
		for (_Bouncer& bouncer : bouncerVector)  // note that a _Bouncer reference is specified so it can be modified
		{
			// if reached the left or right boundary, flip X movement direction
			if ((bouncer.position.X <= 0) || (bouncer.position.X >= windowRect.Right))
				bouncer.movement.X *= -1;
			bouncer.position.X += bouncer.movement.X;
			// if reached the top or bottom boundary, flip Y movement direction
			if ((bouncer.position.Y <= 0) || (bouncer.position.Y >= windowRect.Bottom))
				bouncer.movement.Y *= -1;
			bouncer.position.Y += bouncer.movement.Y;
		}
		// draw bouncers in new positions
		for (auto bouncer : bouncerVector)
		{
			writeChar(hConsoleOutput, bouncer.position, bouncer.letter, bouncer.attributes);
		}

		Sleep(300);
	} while (keepLooping);
	
	// destroy all elements in the vector before exitting, just to be safe
	bouncerVector.erase(bouncerVector.begin(), bouncerVector.end());
	return 0;
}


// ----------------------------------------------------------------------------
void	writeMessage(HANDLE hOutput, const SMALL_RECT& rect, const string& message)
{
	// move the cursor to where the message will be about centred.
	COORD pos = {(rect.Right - (SHORT)message.size())/ (SHORT)2, rect.Bottom / (SHORT)2 };
	SetConsoleCursorPosition(hOutput, pos);

	DWORD d = 0;	// the WriteConsole functions require a variable to store how many characters were written
	WriteConsoleOutputCharacter(hOutput, message.c_str(), message.size(), pos, &d);
}

// ----------------------------------------------------------------------------
bool	setupConsole(HANDLE& hIn, HANDLE& hOut, SMALL_RECT& rect)
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (!hIn || !hOut)
		return false;

	// get the screen size
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(hOut, &info))
		return false;
	rect = info.srWindow;
	return true;
}

// ----------------------------------------------------------------------------
void	writeChar(HANDLE hOutput, const COORD& pos, char writeChar, WORD attribute)
{
	DWORD d;
	WriteConsoleOutputAttribute(hOutput, &attribute, 1, pos, &d);
    WriteConsoleOutputCharacter(hOutput, &writeChar, 1, pos, &d);
}

// ----------------------------------------------------------------------------
int     getEvents(HANDLE hInput, INPUT_RECORD inputEventArray[])
{
	DWORD   eventsInBuffer = 0, eventsRead = 0;

	// check if input events have occurred: keyboard and mouse are the sources of input  
	GetNumberOfConsoleInputEvents(hInput, &eventsInBuffer);

	if (0 == eventsInBuffer)
		return (int)eventsInBuffer;

	// Create a buffer of that size to store the events
	// Read up to INPUT_BUFFER_SIZE number of events into the buffer
	ReadConsoleInput(hInput, inputEventArray, eventsInBuffer, &eventsRead);
	return (int)eventsInBuffer;
}

_Bouncer	newBouncer(COORD mousePos)
{
	mt19937 mt_rand((unsigned int)time(0));

	_Bouncer bouncerTemplate =
	{
		'A' + mt_rand() % 100,
		(WORD)(WHITE_ON_BLACK + mt_rand() % 14),
		{ mousePos.X, mousePos.Y },
		{ (WORD)(mt_rand() % 3 - 1), (WORD)(mt_rand() % 3 - 1) }
	};
	return bouncerTemplate;
}


#if 0
void f()
{
    // Look through any events that have occurred since the last update
    for (auto i = 0; i < eventsRead; ++i)
    {
        // only looking for mouse clicks, so ignore key events
        if (MOUSE_EVENT == inputEventArray[i].EventType)
        {
            int bufferIndex = 
                +80 * inputEventArray[i].Event.MouseEvent.dwMousePosition.Y;
            inputEventArray[i].Event.MouseEvent.dwMousePosition.X;

            // Check for a left click?
            if (inputEventArray[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                //inputEventArray[offsetPos].Char.AsciiChar = (char)0xDB;
            }
            // Check for a right click
            else if (inputEventArray[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
            {

            }
        }
    }
}
#endif

// ----------------------------------------------------------------------------
void	fillScreen(HANDLE hOut, char fillChar, WORD attribute)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hOut, &info);

	DWORD dwBufferLen = info.dwSize.X * info.dwSize.Y;
	const COORD coordWriteStart = { 0, 0 };
	DWORD dwWritten;
	FillConsoleOutputCharacter(hOut, fillChar, dwBufferLen, coordWriteStart, &dwWritten);
	
	// Need to call again?
	GetConsoleScreenBufferInfo(hOut, &info);
	FillConsoleOutputAttribute(hOut, info.wAttributes, dwBufferLen, coordWriteStart, &dwWritten);
	SetConsoleCursorPosition(hOut, coordWriteStart);
}


// a DWORD is a double-word. 1 word is 2 bytes, so a DWORD is 4 bytes in length. 
// a HANDLE is just a number, but it's a term commonly used to hide the details of how something works. It will be an ID



