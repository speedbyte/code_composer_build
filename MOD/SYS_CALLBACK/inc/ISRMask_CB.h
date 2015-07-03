/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     CAN_CB.h
*-----------------------------------------------------------------------------
* Module Name:	  MOD_CAN
*-----------------------------------------------------------------------------
* Description:	  configuration header for the module MOD_CAN
*-----------------------------------------------------------------------------
* $Date: 2009-03-17 10:00:30 +0100 (Di, 17 Mrz 2009) $
* $Rev: 5310 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/inc/ISRMask_CB.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef ISRMASK_CB_H_
#define ISRMASK_CB_H_

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ISRMask_CB_cfg.h"

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
/* Function    : ApplInterruptFlagMask                                        */
/** 
    \brief      Callback function for set interrupt bits
  
    \brief      Description:\n
                function to set bits for the diferent interrupts \n
                
    \return     void
   */
/*----------------------------------------------------------------------------*/
extern void ApplInterruptFlagMask(void);



#endif /* ISRMASK_CB_H_ */

/* End Of File ISRMask_CB.h */
