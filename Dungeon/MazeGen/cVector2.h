#pragma once

// Internal Vector2 class is used to manage map coordinates. Always keeping the 
//  row and col values together just makes life easier and shows a simple utility class
//  in action.
class cVector2
{
public:
    cVector2();
    virtual ~cVector2() {}
    cVector2(int x, int y);
    cVector2(const cVector2& v);
    cVector2& operator=(const cVector2& v);
    bool operator==(const cVector2 & v) const;
    int     m_x{ 0 }, m_y{ 0 };
};
