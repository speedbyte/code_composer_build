/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/bcl/bap_bclutil.c $
 * $Revision: 99 $
 * $Modtime: 12.10.08 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Klaus Neubert, KOPF GmbH, D-76829 Landau-Moerzheim
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt Hilfsfunktionen fuer die BCL Schicht im BAP
 *  Projekt. Die Hilfsfunktionen umfassen auch den Zugriff auf
 *  die Konfigurationstabellen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2007-10-12   1.4.1.2     JOK         Beim Ruecksetzen des BCL Tx Puffer Bereichs auch NeedsAck zuruecksetzen.
 * 2006-11-20   1.4.1       ALA         Optimierung Laufzeit von Reset-Nachrichten
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2006-03-28   1.3.2       ALA         Stoppen der Intertelegramzeitueberwachung bei Reset
 * 2005-04-29   1.3         ALA         Funktion BAP_BCL_GetFctRomRow() entfernt, Anpassung an neue
 *                                      Datenstrukturen, Verbesserte Compilerunterstuetzung.
 * 2004-11-17   1.2         JOK         Interruptsperre bei BAP_BCL_InterTelegramTimerTask() eingebaut
 * 2004-11-17   1.2         JOK         Funktion BAP_BCL_SetInitRamValues() in zwei Funktionen
 *                                      BAP_BCL_InitRxRamValues() und BAP_BCL_InitTxRamValues() aufgeteilt
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Include stdlib.h eingefuegt
 * 2004-06-07   1.1         JOK         Funktion BAP_BCL_SetInitRamValues() eingefuegt
 * 2004-06-07   1.1         JOK         Funktion BAP_BCL_InitLsgRamAreas() mittels Hilfsfunktion
 *                                      BAP_BCL_SetInitRamValues() optimiert
 * 2004-06-07   1.1         JOK         Funktion BAP_BCL_GetLsgStatus() entfernt
 ******************************************************************/


/* Systemincludes mit <...> */
#include <stdlib.h>
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_util.h"
#include "bap_bcl.h"
#include "bap_bclconfig.h"
#include "bap_config.h"
#include "bap_bclutil.h"
#include "bap_bclbus.h"
#include "bap_debug.h"

/* Externe globale Variablen-Definitionen */

/* diese Variable ist in bap_bclutil.h dokumentiert */

#ifdef BAP_USES_SEGMENTATION
bool_t BAP_bInterTelegramTimerRunning;
#endif /* BAP_USES_SEGMENTATION */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

static BAP_IMPL_FAR void
BAP_BCL_InitRxRamValues(BapCanRxRomRow_pot apoCanRomRow);

static BAP_IMPL_FAR void
BAP_BCL_InitTxRamValues(BapCanTxRomRow_pot apoCanRomRow);

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_BCL_HandleInterTelegramTimer(uint16_t au16InterTelegramIndex);
#endif /* #ifdef BAP_USES_SEGMENTATION */

/* Definition (Implementierung) von statischen Funktionen */


/**
 *  Initialisiert alle RAM-Bereiche der BCL TX-Puffer.
 *
 *  @param apoCanRomRow  der zu initialisierende Eintrag in der BCL-TX-Tabelle.
 */
