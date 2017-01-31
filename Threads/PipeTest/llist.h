#ifndef LLIST_H
#define LLIST_H

#include    <stdlib.h>

// Linked list node definition
//***********************************************************************************************************
// The following structure defines a single linked list node occurrence.

#if SENTINEL_STRINGS
static const char LIST_HEADER_START[]		= "ListHeader Start  :";
static const char LIST_HEADER_DATA_START[]	= "ListHeader Data Start  :";
static const char LIST_NODE_START[]			= "ListNode Start:";
static const char LIST_NODE_DATA_START[]	= "ListNodeData Start:";
#endif

typedef struct tagLISTNODE
{
#if SENTINEL_STRINGS
	char					startSentinel[sizeof LIST_NODE_START];
#endif
	struct tagLISTNODE *	pPredecessor;
	struct tagLISTNODE *	pSuccessor;
	PVOID			pData;			// pointer to data for this node

#if SENTINEL_STRINGS
	char					startDataSentinel[sizeof LIST_NODE_DATA_START];
#endif
} _LISTNODE, *P_LISTNODE;


typedef struct tag_LISTHEADER
{
#if SENTINEL_STRINGS
	char					startSentinel[sizeof LIST_HEADER_START];
#endif

	int				nElementCount;	// number of nodes in the list
	P_LISTNODE		pHead;			// pointer to the first node's data
	P_LISTNODE		pTail;			// pointer to the last node's data
	P_LISTNODE		pCurrent;		// pointer to current node's data
	PVOID			pUserHeader;	// pointer to optional data header
	UINT			uHeaderSize;	// sizeof the header, if used

#if SENTINEL_STRINGS
	char					startDataSentinel[sizeof LIST_HEADER_DATA_START];
#endif
} _LISTHEADER, *P_LISTHEADER;


typedef BOOL	(*LLISTCOMPAREFUNC)(PVOID, PVOID);

// A function to apply to each node of an llist.
// It returns false to halt traversal.
typedef BOOL (*P_LLISTFUNCTION)(PVOID pData, PVOID pCallbackData);


// Prototypes & macros (public)
//***********************************************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SEEK_CUR
#define     SEEK_CUR    1
#endif
#ifndef SEEK_END
#define     SEEK_END    2
#endif
#ifndef SEEK_SET
#define     SEEK_SET    0
#endif
    
#define		SEEK_PRIOR	(SEEK_END + 1)	// used for seek from position 

// reserved header size (optional)
PVOID		llistCreate(UINT uHeaderSize);

// reserved header size (optional)
VOID		llistInit(P_LISTHEADER pHeader, UINT uHeaderSize);

// pointer to the allocated list construct
VOID		llistDestroy(PVOID pList);

PVOID       llistUserHeader(PVOID pList);

UINT        llistUserHeaderSize(PVOID pList);

	// pointer to the allocated list construct
BOOL		llistClear(PVOID pList);

PVOID		llistAdd(P_LISTHEADER pHeader, PVOID pElement, UINT uElementSize, int nPosition);

P_LISTNODE	llistAddAtNode(P_LISTHEADER pHeader, PVOID pElement, UINT uElementSize, 
						   P_LISTNODE pPosition, int nPosition);

P_LISTNODE	llistAddEmpty(P_LISTHEADER pHeader, UINT uElementSize,
						  int nPosition);

P_LISTNODE	llistAddEmptyAtNode(P_LISTHEADER pHeader, UINT uElementSize,
						  P_LISTNODE pPosition, int nPosition);

int			llistLink(P_LISTHEADER pHeader, P_LISTNODE pNode, int nPosition);

int			llistLinkAtNode(P_LISTHEADER pHeader, P_LISTNODE pNode, 
							P_LISTNODE pPosition, int nPosition);

BOOL		llistRemove(P_LISTHEADER pHeader, int nDisplacement, int nPosition);

BOOL		llistRemoveNode(P_LISTHEADER pHeader, P_LISTNODE pNode);

BOOL		llistWalker(P_LISTHEADER pHeader,
						P_LLISTFUNCTION pCallback, PVOID pCallbackData);

	// pointer to the allocated list construct
	// relative displacement index
	// relative seek position
PVOID		llistSeek(PVOID pList, int nDisplacement, int nPosition);
   
PVOID		llistFind(P_LISTHEADER pHeader, PVOID pCompare, LLISTCOMPAREFUNC pfnCompare);

#define		llistInsert(h,e,z)		llistAdd(h,e,z,SEEK_CUR)

#define		llistAppend(h,e,z)		llistAdd(h,e,z,SEEK_END)

#define		llistPrepend(h,e,z)		llistAdd(h,e,z,SEEK_SET)

// Alias for seeking to the beginning of the list structure
#define		llistRewind(pList)		llistSeek(pList, 0, SEEK_SET)

// Alias for seeking to the next logical node contained within the list structure
#define		llistNext(pList)		llistSeek(pList, 1, SEEK_CUR)

// Alias for seeking to the previous logical node contained within the list structure
#define		llistPrevious(pList)	llistSeek(pList, -1, SEEK_CUR)

// Alias for seeking to the end of the list structure
#define		llistEnd(pList)			llistSeek(pList, 0, SEEK_END)

// Determine if a list is empty
#define		llistIsEmpty(pList) 	(BOOL)(((P_LISTHEADER)(pList))->nElementCount == 0)

#define		llistCurrentExists(pList)	(BOOL)(((P_LISTHEADER)(pList))->pCurrent != NULL)

#define     llistGetCount(pList)        (int)(((P_LISTHEADER)(pList))->nElementCount)

#define		llistCurrentData(pList)		(PVOID)(((P_LISTHEADER)(pList))->pCurrent->pData)

#ifdef __cplusplus
}
#endif

#endif // LLIST_H

// End			List.h
//***********************************************************************************************************
