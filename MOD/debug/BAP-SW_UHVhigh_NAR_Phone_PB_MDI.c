/******************************************************************
 *
 *     Copyright (c) 2003, 2004, 2005 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP
 * Komponente: Konfiguration
 *
 *
 ******************************************************************
 *
 * Beschreibung
 *
 * Generierte Konfigurationsdatei fuer den BAP Protokoll-Stack.
 * Diese Datei darf nicht manuell veraendert werden.
 *
 ******************************************************************
 * Generiert am:    09.11.2010
 *           um:    16:22:18
 * Quelldatei:      C:\Programme\Eclipse-CPP\eclipse\Workspace\UHV-NAR\MOD\MOD_BAP\xml\BAP-SW_UHVhigh_NAR_Phone_PB_MDI.xml
 * Zieldatei:       C:\Programme\Eclipse-CPP\eclipse\Workspace\UHV-NAR\MOD\MOD_BAP\xml\BAP-SW_UHVhigh_NAR_Phone_PB_MDI.c
 *
 * BAPgen Version:  1.4.0
 * BAPgen Stand:    09.06.2006
 ******************************************************************/

/* Systemincludes mit <...> */

/*lint -e651 -e64 -e740 -e826 -e754 -e708 -e545 -e961 */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_privatetypes.h"
#include "bap_balconfig.h"
#include "bap_bplconfig.h"
#include "bap_bclconfig.h"
#include "bap_config.h"
#include "bap.h"

/* Bibliotheken von externen Herstellen mit <...> */
#ifdef BAP_USES_CAN
#include "can_inc.h"
#include "UHV_NAR.h"
#endif

#ifndef BAP_ROM_CONST
#define BAP_ROM_CONST
#endif

#ifndef BAP_RAM_DATA_FAR
#define BAP_RAM_DATA_FAR
#endif

#ifndef BAP_ROM_DATA_FAR
#define BAP_ROM_DATA_FAR
#endif
#if BAP_VERSION != 1400
#error Datei ist nicht kompatibel mit der Version des BAP-Stacks. Bitte diese Datei noch einmal mit dem mitgeliefertem BAPgen aus der XML-Datei generieren.
#endif
extern BAP_ROM_DATA_FAR BapFctRomRow_ot BAP_ROM_CONST BAP_FctRomTables[];

/* BAP LsgRamRow Tabelle *****************************************/
BAP_RAM_DATA_FAR BapLsgRamRow_ot BAP_LsgRamTable[]=
{
    {BapLayerLsgStat_NoInit, 0, {0}, {0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}},
    {BapLayerLsgStat_NoInit, 0, {0}, {0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}},
    {BapLayerLsgStat_NoInit, 0, {0}, {0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}},
    {BapLayerLsgStat_NoInit, 0, {0}, {0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}}
};
/*------- BAL ------*/
/*------- BPL ------*/

