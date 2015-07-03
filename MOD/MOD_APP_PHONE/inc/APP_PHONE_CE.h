/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_CE.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE
*-----------------------------------------------------------------------------
* Description:  
*-----------------------------------------------------------------------------
* $Date: 2011-04-08 13:18:18 +0200 (Fr, 08 Apr 2011) $
* $Rev: 18941 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_CE_H
#define _APP_PHONE_CE_H

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/


#define APP_PHONE_PIEV_INVALID_VALUE 0xFF
#define APP_PHONE_OUTGOING_CALL_INDICATION_TIMEOUT 15000 // 15 seconds

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	at_cmd_ handler_tel 
*/
//#define IUART_TEMPLATE_CFG 110
//#if (IUART_TEMPLATE_CFG != IUART)
//	#error ("Incompatible interface: configured ifc = %d, reference ifc = %d!!!" ,IUART_TEMPLATE_CFG, IUART)
//#endif
	

#endif /* _APP_PHONE_CE_H */

/* End Of File APP_PHONE_CE.h */
