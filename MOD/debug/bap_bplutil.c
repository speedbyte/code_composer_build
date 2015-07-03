/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/bpl/bap_bplutil.c $
 * $Revision: 57 $
 * $Modtime: 20.04.05 16:23 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Hans-Micheal Schubert, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt Hilfsfunktionen fuer die Schicht im BPL
 *  Projekt. Die Hilfsfunktionen umfassen auch den Zugriff auf
 *  die Konfigurationstabellen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Workaround fuer Cosmic HCS12 Problem mit FAR-Pointern
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer HB-Trigger.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-04-13   1.3         ALA         Optimierung der Datenstrukturen
 * 2005-03-30   1.3         CRI         Bei der Bestimmung des naechsten HBs in SetNextFctIndex
 *                                      wird zukuenftig die aktuelle Funkctionlist einbezogen
 * 2004-11-17   1.2         JOK         Diverse Zeiger als const deklariert
 * 2004-06-07   1.1         JOK         Laufzeitoptimierung in BAP_BPL_SetNextFctIndex() 
 *                                      durch ersetzen der aLsgId durch den Zeiger apoLsgRomRow
 *                                      in die LSG-Tabelle
 ******************************************************************/


/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_balconfig.h"      
#include "bap_bplconfig.h"
#include "bap_bplutil.h"
#include "bap_util.h"
#include "bap_debug.h"

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

/* Definition (Implementierung) von statischen Funktionen */

/* Definition (Implementierung) von globalen Funktionen */

/* Diese Funktion ist in bap_bplutil.h dokumentiert */

#ifdef BAP_FSG
/* Diese Funktion ist in bap_bplutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_SetNextFctIndex(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR uint8_t i;

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

    i = apoLsgRomRow->poLsgRamRow->xHeartbeatInfo.u8LastSentFctIndex;
    for (;;)
    {
        i++;
        if(i == apoLsgRomRow->u8FctRomTableSize)
        {
            i=0;
            BAP_DBG_HeartbeatLoopEvent(apoLsgRomRow->lsgId);
        }
        if(apoLsgRomRow->poFctRomTable[i].bHeartbeatOn 
            && (0 != (apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList[BINARY_ID(apoLsgRomRow->poFctRomTable[i].fctId) / 8] 
                   & (0x80 >> (BINARY_ID(apoLsgRomRow->poFctRomTable[i].fctId) % 8)) ) )
            ) 
        {
            apoLsgRomRow->poLsgRamRow->xHeartbeatInfo.u8LastSentFctIndex=i;
            return;
        }
    }
    /* NEVER REACHED */ /* wird niemals erreicht */
    /*lint +esym(613,apoLsgRomRow)*/
}
#endif /* #ifdef BAP_FSG */


#if defined(BAP_USES_METHODS) || defined(BAP_ASG)
/* Diese Funktion ist in bap_bplutil.h dokumentiert */
BAP_IMPL_FAR void 
BAP_BPL_StopRetries (BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR uint8_t i;
    BapBplRamRow_pot poBplRamRow;

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

    /* Schleife ueber die gesamte Retry-Tabelle und alle Retries ausschalten */
    for(i=apoLsgRomRow->u8FctRomTableSize;i>0;i--)
    {
        poBplRamRow = apoLsgRomRow->poFctRomTable[i-1].poBplRamRow;
        if(poBplRamRow)
        {
            poBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = 0;
            poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;
            poBplRamRow->u16RetryOrProcessingTimer  = 0;
            BAP_DBG_RetryStoppedEvent(apoLsgRomRow->lsgId);
        }
    }

    /*lint +esym(613,apoLsgRomRow)*/
    return;
}
#endif /* #if defined(BAP_USES_METHODS) || defined(BAP_ASG) */


/* Diese Funktion ist in bap_bplutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_RetriggerHeartbeat(BapLsgRomRow_pot apoLsgRomRow)
{
    if (NULL != apoLsgRomRow)
    {
        apoLsgRomRow->poLsgRamRow->u16HeartbeatTimer = *(apoLsgRomRow->pu16HeartbeatTimer);
    }
}


/* Diese Funktion ist in bap_bplutil.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_InitializeHeartbeat(BapLsgRomRow_pot apoLsgRomRow, uint8_t au8HeartbeatTime) 
{
    uint16_t u16HeartbeatMs;
    uint8_t  u8TaskTimeMs;
    uint16_t u16HeartbeatTasks;

    if (NULL != apoLsgRomRow) 
    {      
        u16HeartbeatMs = (uint16_t)((uint16_t) apoLsgRomRow->u8Heartbeat * 100u);
        u8TaskTimeMs =  apoLsgRomRow->u8TaskTimeMs; 
        u16HeartbeatTasks = (uint16_t) ((uint16_t) au8HeartbeatTime * (u16HeartbeatMs / (uint16_t) u8TaskTimeMs));
        *(apoLsgRomRow->pu16HeartbeatTimer) = u16HeartbeatTasks;  
        BAP_BPL_RetriggerHeartbeat(apoLsgRomRow);
    }
}
