#pragma once

#include <Windows.h>
#include <queue>
#include "cGameSettings.h"


class cInputHandler
{
public:
    virtual ~cInputHandler() {}
    virtual bool handler(const INPUT_RECORD& event) = 0;
};

const int			MAX_EVENTS = 32;

class cInput
{
public:
    static cInput&      initInput(const cGameSettings& settings, HWND hWnd=0);
    static cInput&      getObj() { return *pInput; }
	~cInput();
    void                addKeyboardHandler(cInputHandler& handlerObj);
    void                addMouseHandler(cInputHandler& handlerObj);
    bool				isInputPending();
    INPUT_RECORD *		getEvent();
	bool				processEvent(INPUT_RECORD& inputEvent);

private:
    cInput(HWND hWnd); 
    void                clearInput();
    HWND				hWndInput;
	INPUT_RECORD		inputBuffer[MAX_EVENTS];
    std::queue<INPUT_RECORD>    mouseEvents;
    std::queue<INPUT_RECORD>    kbEvents;
    std::vector<cInputHandler *>    kbEventHandlers;
    std::vector<cInputHandler *>    mouseEventHandlers;
    DWORD				dwMaxEvents;
	DWORD				dwEvents;
    int                 nNextEvent;
	static cInput*		pInput;
};
