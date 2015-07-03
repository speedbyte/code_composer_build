/******   STARTSINGLE_OF_MULTIPLE_COMMENT   ******/


/******************************************************************************
| Project Name: OSEK Network Management Extension
|    File Name: ONMXVAG.C
|
|  Description: Source of the OSEK Network Management Extension for Volkswagen.
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
| 2004-01-20 1.01.00 Ths    ESCAN00012016: Include of cwm.h corrected.
|                           ESCAN00012017: Support for indexed OSEK-NM added.
|                           ESCAN00012018: Dummy statements for unused function
|                            parameters added to avoid compiler warnings.
| 2005-09-20 1.02.00 Ths    ESCAN00013702: New call-back functions added.
|*****************************************************************************/



# define ONMXVAG_SOURCE



/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

# include "can_inc.h"
# include "n_vnmdef.h"
#   if defined (NM_ENABLE_CAR_WUP_REQUEST)
# include "cwm.h"
#   endif
# include "onmxvag.h"



/*****************************************************************************/
/* Version check                                                             */
/*****************************************************************************/

#   if (NM_DIROSEKEXT_VAG_VERSION != 0x0102u)
# error "Source and Header file are inconsistent!"
#   endif

#   if (NM_DIROSEKEXT_VAG_RELEASE_VERSION != 0x00u)
# error "Source and Header file are inconsistent!"
#   endif



/*****************************************************************************/
/* Plausibility checks                                                       */
/*****************************************************************************/

#   if defined (NM_TYPE_VAG_OSEK)
#   endif

#   if defined (NM_ENABLE_ONM_EXT_API)
#   else
# error "NM_ENABLE_ONM_EXT_API must be defined in nm_cfg.h"
#   endif

#   if( NM_DIROSEK_VERSION < 0x0364u)
# error "NM version must be greater than 3.64"
#   endif



/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/

# if defined ( NM_ENABLE_CAR_WUP_REQUEST )
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
# define ONM_CAR_WUP_COMM_STATE         onmCarWupCommState[NM_CHANNEL_APPLPARA_ONLY]
# define ONM_LAST_CAR_WUP_COMM_STATE    onmLastCarWupCommState[NM_CHANNEL_APPLPARA_ONLY]
#   else
# define ONM_CAR_WUP_COMM_STATE         onmCarWupCommState
# define ONM_LAST_CAR_WUP_COMM_STATE    onmLastCarWupCommState
#   endif
# endif



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
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kOnmxMainVersion    = (vuint8)(NM_DIROSEKEXT_VAG_VERSION >> 8);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kOnmxSubVersion     = (vuint8)(NM_DIROSEKEXT_VAG_VERSION & 0x00ffu);
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kOnmxReleaseVersion = (vuint8)(NM_DIROSEKEXT_VAG_RELEASE_VERSION);

#   if defined (_NEAR_) && defined (C_COMP_IAR_M16C)
/* const data to const segment, don´t initialize */
# pragma memory=default
#   endif



/*****************************************************************************/
/* Global Data Definitions                                                   */
/*****************************************************************************/



/*****************************************************************************/
/* Local Data Definitions                                                    */
/*****************************************************************************/

# if defined ( NM_ENABLE_CAR_WUP_REQUEST )
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 onmCarWupCommState[kNmNumberOfChannels];
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 onmLastCarWupCommState[kNmNumberOfChannels];
#   else
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 onmCarWupCommState;
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 onmLastCarWupCommState;
#   endif
# endif



/*****************************************************************************/
/* Local Function Prototypes                                                 */
/*****************************************************************************/



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

void NM_API_CALL_TYPE OnmExtInit(NM_CHANNEL_NMTYPE_ONLY) {

# if defined ( NM_ENABLE_CAR_WUP_REQUEST )
  ONM_CAR_WUP_COMM_STATE = (vuint8)0u;
  ONM_LAST_CAR_WUP_COMM_STATE = ONM_CAR_WUP_COMM_STATE;
# endif

# if defined ( V_ENABLE_USE_DUMMY_STATEMENT ) 
  /* avoid compiler warning due to unused parameters */
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  NM_CHANNEL_NMPARA_ONLY = NM_CHANNEL_NMPARA_ONLY;
#   endif
# endif  

}



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

