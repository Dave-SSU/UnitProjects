// ConsoleText.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <Tchar.h>
#include <wincon.h>

#include <string>
#include <vector>
#include <iostream>

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.


int _tmain(int argc, _TCHAR* argv[]) 
{
	struct MAP_POSITION
	{
		bool		isOccupied;
		std::string	occupant;
	};

	MAP_POSITION  empty_map_position = { false, "X" };

	const int NUMBER_OF_ROWS = 10;
	const int NUMBER_OF_COLUMNS = 10;

	// One of the forms of vector initialisation allows you to specify a value to assign to a 
	//	number of elements. In this case, it will initialist the whole thing to empty.
	std::vector<MAP_POSITION> Map(NUMBER_OF_ROWS * NUMBER_OF_COLUMNS, empty_map_position);

	// do some stuff here…maybe move things into map positions…

	for (auto m : Map)
	{
		std::cout << m.occupant;
	}





	


	std::vector<std::vector<COORD>>	v2d(16);
	v2d[0][0] = { 0, 0 };
	//COORD * p2c = v2d.data();

	COORD c{}, c1{};
	std::vector<COORD>	coords = { c };
	std::vector<COORD>	coords1 = { c };

	COORD * cr = coords1.data();

	std::vector<COORD>::size_type t = coords.capacity();
	int s = coords.size();
	coords.push_back(c);
	t = coords.capacity();
	s = coords.size();
	coords.assign(100, c);

	std::string  str;
	if (std::cin >> str)
		s++;
	else
		s--;

	// Set up the handles for reading/writing:
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(wHnd, &info);
	
	_COORD wc = {rand() % info.srWindow.Right, rand() % info.srWindow.Bottom};
    DWORD d = 0;
    WCHAR ballChar = 240;
    WCHAR blankChar = L' ';
	_COORD move = { rand() % 2 ? 1 : -1, rand() % 2 ? 1 : -1 };
	do
	{
        // clear the previous space - unnecessary first time through obviously
		WriteConsoleOutputCharacter(wHnd, &blankChar, 1, wc, &d);
        // if reached the left or right boundary, flip X movement direction
		if ((wc.X <= 0) || (wc.X >= info.srWindow.Right))
			move.X *= -1;
		wc.X += move.X;
        // if reached the top or bottom boundary, flip Y movement direction
        if ((wc.Y <= 0) || (wc.Y >= info.srWindow.Bottom))
			move.Y *= -1;
		wc.Y += move.Y;
		WriteConsoleOutputCharacter(wHnd, &ballChar, 1, wc, &d);
		//_sleep(100);
        Sleep(100);

	} while (0 == _kbhit());

#if 0
	// Change the window title:
	SetConsoleTitle(TEXT("Win32 Console Control Demo"));

	// Set up the required window size:
	SMALL_RECT windowSize = { 0, 0, 79, 49 };

	// Change the console window size:
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);

	// Create a COORD to hold the buffer size:
	COORD bufferSize = { 80, 50 };

	// Change the internal buffer size:
	SetConsoleScreenBufferSize(wHnd, bufferSize);

	// Set up the character buffer:
	CHAR_INFO consoleBuffer[80 * 50];

	// Clear the CHAR_INFO buffer:
	for (int i = 0; i < 80 * 50; ++i) {

		// Fill it with white-backgrounded spaces
		consoleBuffer[i].Char.AsciiChar = ' ';
		consoleBuffer[i].Attributes =
			BACKGROUND_BLUE |
			BACKGROUND_GREEN |
			BACKGROUND_RED |
			BACKGROUND_INTENSITY;
	}

	// Set up the positions:
	COORD charBufSize = { 80, 50 };
	COORD characterPos = { 0, 0 };
	SMALL_RECT writeArea = { 0, 0, 79, 49 };

	// Copy to display:
	WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

	// How many events have happened?
	DWORD numEvents = 0;

	// How many events have we read from the console?
	DWORD numEventsRead = 0;

	// Boolean flag to state whether app is running or not.
	bool appIsRunning = true;

	// If we set appIsRunning to false, the program will end!
	while (appIsRunning) {

		// Find out how many console events have happened:
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);

		// If it's not zero (something happened...)
		if (numEvents != 0) {

			// Create a buffer of that size to store the events
			INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

			// Read the console events into that buffer, and save how
			// many events have been read into numEventsRead.
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

			// Now, cycle through all the events that have happened:
			for (DWORD i = 0; i < numEventsRead; ++i) {

				// Check the event type: was it a key?
				if (eventBuffer[i].EventType == KEY_EVENT) {

					// Yes! Was the key code the escape key?
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {

						// Yes, it was, so set the appIsRunning to false.
						appIsRunning = false;

						// Was if the 'c' key?
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {

						// Yes, so clear the buffer to spaces:
						for (int i = 0; i < 80 * 50; ++i) {
							consoleBuffer[i].Char.AsciiChar = ' ';
						}

						// Redraw our buffer:
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

					}

				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {

					// Set the index to our buffer of CHAR_INFO
					int offsetPos =
						eventBuffer[i].Event.MouseEvent.dwMousePosition.X
						+ 80 * eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

					// Is it a left click?
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {

						// Yep, so set with character 0xDB (solid block)
						consoleBuffer[offsetPos].Char.AsciiChar = (char)0xDB;

						// Redraw our buffer:
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

						// Is it a right click?
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {

						// Yep, so set with character 0xB1 (50% block)
						consoleBuffer[offsetPos].Char.AsciiChar = (char)0xB1;

						// Redraw our buffer:
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);

						// Is it a middle click?
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {

						// Yep, so set with character space.
						consoleBuffer[offsetPos].Char.AsciiChar = ' ';

						// Redraw our buffer:
						WriteConsoleOutputA(wHnd, consoleBuffer, charBufSize, characterPos, &writeArea);
					}
				}
			}

			// Clean up our event buffer:
			delete[] eventBuffer;
		}
	}
#endif
}