#include "stdafx.h"
#include "cInput.h"
#include "cGameSettings.h"

cInput *	cInput::pInput = nullptr;


// Note: this class is not thread safe!

// Important to limit the key input rate - maybe limit to a single key per frame?
//   Check into the repeat rate for the console - change programatically

// ----------------------------------------------------------------------------
cInput::cInput(HWND hWnd)
	: dwEvents(0), dwMaxEvents(MAX_EVENTS)
{
	cInput::pInput = this;
	hWndInput = hWnd;
    clearInput();
}

// ----------------------------------------------------------------------------
void cInput::clearInput()
{
    ZeroMemory(inputBuffer, sizeof(inputBuffer));
    nNextEvent = 0;
}

// ----------------------------------------------------------------------------
cInput&  cInput::initInput(const cGameSettings& settings, HWND hWnd)
{
    if (nullptr != pInput)
        return *pInput;

    if (hWnd == 0)
        hWnd = static_cast<HWND>(GetStdHandle(STD_INPUT_HANDLE));

    pInput = new cInput(hWnd);
    return *pInput;
}

// ----------------------------------------------------------------------------
cInput::~cInput()
{
    kbEventHandlers.erase(kbEventHandlers.begin(), kbEventHandlers.end());
    mouseEventHandlers.erase(mouseEventHandlers.begin(), mouseEventHandlers.end());
    pInput = nullptr;
}

// ----------------------------------------------------------------------------
bool	cInput::isInputPending()
{
    GetNumberOfConsoleInputEvents(hWndInput, &dwEvents);
    if (dwEvents > 0)
    {
        clearInput();
        ReadConsoleInput(hWndInput, inputBuffer, dwMaxEvents, &dwEvents);
    }
    return dwEvents > 0;
}


// ----------------------------------------------------------------------------
void    cInput::addKeyboardHandler(cInputHandler& handlerObj)
{
    kbEventHandlers.push_back(&handlerObj);
    auto itr = kbEventHandlers.begin();
}

// ----------------------------------------------------------------------------
void    cInput::addMouseHandler(cInputHandler& handlerObj)
{
    mouseEventHandlers.push_back(&handlerObj);
}

// ----------------------------------------------------------------------------
// Prepares the next event in the input queue for processing. returns false when there is nothing left to process or do. 
//   automatically discards invalid input so processEvent can be called immediately on the returned event
INPUT_RECORD *	cInput::getEvent()
{
    while (nNextEvent < (int) dwEvents && nNextEvent < (int) dwMaxEvents)
    {
        //if (inputBuffer[nNextEvent].EventType == MOUSE_EVENT)   // mouse events are only for application level control
        
        switch (inputBuffer[nNextEvent].EventType)
        {
        case MOUSE_EVENT:   // mouse events are only for application level control
            if (inputBuffer[nNextEvent].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            {
            }
            break;          // DMC: discard mouse input for now
        case KEY_EVENT:     //keyboard events are for game or application control
        {
            if (inputBuffer[nNextEvent].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
            {
            }
            return &inputBuffer[nNextEvent++];
        }

        case WINDOW_BUFFER_SIZE_EVENT:
        case MENU_EVENT:
        case FOCUS_EVENT:
            break;
        }
        nNextEvent++;
    }
    return nullptr;
}

// ----------------------------------------------------------------------------
bool    cInput::processEvent(INPUT_RECORD& inputEvent)
{
    bool bHandled = false;
    if (KEY_EVENT == inputEvent.EventType)
    {
        auto itr = kbEventHandlers.begin();
        do
        {
            bHandled = (*itr)->handler(inputEvent);
        } while (!bHandled && ++itr != kbEventHandlers.end());
    }
    else if (MOUSE_EVENT == inputEvent.EventType)
    {

    }
    // mouseEventHandlers
    return false;
}

