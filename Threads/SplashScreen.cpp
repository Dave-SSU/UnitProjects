// ----------------------------------------------------------------------
//
// module  -  SplashScreen.cpp
//
// contents - Splash screen window class
//
// ----------------------------------------------------------------------
 
/*   This unpublished source code contains trade secrets and
     copyrighted materials which are the property of Mindscape, Inc.
     Unauthorized use, copying or distribution is a violation of U.S. 
	 and international laws and is strictly prohibited.
*/

#include    <Windows.h>
#include    <process.h>

#include    "dibObject.h"
#include    "winutils.h"
#include    "CommonExterns.h"
#include    "SplashScreen.h"
#include    "StringRes.h"

#define     CM_SPLASH_WNDCLASS      "CM_SPLASH_WNDCLASS"
#define     UM_SPLASHCLOSE          (WM_APP + 1)

static DWORD    dwThreadId = 0;

LRESULT CALLBACK SplashWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
UINT __stdcall  splashThreadFunc(void * pData);

// ----------------------------------------------------------------------
BOOL        splashCreate(LPSTR szSplashResourceName)
{
    HANDLE   hThreadAPI = NULL;

    hThreadAPI = (HANDLE) _beginthreadex(NULL, 0, 
        splashThreadFunc, (void *) szSplashResourceName, 0, (UINT *) &dwThreadId);

    if (NULL == hThreadAPI)
        return(FALSE);
    return(TRUE);
}

// ----------------------------------------------------------------------
BOOL        splashDestroy(void)
{
    if (0 == dwThreadId)
        return(FALSE);

    PostThreadMessage(dwThreadId, UM_SPLASHCLOSE, 0, 0);

    return(TRUE);
}

// ----------------------------------------------------------------------
UINT __stdcall  splashThreadFunc(void * pData)
{
    // force this thread to create a message queue
    GetFocus();

    // instantiate a local splash object
    _SPLASH     splash((LPSTR) pData, STR_APP_NAME);

    // stay in the msg loop until receiving an UM_SPLASHCLOSE message
    MSG     msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
        if (UM_SPLASHCLOSE == msg.message)
        {
            if (NULL != splash.hWnd)
                DestroyWindow(splash.hWnd);
        }
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    // everything is cleaned up with the change of scope
    return(0);
}

