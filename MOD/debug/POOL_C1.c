/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_C1.c
*-----------------------------------------------------------------------------
* Module Name:  POOL
*-----------------------------------------------------------------------------
* Description: Pool handling
* Provieds a generic framework for allocating and releasing predefined
* chunks of memory
* For details a design document is available
*-----------------------------------------------------------------------------
* $Date: 2009-06-12 11:57:39 +0200 (Fr, 12 Jun 2009) $
* $Rev: 6631 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/src/POOL_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

//#include <unistd.h>
//#include <pthread.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "POOL.h"
#include "POOL_CI.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/
/*  Flag to verify that the init procedure is done */
static Uint8Type _POOL_u8InitDone = FALSE; 

/** Description of pools */
static POOL_stControl poolControl[POOL_enLAST];

static POOL_enAllocateErrorCode POOL_lastAllocateFailure = 
                                           POOL_enAllocateNoFailure;
static POOL_enFreeErrorCode POOL_lastFreeFailure = POOL_enFreeNoFailure;

static POOL_enInitErrorCode POOL_lastInitFailure = POOL_enInitNoFailure;

//static pthread_mutex_t pMutex = PTHREAD_MUTEX_INITIALIZER;

static void (*POOL_pcbAllocateCallBack) 
    (POOL_enAllocateErrorCode enCode,
     POOL_enPoolNames         enPool,
     void                     *vptr) = (void *) 0;
static void (*POOL_pcbFreeCallBack) 
    (POOL_enFreeErrorCode enCode,
     POOL_enPoolNames     enPool,
     void                 *vptr) = (void *) 0;
static void (*POOL_pcbInitCallBack) 
    (POOL_enInitErrorCode enCode,
     POOL_enPoolNames     enPool,
     void                 *vptr) = (void *) 0;

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/*
 * takes the value and extends it to the next multiple value of 4
 */
#define ALIGN_TO_MULT4(value) ((value+3)&(~3)) 
/*----------------------------------------------------------------------------*/
/* Prototypes for local functions                                             */
/*----------------------------------------------------------------------------*/


/**
\brief	description of the interface IPOOL_INIT
*/
#ifdef IPOOL_INIT /* BEGIN DECLARE INTERFACE(IPOOL_INIT) */
/*----------------------------------------------------------------------------*/
/* Function     POOL_pu32InitElement                                          */
/**
    \brief      Initialise the POOL element and creates a link list for free 
                elements.

    \param      pu32Addr
                address of element which is initialized.

    \param      u32PoolId
                Pool Id for which element belongs

    \param      u32ElementSize
                size of element.

    \param      u32Offset
                Offset for the next element. It will be size + start marker 
                size + end marker size. It will be passed as 0 if it is last 
                element.

    \return    return the address for the next element 
                
   */
/*----------------------------------------------------------------------------*/

