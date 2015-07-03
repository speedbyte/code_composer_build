/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/bal/bap_balutil.h $
 * $Revision: 65 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Dr. Jochen Kreuzinger, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt Hilfsfunktionen fuer die BAL Schicht im
 *  BAP Projekt. Die Hilfsfunktionen umfassen auch den Zugriff auf 
 *  die Konfigurationstabellen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer kompatible Erweiterung.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer neue Versionspruefung.
 * 2006-04-12   1.4         ALA         Fehler behoben: InitSendBuffer kann nun fuer 
 *                                      jedes Property aufgerufen werden,
 *                                      unabhaengig von der dynamischen FctList   
 * 2006-03-24   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2006-03-24   1.4         ALA         Nur in bap_balutil.c verwendete Funktionen statisch definiert
 * 2005-04-29   1.3         CRI         Anpassungen der Kommentare fuer BAP1.3
 * 2005-04-13   1.3         ALA         Optimierung der Datenstrukturen
 * 2005-03-18   1.3         CRI         Rueckgabewert bei BAP_BAL_UpdateCache
 * 2004-10-21   1.2         JOK         Hilfsfunktionen aus bap_bal in bap_balutil verschoben
 * 2004-09-03   1.2         JOK         Dimension von BAP_Request2OpCodeMatrix 
 *                                      an Funktionsklasse Methode angepasst
 * 2004-09-03   1.2         JOK         Dimension von BAP_OpCode2IndicationMatrix 
 *                                      an Funktionsklasse Methode angepasst
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 ******************************************************************/

#ifndef BAP_BALUTIL_H
#define BAP_BALUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_balconfig.h"

/* Makro-Definitionen */

/** Konstante fuer die benoetigten Bytes pro void */
#define VOID_SIZE ((uint16_t)0u)

/** Konstante fuer die benoetigten Bytes pro uint8_t */
#define INT8_SIZE ((uint16_t)1u)

#ifdef BAP_USES_DATATYPE_16
/** Konstante fuer die benoetigten Bytes pro uint16_t */
#define INT16_SIZE ((uint16_t)2u)
#endif /* #ifdef BAP_USES_DATATYPE_16 */

#ifdef BAP_USES_DATATYPE_32
/** Konstante fuer die benoetigten Bytes pro uint32_t */
#define INT32_SIZE ((uint16_t)4u)
#endif /* #ifdef BAP_USES_DATATYPE_32 */

/** Konstante fuer die groesst moegliche FctID */
#define MAX_FCT_ID 63

/* Typ-Definitionen */

/**
 * Dieser Datentyp wird verwendet, um bei der Pruefung des Datentyps zwischen skalaren Werten 
 * und variablen Werten zu unterscheiden.
 */
typedef enum BapBalCheckDataType_t
{
    /** Pruefung fuer Skalare Groessen VOID, INT8, INT16, INT32, FIXEDBYTESEQUENCE bei BAP_RequestByteSequence */
    BapChkDt_ScalarIn,

#ifdef BAP_FSG
    /** Pruefung fuer Skalare Groessen VOID, INT8, INT16, INT32, FIXEDBYTESEQUENCE bei BAP_InitSendBufferByteSequence */
    BapChkDt_ScalarInit,
#endif /* #ifdef BAP_FSG */

    /** Pruefung fuer Skalare Groessen VOID, INT8, INT16, INT32, FIXEDBYTESEQUENCE bei BAP_CacheGetRequestByteSequence */
    BapChkDt_ScalarOut,

    /** Pruefung fuer BAP_RequestVoid aller Datentypen */
    BapChkDt_VoidIn,

    /** Pruefung fuer BYTESEQUENCE bei BAP_RequestByteSequence */
    BapChkDt_VariableIn,

#ifdef BAP_FSG
    /** Pruefung fuer BYTESEQUENCE bei BAP_InitSendBufferByteSequence */
    BapChkDt_VariableInit,
#endif /* #ifdef BAP_FSG */

    /** Pruefung fuer BYTESEQUENCE bei BAP_CacheGetRequestByteSequence */
    BapChkDt_VariableOut
} BapBalCheckDataType_et;

/* Const Deklarationen */

/* Externe globale Variablen */

/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,Request) auf OpCode
 */
#if defined(BAP_ASG) && defined(BAP_FSG)
#ifdef BAP_USES_METHODS
extern const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][4][10];
#else
extern const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][3][10];
#endif /* BAP_USES_METHODS */
#else
#ifdef BAP_USES_METHODS
extern const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][10];
#else
extern const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][10];
#endif /* BAP_USES_METHODS */
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */

