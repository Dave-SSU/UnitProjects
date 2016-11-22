#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <conio.h>

using namespace std;

const string WELCOME_STR = "Welcome to the Pit. Two enter, only one leaves.";
const string START_STR = "You may strike first.";
const string TURNPROMPT_STR = "Enter a command (1-5, q to quit)";
const string RECORD_STR = "Your record is: ";
const string VERSUS_STR = " vs ";
const string MISS_STR = "...miss";
const string HIT_STR = "...hit!";
const string NORMALSTR = "Normal attack";
const string HEAVY_STR = "Heavy attack";

enum INPUT { NORMAL=1, HEAVY, DEFEND, AMULET, COUNTER };

enum DAMAGE { NORMAL_DAMAGE = 20, HEAVY_DAMAGE = 30, DEFEND_HEALTH = 5, COUNTER_BOOST = 30 };

int main()
{
	unsigned int u = _getch();
	do {
		string s = "tweasdf";
		char& c = s.at(3);
		int n = s.size();
		u = s.length();

		std::cout << "integer";
		std::cin >> n;

		if (cin.fail())
		{
			string str;
			cin.clear();
			cin >> str;
			str.clear();
		}
	} while (u > 0);
	srand(time(0));



	bool bExitGame = false;
	bool bPlayerTurn = true;	// player moves first by default. randomize?
	int nPlayerWins = 0, nMonsterWins = 0;
	bool bPlayerAmuletUsed = false;
	bool bMonsterAmuletUsed = false;
	bool bPlayerCountered = false;
	bool bMonsterCountered = false;

	cout << WELCOME_STR << endl;
	cout << START_STR << endl;
	
	int nInput = 0;
	while (!bExitGame)
	{
		int nDamage = 0;

		if (bPlayerTurn)
		{
			cout << TURNPROMPT_STR << "  ";
			cin >> nInput;
			if (cin.fail())
			{
				cin.clear();
				char c;
				cin >> c;
				if ('q' == c)
				{
					bExitGame = true;
					break;
				}
			}

			switch (nInput)
			{
			case NORMAL:
				nDamage = NORMAL_DAMAGE;
			case HEAVY:
			case DEFEND:
				break;
			case AMULET:
				if (bPlayerAmuletUsed)
					bPlayerAmuletUsed = true;
				break;
			case COUNTER:
				bPlayerCountered = true;
				break;
			default:
				break;
			}
			bPlayerTurn = !bPlayerTurn;
		}
		else
		{
			nInput = rand() % 5 + 1;
		}

		//if (bMonsterAmuletUsed)
		//	bMonsterAmuletUsed = true;
		
	}

	cout << RECORD_STR << nPlayerWins << VERSUS_STR << nMonsterWins;

 
}

