#include "stdafx.h"
#include "cShape.h"

// Note: I am being a bit lazy and putting all Shape classes in the same 
//	implementation file. If the classes were larger, this would be a bad idea,
//	but in this case it somewhat improves readability in that all classes can be
// easily compared.

// ----------------------------------------------------------------------------
cShape::cShape(const std::string& name, const std::string& beats, const std::string& wins)
	:strName(name), strBeats(beats), strWinHand(wins)
{
}

// ----------------------------------------------------------------------------
cShape::~cShape()
{
}

// ----------------------------------------------------------------------------
std::string&	cShape::getName() const
{
	return const_cast<std::string&>(strName);
}

// ----------------------------------------------------------------------------
std::string & cShape::getBeats() const
{
	return const_cast<std::string&>(strBeats);
}

// ----------------------------------------------------------------------------
std::string & cShape::getWinHand() const
{
	return const_cast<std::string&>(strWinHand);
}

// ----------------------------------------------------------------------------
bool	cShape::operator > (cShape& s)
{
	return (this->strBeats == s.getName()) ? true : false;
}

// ----------------------------------------------------------------------------
bool	cShape::operator < (cShape& s)
{
	return (this->strName == s.getBeats()) ? true : false;
}

// ----------------------------------------------------------------------------
bool	cShape::operator == (cShape& s)
{
	return (this->strName == s.getName()) ? true : false;
}

// ----------------------------------------------------------------------------
bool	cShape::operator != (cShape& s)
{
	return (this->strName != s.getName()) ? true : false;
}


// ----------------------------------------------------------------------------
cRock::cRock(const std::string& name) 
	: cShape(name, "Scissors", "Rock crushes Scissors!")  // initialize the base class through its constructor
{
}

// ----------------------------------------------------------------------------
cRock::~cRock()
{
}

// ----------------------------------------------------------------------------
cPaper::cPaper(const std::string& name)
	: cShape(name, "Rock", "Paper covers Rock!")  // initialize the base class through its constructor
{
}

// ----------------------------------------------------------------------------
cPaper::~cPaper()
{
}

// ----------------------------------------------------------------------------
cScissors::cScissors(const std::string& name)
	: cShape(name, "Paper", "Scissors cuts Paper!")
{
}

// ----------------------------------------------------------------------------
cScissors::~cScissors()
{
}