/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Indication
 */
#if defined(BAP_ASG) && defined(BAP_FSG)
#ifdef BAP_USES_METHODS
extern const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][4][7];
#else
extern const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][3][7];
#endif /* BAP_USES_METHODS */
#else
#ifdef BAP_USES_METHODS
extern const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][7];
#else
extern const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][7];
#endif /* BAP_USES_METHODS */
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */

/* Deklarationen von Funktionen */

/**
 *  Diese Funktion prueft den Request-Aufruf auf Gueltigkeit und liefert
 *  einen entsprechenden Fehler (auch BapErr_OK) zurueck.
 *
 *  @param apoPars -\>lsgId [in] bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId [in] bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>u16Length [in] gibt die zur pruefende Datengroesse an.<br>
 *                 -\>poLsgRomRow [out] gibt ein Zeiger auf die BapLsgRomRow der angegebenen LsgId zurueck,
 *                    falls der Request zulaessig ist und sonst NULL.<br>
 *                 -\>poFctRomRow [out] gibt ein Zeiger auf die BapFctRomRow der angegebenen FctId zurueck,
 *                    falls der Request zulaessig ist und sonst NULL.<br>
 *                 -\>eOpCode [out] gibt den aus dem Request resultierenden OpCode zurueck
 *
 *  @param aeRequest bezeichnet die Art des Requests der durchgefuehrt werden soll.
 *
 *  @param eCheckDataType gibt an, nach welchem Algorithmus die Pruefung der Groesse eines Datentyps erfolgen soll.
 *
 *  @retval BapErr_OK
 *      die Pruefung war okay
 *  @retval BapErr_InvalidArg
 *      der gepruefte Request passt nicht zur Konfiguration
 *  @retval BapErr_InvalidState
 *      Das LSG ist im falschen Zustand (ungleich erwarteter Zustand)
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckRequest(BapInternalParameters_pot apoPars
    , DBGVAR BapRequest_et aeRequest
    , BapBalCheckDataType_et eCheckDataType);
    
    
/**
 *  Diese Funktion prueft den DataReceived-Aufruf auf Gueltigkeit und liefert
 *  einen entsprechenden Fehler (auch BapErr_OK) zurueck.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poLsgRomRow bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>poFctRomRow bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>eOpCode bezeichnet den OpCode, welcher die Operation der Daten beschreibt.<br>
 *                 -\>pData zeigt untypisiert auf eine Bytefolge, deren Laenge in au16Length 
 *                    uebergeben wird, und die empfangenen Daten enthaelt<br>
 *                 -\>u16Length enthaelt die Laenge der empfangenen Daten
 *
 *  @retval BapErr_OK
 *      die Pruefung war okay
 *  @retval BapErr_InvalidArg
 *      der gepruefte Request passt nicht zur Konfiguration
 *  @retval BapErr_InvalidState
 *      es wurde was Empfangen obwohl der BAP-Stack noch nicht gestartet wurde
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckDataReceived(BapInternalParameters_cpot apoPars);
    
    
#ifdef BAP_ASG
/**
 *  Diese Funktion setzt den gesamten Cache des LSG und jeden einzelnen
 *  der FunktionIds auf invalid. Zusaetzlich wird in den FctList-Puffer 
 *  die konfigurierte FctList des ASG eingetragen, da fuer die Pruefung der
 *  Cachegueltigkeit immer eine FctList zugrunde liegen muss. 
 *  Diese Funktion wird zu Beginn bzw. im Fehlerfall des BAP Stacks aufgerufen.
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet. Uebergebener Parameter 
 *  darf nicht NULL sein.
 */
