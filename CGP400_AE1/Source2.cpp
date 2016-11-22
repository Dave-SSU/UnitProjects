#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <conio.h>

using namespace std;

// ----------------------------------------------------------------------------
// Constant values declared here. Separation of code and data is a good idea. Constants instead of 
//	"magic" numbers and string literals is also a good practice.

const string NAME_STR = "What is your name?";
const string CALLYOU_STR = "Fine, I will call you ";
const string WELCOME_STR = ". Your opponent is ";
const string START_STR = "You may strike first.";
const string TURNPROMPT_STR = "Enter a command (1-5, or 6 to quit)";
const string AMULET_USED_STR = "Nothing happens.";
const string HEALTH_STR = " health = ";
const string WIN_STR = "You are victorious!";
const string LOSE_STR = "You are dead.";
const string CHOOSES_STR = " chooses ";
const string MISS_STR = "...miss";
const string HIT_STR = "...hit!";
const string NORMAL_STR = "Normal attack";
const string HEAVY_STR = "Heavy attack";
const string COUNTER_STR = "Counter attack";
const string AMULET_STR = "Use Amulet";
const string DEFEND_STR = "Defend";
const string COUNTER_DRAW = "You and your opponent size eachother up.";
const string QUITTER_STR = "You drop your weapon and flee like a coward";
const string EXIT_STR = "... Press any key to exit.";

const int HEAL_ON_DEFEND = 5;
const int MAX_HEALTH = 100;
const float AMULET_DAMAGE_BOOST = 1.5f;
const int DEFEND_HEAL = 5;

// ----------------------------------------------------------------------------
// enumerations

enum eMoveChoice { NONE = -1, NORMAL = 1, HEAVY, DEFEND, AMULET, COUNTER, NUM_MOVE_CHOICES=5, QUIT, COUNTER_ATTACK};
enum eHitChance { NORMAL_HITCHANCE = 80, HEAVY_HITCHANCE = 50, COUNTER_HITCHANCE = 65, COUNTER_HITCHANCE_BOOST = -20, DEFEND_HITCHANCE_BOOST = -30 };
enum eDamage { NORMAL_DAMAGE = 10, HEAVY_DAMAGE = 20, COUNTER_DAMAGE = 6 };
enum eAmuletState { UNUSED = 0, IN_USE, USED };
enum eCounterState { NO_COUNTER = 0, IN_COUNTER };

// ----------------------------------------------------------------------------
// Data structures

struct Warrior
{
	string			name = "Bob";
	int				health = MAX_HEALTH;
	eAmuletState	amuletState = UNUSED;
	eCounterState	counterState = NO_COUNTER;
	bool			isMyTurn = false;
	bool			isDefending = false;
};

// ----------------------------------------------------------------------------
// function prototypes

void			DisplayWarriorStats(const Warrior& warrior);
eMoveChoice		GetPlayerChoice();
eMoveChoice		GetMonsterChoice(bool amuletAlreadyUsed);
bool			Attack(Warrior& attacker, Warrior& defender, eHitChance eHitChance, eDamage eDamage);

