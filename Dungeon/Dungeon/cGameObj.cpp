#include "stdafx.h"
#include "cGameObj.h"


// ----------------------------------------------------------------------------
cGameObj::cGameObj()
{
}


// ----------------------------------------------------------------------------
cGameObj::~cGameObj()
{
}

// ----------------------------------------------------------------------------
void    cGameObj::init(char c, eCharAttribute attrib)
{
    displayChar = c;
    displayAttrib = attrib;
}

// ----------------------------------------------------------------------------
void    cGameObj::setPos(const cPos & newPos)
{
    pos = newPos;
}
