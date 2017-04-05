#pragma once
#include "cPlayer.h"

class cHuman : public cPlayer
{
public:
	cHuman();
	~cHuman();
	virtual	bool	choose();
	virtual void	setQuit() { bQuit = true;  }
	virtual bool	hasQuit() { return bQuit; }
private:
	bool			bQuit;
	static const char	spinnerChars[5];	// static members means shared by
	static const char	backspaceChar;		//    all class instances
};