static BAP_IMPL_FAR void
BAP_BCL_InitTxRamValues(BapCanTxRomRow_pot apoCanRomRow)
{
    BapBclTxRamRow_pot poBclTxRamRow;
    /*lint -esym(613,apoCanRomRow) */
    BAP_ASSERT(apoCanRomRow);

    poBclTxRamRow = apoCanRomRow->poBclTxRamRow;

    /* Ruecksetzen des BCL Tx Puffer Bereichs */
    if(poBclTxRamRow != NULL)
    {
#ifdef BAP_USES_ACKNOWLEDGE
		poBclTxRamRow->flags.fTxRequestNeedsAckFlag = 0;
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */
        poBclTxRamRow->flags.fTxRequestFlag = 0;
#ifdef BAP_USES_SEGMENTATION
        poBclTxRamRow->flags.fTxInProgressFlag = 0;
#endif /* #ifdef BAP_USES_SEGMENTATION */
#ifdef BAP_FSG
        /* Keine Fallunterscheidung fuer das Kombigeraet noetig, da im ASG TxError unbenutzt */
        poBclTxRamRow->flags.fTxErrorFlag = 0;
#endif /* BAP_FSG */
    }

#ifdef BAP_USES_SEGMENTATION
    /* Ruecksetzen des CAN Tx RAM Bereichs (nur bei Segmentierung vorhanden) */
    if (apoCanRomRow->poCanTxRamRow != NULL)
    {
        apoCanRomRow->poCanTxRamRow->flags.fSequenceNumber = 0;
        apoCanRomRow->poCanTxRamRow->u16BclTxBufferByteOffset = 0;
    }
#endif /* #ifdef BAP_USES_SEGMENTATION */

    BAP_InhibitRamTable[apoCanRomRow->u8InhibitIndex].u16InhibitTimer = 0;

    /*lint +esym(613,apoCanRomRow) */
    return;
}


/**
 *  Initialisiert alle RAM-Bereiche der BCL RX-Puffer.
 *
 *  @param apoCanRomRow  der zu initialisierende Eintrag in der BCL-RX-Tabelle.
 *
 */
static BAP_IMPL_FAR void
BAP_BCL_InitRxRamValues(BapCanRxRomRow_pot apoCanRomRow)
{
    BapBclRxRamRow_pot poBclRxRamRow;
    /*lint -esym(613,apoCanRomRow) */
    BAP_ASSERT(apoCanRomRow);

    poBclRxRamRow = apoCanRomRow->poBclRxRamRow;

    /* Ruecksetzen des BCL Tx Puffer Bereichs */
    if(poBclRxRamRow != NULL)
    {
#ifdef BAP_USES_SEGMENTATION
        poBclRxRamRow->flags.fRxInProgressFlag = BAP_FALSE;
#endif /* #ifdef BAP_USES_SEGMENTATION */
        poBclRxRamRow->flags.fRxCompleteFlag = BAP_FALSE;
        poBclRxRamRow->flags.fRxErrorFlag = BAP_FALSE;

        poBclRxRamRow->oBufferWithLength.u16Length = apoCanRomRow->u16Size;
        /* nur bei FixedByteSequence ist das Loeschen des Puffers erforderlich.
           FixedByteSequence ist immer unsegmentiert.
           Bei ByteSequence wird darauf verzichtet, um die Verarbeitungszeit
           einer Reset-Nachricht gering zu halten.
         */
#ifdef BAP_USES_SEGMENTATION
        if ( (BapSegType_Unsegmented == apoCanRomRow->eSegmentationType) &&
             (apoCanRomRow->u16Size > 6u)
            )
#endif /* #ifdef BAP_USES_SEGMENTATION */
        {
            MEMSET(poBclRxRamRow->oBufferWithLength.aru8Buffer, 0
                , poBclRxRamRow->oBufferWithLength.u16Length);
        }
    }

#ifdef BAP_USES_SEGMENTATION
    /* Ruecksetzen des CAN Rx RAM Bereichs (nur bei Segmentierung vorhanden) */
    if (apoCanRomRow->poCanRxRamRow != NULL)
    {
        apoCanRomRow->poCanRxRamRow->flags.fSequenceNumber = 0;
        apoCanRomRow->poCanRxRamRow->u16BclRxBufferByteOffset = 0;
        *apoCanRomRow->pu16InterTelegramTimer = 0;
    }
#endif /* #ifdef BAP_USES_SEGMENTATION */

    /*lint +esym(613,apoCanRomRow) */
    return;
}


#ifdef BAP_USES_SEGMENTATION
/**
 *  Dekrementiert einen InterTelegramTimer um '1' je Funktionsaufruf
 *
 *  @param au16InterTelegramIndex 1 basierter Index des InterTelegramTimers in der RAM-Tabelle
 */
