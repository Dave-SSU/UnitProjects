// SocketTextApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <Windows.h>
#include "SocketTestApp.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, HWND *, HWND *);
LRESULT CALLBACK	mainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       mainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


extern int socketClient(void);

// ----------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
    HWND hWndApp=0, hMainDlg=0;
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SOCKETTEXTAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow, &hWndApp, &hMainDlg))
		return FALSE;
   
    msg.message = WM_USER;
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if ((NULL != hMainDlg) && (IsDialogMessage(hMainDlg, &msg)))
		    {
			    ;
		    }
		    else
		    {
			    TranslateMessage(&msg);
			    DispatchMessage(&msg);
		    }
        }
        else
        {
            socketClient();
        }
    }

	return (int) msg.wParam;
}



// ----------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= mainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOCKETTEXTAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SOCKETTEXTAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// ----------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND * phWndApp, HWND * phMainDlg)
{
   hInst = hInstance; // Store instance handle in our global variable

   *phWndApp = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!*phWndApp)
   {
      return FALSE;
   }

   *phMainDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAIN), *phWndApp, 
        mainDlgProc);

   ShowWindow(*phWndApp, nCmdShow);
   UpdateWindow(*phWndApp);

   return TRUE;
}

// ----------------------------------------------------------------------
LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
		
    case WM_SYSCOMMAND:
        if (SC_CLOSE != (0xFFF0 & wParam))
    		return(DefWindowProc(hWnd, uMsg, wParam, lParam));

    // intentional fall-through

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return(0);

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

    default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }
	return 0;
}

// ----------------------------------------------------------------------
BOOL CALLBACK   mainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
#if 0
                case ID_ENGINESELECT:
                    break;

                case ID_ENGINE:
                    if (IsDlgButtonChecked(hWnd, ID_DEBUGENGINE))
                        launchEngine("gnuchess.exe", NULL ); // DChess1_0_2
                    else                    
                        //launchEngine("wcrafty-176.exe", "");
                        launchEngine("Reuben.exe", "");

                    EnableWindow(GetDlgItem(hWnd, ID_ENGINE), FALSE);
                    EnableWindow(GetDlgItem(hWnd, ID_ENGINECLOSE), TRUE);
                    break;

                case ID_ENGINECLOSE:
                    if (NULL != hEngineComm)
                    {
                        closeEngine(FALSE);
                        EnableWindow(GetDlgItem(hWnd, ID_ENGINE), TRUE);
                        EnableWindow(GetDlgItem(hWnd, ID_ENGINECLOSE), FALSE);
                    }
                    break;

                case ID_TESTONE:
                    sendTestMessage();
                    break;

                case ID_TESTTWO:
                    sendTextMessage( hWnd );
                    break;

                case ID_START:
                    SetTimer(hWnd, ID_TIMER, 1000, NULL);
                    break;

                case ID_STOP:
                    KillTimer(hWnd, ID_TIMER);
                    break;
#endif
                default:
                    break;
            }
            break;


        case WM_DESTROY:
//DMC            if (NULL != hEngineComm)
//DMC                closeEngine(FALSE);
            break;
       
        default:
            return(FALSE);
    }
    return(TRUE);
}
