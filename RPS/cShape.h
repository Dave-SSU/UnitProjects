#pragma once
#include <string>

// cShape is the base class for the Rock, Paper and Scissors classes
// not instantiable as it contains pure virtual methods. Child classes must
//	override and implement the pure functions
class cShape
{
public:
	virtual ~cShape();
	std::string& getName() const;
    std::string& getBeats() const;
	std::string& getWinHand() const;

	// overloaded operators: use to compare the shapes
	bool operator > (cShape& s);
	bool operator < (cShape& s);
	bool operator == (cShape& s);
	bool operator != (cShape& s);

protected:
	cShape(const std::string& name, const std::string& beats="beats", const std::string& wins="wins");	// hide the default constructor!

	const std::string		strName;	// name of the object - set by derived class
	const std::string		strBeats;	// name of object this one beats
	const std::string		strWinHand;	// string displayed on winning

private:
	cShape() {}	// hide the default constructor from unintentional use
};


class cRock : public cShape
{
public:
	cRock(const std::string& name = "Rock");	// constructor parameter allows player name their rock
	virtual ~cRock();

};

class cPaper : public cShape
{
public:
	cPaper(const std::string& name = "Paper");
	virtual ~cPaper();
};

class cScissors : public cShape
{
public:
	cScissors(const std::string& name = "Scissors");
	virtual ~cScissors();
};