static BAP_IMPL_FAR void
BAP_BCL_HandleInterTelegramTimer(uint16_t au16InterTelegramIndex)
{
    DBGVAR uint16_t *pu16InterTelegramTimer;

    au16InterTelegramIndex--;

    pu16InterTelegramTimer = &BAP_u16InterTelegramTimerTable[au16InterTelegramIndex];

    if(0u < *pu16InterTelegramTimer)
    {
        if(0u == --(*pu16InterTelegramTimer))
        {
            DBGVAR fctId_t fctId = BAP_InterTelegramTimerMappingTable[au16InterTelegramIndex].fctId;
            DBGVAR lsgId_t lsgId = BAP_InterTelegramTimerMappingTable[au16InterTelegramIndex].lsgId;

            BAP_DBG_IntertelegramTimeoutError(lsgId);

            /* Melde Fehler an Applikation */
            BAP_BCL_ErrorIndication(lsgId, fctId, BapErr_TimeoutSegmentation);
        }
        BAP_bInterTelegramTimerRunning = BAP_TRUE;
    }
}
#endif  /* #ifdef BAP_USES_SEGMENTATION */


/* Definition (Implementierung) von globalen Funktionen */


/*  Diese Funktion ist in bap_bclutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_InhibitTimerTask(void)
{
    DBGVAR uint16_t u16InhibitIndex;
    DBGVAR uint16_t *pu16InhibitTimer;

    /* Dekrementiere alle InhibitTimer um je '1' pro Funktionsaufruf */
    for(u16InhibitIndex = 0; u16InhibitIndex < BAP_INHIBIT_ROWS; u16InhibitIndex++)
    {
        pu16InhibitTimer = &BAP_InhibitRamTable[u16InhibitIndex].u16InhibitTimer;
        if(0u < *pu16InhibitTimer)
        {
            (*pu16InhibitTimer)--;
        }
    }
    return;
}


#ifdef BAP_USES_SEGMENTATION
/*  Diese Funktion ist in bap_bclutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_InterTelegramTimerTask(void)
{
    DBGVAR uint16_t u16InterTelegramIndex;

    /* Dekrementiere alle InterTelegramTimer um je '1' pro Funktionsaufruf */
    if (BAP_bInterTelegramTimerRunning)
    {
        BAP_bInterTelegramTimerRunning = BAP_FALSE;

        /*lint -e681 Meldung "Schleife wird nicht durchlaufen" moeglich, wenn nichts Segmentiertes empfangen wird */
        for(u16InterTelegramIndex = BAP_INTER_TELEGRAM_TIMER_ROWS;
            u16InterTelegramIndex > 0u;
            u16InterTelegramIndex--)
        {
            BAP_BCL_HandleInterTelegramTimer(u16InterTelegramIndex);
        }
        /*lint +e681 */
    }
    return;
}
#endif /* BAP_USES_SEGMENTATION */


/*  Diese Funktion ist in bap_bclutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_InitLsgRamAreas(BapLsgRomRow_pot apoLsgRomRow)
{
#ifdef BAP_USES_CAN
    BapCanTxRomRow_pot poCanTxRomRow;
    DBGVAR uint16_t i;
    DBGVAR lsgId_t lsgId;

    /*lint -esym(613,apoLsgRomRow) */
    BAP_ASSERT(apoLsgRomRow);

    lsgId = apoLsgRomRow->lsgId;

    /* Initialisierung der CAN Tx RAM Bereiche */
    for (i=0; i<BAP_CAN_TX_ROM_TABLE_ROWS; i++)
    {
        poCanTxRomRow = &BAP_CanTxRomTable[i];
        if(poCanTxRomRow->poFctRomRow->lsgId == lsgId)
        {
            BAP_BCL_InitTxRamValues(poCanTxRomRow);
        }
    }

    /* Initialisierung der CAN Rx RAM Bereiche */

    /* Bug in PC-Lint 8.0 laesst sonst naechste Zeile auflisten, da apoLsgRomRow abgeschaltet */
    /*lint -e613 */
    for (i=0; i<apoLsgRomRow->u16CanRxRomTableSize; i++)
    {

        BAP_BCL_InitRxRamValues(&apoLsgRomRow->poCanRxRomTable[i]);
    }
    /*lint +e613 */
#endif /* BAP_USES_CAN */
    return;
}


/* CallBack-Funktionen */
