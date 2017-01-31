// ----------------------------------------------------------------------
//
// module  -  LList.c
//
// contents - dynamic linked-list maangement routines
//            
// ----------------------------------------------------------------------

/*   This unpublished source code contains trade secrets and
     copyrighted materials which are the property of Mindscape, Inc.
     Unauthorized use, copying or distribution is a violation of U.S. and
	 international laws and is strictly prohibited.  */

#include    <windows.h>

#include	"MemManage.h"
#include	"llist.h"



static VOID llistInitialize(PVOID pList);


// Create a new list
// ----------------------------------------------------------------------
// reserved header size (optional)
PVOID		llistCreate(UINT uHeaderSize)
{
	// pointer to the list header
	P_LISTHEADER pList = NULL;

	// attempt to allocate the list construct block
	if((pList = memAlloc(sizeof(_LISTHEADER) + uHeaderSize)) != NULL)
	{
		llistInit(pList, uHeaderSize);
	}

	// return a pointer to the new list construct - may contain a failure code
	return(pList);
}


// Initializing a new list without allocating space for the header.
// ----------------------------------------------------------------------
// reserved header size (optional)
VOID		llistInit(P_LISTHEADER pList, UINT uHeaderSize)
{
	// initialize the new list construct
	llistInitialize(pList);

	pList->uHeaderSize = uHeaderSize;
}


// Initialize a list header to reflect zero entries - for init & reset
// ----------------------------------------------------------------------
// pointer to the allocated block
static VOID	llistInitialize(PVOID pList)
{
	// pointer to the list construct header
	P_LISTHEADER pHeader;

	// if a valid address has been specified
	if(pList != NULL)
	{
		// setup a pointer to the list construct header
		pHeader = (P_LISTHEADER)pList;

		// initialize the header information
#if SENTINEL_STRINGS
		memcpy(pHeader->startSentinel, LIST_HEADER_START, sizeof(pHeader->startSentinel));
		memcpy(pHeader->startDataSentinel, LIST_HEADER_DATA_START, sizeof(pHeader->startDataSentinel));
#endif

		pHeader->nElementCount = 0;
		pHeader->pHead = NULL;
		pHeader->pTail = NULL;
		pHeader->pCurrent = NULL;
		pHeader->pUserHeader = (PVOID)((PBYTE)pList + (DWORD)sizeof(_LISTHEADER));
	}
}


// ----------------------------------------------------------------------
// returns a pointer to the user-specified header
PVOID       llistUserHeader(PVOID pList)
{
	P_LISTHEADER pHeader;

	pHeader = (P_LISTHEADER) pList;

    if ((pList == NULL) || (pHeader->pUserHeader == NULL))
        return(NULL);

    return(pHeader->pUserHeader);
}


// ----------------------------------------------------------------------
// returns the size of the user-specified header
UINT       llistUserHeaderSize(PVOID pList)
{
	P_LISTHEADER pHeader;

	pHeader = (P_LISTHEADER) pList;

    if ((pList == NULL) || (pHeader->pUserHeader == NULL))
        return(0);

	pHeader = (P_LISTHEADER) pList;
    return(pHeader->uHeaderSize);
}


// Empty a list
// ----------------------------------------------------------------------
// pointer to the list header
BOOL		llistClear(PVOID pList)
{
	P_LISTHEADER pHeader;

	// if a valid address has been specified
	if(NULL == pList)
		return(FALSE);

	pHeader = (P_LISTHEADER) pList;
	
	while (pHeader->pHead != NULL)
		llistRemove(pList, 0, SEEK_END);

	llistInitialize(pList);
	return(TRUE);
}


// Release an list header
// ----------------------------------------------------------------------
// pointer to the list header
VOID		llistDestroy(PVOID pList)
{
	P_LISTHEADER pHeader;

	// if a valid address has been specified
	if(NULL == pList)
		return;

	pHeader = (P_LISTHEADER) pList;
	
	while (pHeader->pHead != NULL)
		llistRemove(pList, 0, SEEK_END);
		
	// release the block containing the list header
	memFree(pList);
}


