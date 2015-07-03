/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/bcl/bap_bclcan.c $
 * $Revision: 223 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Klaus Neubert, KOPF GmbH, D-76829 Landau-Moerzheim
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die CAN Anbindung der BCL Schicht im BAP
 *  Projekt.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2007-10-08   1.4.1.1     RVE         Workaround für die Bestimmung des canInterfaceChannels.
 * 2007-09-24   1.4.1.1     RVE         Änderung an der Initialisierung der ConfirmationFlags gemäß den Anforderungen von VW
 * 2006-11-20   1.4.1       ALA         Optimierung Laufzeit BAP_BCL_InitCanDrvTxConfirmationFlags
 *                                      Optimierung Verarbeitungszeit von Reset-Nachrichten
 * 2006-06-02   1.4         ALA         Unterstuetzung von Bap_Acknowledge
 * 2006-06-01   1.4         ALA         Optimierung Speicherplatzbedarf
 * 2006-06-01   1.4         ALA         Aufsplittung BAP_BCL_CanReceiveData in zwei Funktionen
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2006-04-12   1.4         ALA         Optimierungen fuer Task-Laufzeit.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer neue HB-Trigger.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer kompatible Erweiterung.
 * 2005-07-18   1.3.1       ALA         Bugfix: Bei ungueltiger BAPConfig werden keine Remotefehler mehr
 *                                      angenommen.
 * 2005-04-29   1.3         ALA         Verbesserte Parameteruebergabe, Reduzierung Stackbedarf, Optimierungen,
 *                                      Unterstuetzung von fixem und variablem DLC, einstellbar je LSG
 * 2005-04-21   1.3         ALA         Unterstuetzung von Fuellbotschaften, Robustheit gegenueber falschem DLC,
 *                                      Bugfix: Empfang von Fehlern nun moeglich fuer ByteSequence der Laenge 0.
 * 2005-04-15   1.3         ALA         Reduzierung der CPU-Last bei Busruhe
 * 2005-04-13   1.3         ALA         Reduzierung der CPU-Last bei Multi Konfigurationen im ASG
 * 2005-04-05   1.3         ALA         Verbesserte Heuristik, Ringpuffer zur Reduzierung der Interruptsperrzeiten,
 *                                      dynamische Kanalvergabe bei Segmentierung
 * 2005-03-14   1.2.1       CRI         Compilerwarning fuer gcc mit Konstruktion umgangen
 * 2004-11-08   1.2         ALA         Eigene Puffer fuer Errorcodes. Durch Opcodepuffer den
 *                                      Puffer fuer den BAP-Header eingespart => unveraenderter RAM-Bedarf.
 * 2004-10-26   1.2         ALA         Unterstuetzung von GetAll/StatusAll a Block und mit
 *                                      Einzelantworten auf BCL CAN Ebene.
 *                                      Komplette Verarbeitung von empfangenen Nachrichten refakturiert
 * 2004-09-09   1.2         ALA         Message-Objekte werden nun ueber CanGetTxDataPtr bzw.
 *                                      CanGetRxDataPtr ermittelt und fehlen in Konfiguration.
 *                                      UBS Unterstuetzung
 * 2004-09-17   1.1.1       ALA         Code zum Inkrementieren von 4 Bit Sequenzzaehler
 *                                      an CVI 6.0 Compiler angepasst
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Include stdlib.h eingefuegt
 * 2004-06-07   1.1         JOK         Makro fuer memcpy() mit entsprechenden
 *                                      Casts entsprechend MISRA-C Pruefung definiert
 * 2004-06-07   1.1         JOK         Komplett neuer Ablauf und Funktionen zum Senden
 *                                      einer Nachricht implementiert
 * 2004-06-07   1.1         JOK         Diverse kleinere Optimierungen beim Empfangen
 *                                      einer Nachricht implementiert
 * 2004-06-07   1.1         JOK         Nur noch BapSegType_None (unsegmentiert) und
 *                                      BapSegType_One (segmentiert) unterstuetzt
 * 2004-03-15   1.0         KNE         Initiale Version
 ******************************************************************/


/* Systemincludes mit <...> */
#include <stdlib.h>
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_canubs.h"
#include "bap_types.h"
#include "bap_bcl.h"
#include "bap_bclutil.h"
#include "bap_bclbus.h"
#include "bap_debug.h"
#include "bap_util.h"
#include "bap_bclconfig.h"

#ifdef BAP_RUNTIME_TEST
#include "sw_timer.h"
#endif /* BAP_RUNTIME_TEST */

#ifndef BAP_USES_CAN
#error Die mit BAPgen generierte Konfiguration verwendet kein CAN.
#endif

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/** Konstante fuer die Anzahl der Bytes eines Headers bei CAN */
#define CAN_BUFFER_HEADER_BYTESIZE            2u

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif /* MIN */


/* Interne Typ-Definitionen */


/**
 *  Parameterliste, die innerhalb der Verarbeitung von Daten aus dem Ringpuffer an Subroutinen
 *  uebergeben wird. Spart Stackspeicher und reduziert die Kopiervorgaenge.
 */
typedef struct BapRxParameters_t
{
    /**
     * oPars enthaelt folgende Parameter, welche im Fall von unsegmentieren Nachrichten
     * direkt weiter nach oben durchgereicht werden (ohne umzukopieren)
     * oPars.lsgId - Die LsgId, falls es sich um eine unsegmentierte Nachricht oder eine Startbotschaft handelt
     * oPars.poLsgRomRow - dto
     * oPars.fctId - Die FunctionId, falls es sich um eine unsegmentierte Nachricht oder eine Startbotschaft handelt
     * oPars.poFctRomRow - dto
     *
     * Die folgenden Parameter werden im folgenden im Aufrufstack modifiziert:
     * oPars.pData     - Zeigt auf die Nutzdaten, wird gesetzt vor dem Aufruf der Indication
     * oPars.eOpCode   - Der OpCode, falls es sich um eine unsegmentierte Nachricht oder eine
     *                   Startbotschaft handelt. Dieser wird nach oben gemeldet. Zum Melden des Empfangs
     *                   einer Startbotschaft an den BPL wird dessen Wert geaendert
     * oPars.u16Length - Laenge der Nutzdaten, diese Laenge hat nichts mit u8Length zu tun.
     */
    BapInternalParameters_ot oPars;

    /** Zeiger auf den CAN-Puffer */
    volatile_ptr_t pData;

    /** Laenge der empfangenen Nachricht, bei CAN 0..8 Bytes */
    uint8_t u8Length;

    /** Die CanId oder das CanMsgHandle des Vector-CAN-Treibers */
    BapCanMsgId_t canId;

#ifdef BAP_USES_SEGMENTATION
    /** Nur bei Startbotschaft: Die Laenge der segmentierten Nachricht */
    /** Wird nur bei segmentierten Nachrichten benutzt, nicht im SmallFSG */
    uint16_t u16SegmentationLength;
#endif /* #ifdef BAP_USES_SEGMENTATION */

    /** Der Segmentierungstyp, d.h. bei Segmentierung der Kanal, sonst Unsegmented */
    BapSegmentationType_et eSegmentationType;

    /** Maske fuer den Segmentierungstyp, d.h. bei Segmentierung der Kanal, sonst alle Kanaele */
    BapSegmentationType_et eSegmentationTypeMask;

    /** Beschreibt das CAN Interface, von dem die Nachricht empfangen wurde */
    BapCanInterfaceChannel_t canInterfaceChannel;

    /** Zeiger auf die zu pruefende Zeile der Datenfestlegung */
    BapCanRxRomRow_pot poCanRxRomRow;
} BapRxParameters_ot;

/**
 *  Zeiger zeigt auf ein Element des Typs @see BapRxParameters_t, welches auf dem Stack angelegt wurde.
 */
typedef DBGVAR struct BapRxParameters_t * BapRxParameters_pot;

#ifdef BAP_USES_SEGMENTATION
/**
 *  Zeiger zeigt auf ein Element des Typs @see BapRxParameters_t, welches auf dem Stack angelegt wurde,
 *  und innerhalb der Funktion nicht veraendert wird.
 */
typedef DBGVAR const struct BapRxParameters_t * BapRxParameters_cpot;
#endif /* #ifdef BAP_USES_SEGMENTATION */


/**
 *  Parameterliste, die innerhalb der Sendefunktionen haeufig uebergeben wird.
 *  Reduziert Kopiervorgaenge.
 */
typedef struct BapTxParameters_t
{
    /** der zum LSG/FCT gehoerige Zeiger auf den Eintrag in der CanTxRom-Tabelle. */
    BapCanTxRomRow_pot poCanTxRomRow;

#ifdef BAP_USES_SEGMENTATION
    /** entspricht poCanTxRomRow->poCanTxRamRow
     *  @remarks Optimierung der Dereferenzierungen
     */
    BapCanTxSegmentationRamRow_pot poCanTxRamRow;
#endif  /* #ifdef BAP_USES_SEGMENTATION */

    /** der zum LSG/FCT gehoerige Zeiger auf den Eintrag des RAM-Bereiches */
    BapBclTxRamRow_pot poBclTxRamRow;
} BapTxParameters_ot;

/**
 *  Zeiger zeigt auf ein Element des Typs @see BapTxParameters_t, welches auf dem Stack angelegt wurde.
 */
typedef DBGVAR struct BapTxParameters_t * BapTxParameters_pot;

/**
 *  Zeiger zeigt auf ein Element des Typs @see BapTxParameters_t, welches auf dem Stack angelegt wurde.
 */
typedef DBGVAR const struct BapTxParameters_t * BapTxParameters_cpot;

/**
 *  Funktionszeigertyp fuer Behandlung empfangener Daten
 */
typedef BAP_IMPL_FAR void (*RxHandler_ft)(const BapRxParameters_pot);

/* Interne Const Deklarationen */

/* Interne statische Variablen */

#ifdef BAP_USES_SEGMENTATION
/**
 * Konvertierung von Segmentierungstyp BapSegType_ChannelOne, BapSegType_ChannelTwo, BapSegType_ChannelThree, BapSegType_ChannelFour
 * in die Zahlenwerte 0x80, 0x90, 0xA0, 0xB0, wie sie im BAP Header kodiert sind.
 */
static const uint8_t garu8HeaderBySegType[9] =
{
    0, 0x80, 0x90, 0, 0xA0, 0, 0, 0, 0xB0
};
#endif /* #ifdef BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
/**
 * Konvertierung von Segmentierungstyp BapSegType_ChannelOne, BapSegType_ChannelTwo, BapSegType_ChannelThree, BapSegType_ChannelFour
 * in die Zahlenwerte 0, 1, 2, 3.
 */
static const uint8_t garu8ChannelBySegType[9] =
{
    0, 0, 1, 0, 2, 0, 0, 0, 3
};
#endif /* #ifdef BAP_USES_SEGMENTATION */

/* Vorwaerts-Deklarationen von statischen Funktionen */


#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_BCL_CanCopyTransmitSeg(BapTxParameters_cpot apoTxPars
    , DBGVAR uint8_t au8Header0
    , DBGVAR uint8_t au8Header1
    , DBGVAR uint8_t au8Len
    , DBGVAR uint16_t au16Offset);
#else /* BAP_USES_SEGMENTATION */
static BAP_IMPL_FAR void
BAP_BCL_CanCopyTransmitNonSeg(BapTxParameters_cpot apoTxPars
     , DBGVAR uint8_t au8Len);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR bool_t
BAP_BCL_CanSendStartMsg(BapTxParameters_cpot apoTxPars);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_BCL_CanSendSequenceMsg(BapTxParameters_cpot apoTxPars);
#endif /* BAP_USES_SEGMENTATION */

static BAP_IMPL_FAR bool_t
BAP_BCL_LowerLevelCanTxTask(BapTxParameters_cpot apoTxPars);

static BAP_IMPL_FAR bool_t
BAP_BCL_LowLevelCanTxTask(BapTxParameters_pot apoTxPars);

static BAP_IMPL_FAR void
BAP_CAN_AnalyzeHeaderBytes(const BapRxParameters_pot apoRxPars);

static BAP_IMPL_FAR void
BAP_CAN_RxNothing(const BapRxParameters_pot apoRxPars);

