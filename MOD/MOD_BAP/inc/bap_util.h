/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/util/bap_util.h $
 * $Revision: 16 $
 * $Modtime: 05-04-25 17:52 $
 * $Author: Andreasl $
 *
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
 * 2006-04-12   1.4         ALA         Unterstuetzung von symbolischen IDs in der Konfiguration.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-07-07   1.3.1       ALA         #include "bap_config.h" hinzugefuegt (wg. BapInternalParameters_t)
 * 2004-11-17   1.2         JOK         #include "bap_types.h" eingefuegt
 * 2004-11-17   1.2         JOK         struct BapLsgRomRow_t* mit const versehen
 ******************************************************************/

#ifndef BAP_UTIL_H
#define BAP_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_config.h"

/* Makro-Definitionen */

#ifdef BAP_USES_NAMED_IDS
#define BINARY_ID(lsg_or_fct_id)    ((uint8_t)(lsg_or_fct_id))
#else
#define BINARY_ID(lsg_or_fct_id)    (lsg_or_fct_id)
#endif

#ifndef BAP_ASSERT
#define BAP_ASSERT(cond)    ((void)0)
#endif

/**
 *  Ruft memcpy auf mit den fuer die Misra C Regeln relevanten Casts 
 *  @param dest     - Zieladresse
 *  @param c        - Zeichen zum Fuellen
 *  @param count    - Anzahl der zu setzenden Bytes
 */
#define MEMSET(dest, c, count)    (void) memset((void*)(dest), (c), (const size_t)(count))

/**
 *  Ruft memcpy auf mit den fuer die Misra C Regeln relevanten Casts 
 *  @param dest     - Zieladresse
 *  @param source   - Quelladresse
 *  @param length   - Anzahl der zu kopierenden Bytes
 */
#define MEMCPY(dest, source, length)    (void) memcpy((void*)(dest), (void*)(source), (const size_t)(length))

/**
 *  Diese Funktion ersetzt die Bibliotheksfunktion memcpy, um BAP_ROM_DATA_FAR Konstanten ins RAM zu kopieren.
 *  @param dest     - Zieladresse
 *  @param source   - Quelladresse
 *  @param length   - Anzahl der zu kopierenden Bytes
 */
#ifdef BAP_ROM_DATA_FAR_EMPTY
#define FAR_MEMCPY(dest, source, length)    (void) memcpy((void*)(dest), (void*)(source), (const size_t)(length))
#else
#define FAR_MEMCPY(dest, source, length)    BAP_RomToRamMemCpy(dest, source, (const size_t)(length))
#endif /* #ifdef BAP_ROM_DATA_FAR_EMPTY */

/**
 *  Ruft memcmp auf mit den fuer die Misra C Regeln relevanten Casts 
 *  @param dest     - Zieladresse
 *  @param source   - Quelladresse
 *  @param length   - Anzahl der zu vergleichenden Bytes
 */
#define MEMCMP(dest, source, length)    memcmp((void*)(dest), (void*)(source), (const size_t)(length))

/**
 *  Diese Funktion gibt den Eintrag in der FCT-ROM-Tabelle des adressierten
 *  LSG und der adressierten FCT zurueck.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet
 *  @param aFctId bezeichnet die Funktion
 *
 *  @returns
 *      Falls diese Funktion existiert einen Zeiger auf den Eintrag in der Tabelle
 *      und sonst NULL.
 */
#define BAP_GetFctRomRow(aLsgId, aFctId) BAP_GetLsgFctRomRow(BAP_GetLsgRomRow(aLsgId), (aFctId))

/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

/* Deklarationen von Funktionen */

/**
 *  Diese Funktion gibt den Eintrag in der LSG-ROM-Tabelle des adressierten
 *  LSG zurueck.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @returns 
 *      Falls der Eintrag existiert ein Zeiger auf den Eintrag in der LSG-ROM-Tabelle 
 *      und sonst NULL.
 *
 */
BapLsgRomRow_pot BAP_IMPL_FAR 
BAP_GetLsgRomRow(DBGVAR lsgId_t aLsgId);  
/*lint -sem(BAP_GetLsgRomRow ,r_null) Funktion kann NULL-Zeiger zurueckliefern */

/**
 *  Diese Funktion gibt den Eintrag in der FCT-ROM-Tabelle des adressierten
 *  LSG und der adressierten FCT zurueck
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet (darf NULL sein)
 *  @param aFctId bezeichnet die Funktion
 *
 *  @returns
 *      Falls diese Funktion existiert einen Zeiger auf den Eintrag in der Tabelle
 *      und sonst NULL.
 *
 *  @remarks
 *      liefert NULL falls apoLsgRomRow NULL ist.
 */
BapFctRomRow_pot BAP_IMPL_FAR 
BAP_GetLsgFctRomRow(BapLsgRomRow_pot apoLsgRomRow, DBGVAR fctId_t aFctId);
/*lint -sem(BAP_GetLsgFctRomRow ,r_null) Funktion kann NULL-Zeiger zurueckliefern */

#ifndef BAP_ROM_DATA_FAR_EMPTY
/**
 *  Diese Funktion ersetzt die Bibliotheksfunktion memcpy, um BAP_ROM_DATA_FAR Konstanten ins RAM zu kopieren.
 *
 *  @param dest Pointer auf den Zielspeicher
 *  @param source Pointer auf den Quellspeicher
 *  @param length Laenge der Daten die kopiert werden sollen
 */
BAP_IMPL_FAR void 
BAP_RomToRamMemCpy(DBGVAR void * dest, DBGVAR BAP_ROM_DATA_FAR const void * source, DBGVAR const size_t length);
#endif /* #ifndef BAP_ROM_DATA_FAR_EMPTY */


/**
 *  Fuehrt ein MEMSET(apoPars, 0, sizeof(*apoPars)) durch.
 *
 *  @param apoPars - Zeiger auf die zu initialisierende Struktur
 */
BAP_IMPL_FAR void 
BAP_InitInternalParameters(BapInternalParameters_pot apoPars);

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_UTIL_H */

