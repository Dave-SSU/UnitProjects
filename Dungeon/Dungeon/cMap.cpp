#include "stdafx.h"
#include <vector>
#include "cMap.h"
#include "cPos.h"
#include "cOutput.h"
#include "cMaze.h"

const int MAP_INSET_X = 10;
const int MAP_INSET_Y = 10;

// ----------------------------------------------------------------------------
cMap::cMap(const cPos& posOrigin, int width, int height)
	: origin(posOrigin.x, posOrigin.y), nWidth(width-MAP_INSET_X), nHeight(height- MAP_INSET_Y), bDirty(true)
{
    //wall.init('W'); // settings.getChar(cGameSettings::GC_Wall));
    wall.init((char) 178); // settings.getChar(cGameSettings::GC_Wall));
    floor.init('-'); // settings.getChar(cGameSettings::GC_Floor));
	start.init('S'); // settings.getChar(cGameSettings::GC_Floor));
	exit.init('E'); // settings.getChar(cGameSettings::GC_Floor));

    // maze generator algorithm requires an odd number of row and columns
    if (0 == nWidth % 2)    // or can check the lowest bit: odd if (nWidth & 0x01) 
        --nWidth;
    if (0 == nHeight % 2)
        --nHeight;

	contents.reserve(nWidth * nHeight);
    for (int i = 0; i < nWidth * nHeight; ++i)
        contents.push_back(&floor);
    generate();
}

// ----------------------------------------------------------------------------
cMap::~cMap()
{
    contents.erase(contents.begin(), contents.end());
}

// ----------------------------------------------------------------------------
cGameObj&   cMap::operator[](cPos& pos)
{
    int i = pos.y * (nWidth + 1) + pos.x;
    return *contents[i];
}

// ----------------------------------------------------------------------------
const cGameObj&   cMap::operator[](cPos& pos) const
{
    int i = pos.y * (nWidth + 1) + pos.x;
    return *contents[i];
}

// ----------------------------------------------------------------------------
void        cMap::generate()
{
    pMaze = new cMaze;
    if (nullptr != pMaze)
    {
        pMaze->printOn();
     
//        pMaze->create(rand(), nWidth, nHeight);
        while (1)
        {
            pMaze->create(rand(), (rand() % 15 + 10) | 0x1, (rand() % 15 + 10) | 0x1);
            cOutput& out = cOutput::getObj();
            out.fillScreen(' ', 0);
        }
        // copy the string data from the generated maze into internal format and buffer
        const std::vector<std::string>&   strs = pMaze->getStrings();

        auto mapContentsItr = contents.begin();
        
		for (auto strVectorItr : strs)
		{
			int count = 0;
			for (auto strItr : strVectorItr)
			{
				if (strItr == pMaze->getWallChar())
				{
					*(mapContentsItr)++ = &wall;
				}
				else if (strItr == pMaze->getStartChar())
				{
					*(mapContentsItr)++ = &start;
				}
				else if (strItr == pMaze->getExitChar())
				{
					*(mapContentsItr)++ = &exit;
				}
                else // treat as a floor tile otherwise: if (strItr == pMaze->getFloorChar())
                {
                    *(mapContentsItr)++ = &floor;
                }
                count++;
			}
			count = 0;
		}
        return;
    }

    // otherwise default to the simple map

    // initialize everything to floor first
    for (auto itr = contents.begin(); itr != contents.end(); ++itr)
        *(itr) = &floor;
    // set the perimeter to wall - top and bottom first
    for (int col = 0; col < this->nWidth; ++col)
    {
        contents[col] = &wall;
        contents[(nHeight-1)*nWidth+col] = &wall;
    }
    // sides
    for (int row = 1; row < this->nHeight-1; ++row)
    {
        contents[row * nWidth] = &wall;
        contents[row * nWidth + nWidth-1] = &wall;
    }
    bDirty = true;
}

// ----------------------------------------------------------------------------
void	    cMap::display()
{
    if (!bDirty)
        return;

    cOutput& out = cOutput::getObj();

    // copy map to buffer in rows; cOutput hides implementation of the linear buffer.
    cPos  pos = this->origin;
    for (int row = 0; row < this->nHeight; ++row, ++pos.y)
    {
        for (int col = 0; col < this->nWidth; ++col, ++pos.x)
        {
            out.setBufferChar(pos, contents[row * nWidth + col]->getChar());   // was pos.y * nWidth + pos.x
        }
        pos.x = this->origin.x;
    }
    bDirty = false;
}

// ----------------------------------------------------------------------------
bool        cMap::getRandomEmptyLocation(cPos& pos)
{
    const int MAX_ATTEMPTS = 20;
    int iterations = 0;
    do
    {
        pos.x = rand() % (nWidth - 2) + 1;
        pos.y = rand() % (nHeight - 2) + 1;
        if (contents[pos.y * nWidth + pos.x] == &floor)
            return true;
    } while (MAX_ATTEMPTS > iterations);
    return false;
}

// ----------------------------------------------------------------------------
void    cMap::placeObject(const cPos & pos, cGameObj& obj)
{
    contents[pos.y * nWidth + pos.x] = &obj;
    bDirty = true;
}