/* BPL RAM Tabelle *************************************/
BAP_RAM_DATA_FAR
BapBplRamRow_ot BAP_BplRamTable[]={
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}},
    {0, {0,0,0}}
};
/* BPL HeartbeatTimer ********************************************************************/
BAP_RAM_DATA_FAR
uint16_t BAP_BplHeartbeatTimerTable[]={
    0,
    0,
    0,
    0
};
/*------- BCL ------*/
/* Buffer for Function  lsg.id=40 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer1;
/* Buffer for Function  lsg.id=40 fct.id=27*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer2;
/* Buffer for Function  lsg.id=40 fct.id=28*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[190];
    } oBufferWithLength;
} BCL_RxBuffer3;
/* Buffer for Function  lsg.id=40 fct.id=36*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[40];
    } oBufferWithLength;
} BCL_RxBuffer4;
/* Buffer for Function  lsg.id=41 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_RxBuffer5;
/* Buffer for Function  lsg.id=41 fct.id=18*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer6;
/* Buffer for Function  lsg.id=41 fct.id=19*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer7;
/* Buffer for Function  lsg.id=41 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer8;
/* Buffer for Function  lsg.id=41 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[5];
    } oBufferWithLength;
} BCL_RxBuffer9;
/* Buffer for Function  lsg.id=41 fct.id=24*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[51];
    } oBufferWithLength;
} BCL_RxBuffer10;
/* Buffer for Function  lsg.id=41 fct.id=25*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[40];
    } oBufferWithLength;
} BCL_RxBuffer11;
/* Buffer for Function  lsg.id=41 fct.id=27*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_RxBuffer12;
/* Buffer for Function  lsg.id=41 fct.id=29*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_RxBuffer13;
/* Buffer for Function  lsg.id=41 fct.id=32*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[841];
    } oBufferWithLength;
} BCL_RxBuffer14;
/* Buffer for Function  lsg.id=43 fct.id=16*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_RxBuffer15;
/* Buffer for Function  lsg.id=43 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_RxBuffer16;
/* Buffer for Function  lsg.id=43 fct.id=18*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_RxBuffer17;
/* Buffer for Function  lsg.id=43 fct.id=19*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_RxBuffer18;
/* Buffer for Function  lsg.id=43 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_RxBuffer19;
/* Buffer for Function  lsg.id=48 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_RxBuffer20;
/* Buffer for Function  lsg.id=48 fct.id=25*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_RxBuffer21;
/* Buffer for Function  lsg.id=48 fct.id=26*/
BAP_RAM_DATA_FAR
struct
{
    BapBclRxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_RxBuffer22;
/* Buffer for Function  lsg.id=40 fct.id=1*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[222];
    } oBufferWithLength;
} BCL_TxBuffer1;
/* Buffer for Function  lsg.id=40 fct.id=2*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_TxBuffer2;
/* Buffer for Function  lsg.id=40 fct.id=3*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer3;
/* Buffer for Function  lsg.id=40 fct.id=4*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer4;
/* Buffer for Function  lsg.id=40 fct.id=14*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer5;
/* Buffer for Function  lsg.id=40 fct.id=15*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer6;
/* Buffer for Function  lsg.id=40 fct.id=16*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer7;
/* Buffer for Function  lsg.id=40 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[525];
    } oBufferWithLength;
} BCL_TxBuffer8;
/* Buffer for Function  lsg.id=40 fct.id=18*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[130];
    } oBufferWithLength;
} BCL_TxBuffer9;
/* Buffer for Function  lsg.id=40 fct.id=19*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer10;
/* Buffer for Function  lsg.id=40 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer11;
/* Buffer for Function  lsg.id=40 fct.id=22*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[40];
    } oBufferWithLength;
} BCL_TxBuffer12;
/* Buffer for Function  lsg.id=40 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer13;
/* Buffer for Function  lsg.id=40 fct.id=25*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer14;
/* Buffer for Function  lsg.id=40 fct.id=26*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer15;
/* Buffer for Function  lsg.id=40 fct.id=27*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[985];
    } oBufferWithLength;
} BCL_TxBuffer16;
/* Buffer for Function  lsg.id=40 fct.id=28*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer17;
/* Buffer for Function  lsg.id=40 fct.id=29*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer18;
/* Buffer for Function  lsg.id=40 fct.id=30*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer19;
/* Buffer for Function  lsg.id=40 fct.id=31*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer20;
/* Buffer for Function  lsg.id=40 fct.id=32*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer21;
/* Buffer for Function  lsg.id=40 fct.id=33*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer22;
/* Buffer for Function  lsg.id=40 fct.id=34*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer23;
/* Buffer for Function  lsg.id=40 fct.id=36*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer24;
/* Buffer for Function  lsg.id=40 fct.id=38*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer25;
/* Buffer for Function  lsg.id=40 fct.id=39*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer26;
/* Buffer for Function  lsg.id=40 fct.id=40*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer27;
/* Buffer for Function  lsg.id=40 fct.id=43*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer28;
/* Buffer for Function  lsg.id=40 fct.id=48*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer29;
/* Buffer for Function  lsg.id=41 fct.id=1*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[80];
    } oBufferWithLength;
} BCL_TxBuffer30;
/* Buffer for Function  lsg.id=41 fct.id=2*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_TxBuffer31;
/* Buffer for Function  lsg.id=41 fct.id=3*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer32;
/* Buffer for Function  lsg.id=41 fct.id=4*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer33;
/* Buffer for Function  lsg.id=41 fct.id=14*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer34;
/* Buffer for Function  lsg.id=41 fct.id=15*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer35;
/* Buffer for Function  lsg.id=41 fct.id=16*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer36;
/* Buffer for Function  lsg.id=41 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer37;
/* Buffer for Function  lsg.id=41 fct.id=18*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[738];
    } oBufferWithLength;
} BCL_TxBuffer38;
/* Buffer for Function  lsg.id=41 fct.id=19*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[738];
    } oBufferWithLength;
} BCL_TxBuffer39;
/* Buffer for Function  lsg.id=41 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[738];
    } oBufferWithLength;
} BCL_TxBuffer40;
/* Buffer for Function  lsg.id=41 fct.id=22*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[3];
    } oBufferWithLength;
} BCL_TxBuffer41;
/* Buffer for Function  lsg.id=41 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1545];
    } oBufferWithLength;
} BCL_TxBuffer42;
/* Buffer for Function  lsg.id=41 fct.id=24*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer43;
/* Buffer for Function  lsg.id=41 fct.id=25*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[40];
    } oBufferWithLength;
} BCL_TxBuffer44;
/* Buffer for Function  lsg.id=41 fct.id=26*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer45;
/* Buffer for Function  lsg.id=41 fct.id=27*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer46;
/* Buffer for Function  lsg.id=41 fct.id=29*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[947];
    } oBufferWithLength;
} BCL_TxBuffer47;
/* Buffer for Function  lsg.id=41 fct.id=32*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
    } oBufferWithLength;
} BCL_TxBuffer48;
/* Buffer for Function  lsg.id=43 fct.id=1*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[44];
    } oBufferWithLength;
} BCL_TxBuffer49;
/* Buffer for Function  lsg.id=43 fct.id=2*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_TxBuffer50;
/* Buffer for Function  lsg.id=43 fct.id=3*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer51;
/* Buffer for Function  lsg.id=43 fct.id=4*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer52;
/* Buffer for Function  lsg.id=43 fct.id=15*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer53;
/* Buffer for Function  lsg.id=43 fct.id=16*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_TxBuffer54;
/* Buffer for Function  lsg.id=43 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[2];
    } oBufferWithLength;
} BCL_TxBuffer55;
/* Buffer for Function  lsg.id=43 fct.id=18*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer56;
/* Buffer for Function  lsg.id=43 fct.id=19*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_TxBuffer57;
/* Buffer for Function  lsg.id=43 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[685];
    } oBufferWithLength;
} BCL_TxBuffer58;
/* Buffer for Function  lsg.id=43 fct.id=21*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer59;
/* Buffer for Function  lsg.id=43 fct.id=22*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer60;
/* Buffer for Function  lsg.id=43 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer61;
/* Buffer for Function  lsg.id=48 fct.id=1*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[499];
    } oBufferWithLength;
} BCL_TxBuffer62;
/* Buffer for Function  lsg.id=48 fct.id=2*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[6];
    } oBufferWithLength;
} BCL_TxBuffer63;
/* Buffer for Function  lsg.id=48 fct.id=3*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer64;
/* Buffer for Function  lsg.id=48 fct.id=4*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer65;
/* Buffer for Function  lsg.id=48 fct.id=14*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer66;
/* Buffer for Function  lsg.id=48 fct.id=15*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer67;
/* Buffer for Function  lsg.id=48 fct.id=16*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[8];
    } oBufferWithLength;
} BCL_TxBuffer68;
/* Buffer for Function  lsg.id=48 fct.id=17*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[32];
    } oBufferWithLength;
} BCL_TxBuffer69;
/* Buffer for Function  lsg.id=48 fct.id=20*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer70;
/* Buffer for Function  lsg.id=48 fct.id=22*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer71;
/* Buffer for Function  lsg.id=48 fct.id=23*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[4];
    } oBufferWithLength;
} BCL_TxBuffer72;
/* Buffer for Function  lsg.id=48 fct.id=25*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer73;
/* Buffer for Function  lsg.id=48 fct.id=26*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[1];
    } oBufferWithLength;
} BCL_TxBuffer74;
/* Buffer for Function  lsg.id=48 fct.id=30*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[151];
    } oBufferWithLength;
} BCL_TxBuffer75;
/* Buffer for Function  lsg.id=48 fct.id=31*/
BAP_RAM_DATA_FAR
struct
{
    BapBclTxRamRowFlags_ot flags;
    BapError_et eErrorCode;
    BapOpCodes_et eOpCode;
    struct 
    {
        uint16_t i16Length;
        uint8_t  aru8Buffer[273];
    } oBufferWithLength;
} BCL_TxBuffer76;

/* BAP BCL Inhibit Timer Tabelle ***********************************************/

BAP_RAM_DATA_FAR
BapInhibitRamRow_ot BAP_InhibitRamTable[]=
{
    {0,0,0,{0,0}},
    {0,0,0,{0,0}},
    {0,0,0,{0,0}},
    {0,0,0,{0,0}},
    {0,0,0,{0,0}}
};

extern BAP_ROM_DATA_FAR BapMapCanIdToLsg_ot BAP_ROM_CONST BAP_MapCanIdToLsgRomRowTable[];

BAP_ROM_DATA_FAR BapInhibitRomRow_ot BAP_ROM_CONST BAP_InhibitRomTable []=
{
    {5,CanTxBAP_MDI_02,&BAP_MapCanIdToLsgRomRowTable[0],1},
    {2,CanTxBAP_Telefon_04,&BAP_MapCanIdToLsgRomRowTable[4],1},
    {3,CanTxBAP_Telefon_01,&BAP_MapCanIdToLsgRomRowTable[1],1},
    {3,CanTxBAP_Telefon_02,&BAP_MapCanIdToLsgRomRowTable[2],1},
    {2,CanTxBAP_Telefon_03,&BAP_MapCanIdToLsgRomRowTable[3],1}
};

/* BAP BCL InterTelegram Timer Tabelle *****************************************/


BAP_ROM_DATA_FAR BapInterTelegramTimerMapRow_ot BAP_ROM_CONST BAP_InterTelegramTimerMappingTable[]=
{
    {40,17},
    {40,27},
    {40,28},
    {40,36},
    {41,18},
    {41,19},
    {41,20},
    {41,23},
    {41,24},
    {41,25},
    {41,29},
    {41,32},
    {43,18},
    {43,20}
};

BAP_RAM_DATA_FAR
uint16_t BAP_u16InterTelegramTimerTable[]=
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};
/* BAP BCL CanTxSegmentation Tabelle *****************************************/

BAP_RAM_DATA_FAR
BapCanTxSegmentationRamRow_ot BAP_u16CanTxSegmentationTable[]=
{
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0},
    {{0}, 0, 0}
};
/* BAP BCL CanRxSegmentation Tabelle *****************************************/

BAP_RAM_DATA_FAR
BapCanRxSegmentationRamRow_ot BAP_u16CanRxSegmentationTable[]=
{
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0},
    {{0}, 0}
};

/* BCL CAN Tabellen fuer dynamische Kanalvergabe bei Segmentierung **************/

#ifdef BAP_USES_SEGMENTATION
#define BAP_Uses_SegmentationTables(x) ,x


BAP_RAM_DATA_FAR
BapCanTxDynSegChannels_ot BAP_CanTxSegmentationChannels[]=
{
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}}
};

