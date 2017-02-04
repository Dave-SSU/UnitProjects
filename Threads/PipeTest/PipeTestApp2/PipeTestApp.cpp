// ----------------------------------------------------------------------
//
// module  -  PipeTestApp.cpp
//
// contents - App initialization; main pump
//
// ----------------------------------------------------------------------

#include	<windows.h>
#include	<windowsx.h>
#include    <winbase.h>
#include    <Commctrl.h>
#include    <commdlg.h>
#include	<shellapi.h>
#include	<stdlib.h>
#include	<shlwapi.h>
#include	<SHLOBJ.H>

#include    "MemManage.h"
#include    "resource.h"

#include    "PipeComm.h"
#include    "Commands.inc"

#define     APP_CLASS_NAME      "PIPEENGINETEST_CLASS"

#define		APP_WND_NAME		"App-Engine Test"

#define     ODS(s)              OutputDebugString(s)
#define     NULCHAR             '\0'
#define     DEAD                -1
#define     ID_TIMER            1

typedef struct
{
    _PIPE_COMMUNICATION pc;

    PROCESS_INFORMATION piEngineProcessInfo;
    STARTUPINFO         siEngineStartInfo;

} _APP_PIPE_COMMUNICATION;

#define     TEST_1  "chess\r\n"
#define     TEST_2  "chess\r\nmove d2d4\r\nmove"
#define     TEST_3  " e7e5\r\n"


HWND		hWndApp = NULL,
            hMainDlg = NULL;
HINSTANCE	hInstApp = NULL;
char        szBuffer[1024];
HANDLE      hEngineComm = NULL;

static BOOL init(HINSTANCE hInst);
static BOOL	registerClasses(HINSTANCE hInstance);
static HWND	createMainWindow(HINSTANCE hInstance);
LRESULT CALLBACK appWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK   mainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HWND     mainDlg(HWND hWndParent);

static int      launchEngine(LPSTR szEngine, LPSTR szCmdLine);
static void     closeEngine(BOOL bCleanup);
static void     sendToEngine(LPSTR szCommandString);
static void     sendTestMessage(void);
static void     sendTextMessage( HWND hWnd );
static int      handleEngineCommandString(LPSTR szCommandString);
static int      handleEngineCommand(LPSTR szCommand);
static int      identifyCommand(LPSTR szCommand, int * pnLen);


// ----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow)
{
    if (!init(hInstance))
        return(0);

    MSG     msg;
    int     nIdleCount = 0;

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
        else if (NULL != hEngineComm)
        {
            LPSTR   szCommand;

            _APP_PIPE_COMMUNICATION * pAPC = (_APP_PIPE_COMMUNICATION *) hEngineComm;

            szCommand = pipeGetMessage(pAPC->pc.pMsgQueue);
            if (NULL != szCommand)
            {

#if _TIMING_TEST_ENABLED
                int nDelta = pipeGetDeltaFromTimeString(szCommand);
                if (-1 != nDelta)
                {
                    char    szDelta[32];

                    wsprintf(szDelta, "d: %d\r\n", nDelta);
                    ODS(szDelta);
                }
#endif

                handleEngineCommandString(szCommand);
                pipeRemoveMessage(pAPC->pc.pMsgQueue);
            }
        }
    }

	return(0);

}   //  WinMain()

// ----------------------------------------------------------------------
static BOOL init(HINSTANCE hInst)
{

	hInstApp = hInst;

    registerClasses(hInst);
	hWndApp = createMainWindow(hInst);

    //dbgInit(hInst, "dbgOut");

    srand((unsigned) GetTickCount());

    return(hWndApp != NULL);
}

// ----------------------------------------------------------------------
static BOOL	registerClasses(HINSTANCE hInstance)
{
	char		szSmallBuf[128];
	WNDCLASS	wc;

	memset(&wc, 0, sizeof(WNDCLASS));
    lstrcpy(szSmallBuf, APP_CLASS_NAME);

    wc.lpszClassName = szSmallBuf;
    wc.lpszMenuName  = NULL;
    wc.hInstance     = (HINSTANCE)hInstance;
    wc.lpfnWndProc   = (WNDPROC)appWndProc;
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.hIcon         = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor       = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof(PVOID);

    if (!RegisterClass(&wc))
        return(FALSE);

	return(TRUE);
}

