#pragma once

#include "cGameObj.h"

class cTorch : public cGameObj
{
public:
    cTorch();
    virtual ~cTorch();
    void    collect();
    bool    use();
    int     getUses() { return nUses; }
private:
    int     nUses = 1;
};