BAP_ROM_DATA_FAR const BapCanMsgId_t BAP_ROM_CONST BAP_CanTxMsgIdToSegChannelMappingTable[]=
{
    CanTxBAP_MDI_02,
    CanTxBAP_Telefon_04,
    CanTxBAP_Telefon_01,
    CanTxBAP_Telefon_02,
    CanTxBAP_Telefon_03
};

BAP_RAM_DATA_FAR
BapCanRxDynSegChannels_ot  BAP_CanRxSegmentationChannels[]=
{
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}},
    {{NULL, NULL, NULL, NULL}}
};

BAP_ROM_DATA_FAR const BapCanMsgId_t BAP_ROM_CONST BAP_CanRxMsgIdToSegChannelMappingTable[]=
{
    CanRxBAP_ASG_07,
    CanRxBAP_ASG_03,
    CanRxBAP_ASG_06
};

#else
#define BAP_Uses_SegmentationTables(x)
#endif
/* CAN Tx ROM Tabelle *****************************************************************/


BAP_ROM_DATA_FAR BapCanTxRomRow_ot BAP_ROM_CONST BAP_CanTxRomTable[]={
    /*                               u8InterfaceNumber, eSegmentationType, *pCanTxRamRow, *pBclTxRamRow, u8InhibitTimerIndex, canMsgId, poTxDynSegChannels, *poFctRomRow */
    /* LSG#id="40, Function#id="1"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[0], (BapBclTxRamRow_pot) &BCL_TxBuffer1, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[0])},
    /* LSG#id="40, Function#id="2"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer2, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[1])},
    /* LSG#id="40, Function#id="3"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[1], (BapBclTxRamRow_pot) &BCL_TxBuffer3, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[2])},
    /* LSG#id="40, Function#id="4"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer4, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[3])},
    /* LSG#id="40, Function#id="14"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer5, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[4])},
    /* LSG#id="40, Function#id="15"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer6, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[5])},
    /* LSG#id="40, Function#id="16"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[2], (BapBclTxRamRow_pot) &BCL_TxBuffer7, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[6])},
    /* LSG#id="40, Function#id="17"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[3], (BapBclTxRamRow_pot) &BCL_TxBuffer8, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[7])},
    /* LSG#id="40, Function#id="18"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[4], (BapBclTxRamRow_pot) &BCL_TxBuffer9, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[8])},
    /* LSG#id="40, Function#id="19"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer10, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[9])},
    /* LSG#id="40, Function#id="20"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer11, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[10])},
    /* LSG#id="40, Function#id="22"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[5], (BapBclTxRamRow_pot) &BCL_TxBuffer12, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[11])},
    /* LSG#id="40, Function#id="23"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer13, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[12])},
    /* LSG#id="40, Function#id="25"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer14, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[13])},
    /* LSG#id="40, Function#id="26"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[6], (BapBclTxRamRow_pot) &BCL_TxBuffer15, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[14])},
    /* LSG#id="40, Function#id="27"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[7], (BapBclTxRamRow_pot) &BCL_TxBuffer16, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[2]), (BapFctRomRow_pot) (&BAP_FctRomTables[15])},
    /* LSG#id="40, Function#id="28"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer17, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[16])},
    /* LSG#id="40, Function#id="29"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer18, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[17])},
    /* LSG#id="40, Function#id="30"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer19, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[18])},
    /* LSG#id="40, Function#id="31"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer20, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[19])},
    /* LSG#id="40, Function#id="32"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer21, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[20])},
    /* LSG#id="40, Function#id="33"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer22, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[21])},
    /* LSG#id="40, Function#id="34"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer23, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[22])},
    /* LSG#id="40, Function#id="36"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer24, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[23])},
    /* LSG#id="40, Function#id="38"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer25, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[24])},
    /* LSG#id="40, Function#id="39"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer26, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[25])},
    /* LSG#id="40, Function#id="40"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer27, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[26])},
    /* LSG#id="40, Function#id="43"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer28, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[27])},
    /* LSG#id="40, Function#id="48"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer29, 2, CanTxBAP_Telefon_01  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[28])},
    /* LSG#id="41, Function#id="1"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[8], (BapBclTxRamRow_pot) &BCL_TxBuffer30, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[29])},
    /* LSG#id="41, Function#id="2"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer31, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[30])},
    /* LSG#id="41, Function#id="3"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer32, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[31])},
    /* LSG#id="41, Function#id="4"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer33, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[32])},
    /* LSG#id="41, Function#id="14"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer34, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[33])},
    /* LSG#id="41, Function#id="15"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer35, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[34])},
    /* LSG#id="41, Function#id="16"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[9], (BapBclTxRamRow_pot) &BCL_TxBuffer36, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[35])},
    /* LSG#id="41, Function#id="17"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer37, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[36])},
    /* LSG#id="41, Function#id="18"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[10], (BapBclTxRamRow_pot) &BCL_TxBuffer38, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[37])},
    /* LSG#id="41, Function#id="19"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[11], (BapBclTxRamRow_pot) &BCL_TxBuffer39, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[38])},
    /* LSG#id="41, Function#id="20"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[12], (BapBclTxRamRow_pot) &BCL_TxBuffer40, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[39])},
    /* LSG#id="41, Function#id="22"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer41, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[40])},
    /* LSG#id="41, Function#id="23"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[13], (BapBclTxRamRow_pot) &BCL_TxBuffer42, 3, CanTxBAP_Telefon_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[3]), (BapFctRomRow_pot) (&BAP_FctRomTables[41])},
    /* LSG#id="41, Function#id="24"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer43, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[42])},
    /* LSG#id="41, Function#id="25"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[14], (BapBclTxRamRow_pot) &BCL_TxBuffer44, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[43])},
    /* LSG#id="41, Function#id="26"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer45, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[44])},
    /* LSG#id="41, Function#id="27"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer46, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[45])},
    /* LSG#id="41, Function#id="29"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[15], (BapBclTxRamRow_pot) &BCL_TxBuffer47, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[4]), (BapFctRomRow_pot) (&BAP_FctRomTables[46])},
    /* LSG#id="41, Function#id="32"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer48, 4, CanTxBAP_Telefon_03  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[47])},
    /* LSG#id="43, Function#id="1"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[16], (BapBclTxRamRow_pot) &BCL_TxBuffer49, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[1]), (BapFctRomRow_pot) (&BAP_FctRomTables[48])},
    /* LSG#id="43, Function#id="2"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer50, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[49])},
    /* LSG#id="43, Function#id="3"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[17], (BapBclTxRamRow_pot) &BCL_TxBuffer51, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[1]), (BapFctRomRow_pot) (&BAP_FctRomTables[50])},
    /* LSG#id="43, Function#id="4"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer52, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[51])},
    /* LSG#id="43, Function#id="15"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer53, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[52])},
    /* LSG#id="43, Function#id="16"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer54, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[53])},
    /* LSG#id="43, Function#id="17"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer55, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[54])},
    /* LSG#id="43, Function#id="18"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[18], (BapBclTxRamRow_pot) &BCL_TxBuffer56, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[1]), (BapFctRomRow_pot) (&BAP_FctRomTables[55])},
    /* LSG#id="43, Function#id="19"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer57, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[56])},
    /* LSG#id="43, Function#id="20"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[19], (BapBclTxRamRow_pot) &BCL_TxBuffer58, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[1]), (BapFctRomRow_pot) (&BAP_FctRomTables[57])},
    /* LSG#id="43, Function#id="21"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer59, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[58])},
    /* LSG#id="43, Function#id="22"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer60, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[59])},
    /* LSG#id="43, Function#id="23"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer61, 1, CanTxBAP_Telefon_04  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[60])},
    /* LSG#id="48, Function#id="1"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[20], (BapBclTxRamRow_pot) &BCL_TxBuffer62, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[61])},
    /* LSG#id="48, Function#id="2"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer63, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[62])},
    /* LSG#id="48, Function#id="3"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[21], (BapBclTxRamRow_pot) &BCL_TxBuffer64, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[63])},
    /* LSG#id="48, Function#id="4"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer65, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[64])},
    /* LSG#id="48, Function#id="14"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer66, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[65])},
    /* LSG#id="48, Function#id="15"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer67, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[66])},
    /* LSG#id="48, Function#id="16"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[22], (BapBclTxRamRow_pot) &BCL_TxBuffer68, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[67])},
    /* LSG#id="48, Function#id="17"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[23], (BapBclTxRamRow_pot) &BCL_TxBuffer69, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[68])},
    /* LSG#id="48, Function#id="20"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer70, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[69])},
    /* LSG#id="48, Function#id="22"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer71, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[70])},
    /* LSG#id="48, Function#id="23"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer72, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[71])},
    /* LSG#id="48, Function#id="25"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer73, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[72])},
    /* LSG#id="48, Function#id="26"*/ {1, BapSegType_Unsegmented,(BapCanTxSegmentationRamRow_pot) NULL, (BapBclTxRamRow_pot) &BCL_TxBuffer74, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(NULL), (BapFctRomRow_pot) (&BAP_FctRomTables[73])},
    /* LSG#id="48, Function#id="30"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[24], (BapBclTxRamRow_pot) &BCL_TxBuffer75, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[74])},
    /* LSG#id="48, Function#id="31"*/ {1, BapSegType_ChannelOneTwoThreeFour,&BAP_u16CanTxSegmentationTable[25], (BapBclTxRamRow_pot) &BCL_TxBuffer76, 0, CanTxBAP_MDI_02  BAP_Uses_SegmentationTables(&BAP_CanTxSegmentationChannels[0]), (BapFctRomRow_pot) (&BAP_FctRomTables[75])}
};
/* CAN Rx ROM Tabelle *****************************************************************/

