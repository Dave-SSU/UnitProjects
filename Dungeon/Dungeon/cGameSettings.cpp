#include "stdafx.h"
#include "cGameSettings.h"

cGameSettings*	cGameSettings::pSettings = nullptr;
char	cGameSettings::gameChars[NUM_GAME_CHARS] = 
{
	'W',		// Wall
	'-',		// Floor
	'B',		// Boulder
	'%',		// Food
	'P',		// Player
	'M',		// Mummy
};

// ----------------------------------------------------------------------------
cGameSettings::cGameSettings()
    : bRogue(false),	
    nZombies(3),
    nEnergy(4),
    nSpades(3),
    nHoles(1),
    nLives(3),	
    nWidth(80),	
    nHeight(40)
{
    pSettings = this;
}

// ----------------------------------------------------------------------------
cGameSettings::~cGameSettings()
{
    pSettings = nullptr;
}

// ----------------------------------------------------------------------------
cGameSettings&	cGameSettings::init(int argc, char* argv[])
{
    return *this;
}

// ----------------------------------------------------------------------------
char	cGameSettings::getChar(eGameChar c)
{
	return gameChars[c];
}

// ----------------------------------------------------------------------------
int cGameSettings::getWidth() const
{
    return nWidth;
}

// ----------------------------------------------------------------------------
int cGameSettings::getHeight() const
{
    return nHeight;
}

// command line: 
//	-r		roguelike chars
//	-z##	number of zombies
//	-e##	energy bars
//	-s##	spades
//	-h##	holes
//	-l##	lives
//	-d##,##	size: width, height - excluding bounding walls. max of 78 x 78.
