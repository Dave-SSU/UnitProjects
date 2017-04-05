//-----------------------------------------------------------------------------
// RPS.cpp : Defines the entry point for the console application
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <iostream>
#include "cGame.h"

#pragma warning(push)
#pragma warning (disable: 4996)		// this pragma disables the warning for using _sleep(). I know it's outdated!


//-----------------------------------------------------------------------------
int main()
{
	std::cout << "ROCK - PAPER - SCISSORS" << std::endl;
	
	// create a game object. cGame is simple example of a "singleton" class:
	//	a class that is meant to be instantiated only once. 
	cGame game;
	int nRounds = game.run();

	std::cout << "You survived " << nRounds << " rounds. Bye ..." << std::endl;

	_sleep(900);	// short delay before the app shuts down completely
	return 0;
}

#pragma warning(pop)