BAP_IMPL_FAR void
BAP_BAL_InvalidateCache(BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Diese Funktion gibt den Gesamt-Cache-Status des adressierten LSG zurueck.
 *
 *  @param apoLsgRomRow bezeichnet das logische Steuergeraet.
 *      NULL darf nicht uebergeben werden.
 *
 *  @returns 
 *      Cache-Status des LSG entsprechend des BapBalDataStatus_et
 *  @see BapBalDataStatus_t
 */
BAP_IMPL_FAR DBGVAR BapBalDataStatus_et
BAP_BAL_GetLsgCacheStatus(BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Diese Funktion prueft im ASG die BAPConfig Botschaft. Es wird die Major und Minor Number
 *  der Protokollversion, die Steuergeraeteklasse und die Major Number der
 *  Datenfestlegung auf Gleichheit mit einen Eintrag aus der Referenzliste im ASG
 *  verglichen.
 *
 *  Aktionen:
 *      - Bei einer Reset-Indication wird der Cache auf ungueltig gesetzt und
 *        Retries/Rx/Tx der unteren Schichten ausgeschaltet
 *      - Bei einer fehlerhaften Protokollversion wird eine ErrorIndication mit dem 
 *        Fehlerwert BapErr_IncompatibleProtocolVersion gemeldet (Weiterarbeit mit der Alten).
 *      - Bei einer fehlerhaften SGKlasse oder Datenfestlegung wird eine ErrorIndication
 *        mit dem Fehlerwert BapErr_IncompatibleDataSpecification gemeldet (Weiterarbeit mit der Alten).
 *      - Bei einer gueltigen BAPConfig wird in den Zustand running gewechselt.
 *
 *  @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>fctId ist in diesem Fall BAPCONFIG<br>
 *                 -\>poLsgRomRow bezeichnet das logische Steuergeraet.<br>
 *                 -\>eOpCode bezeichnet den OpCode, welcher die Operation der Daten beschreibt.
 *
 *  @param apData beinhaltet die Daten der BAPConfig Botschaft
 *
 *
 *  @retval BAP_TRUE Die Konfiguration ist ungueltig
 *  @retval BAP_FALSE Die Konfiguration ist gueltig
 *
 *  @remarks
 *  Die Minor-Nummern der Datenfestlegung werden seit BAP 1.4 ignoriert.
 *  Ausserdem muss die komplette Zeile uebereinstimmen. 
 *  Es wird nicht mehr wie in BAP 1.0-1.3 spaltenweise geprueft.
 */
BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_CheckBapConfigAsg(BapInternalParameters_cpot apoPars
    , DBGVAR const volatile_ptr_t apData);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_FSG
/**
 *  Diese Funktion initialisiert die Standard-Sendepuffer im FSG.
 *  2-4 (BAPCONFIG, FCTLIST, HEARTBEAT)
 *
 *  @param apoLsgRomRow 
 *      muss auf das logische Steuergeraet zeigen (NULL ist verboten)
 *
 *  @returns Fehlercode, falls dies nicht funktioniert hat. 
 *      Dann liegt allerdings ein Konfigurationsproblem vor.
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_InitFSGBuffers(BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifdef BAP_FSG */


#ifdef BAP_USES_DATATYPE_16
/**
 *  Diese Funktion wandelt einen 16-Bit Wert in einen Datenstrom im
 *  Little-Endian Format um. 
 *
 *  @param au16Value gibt den 16-Bit Wert an. 
 *
 *  @param apPtr Rueckgabewert im Little-Endian Format als Byte-Array.
 *
 */
BAP_IMPL_FAR void 
BAP_BAL_Int16toDataStream(DBGVAR uint16_t au16Value, DBGVAR uint8_t *apPtr);
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_16
/**
 *  Diese Funktion wandelt einen Datenstrom im
 *  Little-Endian Format in einen 16-Bit Wert um. 
 *
 *  @param apPtr Byte-Array im Little-Endian Format das zu wandlen ist.
 *
 *  @returns gewandelter 16-Bit Wert 
 *
 */
BAP_IMPL_FAR DBGVAR uint16_t 
BAP_BAL_DataStreamToInt16(DBGVAR volatile_ptr_t apPtr);
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_32
/**
 *  Diese Funktion wandelt einen 32-Bit Wert in einen Datenstrom im
 *  Little-Endian Format um. 
 *
 *  @param au32Value gibt den 32-Bit Wert an. 
 *
 *  @param apPtr Rueckgabewert im Little-Endian Format als Byte-Array.
 *
 */
BAP_IMPL_FAR void 
BAP_BAL_Int32toDataStream(DBGVAR uint32_t au32Value, DBGVAR uint8_t *apPtr);
#endif /* BAP_USES_DATATYPE_32 */


#ifdef BAP_USES_DATATYPE_32
/**
 *  Diese Funktion wandelt einen Datenstrom im
 *  Little-Endian Format in einen 32-Bit Wert um. 
 *
 *  @param apPtr Byte-Array im Little-Endian Format das zu wandlen ist.
 *
 *  @returns gewandelter 32-Bit Wert 
 *
 */
BAP_IMPL_FAR DBGVAR uint32_t 
BAP_BAL_DataStreamToInt32(DBGVAR volatile_ptr_t apPtr);
#endif /* BAP_USES_DATATYPE_32 */

/**
 * Diese Funktion testet ob die uebergebene Funktion in der aktuellen Konfiguration erlaubt ist.
 * Valid ist eine Funktion, wenn sie zwischen 1 und 64 liegt und in der
 * aktuellen Fct.list enthalten ist.
 * 
 * @param apoFctRomRow bezeichnet die Fct.Id welche auf Gueltigkeit getestet werden soll
 *
 * @param apoLsgRamRow bezeichnet die LSG Konfiguration des zugehoerigen LSG
 *
 * @remark Falls das logische Steuergeraet im Init-Zustand ist, wird nicht gegen die aktuelle
 *         Funktionsliste geprueft. Erlaubt das Vorbelegen aller Sendepuffer unabhaengig von
 *         der aktivierten dynamischen Fct.list.
 * 
 * @returns BAP_TRUE  wenn die Funktion erlaubt ist 
 *          BAP_FALSE wenn nicht 
 *
 */
BAP_IMPL_FAR DBGVAR bool_t 
BAP_BAL_CheckIfFunctionIsValid(BapFctRomRow_pot apoFctRomRow
    , BapLsgRamRow_cpot apoLsgRamRow);

#ifdef BAP_ASG
/**
 * Alle Spezialbehandlungen die beim Aufruf von BAP_RequestVoid(lsgId, fctId, BapReq_DataGet)
 * vorkommen, werden hier behandelt.
 *
 * @param apoPars -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                -\>poLsgRomRow bezeichnet das logische Steuergeraet.<br>
 *                -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                -\>poFctRomRow bezeichnet die Funktion des angegebenen logischen Steuergeraetes.
 *
 * @remarks: Diese Funktion wird nur von BAP_RequestVoid genutzt. Verringert
 * Groesse und zyklomatische Komplexitaet.
 */
BAP_IMPL_FAR BapError_et 
BAP_BAL_RequestVoidGet(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG 
/**
 *  Diese Funktion fuehrt alle Spezialbehandlungen im FSG beim Empfang einer Nachricht durch.
 *  Sie wird von BAP_BPL_DataReceived aufgerufen.
 *  
 *  @param apoPars - [in] enthaelt die empfangenen Daten
 *  @param aeIndication - [in] enthaelt die aus dem OpCode ermittelte Indication.
 *
 *  @remarks: Diese Funktion darf nur fuer FSGs aufgerufen werden.
 *
 *  @returns den Datentyp, welcher als Indication an die Applikation gemeldet werden soll.
 */
BAP_IMPL_FAR BapDataType_et 
BAP_BAL_FsgDataReceived(BapInternalParameters_cpot apoPars
    , DBGVAR const BapIndication_et aeIndication);
#endif /* #ifdef BAP_FSG */

#ifdef BAP_ASG 
/**
 *  Diese Funktion fuehrt alle Spezialbehandlungen im ASG beim Empfang einer Nachricht durch.
 *  Sie wird von BAP_BPL_DataReceived aufgerufen.
 *  
 *  @param apoPars - [in] enthaelt die empfangenen Daten. apoPars->pData wird nicht genutzt.
 *
 *  @param apConvertedData - [in] enthaelt die konvertierten Daten, welche aus apoPars->pData aufbereitet wurden
 *
 *  @param apeIndication - [in/out] enthaelt einen Zeiger auf die aus dem OpCode ermittelte Indication.
 *  Der Wert aendert sich auf BapInd_Data, falls ein HeartbeatStatus einen Wert im Cache
 *  geaendert hat.
 *
 *  @remarks: Diese Funktion darf nur fuer ASGs aufgerufen werden.
 *
 *  @returns den Datentyp, welcher als Indication an die Applikation gemeldet werden soll.
 */
BAP_IMPL_FAR BapDataType_et 
BAP_BAL_AsgDataReceived(DBGVAR volatile_ptr_t apConvertedData
    , BapInternalParameters_cpot apoPars
    , DBGVAR BapIndication_et * const apeIndication);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Diese Funktion ermittelt den Acknowledge-Typ anhand der Funktionsklasse und des Opcodes
 *
 *  @param apoFctRomRow enthaelt die Funktionsklasse und den Steuergeraete-Typ (ASG/FSG)
 *  @param aeOpCode enthaelt den gesendeten OpCode
 */
BAP_IMPL_FAR BapAcknowledge_et
BAP_BAL_GetAcknowledge(BapFctRomRow_pot apoFctRomRow, BapOpCodes_et aeOpCode);
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_BALUTIL_H */

