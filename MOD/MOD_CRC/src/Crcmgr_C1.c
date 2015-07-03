/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	CRCMGR_C1.c
 * \brief	this module manage Cyclic Redundancy Check for Eeprom CRC32.
 * Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-12-08 09:41:12 +0100 (Mi, 08 Dez 2010) $
* $Rev: 16812 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CRC/trunk/src/Crcmgr_C1.c $
*-----------------------------------------------------------------------------
*/
#include <osek.h>

#include "Common.h"
#include "OSEK_MonitorTask_H1.h"
#include "deh.h"
#include "DEH_DTC_CFG.h"
#include "Crc.h"
#include "Crcmgr.h"
#include "SDS_ext.h"
#include "EEPROM_LAYOUT_CFG.h"

#include "TRACE.h"
#include "Crcmgr_C1.id"
//#include <assert.h>



//DeclareResource(CRC_LockResource);

#if (defined(PROCESSOR_TMS470R1VF55BA) || defined(PROCESSOR_TMS470R1SF55BA))
#define APPLICATION_CODE_START 0x00008024
#define APPLICATION_CODE_END   0x000BFFFB
#define CRC_ADDRESS            0x000BFFFC
#elif (defined(PROCESSOR_TMS470R1VF45AA))
#define APPLICATION_CODE_START 0x00008024
#define APPLICATION_CODE_END   0x0007FFFB
#define CRC_ADDRESS            0x0007FFFC
#endif
static Union32Type applicationCodeCRC;
static Uint32Type  applicationPtr;

#define QUEUE_SIZE 6
#define CRC_LOOKUP_TABLE_LEN 2                             // solange das Schreiben der zur Laufzeit änderbaren EEPROM Daten nicht implementiert ist, 
                                                           // darf keine CRC Überprüfung für diese Bereiche erfolgen!

static FlagType CrcmgrDoCRC = FALSE;
static FlagType EECrcCheckCompleteDone = FALSE;
/* following variable for ring buffer */
static EEAddressType wroteAddresses[QUEUE_SIZE] = {0};
static Uint8Type  writeIndex = 0;
static Uint8Type  readIndex = 0;
/* Reference CRC to check against */
static Uint32Type referenceCRC = 0;
#ifdef CRCROM_CHECK_ON
static void doCRCCheckOnApplication(void);
static _Bool      activeCRCCheck = TRUE;
#endif

static const Uint8Type zeroValue[4]={0,0,0,0};
/* Look up table in order to get a CRC area out of a EEprom area */
static const CRCLookup crcLookUpTable[] = {
 {0x0000,                                                    0x0000,                          0x00},                                                                                          // Dummy
 {VWDB_EE_ADDDRESS_VWSparePartNbr,                           VWDB_EE_ADDDRESS_CRCSgIdent,     VWDB_EE_ADDDRESS_CRCSgIdent}                                                                    // CRC SG Identification
};

                                                                                                        
                                                                                                                                

/*--------------------------------------------------------------------------*/
/* Functions                                                                */
/*--------------------------------------------------------------------------*/
static void CheckEEprom(void);
static Uint8Type CheckForCrcJob(void);
#ifdef CRCROM_CHECK_ON
static void doCRCCheckOnApplication(void);
#endif
static Uint8Type CrcEepromCheck(EEAddressType Dataaddress,
                                Uint16Type Datalength,
                                EEAddressType CrcAddress,
                                Uint16Type CRClength);

/*--------------------------------------------------------------------------*/
/* Globals                                                                  */
/*--------------------------------------------------------------------------*/

void resetCRCValueForAddress(EEAddressType address);

/*--------------------------------------------------------------------------*/
/* NAME:             CRC_sfManagerInit                                         */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION: Init the CRC Module                                         */
/*--------------------------------------------------------------------------*/

SuccessFailType CRC_sfManagerInit(void)
{
    SuccessFailType sF = SUCCESS;
    applicationPtr = APPLICATION_CODE_START;
    applicationCodeCRC.u32 = 0;
    referenceCRC = *(Uint32Type*) (CRC_ADDRESS);
    /*
     *  register a pointer to the function that collects the write operation
     * on the EEprom
     */
    //EERegisterListener(resetCRCValueForAddress);

    DEBUG_VALUE1(CRC_SWI_TRC, MOD_CRC, "ROMCRC=%08X",referenceCRC);
    StartCRCJob();
    return (sF);
}

