/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/bal/bap_balconfig.h $
 * $Revision: 40 $
 * $Modtime: 05-04-18 16:52 $
 * $Author: Andreasl $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Datentypen und Schnittstellen auf
 *  die konfigurierten Daten des Bedien- und Anzeigeprotokolls
 *  welche den BAL (Protokollschicht 6) betreffen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2004-03-30   1.3         CRI         FSG besitzt ebenfalls einen Fct.list Buffer
 * 2004-11-17   1.2         JOK         Gesonderter Datenpuffer fuer aktuelle FctList im ASG
 * 2004-09-08   1.2         JOK         Erweiterung von BapBalRomRow_t um eRx/TxDataType und u16Rx/TxSize
 *                                      fuer asymetrische Konfiguration
 * 2004-06-07   1.1         JOK         Einfuehrung der BapDt_FixedByteSequence fuer nicht-segmentierte
 *                                      ByteSequenzen
 ******************************************************************/

#ifndef BAP_BALCONFIG_H
#define BAP_BALCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_config.h"
#include "bap_privatetypes.h"

/* Makro-Definitionen */

/* Typ-Definitionen */

#ifdef BAP_ASG
/**
 * Enthaelt Status und Wert je Funktion. xValue existiert nur
 * wenn in BapBalRomRow_t das Flag bCacheAvailable wahr ist
 * und eFunctionClass ein Property ist.
 */
typedef struct BapBalRamRow_t
{
    /**
     * Enthaelt den aktuellen Cache-Status der FunctionID.
     */
    BapBalDataStatus_et eDataStatus;
    
    /**
     * Enthaelt den Cache fuer das Property.
     */
    BapData_ut xValue;
} BapBalRamRow_ot;
/* Zeiger auf BapBalRamRow_ot ist in bap_config.h definiert */
#endif /* #ifdef BAP_ASG */
 
/* Const Deklarationen */

/* Deklarationen von Funktionen */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BALCONFIG_H */

