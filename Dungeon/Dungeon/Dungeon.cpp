// Dungeon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		// probably

//#include <stdlib.h>
#include <stdio.h>
//#include <Windows.h>
#include <Tchar.h>
#include <iostream>
#include "cGameSettings.h"
#include "cGame.h"

#define debug(msg)  printf("DEBUG %s %s:%d: ", msg, \
     __FILE__, __LINE__)

// ----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    std::cout << std::endl << std::endl << std::endl << std::endl;
    debug("error here");
    std::cin;

    cGameSettings   settings;
    //settings.init(argc, argv);

	cGame	game;
	game.init(settings);
	return game.run();
}
