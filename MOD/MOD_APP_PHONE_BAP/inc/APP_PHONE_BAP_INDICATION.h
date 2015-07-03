/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_BAP_INDICATION.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE_BAP    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONE_BAP
*-----------------------------------------------------------------------------
* $Date: 2010-07-26 15:01:49 +0530 (Mo, 26 Jul 2010) $
* $Rev: 14027 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/inc/APP_PHONE_BAP_INDICATION.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_BAP_INDICATION_H
#define _APP_PHONE_BAP_INDICATION_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

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
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_vHandleBAPIndication(BapIndication *bapInd)       */  
/** 
    \brief      handles bap indications from dispatcher 
  
    \brief      Description:\n
                Handles Phone application specific BAP indications \n
                from BAP Dispatcher \n
     
    \param      puart
                const SCI_ST*
                no limits
    
    \param      bapInd
                BapIndication *                

    \return     void 
        
   */
/*----------------------------------------------------------------------------*/
void _APP_PHONE_BAP_vHandleBAPIndication(const BapIndication *bapInd );


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessResumeCall                          */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                initPart

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessResumeCall(BAP_enResultProcessing ResumeState);

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessDialNumber                          */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                DialState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessDialNumber(BAP_enResultProcessing DialState);


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessAcceptCall                              */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessAcceptCall(BAP_enResultProcessing CallState);


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessHangUpCall                              */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessHangUpCall(BAP_enResultProcessing CallState);

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall             */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPCallHoldAcceptWaitingCall(BAP_enResultProcessing CallState);


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPSwap                                  */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPSwap(BAP_enResultProcessing CallState);

/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessCallHold                                */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessCallHold(BAP_enResultProcessing CallState);


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessCCJoin                                  */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessCCJoin(BAP_enResultProcessing CallState);


/*----------------------------------------------------------------------------*/
/* Function :   APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall    */
/**
  \brief        This method is called to send the BAP result notification

  \param        BAP_enResultProcessing
                CallState

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_PHONE_BAP_vProcessMPReleaseActiveCallAcceptWaitingCall(BAP_enResultProcessing CallState);


#endif /* _APP_PHONE_BAP_H */

/* End Of File APP_PHONE_BAP_INDICATION.h */

