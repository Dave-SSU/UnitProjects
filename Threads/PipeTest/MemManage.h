// ----------------------------------------------------------------------
//
// include -  MemManage.h
//
// contents - API header for MemManage library
//
// ----------------------------------------------------------------------

/*   This unpublished source code contains trade secrets and
     copyrighted materials which are the property of Mindscape, Inc.
     Unauthorized use, copying or distribution is
     a violation of U.S. and international laws and is strictly
     prohibited.                                                  */

#ifndef MEMMANAGE_H
#define MEMMANAGE_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif


// ----------------------------------------------------------------------
// DMC:  always use until release build code is done

#ifdef  _DEBUG

// Debug function prototypes
BOOL		memInit_dbg(int	nUnderwriteGuardSize, int nOverwriteGuardSize);
PVOID		memAlloc_dbg(DWORD dwSize, UINT uFlags, LPSTR szFile, int nLine);
HGLOBAL		memAllocHandle_dbg(DWORD dwSize, UINT uFlags, LPSTR szFile, int nLine);
HGLOBAL     memGetHandle_dbg(PVOID pMem, LPSTR szFile, int nLine);
PVOID		memReAlloc_dbg(PVOID pMem, DWORD dwNewSize, UINT uFlags, LPSTR szFile, int nLine);
PVOID       memLock_dbg(HGLOBAL hMem, LPSTR szFile, int nLine);
int         memUnlock_dbg(HGLOBAL hMem, LPSTR szFile, int nLine);
char        *memStrdup_dbg(const char *pStr, UINT uFlags, LPSTR szFile, int nLine);
DWORD		memFree_dbg(PVOID pMem, LPSTR szFile, int nLine);
BOOL        memFreeHandleSafe_dbg(HGLOBAL *hMem, LPSTR szFile, int nLine);
DWORD		memSize_dbg(PVOID pMem);
DWORD 		memTest_dbg(PVOID pMem);
DWORD		memTestUnderwrite_dbg(PVOID pMem, PBYTE pContents);
DWORD		memTestOverwrite_dbg(PVOID pMem, PBYTE pContents);

// Debug version of wrapper Macros
#define		memInit(u,o)			memInit_dbg(u, o)
#define		memAlloc(s)				memAlloc_dbg(s, GHND, __FILE__, __LINE__)
#define		memAllocHandle(s, f)	memAllocHandle_dbg(s, f, __FILE__, __LINE__)
#define     memGetHandle(p)         memGetHandle_dbg(p, __FILE__, __LINE__)
#define		memReAlloc(p,s)			memReAlloc_dbg(p,s,GHND, __FILE__, __LINE__)
#define     memLock(h)              memLock_dbg(h, __FILE__, __LINE__)
#define     memUnlock(h)            memUnlock_dbg(h, __FILE__, __LINE__)
#define		memStrdup(s)			memStrdup_dbg(s, GHND, __FILE__, __LINE__)
#define		memFree(p)				memFree_dbg(p, __FILE__, __LINE__)
#define     memFreeHandleSafe(ph)   memFreeHandleSafe_dbg(ph, __FILE__, __LINE__)
#define		memSize(p)				memSize_dbg(p)
#define		memTest(p)				memTest_dbg(p)
#define		memTestUnderwrite(p)	memTestUnderwrite_dbg(p,pc)
#define		memTestOverwrite(p)		memTestOverwrite_dbg(p,pc)

#else

// Release function prototypes
BOOL		memInit_rel(int	nUnderwriteGuardSize, int nOverwriteGuardSize);
PVOID		memAlloc_rel(DWORD dwSize, UINT uFlags);
HGLOBAL		memAllocHandle_rel(DWORD dwSize, UINT uFlags);
HGLOBAL     memGetHandle_rel(PVOID pMem);
PVOID		memReAlloc_rel(PVOID pMem, DWORD dwNewSize, UINT uFlags);
PVOID       memLock_rel(HGLOBAL hMem);
int         memUnlock_rel(HGLOBAL hMem);
char        *memStrdup_rel(const char *pStr, UINT uFlags);
DWORD		memFree_rel(PVOID pMem);
BOOL        memFreeHandleSafe_rel(HGLOBAL *hMem);
DWORD		memSize_rel(PVOID pMem);
DWORD 		memTest_rel(PVOID pMem);
DWORD		memTestUnderwrite_rel(PVOID pMem, PBYTE pContents);
DWORD		memTestOverwrite_rel(PVOID pMem, PBYTE pContents);

// Release version of wrapper Macros
#define		memInit(u,o)			memInit_rel(u, o)
#define		memAlloc(s)				memAlloc_rel(s, GHND)
#define		memAllocHandle(s, f)	memAllocHandle_rel(s, f)
#define     memGetHandle(p)         memGetHandle_rel(p)
#define		memReAlloc(p,s)			memReAlloc_rel(p,s,GHND)
#define     memLock(h)              memLock_rel(h)
#define     memUnlock(h)            memUnlock_rel(h)
#define		memStrdup(s)			memStrdup_rel(s, GHND)
#define		memFree(p)				memFree_rel(p)
#define     memFreeHandleSafe(ph)   memFreeHandleSafe_rel(ph)
#define		memSize(p)				memSize_rel(p)
#define		memTest(p)				memTest_rel(p)
#define		memTestUnderwrite(p)	memTestUnderwrite_rel(p,pc)
#define		memTestOverwrite(p)		memTestOverwrite_rel(p,pc)


#endif


// TryAPI wrapper macros

HANDLE		memResourceLoad(HINSTANCE hInst, LPCSTR lpszBmpName, LPCSTR szType);
PVOID		memResourceLock(HANDLE h);
BOOL		memResourceUnlock(HGLOBAL h);
int			memGetObject(HGDIOBJ hgdi, int i, PVOID	pObject);
HGDIOBJ		memSelectObject(HDC hdc, HGDIOBJ hgdi);


#define     LoadDIBResourceHandle(h, b)     memResourceLoad(h, b, RT_BITMAP)
#define     TryLockResource(h)              memResourceLock(h)
#define     TryUnlockResource(h)            memResourceUnlock(h)
#define     TryGlobalLock(h)                memLock(h)
#define     TryGlobalUnlock(h)              memUnlock(h)
#define     TryGetObject(h,i,d)             memGetObject(h, i, d)
#define     TrySelectObject(hdc, hobj)      memSelectObject(hdc, hobj)



#define		UNDER_GUARD_DATA		0xFA
#define		OVER_GUARD_DATA			0xFB
#define		INIT_GUARD_DATA			0x00

#define		MEMERR_NULL_POINTER		NULL
#define		MEMERR_SUCCESS			0
#define		MEMERR_NOT_MMEM			1	
#define		MEMERR_BAD_SIZE			2
#define		MEMERR_CORRUPTION		3
#define		MEMERR_BAD_PTR			4
#define     MEMERR_UNKNOWN          10


// ----------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif  // MEMMANAGE_H

