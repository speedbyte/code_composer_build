/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: Debug/Diagnose
 *
 * $Archive: /dev/BAP/debug/bap_debug.c $
 * $Revision: 44 $
 * $Modtime: 3.05.05 12:48 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Implementierung der Debug/Diagnose-
 *  Komponente im BAP-Projekt.
 *
 *  Diese C-Datei enthaelt nicht die Funktionen, welche die
 *  Anbindung an die Kommunikationschicht vornehmen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-04-20   1.3         ALA         Funktion BAP_DBG_SetBplState entfernt, da gemeinsamer Zustand fuer alle Schichten.
 * 2005-04-06   1.3         ALA         Funktion BAP_DBG_RequestTimeoutError() und BAP_DBG_DataLostError() eingebaut
 * 2004-11-17   1.2         JOK         Funktion BAP_DBG_GetAllMessageCorruptedError() eingebaut
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Funktion BAP_DBG_SetBclState() entfernt
 ******************************************************************/


/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_debug.h"
#include "bap_util.h"
#include "bap_balconfig.h"
#include "bap_bplconfig.h"
#include "bap_bclconfig.h"

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

#if defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF)
/**
 *  Speichert, fuer welche Lsg gerade Debug-Betrieb aktiv ist.
 */
static DBGVAR lsgId_t gDebugLsgId = (lsgId_t)0;
#endif /* defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF) */

#if defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF)
/**
 *  Speichert die aktuelle Debug-Information.
 */
DBGVAR uint8_t BAP_DBG_DebugInfoTable[7];
#endif /* defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF) */

/* Vorwaerts-Deklarationen von statischen Funktionen */

/* Definition von statischen Funktionen */

/* Definition von global sichtbaren Funktionen */

/**************************************************/
/*         Debug-Flags der BAL-Schicht            */
/**************************************************/

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_SetBalState(lsgId_t aLsgId
    , BapLayerLsgStatus_et aeState)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 1, Bit 2^6 und 2^7 */
        BAP_DBG_DebugInfoTable[1] &= 0x3F;  /* loeschen, weil kein Ereignis */
        BAP_DBG_DebugInfoTable[1] |= ((uint8_t)aeState) << 6;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_SetCacheValidState(lsgId_t aLsgId
    , bool_t abValid)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 1, Bit 2^5 */
        BAP_DBG_DebugInfoTable[1] &= 0xDF;  /* loeschen, weil kein Ereignis */
        BAP_DBG_DebugInfoTable[1] |= (abValid & 1) << 5;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_GetAllMessageCorruptedError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 1, Bit 2^0 */
        BAP_DBG_DebugInfoTable[1] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_InvalidStateError(lsgId_t aLsgId)
{
    if (gDebugLsgId == aLsgId)
    {
        /* Byte 2, Bit 2^7 */
        BAP_DBG_DebugInfoTable[2] |= 0x80;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_CacheInvalidError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^6 */
        BAP_DBG_DebugInfoTable[2] |= 0x40;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_CacheNotAvailableError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^5 */
        BAP_DBG_DebugInfoTable[2] |= 0x20;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_InvalidArgError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^4 */
        BAP_DBG_DebugInfoTable[2] |= 0x10;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_ProtocolVersionError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^3 */
        BAP_DBG_DebugInfoTable[2] |= 0x08;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_DataDefinitionError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^2 */
        BAP_DBG_DebugInfoTable[2] |= 0x04;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_FSG
#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_GetAllEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^1 */
        BAP_DBG_DebugInfoTable[2] |= 0x02;
    }
}
#endif /* BAP_DBG_PER_LSG */
#endif /* BAP_FSG */


#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_StatusAllEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 2, Bit 2^0 */
        BAP_DBG_DebugInfoTable[2] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */

/**************************************************/
/*         Debug-Flags der BPL-Schicht            */
/**************************************************/