BAP_ROM_DATA_FAR BapCanRxRomRow_ot BAP_ROM_CONST BAP_CanRxRomTable[]={
    /*  lsgId, fctId, u16Size, u8InterfaceNumber, eSegmentationType, u16IntertelegramTime, *pCanRxRamRow, *pBclRxRamRow, *pu16InterTelegramTimer, canMsgId, poRxDynSegChannels */
    {40, 1, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 2, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 3, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 4, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 14, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 15, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 16, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 17, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[0], (BapBclRxRamRow_pot) &BCL_RxBuffer1,  &BAP_u16InterTelegramTimerTable[0], CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[1])},
    {40, 18, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 19, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 20, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 22, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 23, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 25, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 26, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 27, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[1], (BapBclRxRamRow_pot) &BCL_RxBuffer2,  &BAP_u16InterTelegramTimerTable[1], CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[1])},
    {40, 28, 190, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[2], (BapBclRxRamRow_pot) &BCL_RxBuffer3,  &BAP_u16InterTelegramTimerTable[2], CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[1])},
    {40, 29, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 30, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 31, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 32, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 33, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 34, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 36, 40, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[3], (BapBclRxRamRow_pot) &BCL_RxBuffer4,  &BAP_u16InterTelegramTimerTable[3], CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[1])},
    {40, 38, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 39, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 40, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 43, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {40, 48, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_03 BAP_Uses_SegmentationTables(NULL)},
    {41, 1, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 2, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 3, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 4, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 14, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 15, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 16, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 17, 4, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer5,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 18, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[4], (BapBclRxRamRow_pot) &BCL_RxBuffer6,  &BAP_u16InterTelegramTimerTable[4], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 19, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[5], (BapBclRxRamRow_pot) &BCL_RxBuffer7,  &BAP_u16InterTelegramTimerTable[5], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 20, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[6], (BapBclRxRamRow_pot) &BCL_RxBuffer8,  &BAP_u16InterTelegramTimerTable[6], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 22, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 23, 5, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[7], (BapBclRxRamRow_pot) &BCL_RxBuffer9,  &BAP_u16InterTelegramTimerTable[7], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 24, 51, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[8], (BapBclRxRamRow_pot) &BCL_RxBuffer10,  &BAP_u16InterTelegramTimerTable[8], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 25, 40, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[9], (BapBclRxRamRow_pot) &BCL_RxBuffer11,  &BAP_u16InterTelegramTimerTable[9], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 26, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 27, 4, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer12,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {41, 29, 8, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[10], (BapBclRxRamRow_pot) &BCL_RxBuffer13,  &BAP_u16InterTelegramTimerTable[10], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {41, 32, 841, 1, BapSegType_ChannelOneTwoThreeFour,30, &BAP_u16CanRxSegmentationTable[11], (BapBclRxRamRow_pot) &BCL_RxBuffer14,  &BAP_u16InterTelegramTimerTable[11], CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[2])},
    {43, 1, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 2, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 3, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 4, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 15, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 16, 2, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer15,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 17, 2, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer16,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 18, 8, 1, BapSegType_ChannelOneTwoThreeFour,50, &BAP_u16CanRxSegmentationTable[12], (BapBclRxRamRow_pot) &BCL_RxBuffer17,  &BAP_u16InterTelegramTimerTable[12], CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[0])},
    {43, 19, 6, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer18,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 20, 4, 1, BapSegType_ChannelOneTwoThreeFour,50, &BAP_u16CanRxSegmentationTable[13], (BapBclRxRamRow_pot) &BCL_RxBuffer19,  &BAP_u16InterTelegramTimerTable[13], CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(&BAP_CanRxSegmentationChannels[0])},
    {43, 21, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 22, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {43, 23, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_07 BAP_Uses_SegmentationTables(NULL)},
    {48, 1, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 2, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 3, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 4, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 14, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 15, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 16, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 17, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 20, 1, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 22, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 23, 4, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer20,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 25, 2, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer21,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 26, 2, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) &BCL_RxBuffer22,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 30, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)},
    {48, 31, 0, 1, BapSegType_Unsegmented,0, (BapCanRxSegmentationRamRow_pot)   NULL, (BapBclRxRamRow_pot) NULL,  NULL, CanRxBAP_ASG_06 BAP_Uses_SegmentationTables(NULL)}
};
/* CAN Rx Ringbuffer ******************************************************************/
BAP_RAM_DATA_FAR
BapCanRxBuffer_ot BAP_CanRxRingBuffer[] =
{

    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0},
    {0, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0}
};

BAP_RAM_DATA_FAR
uint8_t BAP_u8CanRxRingBufferReadOffset = 0;
BAP_RAM_DATA_FAR
volatile uint8_t BAP_u8CanRxRingBufferWriteOffset = 0;



