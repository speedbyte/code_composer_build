/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     PHY_BUT_DRV_CE.h
*-----------------------------------------------------------------------------
* Module Name:    PHY_BUT_DRV
*-----------------------------------------------------------------------------
* Description:    Driver (host task)  part of 3 Button module handling
*-----------------------------------------------------------------------------
* $Date: 2009-02-19 18:33:49 +0100 (Do, 19 Feb 2009) $
* $Rev: 4741 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/trunk/inc/PHY_BUT_DRV_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _PHY_BUT_DRV_CE_H
#define _PHY_BUT_DRV_CE_H
/*----------------------------------------------------------------------------*/
/* Module Documentation                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define IPHY_BUT_DRV_HOST

#define IPHY_BUT_DRV_INIT  

#define IPHY_BUT_DRV_DIAGNOSE_CRADLE

#define IPHY_BUT_DRV_APP_CSHDL
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/**
   \brief minimum hang detection time time

   \brief describes the minimum time for detection of a hanging button
          this is defined in ticks of the host task (currently 10ms)
*/
#define _PHY_BUT_DRV_MINHANG 1000          // corresponds to 30 seconds
#define _PHY_BUT_DRV_LONG_PRESS 325

/**
   \brief Flag to switch to module test

   \brief This flag switches the ADC reading routine to a predefined set  \n
          of voltage values to stimulate different test cases \n
          THis flag MUST NOT be used in production builds
*/
#undef _PHY_BUT_DRV_MODUL_TEST

#endif /* _PHY_BUT_DRV_CE_H */

/* End Of File PHY_BUT_DRV_CE.h */
