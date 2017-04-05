#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include "cHuman.h"
#include "cShape.h"

#pragma warning(push)
#pragma warning (disable: 4996)		// this pragma disables the warning for using _sleep(). I know it's outdated!

// declare the static data members here
const char cHuman::spinnerChars[] = { "|/-\\" };
const char cHuman::backspaceChar = '\b';

//-----------------------------------------------------------------------------
cHuman::cHuman()
: bQuit(false)
{
	name = "Human";
	choice = "chooses";
}

//-----------------------------------------------------------------------------
cHuman::~cHuman()
{
}

// ----------------------------------------------------------------------------
bool	cHuman::choose()
{
	// animate the cursor while waiting...
	// draw char, check for input, wait x, repeat

	reset();
	char choice = 0;
	int i = 0;
	do
	{
		_sleep(1);
		std::cout << backspaceChar << spinnerChars[i++ % 4] << std::flush;

		if (kbhit())
		{
			choice = _getch();
			if ('q' == choice)
				return 0;
			setShape(choice);
			choice = 0;
		}
	} while (nullptr == pChosenShape);

	std::cout << backspaceChar;

	return (bool)(nullptr != pChosenShape);
}

#pragma warning(pop)