#ifdef BAP_USES_STATUS_ALL
#define BAP_Uses_Status_All(x) ,x
#else
#define BAP_Uses_Status_All(x)
#endif
/* BCL Bus Indirection Tx Tabelle ********************************************************************/
BAP_ROM_DATA_FAR BapBusIndirectionTxRomRow_ot BAP_ROM_CONST BAP_BusIndTxTable[]={
    /*                               eInterfaceType, BusTxRomRow, poBclTxRamRow, u16BufferSize, bDataTypeSupportsVarLength */
        /* LSG#id="40, Function#id="1"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[0]}, (BapBclTxRamRow_pot)&BCL_TxBuffer1, 222 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="2"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[1]}, (BapBclTxRamRow_pot)&BCL_TxBuffer2, 6 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="3"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[2]}, (BapBclTxRamRow_pot)&BCL_TxBuffer3, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="4"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[3]}, (BapBclTxRamRow_pot)&BCL_TxBuffer4, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="14"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[4]}, (BapBclTxRamRow_pot)&BCL_TxBuffer5, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="15"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[5]}, (BapBclTxRamRow_pot)&BCL_TxBuffer6, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="16"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[6]}, (BapBclTxRamRow_pot)&BCL_TxBuffer7, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="17"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[7]}, (BapBclTxRamRow_pot)&BCL_TxBuffer8, 525 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="18"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[8]}, (BapBclTxRamRow_pot)&BCL_TxBuffer9, 130 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="19"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[9]}, (BapBclTxRamRow_pot)&BCL_TxBuffer10, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="20"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[10]}, (BapBclTxRamRow_pot)&BCL_TxBuffer11, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="22"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[11]}, (BapBclTxRamRow_pot)&BCL_TxBuffer12, 40 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="23"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[12]}, (BapBclTxRamRow_pot)&BCL_TxBuffer13, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="25"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[13]}, (BapBclTxRamRow_pot)&BCL_TxBuffer14, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="26"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[14]}, (BapBclTxRamRow_pot)&BCL_TxBuffer15, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="27"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[15]}, (BapBclTxRamRow_pot)&BCL_TxBuffer16, 985 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="40, Function#id="28"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[16]}, (BapBclTxRamRow_pot)&BCL_TxBuffer17, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="29"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[17]}, (BapBclTxRamRow_pot)&BCL_TxBuffer18, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="30"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[18]}, (BapBclTxRamRow_pot)&BCL_TxBuffer19, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="31"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[19]}, (BapBclTxRamRow_pot)&BCL_TxBuffer20, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="32"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[20]}, (BapBclTxRamRow_pot)&BCL_TxBuffer21, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="33"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[21]}, (BapBclTxRamRow_pot)&BCL_TxBuffer22, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="34"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[22]}, (BapBclTxRamRow_pot)&BCL_TxBuffer23, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="36"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[23]}, (BapBclTxRamRow_pot)&BCL_TxBuffer24, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="38"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[24]}, (BapBclTxRamRow_pot)&BCL_TxBuffer25, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="39"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[25]}, (BapBclTxRamRow_pot)&BCL_TxBuffer26, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="40"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[26]}, (BapBclTxRamRow_pot)&BCL_TxBuffer27, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="43"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[27]}, (BapBclTxRamRow_pot)&BCL_TxBuffer28, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="40, Function#id="48"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[28]}, (BapBclTxRamRow_pot)&BCL_TxBuffer29, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="1"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[29]}, (BapBclTxRamRow_pot)&BCL_TxBuffer30, 80 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="2"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[30]}, (BapBclTxRamRow_pot)&BCL_TxBuffer31, 6 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="3"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[31]}, (BapBclTxRamRow_pot)&BCL_TxBuffer32, 8 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="4"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[32]}, (BapBclTxRamRow_pot)&BCL_TxBuffer33, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="14"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[33]}, (BapBclTxRamRow_pot)&BCL_TxBuffer34, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="15"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[34]}, (BapBclTxRamRow_pot)&BCL_TxBuffer35, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="16"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[35]}, (BapBclTxRamRow_pot)&BCL_TxBuffer36, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="17"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[36]}, (BapBclTxRamRow_pot)&BCL_TxBuffer37, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="18"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[37]}, (BapBclTxRamRow_pot)&BCL_TxBuffer38, 738 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="19"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[38]}, (BapBclTxRamRow_pot)&BCL_TxBuffer39, 738 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="20"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[39]}, (BapBclTxRamRow_pot)&BCL_TxBuffer40, 738 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="22"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[40]}, (BapBclTxRamRow_pot)&BCL_TxBuffer41, 3 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="23"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[41]}, (BapBclTxRamRow_pot)&BCL_TxBuffer42, 1545 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="24"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[42]}, (BapBclTxRamRow_pot)&BCL_TxBuffer43, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="25"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[43]}, (BapBclTxRamRow_pot)&BCL_TxBuffer44, 40 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="26"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[44]}, (BapBclTxRamRow_pot)&BCL_TxBuffer45, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="27"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[45]}, (BapBclTxRamRow_pot)&BCL_TxBuffer46, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="41, Function#id="29"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[46]}, (BapBclTxRamRow_pot)&BCL_TxBuffer47, 947 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="41, Function#id="32"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[47]}, (BapBclTxRamRow_pot)&BCL_TxBuffer48, 0 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="1"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[48]}, (BapBclTxRamRow_pot)&BCL_TxBuffer49, 44 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="43, Function#id="2"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[49]}, (BapBclTxRamRow_pot)&BCL_TxBuffer50, 6 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="3"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[50]}, (BapBclTxRamRow_pot)&BCL_TxBuffer51, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="43, Function#id="4"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[51]}, (BapBclTxRamRow_pot)&BCL_TxBuffer52, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="15"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[52]}, (BapBclTxRamRow_pot)&BCL_TxBuffer53, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="16"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[53]}, (BapBclTxRamRow_pot)&BCL_TxBuffer54, 2 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="17"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[54]}, (BapBclTxRamRow_pot)&BCL_TxBuffer55, 2 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="18"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[55]}, (BapBclTxRamRow_pot)&BCL_TxBuffer56, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="43, Function#id="19"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[56]}, (BapBclTxRamRow_pot)&BCL_TxBuffer57, 6 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="20"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[57]}, (BapBclTxRamRow_pot)&BCL_TxBuffer58, 685 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="43, Function#id="21"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[58]}, (BapBclTxRamRow_pot)&BCL_TxBuffer59, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="22"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[59]}, (BapBclTxRamRow_pot)&BCL_TxBuffer60, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="43, Function#id="23"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[60]}, (BapBclTxRamRow_pot)&BCL_TxBuffer61, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="1"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[61]}, (BapBclTxRamRow_pot)&BCL_TxBuffer62, 499 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="48, Function#id="2"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[62]}, (BapBclTxRamRow_pot)&BCL_TxBuffer63, 6 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="3"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[63]}, (BapBclTxRamRow_pot)&BCL_TxBuffer64, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="48, Function#id="4"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[64]}, (BapBclTxRamRow_pot)&BCL_TxBuffer65, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="14"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[65]}, (BapBclTxRamRow_pot)&BCL_TxBuffer66, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="15"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[66]}, (BapBclTxRamRow_pot)&BCL_TxBuffer67, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="16"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[67]}, (BapBclTxRamRow_pot)&BCL_TxBuffer68, 8 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="48, Function#id="17"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[68]}, (BapBclTxRamRow_pot)&BCL_TxBuffer69, 32 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="48, Function#id="20"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[69]}, (BapBclTxRamRow_pot)&BCL_TxBuffer70, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="22"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[70]}, (BapBclTxRamRow_pot)&BCL_TxBuffer71, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="23"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[71]}, (BapBclTxRamRow_pot)&BCL_TxBuffer72, 4 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="25"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[72]}, (BapBclTxRamRow_pot)&BCL_TxBuffer73, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="26"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[73]}, (BapBclTxRamRow_pot)&BCL_TxBuffer74, 1 BAP_Uses_Status_All(BAP_FALSE) },
        /* LSG#id="48, Function#id="30"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[74]}, (BapBclTxRamRow_pot)&BCL_TxBuffer75, 151 BAP_Uses_Status_All(BAP_TRUE) },
        /* LSG#id="48, Function#id="31"*/{ BapIft_Can, {(BapCanTxRomRow_pot) &BAP_CanTxRomTable[75]}, (BapBclTxRamRow_pot)&BCL_TxBuffer76, 273 BAP_Uses_Status_All(BAP_TRUE) }
};
/* BCL Bus Indirection Rx Tabelle ********************************************************************/
BAP_ROM_DATA_FAR BapBusIndirectionRxRomRow_ot BAP_ROM_CONST BAP_BusIndRxTable[]=
{
        /* eInterfaceType, BusRxRomRow, poBclRxRamRow */
        /* LSG#id="40, Function#id="1"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[0]}, NULL},
        /* LSG#id="40, Function#id="2"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[1]}, NULL},
        /* LSG#id="40, Function#id="3"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[2]}, NULL},
        /* LSG#id="40, Function#id="4"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[3]}, NULL},
        /* LSG#id="40, Function#id="14"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[4]}, NULL},
        /* LSG#id="40, Function#id="15"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[5]}, NULL},
        /* LSG#id="40, Function#id="16"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[6]}, NULL},
        /* LSG#id="40, Function#id="17"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[7]}, (BapBclRxRamRow_pot) &BCL_RxBuffer1},
        /* LSG#id="40, Function#id="18"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[8]}, NULL},
        /* LSG#id="40, Function#id="19"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[9]}, NULL},
        /* LSG#id="40, Function#id="20"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[10]}, NULL},
        /* LSG#id="40, Function#id="22"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[11]}, NULL},
        /* LSG#id="40, Function#id="23"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[12]}, NULL},
        /* LSG#id="40, Function#id="25"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[13]}, NULL},
        /* LSG#id="40, Function#id="26"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[14]}, NULL},
        /* LSG#id="40, Function#id="27"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[15]}, (BapBclRxRamRow_pot) &BCL_RxBuffer2},
        /* LSG#id="40, Function#id="28"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[16]}, (BapBclRxRamRow_pot) &BCL_RxBuffer3},
        /* LSG#id="40, Function#id="29"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[17]}, NULL},
        /* LSG#id="40, Function#id="30"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[18]}, NULL},
        /* LSG#id="40, Function#id="31"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[19]}, NULL},
        /* LSG#id="40, Function#id="32"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[20]}, NULL},
        /* LSG#id="40, Function#id="33"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[21]}, NULL},
        /* LSG#id="40, Function#id="34"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[22]}, NULL},
        /* LSG#id="40, Function#id="36"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[23]}, (BapBclRxRamRow_pot) &BCL_RxBuffer4},
        /* LSG#id="40, Function#id="38"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[24]}, NULL},
        /* LSG#id="40, Function#id="39"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[25]}, NULL},
        /* LSG#id="40, Function#id="40"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[26]}, NULL},
        /* LSG#id="40, Function#id="43"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[27]}, NULL},
        /* LSG#id="40, Function#id="48"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[28]}, NULL},
        /* LSG#id="41, Function#id="1"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[29]}, NULL},
        /* LSG#id="41, Function#id="2"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[30]}, NULL},
        /* LSG#id="41, Function#id="3"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[31]}, NULL},
        /* LSG#id="41, Function#id="4"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[32]}, NULL},
        /* LSG#id="41, Function#id="14"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[33]}, NULL},
        /* LSG#id="41, Function#id="15"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[34]}, NULL},
        /* LSG#id="41, Function#id="16"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[35]}, NULL},
        /* LSG#id="41, Function#id="17"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[36]}, (BapBclRxRamRow_pot) &BCL_RxBuffer5},
        /* LSG#id="41, Function#id="18"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[37]}, (BapBclRxRamRow_pot) &BCL_RxBuffer6},
        /* LSG#id="41, Function#id="19"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[38]}, (BapBclRxRamRow_pot) &BCL_RxBuffer7},
        /* LSG#id="41, Function#id="20"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[39]}, (BapBclRxRamRow_pot) &BCL_RxBuffer8},
        /* LSG#id="41, Function#id="22"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[40]}, NULL},
        /* LSG#id="41, Function#id="23"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[41]}, (BapBclRxRamRow_pot) &BCL_RxBuffer9},
        /* LSG#id="41, Function#id="24"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[42]}, (BapBclRxRamRow_pot) &BCL_RxBuffer10},
        /* LSG#id="41, Function#id="25"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[43]}, (BapBclRxRamRow_pot) &BCL_RxBuffer11},
        /* LSG#id="41, Function#id="26"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[44]}, NULL},
        /* LSG#id="41, Function#id="27"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[45]}, (BapBclRxRamRow_pot) &BCL_RxBuffer12},
        /* LSG#id="41, Function#id="29"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[46]}, (BapBclRxRamRow_pot) &BCL_RxBuffer13},
        /* LSG#id="41, Function#id="32"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[47]}, (BapBclRxRamRow_pot) &BCL_RxBuffer14},
        /* LSG#id="43, Function#id="1"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[48]}, NULL},
        /* LSG#id="43, Function#id="2"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[49]}, NULL},
        /* LSG#id="43, Function#id="3"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[50]}, NULL},
        /* LSG#id="43, Function#id="4"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[51]}, NULL},
        /* LSG#id="43, Function#id="15"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[52]}, NULL},
        /* LSG#id="43, Function#id="16"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[53]}, (BapBclRxRamRow_pot) &BCL_RxBuffer15},
        /* LSG#id="43, Function#id="17"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[54]}, (BapBclRxRamRow_pot) &BCL_RxBuffer16},
        /* LSG#id="43, Function#id="18"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[55]}, (BapBclRxRamRow_pot) &BCL_RxBuffer17},
        /* LSG#id="43, Function#id="19"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[56]}, (BapBclRxRamRow_pot) &BCL_RxBuffer18},
        /* LSG#id="43, Function#id="20"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[57]}, (BapBclRxRamRow_pot) &BCL_RxBuffer19},
        /* LSG#id="43, Function#id="21"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[58]}, NULL},
        /* LSG#id="43, Function#id="22"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[59]}, NULL},
        /* LSG#id="43, Function#id="23"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[60]}, NULL},
        /* LSG#id="48, Function#id="1"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[61]}, NULL},
        /* LSG#id="48, Function#id="2"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[62]}, NULL},
        /* LSG#id="48, Function#id="3"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[63]}, NULL},
        /* LSG#id="48, Function#id="4"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[64]}, NULL},
        /* LSG#id="48, Function#id="14"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[65]}, NULL},
        /* LSG#id="48, Function#id="15"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[66]}, NULL},
        /* LSG#id="48, Function#id="16"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[67]}, NULL},
        /* LSG#id="48, Function#id="17"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[68]}, NULL},
        /* LSG#id="48, Function#id="20"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[69]}, NULL},
        /* LSG#id="48, Function#id="22"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[70]}, NULL},
        /* LSG#id="48, Function#id="23"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[71]}, (BapBclRxRamRow_pot) &BCL_RxBuffer20},
        /* LSG#id="48, Function#id="25"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[72]}, (BapBclRxRamRow_pot) &BCL_RxBuffer21},
        /* LSG#id="48, Function#id="26"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[73]}, (BapBclRxRamRow_pot) &BCL_RxBuffer22},
        /* LSG#id="48, Function#id="30"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[74]}, NULL},
        /* LSG#id="48, Function#id="31"*/{ BapIft_Can, {(BapCanRxRomRow_pot) &BAP_CanRxRomTable[75]}, NULL}
};

