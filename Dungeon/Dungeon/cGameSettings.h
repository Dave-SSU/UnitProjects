#pragma once

class cGameSettings
{
public:
	typedef enum
	{
		GC_Wall = 0,
		GC_Floor,
		GC_Boulder,
		GC_Food,
		GC_Player,
		GC_Mummy,
		NUM_GAME_CHARS
	} eGameChar;

    cGameSettings();
    ~cGameSettings();
    //static cGameSettings& getSettings()
    cGameSettings&	init(int argc, char* argv[]);
    char        getChar(eGameChar c);
    int         getWidth() const;
    int         getHeight() const;
    static      cGameSettings*   pSettings;

private:
	static char	gameChars[NUM_GAME_CHARS];
	bool	bRogue;			//	-r		roguelike chars
	int		nZombies;		//	-z##	number of zombies
	int		nEnergy;		//	-e##	energy bars
	int		nSpades;		//	-s##	spades
	int		nHoles;			//	-h##	holes
	int		nLives;			//	-l##	lives
	int		nWidth;			//	-d##,##	size: width, height - excluding bounding walls. max of 78 x 78.
	int		nHeight;
};