#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_SetHeartbeatState(lsgId_t aLsgId
    , bool_t bHeartbeatOn)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 3, Bit 2^5 */
        BAP_DBG_DebugInfoTable[3] &= 0xDF;  /* loeschen, weil kein Ereignis */
        BAP_DBG_DebugInfoTable[3] |= (bHeartbeatOn & 0x01) << 5;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_ASG
#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RequestTimeoutError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 3, Bit 2^2 */
        BAP_DBG_DebugInfoTable[3] |= 0x04;
    }
}
#endif /* BAP_DBG_PER_LSG */
#endif /* BAP_ASG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryTimeoutError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 3, Bit 2^1 */
        BAP_DBG_DebugInfoTable[3] |= 0x02;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_FSG
#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_HeartbeatLoopEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 3, Bit 2^0 */
        BAP_DBG_DebugInfoTable[3] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */
#endif /* BAP_FSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_HeartbeatProcessedEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^7 */
        BAP_DBG_DebugInfoTable[4] |= 0x80;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_HeartbeatTimeoutError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^6 */
        BAP_DBG_DebugInfoTable[4] |= 0x40;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryMonitoringStartedEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^5 */
        BAP_DBG_DebugInfoTable[4] |= 0x20;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryProvokedEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^4 */
        BAP_DBG_DebugInfoTable[4] |= 0x10;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryAnsweredEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^3 */
        BAP_DBG_DebugInfoTable[4] |= 0x08;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryRetriggeredEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^2 */
        BAP_DBG_DebugInfoTable[4] |= 0x04;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryStoppedEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^1 */
        BAP_DBG_DebugInfoTable[4] |= 0x02;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RetryBusyError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 4, Bit 2^0 */
        BAP_DBG_DebugInfoTable[4] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */


/**************************************************/
/*         Debug-Flags der BPL-Schicht            */
/**************************************************/

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_TaskTimeLimitEvent(void)
{
    if (gDebugLsgId)
    {
        /* Byte 5, Bit 2^4 */
        BAP_DBG_DebugInfoTable[5] |= 0x10;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_BadDataLengthError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 5, Bit 2^3 */
        BAP_DBG_DebugInfoTable[5] |= 0x08;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_DataLostError(void)
{
    if (gDebugLsgId)
    {
        /* Byte 5, Bit 2^2 */
        BAP_DBG_DebugInfoTable[5] |= 0x04;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_TxStartMsgEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 5, Bit 2^1 */
        BAP_DBG_DebugInfoTable[5] |= 0x02;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RxStartMsgEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 5, Bit 2^0 */
        BAP_DBG_DebugInfoTable[5] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_TxSegMsgCompleteEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 6, Bit 2^7 */
        BAP_DBG_DebugInfoTable[6] |= 0x80;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RxSegMsgCompleteEvent(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 6, Bit 2^6 */
        BAP_DBG_DebugInfoTable[6] |= 0x40;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_IntertelegramTimeoutError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 6, Bit 2^5 */
        BAP_DBG_DebugInfoTable[6] |= 0x20;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_MsgOversizeError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 6, Bit 2^4 */
        BAP_DBG_DebugInfoTable[6] |= 0x10;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_SequenceError(lsgId_t aLsgId)
{
    if (aLsgId == gDebugLsgId)
    {
        /* Byte 6, Bit 2^3 */
        BAP_DBG_DebugInfoTable[6] |= 0x08;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RxDataPurgedEvent(void)
{
    if (gDebugLsgId)
    {
        /* Byte 6, Bit 2^2 */
        BAP_DBG_DebugInfoTable[6] |= 0x04;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_TxEvent(void)
{
    if (gDebugLsgId)
    {
        /* Byte 6, Bit 2^1 */
        BAP_DBG_DebugInfoTable[6] |= 0x02;
    }
}
#endif /* BAP_DBG_PER_LSG */

#ifdef BAP_DBG_PER_LSG
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_RxEvent(void)
{
    if (gDebugLsgId)
    {
        /* Byte 6, Bit 2^0 */
        BAP_DBG_DebugInfoTable[6] |= 0x01;
    }
}
#endif /* BAP_DBG_PER_LSG */


#if defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF)
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_DBG_Task(void)
{
    if (gDebugLsgId)
    {
        /* Ist ein Ereignis oder Fehler aufgetreten? */
        if ( (BAP_DBG_DebugInfoTable[1] & 0x1F) |
             (BAP_DBG_DebugInfoTable[2]) |
             (BAP_DBG_DebugInfoTable[3] & 0xDF) |
             (BAP_DBG_DebugInfoTable[4]) |
             (BAP_DBG_DebugInfoTable[5] & 0x3F) |
             (BAP_DBG_DebugInfoTable[6]))
        {
            /* Versende Botschaft */
            DBGVAR BapError_et bapErr = BAP_SendDebugInfo((uint8_t*)BAP_DBG_DebugInfoTable);

            /* Nach erfolgreichem Versenden */
            if (BapErr_OK == bapErr)
            {
                /* Loesche Ereignisbits */
                BAP_DBG_DebugInfoTable[1] &= 0xE0;
                BAP_DBG_DebugInfoTable[2] = 0u;
                BAP_DBG_DebugInfoTable[3] &= 0x20;
                BAP_DBG_DebugInfoTable[4] = 0u;
                BAP_DBG_DebugInfoTable[5] &= 0xC0;
                BAP_DBG_DebugInfoTable[6] = 0u;
            }
        }
    }
}
#endif /* defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF) */


#if defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF)
/* Diese Funktion ist in bap_debug.h dokumentiert */
BAP_IMPL_FAR void
BAP_ProcessDebugControl(const uint8_t aru8DebugCtrl[2])
{
    BapLsgRomRow_pot poLsgRomRow;
    if (aru8DebugCtrl)
    {
        poLsgRomRow = BAP_GetLsgRomRow((lsgId_t)((uint8_t)aru8DebugCtrl[0] & (uint8_t)0x3Fu));

        if (poLsgRomRow)
        {
            if (!aru8DebugCtrl[1])
            {
                gDebugLsgId = (lsgId_t)0;
            }
            else
            {
                BAP_DBG_DebugInfoTable[0] = aru8DebugCtrl[0];
                gDebugLsgId = (lsgId_t) (aru8DebugCtrl[0] & (uint8_t)0x3Fu);

                BAP_DBG_DebugInfoTable[1] = 0u;
                BAP_DBG_DebugInfoTable[2] = 0u;
                BAP_DBG_DebugInfoTable[3] = 0u;
                BAP_DBG_DebugInfoTable[4] = 0u;
                BAP_DBG_DebugInfoTable[5] = 0u;
                BAP_DBG_DebugInfoTable[6] = 0u;

                /* Die richtigen Anfangszustaende setzen */
                BAP_DBG_SetBalState(gDebugLsgId, poLsgRomRow->poLsgRamRow->eLsgStatus);

#ifdef BAP_ASG
#if defined(BAP_ASG) && defined(BAP_FSG)
                if (BapSG_ASG == poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
                {
                    BAP_DBG_SetCacheValidState(gDebugLsgId
                        , (bool_t)poLsgRomRow->poLsgRamRow->fState.fCacheValid);
                }
#endif /* #ifdef BAP_ASG */
                BAP_DBG_SetHeartbeatState(gDebugLsgId
                    , (bool_t)(poLsgRomRow->poLsgRamRow->xHeartbeatInfo.eStatus == BapBplStat_OK) );

                /* Nach dem Einschalten gleich die Botschaft versenden */
                (void) BAP_SendDebugInfo((uint8_t*)BAP_DBG_DebugInfoTable);
            }
        }
    }
}
#endif /* defined(BAP_DBG_PER_LSG) || defined(BAP_DBG_ON_OFF) */