static
Uint32Type *POOL_pu32InitElement(Uint32Type *pu32Addr, 
				 Uint32Type u32PoolId,
				 Uint32Type u32ElementSize, 
				 Uint32Type u32Offset)
{
  /* Get the address of next element */
    Uint32Type *pu32NextAddr = (Uint32Type *) 0;
  
  if(u32Offset == 0UL) {
      pu32NextAddr = (Uint32Type *)0;
  } else {
      pu32NextAddr = pu32Addr + (ALIGN_TO_MULT4(u32Offset)/sizeof(Uint32Type));
  }

  /* fill the start marker */
  *pu32Addr = POOL_START_MARKER + u32PoolId;

  /* link-information is stored directely after start marker 
     ( in content part ) */
  pu32Addr += (POOL_START_MARKER_OFFSET/sizeof(Uint32Type));
  *pu32Addr = (Uint32Type)pu32NextAddr;   /* link all elements together */

#ifdef POOL_END_PATTERN      /* START DECLARE POOL_END_PATTERN */
  /* set the end marker to POOL_END_FREE */
  pu32Addr += (ALIGN_TO_MULT4(u32ElementSize)/sizeof(Uint32Type));
  *pu32Addr = POOL_END_FREE;
#endif                      /* END DECLARE POOL_END_PATTERN */

  return(pu32NextAddr);
}  /* End function POOL_pu32InitElement */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfInit                                                  */
/**
    \brief      Initialise the POOL module

    \param      void

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfInit(void)
{
  /* Variable declaration */
  SuccessFailType sfRet      = FAIL;
  Uint32Type u32PoolId       = POOL_INVALID_POOLID;
  // Uint32Type u32ElementSize  = 0;
  Uint32Type u32NoOfElements = 0;
  Uint32Type *pu32Addr       = (Uint32Type *) 0;
  Uint32Type //u32Addr         = 0, 
      u32Offset              = 0 , 
      u32Elem                = 0;
  POOL_stGenControl *pstGp   = &poolGenControl[0];
  POOL_stControl  *pstPc     = &poolControl[0];

  /* Return failure if the initialization of the pool is already done */
  if (_POOL_u8InitDone == FALSE) {
       /* Initialization all the pools which are defined */
       for (u32PoolId = 0U; u32PoolId < (Uint32Type)POOL_enLAST; u32PoolId++,pstGp++,pstPc++) {
	   // u32ElementSize = pstGp->elementSize;
	    if ( pstGp->elementSize <  POOL_MINIMUM_SIZE ) {
		POOL_lastInitFailure = POOL_InitMinimumPoolSizeUnderrun;
		if ( POOL_pcbInitCallBack ) {
		    (*POOL_pcbInitCallBack) (POOL_InitMinimumPoolSizeUnderrun,
					     (POOL_enPoolNames)u32PoolId,
					     pstGp->addr);
		}
		return FAIL;
	    }

            /* extracting the number of elements and size of each element in 
               the pool */
	    u32NoOfElements      = pstGp->noOfElements;
            pstPc->u32Size       = pstGp->elementSize;
            pstPc->u16MaxCount   = u32NoOfElements;
            pstPc->u16FreeCount  = u32NoOfElements;
            pstPc->u16StatMaxCount = 0;

            /* filling additional control-elements here .. */
            pstPc->pu8PoolElemPtr  = pstGp->addr;
            pstPc->pvFirstFreeElem = pstGp->addr;
            pstPc->pvLastFreeElem  = pstGp->addr + 
		((u32NoOfElements-1)* (POOL_OFFSET+ ALIGN_TO_MULT4(pstGp->elementSize)));

            /* now link all elements together */
            //u32Addr = (Uint32Type) pstGp->addr;
            pu32Addr = (Uint32Type *) pstGp->addr;
            u32Offset = pstPc->u32Size + POOL_OFFSET;

            for (u32Elem = 0; u32Elem < u32NoOfElements;u32Elem++) {
              if (u32Elem == u32NoOfElements-1) {
                u32Offset = 0U;
              }
              //u32Addr = POOL_pu32init_element(u32Addr,u32PoolId,pstGp->elementSize,u32Offset);
              pu32Addr = POOL_pu32InitElement(pu32Addr,u32PoolId,pstGp->elementSize,u32Offset);
            } /*end for all elements in one pool */
       } /* end for all pools defined in the system */
       _POOL_u8InitDone = TRUE;
       sfRet = SUCCESS;
   } 
  /* Initialization is done properly */
  return sfRet;
} /* End Function POOL_sfInit */

#endif /* END DECLARE INTERFACE IPOOL_INIT */


#ifdef IPOOL_OPERATION /* BEGIN DECLARE INTERFACE(IPOOL_OPERATION) */
/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfRestorePoolId                                         */
/**
    \brief      returns pool Id

    \brief      This function tries to restore Pool Id.

    \param      addr
                address of the pool element which is returned.

    \return     Uint32Type
                returns pool Id
   */
/*----------------------------------------------------------------------------*/