// ----------------------------------------------------------------------
LRESULT CALLBACK    appWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
			hMainDlg = mainDlg(hWnd);
            if (NULL == hMainDlg)
            {
                MessageBox(hWnd, "error creating dialog", "error", 0);
                DestroyWindow(hWnd);
                return(0);
            }
            break;
		
        case WM_SYSCOMMAND:
            if (SC_CLOSE != (0xFFF0 & wParam))
    			return(DefWindowProc(hWnd, uMsg, wParam, lParam));

            // intentional fall-through

		case WM_CLOSE:
    		DestroyWindow(hMainDlg);
			DestroyWindow(hWnd);
			return(0);

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

        default:
			return(DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

	return(0L);
}

// ----------------------------------------------------------------------
static HWND	    createMainWindow(HINSTANCE hInstance)
{
	HWND	hWnd = CreateWindow(APP_CLASS_NAME, APP_WND_NAME, 
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
		NULL, NULL, hInstance, 0);

	return(hWnd);
}

// ----------------------------------------------------------------------
static HWND     mainDlg(HWND hWndParent)
{
    HWND    hWnd = CreateDialog(hInstApp, MAKEINTRESOURCE(IDD_MAIN), hWndParent, 
        mainDlgProc);

    return(hWnd);
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
                case ID_ENGINESELECT:
                    break;

                case ID_ENGINE:
					if (IsDlgButtonChecked(hWnd, ID_DEBUGENGINE))
					{
						if (0 != launchEngine("gnuchess.exe", NULL))
							return TRUE;
					}
					else
					{
						//launchEngine("wcrafty-176.exe", "");
						if (0 != launchEngine("Reuben.exe", ""))
							return TRUE;
					}
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

                default:
                    break;
            }
            break;


        case WM_DESTROY:
            if (NULL != hEngineComm)
                closeEngine(FALSE);
            break;

        case WM_TIMER:
            sendTestMessage();
            break;
        
        default:
            return(FALSE);
    }
    return(TRUE);
}

// ----------------------------------------------------------------------
static int      launchEngine(LPSTR szEngine, LPSTR szCmdLine)
{
    _APP_PIPE_COMMUNICATION * pAPC;

    pAPC = (_APP_PIPE_COMMUNICATION *) memAlloc(sizeof(_APP_PIPE_COMMUNICATION));
    
    if (NULL == pAPC)
        return(1);

    HANDLE      hEngineRead, hEngineWrite;

    if (PIPE_SUCCESS != pipeCreateCommunication(&pAPC->pc.hReadPipe, &hEngineWrite, 
        &hEngineRead, &pAPC->pc.hWritePipe))
    {
        return(1);
    }

    // create the thread for monitoring the read pipe
    pipeInitInput(&pAPC->pc);

    // Set up members of STARTUPINFO structure. 
    ZeroMemory(&pAPC->siEngineStartInfo, sizeof(STARTUPINFO));
    pAPC->siEngineStartInfo.cb = sizeof(STARTUPINFO);
    pAPC->siEngineStartInfo.lpReserved = NULL;
    pAPC->siEngineStartInfo.lpDesktop = NULL;
    pAPC->siEngineStartInfo.lpTitle = NULL;
    pAPC->siEngineStartInfo.dwFlags = STARTF_USESTDHANDLES;
    pAPC->siEngineStartInfo.cbReserved2 = 0;
    pAPC->siEngineStartInfo.lpReserved2 = NULL;
    pAPC->siEngineStartInfo.hStdInput = hEngineRead;
    pAPC->siEngineStartInfo.hStdOutput = hEngineWrite;
    pAPC->siEngineStartInfo.hStdError = hEngineWrite;

    BOOL bSuccess = CreateProcess(szEngine,
		       szCmdLine,    /* command line */
		       NULL,	                /* process security attributes */
		       NULL,	                /* primary thread security attrs */
		       TRUE,	                /* handles are inherited */
		       DETACHED_PROCESS|CREATE_NEW_PROCESS_GROUP,
		       NULL,	                /* use parent's environment */
		       NULL,
		       &pAPC->siEngineStartInfo, /* STARTUPINFO pointer */
		       &pAPC->piEngineProcessInfo); /* receives PROCESS_INFORMATION */

	if (!bSuccess)
	{
		char szMsg[128];
		wsprintf(szMsg, "ERROR %d : ", GetLastError());
		OutputDebugString(szMsg);
		GetCurrentDirectory(127, szMsg);
		OutputDebugString(szMsg);
	}

    // Close the pipe handles only the engine will use
    CloseHandle(hEngineRead);
    CloseHandle(hEngineWrite);

    if (!bSuccess)
        return(1);

    hEngineComm = (HANDLE) pAPC;

    sendToEngine("chess\n");

    return(0);
}
 