static BAP_IMPL_FAR void
BAP_CAN_RxUnsegmented(const BapRxParameters_pot apoRxPars);

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedCompleteCheck(const BapRxParameters_cpot apoRxPars);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedStart(const BapRxParameters_pot apoRxPars);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedSequence(const BapRxParameters_cpot apoRxPars);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR bool_t
BAP_BCL_CanReceiveSequence(const BapRxParameters_pot apoRxPars);
#endif /* BAP_USES_SEGMENTATION */

#ifdef BAP_USES_SEGMENTATION
static BapCanRxDynSegChannels_pot  BAP_IMPL_FAR
BAP_BCL_GetRxDynSegChannel(BapCanMsgId_t aCanId);
#endif /* BAP_USES_SEGMENTATION */

static BAP_IMPL_FAR bool_t
BAP_BCL_CanProcessData(BapRxParameters_pot apoRxPars, RxHandler_ft apfnRxHandler);

static BAP_IMPL_FAR void
BAP_BCL_CanReceiveData(BapCanRxBuffer_cpot apoCanRxBuffer);

#ifdef BAP_USES_ACKNOWLEDGE
static BAP_IMPL_FAR void
BAP_BCL_IfReqAcknowledge(BapTxParameters_cpot apoTxPars);
#else
#define BAP_BCL_IfReqAcknowledge(dummy) (void)(0)
#endif  /* #else #ifdef BAP_USES_ACKNOWLEDGE */


/* Definition (Implementierung) von statischen Funktionen */

#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Ruft BAP_BCL_Acknowledge auf, falls dies erforderlich ist
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 */
static BAP_IMPL_FAR void
BAP_BCL_IfReqAcknowledge(BapTxParameters_cpot apoTxPars)
{
    /*lint -esym(613,apoTxPars)*/
    BAP_ASSERT(apoTxPars);

    if (apoTxPars->poBclTxRamRow->flags.fTxRequestNeedsAckFlag)
    {
        apoTxPars->poBclTxRamRow->flags.fTxRequestNeedsAckFlag = BAP_FALSE;
        /* Acknowledge an BPL melden */
        BAP_BCL_Acknowledge(apoTxPars->poCanTxRomRow->poFctRomRow, apoTxPars->poBclTxRamRow->eOpCode);
    }
    return;
    /*lint +esym(613,apoTxPars)*/
}
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */


#ifdef BAP_USES_SEGMENTATION
/**
 *  Kopiert die zu sendenden Daten in einen Puffer und ruft dann die
 *  Transmit-Funktion der UBS-Schnittstelle auf. Diese Funktion hat zwei
 *  Auspraegungen, eine mit Header-Daten im Falle von Segmentierung
 *  und eine ohne Header-Daten falls keine Segmentierung per Compiler-Schalter
 *  aktiviert wurde.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 *  @param  au8Header0 das nullte Byte des Headers bei einer segmentierten Nachricht
 *
 *  @param  au8Header1 das erste Byte des Headers bei einer segmentierten Nachricht
 *
 *  @param  au8Len die Laenge der zu uebertragenden Daten
 *
 *  @param  au16Offset der Offset im BCL-Sendepuffer, ab dem die Daten aus dem
 *          BCL-Puffer in den CAN-Puffer kopiert werden.
 *
 *  @remarks Diese Funktion wird im Kombigeraet (ASG+FSG), ASG und LargeFSG verwendet.
 *  Im SmallFSG wird stattdessen @see BAP_BCL_CanCopyTransmitNonSeg verwendet
 */
static BAP_IMPL_FAR void
BAP_BCL_CanCopyTransmitSeg(BapTxParameters_cpot apoTxPars
    , DBGVAR uint8_t au8Header0
    , DBGVAR uint8_t au8Header1
    , DBGVAR uint8_t au8Len
    , DBGVAR uint16_t au16Offset)
{
    DBGVAR uint8_t aru8CanMsgObject[8];
    DBGVAR uint8_t u8UserDataLen;
    DBGVAR uint8_t u8CopyOffset;
    BapInternalParameters_ot oPars;

    /*lint -esym(613,apoTxPars)*/
    BAP_ASSERT(apoTxPars);

    oPars.poFctRomRow = apoTxPars->poCanTxRomRow->poFctRomRow;
    oPars.fctId = oPars.poFctRomRow->fctId;
    oPars.lsgId = oPars.poFctRomRow->lsgId;
    oPars.poLsgRomRow = BAP_GetLsgRomRow(oPars.lsgId);
#ifdef BAP_ASG
    oPars.eOpCode = BapOp_MessageSent;
#endif /* #ifdef BAP_ASG */
    oPars.pData = NULL;
    oPars.u16Length = 0;

    /* Initialisiere mit 00 falls DBGVAR genutzt wird */
#ifndef BAP_AUTO_DBGVAR
    MEMSET(aru8CanMsgObject, 0, 8u);
#endif /* BAP_AUTO_DBGVAR */

    /* -- Aufbereitung des BAP-Headers -- */

    /* Kopiere Header1 falls segmentierte Botschaft */
    if(au8Header0 & 0x80)
    {
        aru8CanMsgObject[0] = au8Header0;
        /* Kopiere Header2 falls Startbotschaft */
        if(!(au8Header0 & 0x40))
        {
            aru8CanMsgObject[1] = au8Header1;
            u8CopyOffset = 4u;  /* Startbotschaft hat 4 Bytes BAP Header */
        }
        else
        {
            u8CopyOffset = 1u;  /* Sequenzbotschaft hat 1 Byte BAP Header */
        }
    }
    else
    {
        u8CopyOffset = CAN_BUFFER_HEADER_BYTESIZE;      /* Unsegmentierte Nachricht hat 2 Bytes BAP Header */
    }

    /* Falls unsegmentierte Nachricht oder Startbotschaft, dann BAP Standard-Header hinzufuegen */
    if (u8CopyOffset > (uint8_t) 1u)
    {
        /* Kopiere LSG-ID, FCT-ID und OpCode in Sende-Puffer. */
        const uint8_t u8LsgId = BINARY_ID(oPars.lsgId);
        aru8CanMsgObject[u8CopyOffset-2] = (uint8_t) ((uint8_t)(((uint8_t)apoTxPars->poBclTxRamRow->eOpCode)<<4) | (uint8_t)(u8LsgId>>2));
        aru8CanMsgObject[u8CopyOffset-1] = (uint8_t) ((u8LsgId<<6) | BINARY_ID(oPars.fctId));
    }
    else
    {
        /* Bei Sequenzbotschaften ist kein zusaetzlicher Header erforderlich, da diese Informationen
           bereits aus der Startbotschaft bekannt sind. */
    }

    /* -- Aufbereitung der Nutzdaten -- */

    /* Kopiere Nutzdaten in den Puffer */
    MEMCPY(&aru8CanMsgObject[u8CopyOffset]
        , &apoTxPars->poBclTxRamRow->oBufferWithLength.aru8Buffer[au16Offset]
        , au8Len);

    if (oPars.poLsgRomRow->bUseDLC8)
    {
        /* Verwende feste DLC Laenge 8 */
        u8UserDataLen = 8;
    }
    else
    {
        /* Den Header auch zur Nutzdatenlaenge hinzuzaehlen */
        u8UserDataLen = (uint8_t) (au8Len + u8CopyOffset);
    }

    /* Initiiere Versenden der Nachricht */
    if (BAP_CANUBS_TxData(apoTxPars->poCanTxRomRow->canInterfaceChannel
            , apoTxPars->poCanTxRomRow->canMsgId
            , (ptr_t) aru8CanMsgObject
            , u8UserDataLen))
    {
        /* Nur falls segmentierte Nachricht */
        if ((uint8_t) CAN_BUFFER_HEADER_BYTESIZE != u8CopyOffset)
        {
            /* Setze Tx-InProgress Flag */
            apoTxPars->poBclTxRamRow->flags.fTxInProgressFlag = BAP_TRUE;
        }
        /* Restart Inhibit Timer */
        BAP_InhibitRamTable[apoTxPars->poCanTxRomRow->u8InhibitIndex].u16InhibitTimer
            = BAP_InhibitRomTable[apoTxPars->poCanTxRomRow->u8InhibitIndex].u16ConfiguredInhibitTime;

        /* Loesche Tx-Request Flag */
        apoTxPars->poBclTxRamRow->flags.fTxRequestFlag = BAP_FALSE;

        /* Etwas wurde erfolgreich versendet */
        BAP_DBG_TxEvent();

#ifdef BAP_ASG
        /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da weiter oben verworfen */
        /* Eine Nachricht wurde vom CAN-Treiber erfolgreich verschickt an BPL melden, wird zum retriggern der Retry-Timer benoetigt */
        BAP_BCL_DataReceived(&oPars);
#endif /* #ifdef BAP_ASG */

        /* Verschiebe Offset falls segmentierte Botschaft */
        if(au8Header0 & 0x80)
        {
            apoTxPars->poCanTxRamRow->u16BclTxBufferByteOffset += (uint16_t)au8Len;
            if(au8Header0 & 0x40)
            {
                /* Bei Sequenzbotschaften die Nummer nach dem Versenden erhoehen */
                apoTxPars->poCanTxRamRow->flags.fSequenceNumber
                    = (uint8_t) (((uint8_t) apoTxPars->poCanTxRamRow->flags.fSequenceNumber + 1) & 0x0f);
            }
            else
            {
                /* Melde DebugEvent falls Startbotschaft */
                BAP_DBG_TxStartMsgEvent(oPars.lsgId);
            }

            /* Senden beenden wenn alle Segmente erfolgreich versandt wurden */
            if(apoTxPars->poCanTxRamRow->u16BclTxBufferByteOffset
                >= apoTxPars->poBclTxRamRow->oBufferWithLength.u16Length)
            {
                BAP_DBG_TxSegMsgCompleteEvent(oPars.lsgId);

                apoTxPars->poBclTxRamRow->flags.fTxInProgressFlag = BAP_FALSE;

                BAP_BCL_IfReqAcknowledge(apoTxPars);
            }
        }
        else
        {
            BAP_BCL_IfReqAcknowledge(apoTxPars);
        }
    }
    else
    {
        /* Daten konnten nicht versendet werden.
           Beim naechsten Aufruf wird versucht die Botschaft erneut zu senden */
    }
    return;
    /*lint +esym(613,apoTxPars)*/
}

#else /* #ifdef BAP_USES_SEGMENTATION */

/**
 *  Kopiert die zu sendenden Daten in einen Puffer und ruft dann die
 *  Transmit-Funktion der UBS-Schnittstelle auf. Diese Funktion hat zwei
 *  Auspraegungen, eine mit Header-Daten im Falle von Segmentierung
 *  und eine ohne Header-Daten falls keine Segmentierung per Compiler-Schalter
 *  aktiviert wurde.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 *  @param  au8Len die Laenge der zu uebertragenden Daten
 *
 *  @remarks Diese Funktion wird nur im SmallFSG verwendet.
 *  Im Kombigeraet (ASG+FSG), ASG und LargeFSG  wird stattdessen @see BAP_BCL_CanCopyTransmitSeg verwendet
 */
