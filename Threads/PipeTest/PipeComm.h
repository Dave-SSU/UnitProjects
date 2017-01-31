// ----------------------------------------------------------------------
//
// include -  PipeComm.h
//
// contents - pipe-based communications
//
// ----------------------------------------------------------------------

#define     _TIMING_TEST_ENABLED    1
#define     PIPECOMM_USE_INTERNAL_LLIST 1

#ifndef		PIPECOMM_H
#define		PIPECOMM_H

#ifdef __cplusplus
 extern "C" {
#endif


#define     MAKE_PROTOCOL(ch0, ch1, ch2, ch3)                       \
     		    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
    		    ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#define     PIPECOMM_PROTOCOL       MAKE_PROTOCOL('1','0','0','a')


#define     PIPE_BUFFER_SIZE        4096

#define     NOMINAL_PIPE_SLEEP      8

#define     MAX_WRITE_ATTEMPTS      10

#define     MAX_TERMINATE_WAIT      10

#define     NULLCHAR                '\0'
#define     CR_CHAR                 '\r'
#define     LF_CHAR                 '\n'
#define     SPACE_CHAR              ' '

#define     MAX_PIPE_MSG_LEN        1024

#define     PIPE_SUCCESS            0
#define     PIPE_BAD_POINTER        -1
#define     PIPE_ERROR              1
#define     PIPE_BUFFER_EXCEDED     2
#define     PIPE_FATAL_ERROR        9998
#define     PIPE_CLOSED             9999


typedef struct
{
    HANDLE          hReadPipe;
    HANDLE          hWritePipe;
    HANDLE          hReadThread;
    UINT            uReadThreadId;
    BOOL            bTerminateReadThread;
    PVOID           pMsgQueue;

} _PIPE_COMMUNICATION;


// ----------------------------------------------------------------------

//  Basic communication functions
int             pipeCreateCommunication(HANDLE *phToAppRead, HANDLE *phToAppWrite, 
                                        HANDLE *phToEngineRead, HANDLE *phToEngineWrite);
int             pipeDestroyCommunication(HANDLE *phToAppRead, HANDLE *phToEngineWrite);
int             pipeAttachEngine(HANDLE *phReadPipe, HANDLE *phWritePipe);
int             pipeDetachEngine(HANDLE *phReadPipe, HANDLE *phWritePipe);
int             pipeWriteCommand(_PIPE_COMMUNICATION * pPC, char * szCommand);

//  Pipe read-thread code that uses a linked-list message queue 
int             pipeInitInput(_PIPE_COMMUNICATION * pPC);
int             pipeTerminateInput(_PIPE_COMMUNICATION * pPC);
PVOID           pipeCreateMsgQueue(void);
void            pipeDestroyMsgQueue(PVOID pMsgQueue);
char *          pipeGetMessage(PVOID pMsgQueue);
void            pipeRemoveMessage(PVOID pMsgQueue);


#if _TIMING_TEST_ENABLED

#define     TIME_TEST_STRING        "TIMETEST:"
#define		nS_SCALE				1000000000		// 1 billion
#define		uS_SCALE				1000000			// 1 million
#define		mS_SCALE				1000			// 1 thousand

void            pipeMakeTimeString(char * szTimeString);
int             pipeGetDeltaFromTimeString(char * szTimeString);

#endif

#ifdef __cplusplus
 } // extern "C"
#endif


#endif	// PIPECOMM_H