// Add a new node to the list; link in the data element
// ----------------------------------------------------------------------
// pointer to the list header
// pointer to the data element
PVOID		llistAdd(P_LISTHEADER pHeader, PVOID pElement, UINT uElementSize, int nPosition)
{
	P_LISTNODE	pNode;
	
	pNode = llistAddEmpty(pHeader, uElementSize, nPosition);
	if (!pNode)
	{
		return NULL;	// Failure.
	}

	memcpy(pNode->pData, pElement, uElementSize);

	return(pNode->pData);
}


// Add a new node to the list; link in the data element
// ---------------------------------------------------------------------------
// pointer to the list header
// pointer to the data element
P_LISTNODE	llistAddAtNode(P_LISTHEADER pHeader,
						   PVOID pElement, UINT uElementSize,
						   P_LISTNODE pPosition, int nPosition)
{
	P_LISTNODE	pNode;
	
	pNode = llistAddEmptyAtNode(pHeader, uElementSize, pPosition, nPosition);
	if (!pNode)
	{
		return NULL;	// Failure.
	}

	memcpy(pNode->pData, pElement, uElementSize);

	return pNode;
}


// Add a new node to the list; make room for the data element
// ----------------------------------------------------------------------
// pointer to the list header
// pointer to the data element
P_LISTNODE	llistAddEmpty(P_LISTHEADER pHeader, UINT uElementSize,
						  int nPosition)
{
	if (!pHeader)
	{
		return NULL;	// Failure.
	}

	return llistAddEmptyAtNode(pHeader, uElementSize,
							   pHeader->pCurrent, nPosition);
}


// Add a new node to the list; make room for the data element
// ---------------------------------------------------------------------------
// pointer to the list header
// pointer to the data element
P_LISTNODE	llistAddEmptyAtNode(P_LISTHEADER pHeader, UINT uElementSize,
						  P_LISTNODE pPosition, int nPosition)
{
	P_LISTNODE	pNode;

	pNode = memAlloc(uElementSize + sizeof(_LISTNODE));
	if (!pNode)
	{
		return NULL;	// Failure.
	}

#if SENTINEL_STRINGS
	memcpy(pNode->startSentinel, LIST_NODE_START, sizeof(pNode->startSentinel));
	memcpy(pNode->startDataSentinel, LIST_NODE_DATA_START, sizeof(pNode->startDataSentinel));
#endif

	// DMC - memory overwrite checking
	//memset(pNode, 1, uElementSize + sizeof(_LISTNODE));
	
	// the data is located in the contiguous memory
	// following the data pointer
	// WARNING:  this statement assumes pData is the last member.
//	pNode->pData = &pNode->pData + 1;
	pNode->pData = ((PBYTE) pNode) + sizeof *pNode;
	
	if (!llistLinkAtNode(pHeader, pNode, pPosition, nPosition))
	{
		memFree(pNode);
		return 0;	// Failure.
	}
	pHeader->nElementCount++;

	return pNode;
}


// Remove a node from the list, and update all affected node pointers
// ----------------------------------------------------------------------
// pointer to the list header
// pointer to the node to be removed
// ----------------------------------------------------------------------
// returns TRUE on success; FALSE on failure
BOOL	llistRemoveNode(P_LISTHEADER pHeader, P_LISTNODE pNode)
{
	if (!pHeader || !pNode)
		return 0;
	
	// resolve the node pointers for after removal of the current
	if (pNode->pSuccessor != NULL)
		pNode->pSuccessor->pPredecessor = pNode->pPredecessor;

	if (pNode->pPredecessor != NULL)
		pNode->pPredecessor->pSuccessor = pNode->pSuccessor;

	// fix any header pointer problems
	if (pHeader->pHead == pNode)
		pHeader->pHead = pNode->pSuccessor;

	if (pHeader->pTail == pNode)
		pHeader->pTail = pNode->pPredecessor;

	if (pHeader->pCurrent == pNode)
		pHeader->pCurrent = pHeader->pHead;

	// finally, free the memory
	memFree(pNode);
	pHeader->nElementCount--;
	
	return(TRUE);
}


