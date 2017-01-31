#pragma once
#include "cInput.h"
#include "cGameObj.h"

class cPlayer :  public cInputHandler, public cGameObj
{
public:
    cPlayer();
    virtual ~cPlayer();
    virtual bool handler(const INPUT_RECORD& event);
    void        init();
    void        resetPos(const cPos& newPos);
private:
    int         nFood = 0;
    int         nSpades = 0;
};