/* MapCanIdToLsgRomRow Tabelle ********************************************************/
extern BAP_ROM_DATA_FAR BapMapCanIdByLsg_ot BAP_ROM_CONST BAP_MapCanIdByLsgToCanTxRomRowTable[];

BAP_ROM_DATA_FAR BapMapCanIdToLsg_ot BAP_ROM_CONST BAP_MapCanIdToLsgRomRowTable[]=
{
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_LsgRomTables[3],&BAP_MapCanIdByLsgToCanTxRomRowTable[0],15},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_LsgRomTables[0],&BAP_MapCanIdByLsgToCanTxRomRowTable[15],29},
       /* canId=CanTxBAP_Telefon_02 LsgId=41*/ {&BAP_LsgRomTables[1],&BAP_MapCanIdByLsgToCanTxRomRowTable[44],1},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_LsgRomTables[1],&BAP_MapCanIdByLsgToCanTxRomRowTable[45],18},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_LsgRomTables[2],&BAP_MapCanIdByLsgToCanTxRomRowTable[63],13}
};


/* MapCanIdByLsgToCanTxRomRow Tabelle ******************************************************/

BAP_ROM_DATA_FAR BapMapCanIdByLsg_ot BAP_ROM_CONST BAP_MapCanIdByLsgToCanTxRomRowTable[]=
{
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[61]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[62]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[63]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[64]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[65]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[66]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[67]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[68]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[69]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[70]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[71]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[72]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[73]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[74]},
       /* canId=CanTxBAP_MDI_02 LsgId=48*/ {&BAP_CanTxRomTable[75]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[0]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[1]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[2]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[3]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[4]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[5]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[6]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[7]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[8]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[9]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[10]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[11]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[12]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[13]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[14]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[15]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[16]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[17]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[18]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[19]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[20]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[21]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[22]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[23]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[24]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[25]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[26]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[27]},
       /* canId=CanTxBAP_Telefon_01 LsgId=40*/ {&BAP_CanTxRomTable[28]},
       /* canId=CanTxBAP_Telefon_02 LsgId=41*/ {&BAP_CanTxRomTable[41]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[29]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[30]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[31]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[32]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[33]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[34]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[35]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[36]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[37]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[38]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[39]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[40]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[42]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[43]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[44]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[45]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[46]},
       /* canId=CanTxBAP_Telefon_03 LsgId=41*/ {&BAP_CanTxRomTable[47]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[48]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[49]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[50]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[51]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[52]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[53]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[54]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[55]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[56]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[57]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[58]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[59]},
       /* canId=CanTxBAP_Telefon_04 LsgId=43*/ {&BAP_CanTxRomTable[60]}
};


