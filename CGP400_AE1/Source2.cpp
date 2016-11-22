#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <conio.h>

using namespace std;

// Constant values declared here. Separation of code and data is a good idea. Constants instead of 
//	"magic" numbers and string literals is also a good practice.
const string NAME_STR = "What is your name?";
const string CALLYOU_STR = "Fine, I will call you ";
const string WELCOME_STR = ". Your opponent is ";
const string START_STR = "You may strike first.";
const string TURNPROMPT_STR = "Enter a command (1-5, or 6 to quit)";
const string WIN_STR = "You are victorious!";
const string LOSE_STR = "You are dead.";
const string VERSUS_STR = " vs ";
const string MISS_STR = "...miss";
const string HIT_STR = "...hit!";
const string NORMALSTR = "Normal attack";
const string HEAVY_STR = "Heavy attack";
const string QUITTER_STR = "You drop your weapons and flee like the coward you are... Press any key to exit.";

const int HEAL_ON_DEFEND = 5;
const int MAX_HEALTH = 100;
const float AMULET_DAMAGE_BOOST = 1.5f;

enum eMoveChoice { NONE = -1, NORMAL = 1, HEAVY, DEFEND, AMULET, COUNTER, NUM_MOVE_CHOICES=5, QUIT };
enum eHitChance { NORMAL_eHitChance = 80, HEAVY_eHitChance = 50, DEFEND_eHitChance = 30, COUNTER_eHitChance = 65 };
enum eDamage { NORMAL_eDamage = 20, HEAVY_eDamage = 30, DEFEND_HEALTH = 5, COUNTER_BOOST = 30 };
enum eAmuletState { UNUSED = 0, IN_USE, USED };
enum eCounterState { NO_COUNTER = 0, IN_COUNTER, COUNTER_APPLIED};

struct Warrior
{
	string	name = "Bob";
	int		health = MAX_HEALTH;
	eAmuletState	amuletState = UNUSED;
	eCounterState	counterState = NO_COUNTER;
	eMoveChoice		choice = NONE;
	bool			isMyTurn = false;
};

void			DisplayWarriorStats(const Warrior& warrior);
eMoveChoice		GetPlayerChoice();
eMoveChoice		GetMonsterChoice();
bool			Attack(Warrior& attacker, Warrior& defender, eHitChance eHitChance, eDamage eDamage);

int main()
{
	srand(time(0));

	Warrior monster;
	monster.name = "Ogre";
	Warrior player;
	player.isMyTurn = true;		// player moves first by default. 

	// get the player name
	std::cout << NAME_STR << "  ";
	std::cin >> player.name;

	// check for an error reading the. Don't ask for a new name though - just resort to the default.
	if (cin.fail())
	{
		string str;
		cin.clear();
		getline(cin, str);
	}
	cout << CALLYOU_STR << player.name;
	cout << WELCOME_STR << monster.name << endl;
	cout << START_STR << endl;

	bool bExitGame = false;

	DisplayWarriorStats(player);
	DisplayWarriorStats(monster);
	
	do
	{
		Warrior& attacker = (player.isMyTurn) ? player : monster;
		Warrior& defender = (player.isMyTurn) ? monster : player;

		if (player.isMyTurn)
			player.choice = GetPlayerChoice();
		else
			monster.choice = GetMonsterChoice();

		switch (attacker.choice)
		{
		case NORMAL:
			Attack(attacker, defender, NORMAL_eHitChance, NORMAL_eDamage);
			break;
		case HEAVY:
			Attack(attacker, defender, HEAVY_eHitChance, HEAVY_eDamage);
			break;
		case DEFEND:	// take no action, but heal 5 points, up to max; boost to defense
			attacker.health += HEAL_ON_DEFEND;
			attacker.health = (HEAL_ON_DEFEND > MAX_HEALTH) ? MAX_HEALTH : attacker.health;
			break;
		case AMULET:
			if (UNUSED == attacker.amuletState)
				attacker.amuletState = IN_USE;
			continue;	// use CONTINUE instead of break so the loop is repeated! Still the attacker's turn!
		case COUNTER:
			attacker.counterState = IN_COUNTER;
			break;
		case QUIT:
			cout << QUITTER_STR;
			bExitGame = true;
			_getch();
			return 0;
		default:
			break;
		}

		if (defender.health <= 0)
		{
			cout << defender.name << "is dead" << endl;
		}
		else
		{
			// swap turns
			attacker.isMyTurn = !attacker.isMyTurn;
			defender.isMyTurn = !defender.isMyTurn;
		}
		DisplayWarriorStats(player);
		DisplayWarriorStats(monster);

	} while (!bExitGame);

	cout << WIN_STR;
}

void DisplayWarriorStats(const Warrior& warrior)
{
	cout << warrior.name << " " << warrior.health << endl;
}

eMoveChoice GetPlayerChoice()
{
	int choice;
	cout << TURNPROMPT_STR << "  ";
	cin >> choice;
	if (cin.fail())
	{
		cin.clear();
		char c;
		cin >> c;
	}
	return (eMoveChoice) choice;
}

eMoveChoice GetMonsterChoice()
{
	int choice = rand() % (NUM_MOVE_CHOICES-1) + 1;
	return (eMoveChoice) choice;	// force the choice to be treated as eMoveChoice type
}

bool Attack(Warrior& attacker, Warrior& defender, eHitChance baseHitChance, eDamage baseDamage)
{
	int hitChance = baseHitChance;
	if (defender.counterState)
		hitChance -= 
	else
		hitChance -= 

	bool didAttackHit = rand() % 100 < hitChance;
	if (didAttackHit)
	{
		int damage = baseDamage;
		if (IN_USE == attacker.amuletState)
			damage *= AMULET_DAMAGE_BOOST;
		defender.health -= damage;
	}
	return didAttackHit;
}
