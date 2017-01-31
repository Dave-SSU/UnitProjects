#pragma once


class cPos
{
public:
    int x;
    int y;
    cPos();
    cPos(int _x, int _y);
    cPos(const cPos& pos);
    cPos&   operator=(const cPos& pos);
    bool    operator==(const cPos& pos);
	bool	operator!=(const cPos& pos);
	cPos&	operator+=(const cPos& pos);
	cPos&	operator-=(const cPos& pos);
};