/*--------------------------------------------------------------------------*/
/* NAME:             resetCRCValueForAddress                                */
/* INPUT PARAMETERS: address                                                */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION: A write action has been performed within the EEProm.        */
/* The CRC has to be reseted                                                */
/*--------------------------------------------------------------------------*/
void resetCRCValueForAddress(EEAddressType address)
{
    //semP(CRC_LockResource);
    /* stores the address in order to initiate a single write */
    wroteAddresses[writeIndex] = address;
    /* increment the ring buffer write index and be sure that we */
    /* reset it at the boundary */
    writeIndex++;
    if (writeIndex == QUEUE_SIZE)
    {
        writeIndex = 0;
    }
    // assert for buffer overflow.
//    assert(writeIndex != readIndex);    
    //semV(CRC_LockResource);
}
/*--------------------------------------------------------------------------*/
/* NAME:             resetCRCValueForAddress                                */
/* INPUT PARAMETERS: address                                                */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION: A write action has been performed within the EEProm. Reset  */
/* accordingly the CRC                                                      */
/*--------------------------------------------------------------------------*/
static EEAddressType* getNextCRCToReset(void)
{
    EEAddressType *retval = NULL;
    //semP(CRC_LockResource);
    if (readIndex != writeIndex)
    {
        retval = &wroteAddresses[readIndex];
        readIndex++;
        if (readIndex == QUEUE_SIZE)
        {
            readIndex = 0;
        }
    }
    //semV(CRC_LockResource);
    return retval;
}
/*--------------------------------------------------------------------------*/
/* NAME:             crcIsAnyJobPending                                     */
/* INPUT PARAMETERS: void                                                   */
/* RETURN VALUES:    FlagType                                               */
/* DESCRIPTION: Check whether some CRC has still to be reseted              */
/*--------------------------------------------------------------------------*/
FlagType CRCIsAnyJobPending(void)
{
    return readIndex != writeIndex;
}
/*--------------------------------------------------------------------------*/
/* NAME:             getCRCAdress                                           */
/* INPUT PARAMETERS: EEAddressType                                          */
/* RETURN VALUES:    EEAddressType                                          */
/* DESCRIPTION: Out of a address within the EEprom determines the adress for*/
/* the CRC information linked to those data                                 */
/*--------------------------------------------------------------------------*/
static EEAddressType* getCRCAdress(EEAddressType data)
{
    Uint16Type i = 0;
    static CRCLookup lookUp = {0};
    
    // search for a free entry with the register
    for (i=0; i <CRC_LOOKUP_TABLE_LEN; i++)
    {
        lookUp = crcLookUpTable[i];
        if (lookUp.data == data)
        {
            return &lookUp.CRC;
        }
    }
    // at that point we handle a write at an adress where there is no
    // CRC associated.
    return NULL;
}
/*--------------------------------------------------------------------------*/
/* NAME:             CrcManager                                             */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION: Manage whole CRC check in the system over state maschine    */
/*              For different types of memories. Currently only Eeprom check*/
/*              is done                                                     */
/*--------------------------------------------------------------------------*/
void CrcManager(void)
{
  EEDeviceControlBlockType    *pEECrc = (EEDeviceControlBlockType *)getEEMonitorControl();

    /* Check for CRC to be reseted first */
    if (EEPROM_flagIsEEFree(pEECrc) == TRUE)
    {
        EEAddressType *nextCRCToReset = getNextCRCToReset();
        // We may post one write request. So let reset the CRC area
        // corresponding to the data that have been written in EEProm
        if (nextCRCToReset != NULL)
        {
            EEAddressType* crcAddress = getCRCAdress(*nextCRCToReset);
            if (crcAddress != NULL)
            {
                CommonResponseCodeType rC = EEPROM_LAYOUT_rcWrite(pEECrc, 
                                           *crcAddress, 
                                           (Uint8Type*)zeroValue, CRC32WORD_LEN, NULL);
            }
        }
    }
    // check the application code in an iterative way
#ifdef CRCROM_CHECK_ON
    if (activeCRCCheck)
    {
    	doCRCCheckOnApplication();
    }
        
#endif    

    if (CheckForCrcJob()== TRUE)
    {
       CheckEEprom();
    }
}

/*--------------------------------------------------------------------------*/
/* NAME:             StartCrcJob                                            */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION:  Set StartCrc condition flag to true if you want to do a    */
/*               CRC check                                                  */
/*--------------------------------------------------------------------------*/
void StartCRCJob(void)
{
    /* Set the CRCdoing flag to true that we want to do a CRC */
    /* it it possible to set this flag cyclically by a timer, event or alarm  */
    /* if it is wanted */
    CrcmgrDoCRC = TRUE;
}

