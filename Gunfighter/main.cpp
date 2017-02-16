#include <iostream>
#include <conio.h>
#include "Player.h"

const int KEY_Esc = 27;

int main()
{
	Character	enemy;
	Player	player;

	bool quitGame = false;
	do
	{
		int c = _getch();		// _getch() reads a single keypress without waiting for a return
		switch (c)
		{
		case ' ':
			player.shoot();
			break;

		case KEY_Esc:
			quitGame = true;
			break;

		default:
			break;
		}

	} while (!quitGame);
}