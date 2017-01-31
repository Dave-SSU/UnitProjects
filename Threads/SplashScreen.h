// ----------------------------------------------------------------------
//
// include -  SplashScreen.h
//
// contents - Startup splash screen
//
// ----------------------------------------------------------------------

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include    <windows.h>
#include    "dibObject.h"

#ifdef __cplusplus
typedef class Splash
{
    public:
    // ------------------------------------------
    HWND        hWnd;
    HANDLE		hDS;

    Splash::Splash(LPSTR szSplashResourceName, int nTitleStringResource);
    ~Splash();

    private:
    // ------------------------------------------
    char        szSplashResourceName[64];
    char        szWindowTitle[64];
    
    BOOL        Splash::RegisterWindowClass(void);
    void        Splash::UnregisterWindowClass(void);
    HWND        Splash::CreateSplashWindow(void);

} _SPLASH, * P_SPLASH;
#endif


#ifdef __cplusplus
extern "C" {
#endif

BOOL        splashCreate(LPSTR szSplashResourceName);
BOOL        splashDestroy(void);

#ifdef __cplusplus
} //extern "C"
#endif



#endif  //SPLASHSCREEN_H
