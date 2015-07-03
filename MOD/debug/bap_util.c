/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/util/bap_util.c $
 * $Revision: 24 $
 * $Modtime: 3.05.05 12:51 $
 * $Author: Clausr $
 *
 * Ersteller: 
 * Dr. Jochen Kreuzinger, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt Hilfsfunktionen fuer alle Schichten im
 *  BAP Projekt. Die Hilfsfunktionen umfassen auch den Zugriff auf 
 *  die Konfigurationstabellen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Neue Funktion BAP_RomToRamMemCpy
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 *
 ******************************************************************/


/* Systemincludes mit <...> */
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_util.h" 
#include "bap_balconfig.h"  
#include "bap_debug.h"

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

/* Definition (Implementierung) von statischen Funktionen */

/* Definition (Implementierung) von globalen Funktionen */


/*
 *  Diese Funktion wird in bap_util.h dokumentiert.
 */
BapLsgRomRow_pot BAP_IMPL_FAR
BAP_GetLsgRomRow(DBGVAR lsgId_t aLsgId)
{
    BapLsgRomRow_pot poReturnLsg = NULL;
    uint8_t u8LsgIndex = BAP_P_LSG_ROM_TABLE_ROWS;

    /* Schleife bis LsgId gefunden oder Ende der Tabelle erreicht */
    while((u8LsgIndex > (uint8_t)0) && (NULL == poReturnLsg))
    {
        u8LsgIndex--;
        if((BAP_pLsgRomTable[u8LsgIndex] != NULL) && (BAP_pLsgRomTable[u8LsgIndex]->lsgId == aLsgId))
        {   
            poReturnLsg = BAP_pLsgRomTable[u8LsgIndex];
        }
    }
    
    if (NULL == poReturnLsg)
    {
        BAP_DBG_InvalidArgError(aLsgId);
    }
    
    return poReturnLsg;
}


/*
 *  Diese Funktion wird in bap_util.h dokumentiert.
 */
BapFctRomRow_pot BAP_IMPL_FAR
BAP_GetLsgFctRomRow(BapLsgRomRow_pot apoLsgRomRow, DBGVAR fctId_t aFctId)
{
    BapFctRomRow_pot poFctRomTable;
    BapFctRomRow_pot poReturnFct = NULL;

    if (apoLsgRomRow)
    {
        DBGVAR uint8_t u8FctIndexMiddle;
        DBGVAR uint8_t u8FctIndexHighest;
        DBGVAR uint8_t u8FctIndexLowest = 0;

        /* binaerer Suchalgorithmus O(lg N) */
        poFctRomTable = apoLsgRomRow->poFctRomTable;

        u8FctIndexHighest = apoLsgRomRow->u8FctRomTableSize;

        while((u8FctIndexHighest>u8FctIndexLowest))
        {
            u8FctIndexMiddle = (uint8_t) ((uint8_t)(u8FctIndexHighest + u8FctIndexLowest) / (uint8_t)2u);

            if (poFctRomTable[u8FctIndexMiddle].fctId == aFctId)
            {
                /* passende Zeile wurde gefunden */
                poReturnFct = &poFctRomTable[u8FctIndexMiddle];
                u8FctIndexHighest = 0;  /* Schleife abbrechen */
            }
            else
            {
                if (poFctRomTable[u8FctIndexMiddle].fctId < aFctId)
                {
                    u8FctIndexLowest = (uint8_t) (u8FctIndexMiddle + (uint8_t)1u);
                }
                else
                {
                    u8FctIndexHighest = u8FctIndexMiddle;
                }
            }
        }
        /* falls keine Zeile gefunden wurde, bleibt poReturnFct = NULL */
    }
    return poReturnFct;
}


#ifndef BAP_ROM_DATA_FAR_EMPTY
/*
 *  Diese Funktion wird in bap_util.h dokumentiert.
 */
BAP_IMPL_FAR void 
BAP_RomToRamMemCpy(DBGVAR void * dest, DBGVAR BAP_ROM_DATA_FAR const void * source, DBGVAR const size_t length)
{
    size_t index;

    /*lint -e613*/
    BAP_ASSERT( NULL != dest );
    BAP_ASSERT( NULL != source );

    for(index = 0; index<length; index++)
    {   /* Kopiere von source nach dest */
        ((uint8_t *) dest)[index] = ((BAP_ROM_DATA_FAR const uint8_t *)source)[index];
    }
    /*lint +e613*/
}
#endif /* #ifndef BAP_ROM_DATA_FAR_EMPTY */


/*
 *  Diese Funktion wird in bap_util.h dokumentiert.
 */
BAP_IMPL_FAR void 
BAP_InitInternalParameters(BapInternalParameters_pot apoPars)
{
    /*lint -e668*/ /* Alle aufrufenden Funktionen wurden ueberprueft, ob sie fuer apoPars NULL uebergeben */ 
    MEMSET(apoPars, 0, sizeof(*apoPars));
    /*lint +e668*/
}

/* CallBack-Funktionen */