// ----------------------------------------------------------------------------
int main()
{
	srand(time(0));

	Warrior monster;
	monster.name = "Dave";
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
		// On each turn the player and monster swap roles as attacker and defender
		Warrior& attacker = (player.isMyTurn) ? player : monster;
		Warrior& defender = (player.isMyTurn) ? monster : player;

		// reset variables that track states to normal values
		attacker.isDefending = false;

		// attack handling part
		eMoveChoice choice;
		if (player.isMyTurn)
			choice = GetPlayerChoice();
		else
			choice = GetMonsterChoice(UNUSED != monster.amuletState);
		cout << attacker.name << CHOOSES_STR;

		switch (choice)
		{
		case NORMAL:
			cout << NORMAL_STR;
			Attack(attacker, defender, NORMAL_HITCHANCE, NORMAL_DAMAGE);
			break;

		case HEAVY:
			cout << HEAVY_STR;
			Attack(attacker, defender, HEAVY_HITCHANCE, HEAVY_DAMAGE);
			break;

		case DEFEND:	// take no action, but heal 5 points, up to max; boost to defense
			cout << DEFEND_STR << endl;
			attacker.health += DEFEND_HEAL;
			if (attacker.health > MAX_HEALTH)
				attacker.health = MAX_HEALTH;
			attacker.isDefending = true;
			break;

		case AMULET:	// note the amulet is in use, but don't attack immediately
			cout << AMULET_STR << endl;
			if (UNUSED == attacker.amuletState)
				attacker.amuletState = IN_USE;
			else
				cout << AMULET_USED_STR;
			continue;	// use CONTINUE instead of break so the loop is repeated! Still the attacker's turn!

		case COUNTER:	// counter attack initiated
			cout << COUNTER_STR << endl;
			attacker.counterState = IN_COUNTER;
			break;

		case QUIT:
			cout << QUITTER_STR << EXIT_STR;
			bExitGame = true;
			_getch();
			return 0;
		default:
			break;
		}

		// Check for counter-attack now being launched by the defender.
		//	Follow-up when previous attacker chose to counter
		if (IN_COUNTER == defender.counterState)
		{
			if (IN_COUNTER == attacker.counterState)
			{
				cout << COUNTER_DRAW << endl;
				attacker.counterState = defender.counterState = NO_COUNTER;
				continue;
			}
			else
			{
				// special case ... roles are swapped
				Attack(defender, attacker, COUNTER_HITCHANCE, COUNTER_DAMAGE);
			}
		}

		// check if either warrior is dead
		if (monster.health <= 0)
		{
			cout << defender.name << " is dead" << endl;
			cout << WIN_STR << EXIT_STR;
			_getch();
			break;
		}
		else if (player.health <= 0)
		{
			cout << LOSE_STR << EXIT_STR;
			_getch();
			break;
		}

		// End the turn; swap roles
		attacker.isMyTurn = !attacker.isMyTurn;
		defender.isMyTurn = !defender.isMyTurn;

		DisplayWarriorStats(player);
		DisplayWarriorStats(monster);

	} while (!bExitGame);
}

// ----------------------------------------------------------------------------
void DisplayWarriorStats(const Warrior& warrior)
{
	cout << warrior.name << HEALTH_STR << warrior.health << endl;
}

// ----------------------------------------------------------------------------
eMoveChoice GetPlayerChoice()
{
	cout << NORMAL << ". " << NORMAL_STR << endl;
	cout << HEAVY << ". " << HEAVY_STR << endl;
	cout << DEFEND << ". " << DEFEND_STR << endl;
	cout << AMULET << ". " << AMULET_STR << endl;
	cout << COUNTER << ". " << COUNTER_STR << endl;

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

// ----------------------------------------------------------------------------
eMoveChoice GetMonsterChoice(bool amuletAlreadyUsed)
{
	int choice = 0;
	do 
	{
		choice = rand() % (NUM_MOVE_CHOICES - 1) + 1;
	} while (choice == AMULET && amuletAlreadyUsed);
	return (eMoveChoice) choice;	// force the choice to be treated as eMoveChoice type
}

// ----------------------------------------------------------------------------
bool Attack(Warrior& attacker, Warrior& defender, eHitChance baseHitChance, eDamage baseDamage)
{
	// calculate if the attack hits, based on attack type, 
	int hitChance = baseHitChance;
	if (defender.counterState)
		hitChance += COUNTER_HITCHANCE_BOOST;	// negative value, so substracted from chances
	else if (defender.isDefending)
		hitChance += DEFEND_HITCHANCE_BOOST;	// negative value, so substracted from chances

	bool didAttackHit = rand() % 100 < hitChance;
	if (didAttackHit)
	{
		// calculate damage
		int damage = baseDamage;
		if (IN_USE == attacker.amuletState)
		{
			damage = (int)(damage  * AMULET_DAMAGE_BOOST);
			attacker.amuletState = USED;
		}
		defender.health -= damage;
		cout << HIT_STR << endl;
	}
	else
	{
		cout << MISS_STR << endl;
	}
	return didAttackHit;
}
