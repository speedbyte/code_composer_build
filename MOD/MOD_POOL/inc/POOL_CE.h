/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_CE.h
*-----------------------------------------------------------------------------
* Module Name:    POOL Handling
*-----------------------------------------------------------------------------
* Description:  This file contains External interface definition (prototype).
*-----------------------------------------------------------------------------
* $Date: 2009-03-16 10:34:10 +0100 (Mo, 16 Mrz 2009) $
* $Rev: 5237 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/inc/POOL_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _POOL_CE_H
#define _POOL_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
/** \brief CONFIGURATE EXPORT INTERFACE(IPOOL_INIT) */
#define IPOOL_INIT  100 
/** \brief CONFIGURATE EXPORT INTERFACE(IPOOL_OPERATION) */
#define IPOOL_OPERATION 100 

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/* Mutex Hash defines */
//#define POOL_MUTEX_LOCK()   pthread_mutex_lock(&pMutex)
//#define POOL_MUTEX_UNLOCK() pthread_mutex_unlock(&pMutex)
//#define POOL_MUTEX_LOCK()   (void)0
//#define POOL_MUTEX_UNLOCK() (void)0
#define POOL_MUTEX_LOCK()   semP(POOL_LockResource)
#define POOL_MUTEX_UNLOCK() semV(POOL_LockResource)

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#ifdef POOL_ADDRESS_RANGE
/**
  \brief Base address where all pools start. This must be configured by the integrator
  for default an adddress of 4 is the minimun reasonable value
*/
#define POOL_BASE_ADDRESS ((void *) 4)
/**
  \brief Highest address where all pools end. This must be configured by thw integrator
*/
#define POOL_END_ADDRESS ((void *) 0xffffffff)
#endif

#endif /* _POOL_CE_H */

/* End Of File POOL_CE.h */
