#pragma once
#include "cGameObj.h"

class cSpade : public cGameObj
{
public:
    cSpade();
    virtual ~cSpade();
    void    collect();
    bool    use();
    int     getUses() { return nUses; }
private:
    int     nUses = 1;
};

