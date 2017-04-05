#include "stdafx.h"
#include "cRandBot.h"


//-----------------------------------------------------------------------------
cRandBot::cRandBot()
{
	name = "Random";
	choice = "chooses";
}


//-----------------------------------------------------------------------------
cRandBot::~cRandBot()
{
}

//-----------------------------------------------------------------------------
bool cRandBot::choose()
{
	setShape(rand() % 3);
	return (bool)(nullptr != pChosenShape);;
}
