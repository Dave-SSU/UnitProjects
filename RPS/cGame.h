#pragma once
#include <string>

// forward declaration - only a cPlayer pointer is used here so don't need to include the .h file
class cPlayer;

// cGame class - this is the main class that manages the overall game and players.
//  The run() method is called from main and stays in a loop until the human player 
//	chooses to quit, or MAX_ROUNDS have been played (100).
class cGame
{
public:
	cGame();
	~cGame();
	int 		run();

private:
	void		round();

	cPlayer	*	pHuman;
	cPlayer	*	pBot;
	int			nRounds;

	const std::string	strWelcome;
	const std::string	strReady;
	const std::string	strWin;
	const std::string	strLose;
	const std::string	strDraw;
	const std::string	strStandings;
	const int			MAX_ROUNDS;
};

