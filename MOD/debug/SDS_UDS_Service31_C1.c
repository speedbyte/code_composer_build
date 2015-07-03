/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	SDS_UDS_Service31_C1.c
 * \brief	This file contains the program to xxxx
 * \brief	This file fulfills the following requirements:
 * 			1. Requirement 1
 * 			2. Requirement 2
 * 			3. Requirement x
 */
/*-----------------------------------------------------------------------------
 * $Date: 2011-09-28 11:10:06 +0200 (Mi, 28 Sep 2011) $
 * $Rev: 21962 $
 * $Author: maennel $
 * $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_SDS_UDS/trunk/src/SDS_UDS_Service31_C1.c $
 *-----------------------------------------------------------------------------
 */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "tms470r1.h"
#include <string.h>
#include <osek.h>           // vector
#include <osekext.h>        // vector
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "sds.h"
#include "uds.h"
#include "tp.h"
#include "deh.h"
#include "VCR.h"
#include "sds_ext.h"
#include "ISRMask_CB_cfg.h"
#include "APP_DIAG.h"
#include "can_def.h"
#include "can_cfg.h"
#include "crcmgr.h"
#include "deh_dtc_cfg.h"
#include "sds_cfg.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "SDS_ext.h"
#include "SDS_UDS_CI.h"
#include "SDS_UDS_Service31_C1.id"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
 */
static BooleanType _SDS_UDS_bStartRoutineControl = BT_FALSE;

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
/* return values of SDSext functions  */
typedef enum
{
    SDS_SERVICE_FINISH_AN_AVAILABLE=0,     /* action e.q. function call okay */
    SDS_SUB_FUNCTION_NOT_SUPPORTED,        /* subfunction not supported */
    SDS_IMPROPER_UPLOAD_TYPE,              /* improper upload type */
    SDS_WRONG_START_ADDRESS,
    SDS_WRONG_NUMBER_OF_BYTES
} tSDSextStatus;

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
 */

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static void _SDS_UDS_vFlashspeicherbereichDeleteStart(void);

static Uint8Type _SDS_UDS_ubFlashspeicherbereichDeleteStatus(void);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
 */

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private variable
 */
static Uint8Type ubFlashspeicherbereichDeleteState = FALSE;

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
 */

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : SDS_UDS_vInitialiseService31                       */
/** 
 \brief      To reset the Static variables

 \brief      Description:
 this function resets the static variables.


 \return     void 
 */
/*----------------------------------------------------------------------------*/
void SDS_UDS_vInitialiseService31(void) 
{
    _SDS_UDS_bStartRoutineControl = BT_FALSE;	
}

/*  FUNCTION Uint8Type _SDS_UDS_vFlashspeicherbereichDeleteStart
 **************************************************************************
 *  PURPOSE:        startet das Löschen vom Flasspeicher
 **************************************************************************
 *  PARAMETERS:     
 *                  
 *                  
 **************************************************************************
 *  RETURN VALUE:   speichert das Ergebnis in ubFlashspeicherbereichDeleteState
 *************************************************************************/
static void _SDS_UDS_vFlashspeicherbereichDeleteStart(void)
{
    ubFlashspeicherbereichDeleteState = TRUE;
}

/*  FUNCTION Uint8Type _SDS_UDS_ubFlashspeicherbereichDeleteStatus
 **************************************************************************
 *  PURPOSE:        gibt den Status des Flashspeicherbereich Löschens zurück
 **************************************************************************
 *  PARAMETERS:     
 *                  
 *                  
 **************************************************************************
 *  RETURN VALUE:   ubFlashspeicherbereichDeleteState: valid or not valid
 *************************************************************************/
static Uint8Type _SDS_UDS_ubFlashspeicherbereichDeleteStatus(void)
{
    return ubFlashspeicherbereichDeleteState;
}


/***************************************************************************
 *  PURPOSE:        UDS_ubDiagService31
 **************************************************************************
 *  PARAMETERS:     see UDS_ubDiagService10
 **************************************************************************
 *  RETURN VALUE:   see UDS_ubDiagService10
 *************************************************************************/
// TODO; Automatic generation of service 31 parameters and copy in
// APP_DIAG_ID_CHART.h and APP_DIAG_PARAMETER.h

#define SERVICE31ROUTINECONTROLTYPE RxTxBuf[0]
#define SERVICE31STARTROUTINE          1
#define SERVICE31STOPROUTINE           2
#define SERVICE31REQUESTROUTINERESULTS 3

#define SERVICE31ROUTINEIDENTIFIER               ((RxTxBuf[1] << 8) + RxTxBuf[2])
#define SERVICE31_02EF_CHECKSUMMENBERECHNUNG           0x02EF
#define SERVICE31_0300_FLASHSPEICHERBEREICHDELETE      0x0300
#define ID_31_0317_RESET_TO_FACTORY_SETTING           0x0317

#define SERVICE31_0202_CHECKMEMORY                     0x0202
#define SERVICE31_0203_CHEPROGRAMMINGPRECONDITIIONS    0x0203

