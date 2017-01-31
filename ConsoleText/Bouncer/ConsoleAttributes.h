#pragma once
#include <wincon.h>

enum ATTRIBUTE_COLOUR
{
	WHITE_ON_BLACK  = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED_ON_BLACK	= FOREGROUND_INTENSITY | FOREGROUND_RED,
	GREEN_ON_BLACK	= FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	YELLOW_ON_BLACK = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	BLUE_ON_BLACK	= FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	MAGENTA_ON_BLACK= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	CYAN_ON_BLACK	= FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	BLACK_ON_GRAY	= BACKGROUND_INTENSITY | BACKGROUND_INTENSITY,
	BLACK_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	RED_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED,
	GREEN_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN,
	YELLOW_ON_WHITE = BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN,
	BLUE_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE,
	MAGENTA_ON_WHITE= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE,
	CYAN_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE,
	WHITE_ON_WHITE	= BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

const WORD ATTRIBUTES[ ] = 
{
	// White on Black
	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	// Red on Black
	FOREGROUND_INTENSITY | FOREGROUND_RED,
	// Green on Black
	FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	// Yellow on Black
	FOREGROUND_INTENSITY | FOREGROUND_RED |	FOREGROUND_GREEN,
	// Blue on Black
	FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	// Magenta on Black
	FOREGROUND_INTENSITY | FOREGROUND_RED |	FOREGROUND_BLUE,	
	// Cyan on Black
	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	
	// Black on Gray
	BACKGROUND_INTENSITY | BACKGROUND_INTENSITY,

	// Black on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,	
	// Red on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |	FOREGROUND_RED,
	// Green on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN,
	// Yellow on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN,
	// Blue on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE,	
	// Magenta on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE,
	// Cyan on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_BLUE,
	// White on White
	BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};