#include "stdafx.h"
#include "cPos.h"


// ----------------------------------------------------------------------------
cPos::cPos()
    : x(0), y(0)
{
}

// ----------------------------------------------------------------------------
cPos::cPos(int x, int y)
	: x(x), y(y)
{
}

// ----------------------------------------------------------------------------
cPos::cPos(const cPos& pos)
	: x(pos.x), y(pos.y)
{
}

// ----------------------------------------------------------------------------
cPos&   cPos::operator=(const cPos& pos)
{
    if (this == &pos)
        return *this;
    this->x = pos.x;
    this->y = pos.y;
    return *this;
}

// ----------------------------------------------------------------------------
bool    cPos::operator==(const cPos& pos)
{
    if ((this == &pos) || (this->x == pos.x && this->y == pos.y))
        return true;
    return false;
}

// ----------------------------------------------------------------------------
bool	cPos::operator!=(const cPos& pos)
{
	return !(*this == pos);
}

// ----------------------------------------------------------------------------
cPos&	cPos::operator+=(const cPos& pos)
{
    this->x += pos.x;
    this->y += pos.y;
    return *this;
}

// ----------------------------------------------------------------------------
cPos&	cPos::operator-=(const cPos& pos)
{
	this->x -= pos.x;
	this->y -= pos.y;
    return *this;
}
