#pragma once

#include "cPos.h"
#include "cOutput.h"

class cGameObj
{
public:
    cGameObj();
    virtual ~cGameObj();
    void    init(char c, eCharAttribute attrib=BLACK_ON_WHITE);
    void    setPos(const cPos & newPos);
    cPos&   getPos() { return pos; }
    char    getChar() { return displayChar; }
protected:
    char        displayChar = 'Z';
    eCharAttribute displayAttrib = BLACK_ON_WHITE;
    cPos        pos = { -1, -1 };
};

