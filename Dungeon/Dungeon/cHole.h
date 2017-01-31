#pragma once
#include "cGameObj.h"

class cHole : public cGameObj
{
public:
    cHole();
    virtual ~cHole();
private:
    cGameObj*   pObj;
};

