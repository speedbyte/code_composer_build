/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL.h
*-----------------------------------------------------------------------------
* Module Name: Pool handling
*-----------------------------------------------------------------------------
* Description: This file contains internal data structures flags used for Pool
*              handling (prototype).
*-----------------------------------------------------------------------------
* $Date: 2009-06-12 11:57:39 +0200 (Fr, 12 Jun 2009) $
* $Rev: 6631 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/inc/POOL.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _POOL_H
#define _POOL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <string.h>

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "POOL_GENERATED.h"
#include "POOL_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/
/**
    \brief      description of a pool
*/
typedef struct {
    Uint8Type *pu8PoolElemPtr;    /* address of static array of pool */
    void *pvFirstFreeElem;       /* address of first element */
    void *pvLastFreeElem;        /* address of last element  */
    /** \brief Semaphore to block multithreaded access 
               Todo: Clarify type
    */
    Uint32Type u32Semaphore;  /* Semaphore for each pool */
    /** \brief Size of pool elements (only for non production code)*/
    Uint32Type u32Size;
    /** \brief Maximum count of elements */
    Uint16Type u16MaxCount;
    /** \brief Maximum count of elements during life cycle */
    Uint16Type u16StatMaxCount;
    /** \brief Free count of elements */
    Uint16Type u16FreeCount;
    /** \brief Flags to control error checking and guard functions
               will be used in error handling */
    Uint16Type u16BehaviourFlags;
} POOL_stControl;

/**
  \brief Error codes of pool handling
*/
typedef enum {
    /** \brief No error */
    POOL_enAllocateNoFailure,
    /** \brief Illegal parameter */
    POOL_enAllocateIllegalValue,
    /** \brief Out of buffer elements in this pool*/
    POOL_enPoolEmpty,
    /** \brief Signal that pool handling was not initialized*/
    POOL_enPoolNotInitializedAllocate,
    /** \brief start of element marker destroyed, 
         this happens, if an element in the free chain 
         has it's header destroyed */
    POOL_enStartCorruption,
    /** \brief end of element marker destroyed, 
         this happens, if an element in the free chain 
         has it's end marker destroyed */
    POOL_enEndCorruption

    /* TODO: There will be more error conditions */
} POOL_enAllocateErrorCode;

typedef enum {
    /** \brief No Error */
    POOL_enFreeNoFailure,
    /** \brief start of element marker destroyed, 
         this will only reported by POOL_enPut */
    POOL_enStartViolation,
    /** \brief End of element marker destroyed, 
         this will only reported by POOL_enPut */
    POOL_enEndViolation,
    /** \brief This element was allready freed */
    POOL_enAllreadyFreed,
    /** \brief This element was NULL */
    POOL_enElementNull,
    /** \brief Invalid Pool Id */
    POOL_enInvalidPoolId,
    /** \brief Address outside of the address range */
    POOL_enOutOfAdressRange,
    /** \brief Signal that pool handling was not initialized*/
    POOL_enPoolNotInitializedFree
    /* TODO: There will be more error conditions */
} POOL_enFreeErrorCode;

typedef enum {
    /** \brief No Error */
    POOL_enInitNoFailure,
    /** \brief Minimum pool size underrun */
    POOL_InitMinimumPoolSizeUnderrun
} POOL_enInitErrorCode;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/**
\brief	description of the interface IPOOL_INIT
*/
#ifdef IPOOL_INIT /* BEGIN DECLARE INTERFACE(IPOOL_INIT) */
/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfInit                                                  */
/**
    \brief      Initialise the POOL module

    \param      void

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType POOL_sfInit(void);

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
extern SuccessFailType POOL_sfSetInitCallback(void (*pcbCallback) 
					      (POOL_enInitErrorCode enCode,
					       POOL_enPoolNames     enPool,
					       void                 *vptr));


#endif /* END DECLARE INTERFACE IPOOL_INIT */


#ifdef IPOOL_OPERATION /* BEGIN DECLARE INTERFACE(IPOOL_OPERATION) */
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
extern SuccessFailType POOL_sfAllocate(POOL_enPoolNames enPoolName,
                                        void             **pvElement);

/*----------------------------------------------------------------------------*/
/* Function    : POOL_sfGetCurrentPoolSize                                    */
/**
    \brief     defines a callback for errors in Allocate
    \brief     defines a callback routine to be called, if an error in the  \n
               POOL_sfAllocate routine happens
    \param     enPoolName
               Name of the Pool (enum value)

    \return    Uint32Type
               Returns number of elements available in the same pool
*/
/*----------------------------------------------------------------------------*/
extern Uint32Type POOL_sfGetCurrentPoolSize(POOL_enPoolNames enPoolName);

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
extern POOL_enAllocateErrorCode POOL_enGetAllocateErrorCode(void);
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
extern Uint32Type POOL_sfGetMaximumPoolSize(POOL_enPoolNames enPoolName);
/* Function    : POOL_sfGetPoolElementSize                                    */
/**
    \brief     return the size of a elements in a pool

    \param     enPoolName
               Name of the Pool (enum value)

    \return    Uint32Type
               Returns the size of a element in the pool
*/
/*----------------------------------------------------------------------------*/
extern Uint32Type POOL_sfGetPoolElementSize(POOL_enPoolNames enPoolName);

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

    \param     vPtr
               Pointer to the payload field of the pool element

    \return    SuccessFailType
*/
/*----------------------------------------------------------------------------*/
extern SuccessFailType POOL_sfSetAllocateCallback(void (*pcbCallback) 
                                            (POOL_enAllocateErrorCode enCode,
					     POOL_enPoolNames     enPool,
					     void                 *vPtr));

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
extern SuccessFailType POOL_sfFree(void             *pvElement);

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

extern POOL_enFreeErrorCode POOL_enGetFreeErrorCode(void);
extern POOL_enInitErrorCode POOL_enGetInitErrorCode(void);

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
extern SuccessFailType POOL_sfSetFreeCallback(void (*pcbCallback)
                                           (POOL_enFreeErrorCode enCode,
                                            POOL_enPoolNames     enPool,
					    void                 *vPtr));

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
extern Uint32Type POOL_sfRestorePoolId(Uint32Type addr);


/*----------------------------------------------------------------------------*/
/* Function    : POOL_enGetAllocateErrorCode                                  */
/**
    \brief     returns the last error from a allocate call
    \brief     a successfull call to POOL_sfAllocate will NOT \n
               overwrite this error code
    \param     void

    \return    detailed error code
*/
/*----------------------------------------------------------------------------*/
extern POOL_enAllocateErrorCode POOL_enGetAllocateErrorCode(void);

#endif /* END DECLARE INTERFACE IPOOL_OPERATION */

#endif /* _POOL_H */

/* End Of File pool.h */
