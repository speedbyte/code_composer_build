/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 * 
 * $Archive: /dev/BAP/bal/bap_bal.h $
 * $Revision: 17 $
 * $Modtime: 29.04.05 11:12 $
 * $Author: Clausr $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Schnittstelle zur BAL Schicht im BAP
 *  Projekt. Diese Schnittstelle darf nicht ausserhalb der BAP
 *  Komponente verwendet werden, da sie in zukuenftigen Versionen
 *  Aenderungen unterliegen darf.
 *
 *  Stattdessen ist die freigegebene Schnittstelle bap.h zu ver-
 *  wenden.
 *
 *  Diese Header-Datei enthaelt nicht die Funktionen BAP_Request* und
 *  BAP_Indication*, da diese in bap.h deklariert sind und in der
 *  Implementierungsdatei definiert sind.
 *  Die BAP_Indication* muessen allerings in der Applikation
 *  implementiert werden, welche den BAP Stack nutzt.
 *
 *  Ausserdem enthaelt sie nicht die CallBack-Funktionen des BPL,
 *  BAP_BPL_Received und BAP_BPL_ErrorIndication (bap_bpl.h), 
 *  welche vom BAL implementiert werden.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 *
 ******************************************************************/

#ifndef BAP_BAL_BAL_H
#define BAP_BAL_BAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap.h"

/* Makro-Definitionen */

/* Vordefinierte FunctionIds */

/* Typ-Definitionen */

/* Const Deklarationen */

/* Externe globale Variablen */

/* Deklarationen von Funktionen */

/* 
 *  Alle Funktionen von bap_bal.c sind in bap.h deklariert.
 *  Ausnahme bilden die CallBack-Funktionen des BPL, welche
 *  in bap_bpl.h deklariert sind.
 */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BAL_BAL_H */