/*  Globale BAP ROM Tabelle ************************************************************/
/*  Enthaelt die ROM Konfiguration fuer alle Schichten je Funktions-ID                 */


BAP_ROM_DATA_FAR BapFctRomRow_ot BAP_ROM_CONST BAP_FctRomTables[]={
    /* lsgId, fctId, eFunctionClass, u8OpCodeSendMask, u8OpCodeReceiveMask, eRxDataType, u16RxSize, eTxDataType, u16TxSize, *poBalRamRow, bHeartbeatOn, nMaxRetryCounter, nMaxRetryTimer, *poBplRamRow, u8TxTableSize, *poBusTxTable, u8RxTableSize, *poBusRxTable*/
    {40, 1, BapFctCls_Cache, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,222 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [0], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [0]},
    {40, 2, BapFctCls_Property, 0x91, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,6 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [1], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [1]},
    {40, 3, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [2], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [2]},
    {40, 4, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [3], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [3]},
    {40, 14, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,4 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [4], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [4]},
    {40, 15, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [5], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [5]},
    {40, 16, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [6], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [6]},
    {40, 17, BapFctCls_Array, 0x98, 0x2, BapDt_ByteSequence,5 , BapDt_ByteSequence,525 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [7], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [7]},
    {40, 18, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,130 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [8], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [8]},
    {40, 19, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [9], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [9]},
    {40, 20, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [10], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [10]},
    {40, 22, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,40 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [11], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [11]},
    {40, 23, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [12], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [12]},
    {40, 25, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [13], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [13]},
    {40, 26, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [14], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [14]},
    {40, 27, BapFctCls_Array, 0x98, 0x2, BapDt_ByteSequence,5 , BapDt_ByteSequence,985 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [15], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [15]},
    {40, 28, BapFctCls_Method, 0x98, 0xe, BapDt_ByteSequence,190 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[0], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [16], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [16]},
    {40, 29, BapFctCls_Method, 0x98, 0xc, BapDt_Int8,1 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[1], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [17], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [17]},
    {40, 30, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[2], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [18], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [18]},
    {40, 31, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[3], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [19], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [19]},
    {40, 32, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[4], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [20], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [20]},
    {40, 33, BapFctCls_Property, 0x90, 0x6, BapDt_Int8,1 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [21], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [21]},
    {40, 34, BapFctCls_Property, 0x90, 0x6, BapDt_Int8,1 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [22], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [22]},
    {40, 36, BapFctCls_Method, 0x98, 0x9, BapDt_ByteSequence,40 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[5], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [23], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [23]},
    {40, 38, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[6], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [24], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [24]},
    {40, 39, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[7], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [25], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [25]},
    {40, 40, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[8], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [26], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [26]},
    {40, 43, BapFctCls_Method, 0x98, 0xc, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[9], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [27], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [27]},
    {40, 48, BapFctCls_Method, 0x98, 0xc, BapDt_Int8,1 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[10], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [28], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [28]},
    {41, 1, BapFctCls_Cache, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,80 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [29], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [29]},
    {41, 2, BapFctCls_Property, 0x91, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,6 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [30], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [30]},
    {41, 3, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [31], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [31]},
    {41, 4, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [32], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [32]},
    {41, 14, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [33], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [33]},
    {41, 15, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [34], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [34]},
    {41, 16, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [35], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [35]},
    {41, 17, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,4 , BapDt_FixedByteSequence,4 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [36], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [36]},
    {41, 18, BapFctCls_Array, 0x98, 0x6, BapDt_ByteSequence,5 , BapDt_ByteSequence,738 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [37], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [37]},
    {41, 19, BapFctCls_Array, 0x98, 0x6, BapDt_ByteSequence,5 , BapDt_ByteSequence,738 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [38], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [38]},
    {41, 20, BapFctCls_Array, 0x98, 0x6, BapDt_ByteSequence,5 , BapDt_ByteSequence,738 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [39], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [39]},
    {41, 22, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,3 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [40], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [40]},
    {41, 23, BapFctCls_Array, 0x98, 0x2, BapDt_ByteSequence,5 , BapDt_ByteSequence,1545 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [41], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [41]},
    {41, 24, BapFctCls_Method, 0x98, 0xc, BapDt_ByteSequence,51 , BapDt_FixedByteSequence,4 , NULL, 0, 0, 30, &BAP_BplRamTable[11], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [42], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [42]},
    {41, 25, BapFctCls_Property, 0x90, 0x6, BapDt_ByteSequence,40 , BapDt_ByteSequence,40 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [43], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [43]},
    {41, 26, BapFctCls_Method, 0x98, 0xe, BapDt_Void,0 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[12], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [44], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [44]},
    {41, 27, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,4 , BapDt_FixedByteSequence,4 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [45], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [45]},
    {41, 29, BapFctCls_Array, 0x98, 0x6, BapDt_ByteSequence,8 , BapDt_ByteSequence,947 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [46], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [46]},
    {41, 32, BapFctCls_Method, 0x98, 0xc, BapDt_ByteSequence,841 , BapDt_Void,0 , NULL, 0, 0, 30, &BAP_BplRamTable[13], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [47], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [47]},
    {43, 1, BapFctCls_Cache, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,44 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [48], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [48]},
    {43, 2, BapFctCls_Property, 0x91, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,6 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [49], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [49]},
    {43, 3, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [50], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [50]},
    {43, 4, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [51], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [51]},
    {43, 15, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [52], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [52]},
    {43, 16, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,2 , BapDt_FixedByteSequence,2 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [53], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [53]},
    {43, 17, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,2 , BapDt_FixedByteSequence,2 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [54], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [54]},
    {43, 18, BapFctCls_Property, 0x90, 0x6, BapDt_ByteSequence,8 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [55], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [55]},
    {43, 19, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,6 , BapDt_FixedByteSequence,6 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [56], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [56]},
    {43, 20, BapFctCls_Array, 0x90, 0x2, BapDt_ByteSequence,4 , BapDt_ByteSequence,685 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [57], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [57]},
    {43, 21, BapFctCls_Property, 0x90, 0x1, BapDt_Int8,1 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [58], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [58]},
    {43, 22, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,4 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [59], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [59]},
    {43, 23, BapFctCls_Property, 0x90, 0x1, BapDt_Int8,1 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [60], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [60]},
    {48, 1, BapFctCls_Cache, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,499 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [61], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [61]},
    {48, 2, BapFctCls_Property, 0x91, 0x2, BapDt_Void,0 , BapDt_FixedByteSequence,6 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [62], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [62]},
    {48, 3, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [63], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [63]},
    {48, 4, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [64], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [64]},
    {48, 14, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [65], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [65]},
    {48, 15, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [66], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [66]},
    {48, 16, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,8 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [67], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [67]},
    {48, 17, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,32 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [68], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [68]},
    {48, 20, BapFctCls_Property, 0x90, 0x6, BapDt_Int8,1 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [69], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [69]},
    {48, 22, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_Int8,1 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [70], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [70]},
    {48, 23, BapFctCls_Property, 0x90, 0x6, BapDt_FixedByteSequence,4 , BapDt_FixedByteSequence,4 , NULL, 1, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [71], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [71]},
    {48, 25, BapFctCls_Method, 0x98, 0xe, BapDt_FixedByteSequence,2 , BapDt_Int8,1 , NULL, 0, 0, 30, &BAP_BplRamTable[14], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [72], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [72]},
    {48, 26, BapFctCls_Method, 0x98, 0xe, BapDt_FixedByteSequence,2 , BapDt_Int8,1 , NULL, 0, 0, 30, &BAP_BplRamTable[15], 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [73], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [73]},
    {48, 30, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,151 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [74], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [74]},
    {48, 31, BapFctCls_Property, 0x90, 0x2, BapDt_Void,0 , BapDt_ByteSequence,273 , NULL, 0, 0, 0, NULL, 1, (BapBusIndirectionTxRomRow_pot)& BAP_BusIndTxTable [75], 1, (BapBusIndirectionRxRomRow_pot)& BAP_BusIndRxTable [75]}
};


