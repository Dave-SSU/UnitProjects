// ----------------------------------------------------------------------
//
// module  -   PipeComm.c
//
// contents -  Inter-process communication using anaonymous read & write pipes
//
// ----------------------------------------------------------------------

#include    <windows.h>
#include    <process.h>
#include    <stdio.h>

#include    "PipeComm.h"

#ifdef PIPECOMM_USE_INTERNAL_LLIST
#include    "llist.h"
#endif

static int      nErrorLine;
static char     szErrorMsg[64];


// ----------------------------------------------------------------------
UINT __stdcall	pipeThreadReadFunc(PVOID pData);
static int      pipeReadCommand(_PIPE_COMMUNICATION * pPC);
static int      pipeQueueMessages(PVOID pMsgQueue, char * szMessage);
static void     pipeBeginAccessMsgQueue(PVOID pMsgQueue);
static void     pipeEndAccessMsgQueue(PVOID pMsgQueue);


// ----------------------------------------------------------------------
int             pipeCreateCommunication(HANDLE *phToAppRead, HANDLE *phToAppWrite, 
                                        HANDLE *phToEngineRead, HANDLE *phToEngineWrite)
{ 
    SECURITY_ATTRIBUTES saAttr; 
    BOOL    bSuccess; 
    HANDLE  hToEngineWriteTemp,
            hToAppReadTemp;
            

    // Set the bInheritHandle flag so pipe handles are inherited. 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 
    
    // The steps for redirecting child process's STDOUT: 
    //     1. Save current STDOUT, to be restored later. 
    //     2. Create anonymous pipe to be STDOUT for child process. 
    //     3. Set STDOUT of the parent process to be write handle to 
    //        the pipe, so it is inherited by the child process. 
    //     4. Create a noninheritable duplicate of the read handle and
    //        close the inheritable read handle. 

    // Create a pipe for the child process's STDOUT. 
    if (!CreatePipe(&hToAppReadTemp, phToAppWrite, &saAttr, PIPE_BUFFER_SIZE + 1))
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    // create a non-inheritable read handle to the pipe
    bSuccess = DuplicateHandle(GetCurrentProcess(), hToAppReadTemp,
        GetCurrentProcess(), phToAppRead, 0,
        FALSE,
        DUPLICATE_SAME_ACCESS);
    if (!bSuccess)
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    // close the inheritable read handle. 
    CloseHandle(hToAppReadTemp);


    // The steps for redirecting child process's STDIN: 
    //     1.  Save current STDIN, to be restored later. 
    //     2.  Create anonymous pipe to be STDIN for child process. 
    //     3.  Set STDIN of the parent to be the read handle to the 
    //         pipe, so it is inherited by the child process. 
    //     4.  Create a noninheritable duplicate of the write handle, 
    //         and close the inheritable write handle. 

    // Create a pipe for the child process's STDIN. 
    if (! CreatePipe(phToEngineRead, &hToEngineWriteTemp, &saAttr, PIPE_BUFFER_SIZE + 1))
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    // create a non-inheritable write handle to the pipe
    bSuccess = DuplicateHandle(GetCurrentProcess(), hToEngineWriteTemp, 
        GetCurrentProcess(), phToEngineWrite, 0, 
        FALSE,                  // not inherited 
        DUPLICATE_SAME_ACCESS); 
    if (! bSuccess) 
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    // close the inheritable write handle. 
    CloseHandle(hToEngineWriteTemp); 
    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
int             pipeDestroyCommunication(HANDLE *phToAppRead, HANDLE *phToEngineWrite)
{
    if ((NULL != phToAppRead) && (NULL != *phToAppRead))
    {
        CloseHandle(*phToAppRead); 
        *phToAppRead = NULL; 
    }
    if ((NULL != phToEngineWrite) && (NULL != *phToEngineWrite))
    {
        CloseHandle(*phToEngineWrite); 
        *phToEngineWrite = NULL;
    }

    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
int             pipeAttachEngine(HANDLE *phReadPipe, HANDLE *phWritePipe)
{
    *phReadPipe = GetStdHandle(STD_INPUT_HANDLE); 
    *phWritePipe = GetStdHandle(STD_OUTPUT_HANDLE); 

    if ((*phReadPipe == INVALID_HANDLE_VALUE)
        || (*phWritePipe == INVALID_HANDLE_VALUE)) 
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
int             pipeDetachEngine(HANDLE *phReadPipe, HANDLE *phWritePipe)
{
    if (NULL != *phReadPipe)
    {
        CloseHandle(*phReadPipe);
        *phReadPipe = NULL;
    }
    if (NULL != *phWritePipe)
    {
        CloseHandle(*phWritePipe);
        *phWritePipe = NULL;
    }

    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
int             pipeWriteCommand(_PIPE_COMMUNICATION * pPC, char * szCommand)
{ 
    DWORD       dwWritten;
    DWORD       dwTotalWritten = 0;
    int         nWritePass = 0;
    int         nCommandLen;

    if ((NULL == pPC->hWritePipe) || (NULL == szCommand))
        return(PIPE_BAD_POINTER);

    nCommandLen = lstrlen(szCommand);
    if (0 >= nCommandLen)
        return(PIPE_SUCCESS);

    do
    {
        WriteFile(pPC->hWritePipe, (PBYTE) szCommand + dwTotalWritten, 
            nCommandLen - dwTotalWritten, &dwWritten, NULL);

        dwTotalWritten += dwWritten;
        if (nCommandLen <= (int) dwTotalWritten)
            break;

        if ((MAX_WRITE_ATTEMPTS <= ++nWritePass)
            && (0 == dwTotalWritten))
        {
            return(PIPE_ERROR);
        }

        Sleep(NOMINAL_PIPE_SLEEP);
    } while (1);
#if 0
{
    char    szDelta[32];

    wsprintf(szDelta, "t1: %d - ", GetTickCount());
    OutputDebugString(szDelta);
}
#endif
    return(PIPE_SUCCESS);
} 

#ifdef PIPECOMM_USE_INTERNAL_LLIST

// ----------------------------------------------------------------------
int             pipeInitInput(_PIPE_COMMUNICATION * pPC)
{
    if (NULL == pPC)
        return(-1);

    pPC->pMsgQueue = pipeCreateMsgQueue();

    pPC->bTerminateReadThread = FALSE;

    pPC->hReadThread = (HANDLE) _beginthreadex(NULL, 0, 
        pipeThreadReadFunc, (PVOID) pPC, 0, &pPC->uReadThreadId);
    
    if (NULL == pPC->hReadThread)
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
int             pipeTerminateInput(_PIPE_COMMUNICATION * pPC)
{
    if (NULL == pPC)
        return(-1);

    pPC->bTerminateReadThread = TRUE;
    pipeDestroyMsgQueue(pPC->pMsgQueue);
    
    return(PIPE_SUCCESS);
}

// ----------------------------------------------------------------------
UINT __stdcall	pipeThreadReadFunc(PVOID pData)
{
    _PIPE_COMMUNICATION * pPC = (_PIPE_COMMUNICATION *) pData;
    int     nReadCode;

    if (NULL == pData)
        return(PIPE_BAD_POINTER);
    
    do
    {
        nReadCode = pipeReadCommand(pPC);
        if (PIPE_CLOSED == nReadCode)
        {
            // send terminating message
            break;
        }
        else if (PIPE_FATAL_ERROR == nReadCode)
        {
            // send terminating message
            break;
        }

        // dispatch command

    } while (!pPC->bTerminateReadThread);
    
    // zero read thread id to indicate the thread has terminated
    pPC->uReadThreadId = 0;
    return(PIPE_CLOSED);
}

// ----------------------------------------------------------------------
static int      pipeReadCommand(_PIPE_COMMUNICATION * pPC)
{
    BOOL    bCommandReady = FALSE;
    DWORD   dwRead = 0;
    DWORD   dwResult;
    static char readBuffer[PIPE_BUFFER_SIZE];

    while (!bCommandReady)
    {
        dwResult = ReadFile(pPC->hReadPipe, 
            readBuffer, PIPE_BUFFER_SIZE, 
            &dwRead, NULL);
    
        readBuffer[dwRead] = NULLCHAR;


        if ((dwResult) && (0 < dwRead))
        {
            char    * pCur = NULL, * pPrev = NULL, * pCheckLF = NULL;

            pPrev = &readBuffer[0];
            pCur = &readBuffer[0];

            // Change CR-LF pairs to single LF, put a NULL at the end
            while (0 < dwRead)
            {
                pCheckLF = pCur + 1;
	            if ((*pCheckLF == LF_CHAR) && (*pCur == CR_CHAR))
                {
                    *pPrev = LF_CHAR;    // this ends up being redundant
                    *pCur++;
                    dwRead--;
                    bCommandReady = TRUE;
                }   

                *pPrev++ = *pCur++;
	            dwRead--;
            }

            *pPrev++ = NULLCHAR;

            pipeQueueMessages(pPC->pMsgQueue, readBuffer);
        }
        else // Error reading from pipe
        {
	        // Read EOF from pipe - not a problem
            if (ERROR_BROKEN_PIPE == GetLastError()) 
                return(PIPE_CLOSED);
            dwRead = PIPE_BUFFER_SIZE + 1;
        }

        if (dwRead > PIPE_BUFFER_SIZE)
            break;  // Quit on error 

    }
    return 0;
}


// ----------------------------------------------------------------------
//  linked-list based message queue
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
PVOID           pipeCreateMsgQueue(void)
{
    P_LISTHEADER        pMsgQueue;
    CRITICAL_SECTION *  pCS;

    pMsgQueue = (P_LISTHEADER) llistCreate(sizeof(CRITICAL_SECTION));

    pCS = (CRITICAL_SECTION *) llistUserHeader(pMsgQueue);
    if (NULL == pCS)
    {
        nErrorLine = __LINE__;
        return(NULL);
    }

    InitializeCriticalSection(pCS);
    
    return((PVOID) pMsgQueue);
}

// ----------------------------------------------------------------------
void            pipeDestroyMsgQueue(PVOID pMsgQueue)
{
    CRITICAL_SECTION *   pCS;

    pCS = (CRITICAL_SECTION *) llistUserHeader((P_LISTHEADER) pMsgQueue);
    if (NULL == pCS)
    {
        nErrorLine = __LINE__;
        return;
    }

    DeleteCriticalSection(pCS);

    llistDestroy(pMsgQueue);
}

// ----------------------------------------------------------------------
static void     pipeBeginAccessMsgQueue(PVOID pMsgQueue)
{
    CRITICAL_SECTION *   pCS;

    pCS = (CRITICAL_SECTION *) llistUserHeader((P_LISTHEADER) pMsgQueue);
    if (NULL == pCS)
    {
        nErrorLine = __LINE__;
        return;
    }

    EnterCriticalSection(pCS);
}

// ----------------------------------------------------------------------
static void     pipeEndAccessMsgQueue(PVOID pMsgQueue)
{
    CRITICAL_SECTION *   pCS;

    pCS = (CRITICAL_SECTION *) llistUserHeader((P_LISTHEADER) pMsgQueue);
    if (NULL == pCS)
    {
        nErrorLine = __LINE__;
        return;
    }

    LeaveCriticalSection(pCS);
}

// ----------------------------------------------------------------------
static int      pipeQueueMessages(PVOID pMsgQueue, char * szMessage)
{
    LPSTR       szCmd;
    char        szDelimiters[2] = { LF_CHAR, 0};
    int         nCommands = 0;

    if ((NULL == pMsgQueue) || (NULL == szMessage))
    {
        nErrorLine = __LINE__;
        return(PIPE_ERROR);
    }

    pipeBeginAccessMsgQueue(pMsgQueue);

#if _TIMING_TEST_ENABLED
    {
        int nDelta = pipeGetDeltaFromTimeString(szMessage);
        if (-1 != nDelta)
        {
            char    szDelta[32];

            wsprintf(szDelta, "d: %d\r\n", nDelta);
            OutputDebugString(szDelta);
        }
    }
#endif

    szCmd = strtok(szMessage, szDelimiters);
    
    while (NULL != szCmd)
    {
        llistAdd((P_LISTHEADER) pMsgQueue, szCmd, lstrlen(szCmd) + 1, SEEK_END);

        ++nCommands;
        szCmd = strtok(NULL, szDelimiters);
    }

    pipeEndAccessMsgQueue(pMsgQueue);
    return(nCommands);
}

// ----------------------------------------------------------------------
char *          pipeGetMessage(PVOID pMsgQueue)
{
    char *      szMessage;

    if (NULL == pMsgQueue)
    {
        nErrorLine = __LINE__;
        return(NULL);
    }

    pipeBeginAccessMsgQueue(pMsgQueue);

    szMessage = (char *) llistRewind((P_LISTHEADER) pMsgQueue);

    pipeEndAccessMsgQueue(pMsgQueue);
    return(szMessage);
}

// ----------------------------------------------------------------------
void            pipeRemoveMessage(PVOID pMsgQueue)
{
    char *      szMessage;

    if (NULL == pMsgQueue)
    {
        nErrorLine = __LINE__;
        return;
    }

    pipeBeginAccessMsgQueue(pMsgQueue);

    szMessage = (char *) llistRewind((P_LISTHEADER) pMsgQueue);
    if (NULL != szMessage)
    {
		llistRemove((P_LISTHEADER) pMsgQueue, 0, SEEK_SET);
    }
    pipeEndAccessMsgQueue(pMsgQueue);
}
#endif  // PIPECOMM_USE_INTERNAL_LLIST

#if _TIMING_TEST_ENABLED
// ----------------------------------------------------------------------
void            pipeMakeTimeString(char * szTimeString)
{
    LARGE_INTEGER	liTime;

    if (NULL == szTimeString)
    {
        szTimeString[0] = NULLCHAR;
        return;
    }

	QueryPerformanceCounter(&liTime);
   
    wsprintf(szTimeString, "%s %08X%8X", TIME_TEST_STRING, liTime.HighPart, liTime.LowPart);
}

// ----------------------------------------------------------------------
int             pipeGetDeltaFromTimeString(char * szTimeString)
{
    LARGE_INTEGER	liTime, liStringTime;
    LARGE_INTEGER	liClockFrequency;
    __int64			i64FreqConversion;
    char *          szFindTimeTest;
    char            szTimeTest[32];
    int             nDelta;

	QueryPerformanceCounter(&liTime);

	if (!QueryPerformanceFrequency(&liClockFrequency))
    {
        nErrorLine = __LINE__;
        return(-1);
    }

    szFindTimeTest = strstr(szTimeString, TIME_TEST_STRING);
    if (NULL == szFindTimeTest)
    {
        nErrorLine = __LINE__;
        return(-1);
    }
        
    sscanf(szFindTimeTest, "%s%08X%08X", szTimeTest, &liStringTime.HighPart, &liStringTime.LowPart);

    i64FreqConversion = liClockFrequency.QuadPart / mS_SCALE;

    nDelta = (int) ((liTime.QuadPart - liStringTime.QuadPart) / i64FreqConversion);
    return(nDelta);
}
#endif

