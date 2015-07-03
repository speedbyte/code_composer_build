/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/bcl/bap_bclbus.h $
 * $Revision: 29 $
 * $Modtime: 05-04-29 14:09 $
 * $Author: Andreasl $
 *
 * Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Deklaration (Schnittstelle) der
 *  busspezifischen Funktionen fuer die BCL Schicht im BAP Projekt.
 *
 *  Je nach Compiler-Schalter werden die Bussysteme eingebunden
 *  oder leere Funktionen dafuer erzeugt.
 *
 *  Aktuell werden folgende Bussysteme unterstuetzt:
 *      - CAN
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-11-20   1.4.1       ALA         Zur Laufzeitoptimierung der Funktion BAP_BCL_InitCanDrvTxConfirmationFlags
 *                                      den Parameter aLsgId hinzugefuegt.
 * 2005-04-29   1.3         ALA         Funktion BAP_BCL_CanReceiveTask hinzugefuegt, LIN und IPC Bus
 *                                      werden seit BAP 1.0 nicht mehr unterstuetzt. Deklarationen entfernt.
 * 2004-11-17   1.2         JOK         Funktionen BAP_BCL_Can/Lin/IPCRxNotifyTask() entfernt
 * 2004-11-17   1.2         JOK         Funktionen BAP_BCL_SendOnCan/Lin/IPC() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_LowLevelCanTxTask() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_LowLevelLinTxTask() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_ProcessNotSegmentedCanRxMsg() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_ProcessNotSegmentedLinRxMsg() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_ProcessSegmentedCanRxMsg() entfernt
 * 2004-06-07   1.1         JOK         Spezielle Funktion BAP_BCL_ProcessSegmentedLinRxMsg() entfernt
 ******************************************************************/

#ifndef BAP_BCLBUS_H
#define BAP_BCLBUS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_bclconfig.h"

/* Makro-Definitionen */

/** Konstante fuer eine nicht vorhandene Funktion */
#define BAP_DUMMY_FUNCTION      BapErr_NotImplemented

/** Konstante fuer eine leere Funktion */
#define BAP_DUMMY_VOID_FUNCTION ((void)0)

/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

/* Deklarationen von Funktionen */

#ifdef BAP_USES_CAN
/**
 *  Uebertragungsmediumspezifische Task-Funktion, die alle
 *  zu versendenden Datenelemente ueber CAN uebertraegt.
 *
 *  Die Auswahl der zu sendenden Daten wird ueber eine Heuristik
 *  gemacht, um ein faires Verhalten zwischen den einzelnen
 *  Datenelementen zu erreichen. Die Datenelemente konkurrieren
 *  um den Zugriff auf den CAN-Bus und werden noch ueber die
 *  Inhibit-Zeit zurueckgehalten.
 */
BAP_IMPL_FAR void
BAP_BCL_CanSendTask(void);
#else   /* BAP_USES_CAN */
#define BAP_BCL_CanSendTask() BAP_DUMMY_VOID_FUNCTION
#endif  /* BAP_USES_CAN */

#ifdef BAP_USES_CAN
/**
 *  Uebertragungsmediumspezifische Task-Funktion, die alle
 *  empfangenenen Datenelemente von CAN aus dem Ringpuffer
 *  an den BCL uebergibt.
 */
BAP_IMPL_FAR void
BAP_BCL_CanReceiveTask(void);
#else /* BAP_USES_CAN */
#define BAP_BCL_CanReceiveTask() BAP_DUMMY_VOID_FUNCTION
#endif /* BAP_USES_CAN */

#ifdef BAP_USES_CAN
/**
 *  Initialisiert die Tx Confirmation Flags des CAN Treibers
 *  fuer die von BAP verwendeten CAN-Ids.
 *
 *  @param apoLsgRomRow Das Steuergeraet, dessen Confirmationflags initialisiert werden
 *
 *  @remarks
 *  Die uebergebene LsgId verringert die Suchzeit fuer CAN-Ids, die von diesem Steuergeraet nicht verwendet werden.
 *  Es werden nur bei den CAN-Ids, welche nicht von gestarteten Lsgs verwendet werden, die Confirmation-Flags initialisiert.
 *
 */
BAP_IMPL_FAR void
BAP_BCL_InitCanDrvTxConfirmationFlags(BapLsgRomRow_pot apoLsgRomRow);
#else   /* BAP_USES_CAN */
#define BAP_BCL_InitCanDrvTxConfirmationFlags(apoLsgRomRow) BAP_DUMMY_VOID_FUNCTION
#endif  /* BAP_USES_CAN */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BCLBUS_H */

