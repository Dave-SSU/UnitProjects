#include "Character.h"

int Character::Max_Health = 100;

// ----------------------------------------------------------------------------
Character::Character()
{
}

// ----------------------------------------------------------------------------
Character::~Character()
{
}

// ----------------------------------------------------------------------------
std::string Character::getName()
{
	return m_name;
}

void Character::setName(const std::string& newName)
{
	m_name = newName;
}

// ----------------------------------------------------------------------------
int Character::getHealth()
{
	return m_health;
}

// ----------------------------------------------------------------------------
int  Character::changeHealth(int changeValue)
{
	m_health += changeValue;
	return m_health;
}

