#include "stdafx.h"
#include <string>
#include "cStatus.h"
#include "cOutput.h"

// ----------------------------------------------------------------------------
cStatus::cStatus(const cPos& posOrigin)
    : origin(posOrigin),
    vElements(TID_End),
    bDirty(false)
{
}

// ----------------------------------------------------------------------------
void    cStatus::init()
{
	_TextElement	element;

	element.init("Player", 1, 1);
	vElements[TID_PlayerHealth] = element; 
	element.init("Inventory", 1, 38);
	vElements[TID_Inventory] = element;
	element.init("msg0", 40, 36);
	vElements[TID_Message0] = element;
	element.init("msg1", 40, 37);
	vElements[TID_Message1] = element;
	element.init("msg2", 40, 38);
	vElements[TID_Message2] = element;
}

// ----------------------------------------------------------------------------
cStatus::~cStatus()
{
	vElements.erase(vElements.cbegin(), vElements.end());
}

// ----------------------------------------------------------------------------
void	cStatus::display()
{
    if (!bDirty)
        return;
	cOutput& out = cOutput::getObj();
	for (std::vector<_TextElement>::const_iterator itr = vElements.cbegin(); itr != vElements.cend(); ++itr)
	{
		out.setBufferChunk(itr->pos, itr->text.data(), itr->text.length());
	}
    bDirty = false;
}

// ----------------------------------------------------------------------------
void  _TextElement::init(std::string str, int x, int y)
{
    set(str);
    pos.x = x;
    pos.y = y;
}

// ----------------------------------------------------------------------------
void  _TextElement::set(std::string str)
{
    text = str;
}