// ----------------------------------------------------------------------
static void     closeEngine(BOOL bCleanup)
{
    _APP_PIPE_COMMUNICATION * pAPC = (_APP_PIPE_COMMUNICATION *) hEngineComm;

    if (NULL == pAPC)
        return;

    sendToEngine("quit\n");
    
    // must be called early to close the read pipe, and allow the read thread
    //  to terminate
    pipeDestroyCommunication(NULL, &pAPC->pc.hWritePipe);

    pipeTerminateInput(&pAPC->pc);

    int     nThreadTerminateWaitCount = 0;
    pAPC->pc.bTerminateReadThread = TRUE;
    while (0 != pAPC->pc.uReadThreadId)
    {
        if (MAX_TERMINATE_WAIT < ++nThreadTerminateWaitCount)
        {
            MessageBox(NULL, "read thread not closing down", "error", 0);
            break;
        }
        Sleep(NOMINAL_PIPE_SLEEP);
    }

    pipeDestroyCommunication(&pAPC->pc.hReadPipe, NULL);

    memFree(pAPC);
    hEngineComm = NULL;
}

// ----------------------------------------------------------------------
static void     sendToEngine(LPSTR szCommandString)
{
    _APP_PIPE_COMMUNICATION * pAPC = (_APP_PIPE_COMMUNICATION *) hEngineComm;

    if ((NULL == pAPC) || (NULL == szCommandString) || (NULLCHAR == *szCommandString))
        return;

    pipeWriteCommand(&pAPC->pc, szCommandString);
}

#define MESSENGER( x ) ( #x "\n")
// ----------------------------------------------------------------------
static void     sendTextMessage( HWND hWnd )
{
    char szMessage[1024];
    char szMsg[40296];
    int  nTopIndex = 0;

    // get edit text
    SendDlgItemMessage( hWnd, IDC_EDIT1, WM_GETTEXT, (WPARAM)1024, (LPARAM)szMessage );

    // send to list box
    nTopIndex = SendDlgItemMessage( hMainDlg, ID_MSGLIST, LB_GETTOPINDEX, 0, 0 );
    wsprintf( szMsg, "==> %s", szMessage );
    SendDlgItemMessage(hMainDlg, ID_MSGLIST, LB_ADDSTRING, 0, (LPARAM) szMsg );
    SendDlgItemMessage( hMainDlg, ID_MSGLIST, LB_SETTOPINDEX, nTopIndex+1, 0 );

    // send to engine
    lstrcat( szMessage, "\n" );
    sendToEngine( szMessage );

    // clear edit text
    SendDlgItemMessage( hWnd, IDC_EDIT1, WM_SETTEXT, 0, 0 );

}