static BAP_IMPL_FAR void
BAP_BCL_CanCopyTransmitNonSeg(BapTxParameters_cpot apoTxPars
     , DBGVAR uint8_t au8Len)
{
    /*lint -esym(613,apoTxPars)*/
    DBGVAR uint8_t aru8Buffer[8];
    BapFctRomRow_pot poFctRomRow = apoTxPars->poCanTxRomRow->poFctRomRow;
    DBGVAR fctId_t fctId = poFctRomRow->fctId;
    DBGVAR lsgId_t lsgId = poFctRomRow->lsgId;
    DBGVAR uint8_t u8UserDataLen;
    BapLsgRomRow_pot poLsgRomRow = BAP_GetLsgRomRow(lsgId);

    /* Begrenze die Laenge auf die von aru8Buffer, damit der Stack nicht ueberschrieben wird.
     */
    if (au8Len > (uint8_t)6u)
    {
        /* Schneide ggfs. Daten ab, es koennen in einer CAN Botschaft sowieso
           nur max. 6 Nutzdatenbytes + 2 Bytes BAP Header
           uebertragen werden. */
        au8Len = (uint8_t)6u;
    }
    else
    {
        /* nimm die uebergebene Laenge */
    }

    /* Initialisiere mit 00 falls DBGVAR genutzt wird */
#ifndef BAP_AUTO_DBGVAR
    MEMSET(aru8Buffer, 0, 8u);
#endif /* BAP_AUTO_DBGVAR */

    /* Kopiere LSG-ID, FCT-ID und OpCode in Sende-Puffer */
    aru8Buffer[0] = (uint8_t) ((((uint8_t)apoTxPars->poBclTxRamRow->eOpCode)<<4) | ((const uint8_t)lsgId/4));
    aru8Buffer[1] = ((uint8_t)((const uint8_t)lsgId<<6)) | ((const uint8_t)fctId);

    /* Fuelle Datenelement in den temporaeren UBS Puffer */
    MEMCPY(&aru8Buffer[CAN_BUFFER_HEADER_BYTESIZE], apoTxPars->poBclTxRamRow->oBufferWithLength.aru8Buffer, au8Len);

    BAP_ASSERT(poLsgRomRow != NULL);

    if (poLsgRomRow->bUseDLC8) /*lint !e613  nur wenn es das Steuergeraet gibt, wird diese Funktion aufgerufen */
    {
        /* Verwende feste DLC Laenge 8 (Unterstuetzung BAP 1.1) */
        u8UserDataLen = 8;
    }
    else
    {
        /* Den Header auch zur Nutzdatenlaenge hinzuzaehlen */
        u8UserDataLen = (uint8_t) (au8Len + (uint8_t) CAN_BUFFER_HEADER_BYTESIZE);
    }

    /* Initiiere das Versenden der CAN Message */
    if(BAP_CANUBS_TxData(apoTxPars->poCanTxRomRow->canInterfaceChannel
        , apoTxPars->poCanTxRomRow->canMsgId
        , aru8Buffer
        , u8UserDataLen))
    {
        /* Restart Inhibit Timer */
        BAP_InhibitRamTable[apoTxPars->poCanTxRomRow->u8InhibitIndex].u16InhibitTimer
            = BAP_InhibitRomTable[apoTxPars->poCanTxRomRow->u8InhibitIndex].u16ConfiguredInhibitTime;
        /* Loesche Tx-Request Flag */
        apoTxPars->poBclTxRamRow->flags.fTxRequestFlag = BAP_FALSE;

        BAP_BCL_IfReqAcknowledge(apoTxPars);

        /* Etwas wurde erfolgreich versendet */
        BAP_DBG_TxEvent();
    }
    else
    {
        /* Daten konnten nicht versendet werden.
           Beim naechsten Aufruf wird versucht die Botschaft erneut zu senden */
    }
    return;
    /*lint +esym(613,apoTxPars)*/
}
#endif /* #ifdef BAP_USES_SEGMENTATION /#else  */



#ifdef BAP_USES_SEGMENTATION
/**
 *  Initialisiert das Senden einer segmentierten Botschaft und beginnt mit der
 *  Uebertragung durch das Senden der Startbotschaft.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 *  @retval BAP_TRUE - Ein Kanal wurde reserviert und der Versuch unternommen,
 *  die Daten an den CAN-Treiber zu uebergeben.
 *
 *  @retval BAP_FALSE - Kein freier Segmentierungskanal vorhanden
 */
static BAP_IMPL_FAR bool_t
BAP_BCL_CanSendStartMsg(BapTxParameters_cpot apoTxPars)
{
    /*lint -esym(613,apoTxPars)*/
    DBGVAR uint16_t u16Length = apoTxPars->poBclTxRamRow->oBufferWithLength.u16Length;

    DBGVAR bool_t bSegmentationChannelIsFree = BAP_FALSE;

    /* Merkt sich, welche Funktion den aktuellen Segmentierungskanal verwendet */
    BapCanTxRomRow_pot * ppoSelectedCanTxRomRow = NULL;
    /*lint -esym(613,ppoSelectedCanTxRomRow)*/ /* Im folgenden Code ist sichergestellt dass nicht NULL */

    DBGVAR uint8_t u8SegmentationChannel;
    DBGVAR bool_t bRetVal = BAP_TRUE;

    /* insgesamt 2 mal soll die do-while-Schleife durchlaufen werden */
    DBGVAR uint8_t u8LoopCounter = 1;

    do
    {
        for (u8SegmentationChannel=0; !bSegmentationChannelIsFree && (u8SegmentationChannel<4); u8SegmentationChannel++)
        {
            apoTxPars->poCanTxRamRow->eSelectedSegmentationChannel = (BapSegmentationType_et) (1 << u8SegmentationChannel);

            /* Pruefen, ob Segmentierungskanal fuer diese Funktion zulaessig ist */
            if ( (uint8_t)(1 << u8SegmentationChannel) & (uint8_t)(apoTxPars->poCanTxRomRow->eSegmentationType) )
            {
                ppoSelectedCanTxRomRow = &apoTxPars->poCanTxRomRow->poTxDynSegChannels->arpoTxDynSegChannels[u8SegmentationChannel];

                /* 1. aeusserer Schleifendurchlauf */
                if (u8LoopCounter)
                {
                    /* Pruefe, ob ein reservierter Segmentierungskanal zur Verfuegung steht, */
                    /* Falls der Kanal durch die aktuelle Funktion belegt wurde, dann kann er wieder verwendet werden */
                    bSegmentationChannelIsFree = (bool_t) (*ppoSelectedCanTxRomRow == apoTxPars->poCanTxRomRow);
                }
                else
                /*  2. aeusserer Schleifendurchlauf */
                /*  Pruefe, ob ein freier Segmentierungskanal zur Verfuegung steht */
                if ( *ppoSelectedCanTxRomRow == NULL )
                {
                    /*  Falls ja, dann muss der Kanal auch frei sein */
                    bSegmentationChannelIsFree = BAP_TRUE;
                }
                else
                {
                    /* Ueberpruefe, ob alte Uebertragung bereits zu Ende, dann kann der Kanal auch wieder verwendet werden */
                    bSegmentationChannelIsFree
                        = (bool_t) !(
                            (*ppoSelectedCanTxRomRow)->poBclTxRamRow->flags.fTxRequestFlag
                            || (*ppoSelectedCanTxRomRow)->poBclTxRamRow->flags.fTxInProgressFlag
#ifdef BAP_FSG
                            /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im ASG TxError inaktiv */
                            || (*ppoSelectedCanTxRomRow)->poBclTxRamRow->flags.fTxErrorFlag
#endif /* #ifdef BAP_FSG */
                        );
                }
            }
        }
    }
    while (u8LoopCounter--);


    /* Versende Daten nur dann, wenn ein freier Segmentierungskanal gefunden wurde */
    if (!bSegmentationChannelIsFree)
    {
        apoTxPars->poCanTxRamRow->eSelectedSegmentationChannel = BapSegType_Unsegmented;
        bRetVal = BAP_FALSE;
    }
    else
    {
        DBGVAR uint8_t u8HeaderByte1;
        DBGVAR uint8_t u8HeaderByte2;

        /* Senden darf gestartet werden */

        /* Initialisiere Sequenzzaehler und Offsetzaehler */
        apoTxPars->poCanTxRamRow->flags.fSequenceNumber = 0;
        apoTxPars->poCanTxRamRow->u16BclTxBufferByteOffset = 0;

        /* Belege den Segmentierungskanal durch die aktuelle Funktion */
        *ppoSelectedCanTxRomRow = apoTxPars->poCanTxRomRow;

        /* Baue Header auf */
        u8HeaderByte1 = (uint8_t) (
                /* berechne das obere Nibble des Headers der StartMessage 1->80, 2->90, 4->A0, 8->B0 */
                + garu8HeaderBySegType[apoTxPars->poCanTxRamRow->eSelectedSegmentationChannel]
                + (uint8_t)((u16Length /*& 0xff00u implizit durch >>8 und cast */)>>8));

        u8HeaderByte2 = (uint8_t)(u16Length /*& 0x00ffu implizit durch (uint8_t) cast */);

        /* Kopiere die Daten in den CAN Puffer und versende sie */
        /* Daten haben bei Startbotschaft noch 2Bytes Header */
        BAP_BCL_CanCopyTransmitSeg(apoTxPars
            , u8HeaderByte1
            , u8HeaderByte2
            , (uint8_t) MIN(u16Length, (uint16_t) 4u)
            , (uint16_t) 0u);
    }

    return bRetVal;
    /*lint +esym(613,ppoSelectedCanTxRomRow)*/
    /*lint +esym(613,apoTxPars)*/
}
#endif /* BAP_USES_SEGMENTATION */


#ifdef BAP_USES_SEGMENTATION
/**
 *  Prueft, ob schon alle Segmente der segmentierten Botschaft versand wurden
 *  und setzt im Falle der vollstaendigen Botschaft die entsprechenden Flags.
 *  Stehen noch Teile der Botschaft aus, werden diese als Sequenzbotschaft
 *  verschickt.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 */
static BAP_IMPL_FAR void
BAP_BCL_CanSendSequenceMsg(BapTxParameters_cpot apoTxPars)
{
    DBGVAR uint16_t u16SequenceMessageMemcpyLength;

    /*lint -esym(613,apoTxPars)*/
    BAP_ASSERT(apoTxPars);

    /* Fuelle Folgedaten des Datenelements in das CAN Message
       Object fuer die Sequenzebotschaft und verwalte CanTxRam
       Daten */
    u16SequenceMessageMemcpyLength
        = (apoTxPars->poBclTxRamRow->oBufferWithLength.u16Length
        - apoTxPars->poCanTxRamRow->u16BclTxBufferByteOffset);

    if(u16SequenceMessageMemcpyLength >= 7u)
    {
        u16SequenceMessageMemcpyLength = 7u;
    }

    /* Kopiere die Daten in den CAN Puffer und versende sie */
    /* Daten haben bei Sequenzbotschaft noch 1Byte Header */
    BAP_BCL_CanCopyTransmitSeg(apoTxPars
        , (uint8_t)((uint8_t) 0x40 /* Kennung fuer Sequenzbotschaft */
          /* berechne das obere Nibble des Headers der StartMessage 1->00, 2->10, 4->20, 8->30 */
          + garu8HeaderBySegType[apoTxPars->poCanTxRamRow->eSelectedSegmentationChannel]
          + (uint8_t)(apoTxPars->poCanTxRamRow->flags.fSequenceNumber))
        , (uint8_t)0
        , (uint8_t)u16SequenceMessageMemcpyLength
        , apoTxPars->poCanTxRamRow->u16BclTxBufferByteOffset);

     return;
    /*lint +esym(613,apoTxPars)*/
}
#endif /* BAP_USES_SEGMENTATION */


/**
 *  Diese Funktion setzt Senden einzelner BCL Sendepuffer auf die UBS-Schnittstelle um.
 *
 *  Die Funktion wird vom BAP_BCL_LowLevlCanTxTask aufgerufen.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 *  @retval BAP_TRUE - Ein Eintragen in den CAN-Treiber wurde versucht
 *
 *  @retval BAP_FALSE - Der CAN-Treiber ist noch belegt (ConfirmationFlag) oder
 *  kein Segmentierungskanal frei.
 *
 *  @remarks:
 *  Fuer normale Uebertragung entspricht der Parameter poBclTxRamRow poCanTxRomRow->poBclTxRamRow.
 *  Werden jedoch Fehlerwerte gesendet, dann zeigt dieser Zeiger auf einen temporaeren Puffer.
 *
 */
