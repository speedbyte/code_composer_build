/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_DISPATCHER_CE.h
*-----------------------------------------------------------------------------
* Module Name:    BAP_DISPATCHER
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2010-01-29 15:16:54 +0100 (Fr, 29 Jan 2010) $
* $Rev: 10936 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/inc/BAP_Dispatcher_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _TEMPLATE_CE_H
#define _TEMPLATE_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define ITEMPLATE 100 /* CONFIGURATE EXPORT INTERFACE(ITEMPLATE) */

typedef enum
{   
    BAP_MAX_BYTE_SEQ_enAppPBPbState                 = 3,
    BAP_MAX_BYTE_SEQ_enAppTelFsgSetup               = 4,
    MAP_MAX_BYTE_SEQ_enAppMissedCallsIndication     = 4,
    BAP_MAX_BYTE_SEQ_enAppTelMobileServiceSupport   = 6,
    BAP_MAX_BYTE_SEQ_enAppTelCallState              = 8
}BAP_MAX_BYTE_SEQ_SIZES;


/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/
//#define IUART_TEMPLATE_CFG 110
//#if (IUART_TEMPLATE_CFG != IUART)
//	#error ("Incompatible interface: configured ifc = %d, reference ifc = %d!!!" ,IUART_TEMPLATE_CFG, IUART)
//#endif
	


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
/*
 \brief Constant for the default value for FSG-Setup function
 */
//#define     TELEPHON_DEFAULT_FSG_SETUP            (0x00)
#define TELEPHONE_DEFAULT_FSG_SETUP__PHONE_CHARACTERISTICS  (0x04U) /* HFP possible */
#define TELEPHONE_DEFAULT_FSG_SETUP__MOBILE_CONNECTION_TYPE (0x00U) /* not connected */
#define TELEPHONE_DEFAULT_FSG_SETUP__SUPP_NUM_USER_PROFILES (0x04U) /* number of user profiles supported */  
#define TELEPHONE_DEFAULT_FSG_SETUP__PHONE_TYPE             (0x02U) /* UHV High */

/*
 \brief Constant for the default value for FSG-OperationState function
 */
#define     TELEPHON_DEFAULT_FSG_OPERATIONSTATE   (0x00)

/*
 \brief Constant for the default value for Registerstate function
 */
#define     TELEPHON_DEFAULT_REGISTERSTATE         (0x00)

/*
 \brief Constant for the default value for Linestate function
 */
#define     TELEPHON_DEFAULT_LINESTATE             (0x00)

/*
 \brief Constant for the default value for MediaEvent function
 */
#define     TELEPHON_DEFAULT_CALLSTATE             (0x00)

/*
 \brief Constant for the length of MOBILESERVICESUPPORT
 */
#define     BAP_TELEPHON_FCTID_MOBILESERVICESUPPORT_SIZE        ((uint16_t)6)

#endif /* _TEMPLATE_CE_H */

/* End Of File template.h */