void NM_API_CALL_TYPE OnmExtTask(NM_CHANNEL_NMTYPE_FIRST NmMsgType onmStateParam) {

# if defined ( NM_ENABLE_CAR_WUP_REQUEST )
  if (ONM_CAR_WUP_COMM_STATE != ONM_LAST_CAR_WUP_COMM_STATE) {

    ONM_LAST_CAR_WUP_COMM_STATE = ONM_CAR_WUP_COMM_STATE;

    if (ONM_CAR_WUP_COMM_STATE == (vuint8)1u) {
      ApplNmCarWupRequestConfirmation(NM_CHANNEL_NMPARA_ONLY);
    }
    else {
      ApplNmCarWupReleaseConfirmation(NM_CHANNEL_NMPARA_ONLY);
    }
  }
# endif

  ApplNmWriteUserData(NM_CHANNEL_APPLPARA_FIRST NmGetUdStartAddress(NM_CHANNEL_NMPARA_ONLY));
  
# if defined ( V_ENABLE_USE_DUMMY_STATEMENT ) 
  /* avoid compiler warning due to unused parameters */
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  NM_CHANNEL_NMPARA_ONLY = NM_CHANNEL_NMPARA_ONLY;
#   endif
  onmStateParam = onmStateParam;
# endif  

}



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

void NM_INTERNAL_CALL_TYPE OnmExtConfirmation(NM_CHANNEL_NMTYPE_FIRST NmMsgType onmStateParam) {

# if defined ( NM_ENABLE_CAR_WUP_REQUEST )
  if ((onmStateParam & NM_MSG_CAR_WUP) == NM_MSG_CAR_WUP) {
    ONM_CAR_WUP_COMM_STATE = (vuint8)1;
  }
  else {
    ONM_CAR_WUP_COMM_STATE = (vuint8)0;
  }
# endif  

# if defined ( NM_ENABLE_SLEEPACK_CALLBACK )
  if ((onmStateParam & NM_MSG_SLEEP_ACK) == NM_MSG_SLEEP_ACK) {
    ApplNmSleepAck(NM_CHANNEL_NMPARA_ONLY);
  }
# endif  

# if defined ( V_ENABLE_USE_DUMMY_STATEMENT ) 
  /* avoid compiler warning due to unused parameters */
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  NM_CHANNEL_NMPARA_ONLY = NM_CHANNEL_NMPARA_ONLY;
#   endif
  onmStateParam = onmStateParam;
# endif  

}



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

void NM_INTERNAL_CALL_TYPE OnmExtPrecopy(NM_CHANNEL_NMTYPE_FIRST NmMsgType onmStateParam, CanChipDataPtr pRecvMsgUser0) {

# if defined ( NM_ENABLE_READ_USER_DATA )
  NmNodeType source;
  
  (void)NmReadRecvSourceAddress(NM_CHANNEL_NMPARA_FIRST &source);
  ApplNmReadUserData(NM_CHANNEL_NMPARA_FIRST source, pRecvMsgUser0);
# endif

# if defined ( NM_ENABLE_SLEEPACK_CALLBACK )
  if ((onmStateParam & NM_MSG_SLEEP_ACK) == NM_MSG_SLEEP_ACK) {
    ApplNmSleepAck(NM_CHANNEL_NMPARA_ONLY);
  }
# endif  

# if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
#   if defined ( NM_ENABLE_INDEXED_NM ) && defined ( C_MULTIPLE_RECEIVE_CHANNEL )
  NM_CHANNEL_NMPARA_ONLY = NM_CHANNEL_NMPARA_ONLY;
#   endif
  onmStateParam = onmStateParam;
  pRecvMsgUser0 = pRecvMsgUser0;
# endif  

}



/******   STOPSINGLE_OF_MULTIPLE_COMMENT   ******/
/************   Organi, Version 3.4.1 Vector-Informatik GmbH  ************/