/*--------------------------------------------------------------------------*/
/* NAME:             CompleteEECrcCheckDone                                 */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    TRUE   Complete CRC is finish                          */
/*                   FALSE  Complete CRC is not finish                      */
/* DESCRIPTION:  Check that complete Eeprom CRC check is done               */
/*--------------------------------------------------------------------------*/
Uint8Type CompleteEECrcCheckDone(void)
{
    return (EECrcCheckCompleteDone == TRUE ? TRUE : FALSE);
}

/*--------------------------------------------------------------------------*/
/* NAME:             CheckForCrcJob                                         */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    TRUE   is allowed to do CRC job                        */
/*                   FALSE  is  not allowed to do CRC job                   */
/* DESCRIPTION:  Check if is allowed to run the CRC job                     */
/*               Look also that Power is in normal state                    */
/*--------------------------------------------------------------------------*/
static Uint8Type CheckForCrcJob(void)
{
    Uint8Type FuncRet=FALSE;

    if(CrcmgrDoCRC == TRUE)
    {
        if (SDSext_GetUhvInRestrictedMode()== OFF)         
        {
            FuncRet=TRUE;
        }
        else
        {
            FuncRet=FALSE;
        }
    }
    else
    {
        FuncRet=FALSE;
    }


    return (FuncRet);
}

/*--------------------------------------------------------------------------*/
/* NAME:             CheckEEprom                                            */
/* INPUT PARAMETERS: none                                                   */
/* RETURN VALUES:    none                                                   */
/* DESCRIPTION: scans all the CRC entry to check and check their CRC        */
/*--------------------------------------------------------------------------*/
static void CheckEEprom(void)
{
    static Uint8Type CRCLookUpEntryCounter = 1;
    CRCLookup crcLookUpEntry = crcLookUpTable[CRCLookUpEntryCounter];
    if (CRCLookUpEntryCounter == 0)
    {
        EECrcCheckCompleteDone = FALSE;
    }
    else if (CRCLookUpEntryCounter < CRC_LOOKUP_TABLE_LEN) 
    {
        if (DONE == CrcEepromCheck(crcLookUpEntry.data,
                       crcLookUpEntry.datalength,
                       crcLookUpEntry.CRC,
                       CRC32WORD_LEN) )
        {
            CRCLookUpEntryCounter++;
        }
    }
    else
    {
        EECrcCheckCompleteDone = TRUE;
    }
}

