#include "cVector2.h"


// internal only
//-----------------------------------------------------------------------------
cVector2::cVector2()
    : m_x(0), m_y(0)
{
}

//-----------------------------------------------------------------------------
cVector2::cVector2(int x, int y)
    : m_x(x), m_y(y)
{
}

//-----------------------------------------------------------------------------
cVector2::cVector2(const cVector2 & v)
    : m_x(v.m_x), m_y(v.m_y)
{
}

//-----------------------------------------------------------------------------
cVector2 &   cVector2::operator=(const cVector2 & v)
{
    m_x = v.m_x;
    m_y = v.m_y;
    return *this;
}

//-----------------------------------------------------------------------------
bool   cVector2::operator==(const cVector2 & v) const
{
    if (this->m_x == v.m_x && this->m_y == v.m_y)
        return true;
    return false;
}