/******   STARTSINGLE_OF_MULTIPLE_COMMENT   ******/


/******************************************************************************
| Project Name: OSEK Network Management Extension
|    File Name: ONMXVAG.H
|
|  Description: Header of the OSEK Network Management Extension for Volkswagen.
|
|------------------------------------------------------------------------------
|               C O P Y R I G H T
|------------------------------------------------------------------------------
| Copyright (c) 2005 by Vector Informatik GmbH.            All rights reserved.
|
|               This software is copyright protected and proprietary to
|               Vector Informatik GmbH. Vector Informatik GmbH grants to you
|               only those rights as set out in the license conditions. All
|               other rights remain with Vector Informatik GmbH.
|
|------------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|------------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     --------------------------------------
| Ths          Thomas Sommer             Vector Informatik GmbH
|------------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|------------------------------------------------------------------------------
| Date       Version Author Description
| ---------- ------- ------ ---------------------------------------------------
| 2004-01-17 1.00.00 Ths    Initial version
| 2004-01-20 1.01.00 Ths    ESCAN00012016: No changes in header file.
|                           ESCAN00012017: No changes in header file.
|                           ESCAN00012018: No changes in header file.
| 2005-09-20 1.02.00 Ths    ESCAN00013702: New call-back functions added.
|*****************************************************************************/



#   if defined ONMXVAG_HEADER
#   else
# define ONMXVAG_HEADER



/*****************************************************************************/
/* Version              (abcd: Main version ab Sub Version cd )              */
/*****************************************************************************/

# define NM_DIROSEKEXT_VAG_VERSION           0x0102u
# define NM_DIROSEKEXT_VAG_RELEASE_VERSION   0x00u



/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/



/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/



/*****************************************************************************/
/* Types / Structs / Unions                                                  */
/*****************************************************************************/



/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/

#   if defined (_NEAR_) &&  defined (C_COMP_IAR_M16C)
/* const data to const segment, don´t initialize */
# pragma memory=constseg(CONST_DATA) :far
#   endif

/* Global constants with ONMX layer main and subversion */
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kOnmxMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kOnmxSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kOnmxReleaseVersion;

#   if defined (_NEAR_) && defined (C_COMP_IAR_M16C)
/* const data to const segment, don´t initialize */
# pragma memory=default
#   endif



/*****************************************************************************/
/* Global Data Definitions                                                   */
/*****************************************************************************/



/*****************************************************************************/
/* Function Prototypes                                                       */
/*****************************************************************************/

#   if defined ( NM_ENABLE_CAR_WUP_REQUEST )
extern void NM_API_CALLBACK_TYPE ApplNmCarWupRequestConfirmation(NM_CHANNEL_APPLTYPE_ONLY);
extern void NM_API_CALLBACK_TYPE ApplNmCarWupReleaseConfirmation(NM_CHANNEL_APPLTYPE_ONLY);
#   endif
extern void NM_API_CALLBACK_TYPE ApplNmWriteUserData(NM_CHANNEL_APPLTYPE_FIRST vuint8*);
#   if defined ( NM_ENABLE_READ_USER_DATA )
extern void NM_API_CALLBACK_TYPE ApplNmReadUserData(NM_CHANNEL_APPLTYPE_FIRST NmNodeType, CanChipDataPtr);
#   endif
#   if defined ( NM_ENABLE_SLEEPACK_CALLBACK )
extern void NM_API_CALLBACK_TYPE ApplNmSleepAck(NM_CHANNEL_APPLTYPE_ONLY);
#   endif



/******************************************************************************
|
| Name:          OnmExtInit
| Parameters:    channel - Handle of OSEK-NM channel.
| Return code:   void
| Description:   Initializes the OSEK-NM extension. It is called by
|                NmOsekInit()
| Particularity: Must not be called by the application.
|
|*****************************************************************************/

void NM_API_CALL_TYPE OnmExtInit(NM_CHANNEL_NMTYPE_ONLY);



/******************************************************************************
|
| Name:          OnmExtTask
| Parameters:    channel - Handle of OSEK-NM channel.
|                onmStateParam - Internal state of the OSEK-NM
| Return code:   void
| Description:   Processes the Car Wakeup confirmation and calls the appropri-
|                ated CWM functions. This function is called by NmTask().
| Particularity: Must not be called by the application.
|
|*****************************************************************************/

void NM_API_CALL_TYPE OnmExtTask(NM_CHANNEL_NMTYPE_FIRST NmMsgType);



/******************************************************************************
|
| Name:          OnmExtConfirmation
| Parameters:    channel - Handle of OSEK-NM channel.
|                onmStateParam - Internal state of the OSEK-NM
| Return code:   void
| Description:   Stores the Car Wakeup confirmation state. Is called by
|                NmConfirmation().
| Particularity: Must not be called by the application.
|
|*****************************************************************************/

void NM_INTERNAL_CALL_TYPE OnmExtConfirmation(NM_CHANNEL_NMTYPE_FIRST NmMsgType);



/******************************************************************************
|
| Name:          OnmExtPrecopy
| Parameters:    channel - Handle of OSEK-NM channel.
|                onmStateParam - Internal state of the OSEK-NM
|                pRecvMsgUser0 - Address of the first byte of the user data.
| Return code:   void
| Description:   Not used for Volkswagen/Audi. Is called by NmPrecopy().
| Particularity: Must not be called by the application.
|
|*****************************************************************************/

void NM_INTERNAL_CALL_TYPE OnmExtPrecopy(NM_CHANNEL_NMTYPE_FIRST NmMsgType, CanChipDataPtr);



#   endif /* ONMXVAG_HEADER */



/******   STOPSINGLE_OF_MULTIPLE_COMMENT   ******/
/************   Organi, Version 3.4.1 Vector-Informatik GmbH  ************/