static BAP_IMPL_FAR bool_t
BAP_BCL_LowerLevelCanTxTask(BapTxParameters_cpot apoTxPars)
{
    DBGVAR bool_t bRetVal = BAP_FALSE;
    /*lint -esym(613,apoTxPars)*/
    BAP_ASSERT(apoTxPars);

    /* Abbrechen wenn CAN Tx Handle bereits in Verwendung */
    if(BAP_CANUBS_IsReadyForTx(apoTxPars->poCanTxRomRow->canInterfaceChannel, apoTxPars->poCanTxRomRow->canMsgId))
    {
        bRetVal = BAP_TRUE;
#ifdef BAP_TASKTIME_LIMIT
        BAP_u16TaskTimeCounter += BAP_BUS_TX_TIME;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

#ifndef BAP_USES_SEGMENTATION
        /* Neuen Sendeauftrag verarbeiten */
        if(apoTxPars->poBclTxRamRow->flags.fTxRequestFlag)
        {
            BAP_BCL_CanCopyTransmitNonSeg(apoTxPars, (uint8_t) apoTxPars->poBclTxRamRow->oBufferWithLength.u16Length);
        }
#else  /* #ifndef BAP_USES_SEGMENTATION */
        /* Warte auf das Ende des Sendevorgangs */
        if(apoTxPars->poBclTxRamRow->flags.fTxInProgressFlag)
        {
            BAP_BCL_CanSendSequenceMsg(apoTxPars);
        }

        /* Neuen Sendeauftrag verarbeiten */
        if((apoTxPars->poBclTxRamRow->flags.fTxRequestFlag)
            && (!apoTxPars->poBclTxRamRow->flags.fTxInProgressFlag)
          )
        {
            if(BapSegType_Unsegmented != apoTxPars->poCanTxRomRow->eSegmentationType)
            {
                bRetVal = BAP_BCL_CanSendStartMsg(apoTxPars);
            }
            else
            {
                BAP_BCL_CanCopyTransmitSeg(apoTxPars, (uint8_t) 0u, (uint8_t) 0u
                    ,(uint8_t) MIN(apoTxPars->poBclTxRamRow->oBufferWithLength.u16Length, (uint16_t) 6u), (uint16_t) 0u);
            }
        }
#endif /* #else .. #ifndef BAP_USES_SEGMENTATION */
    }
    return bRetVal;
    /*lint +esym(613,apoTxPars)*/
}


/**
 *  Diese Funktion setzt Senden einzelner BCL Sendepuffer auf die UBS-Schnittstelle um.
 *
 *  Die Funktion wird vom BAP_BCL_TaskSend aufgerufen.
 *
 *  @param apoTxPars - Zeiger auf die Uebergabeparameterliste
 *
 *  @returns BAP_TRUE wenn ein Tx-Request gefunden wurde, BAP_FALSE falls nicht
 *
 *  @remarks:
 *  Diese Funktion macht die Unterscheidung, ob nun Datenwerte oder Fehlerwerte gesendet werden sollen.
 *
 *  Diese Funktion aendert apoTxPars->poBclTxRamRow, falls ein Fehler versendet wird
 *
 */
static BAP_IMPL_FAR bool_t
BAP_BCL_LowLevelCanTxTask(BapTxParameters_pot apoTxPars)
/*lint -esym(818,apoTxPars) */ /* Im ASG koennte apoTxPars const sein, im FSG jedoch nicht */
{
    DBGVAR bool_t bCanTxRequestFound = BAP_FALSE;

    /*lint -esym(613,apoTxPars) */
    BAP_ASSERT(apoTxPars);

#ifdef BAP_FSG
    /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im ASG TxError inaktiv */
    if ((apoTxPars->poBclTxRamRow->flags.fTxErrorFlag))
    {
        /* Nachfolgende Struktur ist ein Datenpuffer, der zum Versenden des Fehlerwertes genutzt wird */
        DBGVAR struct
        {
            /* Zustandsflags in Senderichtung */
            BapBclTxRamRowFlags_ot flags;

            /* ungenutzt */
            BapError_et eErrorCode;

            /* Sendepuffer fuer den zum Fehlerwert gehoerenden Opcode, konstant BapOp_PropError */
            BapOpCodes_et eOpCode;

            /* Sendepuffer fuer Fehlerwert */
            struct
            {
                /* Die Laenge des Fehlerwertes, konstant 1 */
                uint16_t u16Length;

                /* Enthaelt den Fehlerwert */
                uint8_t aru8Buffer[1];
            } oBufferWithLength;
        } oBclTxRamRow;

        /* Fehlerwert als Datenwert im eigenen RAM Puffer versenden */
        oBclTxRamRow.eErrorCode = apoTxPars->poBclTxRamRow->eErrorCode;
        oBclTxRamRow.oBufferWithLength.aru8Buffer[0] = (uint8_t) oBclTxRamRow.eErrorCode;
        oBclTxRamRow.eOpCode = BapOp_PropError;
        oBclTxRamRow.oBufferWithLength.u16Length = 1;
        oBclTxRamRow.flags.fTxRequestFlag = BAP_TRUE;
#ifdef BAP_USES_ACKNOWLEDGE
        oBclTxRamRow.flags.fTxRequestNeedsAckFlag = apoTxPars->poBclTxRamRow->flags.fTxErrorNeedsAckFlag;
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */
#ifdef BAP_USES_SEGMENTATION
        oBclTxRamRow.flags.fTxInProgressFlag = BAP_FALSE;
#endif /* BAP_USES_SEGMENTATION */

        /* Folgendes Cast erzeugt Lint-Meldung, ist aber OK */
        apoTxPars->poBclTxRamRow = (BapBclTxRamRow_pot)(void*)&oBclTxRamRow;
        /* Fehler senden */
        (void) BAP_BCL_LowerLevelCanTxTask(apoTxPars);

        /* erfolgreich */
        if (!oBclTxRamRow.flags.fTxRequestFlag)
        {
            /* dann TxError-Request loeschen */
            apoTxPars->poCanTxRomRow->poBclTxRamRow->flags.fTxErrorFlag = BAP_FALSE;
#ifdef BAP_USES_ACKNOWLEDGE
            apoTxPars->poCanTxRomRow->poBclTxRamRow->flags.fTxErrorNeedsAckFlag = BAP_FALSE;
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */
            bCanTxRequestFound = BAP_TRUE;
        }
        else
        {
            /* wird beim naechsten BAP_Task wiederholt */
        }
    }
    else
#endif /* BAP_FSG */
    if((apoTxPars->poBclTxRamRow->flags.fTxRequestFlag)
#ifdef BAP_USES_SEGMENTATION
        || (apoTxPars->poBclTxRamRow->flags.fTxInProgressFlag)
#endif /* BAP_USES_SEGMENTATION */
    )
    {
        /* Daten senden */
        bCanTxRequestFound = BAP_BCL_LowerLevelCanTxTask(apoTxPars);
    }
    else
    {
        /* es gab nichts zu versenden */
    }
    return bCanTxRequestFound;
    /*lint +esym(613,apoTxPars) */
}
/*lint +esym(818,apoTxPars) */


/**
 * Diese Funktion analysiert die Standard BAP-Header-Bytes und
 * speichert das Ergebnis in apoRxPars als lsgId, fctId und OpCode
 *
 * @param apoRxPars: Eingabe: pData, Ausgabe: lsgId, fctId, eOpCode
 */
static BAP_IMPL_FAR void
BAP_CAN_AnalyzeHeaderBytes(const BapRxParameters_pot apoRxPars)
{
    /*lint -esym(613,apoRxPars)*/
    DBGVAR const volatile_ptr_t pData = apoRxPars->pData;
    /* LsgId extrahieren */
    apoRxPars->oPars.lsgId = (lsgId_t) (( (pData[0] & 0x0f) << 2) | (((uint8_t) pData[1]) >> 6));
    /* FctId extrahieren */
    apoRxPars->oPars.fctId = (fctId_t) (pData[1] & 0x3f);
    /* OpCode extrahieren */
    apoRxPars->oPars.eOpCode = (BapOpCodes_et)((pData[0] & 0x70 /*0b01110000*/ ) >> 4);
    return;
    /*lint +esym(613,apoRxPars)*/
}

/**
 * Diese Funktion verwirft die empfangenen Daten. Sie wird aufgerufen,
 * falls segmentierte Daten empfangen werden, aber keine Segmentierung
 * unterstuetzt wird.
 *
 * @param apoRxPars: Parameter wird nicht genutzt.
 *
 * @remarks: Schnittstelle entspricht RxHandler_t
 */
static BAP_IMPL_FAR void
BAP_CAN_RxNothing(const BapRxParameters_pot apoRxPars)
{
    /* Nutzloser Code, damit die Prueftools Ruhe geben */
    (void) apoRxPars;
    return;
}


/**
 * Diese Funktion verarbeitet unsegmentierte, empfangene Daten.
 *
 * @param apoRxPars: Enthaelt alle Eingangsparameter. Siehe Beschreibung des Datentyps.
 *
 * @remarks: Schnittstelle entspricht RxHandler_t
 */
static BAP_IMPL_FAR void
BAP_CAN_RxUnsegmented(const BapRxParameters_pot apoRxPars)
{
    BapCanRxRomRow_pot  poCanRxRomRow;
    BapInternalParameters_pot poPars;
    DBGVAR uint16_t u16Length;
    bool_t bLengthCheck;

    /*lint -esym(613,apoRxPars) */
    BAP_ASSERT(apoRxPars);

    poCanRxRomRow = apoRxPars->poCanRxRomRow;

#ifdef BAP_RUNTIME_TEST
    BAP_RUNTIME_TEST_INDINT8_START();
#endif /* BAP_RUNTIME_TEST */

    poPars = &apoRxPars->oPars;
    poPars->pData = (ptr_t) &(apoRxPars->pData[2]);
    u16Length = (uint16_t)apoRxPars->u8Length-CAN_BUFFER_HEADER_BYTESIZE;
    bLengthCheck = BAP_TRUE;

#ifdef BAP_ASG
    /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG kein PropError in Rx Richtung */
    /* Speichere Fehler in speziellem Puffer */
    if ( (BapOp_PropError == poPars->eOpCode)
#if defined(BAP_ASG) && defined(BAP_FSG)
          && (BapSG_ASG == poPars->poLsgRomRow->eSGType)
#endif /* #if defined(BAP_ASG) && defined(BAP_FSG) */
       )
    {
        bLengthCheck = (bool_t) (u16Length>=(uint16_t)1);
        if ( (bLengthCheck)
#ifdef BAP_ASG
        /* Im WaitForConfig-Zustand keine Remote-Fehler annehmen */
            && (BapLayerLsgStat_Running == poPars->poLsgRomRow->poLsgRamRow->eLsgStatus)
#endif /* #ifdef BAP_ASG */
           )
        {
            BAP_BCL_ErrorIndication(poPars->lsgId, poPars->fctId, (BapError_et) *poPars->pData);
#ifdef BAP_TASKTIME_LIMIT
            BAP_u16TaskTimeCounter += BAP_IND_RX_TIME + sizeof(BapError_et);
#endif /* #ifdef BAP_TASKTIME_LIMIT */
        }
    }
    else
#endif /* #ifdef BAP_ASG */
    {
        DBGVAR uint16_t u16CopyLength;
        poPars->u16Length = poCanRxRomRow->u16Size;
        u16CopyLength = MIN(poPars->u16Length, (uint16_t) 6);
        bLengthCheck  /* Datenlose Requests durchlassen (BapOp_PropGet, BapOp_ArrGet, BapOp_CacheGetAll, BapOp_MethAbort */
            = (bool_t)((u16CopyLength<=u16Length) || (BapOp_PropGet == poPars->eOpCode));

        if (bLengthCheck)
        {
            /* Falls FixedByteSequence groesser als die Datenmenge im CAN-Puffer, dann
               kopiere Daten um, damit die Laenge mit 0 Bytes aufgefuellt ist */
            if ( (BapDt_FixedByteSequence == poPars->poFctRomRow->eRxDataType)
                 && (poPars->u16Length>(uint16_t)6) )
            {
                poPars->pData = (ptr_t) poCanRxRomRow->poBclRxRamRow->oBufferWithLength.aru8Buffer;
                MEMCPY(poPars->pData, &apoRxPars->pData[2], u16CopyLength);
            }

            BAP_BCL_DataReceived(poPars);
#ifdef BAP_TASKTIME_LIMIT
            BAP_u16TaskTimeCounter += BAP_IND_RX_TIME + poPars->u16Length;
#endif /* #ifdef BAP_TASKTIME_LIMIT */
        }
    }

    if (!bLengthCheck)
    {
        BAP_BCL_ErrorIndication(poPars->lsgId, poPars->fctId, BapErr_BadDataLength);
        BAP_DBG_BadDataLengthError( poPars->lsgId );
    }

#ifdef BAP_TASKTIME_LIMIT
    poPars->poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_TRUE;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

#ifdef BAP_RUNTIME_TEST
    BAP_RUNTIME_TEST_INDINT8_STOP();
#endif /* BAP_RUNTIME_TEST */

    return;
    /*lint +esym(613,apoRxPars) */
}


