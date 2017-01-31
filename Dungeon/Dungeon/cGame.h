#include <string>
#include <vector>
#include "cGameSettings.h"
#include "cGameObj.h"

// forward declarations for systems
class cInput;
class cOutput;
class cMap;
class cStatus;
class cPlayer;

class cGame
{
public:
	cGame();
	~cGame();
    cGame&      init(cGameSettings& settings);
    void        reset();
    int			run();

	// Randomizer/seed as singleton system?

private:
	std::string	    strTitle;
    int             nMoves;
    bool			bQuit;

    cPlayer*        pPlayer;
    cInput*         pInput;
    cMap*           pMap;
	cStatus*	    pStatus;

    std::vector<cGameObj*>  vDynamics;
};