/*--------------------------------------------------------------------------*/
/* NAME:             CrcEepromCheck                                         */
/* INPUT PARAMETERS: Dataaddress: Eepromstartaddress data which is to check */
/*                   Datalength: Eepromdatalengthaddress which is to check  */
/*                   CrcAddress: EepromdCRCInfostartaddress for check       */
/*                   CRClength:  EepromCRCInfolengthstartaddress for check  */
/* RETURN VALUES:    CONTINUE: CRCcheck need additional cycle for check     */
/*                   DONE: CRCcheck is finish                               */
/* DESCRIPTION:  Check data from Eeprom against the save CRC value          */
/*               If CRC value is zero calculate first CRC and then the next */
/*--------------------------------------------------------------------------*/
static Uint8Type CrcEepromCheck(EEAddressType Dataaddress,
                                Uint16Type Datalength,
                                EEAddressType CrcAddress,
                                Uint16Type CRClength)
{
    Uint8Type rCRC = CONTINUE;
    CommonResponseCodeType rC = COMMON_RSP_OK;
    EEDeviceControlBlockType    *pEECrc = (EEDeviceControlBlockType *)getEEMonitorControl();
    CommonResponseCodeType retVal_E2P = COMMON_RSP_OK;

    static Union32Type CalculateCRC;
    static Union32Type EEpromCRC;
    static Uint8Type DataBuffer[150];
    static CrcmgrBackupStatesType BackupDataModeCRC = CRC_BACKUP_NORMALMODE_NO_CRC_ERROR;

    switch (BackupDataModeCRC)
    {
    case CRC_BACKUP_NORMALMODE_NO_CRC_ERROR:
        /* First read CRC from EEprom to check if data was change or for first initalisation */
        /* In both case (data change or initalisation) CRC was set to zero                   */
        /* Now check if CRC is zero or not                                                   */
        retVal_E2P = EEPROM_sfRead(pEECrc, CrcAddress,EEpromCRC.u8,CRClength);
        if (retVal_E2P == COMMON_RSP_OK)
        {
            if (EEpromCRC.u32 == CRCINITVALUE)
            {
                /* Read Eeprom data */
                retVal_E2P = EEPROM_sfRead(pEECrc, Dataaddress,DataBuffer,Datalength);
                if (retVal_E2P == COMMON_RSP_OK)
                {
                    /* Calculate the CRC for the new data */
                    Crc32Reset(&CalculateCRC.u32);
                    CalculateCRC.u32 = Crc32Update(DataBuffer,Datalength,CalculateCRC.u32);
                    /* Write new CRC data to EEprom */
                    rC = EEPROM_LAYOUT_rcWrite(pEECrc, CrcAddress, CalculateCRC.u8,CRClength,NULL);
                }
                else
                {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                    (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
                }
            }
            else
            {
                /* Read Eeprom data */
                retVal_E2P = EEPROM_sfRead(pEECrc, Dataaddress,DataBuffer,Datalength);
                if (retVal_E2P == COMMON_RSP_OK)
                {
                    /* Calculate the CRC by seeding the CRC  */
                    Crc32Reset(&CalculateCRC.u32);
                    CalculateCRC.u32 = Crc32Update(DataBuffer,Datalength,CalculateCRC.u32);
                    /* Read the stored CRC  */
                    retVal_E2P = EEPROM_sfRead(pEECrc, CrcAddress,EEpromCRC.u8,CRClength);
                    if (retVal_E2P == COMMON_RSP_OK)
                    {
                        /* If the calculated and stored CRC not match try to backup data */
                        if (CalculateCRC.u32 != EEpromCRC.u32)
                        {
                            /* first try to recover old save value from the mirrow image backup */
                            BackupDataModeCRC = CRC_BACKUP_RESET_CRC;
                        }
                        else
                        {
                            /* All is correct check next */
                            rCRC = DONE;
                            BackupDataModeCRC = CRC_BACKUP_NORMALMODE_NO_CRC_ERROR;
                        }
                    }
                    else
                    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                        (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
                    }
                }
                else
                {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                    (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
                }
            } /* end if EEpromCRC == 0 */
        } /* end if check EEpromCRC */
        else
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
        }
        break;
    case CRC_BACKUP_RESET_CRC:
        /* First Reset CRC of EEprom */
        EEpromCRC.u32 = CRCINITVALUE;
        rC = EEPROM_LAYOUT_rcWrite(pEECrc, CrcAddress, EEpromCRC.u8, CRClength, NULL);
        if (COMMON_RSP_OK == rC)
        {
            BackupDataModeCRC = CRC_BACKUP_READ_BACKUPDATA;
        }
        break;
    case CRC_BACKUP_READ_BACKUPDATA:
        /* Read Backup Eeprom data */
        retVal_E2P = EEPROM_sfRead(pEECrc, Dataaddress+D_EE_BACKUP_DELTA ,DataBuffer,Datalength);
        if (retVal_E2P == COMMON_RSP_OK)
        {
            BackupDataModeCRC = CRC_BACKUP_WRITE_BACKUPDATA;
        }
        else
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
        }
        break;
    case CRC_BACKUP_WRITE_BACKUPDATA:
        rC = EEPROM_LAYOUT_rcWrite(pEECrc, Dataaddress, DataBuffer, Datalength, NULL);
        if (COMMON_RSP_OK == rC)
        {
            BackupDataModeCRC = CRC_BACKUP_CALULATE_NEW_CRC;
        }
        break;
    case CRC_BACKUP_CALULATE_NEW_CRC:
        /* Read Backup Eeprom data from orginal place and build new CRC */
        retVal_E2P = EEPROM_sfRead(pEECrc, Dataaddress,DataBuffer,Datalength);
        if (retVal_E2P == COMMON_RSP_OK)
        {
            /* Calculate the CRC for the new data */
            Crc32Reset(&CalculateCRC.u32);
            CalculateCRC.u32 = Crc32Update(DataBuffer,Datalength,CalculateCRC.u32);
            /* Write new CRC data to EEprom */
            rC = EEPROM_LAYOUT_rcWrite(pEECrc, CrcAddress, CalculateCRC.u8,CRClength,NULL);
            if (COMMON_RSP_OK == rC)
            {
                BackupDataModeCRC = CRC_BACKUP_CHECK_RECOVER_DATA_AGAINST_NEW_CRC;
            }
        }
        else
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
        }
        break;
    case CRC_BACKUP_CHECK_RECOVER_DATA_AGAINST_NEW_CRC:
        retVal_E2P = EEPROM_sfRead(pEECrc, Dataaddress,DataBuffer,Datalength);
        if (retVal_E2P == COMMON_RSP_OK)
        {
            /* Calculate the CRC by seeding the CRC  */
            Crc32Reset(&CalculateCRC.u32);
            CalculateCRC.u32 = Crc32Update(DataBuffer,Datalength,CalculateCRC.u32);
            /* Read the stored CRC  */
            retVal_E2P = EEPROM_sfRead(pEECrc, CrcAddress,EEpromCRC.u8,CRClength);
            if (retVal_E2P == COMMON_RSP_OK)
            {
                rCRC = DONE;
                BackupDataModeCRC = CRC_BACKUP_NORMALMODE_NO_CRC_ERROR;
                /* If the calculated and stored CRC not match generate a DTC  */
                /* Backup recover data failed                                 */
                if (CalculateCRC.u32 != EEpromCRC.u32)
                {
                  semP(DEH_LockResource);
                  (void)DEH_AdditionalReportEvent(DTC_A00046_STEUERGERAETDEFEKT_EEPROMFEHLER, TRUE);
                  semV(DEH_LockResource);
                }
// This code will be commented out, because it is not allowed the to set the state "sporadisch" for this DTC type, see ticket number 1417
/*              
                else
                {
                  semP(DEH_LockResource);
                  (void)DEH_AdditionalReportEvent(DTC_A00046_STEUERGERAETDEFEKT_EEPROMFEHLER, FALSE);
                  semV(DEH_LockResource);
                }*/
           }
           else
           {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
               (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
           }
        }
        else
        {  
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR, UHV_CRC, "E2P Read FAIL: %d, %s", __LINE__, __FILE__);
#endif
        }    
        break;
    } /* end switch BackupDataModeCRC */

    return (rCRC);
}