#ifdef BAP_USES_SEGMENTATION
/**
 * Diese Funktion prueft, ob nach dem Empfang einer segmentierten Nachricht
 * diese vollstaendig empfangen wurde und setzt entsprechende Notifizierungsflags.
 *
 * @param apoRxPars: Enthaelt alle Eingangsparameter. Siehe Beschreibung des Datentyps.
 *
 * @remarks: Schnittstelle entspricht RxHandler_t
 */
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedCompleteCheck(const BapRxParameters_cpot apoRxPars)
{
    /*lint -esym(613,apoRxPars) */
    const BapCanRxRomRow_pot poCanRxRomRow = apoRxPars->poCanRxRomRow;
    BapBclRxRamRow_pot poBclRxRamRow = poCanRxRomRow->poBclRxRamRow;
    DBGVAR lsgId_t lsgId = poCanRxRomRow->lsgId;
    BapLsgRomRow_pot poLsgRomRow = BAP_GetLsgRomRow(lsgId);
    uint16_t u16IntertelegramTime;
    DBGVAR uint8_t u8SegChannel;
    BapCanRxDynSegChannels_pot  poCanRxDynSegChannel;

    if (!(poCanRxRomRow->poCanRxRamRow))
    {
        /* tritt nur dann auf, wenn Sequenzbotschaften einer nicht konfigurierten Fct-Id */
        /* empfangen wurden. Wird aber zum Retriggern benoetigt */
    }
    else if ( (poCanRxRomRow->poCanRxRamRow->u16BclRxBufferByteOffset
                >= poBclRxRamRow->oBufferWithLength.u16Length)
              && poBclRxRamRow->flags.fRxInProgressFlag
            )
    {
        poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;
        poBclRxRamRow->flags.fRxCompleteFlag = BAP_TRUE;

#ifdef BAP_TASKTIME_LIMIT
        if (poLsgRomRow)
        {
            poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_TRUE;
        }
#endif /* #ifdef BAP_TASKTIME_LIMIT */

        /* InterTelegramTimer auf 0 setzen */
        *(poCanRxRomRow->pu16InterTelegramTimer) = 0;
        u8SegChannel = garu8ChannelBySegType[apoRxPars->eSegmentationType];

        /* Suche nach dem Can-Handle, ob es einen Eintrag fuer den gewaehlten Segmentierungskanal gibt */
        poCanRxDynSegChannel = BAP_BCL_GetRxDynSegChannel(apoRxPars->canId);

        /* gib den Empfangskanal wieder frei */
        if (NULL != poCanRxDynSegChannel)
        {
            poCanRxDynSegChannel->arpoRxDynSegChannels[u8SegChannel] = NULL;
        }

        BAP_DBG_RxSegMsgCompleteEvent(lsgId);
    }
#ifdef BAP_ASG
    /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG RxError inaktiv */
    else if (poBclRxRamRow->flags.fRxErrorFlag
        && !poBclRxRamRow->flags.fRxInProgressFlag)
    {

#ifdef BAP_TASKTIME_LIMIT
        if (poLsgRomRow)
        {
            poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_TRUE;
        }
#endif /* #ifdef BAP_TASKTIME_LIMIT */

        /* InterTelegramTimer auf 0 setzen */
        *(poCanRxRomRow->pu16InterTelegramTimer) = 0;

        BAP_DBG_RxSegMsgCompleteEvent(lsgId);
    }
#endif /* #ifdef BAP_ASG */
    else if (poBclRxRamRow->flags.fRxInProgressFlag)
    {
        /* InterTelegramTimer neu starten (retriggern) */
        u16IntertelegramTime = poCanRxRomRow->u16IntertelegramTime;
        *(poCanRxRomRow->pu16InterTelegramTimer) = u16IntertelegramTime;
        BAP_bInterTelegramTimerRunning = BAP_TRUE;
    }
    else
    {
            /* Misra Rule 60 */
    }
    return;
    /*lint -esym(613,apoRxPars) */
}
#endif /* BAP_USES_SEGMENTATION */


#ifdef BAP_USES_SEGMENTATION
/**
 * Diese Funktion verarbeitet eine Startbotschaft einer empfangenen, segmentierten Nachricht.
 * Alte, laufende Uebertragungen werden ohne Fehlermeldung abgebrochen.
 *
 * @param apoRxPars: Enthaelt alle Eingangsparameter. Siehe Beschreibung des Datentyps.
 *
 * @remarks: Schnittstelle entspricht RxHandler_t
 */
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedStart(const BapRxParameters_pot apoRxPars)
{
    /*lint -esym(613,apoRxPars) */
    BapCanRxRomRow_pot  const poCanRxRomRow = apoRxPars->poCanRxRomRow;
    BapBclRxRamRow_pot poBclRxRamRow = poCanRxRomRow->poBclRxRamRow;
    BapInternalParameters_pot poPars;
    DBGVAR BapOpCodes_et eOpCode;

    poPars = &apoRxPars->oPars;

    /* OpCode, welcher empfangen wurde, retten */
    eOpCode = poPars->eOpCode;
    poPars->eOpCode = BapOp_StartMessage;


    if (    (   /* Pruefe, ob Laenge i.O. */
                (apoRxPars->u16SegmentationLength > poCanRxRomRow->u16Size)

                /* Bei Errors muss die Laenge 1 betragen */
                && ((apoRxPars->u16SegmentationLength != (uint16_t)1) || (BapOp_PropError != eOpCode))

                /* Beim ASG darf bei kompatibler Erweiterung die Laenge der StatusAll Nachricht */
                /* groesser sein wie konfiguriert */
#ifdef BAP_ASG
                && (

#if defined(BAP_ASG) && defined(BAP_FSG)
                    (poPars->poLsgRomRow->eSGType != BapSG_ASG) ||  /** @todo: Diskutieren, ob noetig, da kein SetGetAll. */
#endif  /* #if defined(BAP_ASG) && defined(BAP_FSG) */
                    (BAP_FCTID_GETALL != poPars->fctId)
                )
#endif  /* #ifdef BAP_ASG */
            )
        )
    {
        /* Falls nein */

        /*  Die folgende Anweisung asynchron im naechsten Task an die Applikation senden.
         *  BAP_BCL_ErrorIndication(apoRxPars->lsgId, apoRxPars->fctId, BapErr_OversizeSegmentation);
         */
        if ( !poBclRxRamRow->flags.fRxErrorFlag )   /* Remote-Fehler haben Prioritaet vor internem */
        {
            poBclRxRamRow->flags.fRxErrorFlag = BAP_TRUE;
            poBclRxRamRow->eErrorCode = BapErr_OversizeSegmentation;
        }

        BAP_DBG_MsgOversizeError(poPars->lsgId);
    }
    else
    {
        /* Alte Segmentierung abbrechen */
        poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;

#ifdef BAP_ASG
        /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG Rx ErrorOpcode verboten */
        /* Speichere Fehler in speziellem Puffer */
        if ( (BapOp_PropError == eOpCode)
#if defined(BAP_ASG) && defined(BAP_FSG)
              && (BapSG_ASG == poPars->poLsgRomRow->eSGType)
#endif /* #if defined(BAP_ASG) && defined(BAP_FSG) */
           )
        {
            /* Im WaitForConfig-Zustand keine Remote-Fehler annehmen */
            if (BapLayerLsgStat_Running == poPars->poLsgRomRow->poLsgRamRow->eLsgStatus)
            {
                poBclRxRamRow->flags.fRxErrorFlag = BAP_TRUE;

                if ( (apoRxPars->u8Length<(uint8_t)5u) || (apoRxPars->u16SegmentationLength!=(uint16_t)1) )
                {
                    BAP_DBG_BadDataLengthError( poPars->lsgId );
                    poBclRxRamRow->eErrorCode = BapErr_BadDataLength;
                }
                else
                {
                    poBclRxRamRow->eErrorCode = (BapError_et) apoRxPars->pData[2];
                }
            }
        }
        else
#endif /* #ifdef BAP_ASG */
        {
            BapCanRxSegmentationRamRow_pot poCanRxRamRow = poCanRxRomRow->poCanRxRamRow;
            /* Berechne aus der Bitkodierung des Segmentationstyps den Segmentierungskanal:
               Konvertiere 1->0, 2->1, 4->2, 8->3, entspricht lg2 */
            DBGVAR uint8_t u8SegChannel = garu8ChannelBySegType[apoRxPars->eSegmentationType];

            poCanRxRamRow->flags.fSequenceNumber = 0;
            poBclRxRamRow->eOpCode = eOpCode;

#ifdef BAP_ASG
            /* Spezialbehandlung wegen StatusAll bei kompatibler Erweiterung */
            if (apoRxPars->u16SegmentationLength > poCanRxRomRow->u16Size)
            {
                apoRxPars->u16SegmentationLength = poCanRxRomRow->u16Size;
                /* dennoch den Fehler anzeigen */
                BAP_DBG_MsgOversizeError(poPars->lsgId);
            }
#endif /* #ifdef BAP_ASG */

            poBclRxRamRow->oBufferWithLength.u16Length = apoRxPars->u16SegmentationLength;

            /* Offset als zu kopierende Laenge verwenden, muesste sonst nach memcpy gesetzt werden */
            poCanRxRamRow->u16BclRxBufferByteOffset
                = MIN(apoRxPars->u16SegmentationLength, (uint16_t)4u);

            if (poCanRxRamRow->u16BclRxBufferByteOffset <= (uint16_t)((uint16_t)apoRxPars->u8Length-(uint16_t)4)) /* 4 Bytes Segmentierungs-Header */
            {
                /* Kopiere die Daten Byte 4 .. max. zu Byte 8 aus dem CAN-Treiber in den BCL-Puffer */
                MEMCPY( poBclRxRamRow->oBufferWithLength.aru8Buffer
                    , &apoRxPars->pData[2]   /* weist noch auf Byte 2 des Puffers wg. Optimierung. Dies ist so gewollt. */
                    , poCanRxRamRow->u16BclRxBufferByteOffset);

                /* Merke den aktiven Segmentierungskanal fuer schnelles Einsortieren der Sequenzbotschaften */
                poCanRxRomRow->poRxDynSegChannels->arpoRxDynSegChannels[u8SegChannel] = poCanRxRomRow;

                poBclRxRamRow->flags.fRxInProgressFlag = BAP_TRUE;
            }
            else
            {
#ifdef BAP_ASG
                /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG RxError inaktiv */
                if ( !poBclRxRamRow->flags.fRxErrorFlag )   /* Remote-Fehler haben Prioritaet vor internem */
#endif /* BAP_ASG */
                {
                    BAP_DBG_BadDataLengthError( poPars->lsgId );
                    poBclRxRamRow->eErrorCode = BapErr_BadDataLength;
                    poBclRxRamRow->flags.fRxErrorFlag = BAP_TRUE;
                }
            }
        }
        BAP_DBG_RxStartMsgEvent(poPars->lsgId);
        /* BPL ueber den Empfang einer Startbotschaft informieren zwecks Retry-Mechanismus */
        BAP_BCL_DataReceived(poPars);
    }
    return;
    /*lint +esym(613,apoRxPars) */
}
#endif /* BAP_USES_SEGMENTATION */


#ifdef BAP_USES_SEGMENTATION
/**
 * Diese Funktion verarbeitet eine Sequenzbotschaft einer empfangenen, segmentierten Nachricht.
 * Hierbei werden Intertelegrammtimer, Sequenzfehler usw. ueberprueft.
 *
 * @param apoRxPars: Enthaelt alle Eingangsparameter. Siehe Beschreibung des Datentyps.
 *
 * @remarks: Schnittstelle entspricht RxHandler_t
 */
