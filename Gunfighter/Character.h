#pragma once
#include <string>

class Character
{
public:
	Character();
	~Character();	// this is a "Destructor". Ignore it for now
	std::string		getName();
	void			setName(const std::string& newName);
	int				getHealth();
	int 			changeHealth(int changeValue);

private:
	std::string	m_name;
	int			m_health;
	static int  Max_Health;
};

