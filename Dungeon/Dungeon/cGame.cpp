#include "stdafx.h"
#include <Windows.h>
#include "cGame.h"
#include "cGameSettings.h"
#include "cInput.h"
#include "cOutput.h"
#include "cGameObj.h"
#include "cMap.h"
#include "cStatus.h"
#include "cPlayer.h"
#include "cSpade.h"

const cPos posMapOrigin = { 4, 3 };		// top-right based offset for where the map is positioned
const cPos posStatus = { 4, 3 };		// note that for Status, pos.y is the lines up from bottom

cGame::cGame()
	: strTitle("Dave's Dungeon"),
    nMoves(0),
	bQuit(false),
    pInput(nullptr),
    pMap(nullptr),
    pPlayer(nullptr),
	pStatus(nullptr)
{
}

// ----------------------------------------------------------------------------
cGame::~cGame() 
{
    reset();
    if (nullptr != pPlayer)
        delete pPlayer;
    if (nullptr != pMap)
        delete pMap;
    if (nullptr != pStatus)
        delete pStatus;
}

// ----------------------------------------------------------------------------
void    cGame::reset()
{
    for (auto itr = vDynamics.begin(); itr != vDynamics.end(); itr++)
        delete (*itr);
    vDynamics.erase(vDynamics.begin(), vDynamics.end());
}

// ----------------------------------------------------------------------------
cGame&  cGame::init(cGameSettings& settings)
{
    cInput::initInput(settings);
    cOutput::initOutput(settings);
    
    pMap = new cMap(posMapOrigin, settings.getWidth(), settings.getHeight());
	pStatus = new cStatus(posStatus);
    pStatus->init();

    // same number of spades and mummies, random
    for (int r = (rand() % 5) + 2; r >= 0; r--)
    {
        cGameObj* pObj = new cSpade;

        vDynamics.push_back(pObj);
        cPos& pos = pObj->getPos();
        pMap->getRandomEmptyLocation(pos);
        pMap->placeObject(pos, *pObj);
    }

    pPlayer = new cPlayer;
    cInput& input = cInput::getObj();
    input.addKeyboardHandler(*pPlayer);
    cPos& pos = pPlayer->getPos();
    pMap->getRandomEmptyLocation(pos);
    pMap->placeObject(pos, *pPlayer);

    return *this;
}

// ----------------------------------------------------------------------------
int     cGame::run()
{
    // could make these pointers and add to class?
    cInput& input = cInput::getObj();
    cOutput& output = cOutput::getObj();

    while (!bQuit)
    {
        output.clearBuffer();
        // call display methods on the map & status elements to update output buffer
        pStatus->display();
        pMap->display();
        output.display();

        if (!input.isInputPending())
            continue;
        ++nMoves;
        for (INPUT_RECORD * pEvent = input.getEvent(); pEvent != nullptr; pEvent = input.getEvent())
            input.processEvent(*pEvent);
    }
	return 0;
}