static BAP_IMPL_FAR void
BAP_CAN_RxSegmentedSequence(const BapRxParameters_cpot apoRxPars)
{
    /*lint -esym(613,apoRxPars) */
    BapCanRxRomRow_pot  const poCanRxRomRow = apoRxPars->poCanRxRomRow;
    BapBclRxRamRow_pot poBclRxRamRow = poCanRxRomRow->poBclRxRamRow;
    BapCanRxSegmentationRamRow_pot poCanRxRamRow = poCanRxRomRow->poCanRxRamRow;

    if( (apoRxPars->canId == poCanRxRomRow->canMsgId)
        &&  (poCanRxRomRow->canInterfaceChannel == apoRxPars->canInterfaceChannel) )
    {
        DBGVAR uint16_t * const pu16BclRxBufferByteOffset = &poCanRxRamRow->u16BclRxBufferByteOffset;

        /* Sequenzbotschaft ohne Startbotschaft? */
        /* IntertelegrammTimer abgelaufen? */
        if ( (!poBclRxRamRow->flags.fRxInProgressFlag)
            || (0u == *(poCanRxRomRow->pu16InterTelegramTimer)) )
        {
            poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;

            /* In BAP 1.4 wird kein BapErr_IllegalSequence mehr an die Applikation gemeldet,
               sondern die Nachricht stillschweigend ignoriert */
        }
        /* Sequenznummerfehler aufgetreten? */
        else if( (uint8_t) ((apoRxPars->pData[0]) & 0x0f)
            != (uint8_t) (poCanRxRamRow->flags.fSequenceNumber))
        {
            /*  Die folgende Anweisung asynchron im naechsten Task an die Applikation senden.
             *  BAP_BCL_ErrorIndication(poCanRxRomRow->lsgId, poCanRxRomRow->fctId, BapErr_SequenceNumber);
             */
#ifdef BAP_ASG
            /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG RxError inaktiv */
            if ( !poBclRxRamRow->flags.fRxErrorFlag )   /* Remote-Fehler haben Prioritaet vor internem */
#endif /* BAP_ASG */
            {
                poBclRxRamRow->flags.fRxErrorFlag = BAP_TRUE;
                poBclRxRamRow->eErrorCode = BapErr_SequenceNumber;
            }

            BAP_DBG_SequenceError(poCanRxRomRow->lsgId);

            /* InterTelegramTimer auf 0 setzen */
            *(poCanRxRomRow->pu16InterTelegramTimer) = 0;

            poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;
        }
        else
        {
            /* Extrahiere Folgedaten des Datenelements aus dem CAN
               Message Object fuer die Sequenzebotschaft und verwalte
               CanRxRam Daten */
            DBGVAR uint16_t u16SequenceMessageMemcpyLength =
                MIN((uint16_t)7u, (uint16_t)(poBclRxRamRow->oBufferWithLength.u16Length
                         - *pu16BclRxBufferByteOffset));

            if ( u16SequenceMessageMemcpyLength <= (uint16_t)((uint16_t)apoRxPars->u8Length-1u /* 1 Byte Header fuer Sequenzbotschaft */) )
            {
                MEMCPY( &poBclRxRamRow->oBufferWithLength.aru8Buffer[*pu16BclRxBufferByteOffset]
                    , &apoRxPars->pData[1]
                    , u16SequenceMessageMemcpyLength);

                *pu16BclRxBufferByteOffset += u16SequenceMessageMemcpyLength;
            }
            else
            {
#ifdef BAP_ASG
                /* Keine explizite Fallunterscheidung fuer das Kombigeraet, da im FSG RxError inaktiv */
                if ( !poBclRxRamRow->flags.fRxErrorFlag )   /* Remote-Fehler haben Prioritaet vor internem */
#endif /* BAP_ASG */
                {
                    BAP_DBG_BadDataLengthError( apoRxPars->oPars.lsgId );
                    poBclRxRamRow->eErrorCode = BapErr_BadDataLength;
                    poBclRxRamRow->flags.fRxErrorFlag = BAP_TRUE;
                }

                /* InterTelegramTimer auf 0 setzen */
                *(poCanRxRomRow->pu16InterTelegramTimer) = 0;

                poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;
            }

            /* Sequenznummer hochzaehlen (4 Bits) */
            poCanRxRamRow->flags.fSequenceNumber = (poCanRxRamRow->flags.fSequenceNumber + 1u) & 15u;
        }
    }
    return;
    /*lint +esym(613,apoRxPars) */
}
#endif /* BAP_USES_SEGMENTATION */


#ifdef BAP_USES_SEGMENTATION
/**
 *  Hole die zur CanId gehoerenden dynamischen Segmentierungskanaele
 *
 *  @param aCanId can id zu der die Segmentierungskanaele gesucht werden sollen
 */
static BapCanRxDynSegChannels_pot  BAP_IMPL_FAR
BAP_BCL_GetRxDynSegChannel(BapCanMsgId_t aCanId)
{
    DBGVAR uint16_t u16CanHandleCounter;
    BapCanRxDynSegChannels_pot  poCanRxDynSegChannel = NULL;

    /* Suche nach dem Can-Handle, ob es einen Eintrag fuer den gewaehlten Segmentierungskanal gibt */
    for ( u16CanHandleCounter = 0; (u16CanHandleCounter<BAP_CAN_RX_SEGMENTATION_CHANNELS) && !poCanRxDynSegChannel; u16CanHandleCounter++)
    {
        /* Falls CAN-Id gefunden, pruefe ob Segmentierungskanal vergeben */
        if ( BAP_CanRxMsgIdToSegChannelMappingTable[u16CanHandleCounter] == aCanId )
        {
            poCanRxDynSegChannel = &BAP_CanRxSegmentationChannels[u16CanHandleCounter];
        }
    }
    return poCanRxDynSegChannel;
}
#endif

#ifdef BAP_USES_SEGMENTATION
/**
 *  Diese Funktion verarbeitet eine Sequenzbotschaft optimiert.
 *
 *  @param apoRxPars - modifiziert folgende Elemente
 *  apoRxPars->poLsgRomRow zeigt nach dem Funktionsaufruf auf das Lsg, welchem die Nachricht zugeordnet wurde.
 *  apoRxPars->poCanRxRomRow zeigt nach dem Funktionsaufruf auf die Funktion, welcher die Nachricht zugeordnet wurde.
 *
 *  @returns
 *  BAP_TRUE, falls die Nachricht verarbeitet wurde
 *  BAP_FALSE, falls die Nachricht verworfen wurde
 */
static BAP_IMPL_FAR bool_t
BAP_BCL_CanReceiveSequence(const BapRxParameters_pot apoRxPars)
{
    /*lint -esym(613,apoRxPars) */ /* wird nur durch BAP_BCL_CanReceiveData aufgerufen, wo benutzte Pars gesetzt werden */
    /* Optimierung fuer Sequenzbotschaften */
    DBGVAR bool_t bDataProcessed = BAP_FALSE;

    DBGVAR uint8_t u8SegChannel = garu8ChannelBySegType[apoRxPars->eSegmentationType];

    /* Suche nach dem Can-Handle, ob es einen Eintrag fuer den gewaehlten Segmentierungskanal gibt */
    BapCanRxDynSegChannels_pot  poCanRxDynSegChannel = BAP_BCL_GetRxDynSegChannel(apoRxPars->canId);

    /* Falls CAN-Id gefunden, pruefe ob Segmentierungskanal vergeben */
    if (NULL != poCanRxDynSegChannel)
    {
        apoRxPars->oPars.poLsgRomRow = NULL;
        apoRxPars->poCanRxRomRow = poCanRxDynSegChannel->arpoRxDynSegChannels[u8SegChannel];
        if (apoRxPars->poCanRxRomRow != NULL)
        {
            apoRxPars->oPars.poLsgRomRow = BAP_GetLsgRomRow(apoRxPars->poCanRxRomRow->lsgId);
        }

        if ( (NULL != apoRxPars->oPars.poLsgRomRow)
             && ( (apoRxPars->oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running)
#ifdef BAP_ASG  /* Im Kombigeraet gibt es WaitForConfig nicht fuer FSGs */
                  || (apoRxPars->oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_WaitForConfig)
        /* keine Spezialbehandlung fuer kompatible erweiterte Funktionsliste noetig, da in diesem
           Fall BAP_CAN_RxSegmentedSequence die Eingabe ignoriert (Vergleich der Segmentierungskanaele) */
#endif /* #ifdef BAP_ASG */
               )
           )
        {
            BAP_CAN_RxSegmentedSequence(apoRxPars);
            bDataProcessed = BAP_TRUE;
        }
    }
    /*lint +esym(613,apoRxPars) */
    return bDataProcessed;
}
#endif /* #ifdef BAP_USES_SEGMENTATION */


/**
 *  Diese Funktion verarbeitet Startbotschaften und unsegmentierte Nachrichten.
 *
 *  @param apoRxPars enthaelt die Empfangsdaten, welche weiterverarbeitet werden.
 *  @param apfnRxHandler enthaelt die Verarbeitungsfunktion, welche aufgerufen werden soll.
 *
 *  @return Gibt zurueck, ob die Daten verarbeitet wurden
 *
 *  @remarks: Ist eine Hilfsfunktion fuer BAP_BCL_CanReceiveData. Enthaelt die Sonderbehandlung
 *  fuer die Verarbeitung von Startbotschaften nicht vorhandener Funktionen bei kompatibler
 *  Erweiterung.
 */
static BAP_IMPL_FAR bool_t
BAP_BCL_CanProcessData(BapRxParameters_pot apoRxPars, RxHandler_ft apfnRxHandler)
{
    DBGVAR bool_t bDataProcessed = BAP_FALSE;

    /*lint -esym(613, apoRxPars) */
    /*lint -esym(613, apfnRxHandler) */
    BAP_ASSERT(apoRxPars != NULL );
    BAP_ASSERT(apfnRxHandler != NULL );

    if ( NULL != apoRxPars->oPars.poFctRomRow )
    {
        BapBusIndirectionRxRomRow_pot poBusIndRx = apoRxPars->oPars.poFctRomRow->poBusRxTable;
        DBGVAR uint8_t u8RxTableSize = apoRxPars->oPars.poFctRomRow->u8RxTableSize;
        DBGVAR uint8_t u8RxTableIdx;

        /* Suche unter den moeglichen CAN Rx Eintraegen anch dem Eintrag, bei dem canId usw. uebereinstimmen.
           Da i.d.R. nur 1-2 Rx konfiguriert werden, lohnt es sich nicht, die Schleife vorzeitig abzubrechen
         */
        for (u8RxTableIdx=0; u8RxTableIdx<u8RxTableSize; u8RxTableIdx++)
        {
            if (BapIft_Can == poBusIndRx[u8RxTableIdx].eInterfaceType)
            {
                apoRxPars->poCanRxRomRow = poBusIndRx[u8RxTableIdx].xBusRxRomRow.poCanRx;

                if (   (apoRxPars->canId == apoRxPars->poCanRxRomRow->canMsgId)
                    && (apoRxPars->canInterfaceChannel == apoRxPars->poCanRxRomRow->canInterfaceChannel)
                    && (((uint8_t)apoRxPars->poCanRxRomRow->eSegmentationType & (uint8_t)apoRxPars->eSegmentationTypeMask)
                        == (uint8_t)apoRxPars->eSegmentationType) )
                {
                    /* Rufe abhaengig von der Nachricht den entsprechenden RxHandler auf */
                    apfnRxHandler(apoRxPars);
                    bDataProcessed = BAP_TRUE;
                }
            }
        }
    }
#if defined(BAP_ASG) && defined(BAP_USES_SEGMENTATION)
    else if ( (BapSegType_Unsegmented != apoRxPars->eSegmentationType )
#if defined(BAP_ASG) && defined(BAP_USES_SEGMENTATION) && defined(BAP_FSG)
        && (BapSG_ASG == apoRxPars->oPars.poLsgRomRow->eSGType)
#endif /* #if defined(BAP_ASG) && defined(BAP_USES_SEGMENTATION) && defined(BAP_FSG) */
    )
    {
        /* Spezialbehandlung fuer Funktionen bei kompatibler Erweiterung (Superset) */
        /* In diesem Fall wird bei Segmentierungsstart der dynamische Empfangskanal auf Fct-Id 2 */
        /* (BAPConfig) gesetzt, welcher immer unsegmentiert ist. Dies ist noetig, damit beim Empfang */
        /* der Sequenzbotschaften diese einem logischen Steuergeraet zugeordnet werden koennen. */
        /* Dies ist noetig, damit diese Sequenzbotschaften den Heartbeat-Timer retriggern koennen. */

        /* Berechne aus der Bitkodierung des Segmentationstyps den Segmentierungskanal:
           Konvertiere 1->0, 2->1, 4->2, 8->3, entspricht lg2 */
        DBGVAR uint8_t u8SegChannel = garu8ChannelBySegType[apoRxPars->eSegmentationType];

        BapCanRxDynSegChannels_pot poRxDynSegChannels;

        poRxDynSegChannels = BAP_BCL_GetRxDynSegChannel(apoRxPars->canId);

        if (NULL != poRxDynSegChannels)
        {
            BapBusIndirectionRxRomRow_pot poBusRxTable;
            apoRxPars->oPars.poFctRomRow = BAP_GetLsgFctRomRow(apoRxPars->oPars.poLsgRomRow, BAP_FCTID_BAPCONFIG);
            poBusRxTable = apoRxPars->oPars.poFctRomRow->poBusRxTable;
            poRxDynSegChannels->arpoRxDynSegChannels[u8SegChannel] = poBusRxTable->xBusRxRomRow.poCanRx;
        }
    }
#endif /* #if defined(BAP_ASG) && defined(BAP_USES_SEGMENTATION) */
    else
    {
        /* ignoriere empfangene Daten */
    }

    return bDataProcessed;
    /*lint +esym(613, apoRxPars) */
    /*lint +esym(613, apfnRxHandler) */
}


