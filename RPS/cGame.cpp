#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "cGame.h"
#include "cHuman.h"
#include "cRandBot.h"
#include "cShape.h"

// ----------------------------------------------------------------------------
// Game class constructor. Constant values are set using the initialisation list
cGame::cGame()
	: pHuman(nullptr), pBot(nullptr), nRounds(0),
	strWelcome("Try your luck, test your wits!  Press:\r\n  r for Rock\r\n  p for Paper\r\n  s for Scissors\r\n  q to quit\r\n"),
	strReady("Ready?...."),
	strWin("You win"),
	strLose("You lose"),
	strDraw("You have drawn..."),
	strStandings("Record: "),
	MAX_ROUNDS(100)

{
	// seed the random number generator
	srand(time(NULL));

	pHuman = new cHuman();
	pBot = new cRandBot();
}

// ----------------------------------------------------------------------------
// class destructor. Delete the human and bot players that were created in the Constructor
cGame::~cGame()
{
	if (pHuman)
	{
		delete pHuman;
		pHuman = nullptr;
	}
	if (pBot)
	{
		delete pBot;
		pBot = nullptr;
	}
}

// ----------------------------------------------------------------------------
// the run() method holds the game loop and displays the results of each round
int  cGame::run()
{
	std::cout << strWelcome;
	while (nRounds++ < MAX_ROUNDS && !pHuman->hasQuit())
	{
		// new round starting

		if ((false == pHuman->choose()) || (pHuman->hasQuit()))
			break;

		pBot->choose();

		// compare hands
		cShape * pHumanChoice = pHuman->getChoice();
		std::cout << pHuman->getNameStr() << " " << pHuman->getChoiceStr() << " " << pHumanChoice->getName() << std::endl;

		cShape * pBotChoice = pBot->getChoice();
		std::cout << pBot->getNameStr() << " " << pBot->getChoiceStr() << " " << pBotChoice->getName() << std::endl;

		if (*pHumanChoice > *pBotChoice)
		{
			std::cout << pHumanChoice->getWinHand() << std::endl;
			std::cout << cGame::strWin << std::endl;
			pHuman->win();
		}
		else if (*pHumanChoice < *pBotChoice)
		{
			std::cout << pBotChoice->getWinHand() << std::endl;
			std::cout << cGame::strLose << std::endl;
			pBot->win();
		}
		else if (*pHumanChoice == *pBotChoice)
		{
			std::cout << cGame::strDraw << std::endl;
		}

		std::cout << strStandings << pHuman->getWins() << " - " << pBot->getWins() << "  Ready to try again? " << std::endl << std::endl;
	}
	return nRounds;
}

// ----------------------------------------------------------------------------
// the round method just gets the players' decisions for each round.
void	cGame::round()
{
	std::cout << cGame::strReady << std::flush;
	pBot->choose();
	pHuman->choose();
}

