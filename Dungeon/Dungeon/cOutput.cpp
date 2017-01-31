#include "stdafx.h"
#include <iostream>
#include "cOutput.h"
#include "cPos.h"

cOutput *   cOutput::pOutput = nullptr;

const WORD attribute_colours[] =
{
    BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,                                                // BLACK_ON_GRAY,
    BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,                         // BLACK_ON_WHITE
    FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,                         // WHITE_ON_BLACK
    FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE,       // WHITE_ON_BLUE,
    FOREGROUND_RED | BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED,        // RED_ON_WHITE,
    FOREGROUND_RED,                                                                                     // RED_ON_BLACK,
};

// ----------------------------------------------------------------------------
cOutput::cOutput(HWND hWnd)
{
    cOutput::pOutput = this;
    this->hWnd = hWnd;
}

// ----------------------------------------------------------------------------
cOutput&    cOutput::initOutput(const cGameSettings& settings, HWND hWnd)
{
    if (nullptr != pOutput)
        return *pOutput;
 
    if (hWnd == 0)
        hWnd = static_cast<HWND>(GetStdHandle(STD_OUTPUT_HANDLE));

    pOutput = new cOutput(hWnd);
    pOutput->setupWindow(settings);
    return *pOutput;
}

// ----------------------------------------------------------------------------
cOutput::~cOutput()
{
    pOutput = nullptr;
    vConsoleBuffer.erase(vConsoleBuffer.begin(), vConsoleBuffer.end());
}

// ----------------------------------------------------------------------------
void    cOutput::setWindowTitle(const std::string&  title)
{
    strTitle = title;
    SetConsoleTitle(strTitle.c_str());
}

// ----------------------------------------------------------------------------
bool    cOutput::setupWindow(const cGameSettings& settings)
{
    displaySize = { 0, 0, (short)settings.getWidth()-1, (short)settings.getHeight()-1 };
    SetConsoleWindowInfo(hWnd, true, &displaySize);

    displayBufferSize = { (short)settings.getWidth(), (short)settings.getHeight() };
    SetConsoleScreenBufferSize(hWnd, displayBufferSize);

    vConsoleBuffer.reserve(displaySize.Right * displaySize.Bottom);
    // forcibly grow the buffer to thr right size
    CHAR_INFO clear;
    clear.Char.AsciiChar = ' ';
    for (int i = 0; i < displayBufferSize.X * displayBufferSize.Y; ++i)
        vConsoleBuffer.push_back(clear);

    return true;
}

// ----------------------------------------------------------------------------
void    cOutput::clearBuffer()
{
    CHAR_INFO clear;
    clear.Char.AsciiChar = ' ';
    clear.Attributes =
        BACKGROUND_BLUE |
        BACKGROUND_GREEN |
        BACKGROUND_RED; // |
       // BACKGROUND_INTENSITY;
    std::fill(vConsoleBuffer.begin(), vConsoleBuffer.end(), clear);
}

// ----------------------------------------------------------------------------
bool cOutput::setBufferChunk(const cPos& pos, const char * pChars, int count)
{
    if (pos.x < 0 || pos.x >= displaySize.Right ||
        pos.y < 0 || pos.y >= displaySize.Bottom)
    {
        std::cout << "Invalid arguments: file " << __func__ << " line " << __LINE__;
        return false;
    }

    int nStart = pos.y * (this->displaySize.Right + 1)+ pos.x;
    int nPos = 0;
    for (std::vector<CHAR_INFO>::iterator itr = vConsoleBuffer.begin() + nStart; itr != vConsoleBuffer.end() && nPos <= count; itr++, nPos++)
    {
        itr->Char.AsciiChar = *(pChars + nPos);
    }
    dirtyRect.Left = pos.x;
    dirtyRect.Top = pos.y;
    dirtyRect.Right = pos.x + count;
    dirtyRect.Bottom = pos.y + 1;
    return true;
}

// ----------------------------------------------------------------------------
bool cOutput::setBufferChar(const cPos& pos, const char& cChar, eCharAttribute attrib)
{
    int nStart = pos.y * (this->displaySize.Right + 1) + pos.x;
    vConsoleBuffer[nStart].Char.AsciiChar = cChar;
    
    std::vector<CHAR_INFO>::iterator itr = vConsoleBuffer.begin() + nStart;

    //itr->Char.AsciiChar = cChar;

    //static int c;
    //itr->Attributes = colors[c++ % 11];

    dirtyRect.Left = pos.x;
    dirtyRect.Top = pos.y;
    dirtyRect.Right = pos.x + 1;
    dirtyRect.Bottom = pos.y + 1;
    return true;
}

// ----------------------------------------------------------------------------
void    cOutput::display()
{
    // don't do anything unless there have been changes
    if (dirtyRect.Left == dirtyRect.Right && dirtyRect.Top == dirtyRect.Bottom)
        return;

    COORD characterPos = { 0, 0 };

    // for now, redraw the whole screen; no-sub rect dirty areas
    dirtyRect = displaySize;

    //WriteConsoleOutputA(hWnd, pConsoleBuffer, charBufSize, characterPos, &displaySize);
    WriteConsoleOutput(hWnd, vConsoleBuffer.data(), displayBufferSize, characterPos, &dirtyRect);
    dirtyRect.Left = 0;
    dirtyRect.Top = 0;
    dirtyRect.Right = 0;
    dirtyRect.Bottom = 0;
}