/**
 *  Diese Funktion verarbeitet eine empfangene CAN-Nachricht.
 *
 *  Falls es sich um eine unsegmentierte Nachricht handelt, dann wird diese direkt an die Applikation gemeldet.
 *
 *  Bei segmentierten Nachrichten wird die Nachricht zunaechst in den Puffer eingetragen und falls die
 *  Nachricht vollstaendig empfangen wurde, wird diese direkt an die Applikation gemeldet.
 *
 *  Es erfolgt keine Interrupt-Entkopplung innerhalb dieser Funktion.
 */
static BAP_IMPL_FAR void
BAP_BCL_CanReceiveData(BapCanRxBuffer_cpot apoCanRxBuffer)
{
    DBGVAR RxHandler_ft pfnRxHandler = BAP_CAN_RxNothing;
    DBGVAR bool_t bDataProcessed = BAP_FALSE;
    DBGVAR BapRxParameters_ot oRxPars;

    /*lint -esym(613,apoCanRxBuffer) */
    BAP_ASSERT(apoCanRxBuffer);

    MEMSET(&oRxPars, 0, sizeof(oRxPars));
    /* memset belegt folgende Felder mit Werten:
       oRxPars.fctId = 0;
       oRxPars.lsgId = 0;
       oRxPars.u16SegmentationLength = 0;
       oRxPars.poCanRxRomRow = NULL;
       oRxPars.eSegmentationType = BapSegType_Unsegmented;
       oRxPars.poLsgRomRow = NULL;
       oRxPars.poFctRomRow = NULL;
       oRxPars.eSegmentationTypeMask = BapSegType_Unsegmented;
    */
#ifdef BAP_USES_SEGMENTATION
    oRxPars.eSegmentationTypeMask = BapSegType_ChannelOneTwoThreeFour;
#endif
    oRxPars.pData = apoCanRxBuffer->aru8Data;
    oRxPars.u8Length = apoCanRxBuffer->u8Length;
    oRxPars.canId = apoCanRxBuffer->canMsgId;
    oRxPars.canInterfaceChannel = apoCanRxBuffer->canInterfaceChannel;

    /* Analysiere Telegramm, ob segmentiert oder unsegmentiert */
    if (!(oRxPars.pData[0] & 0x80))
    {
        pfnRxHandler = BAP_CAN_RxUnsegmented;

        /* Analysiere BAP Header (lsgId, fctId) */
        BAP_CAN_AnalyzeHeaderBytes(&oRxPars);
    }
#ifdef BAP_USES_SEGMENTATION
    else
    {
        /* Ermittle Segmentierungskanal. Bilde Zweierpotenz, dies entspricht dem Segmentierungstyp */
        oRxPars.eSegmentationTypeMask
            = (
                oRxPars.eSegmentationType
                = (BapSegmentationType_et) ( 1 << ((oRxPars.pData[0] & 0x30) >> 4))
              );

        if (!(oRxPars.pData[0] & 0x40))
        {
            if (apoCanRxBuffer->u8Length<4)
            {
                /* Segmentierte Startbotschaften, auch die Fuellbotschaften, muessen mindestens 4 Bytes haben */
                BAP_BCL_ErrorIndication((lsgId_t)0, (fctId_t)0, BapErr_BadDataLength);
                BAP_DBG_BadDataLengthError( (lsgId_t)0 );
            }
            else
            {
                /* Extrahiere Informationen aus Segmentierungsheader (BCL Rx Pufferlaenge und Segmentierungskanal) */
                oRxPars.u16SegmentationLength = (uint16_t)oRxPars.pData[1]
                    + (uint16_t)( (uint16_t) (oRxPars.pData[0] & 0x0f) << 8);

                /* Analysiere BAP Header (lsgId, fctId) */
                oRxPars.pData = &oRxPars.pData[2];  /* Optimierung: Bap Header beginnt ab Byte 2 */

                pfnRxHandler = BAP_CAN_RxSegmentedStart;

                BAP_CAN_AnalyzeHeaderBytes(&oRxPars);
            }
        }
        else
        {
            /* Optimierung fuer Sequenzbotschaften */
            bDataProcessed = BAP_BCL_CanReceiveSequence(&oRxPars);

#ifdef BAP_ASG
            /* Melde dem BPL dass Daten empfangen wurden (zum Retriggern der Heartbeat-Ueberwachung) */
            oRxPars.oPars.eOpCode = BapOp_MessageReceived;
            BAP_BCL_DataReceived(&oRxPars.oPars);
#endif /* #ifdef BAP_ASG */

            /* Auch wenn die Daten hier nicht verarbeitet werden bewirkt nachfolgender Code nichts
             * da in diesem Fall keine uebereinstimmende LsgId gefunden wird.
             */
        }
    }
#endif /* BAP_USES_SEGMENTATION */

    /* Fuer Sequenzbotschaften werden beide Zeiger = NULL */
    oRxPars.oPars.poLsgRomRow = BAP_GetLsgRomRow(oRxPars.oPars.lsgId);
    oRxPars.oPars.poFctRomRow = BAP_GetLsgFctRomRow(oRxPars.oPars.poLsgRomRow, oRxPars.oPars.fctId);

    if ( (NULL != oRxPars.oPars.poLsgRomRow)
            && ( (oRxPars.oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running)
#ifdef BAP_ASG  /* Im Kombigeraet gibt es WaitForConfig nicht fuer FSGs */
                  || (oRxPars.oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_WaitForConfig)
#endif /* #ifdef BAP_ASG */
               )
       )
    {
        bDataProcessed = BAP_BCL_CanProcessData(&oRxPars, pfnRxHandler);

#ifdef BAP_ASG
        /* Melde dem BPL dass Daten empfangen wurden (zum Retriggern der Heartbeat-Ueberwachung) */
        oRxPars.oPars.eOpCode = BapOp_MessageReceived;
        BAP_BCL_DataReceived(&oRxPars.oPars);
#endif /* BAP_ASG */
    }

    if (!bDataProcessed)
    {
        BAP_DBG_RxDataPurgedEvent();
    }
#ifdef BAP_USES_SEGMENTATION
    else
    if (oRxPars.eSegmentationType != BapSegType_Unsegmented)
    {
        BAP_CAN_RxSegmentedCompleteCheck(&oRxPars);
    }
    else
    {
        /* Unsegmentierte Daten wurden verarbeitet, keine weitere Arbeiten noetig.
           Else-Zweig wird fuer Einhaltung der Misra Regel 60 benoetigt */
    }
#endif /* BAP_USES_SEGMENTATION */

   return;
    /*lint +esym(613,apoCanRxBuffer) */
}


/* Definition (Implementierung) von globalen Funktionen */

/*  Diese Funktion ist in bap_bclbus.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_CanSendTask(void)
{
    DBGVAR uint16_t u16CanTxRomRow;
    static DBGVAR uint16_t u16CanTxRomStartRow = 0;
    DBGVAR uint16_t u16CanTxRomStartRowBuffer = u16CanTxRomStartRow;
    DBGVAR bool_t bCanTxRequestFound = BAP_FALSE;
    DBGVAR BapTxParameters_ot oTxPars;
    DBGVAR lsgId_t lastLsgId = (lsgId_t) 0;
#ifdef BAP_TASKTIME_LIMIT
    BapLsgRomRow_pot poLsgRomRow = NULL;
    BapLsgState_pot pfLsgState = NULL;
    DBGVAR uint8_t u8LsgRomIdx;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

    /* Heuristik fuer die Priorisierung der Datenelemente besteht aus dem
       speichern der zuletzt versandten Botschaft und beim naechsten Aufruf
       mit der nachfolgenden Botschaft fortzufahren */

#ifdef BAP_TASKTIME_LIMIT
    DBGVAR bool_t bLsgTxFlag = BAP_FALSE;

    /* Setze Merker, ob noch etwas zu versenden ist, je Lsg zurueck */
    for (u8LsgRomIdx=0; u8LsgRomIdx < BAP_P_LSG_ROM_TABLE_ROWS; u8LsgRomIdx++)
    {
        if (BAP_pLsgRomTable[u8LsgRomIdx])
        {
            pfLsgState = &BAP_pLsgRomTable[u8LsgRomIdx]->poLsgRamRow->fState;
            pfLsgState->fTxCanRequestFound = pfLsgState->fTxFlag;
            bCanTxRequestFound |= (bool_t) pfLsgState->fTxFlag;
            pfLsgState->fTxFlag = BAP_FALSE;
        }
    }

    if (bCanTxRequestFound)
#endif /* #ifdef BAP_TASKTIME_LIMIT */
    {
        bCanTxRequestFound = BAP_FALSE;
        /* Durchsuche alle CAN BCL Tx Puffer nach zu sendenden Datenelementen */
        for( u16CanTxRomRow = 0;
#ifdef BAP_TASKTIME_LIMIT
             (BAP_u16TaskTimeCounter<BAP_MAX_TASK_TIME) &&
#endif /* #ifdef BAP_TASKTIME_LIMIT */
             (u16CanTxRomRow < BAP_CAN_TX_ROM_TABLE_ROWS);
             u16CanTxRomRow++ )
        {
            /* Hole Zeiger auf die zugehoerigen CAN ROM Tabelle */
            oTxPars.poCanTxRomRow = &BAP_CanTxRomTable[(u16CanTxRomRow + u16CanTxRomStartRow) % BAP_CAN_TX_ROM_TABLE_ROWS];

            /* Falls die neue Zeile zu einem anderen Lsg gehoert */
            if (oTxPars.poCanTxRomRow->poFctRomRow->lsgId != lastLsgId)
            {
                lastLsgId = oTxPars.poCanTxRomRow->poFctRomRow->lsgId;
#ifdef BAP_TASKTIME_LIMIT
                poLsgRomRow = BAP_GetLsgRomRow(lastLsgId);
                /* Hole Information, ob fuer dieses Lsg ueberhaupt Sendeauftraege anstehen fuer schnellere Abfrage */
                if (poLsgRomRow)
                {
                    pfLsgState = &poLsgRomRow->poLsgRamRow->fState;
                    bLsgTxFlag = (bool_t) pfLsgState->fTxCanRequestFound;
                }
                else
                {
                    bLsgTxFlag = BAP_FALSE;
                }
#endif /* #ifdef BAP_TASKTIME_LIMIT */
            }

            /* Datenelement fuer das Senden auf CAN markiert und InhibitTimer abgelaufen? */
#ifdef BAP_TASKTIME_LIMIT
            if (bLsgTxFlag)
#endif /* #ifdef BAP_TASKTIME_LIMIT */
            {
                DBGVAR bool_t bCanTxSentNow = BAP_FALSE;
                oTxPars.poBclTxRamRow = oTxPars.poCanTxRomRow->poBclTxRamRow;
#ifdef BAP_USES_SEGMENTATION
                oTxPars.poCanTxRamRow = oTxPars.poCanTxRomRow->poCanTxRamRow;
#endif  /* #ifdef BAP_USES_SEGMENTATION */

                if  (
                        oTxPars.poBclTxRamRow->flags.fTxRequestFlag
#ifdef BAP_USES_SEGMENTATION
                        || oTxPars.poBclTxRamRow->flags.fTxInProgressFlag
#endif  /* #ifdef BAP_USES_SEGMENTATION */
#ifdef BAP_FSG
                        || oTxPars.poBclTxRamRow->flags.fTxErrorFlag
#endif /* #ifdef BAP_FSG */
                    )
                {
#ifdef BAP_TASKTIME_LIMIT
                    /* Ubernehme Ergebnis in Zwischenpuffer, da durch die Heuristik bedingt ein Lsg zweimal drankommen kann */
                    pfLsgState->fTxFlag = BAP_TRUE; /*lint !e613*/ /* pfLsgState wird immer initialisiert wenn bLsgTxFlag != BAP_FALSE */
#endif /* #ifdef BAP_TASKTIME_LIMIT */

                    if (0u == BAP_InhibitRamTable[oTxPars.poCanTxRomRow->u8InhibitIndex].u16InhibitTimer)
                    {
                        bCanTxSentNow = BAP_BCL_LowLevelCanTxTask(&oTxPars);
                    }
                }

                if (bCanTxSentNow && !bCanTxRequestFound)
                {
                    u16CanTxRomStartRowBuffer = (u16CanTxRomRow+u16CanTxRomStartRow+1u) % BAP_CAN_TX_ROM_TABLE_ROWS;
                    bCanTxRequestFound = BAP_TRUE;
                }
            }
        }
    }