Uint32Type POOL_sfRestorePoolId(Uint32Type u32Addr)
{
  Uint32Type u32I = POOL_INVALID_POOLID;
  POOL_stControl  *pstPc=NULL;
  Uint32Type u32StartAddr = 0,u32EndAddr = 0;

  /* Loop over the Pools */
  for (u32I = 0U,pstPc = &poolControl[0]; u32I < (Uint32Type)POOL_enLAST; u32I++,pstPc++) {
    /* Try to get the pool id. Check if the address returned is lying in the 
       Pool range */
    u32StartAddr = (Uint32Type)pstPc->pu8PoolElemPtr;
    u32EndAddr = u32StartAddr + (ALIGN_TO_MULT4(pstPc->u32Size)+POOL_OFFSET)*(pstPc->u16MaxCount);

    if ((u32StartAddr <= u32Addr) && (u32Addr < u32EndAddr)) {
       break;
    }
  }
  return u32I;
} /* End function POOL_sfRestorePoolId */


/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfAllocate                                              */
/**
    \brief      allocates a pool element from a pool

    \brief      fetches a pool element from a pool. It will always fetch the 
                element from starting of the list. 

    \param      enPoolName
                Name of the Pool(enum value) from which element is requested.

    \param      pvElement
                location to store the element address

    \return     SuccessFailType
                succes or FAIL, for details on errors call the correspondig \n
                error reporting routine
   */
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfAllocate (POOL_enPoolNames enPoolName,
				void             **pvElement)
{
    Uint32Type      *pu32Var=NULL;
    Uint32Type      *puNextPtr=NULL;
    POOL_stControl  *pstPc=NULL;
#ifdef POOL_END_PATTERN     /* START DECLARE POOL_END_PATTERN */
    Uint8Type *pu8CharPtr=NULL;
#endif                     /* END DECLARE POOL_END_PATTERN */

    /* MUTEX is taken before starting operation on the pools. This will be 
       changed if we will have one semaphore per pool. Currently one semaphore
       is taken for complete Memory pool handling and it is using semaphore 
       provided by linux OS. This needs to be ported for  */

    if (_POOL_u8InitDone == TRUE) {
	POOL_MUTEX_LOCK();
	
#ifdef POOL_DEBUG_CONC_TEST
	/*
	 * we delay it to force a second thread 
	 * to be able to access the mutex
	 */
	usleep(1);
#endif
	/* Check if the Pool Id is in the defined range */
	//if ((enPoolName < 0) || (enPoolName >= POOL_enLAST)) {
	if (enPoolName >= POOL_enLAST) {
	    /* In the failure case application should take the action if this error 
	       is being detected by pool handler i.e. reseting the complete system, 
	       which can only be done by S1NN application.*/
	    *pvElement = NULL;
	    POOL_lastAllocateFailure = POOL_enAllocateIllegalValue;
	    if ( POOL_pcbAllocateCallBack ) {
		(*POOL_pcbAllocateCallBack) (POOL_enAllocateIllegalValue,
					     enPoolName,
					     (char *) 0);
	    }
	    POOL_MUTEX_UNLOCK();	    
	    return(FAIL);
	}
	
	/* Fetch the correponding Pool address */
	pstPc=&poolControl[enPoolName];
	
	/* check the number of free elements available */
	if (pstPc->u16FreeCount == 0) {
	    /* In the failure case application should take the action if this error 
	       is being detected by pool handler i.e. reseting the complete system, 
	       which can only be done by S1NN application.*/
	    *pvElement = NULL;
	    POOL_lastAllocateFailure = POOL_enPoolEmpty;
	    if ( POOL_pcbAllocateCallBack ) {
		(*POOL_pcbAllocateCallBack) (POOL_enPoolEmpty,
					     enPoolName,
					     (char *) 0);
	    }
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
	
	/* take first element */
	pu32Var = pstPc->pvFirstFreeElem; /* points to startHead */
	
	/* Check for head marker */
	if ( *pu32Var != ( POOL_START_MARKER + enPoolName ) ) {
	    *pvElement = NULL;
	    POOL_lastAllocateFailure = POOL_enStartCorruption;
	    if ( POOL_pcbAllocateCallBack ) {
		(*POOL_pcbAllocateCallBack) (POOL_lastAllocateFailure,
					     enPoolName,
					     (char *) 0);
	    }
	    /* 
	     * repair it, perhaps we can continue to work
	     */
	    *pu32Var = ( POOL_START_MARKER + enPoolName );
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
	    
	/* Skip the start header i.e, 4 bytes */
	pu32Var ++;
	
	*pvElement = pu32Var;  /* points to first byte of content */
	/* set the StatMaxCounter to the maximum value, to check how much pool elements was used 
	during one driving cycle */
    if(pstPc->u16StatMaxCount < (pstPc->u16MaxCount - pstPc->u16FreeCount))
    {
        pstPc->u16StatMaxCount = (pstPc->u16MaxCount - pstPc->u16FreeCount);
    }
	

	/* remember the remaining link list */
	puNextPtr = (Uint32Type*) *pu32Var; /* point to link */	
	
	/* Initialize the memory if it is defined in the configurable file*/

#ifdef POOL_INITIALIZATION_AT_ALLOCATE 
	/* BEGIN DECLARE POOL_INITIALIZATION_AT_ALLOCATE */
	memset(pu32Var,0,pstPc->u32Size);
#endif  /* END DECLARE POOL_INITIALIZATION_AT_ALLOCATE */

#ifdef POOL_END_PATTERN     /* START DECLARE POOL_END_PATTERN */
	/* change the END_TRAILER to POOL_END_BUSY */
	pu8CharPtr = (Uint8Type*)pu32Var + ALIGN_TO_MULT4(pstPc->u32Size);
	/* Is the end marker ok ? */
	if ( *(Uint32Type*)pu8CharPtr != POOL_END_FREE ) {
	    POOL_lastFreeFailure = POOL_enEndCorruption;
	    if ( POOL_pcbAllocateCallBack ) {
		(*POOL_pcbAllocateCallBack) (POOL_lastFreeFailure,
					     enPoolName,
					     pvElement);
	    }
	    /*
	     * we can repair this, so let's do it
	     */
	    *(Uint32Type*)pu8CharPtr = POOL_END_FREE;
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
	*(Uint32Type*)pu8CharPtr = POOL_END_BUSY;
#endif                     /* END DECLARE POOL_END_PATTERN */
	/* Decrement the count i.e. available number of elements */
	pstPc->u16FreeCount--;
	
	/* If we removed the last element, we must clear the pomter to the last element */
	if ( pstPc->u16FreeCount == 0 ) {
	    pstPc->pvLastFreeElem = 0;
	}
	
	/* Update the last free element */
	pstPc->pvFirstFreeElem = puNextPtr;
    
	/* Release the semaphore */
	POOL_MUTEX_UNLOCK();
	return SUCCESS;
    } else { /* Pool was not initialized */
	POOL_lastAllocateFailure = POOL_enPoolNotInitializedAllocate;
	if ( POOL_pcbAllocateCallBack ) {
	    (*POOL_pcbAllocateCallBack) (POOL_lastAllocateFailure,
				     (POOL_enPoolNames)POOL_INVALID_POOLID,
				     (void *) 0);
	}
	/* No Mutex was locked */
	return FAIL;
    }

} /* End function POOL_sfAllocate */
/*----------------------------------------------------------------------------*/
/* Function    : POOL_enGetAllocateErrorCode                                  */
/**
    \brief     returns the last error from an allocate call
    \brief     a successfull call to POOL_sfAllocate will NOT \n
               overwrite this error code
    \param     void

    \return    detailed error code
*/
/*----------------------------------------------------------------------------*/
POOL_enAllocateErrorCode POOL_enGetAllocateErrorCode(void)
{
    return POOL_lastAllocateFailure;
} /* End function POOL_enAllocateErrorCode */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfFree                                                  */
/**
    \brief      returns a pool element to a pool

    \brief      The element is check according to the check rules and \n
                then reinserted into the pool, it was take from. It will 
                insert the element at the end of the pool.

    \param      pvElement
                pointer to an element, which needs to be freed.

    \return     SuccessFailType
                succes or FAIL, for details on errors call the correspondig \n
                error reporting routine
   */
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfFree(void *pvElement)
{
    /* Variable declaration */
    Uint32Type *pu32ElemPtr = NULL;
    Uint32Type *pu32ElemPtrHdrMarker = NULL;
    Uint32Type  u32PoolId=POOL_INVALID_POOLID;
    POOL_stControl  *pstPc=NULL;
#ifdef POOL_END_PATTERN /* START DECLARE POOL_END_PATTERN */
    Uint8Type *pu8elemPtrTailMarker = NULL;
    Uint32Type *pu32elemPtrTailMarker32 = NULL;
#endif /* END DECLARE POOL_END_PATTERN */
    Uint32Type *pu32lastElement = NULL;
    SuccessFailType sfStatus = SUCCESS;
    
    if (_POOL_u8InitDone == TRUE) {
	/* Lock the semaphore */
	POOL_MUTEX_LOCK();
#ifdef POOL_DEBUG_CONC_TEST
	/*
	 * we delay it to force a second thread 
	 * to be able to access the mutex
	 */
	usleep(1);
#endif
	
	/* check for the NULL element */
	if (pvElement == NULL) {
	    /* In the failure case application should take the action if this error 
	       is being detected by pool handler i.e. reseting the complete system, 
	       which can only be done S1NN application.*/
	    POOL_lastFreeFailure = POOL_enElementNull;
	    if ( POOL_pcbFreeCallBack ) {
		(*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
					 (POOL_enPoolNames)POOL_INVALID_POOLID,
					 pvElement);
	    }
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
#ifdef POOL_ADDRESS_RANGE
	if ( (pvElement < POOL_BASE_ADDRESS) || (pvElement > POOL_END_ADDRESS) ) {
	    POOL_lastFreeFailure = POOL_enOutOfAdressRange;
	    if ( POOL_pcbFreeCallBack ) {
		(*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
					 POOL_INVALID_POOLID,
					 pvElement);
	    }
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
#endif
	/* Initialize the local variables  for traversing the element of pool */
	pu32ElemPtr = (Uint32Type*)pvElement; /* points to content now */
	pu32ElemPtrHdrMarker = pu32ElemPtr-1; /* points to header field */
	u32PoolId = (*pu32ElemPtrHdrMarker)- POOL_START_MARKER;
	
	/* check if the extracted pool id is in the range of pool id's */
	if (u32PoolId >= (Uint32Type)POOL_enLAST) {
	    /* maybe we can restore the u32PoolId from the element-address: */
	    u32PoolId = POOL_sfRestorePoolId((Uint32Type)pvElement);
	    if (u32PoolId != POOL_INVALID_POOLID) {
		/* Pool id is restored but we have to set the last error as invalid 
		   Pool id else application will not come to know about the same */
		POOL_lastFreeFailure = POOL_enInvalidPoolId;
	    } else {
		POOL_lastFreeFailure = POOL_enStartViolation;
	    }
	    if ( POOL_pcbFreeCallBack ) {
		(*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
					 (POOL_enPoolNames)u32PoolId,
					 pvElement);
	    }
	    /*
	     * This error can be recovered, here the start header was overwritten
	     * but if we have been able to recover the pool id, we can put the 
	     * element back into the 
	     */
	    if (u32PoolId == POOL_INVALID_POOLID) {    
		POOL_MUTEX_UNLOCK();
		return FAIL;
	    } else {
		/*
		 * restore the start marker
		 */
		(*pu32ElemPtrHdrMarker) = u32PoolId + POOL_START_MARKER;
		sfStatus = FAIL;
	    }
	}

	/* make some checks on poolControl */
	pstPc=&poolControl[u32PoolId];
	
#ifdef POOL_END_PATTERN /* START DECLARE POOL_END_PATTERN */
	pu8elemPtrTailMarker = (Uint8Type*)pu32ElemPtr + ALIGN_TO_MULT4(pstPc->u32Size);
	pu32elemPtrTailMarker32 = (Uint32Type*)pu8elemPtrTailMarker;
	
#ifdef POOL_FREE /* BEGIN DECLARE POOL_FREE */
	/* check if the element has been freed earlier or not */
	if (*pu32elemPtrTailMarker32 == POOL_END_FREE) { 
	    POOL_lastFreeFailure = POOL_enAllreadyFreed;
	    if ( POOL_pcbFreeCallBack ) {
		(*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
					 u32PoolId,
					 pvElement);
	    }
	    POOL_MUTEX_UNLOCK();
	    return FAIL;
	}
#endif /* END DECLARE POOL_FREE */

	/* check if the end of the buffer i.e. end marker has been overwritten by
	   application */
	if (*pu32elemPtrTailMarker32 != POOL_END_BUSY) {
	    POOL_lastFreeFailure = POOL_enEndViolation;
	    if ( POOL_pcbFreeCallBack ) {
		(*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
					 u32PoolId,
					 pvElement);
	    }
	    /*
	     * this can be repaired by writing a new end marker
	     */
	    sfStatus = FAIL;
	    *pu32elemPtrTailMarker32 = POOL_END_BUSY;
	}
	//#ifdef POOL_FREE /* BEGIN DECLARE POOL_FREE */
	/*
	 * Mark the element as freed
	 */
	*pu32elemPtrTailMarker32 = POOL_END_FREE;
	//#endif
#endif /* END DECLARE POOL_END_PATTERN */
	
	/* ok, the checks for both Marker are ok,
	   now put the element back into pool-control list at the end of the list */
	
	/* we can do this in case list is non empty */
	if(pstPc->pvLastFreeElem !=NULL) {
	    /* get the last element */
	    pu32lastElement = (Uint32Type*)pstPc->pvLastFreeElem;    
	    /* skip the start marker */
	    pu32lastElement++;       
	    /* Add to the current list of free elements */
	    *pu32lastElement = (Uint32Type)pu32ElemPtrHdrMarker;
	    /* assign the last element value as NULL */           
	    *pu32ElemPtr = (Uint32Type)NULL;             
	    /* Update the last free element */             
	    pstPc->pvLastFreeElem = pu32ElemPtrHdrMarker;            
	} else {
	    *pu32ElemPtr = (Uint32Type)NULL; /* clear forward ptr */
	    pstPc->pvLastFreeElem  = pu32ElemPtrHdrMarker; /* first and last pint to this element */
	    pstPc->pvFirstFreeElem = pu32ElemPtrHdrMarker;
	}
	
	/* increment the element count */
	pstPc->u16FreeCount++;
	
	/* Unlock the semaphore */
	POOL_MUTEX_UNLOCK();
    } else { /* Pool was not initialized */
	POOL_lastFreeFailure = POOL_enPoolNotInitializedFree;
	if ( POOL_pcbFreeCallBack ) {
	    (*POOL_pcbFreeCallBack) (POOL_lastFreeFailure,
				     (POOL_enPoolNames)POOL_INVALID_POOLID,
				     (void *) 0);
	}
	/* No Mutex was locked */
	sfStatus = FAIL;
    }
    return sfStatus;
} /* End function POOL_sfFree */


/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfGetCurrentPoolSize                                    */
/**
    \brief     return the number of free elements in a pool

    \param     enPoolName
               Name of the Pool (enum value)

    \return    Uint32Type
               Returns number of elements available in the same pool
*/
/*----------------------------------------------------------------------------*/
Uint32Type POOL_sfGetCurrentPoolSize(POOL_enPoolNames enPoolName)
{ 
    /* Get the Pool control structure */
    POOL_stControl  *pstPc=&poolControl[enPoolName];
    return pstPc->u16FreeCount;
} /* End function POOL_sfGetCurrentPoolSize */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfGetMaximumPoolSize                                    */
/**
    \brief     return the total number of elements in a pool

    \param     enPoolName
               Name of the Pool (enum value)

    \return    Uint32Type
               Returns number of elements in the pool
*/
/*----------------------------------------------------------------------------*/
Uint32Type POOL_sfGetMaximumPoolSize(POOL_enPoolNames enPoolName)
{ 
    /* Get the Pool control structure */
    POOL_stControl  *pstPc=&poolControl[enPoolName];
    return pstPc->u16MaxCount;
} /* End function POOL_sfGetMaximumPoolSize */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfGetPoolElementSize                                    */
/**
    \brief     return the size of a elements in a pool

    \param     enPoolName
               Name of the Pool (enum value)

    \return    Uint32Type
               Returns the size of a element in the pool
*/
/*----------------------------------------------------------------------------*/
Uint32Type POOL_sfGetPoolElementSize(POOL_enPoolNames enPoolName)
{ 
    /* Get the Pool control structure */
    POOL_stControl  *pstPc=&poolControl[enPoolName];
    return pstPc->u32Size;
} /* End function POOL_sfGetPoolElementSize */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_enGetFreeErrorCode                                      */
/**
    \brief     returns the last error from a free call
    \brief     a successfull call to POOL_sfFree will NOT \n
               overwrite this error code
    \param     void

    \return    detailed error code
*/
/*----------------------------------------------------------------------------*/
POOL_enFreeErrorCode POOL_enGetFreeErrorCode(void)
{
    return POOL_lastFreeFailure;
} /* End function POOL_enGetFreeErrorCode */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_enGetInitErrorCode                                      */
/**
    \brief     returns the last error from a init call
    \brief     a successfull call to POOL_sfInit will NOT \n
               overwrite this error code
    \param     void

    \return    detailed error code
*/
/*----------------------------------------------------------------------------*/
POOL_enInitErrorCode POOL_enGetInitErrorCode(void)
{
    return POOL_lastInitFailure;
} /* End function POOL_enGetInitErrorCode */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfSetAllocateCallback                                   */
/**
    \brief     defines a callback for errors in Allocate
    \brief     defines a callback routine to be called, if an error in the  \n
               POOL_sfAllocate routine happens

    \param     pcbCallback
               Name of the Callback function which needs to be called.

    \param     enCode
               Error code

    \param     enPool
               Pool name.

    \return    SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfSetAllocateCallback(void (*pcbCallback) 
                                           (POOL_enAllocateErrorCode enCode, 
					    POOL_enPoolNames         enPool,
					    void                     *vPtr))
{
    POOL_pcbAllocateCallBack = pcbCallback;
    return SUCCESS;
} /* End function POOL_sfSetAllocateCallback */

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfSetFreeCallback                                       */
/**
    \brief     defines a callback for errors in Free
    \brief     defines a callback routine to be called, if an error in the  \n
               POOL_sfFree routine happens
    \param     pcbCallback
               Name of the Callback function which needs to be called.

    \param     enCode
               Error code

    \param     enPool
               Pool name.

    \return    SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfSetFreeCallback(void (*pcbCallback) 
                                       (POOL_enFreeErrorCode enCode,
                                        POOL_enPoolNames     enPool,
					void                 *vPtr))
{
    POOL_pcbFreeCallBack = pcbCallback;
    return SUCCESS;
} /* End function POOL_sfSetFreeCallback */
/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfSetFreeCallback                                       */
/**
    \brief     defines a callback for errors in Free
    \brief     defines a callback routine to be called, if an error in the  \n
               POOL_sfFree routine happens
    \param     pcbCallback
               Name of the Callback function which needs to be called.

    \param     enCode
               Error code

    \param     enPool
               Pool name.

    \param     vPtr
               Pointer to the payload field of the pool element

    \return    SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType POOL_sfSetInitCallback(void (*pcbCallback) 
                                       (POOL_enInitErrorCode enCode,
                                        POOL_enPoolNames     enPool,
					void                 *vptr))
{
    POOL_pcbInitCallBack = pcbCallback;
    return SUCCESS;
} /* End function POOL_sfSetInitCallback */

#endif /* END DECLARE INTERFACE IPOOL_OPERATION */


/* End Of File POOL_C1.c */
