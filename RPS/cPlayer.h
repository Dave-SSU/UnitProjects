#pragma once
#include <string>

// forward declaration - only a pointer is used here so don't need to include the .h file
class cShape;

// player class - not instantiable as it contains pure virtual methods. Child classes must
//	override and implement the pure functions
class cPlayer
{
public:
	cPlayer();
	virtual ~cPlayer();
	virtual bool	choose() = 0;		// pure virtual methods defines the child class interface
	virtual void	setQuit() = 0;		
	virtual bool	hasQuit() = 0;
	virtual cShape*	getChoice();
	virtual std::string& getNameStr() const;
	virtual std::string& getChoiceStr() const;
	virtual int		getWins();
	virtual	int		win();
	virtual void	reset();

private:
	int				wins;

protected:
	virtual cShape*	setShape(char choice);
	virtual cShape*	setShape(int choice);
	std::string		name;
	std::string		choice;
	cShape*			pChosenShape;
	cShape*			pRock;
	cShape*			pPaper;
	cShape*			pScissors;
};