#ifdef BAP_TASKTIME_LIMIT
    if (BAP_u16TaskTimeCounter>=BAP_MAX_TASK_TIME)
    {
        /* Stelle Tx-Flags wieder her, wenn Task-Zeit ueberschritten, sonst gehen Tx-Auftraege verloren */
        for (u8LsgRomIdx=0; u8LsgRomIdx < BAP_P_LSG_ROM_TABLE_ROWS; u8LsgRomIdx++)
        {
            if (BAP_pLsgRomTable[u8LsgRomIdx])
            {
                pfLsgState = &BAP_pLsgRomTable[u8LsgRomIdx]->poLsgRamRow->fState;
                pfLsgState->fTxFlag = pfLsgState->fTxCanRequestFound;
            }
        }
    }
#endif /* #ifdef BAP_TASKTIME_LIMIT */

    /* Startzeile fuer den naechsten Funktionsaufruf merken */
    u16CanTxRomStartRow = u16CanTxRomStartRowBuffer;
    return;
}


/*  Diese Funktion ist in bap_bclbus.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_InitCanDrvTxConfirmationFlags(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR uint16_t u16InhibitIndex;
    DBGVAR uint8_t u8LsgIndex;
    DBGVAR uint8_t u8LsgTableSize;
    DBGVAR BapMapCanIdToLsg_cpot poMapCanIdToLsg;
    DBGVAR BapLsgRomRow_pot poLsgRomRow;
    /*DBGVAR uint16_t u16CanTxRomTableIndex;*/

    /* Gibt an, ob es ein Steuergeraet mit dieser CAN-Id gibt, welches schon gestartet ist.
       Wird auch als Abbruchbedingung von Schleifen verwendet */
    DBGVAR bool_t bLsgStarted;

    /* Gibt an, ob die betroffene CAN-Id auch vom aktuellen Steuergeraet verwendet wird */
    DBGVAR bool_t bCurrentLsgOwns;

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow != NULL);

    /* [ALA 2006-11-17] Optimierung Verarbeitungszeit von Reset-Nachrichten */
    /* Verwendung der BAP_InhibitRomTable um CAN Msg Ids zu finden */
    for(u16InhibitIndex = 0; u16InhibitIndex < BAP_INHIBIT_ROWS; u16InhibitIndex++)
    {
        bLsgStarted = BAP_FALSE;
        bCurrentLsgOwns = BAP_FALSE;
        u8LsgTableSize = BAP_InhibitRomTable[u16InhibitIndex].u8MapCanIdToLsgSize;
        poMapCanIdToLsg = &BAP_InhibitRomTable[u16InhibitIndex].aroMapCanIdToLsgTable[0];

        /* Nur falls kein Steuergeraet gestartet ist, welches diese CAN-Id verwendet, darf das Tx-Flag initialisiert werden */
        for(u8LsgIndex = 0; (u8LsgIndex < u8LsgTableSize) && !bLsgStarted; u8LsgIndex++)
        {
            poLsgRomRow = poMapCanIdToLsg[u8LsgIndex].poLsgRomRow;
            if (poLsgRomRow->poLsgRamRow->eLsgStatus > BapLayerLsgStat_Initialized)
            {
                if (poLsgRomRow->lsgId != apoLsgRomRow->lsgId)
                {
                    bLsgStarted = BAP_TRUE;
                }
            }
            if (poLsgRomRow->lsgId == apoLsgRomRow->lsgId)
            {
                bCurrentLsgOwns = BAP_TRUE;
            }
        }

        if (!bCurrentLsgOwns)
        {
            /* CAN-Id soll nicht initialisiert werden, falls sie vom aktuellen Steuergeraet ueberhaupt nicht verwendet wird */
            bLsgStarted = BAP_TRUE;
        }

        /* Suche den passenden Interface-Channel */
        /** @todo BAP1.5: Nehme Interface-Channel in BAP_InhibitRomTable mit auf um Suche zu sparen.
            Verringert sonst Laufzeit waehrend der normalen Initialisierungsphase */

        /*Workaround für Patch 1.4.1.1*/
        if (!bLsgStarted)
        {
            BAP_CANUBS_InitReadyForTx(1, BAP_InhibitRomTable[u16InhibitIndex].canMsgId);
        }

        /*
        for(u16CanTxRomTableIndex = 0;
            !bLsgStarted && (u16CanTxRomTableIndex < apoLsgRomRow->u16CanTxRomTableSize);
            u16CanTxRomTableIndex++
           )
        {
            if (BAP_InhibitRomTable[u16InhibitIndex].canMsgId == apoLsgRomRow->poCanTxRomTable[u16CanTxRomTableIndex].canMsgId)
            {
                BAP_CANUBS_InitReadyForTx(apoLsgRomRow->poCanTxRomTable[u16CanTxRomTableIndex].canInterfaceChannel
                    , BAP_InhibitRomTable[u16InhibitIndex].canMsgId);

                // breche Schleife ab
                bLsgStarted = BAP_TRUE;
            }
        }*/
    }
    return;
    /*lint +esym(613,apoLsgRomRow)*/
}


/* Diese Funktion ist in bap_bclbus.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_CanReceiveTask(void)
{
    uint8_t u8IRQState;
    DBGVAR bool_t bFlagBufferOverflow;

    DBGVAR bool_t bReadWriteDifference;      /* Beschreibt den Abstand zwischen WriteOffset und ReadOffset,
                                                wird als Boolean interpretiert, neg. Werte als Komplement gespeichert */
    do
    {
        disableint(&u8IRQState);
         /* Differenz wird wie ein bool verwendet */
        bReadWriteDifference = BAP_u8CanRxRingBufferWriteOffset - BAP_u8CanRxRingBufferReadOffset;
        bFlagBufferOverflow = BAP_u8CanRxRingBufferWriteOffset > BAP_CAN_RX_RING_BUFFER_SIZE;
        restoreint(u8IRQState);

        /* Falls Pufferueberlauf, dann Fehler melden */
        if (bFlagBufferOverflow)
        {
            BAP_BCL_ErrorIndication( (lsgId_t)0u, (fctId_t)0u, BapErr_ReceivedDataLost);
            BAP_DBG_DataLostError();
        }

        /* Ist noch etwas im Ringpuffer enthalten? */
        if (bReadWriteDifference)
        {
            BapCanRxBuffer_pot poCanRxBuffer = &BAP_CanRxRingBuffer[BAP_u8CanRxRingBufferReadOffset];

            /* Verarbeite die Daten aus dem Ringpuffer */
            BAP_BCL_CanReceiveData(poCanRxBuffer);

            disableint(&u8IRQState);
            if (BAP_u8CanRxRingBufferWriteOffset >= BAP_CAN_RX_RING_BUFFER_SIZE)
            {
                BAP_u8CanRxRingBufferWriteOffset = BAP_u8CanRxRingBufferReadOffset;
            }
            restoreint(u8IRQState);

            BAP_u8CanRxRingBufferReadOffset = (uint8_t)((uint8_t)(BAP_u8CanRxRingBufferReadOffset+(uint8_t)1u) % BAP_CAN_RX_RING_BUFFER_SIZE);
        }
    }
    while (bReadWriteDifference);
    return;
}

/* CallBack-Funktionen */


/*  Diese Funktion ist in bap_canubs.h dokumentiert */
BAP_IMPL_FAR void
BAP_CANUBS_RxData(DBGVAR BapCanInterfaceChannel_t aCanInterfaceChannel
    , DBGVAR BapCanMsgId_t aCanMsgId
    , DBGVAR const ptr_t apData
    , DBGVAR uint8_t au8Length)
{
    if ( (au8Length < (uint8_t)2u) || (au8Length > (uint8_t)8u) || !apData )
    {
        /* ignoriere Nachrichten, welche keinen BAP-Header haben
           1 Byte Header gibt es zwar bei Folgebotschaften segmentierter Nachrichten,
           diese muessen aber mindestens ein Datenbyte beinhalten.
           Unsegmentierte Nachrichten haben einen 2 Byte Header.
           Segmentierte Nachrichten haben einen 4 Byte Header.
           CAN Botschaften koennen nicht mehr als 8 Bytes haben. Falls diese
           Laenge uebergeben wird, handelt es sich um einen Integrationsfehler.
        */
    }
    else
#ifdef BAP_USES_SEGMENTATION
    if ( (0x8F == (apData[0] & 0xCF)) && (0xFF == apData[1]) )
    {
        /* ignoriere Fuellbotschaft, ohne dass eine Diagnosemeldung auftritt */
    }
    else
#endif /* #ifdef BAP_USES_SEGMENTATION */
    {
        BAP_DBG_RxEvent();

        if (BAP_CAN_RX_RING_BUFFER_SIZE <= BAP_u8CanRxRingBufferWriteOffset)
        {
            /* Falls Puffer voll, dann verwerfe die Nachricht. Korrigiere Offset,
             * so dass der Datenverlust im BAP_BCL_CanReceiveTask festgestellt werden kann.
             */
            BAP_u8CanRxRingBufferWriteOffset = (uint8_t)(BAP_CAN_RX_RING_BUFFER_SIZE + (uint8_t)1u);

            BAP_DBG_RxDataPurgedEvent();
        }
        else
        {
            BapCanRxBuffer_pot poCanRxBuffer = &BAP_CanRxRingBuffer[BAP_u8CanRxRingBufferWriteOffset];
            DBGVAR uint8_t u8WriteOffSet;  /* Hilfsvariable Optimierung volatile */

            /* Trage die Daten in den Ring-Puffer ein */
            poCanRxBuffer->canInterfaceChannel = aCanInterfaceChannel;
            poCanRxBuffer->canMsgId = aCanMsgId;
            poCanRxBuffer->u8Length = au8Length;
            MEMCPY( poCanRxBuffer->aru8Data, apData, au8Length );

            /* Setze neuen Schreib-Index */
            u8WriteOffSet = (uint8_t)((uint8_t)(BAP_u8CanRxRingBufferWriteOffset+(uint8_t)1u) % BAP_CAN_RX_RING_BUFFER_SIZE);

            /* Falls Puffer voll, dann Setze Index hinter den Puffer.
             * Dadurch wird verhindert, dass weitere Nachrichten eingetragen werden koennen.
             */
            if (u8WriteOffSet == BAP_u8CanRxRingBufferReadOffset)
            {
                BAP_u8CanRxRingBufferWriteOffset = BAP_CAN_RX_RING_BUFFER_SIZE;
            }
            else
            {
                BAP_u8CanRxRingBufferWriteOffset = u8WriteOffSet;
            }
        }
    }
    return;
}

