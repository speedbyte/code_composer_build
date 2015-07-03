/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/bcl/bap_bclutil.h $
 * $Revision: 41 $
 * $Modtime: 05-04-29 14:05 $
 * $Author: Andreasl $
 *
 * Klaus Neubert, KOPF GmbH, D-76829 Landau-Moerzheim
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Deklaration (Schnittstelle) der
 *  Hilfsfunktionen fuer die BCL Schicht im BAP Projekt. Die
 *  Hilfsfunktionen umfassen auch den Zugriff auf die
 *  Konfigurationstabellen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-04-29   1.3         ALA         Anpassungen an div. Compiler, Parameteruebergabe optimiert,
 *                                      BAP_BCL_GetFctRomRow entfernt (geaenderte Datenstrukturen)
 * 2004-11-17   1.2         JOK         struct BapBclRomRow_t* wurde als const deklariert
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Funktion BAP_BCL_GetLsgStatus() entfernt
 ******************************************************************/

#ifndef BAP_BCLUTIL_H
#define BAP_BCLUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_bclconfig.h"

/* Makro-Definitionen */

/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

#ifdef BAP_USES_SEGMENTATION
/**
 *  Falls dieses Flag gestetzt ist, dann laeuft fuer mindestens eine 
 *  CAN-Id eine Intertelegramzeitueberwachung.
 */
extern bool_t BAP_bInterTelegramTimerRunning;
#endif /* BAP_USES_SEGMENTATION */

/* Deklarationen von Funktionen */

/**
 *  Diese Funktion verwaltet alle fuer die Datenelemente existierenden
 *  Sende-Inhibit-Timer. Die Timer werden bis auf "0" dekrementiert.
 *  Hat ein Timer den Wert "0" erreicht, so gilt dieser als abgelaufen.
 *
 *  Die Funktion wird vom BAP_BCL_TaskSend aufgerufen.
 */
BAP_IMPL_FAR void 
BAP_BCL_InhibitTimerTask(void);


#ifdef BAP_USES_SEGMENTATION
/**
 *  Diese Funktion verwaltet fuer alle segmentiert zu uebertragenden
 *  Datenelemente die Inter-Telegramm-Timer. Die Timer werden bis auf
 * "0" dekrementiert.
 *
 *  Hat ein Timer den Wert "0" erreicht, so gilt dieser als abgelaufen.
 */
BAP_IMPL_FAR void 
BAP_BCL_InterTelegramTimerTask(void);
#else  /* BAP_USES_SEGMENTATION */
#define BAP_BCL_InterTelegramTimerTask() ((void)0)
#endif /* BAP_USES_SEGMENTATION */

/**
 *  Diese Funktion initialisiert alle fuer den BCL relevanten
 *  RAM Bereiche eines LSGs.
 *
 *  Fuer CAN werden dabei folgende Strukturen initialisiert:
 *      - Sende-Flags fuer TX
 *      - Empfangs-Flags fuer RX
 *      - Puffer fuer RX
 *      - Segmentierungsdaten falls vorhanden bei TX
 *      - Segmentierungsdaten falls vorhanden bei RX
 *  
 *  @param apoLsgRomRow Zeiger auf den Tabelleneintrag fuer das LSG
 */
BAP_IMPL_FAR void 
BAP_BCL_InitLsgRamAreas(BapLsgRomRow_pot apoLsgRomRow);

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BCLUTIL_H */

