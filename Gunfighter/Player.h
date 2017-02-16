#pragma once
#include "Character.h"
#include "Gun.h"

class Player :
	public Character
{
public:
	Player();
	~Player();			// this is a "Destructor". Ignore it for now
	bool	shoot();	// returns true if hit

private:
	Gun*	m_gunPtr = nullptr;
};

