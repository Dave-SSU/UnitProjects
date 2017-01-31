#include "stdafx.h"
#include "cPlayer.h"


// ----------------------------------------------------------------------------
cPlayer::cPlayer()
    : cInputHandler(), cGameObj()
{
    displayChar = '@';
}

// ----------------------------------------------------------------------------
cPlayer::~cPlayer()
{
}

// ----------------------------------------------------------------------------
bool        cPlayer::handler(const INPUT_RECORD& input)
{
    switch (input.Event.KeyEvent.wVirtualKeyCode)
    {
    case VK_LEFT:
        return true;
    case VK_UP:
        return true;
    case VK_RIGHT:
        return true;
    case VK_DOWN:
        return true;
    case VK_SPACE:
        return true;
    default:
        break;
    }
    return false;
}

// ----------------------------------------------------------------------------
void        cPlayer::init()
{
    
}

// ----------------------------------------------------------------------------
void        cPlayer::resetPos(const cPos& newPos)
{
    this->pos = newPos;
}

// ----------------------------------------------------------------------------