// ----------------------------------------------------------------------
static void     sendTestMessage(void)
{
    LPSTR       szMessage;

    switch(rand() % LAST_CMD_TO_ENGINE)
    {
        case CMDE_STATUS:
            szMessage = MESSENGER(CMDE_STATUS);
            break;
        case CMDE_REQUEST_VERSION:
            szMessage = MESSENGER(CMDE_REQUEST_VERSION);
            break;
        case CMDE_TERMINATE:
            szMessage = MESSENGER(CMDE_TERMINATE);
            break;
        case CMDE_SET_TRANSPOSITION_SIZE:
            szMessage = MESSENGER(CMDE_SET_TRANSPOSITION_SIZE);
            break;
        case CMDE_INITIALIZE_BOARD:
            szMessage = MESSENGER(CMDE_INITIALIZE_BOARD);
            break;
        case CMDE_ADD_MOVES:
            szMessage = MESSENGER(CMDE_ADD_MOVES);
            break;
        case CMDE_YOUR_TURN:
            szMessage = MESSENGER(CMDE_YOUR_TURN);
            break;
        case CMDE_BEST_LINE:
            szMessage = MESSENGER(CMDE_BEST_LINE);
            break;
        case CMDE_NEXT_BEST_LINE:
            szMessage = MESSENGER(CMDE_NEXT_BEST_LINE);
            break;
        case CMDE_CURRENT_LINE:
            szMessage = MESSENGER(CMDE_CURRENT_LINE);
            break;
        case CMDE_QUICK_HINT:
            szMessage = MESSENGER(CMDE_QUICK_HINT);
            break;
        case CMDE_LAST_VALUATION:
            szMessage = MESSENGER(CMDE_LAST_VALUATION);
            break;
        case CMDE_POSITIONS_SEEN:
            szMessage = MESSENGER(CMDE_POSITIONS_SEEN);
            break;
        case CMDE_FORCE_MOVE:
            szMessage = MESSENGER(CMDE_FORCE_MOVE);
            break;
        case CMDE_ABORT_SEARCH:
            szMessage = MESSENGER(CMDE_ABORT_SEARCH);
            break;
        case CMDE_SET_PERSONALITY:
            szMessage = MESSENGER(CMDE_SET_PERSONALITY);
            break;
        case CMDE_SOLVE_FOR_MATE:
            szMessage = MESSENGER(CMDE_SOLVE_FOR_MATE);
            break;
        case CMDE_SEARCH_BEST_LINE:
            szMessage = MESSENGER(CMDE_SEARCH_BEST_LINE);
            break;
        case CMDE_SEARCH_FIXED_PLY:
            szMessage = MESSENGER(CMDE_SEARCH_FIXED_PLY);
            break;
        case CMDE_SEARCH_PERSONALITY_LINE:
            szMessage = MESSENGER(CMDE_SEARCH_PERSONALITY_LINE);
            break;

        default:
            return;
    }

    if (NULL != szMessage)
        sendToEngine(szMessage);
}

// ----------------------------------------------------------------------
static int      handleEngineCommandString(LPSTR szCommandString)
{
    char        szDelimiters[2] = { LF_CHAR, 0};
    int         nCommands = 0;
    int         nTopIndex = 0;
    char        szMsg[4096];

    if ((NULL == szCommandString) || (NULLCHAR == *szCommandString))
        return(-1);

    nTopIndex = SendDlgItemMessage( hMainDlg, ID_MSGLIST, LB_GETTOPINDEX, 0, 0 );
    wsprintf( szMsg, "<== %s", szCommandString );
    SendDlgItemMessage(hMainDlg, ID_MSGLIST, LB_ADDSTRING, 0, (LPARAM) szMsg );
    SendDlgItemMessage( hMainDlg, ID_MSGLIST, LB_SETTOPINDEX, nTopIndex+1, 0 );

    return 0;
}

// ----------------------------------------------------------------------
static int      handleEngineCommand(LPSTR szCommand)
{
    int         nCommand, nCommandLen;

    if ((NULL == szCommand) || (NULLCHAR == *szCommand))
        return(0);

    nCommand = identifyCommand(szCommand, &nCommandLen);
    return(nCommand);
}

// ----------------------------------------------------------------------
static int      identifyCommand(LPSTR szCommand, int * pnLen)
{
    int     nCommand = CMD_NONE;

    return(nCommand);
}



