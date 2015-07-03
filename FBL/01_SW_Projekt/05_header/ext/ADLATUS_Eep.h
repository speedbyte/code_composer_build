/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_Eep.h                                                    |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the eeprom handling.                                             |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   SN         Sven Neuer                SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Eep.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Eep.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/


/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __EEP_
#define __EEP_
/* ========================================================================== */

/* Bitmasks for attribute/status */
/* Mask for number of copies */
#define EEP_NUM_OF_COPIES_MASK 0x0F
/* Checksum bit; if set data block is saved with checksum */
#define EEP_CHECKSUM_MASK      0x10
/* Read command bit; set: read command pending; reset: read command finished */
#define EEP_READ_CMD_PENDING   0x20
/* Write command bit; set: write command pending; reset: write command finished */
#define EEP_WRITE_CMD_PENDING  0x40
/* Error flag bit; is reset with every open/close/read/write/forcedwrite command;
   possible errors when set:
     EepOpen:    either handle buffer full or data block address in EEPROM or block size
                 or number of copies out of range.
     EepClose:   handle can not be closed because either handle no. is invalid or read
                 or write command is pending.
     EepRead:    handle number was out of range or handle was not opened
     EepWrite:   handle number was out of range or handle was not opened
     EepForcedWrite: handle number was out of range or handle was not opened
     EepStatus:  last read command failed because of crc error in all copies. */
#define EEP_ERR_FLAG           0x80
/* defines for parameter checksum */
#define WITHOUT_CHECKSUM (ULONG)FALSE

#define EEPROM_COM_WRITE       (T_U8) 0x02u
#define EEPROM_COM_READ        (T_U8) 0x03u
#define EEPROM_COM_STATUS_READ (T_U8) 0x05u
#define EEPROM_COM_WREN        (T_U8) 0x06u /* Write enable command */

/* EEPROM status register masks */
#define EEPROM_STATUS_WIP      (T_U8) 0x01u
/* for eeproms with A8 in the command byte set this to 2 */
/* if you have a 16 bit address set the offset to 3 */
#define EEPROM_DATA_OFFSET     (UBYTE) 0x03u

#define WR_PAGESIZE (UWORD) 0x0020

/***********************************************************/
UBYTE FUN_BioEepBusy(void);

void FUN_BioEepInit(void);

/***********************************************************

FUNCTION NAME     : FUN_BioEepRead
INPUT PARAMETERS  : EepBuffer:    destination address
                    EepAddress:  source address (in EEPROM)
                    DataLength:   number of bytes to read (max. 61)
OUTPUT PARAMETERS : number of bytes read
DESCRIPTION       : This function reads a data block from the EEPROM to *EepBuffer.
                    The number of bytes which can be read within one read cycle is limited;
                    This routine executes only one read cycle, so number_of_bytes_read can be
                    less than DataLength. The calling routine has to check if all bytes were read.
                    If not, it has to call BioEepRead() again with adjusted parameters.
                    !!!EEPROM must be ready; should have be checked by calling BioEepBusy() !!!

***********************************************************/
UBYTE FUN_BioEepRead  (UBYTE *EepBuffer, UWORD EepAddress, UBYTE DataLength );


/***********************************************************

FUNCTION NAME     : FUN_BioEepWrite
INPUT PARAMETERS  : EepBuffer:    source address (in RAM)
                    EepAddress:  destination address (in EEPROM)
                    DataLength:   number of bytes to write
OUTPUT PARAMETERS : number of bytes written
DESCRIPTION       : This function writes a data block beginning at *EepBuffer to the EEPROM.
                    The number of bytes which can be written within one write cycle is limited;
                    This routine executes only one write cycle, so number_of_bytes_written can be
                    less than DataLength. The calling routine has to check if all bytes are written.
                    If not, it has to call BioEepWrite() again with adjusted parameters.
                    !!!EEPROM must be ready; should have be checked by calling BioEepBusy() !!!

***********************************************************/
UBYTE FUN_BioEepWrite (UBYTE *EepBuffer, UWORD EepAddress, UBYTE DataLength );

#endif

