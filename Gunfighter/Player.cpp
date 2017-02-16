#include <iostream>
#include "Player.h"


// ----------------------------------------------------------------------------
Player::Player()
{
}

// ----------------------------------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------------------------------
bool	Player::shoot()
{
	if (m_gunPtr == nullptr)
		std::cout << "I don't have a gun!" << std::endl;
	else
		std::cout << "Bang" << std::endl;
	return false;
}
