/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     NM_CB.h
*-----------------------------------------------------------------------------
* Module Name:	  MOD_NM
*-----------------------------------------------------------------------------
* Description:	  callback header for the module MOD_NM
*-----------------------------------------------------------------------------
* $Date: 2009-03-17 10:00:30 +0100 (Di, 17 Mrz 2009) $
* $Rev: 5310 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/NM_CB.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef NM_CB_H_
#define NM_CB_H_

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "NM_CB_cfg.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : ApplCheckLimphome                                            */
/** 
    \brief      Callback function for test if NM is in limphome state
  
    \brief      Description:\n
                function for limphome handling \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplCheckLimphome(void);

/*----------------------------------------------------------------------------*/
/* Function    : ApplHandleBusOffWaitingTime                                  */
/** 
    \brief      Callback function for handling the bus off waiting time
  
    \brief      Description:\n
                function for limphome handling \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplHandleBusOffWaitingTime(void);

/*----------------------------------------------------------------------------*/
/* Function    : IsBusOff                                                     */
/** 
    \brief      Callback function to get the bus off state
  
    \brief      Description:\n
                function to get the actual CAN bus state \n

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type IsBusOff(void);

/*----------------------------------------------------------------------------*/
/* Function    : IsCanActive                                                    */
/** 
    \brief      Callback function to get the CAN state
  
    \brief      Description:\n
                function to get the actual CAN driver state \n

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type IsCanActive(void);

#endif /* _NM_CB_H */

/* End Of File NM_CB.h */