/**--------------------------------------------------------------------------*
* Function:       doCRCCheckOnApplication                                    *
* Description:    Make an incremental CRC check on the application code      *
* Parameters:                                                                *
* Returns:        void                                                       *
*----------------------------------------------------------------------------*/
#ifdef CRCROM_CHECK_ON
static void doCRCCheckOnApplication(void)
{
    /* do the following 10 times per cycle to reduce the elapsed time to get
     * the CRC 
     */
    Uint8Type i;

    for (i=0; i<10; i++)
    {
        if (applicationPtr+4 <= APPLICATION_CODE_END)
        {
            Union32Type temp;
            temp.u32 = *(Uint32Type*)(applicationPtr);
            applicationCodeCRC.u32 = Crc32Update(temp.u8, 4, applicationCodeCRC.u32);
            applicationPtr+=4;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3,UHV_CRC,"calcCRC=%08X,appptr=%08X",applicationCodeCRC.u32,applicationPtr);
#endif
        }
        else if (applicationPtr < APPLICATION_CODE_END)
        {
            // we have less that 4 bytes to handle
            Union32Type temp;
            Uint8Type nbOfBytes = (Uint8Type)(APPLICATION_CODE_END - (applicationPtr) );
            temp.u32 = *(Uint32Type*)(applicationPtr);
            applicationCodeCRC.u32 = Crc32Update(temp.u8, nbOfBytes, applicationCodeCRC.u32);
            applicationPtr +=nbOfBytes;  
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG_3,UHV_CRC,"LAST:calcCRC=%08X,appptr=%08X",applicationCodeCRC.u32,applicationPtr);
#endif
        }
        else
        {
            activeCRCCheck = FALSE;          
                  
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_TRACE,UHV_CRC,"FINISH:calcCRC=%08X,ROMCRC=%08X",applicationCodeCRC.u32,referenceCRC);
#endif
            if (applicationCodeCRC.u32 != referenceCRC)
            {
          	  /* no valid Application SW */
              semP(DEH_LockResource);
              (void)DEH_AdditionalReportEvent(DTC_A00045_STEUERGERAETDEFEKT_PROGRAMMSPEICHERFEHLER, TRUE);
              semV(DEH_LockResource);
#ifndef USE_DEBUGGING_WITH_TRACE_ID
              (void)DEBUG_Printf(UHV_ERROR,UHV_CRC,"checksum not okay !!!");
#endif
            }
// This code will be commented out, because it is not allowed the to set the state "sporadisch" for this DTC type, see ticket number 1417
/*              
            else
            {
              semP(DEH_LockResource);
              (void)DEH_AdditionalReportEvent(DTC_A00045_STEUERGERAETDEFEKT_PROGRAMMSPEICHERFEHLER, FALSE);
              semV(DEH_LockResource);
                }*/
            break;
        }
    }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
    (void)DEBUG_Printf(UHV_DEBUG_2,UHV_CRC,"calcCRC=%08X,appptr=%08X",applicationCodeCRC.u32,applicationPtr);
#endif
}
#endif