// Remove a node from the list, and update all affected node pointers
// ----------------------------------------------------------------------
// pointer to the list header
// pointer to the node to be removed
// ----------------------------------------------------------------------
// returns TRUE on success; FALSE on failure
BOOL		llistRemove(P_LISTHEADER pHeader, int nDisplacement, int nPosition)
{
	P_LISTNODE	pNode;
	int			i;

	if ((pHeader == NULL) || ((pNode = pHeader->pCurrent) == NULL))
		return(FALSE);

	// determine action based on the relative seek position
	switch(nPosition)
	{
		case SEEK_SET:
			// seek relative to the first node contained within the list construct
			pNode = pHeader->pHead;
			break;

		case SEEK_CUR:
			// seek relative to the current read/write index within the list construct
			pNode = pHeader->pCurrent;
			break;

		case SEEK_END:
			// seek relative to the last node contained within the list construct
			pNode = pHeader->pTail;
			break;
	}

	// step through the specified number of nodes
	for	(i = 0;(i < (int)abs(nDisplacement)) && (pNode != NULL); i++)
	{
		// if the displacement is negative (seek backward)
		if (nDisplacement < 0)
		{
			// get the predecessor link of the currently referenced node
			pNode = pNode->pPredecessor;
		}
		else if (nDisplacement > 0)
		{
			// get the successor link of the currerntly referenced node
			pNode = pNode->pSuccessor;
		}
	}

	// if a seek past the end has occured, report failure
	if((pNode == NULL) && (nDisplacement < 0))
		return(FALSE);

	return llistRemoveNode(pHeader, pNode);
}


// Seek to a relative position within the list structure
// ----------------------------------------------------------------------
// pointer to the list header
// relative displacement index
// relative seek position
// ----------------------------------------------------------------------
// returns the data block for the new current node on success; NULL on failure
PVOID		llistSeek(PVOID pList, int nDisplacement, int nPosition)
{
	P_LISTHEADER pHeader;
	int			i;
	P_LISTNODE	pNode;

	
	// if the specified list construct address is valid
	if (pList == NULL)
		return(NULL);

	// setup a pointer to the list construct header
	pHeader = (P_LISTHEADER)pList;
	if (0 == pHeader->nElementCount)
		return(NULL);

	// determine action based on the relative seek position
	switch(nPosition)
	{
		case SEEK_SET:
			// seek relative to the first node contained within the list construct
			pNode = pHeader->pHead;
			break;

		case SEEK_CUR:
			// seek relative to the current read/write index within the list construct
			pNode = pHeader->pCurrent;
			break;

		case SEEK_END:
			// seek relative to the last node contained within the list construct
			pNode = pHeader->pTail;
			break;
	}

	// step through the specified number of nodes
	for	(i = 0;(i < (int)abs(nDisplacement)) && (pNode != NULL); i++)
	{
		// if the displacement is negative (seek backward)
		if(nDisplacement < 0)
		{
			// get the predecessor link of the currently referenced node
			pNode = pNode->pPredecessor;
		}
		else	
		{
			// get the successor link of the currerntly referenced node
			pNode = pNode->pSuccessor;
		}
	}

	// if a seek past the start of the list structure has occured
	if(pNode == NULL) 
	{
		// do not allow seeks past the beginning of the list structure
		if (nDisplacement < 0)
			pHeader->pCurrent = pHeader->pHead;
		// or past the end
		else if (nDisplacement > 0)
			pHeader->pCurrent = pHeader->pTail;

	}
	else
	{
		// set the currernt read/write index (may be invalid)
		pHeader->pCurrent = pNode;
	}
	// return the address of the associated data element
	if (NULL == pNode)
		return(NULL);

	return(pHeader->pCurrent->pData);
}

// Seek a new current by the value of the pData field
// ----------------------------------------------------------------------
// pointer to the list header
// pointer to the data value to search for
// sizeof the data item
// ----------------------------------------------------------------------
// returns pointer to the found node's data on success; NULL on failure
PVOID		llistFind(P_LISTHEADER pHeader, PVOID pCompare, LLISTCOMPAREFUNC pfnCompare)
{

	if ((NULL == pHeader) || (pHeader->nElementCount == 0) || (NULL == pCompare))
		return(FALSE);

	while ((llistCurrentExists(pHeader)) && (NULL != llistCurrentData(pHeader)))
	{
		if (pfnCompare(pCompare, llistCurrentData(pHeader)))
			return(pHeader->pCurrent->pData);
		
		if (NULL == llistNext(pHeader))
			break;
	}

	return(NULL);
}


