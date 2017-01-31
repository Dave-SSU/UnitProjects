#pragma once

#include <vector>
#include "cGameObj.h"
#include "cPos.h"

// external maze class
#include "cMaze.h"


class cMap
{
public:
	cMap(const cPos& posOrigin, int width, int height);
    ~cMap();
    cGameObj&   operator[](cPos& pos);
    const cGameObj&   operator[](cPos& pos) const;

    void        generate();
    void        display();                  // draw map to the output buffer
    bool        getRandomEmptyLocation(cPos& pos);
    void        placeObject(const cPos& pos, cGameObj& obj);
    void        save();
    cPos&       load();

private:
	cPos	    origin;                     // offset position of top-left of map with the window 
    int         nWidth;                     // width of map in chars
    int         nHeight;                    // height of map in chars
    bool        bDirty;
    std::vector<cGameObj*>  contents;
    //typedef std::vector<cGameObj*>::iterator _MAPITR;
    cGameObj    wall;
    cGameObj    floor;                     // non-interactive

    cMaze *     pMaze = nullptr;
};


