#pragma once
#include <string>
class Gun
{
public:
	Gun();
	~Gun();		// this is a "Destructor". Ignore it for now
	void		fire();
	std::string name = "none";
};

