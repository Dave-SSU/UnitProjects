#pragma once
#include "cPlayer.h"

class cRandBot : public cPlayer
{
public:
	cRandBot();
	virtual ~cRandBot();
	virtual bool	choose();
	virtual void	setQuit() { }
	virtual bool	hasQuit() { return false; }
};

