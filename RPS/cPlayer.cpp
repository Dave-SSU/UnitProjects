#include "stdafx.h"
#include <iostream>
#include "cPlayer.h"
#include "cShape.h"


// ----------------------------------------------------------------------------
cPlayer::cPlayer()
	: name(""), choice(""), pChosenShape(nullptr), wins(0)
{
	pRock = new cRock("Rock");
	pPaper = new cPaper("Paper");
	pScissors = new cScissors("Scissors");
}

// ----------------------------------------------------------------------------
cPlayer::~cPlayer()
{
	delete pRock;
	delete pPaper;
	delete pScissors;
}

cShape * cPlayer::getChoice()
{
	return pChosenShape;
}

// ----------------------------------------------------------------------------
std::string&	cPlayer::getNameStr() const
{
	return const_cast<std::string&>(name);
}

// ----------------------------------------------------------------------------
std::string&	cPlayer::getChoiceStr() const
{
	return const_cast<std::string&>(choice);
}

// ----------------------------------------------------------------------------
int cPlayer::getWins()
{
	return this->wins;
}

// ----------------------------------------------------------------------------
int cPlayer::win()
{
	return ++this->wins;
}

// ----------------------------------------------------------------------------
void cPlayer::reset()
{
	pChosenShape = nullptr;
}

// ----------------------------------------------------------------------------
cShape*			cPlayer::setShape(char choice)
{
	switch (choice)
	{
	case 'R':
	case 'r':
		pChosenShape = pRock;
		break;
	case 'P':
	case 'p':
		pChosenShape = pPaper;
		break;
	case 'S':
	case 's':
		pChosenShape = pScissors;
		break;
	case 'Q':
	case 'q':
		setQuit();
		break;
	}
	return pChosenShape;
}

// ----------------------------------------------------------------------------
cShape*			cPlayer::setShape(int choice)
{
	switch (choice)
	{
	case 0:
		pChosenShape = pRock;
		break;
	case 1:
		pChosenShape = pPaper;
		break;
	case 2:
		pChosenShape = pScissors;
		break;
	}
	return pChosenShape;
}

