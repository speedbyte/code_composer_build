/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     SYS_INIT.h
*-----------------------------------------------------------------------------
* Module Name:	  SYS_INIT
*-----------------------------------------------------------------------------
* Description:	  header file to include in the files which use the Init part
*-----------------------------------------------------------------------------
* $Date: 2009-03-27 09:02:29 +0100 (Fr, 27 Mrz 2009) $
* $Rev: 5560 $
* $Author: rammelt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SYS_INIT/trunk/inc/SYS_INIT.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _SYS_INIT_H
#define _SYS_INIT_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "MOD_SYS_INIT_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface ISYS_COMPLETEINIT
*/
#ifdef ISYS_COMPLETEINIT /* BEGIN DECLARE INTERFACE(ISYS_COMPLETEINIT) */

/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_Init                                                 */
/** 
    \brief      Initialization function after Power down
  
    \brief      Description:\n
                complete init part after Power down. This \n
                includes HW, Peripherals, driver and protocols \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfInit(void);

#endif /* END DECLARE INTERFACE(ISYS_COMPLETEINIT) */

#ifdef ISYS_PARTINIT /* BEGIN DECLARE INTERFACE(ISYS_PARTINIT) */
/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfSwInit                                             */
/** 
    \brief      Initialization function for Software  
  
    \brief      Description:\n
                Initialization function for standard software \n
                core, buffer and other software dependent parts \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfSwInit(void);

/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfPeripheralsInit                                    */
/** 
    \brief      Initialization function for Peripherals
  
    \brief      Description:\n
                Initialization function for the a2d, uart, spi, i2C \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfPeripheralsInit(void);

/*----------------------------------------------------------------------------*/
/* Function    : SYSINIT_sfHwInit                                             */
/** 
    \brief      Initialization function for HW
  
    \brief      Description:\n
                Initialization function for the Hardware parts Port, external \n
                watchdog \n

    \return     SuccessFailType
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType SYSINIT_sfHwInit(void);

#endif /* END DECLARE INTERFACE(ISYS_PARTINIT) */


#endif /* _SYS_INIT_H */

/* End Of File SYS_INIT.h */