// ----------------------------------------------------------------------
Splash::Splash(LPSTR szSplashResourceName, int nTitleStringResource)
{
    memset(this, 0, sizeof(_SPLASH));

    lstrcpy(this->szSplashResourceName, szSplashResourceName);
    if (!RegisterWindowClass())
        return;

	hDS = LoadImage(hInstApp, this->szSplashResourceName, IMAGE_BITMAP, 0, 0,
		LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

    if (NULL == hDS)
        return;

    LoadString(hInstApp, nTitleStringResource, szWindowTitle, sizeof szWindowTitle);
    CreateSplashWindow();
}

// ----------------------------------------------------------------------
Splash::~Splash()
{
    UnregisterWindowClass();

	DeleteObject(hDS);
    hDS = NULL;
}

// ----------------------------------------------------------------------
BOOL        Splash::RegisterWindowClass(void)
{
    char    szBuffer[64];
    WNDCLASS wndClass;

    lstrcpy(szBuffer, CM_SPLASH_WNDCLASS);
    wndClass.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
    wndClass.cbWndExtra = 0;
    wndClass.cbClsExtra = 0;
    wndClass.hIcon = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = szBuffer;
    wndClass.hbrBackground = NULL;
    wndClass.hInstance = hInstApp;
    wndClass.style = 0;
    wndClass.lpfnWndProc = SplashWndProc;

    if (!RegisterClass((LPWNDCLASS) &wndClass))
        return FALSE;

    return TRUE;
}

// ----------------------------------------------------------------------
void        Splash::UnregisterWindowClass(void)
{
    char    szBuffer[64];

    lstrcpy(szBuffer, CM_SPLASH_WNDCLASS);
    UnregisterClass(szBuffer, hInstApp);
}

// ----------------------------------------------------------------------
HWND        Splash::CreateSplashWindow(void)
{
	hWnd = CreateWindowEx(0, CM_SPLASH_WNDCLASS, 
        "", //szWindowTitle,
		WS_VISIBLE | WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstApp, (LPVOID) this);

    return(hWnd);
}

// ----------------------------------------------------------------------
LRESULT CALLBACK SplashWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    P_SPLASH    pSplash;

    switch (uMsg)
    {
		case WM_CREATE:
		{
            LPCREATESTRUCT  pCS = (LPCREATESTRUCT) lParam;

            pSplash = (P_SPLASH) pCS->lpCreateParams;
            if (pSplash == NULL)
                return(0);
            SetWindowLong(hWnd, GWL_USERDATA, (LONG) pSplash);

			DIBSECTION	ds;

			if (GetObject(pSplash->hDS, sizeof(DIBSECTION), &ds) != sizeof(DIBSECTION))
				return(0);

            RECT        rDesktop, rNew;

    		GetWindowRect(GetDesktopWindow(), &rDesktop);

            SetRect(&rNew, rDesktop.left + ((rDesktop.right - rDesktop.left) / 2) - (ds.dsBmih.biWidth / 2),
                rDesktop.top + ((rDesktop.bottom - rDesktop.top) / 2) - (ds.dsBmih.biHeight / 2),
                rDesktop.left + ((rDesktop.right - rDesktop.left) / 2) - (ds.dsBmih.biWidth / 2) + ds.dsBmih.biWidth, 
                rDesktop.top + ((rDesktop.bottom - rDesktop.top) / 2) - (ds.dsBmih.biHeight / 2) + ds.dsBmih.biHeight); 

            SetWindowPos(hWnd, HWND_TOP, rNew.left, rNew.top, ds.dsBmih.biWidth, ds.dsBmih.biHeight, 
                SWP_SHOWWINDOW | SWP_FRAMECHANGED);
            ShowWindow(hWnd, SW_SHOW);
          	break;
		}

        // on the destroy, request destruction of the message queue
        case WM_DESTROY:
            PostQuitMessage(2);
            break;

        case WM_PAINT :
		{
            PAINTSTRUCT ps;
            
            // if the global palette is not initialized, wait for it
            while (NULL == hPalApp)
                Sleep(5);

            BeginPaint(hWnd, &ps);

            if (NULL == (pSplash = (P_SPLASH) GetWindowLong(hWnd, GWL_USERDATA)))
            {
                EndPaint(hWnd, &ps);
                DestroyWindow(hWnd);
                return(0);
            }

			SelectPalette(ps.hdc, hPalApp, FALSE);
			if (RealizePalette(ps.hdc))
			{
                RedrawWindow(hWnd, NULL, NULL,
					RDW_INTERNALPAINT | RDW_ERASE | RDW_INVALIDATE
					| RDW_FRAME | RDW_ALLCHILDREN);
			}

			DIBSECTION	ds;
			HDC			hMemDC;
			HGDIOBJ		hObj;

			if(GetObject(pSplash->hDS, sizeof(DIBSECTION), &ds) != sizeof(DIBSECTION))
				break;

			hMemDC = CreateCompatibleDC(ps.hdc);
			if (NULL != hMemDC)
			{
				hObj = SelectObject(hMemDC, pSplash->hDS);
				BitBlt(ps.hdc, 0, 0, ds.dsBmih.biWidth, ds.dsBmih.biHeight, hMemDC,
					0, 0, SRCCOPY);
				SelectObject(hMemDC, hObj);
				DeleteDC(hMemDC);
			}
            EndPaint (hWnd, &ps);
            break;
		}
    }

    return DefWindowProc (hWnd, uMsg, wParam, lParam) ;
}