// Link a new node into the list at the specified location
// ----------------------------------------------------------------------
// pointer to the data element (contained within the list construct)
// ----------------------------------------------------------------------
// returns FALSE on failure, !FALSE on success
int			llistLink(P_LISTHEADER pHeader, P_LISTNODE pNode, int nPosition)
{

	// if the specified list construct address is valid
	if ((pHeader == NULL) || (pNode == NULL))
		return(FALSE);

	return llistLinkAtNode(pHeader, pNode, pHeader->pCurrent, nPosition);
}


// Link a new node into the list at the specified location
// ---------------------------------------------------------------------------
// pointer to the data element (contained within the list construct)
// ---------------------------------------------------------------------------
// returns FALSE on failure, !FALSE on success
int			llistLinkAtNode(P_LISTHEADER pHeader, P_LISTNODE pNode, 
							P_LISTNODE pPosition, int nPosition)
{
	if (!pHeader || !pNode)
	{
		return 0;	// Failure.
	}

	if (llistIsEmpty(pHeader))
	{
		pHeader->pCurrent = pNode;
		pHeader->pHead = pNode;
		pHeader->pTail = pNode;
		pNode->pPredecessor = NULL;	// Just for safety.
		pNode->pSuccessor	= NULL;	// Just for safety.
		return(TRUE);
	}

	switch (nPosition)
	{
		case SEEK_SET:
			pNode->pPredecessor				= NULL;
			pNode->pSuccessor				= pHeader->pHead;
			pNode->pSuccessor->pPredecessor = pNode;
			pHeader->pHead					= pNode;
			break;

		// AFTER the node specified by pPosition.
		case SEEK_CUR:
			if (!pPosition)
			{
				return 0;	// Failure.
			}
			pNode->pSuccessor		= pPosition->pSuccessor;
			pNode->pPredecessor		= pPosition;
			if (pHeader->pTail == pPosition)
			{
				pHeader->pTail = pNode;
			}
			pPosition->pSuccessor	= pNode;
			if (pNode->pSuccessor)
			{
				pNode->pSuccessor->pPredecessor = pNode;
			}
			break;

		case SEEK_END:
			pNode->pPredecessor = pHeader->pTail;
			pNode->pSuccessor	= NULL;
			pNode->pPredecessor->pSuccessor = pNode;
			pHeader->pTail = pNode;
			break;

		case SEEK_PRIOR:
			if (!pPosition)
			{
				return 0;	// Failure.
			}
			if (pHeader->pHead == pPosition)
			{
				pHeader->pHead = pNode;
			}
			pNode->pPredecessor		= pPosition->pPredecessor;
			pNode->pSuccessor		= pPosition;
			pPosition->pPredecessor	= pNode;
			if (pNode->pPredecessor)
			{
				pNode->pPredecessor->pSuccessor = pNode;
			}
			break;

		default:
			return 0;	// Failure:  unknown option.
	}

	if (NULL == pHeader->pCurrent)
		pHeader->pCurrent = pNode;

	return 1;	// Success.
}

// walk an llist, applying a function to each node.
// ----------------------------------------------------------------------
BOOL		llistWalker(P_LISTHEADER pHeader,
						P_LLISTFUNCTION pCallback, PVOID pCallbackData)
{
	P_LISTNODE	pNode;
	BOOL		fResult = TRUE;	// If there are no nodes, it's okay.

	// if a valid address has been specified
	if(NULL == pHeader)
		return TRUE;	// One missing node isn't enough
						// to cancel the caller's walk (why?).
	if(NULL == pCallback)
		return FALSE;	// Quit the walk:  there's no callback.

	for (pNode = pHeader->pHead; pNode; pNode = pNode->pSuccessor)
	{
		fResult =(*pCallback)(pNode->pData, pCallbackData);
		if (!fResult)
		{
			break;	// fResult == FALSE means the callback says it's time
					// to break out.
		}
	}

	return fResult;
}


// 	LList.c
// ----------------------------------------------------------------------