#define SERVICE31ROUTINECONTROLOPTION1             RxTxBuf[3]
#define SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY 3
#define SERVICE31BASICSETTINGENTRY                 4

#define SERVICE31ROUTINECONTROLOPTION2_3             ((RxTxBuf[4] << 8) + RxTxBuf[5])
#define SERVICE31BASICSETTINGCONTROLBEGIN            0x0000
#define SERVICE31BASICSETTINGCONTROLEND              0x00FF
#define SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL  0x0100
#define SERVICE31VWCALIBRATIONAPPLICATIONDATAHISTORY 0x0FFF
#define SERVICE31VWCALIBRATIONAPPLICATIONDATAINFO    0x1000

Uint8Type UDS_ubDiagService31(Uint8Type* RxTxBuf, Uint16Type* LenOfMsg,
        Uint8Type Mode)
{

    Mode = Mode;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_TRACE, UHV_SDS_UDS,
            "Service31:ControlType = %02X, RoutineIdentifier = %02X%02X, ControlOption1 = %02X, ControlOption2+3 = %04X",
            RxTxBuf[0], RxTxBuf[1], RxTxBuf[2],SERVICE31ROUTINECONTROLOPTION1,SERVICE31ROUTINECONTROLOPTION2_3);
#endif

    switch (SERVICE31ROUTINECONTROLTYPE)
    {
        case SERVICE31STARTROUTINE:
        {
            switch (SERVICE31ROUTINEIDENTIFIER)
            {
                case SERVICE31_0202_CHECKMEMORY:
                {
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x02;
                    break;
                }

                case SERVICE31_0203_CHEPROGRAMMINGPRECONDITIIONS:
                {
                    if (LenOfMsg[0] != 3)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x03;
                    break;
                }

                case SERVICE31_0300_FLASHSPEICHERBEREICHDELETE:
                {
                    if (((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE) &&
                            (SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL))
                    {
                        return UDScub_ACCESS_DENIED;
                    }
                    if (LenOfMsg[0] != 6)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }

                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            ((SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAHISTORY) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAINFO)))
                    {
                        ; // alles okay
                    }
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    _SDS_UDS_vFlashspeicherbereichDeleteStart();
                    _SDS_UDS_bStartRoutineControl = BT_TRUE;
                    *LenOfMsg = 3; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }

                case SERVICE31_02EF_CHECKSUMMENBERECHNUNG:
                {
                    if (LenOfMsg[0] != 6)
                        return UDScub_INCORRECT_MSG_LEN;

                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            ((SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAHISTORY) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAINFO)))
                        ; // alles okay
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    SDS_UDS_vChecksumStart();
                    _SDS_UDS_bStartRoutineControl = BT_TRUE;
                    *LenOfMsg = 3; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }

                case ID_31_0317_RESET_TO_FACTORY_SETTING:
                {
                    if (LenOfMsg[0] != 6)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }

                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31BASICSETTINGENTRY) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 >= SERVICE31BASICSETTINGCONTROLBEGIN) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 <= SERVICE31BASICSETTINGCONTROLEND))
                    {
                        ; // alles okay
                    }
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    if ( _SDS_UDS_u8GetStatusBasicSettings() != BASIC_SETTINGS_NOT_RUNNING )
                    {
                        *LenOfMsg = 2;
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }
                    _SDS_UDS_bStartRoutineControl = BT_TRUE;
                    ub_RequestPending = 0;
                    _SDS_UDS_vResetOfAdaptionValues(BT_TRUE, BT_TRUE);
                    *LenOfMsg = 3;          // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }

                default:
                {
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"$31: RoutineIdentifier = %04X not allowed",SERVICE31ROUTINEIDENTIFIER);
                    *LenOfMsg = 2;
                    return UDScub_REQUEST_OUT_OF_RANGE;
                }
            }
            break;
        }
        case SERVICE31STOPROUTINE:
        {
            switch (SERVICE31ROUTINEIDENTIFIER)
            {
                case SERVICE31_0202_CHECKMEMORY:
                {
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x02;
                    break;
                }

                case SERVICE31_0203_CHEPROGRAMMINGPRECONDITIIONS:
                {
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x03;
                    break;
                }

                case SERVICE31_0300_FLASHSPEICHERBEREICHDELETE:
                {
                    if (((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE) &&
                            (SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL))
                    {
                        return UDScub_ACCESS_DENIED;
                    }
                    if (LenOfMsg[0] != 6)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }
                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            ((SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAHISTORY) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAINFO)))
                    {
                        ; // alles okay
                    }
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                    {
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }

                    _SDS_UDS_bStartRoutineControl = BT_FALSE;
                    *LenOfMsg = 3; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }
                case SERVICE31_02EF_CHECKSUMMENBERECHNUNG:
                {
                    if (LenOfMsg[0] != 6)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }
                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            ((SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAHISTORY) ||
                                    (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATAINFO)))
                    {
                        ; // alles okay
                    }
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                    {
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }

                    _SDS_UDS_bStartRoutineControl = BT_FALSE;
                    *LenOfMsg = 3; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }

                case ID_31_0317_RESET_TO_FACTORY_SETTING:
                {
                    if (LenOfMsg[0] != 3)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }


                    if ((SERVICE31ROUTINECONTROLOPTION1 == SERVICE31BASICSETTINGENTRY) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 >= SERVICE31BASICSETTINGCONTROLBEGIN) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 <= SERVICE31BASICSETTINGCONTROLEND))
                        ; // alles okay
                    else
                    {
                        *LenOfMsg = 2;
                        return UDScub_REQUEST_OUT_OF_RANGE;
                    }
                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                    {
                        *LenOfMsg = 2;
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }
                    if ( _SDS_UDS_u8GetStatusBasicSettings() == BASIC_SETTINGS_RUNNING )
                    {
                        *LenOfMsg = 2;
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }
                    _SDS_UDS_bStartRoutineControl = BT_FALSE;
                    _SDS_UDS_vResetOfAdaptionValues(BT_FALSE, BT_TRUE);
                    *LenOfMsg = 3; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    break;
                }

                default:
                {
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"$31: RoutineIdentifier = %04X not allowed",SERVICE31ROUTINEIDENTIFIER);
                    *LenOfMsg = 2;
                    return UDScub_REQUEST_OUT_OF_RANGE;
                }
            }
            break;
        }
        case SERVICE31REQUESTROUTINERESULTS:
        {
            switch (SERVICE31ROUTINEIDENTIFIER)
            {
                case SERVICE31_0202_CHECKMEMORY:
                {
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x02;
                    break;
                }

                case SERVICE31_0203_CHEPROGRAMMINGPRECONDITIIONS:
                {
                    *LenOfMsg = 3;
                    RxTxBuf[1] = 0x02;
                    RxTxBuf[2] = 0x03;
                    break;
                }

                case SERVICE31_02EF_CHECKSUMMENBERECHNUNG:
                {
                    if (LenOfMsg[0] != 3)
                        return UDScub_INCORRECT_MSG_LEN;

                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    *LenOfMsg = 6;
                    RxTxBuf[3] = 0x02; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    if (_SDS_UDS_ubChecksumStatus() == TRUE)
                    {
                        RxTxBuf[4] = 0xFF;
                        RxTxBuf[5] = 0xFF;
                    }
                    else
                    {
                        RxTxBuf[4] = 0x00;
                        RxTxBuf[5] = 0x00;
                    }
                    break;
                }

                case SERVICE31_0300_FLASHSPEICHERBEREICHDELETE:
                {
                    if (((ubSecState & MASK_SEC_DD_ACTIVE) != MASK_SEC_DD_ACTIVE) &&
                            (SERVICE31ROUTINECONTROLOPTION1 == SERVICE31VWCALIBRATIONAPPLICATIONDATAENTRY) &&
                            (SERVICE31ROUTINECONTROLOPTION2_3 == SERVICE31VWCALIBRATIONAPPLICATIONDATACONTOL))
                    {
                        return UDScub_ACCESS_DENIED;
                    }
                    if (LenOfMsg[0] != 3)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }

                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                    {
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }
                    *LenOfMsg = 6;
                    RxTxBuf[3] = 0x02; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    if (_SDS_UDS_ubFlashspeicherbereichDeleteStatus() == TRUE)
                    {
                        RxTxBuf[4] = 0xFF;
                        RxTxBuf[5] = 0xFF;
                    }
                    else
                    {
                        RxTxBuf[4] = 0x00;
                        RxTxBuf[5] = 0x00;
                    }
                    break;
                }

                case ID_31_0317_RESET_TO_FACTORY_SETTING:
                {
                    if (LenOfMsg[0] != 3)
                    {
                        return UDScub_INCORRECT_MSG_LEN;
                    }

                    if (_SDS_UDS_bStartRoutineControl == BT_FALSE)
                    {
                        return SDS_UDS_REQUEST_SEQUENCE_ERROR;
                    }
                    *LenOfMsg = 6;
                    RxTxBuf[3] = 0x02; // Hinweis: RxTxBuf[1+2] kann direkt vom Sender (Client) übernommen werden
                    if (_SDS_UDS_bResetOfAdaptionValuesStatus() == BT_TRUE)
                    {
                        RxTxBuf[4] = 0xFF;
                        RxTxBuf[5] = 0xFF;
                    }
                    else
                    {
                        RxTxBuf[4] = 0x00;
                        RxTxBuf[5] = 0x00;
                    }
                    break;
                }

                default:
                {
                    DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"$31: RoutineIdentifier = %04X not allowed",SERVICE31ROUTINEIDENTIFIER);
                    *LenOfMsg = 2;
                    return UDScub_REQUEST_OUT_OF_RANGE;
                }
            }
            break;
        }

        default:
        {
            DEBUG_VALUE1(SDS_UDS_SWI_ERR, MOD_SDS_UDS,"$31: ControlType %02X not allowed",SERVICE31ROUTINECONTROLTYPE);
            *LenOfMsg = 2;
            return UDScub_SUBFUNCTION_NOT_SUPPORTED;
        }
    }
    return UDScub_FINISHED;
}


/* End Of File SDS_UDS_Service31_C1.c */
