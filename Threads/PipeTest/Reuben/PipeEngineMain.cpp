// ----------------------------------------------------------------------
//
// module  -   PipeEngineMain.cpp
//
// contents -  winboard-style test console engine 
//
// ----------------------------------------------------------------------

#include    <windows.h>
#include    <stdio.h>
#include    <process.h>

#include    "MemManage.h"

#include    "PipeComm.h"
//#include    "Commands.inc"


static void     sendToApplication(_PIPE_COMMUNICATION * pPC, LPSTR szCommandString);
static int      handleCommandString(_PIPE_COMMUNICATION * pPC, LPSTR szCommandString);


#if 0
int fd;

if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("cannot create socket");
	return;
}
#endif

// ----------------------------------------------------------------------
void        main(int argc, char *argv[])
{ 
    if ((argc >= 1) && (!lstrcmp(argv[0], "-d")))
        MessageBox(NULL, "Engine Launch", "PipeTestEngine", 0);

    _PIPE_COMMUNICATION * pPC;

    pPC = (_PIPE_COMMUNICATION *) memAlloc(sizeof(_PIPE_COMMUNICATION));
    if (NULL == pPC)
        exit(1);

    if (PIPE_SUCCESS != pipeAttachEngine(&pPC->hReadPipe, &pPC->hWritePipe))
    {
        memFree(pPC);
        exit(0);
    }

    if (PIPE_SUCCESS != pipeInitInput(pPC))
    {
        memFree(pPC);
        exit(0);
    }

    sendToApplication(pPC, "Hello, this is Reuben!\n");

    BOOL    bTerminate = FALSE;

    while (!bTerminate)
    {
        LPSTR   szCommand;

        szCommand = pipeGetMessage(pPC->pMsgQueue);
        if (NULL != szCommand)
        {
            if (1 == handleCommandString(pPC, szCommand))
                bTerminate = TRUE;
            pipeRemoveMessage(pPC->pMsgQueue);
        }
    }

    pipeDetachEngine(&pPC->hReadPipe, &pPC->hWritePipe);
    pipeTerminateInput(pPC);
    memFree(pPC);

    exit(0);
}

// ----------------------------------------------------------------------
static void     sendToApplication(_PIPE_COMMUNICATION * pPC, LPSTR szCommandString)
{
    if ((NULL == pPC) || (NULL == szCommandString) || (NULLCHAR == *szCommandString))
        return;

    pipeWriteCommand(pPC, szCommandString);
}

// ----------------------------------------------------------------------
static int      handleCommandString(_PIPE_COMMUNICATION * pPC, LPSTR szCommandString)
{
    LPSTR       szCmd;
    char        szDelimiters[2] = { LF_CHAR, 0};
    int         nCommands = 0;


    if ((NULL == szCommandString) || (NULLCHAR == *szCommandString))
        return(-1);

    szCmd = strtok(szCommandString, szDelimiters);
    while (NULL != szCmd)
    {
        char        szWriteBuffer[256];
        
        if (0 == _strnicmp(szCmd, "quit", lstrlen("quit")))
            return(1);

        // echo back to app
        wsprintf(szWriteBuffer, "telluser %s\n", szCmd);
        sendToApplication(pPC, szWriteBuffer);

#if _TIMING_TEST_ENABLED
        char    szTime[64];

        pipeMakeTimeString(szTime);
        lstrcat(szTime, "\r\n\0");
        //sendToApplication(pPC, szTime);
#endif

        ++nCommands;
        szCmd = strtok(NULL, szDelimiters);
    }
    return(0);
}

