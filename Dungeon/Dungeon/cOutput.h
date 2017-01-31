#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "cGameSettings.h"
#include "cPos.h"

typedef enum
{
    BLACK_ON_GRAY,
    BLACK_ON_WHITE,
    WHITE_ON_BLACK,
    WHITE_ON_BLUE,
    RED_ON_WHITE,
    RED_ON_BLACK,

} eCharAttribute;


class cOutput
{
public:
    static cOutput&     initOutput(const cGameSettings& settings, HWND hWnd=0);
    static cOutput&     getObj() { return *pOutput; }
    ~cOutput();
    void                setWindowTitle(const std::string& title);
    bool                setupWindow(const cGameSettings& settings);
    void                clearBuffer();
    bool                setBufferChunk(const cPos& pos, const char* pChars, int count);
    bool                setBufferChar(const cPos& pos, const char& cChar, eCharAttribute attrib=BLACK_ON_GRAY);
    void                display();
    void                fillScreen(const char fillChar, const WORD attribute);

private:
    cOutput(HWND hWnd);                         // private constructor!
    HWND				hWnd;
    static cOutput*		pOutput;
    std::string         strTitle;
    SMALL_RECT          displaySize;
    COORD               displayBufferSize;
    SMALL_RECT          dirtyRect;
    std::vector<CHAR_INFO>  vConsoleBuffer;
};