BAP_ROM_DATA_FAR const BapBAPConfig_t BAP_ROM_CONST BAP_BAPConfigTable[]={
    /* PV-Major, PV-Minor, SG-Major, SG-Minor, DF-Major, DF-Minor */
    {3, 0, 40, 0, 3, 0},
    {3, 0, 41, 0, 3, 0},
    {3, 0, 43, 0, 3, 1},
    {3, 0, 47, 0, 3, 1}
};


/* Globale BAP ROM Tabelle ************************************************************/

BAP_ROM_DATA_FAR BapLsgRomRow_ot BAP_ROM_CONST BAP_LsgRomTables[]={
    /* LSG40 */
{
40,/*lsgId*/
#ifdef BAP_ASG
1,/* u8BAPConfigTableSize */
#endif
&BAP_BAPConfigTable [0], /* *poBAPConfigTable */
{  /* FunctionList */0x38,0x3,0xfb,0x7f,0xeb,0x90,0x80,0x0},
&BAP_BplHeartbeatTimerTable [0], /* pu16HeartbeatTimer */
10,/* u8TaskTimeMs */
10,/*u8Heartbeat*/
29, /* u8FctRomTableSize */
29, /* u16CanRxRomTableSize */
&BAP_FctRomTables [0], /* *poFctRomTable */
&BAP_CanRxRomTable [0], /* *poCanRxRomTable */
BAP_FALSE, /* bUseDLC8 */
#if defined(BAP_ASG) && defined(BAP_FSG)
BapSG_FSG, /*eSGType;*/
#endif /*#if defined(BAP_ASG) && defined(BAP_FSG)*/
&BAP_LsgRamTable [0], /* *poLsgRamRow */
29, /* u16CanTxRomTableSize */
&BAP_CanTxRomTable [0] /* *poCanTxRomTable */
},
    /* LSG41 */
{
41,/*lsgId*/
#ifdef BAP_ASG
1,/* u8BAPConfigTableSize */
#endif
&BAP_BAPConfigTable [1], /* *poBAPConfigTable */
{  /* FunctionList */0x38,0x3,0xfb,0xf4,0x80,0x0,0x0,0x0},
&BAP_BplHeartbeatTimerTable [1], /* pu16HeartbeatTimer */
10,/* u8TaskTimeMs */
10,/*u8Heartbeat*/
19, /* u8FctRomTableSize */
19, /* u16CanRxRomTableSize */
&BAP_FctRomTables [29], /* *poFctRomTable */
&BAP_CanRxRomTable [29], /* *poCanRxRomTable */
BAP_FALSE, /* bUseDLC8 */
#if defined(BAP_ASG) && defined(BAP_FSG)
BapSG_FSG, /*eSGType;*/
#endif /*#if defined(BAP_ASG) && defined(BAP_FSG)*/
&BAP_LsgRamTable [1], /* *poLsgRamRow */
19, /* u16CanTxRomTableSize */
&BAP_CanTxRomTable [29] /* *poCanTxRomTable */
},
    /* LSG43 */
{
43,/*lsgId*/
#ifdef BAP_ASG
1,/* u8BAPConfigTableSize */
#endif
&BAP_BAPConfigTable [2], /* *poBAPConfigTable */
{  /* FunctionList */0x38,0x1,0xff,0x0,0x0,0x0,0x0,0x0},
&BAP_BplHeartbeatTimerTable [2], /* pu16HeartbeatTimer */
10,/* u8TaskTimeMs */
10,/*u8Heartbeat*/
13, /* u8FctRomTableSize */
13, /* u16CanRxRomTableSize */
&BAP_FctRomTables [48], /* *poFctRomTable */
&BAP_CanRxRomTable [48], /* *poCanRxRomTable */
BAP_FALSE, /* bUseDLC8 */
#if defined(BAP_ASG) && defined(BAP_FSG)
BapSG_FSG, /*eSGType;*/
#endif /*#if defined(BAP_ASG) && defined(BAP_FSG)*/
&BAP_LsgRamTable [2], /* *poLsgRamRow */
13, /* u16CanTxRomTableSize */
&BAP_CanTxRomTable [48] /* *poCanTxRomTable */
},
    /* LSG48 */
{
48,/*lsgId*/
#ifdef BAP_ASG
1,/* u8BAPConfigTableSize */
#endif
&BAP_BAPConfigTable [3], /* *poBAPConfigTable */
{  /* FunctionList */0x38,0x3,0xcb,0x63,0x0,0x0,0x0,0x0},
&BAP_BplHeartbeatTimerTable [3], /* pu16HeartbeatTimer */
10,/* u8TaskTimeMs */
10,/*u8Heartbeat*/
15, /* u8FctRomTableSize */
15, /* u16CanRxRomTableSize */
&BAP_FctRomTables [61], /* *poFctRomTable */
&BAP_CanRxRomTable [61], /* *poCanRxRomTable */
BAP_FALSE, /* bUseDLC8 */
#if defined(BAP_ASG) && defined(BAP_FSG)
BapSG_FSG, /*eSGType;*/
#endif /*#if defined(BAP_ASG) && defined(BAP_FSG)*/
&BAP_LsgRamTable [3], /* *poLsgRamRow */
15, /* u16CanTxRomTableSize */
&BAP_CanTxRomTable [61] /* *poCanTxRomTable */
}
};


BAP_RAM_DATA_FAR BapLsgRomRow_pot BAP_pLsgRomTable[]={
    &BAP_LsgRomTables[0],
    &BAP_LsgRomTables[1],
    &BAP_LsgRomTables[2],
    &BAP_LsgRomTables[3]
};

