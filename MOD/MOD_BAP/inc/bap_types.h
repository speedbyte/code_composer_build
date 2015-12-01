/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: UTIL
 *
 * $Archive: /dev/BAP/include/bap_types.h $
 * $Revision: 67 $
 * $Modtime: 05-04-28 14:07 $
 * $Author: Andreasl $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt alle zugelassenen Typdefinitionen im BAP
 *  Projekt, die in mehr als einer Schicht verwendet werden.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-06-02   1.4         ALA         Defines, welche in bap_user_stdtypes.h bereits vorhanden sind, entfernt.
 * 2006-04-12   1.4         MWI         Verlagerung der Standardtypen nach bap_stdtypes.h bzw bap_user_stdtypes.h 
 * 2006-04-12   1.4         ALA         Unterstuetzung von symbolischen IDs in der Konfiguration.
 * 2006-04-12   1.4         ALA         Enum Datentyp fuer BAP_Acknowledge hinzugefuegt.
 * 2006-04-12   1.4         ALA         Unterstuetzung von BAP_ROM_DATA_FAR.
 * 2005-04-25   1.3         CRI         BapErr_Aborted entfernt, muss von Applikation definiert werden
 * 2005-04-10   1.3         CRI         BapErr_Aborted hinzugefuegt fuer FSG beim Methodenabbruch
 * 2005-04-10   1.3         CRI         BapInd_Processing_CNF hinzugefuegt fuer Methoden
 * 2004-11-17   1.2         JOK         enum BapError_t ist nun in bap_defines.h definiert und hier
 *                                      nur noch zu Dokumentationszwecken enthalten.
 * 2004-09-03   1.2         JOK         Konstante Methode Indications definiert
 * 2004-09-03   1.2         JOK         Konstante Methode Requests definiert
 * 2004-09-17   1.1.1       ALA         Defines von internen FctIds von bap_config.h uebernommen, 
 *                                      da diese auch in der Applikation benoetigt werden.
 ******************************************************************/

#ifndef BAP_UTIL_BAPTYPES_H
#define BAP_UTIL_BAPTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */
#include <stddef.h>

/* Bibliotheken von externen Herstellen mit <...> */


/* Eigene Header-Dateien "..." */
#include "bap_user_stdtypes.h"
#include "bap_defines.h"


/* Makro-Definitionen */

/** Konstante fuer ein boolean TRUE */
#define BAP_TRUE    ((bool_t)1)

/** Konstante fuer ein boolean FALSE */
#define BAP_FALSE   ((bool_t)0)

#if !defined(DBGVAR)
/**
 *  Wird definiert, um bei der automatischen Definition von DBGVAR zusaetzlichen
 *  Debug-Code zu entfernen.
 */
#define BAP_AUTO_DBGVAR
/**
 *  DBGVAR kann im Makefile als -DDBGVAR=volatile angegeben werden, um Anzeigeproblemen
 *  von Variablen beim Debuggen wegen Registeroptimierung zu umgehen.
 */
#define DBGVAR
/**
 *  Je nachdem, ob DBGVAR definiert ist, ist ein cast nach (DBGVAR ptr_t) erforderlich.
 *  Wenn der Typ bereits ein ptr_t ist, darf nach MISRA Rule 44 kein cast erfolgen.
 */
#define CAST_TO_PTR(arg) (arg)
#else
#define CAST_TO_PTR(arg) (DBGVAR ptr_t)(arg)
#endif

/**
 *  BAP_ROM_CONST kann im Makefile als -DBAP_ROM_CONST=const angegeben werden, um 
 *  Compiler zu unterstuetzen, welche ein const hinter dem Datentyp benoetigen, damit
 *  diese die Variablen ins ROM legen.
 */
#if !defined(BAP_ROM_CONST)
#define BAP_ROM_CONST
#endif

/**
 *  BAP_ROM_DATA_FAR kann beim Compilieren als "#define BAP_ROM_DATA_FAR __far " oder "@far" angegeben werden
 *  dann koennen alle Variablen in den groesseren Adressbereich verschoben werden
 *  @remarks Falls diese Option nicht verwendet wird, sollte dieses Makro nicht von ausserhalb definiert werden.
 */
#if !defined(BAP_ROM_DATA_FAR)
#define BAP_ROM_DATA_FAR
#define BAP_ROM_DATA_FAR_EMPTY
#endif

/**
 *  Hiermit kann die komplette Implementierung des BAP-Stack in den FAR-Bereich gelegt werden.
 *  @remarks 
 *  Dies vergroessert den ROM Bedarf fuer den Code. 
 *  Beim Cosmic-Compiler fuer HCS 12 kann alternativ auch +modf angegeben werden.
 */
#if !defined(BAP_IMPL_FAR)
#define BAP_IMPL_FAR
#endif

/**
 *  Hiermit kann fuer alle RAM-Werte, welche vom BAP-Stack genutzt werden, ein FAR Zeiger verwendet werden.
 *  Dadurch wird es moeglich, die Daten in die Paged-Area zu legen.
 *
 *  @remarks 
 *  Fuer den Tasking Compiler fuer ST 10 muss hier shuge angegeben werden, wenn mehr als 16kBytes
 *  RAM erforderlich sind.
 *  Dies vergroessert den ROM Bedarf fuer den Code
 */
#if !defined(BAP_RAM_DATA_FAR)
#define BAP_RAM_DATA_FAR
#endif

/**
 *  Hiermit koennen alle Callbacks, welche von der Applikation implementiert werden muessen, in den FAR-Bereich gelegt werden.
 *  @remarks 
 *  Dies vergroessert den ROM Bedarf fuer den Code geringfuegig. 
 *  Beim Cosmic-Compiler fuer HCS 12 kann alternativ auch +modf angegeben werden.
 */
#if !defined(BAP_FAR)
#define BAP_FAR
#endif  /* #ifndef BAP_FAR */


/**
 *  Auswertung der Steuergeraeteklasse und Abbildung auf
 *  die Compilerschalter fuer die einzelnen Funktionaliteaten.
 */
#ifdef BAP_SGTYPE_IS_ASG_AND_LARGE_FSG
    /** Compiler-Schalter, beide Varianten zusammen */
    #define BAP_ASG
    #define BAP_FSG
    /** Compiler-Schalter: 8-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_8
    /** Compiler-Schalter: 16-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_16
    /** Compiler-Schalter: 32-Bit Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_32
    /** Compiler-Schalter: ByteSequence Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_BS
    /** Compiler-Schalter: Segmentierung von Botschaften ueber Bus einschalten */
    #define BAP_USES_SEGMENTATION
    /** Compiler-Schalter: Unterstuetzung von Acknowledge-Mechanismus */
    #define BAP_USES_ACKNOWLEDGE
    /** Compiler-Schalter: Unterstuetzung von Funktionsklasse Array einschalten */
    #define BAP_USES_ARRAYS
    /** Compiler-Schalter: Unterstuetzung von Methoden einschalten */
    #define BAP_USES_METHODS
    /** Compiler-Schalter: StatusAll als Block */
    #define BAP_USES_STATUS_ALL
    /** Compiler-Schalter: Debug/Diagnose kann fuer eine LsgId ein/ausgeschaltet werden */
    #define BAP_DBG_PER_LSG
    /** Compiler-Schalter: Begrenzung der Taskzeit durch Begrenzung der Anzahl an Indications bzw. CanTransmit-Aufrufen */
    #define BAP_TASKTIME_LIMIT
#endif
 
#ifdef BAP_SGTYPE_IS_ASG
    /** Compiler-Schalter: Spezielle Funktionalitaet eines ASG (Cache/Retry/HB-Monitoring) */
    #define BAP_ASG
    /** Compiler-Schalter: 8-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_8
    /** Compiler-Schalter: 16-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_16
    /** Compiler-Schalter: 32-Bit Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_32
    /** Compiler-Schalter: ByteSequence Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_BS
    /** Compiler-Schalter: Segmentierung von Botschaften ueber Bus einschalten */
    #define BAP_USES_SEGMENTATION
    /** Compiler-Schalter: Unterstuetzung von Acknowledge-Mechanismus */
    #define BAP_USES_ACKNOWLEDGE
    /** Compiler-Schalter: Unterstuetzung von Funktionsklasse Array einschalten */
    #define BAP_USES_ARRAYS
    /** Compiler-Schalter: Unterstuetzung von Methoden einschalten */
    #define BAP_USES_METHODS
    /** Compiler-Schalter: Debug/Diagnose kann fuer eine LsgId ein/ausgeschaltet werden */
    #define BAP_DBG_PER_LSG
    /** Compiler-Schalter: Begrenzung der Taskzeit durch Begrenzung der Anzahl an Indications bzw. CanTransmit-Aufrufen */
    #define BAP_TASKTIME_LIMIT
#endif

#ifdef BAP_SGTYPE_IS_LARGE_FSG
    /** Compiler-Schalter: Spezielle Funktionalitaet eines FSG (HB-Senden) */
    #define BAP_FSG
    /** Compiler-Schalter: 8-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_8
    /** Compiler-Schalter: 16-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_16
    /** Compiler-Schalter: 32-Bit Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_32
    /** Compiler-Schalter: ByteSequence Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_BS
    /** Compiler-Schalter: Segmentierung von Botschaften ueber Bus einschalten */
    #define BAP_USES_SEGMENTATION
    /** Compiler-Schalter: Unterstuetzung von Acknowledge-Mechanismus */
    #define BAP_USES_ACKNOWLEDGE
    /** Compiler-Schalter: Unterstuetzung von Funktionsklasse Array einschalten */
    #define BAP_USES_ARRAYS
    /** Compiler-Schalter: Unterstuetzung von Methoden einschalten */
    #define BAP_USES_METHODS
    /** Compiler-Schalter: Debug/Diagnose kann nur global ein/ausgeschaltet werden */
    #define BAP_DBG_ON_OFF
    /** Compiler-Schalter: StatusAll als Block */
    #define BAP_USES_STATUS_ALL
    /** Compiler-Schalter: Begrenzung der Taskzeit durch Begrenzung der Anzahl an Indications bzw. CanTransmit-Aufrufen */
    #define BAP_TASKTIME_LIMIT
#endif

#ifdef BAP_SGTYPE_IS_SMALL_FSG
    /** Compiler-Schalter: Spezielle Funktionalitaet eines FSG (HB-Senden) */
    #define BAP_FSG
    /** Compiler-Schalter: 8-Bit Datentyp wird unterstuetzt */
    #define BAP_USES_DATATYPE_8
    /** Compiler-Schalter: ByteSequence Datentyp wird unterstuetzt*/
    #define BAP_USES_DATATYPE_BS
    /** Compiler-Schalter: Debug/Diagnose kann nur global ein/ausgeschaltet werden */
    #define BAP_DBG_ON_OFF
#endif

/** Konstante fuer die interne FctId fuer GETALL */
#define BAP_FCTID_GETALL    ((fctId_t)1)

/** Konstante fuer die interne FctId fuer BAPCONFIG */
#define BAP_FCTID_BAPCONFIG ((fctId_t)2)

/** Konstante fuer die interne FctId fuer FUNCTIONLIST */
#define BAP_FCTID_FCTLIST   ((fctId_t)3)

/** Konstante fuer die interne FctId fuer HEARTBEAT */
#define BAP_FCTID_HEARTBEAT ((fctId_t)4)


/* Typ-Definitionen */


#ifndef BAP_DEFINES_H
/**
 *  Diese Auflistung enthaelt alle Fehlercodes von BAP.
 *  Diese werden als Rueckgabewert bei Funktionsaufrufen zurueckgegeben
 *
 *  @remark Diese Definition wird von der generierten Konfiguration ersetzt.
 *      Hier sind die Fehler nur noch zu Dokumentationszwecken enthalten.      
 */
enum BapError_t
{
    /**
     * Der Funktionsaufruf war erfolgreich.
     */
    BapErr_OK = 0x00,

    /* BCL Fehlermeldungen */

    /**
     * Sequenzbotschaft ohne vorangegangene Startbotschaft empfangen
     *
     * @remarks:
     * Dieser Fehlercode tritt in BAP 1.3 nur dann auf, wenn nach dem Ausloesen
     * des Fehlers BapErr_TimeoutSegmentation noch Sequenzbotschaften folgen.
     *
     * In Vorgaengerversionen trat der Fehler immer auf, wenn eine Sequenzbotschaft
     * ohne Startbotschaft empfangen wurde.
     *
     * Grund fuer die Aenderung: Die dynamische Kanalvergabe erlaubt es, dass eine
     * CAN-Id/Segmentierungskanal nun mehrere LSGs adressieren kann, welche auf 
     * unterschiedlichen physikalischen Steuergeraeten realisiert sein koennen.
     *
     * Ein Aussenden einer segmentierten Botschaft haette zur Folge gehabt, dass
     * im jeweils gerade nicht adressierten Steuergeraet sehr viele Fehlermeldungen 
     * ausgeloest worden waeren.
     *      
     * @deprecated  In BAP 1.4 wird dieser Fehler unterdrueckt, tritt nicht auf.
     */
    BapErr_IllegalSequence = 0x12,

    /**
     * Nicht erwartete Sequenznummer empfangen
     */
    BapErr_SequenceNumber = 0x13,

    /**
     * Folgebotschaft nicht rechtzeitig empfangen.
     * Die Intertelegrammzeit wurde verletzt.
     */
    BapErr_TimeoutSegmentation = 0x14,

    /**
     * Datenelement passt nicht in BCL Rx Puffer
     */
    BapErr_OversizeSegmentation = 0x15,

    /**
     * Beim Datenempfang wurden weniger Datenbytes empfangen wie erforderlich gewesen waeren.
     *
     * @example
     * Im ASG ist Funktion 17 als Int16 definiert.
     * Im FSG ist Funktion 17 als Int8 definiert.
     * Es erfolgt eine Datenuebertragung mit eingeschaltetem variablen DLC ueber CAN.
     * Das ASG erhaelt also 3 Bytes (2 Byte Header + 1 Byte Nutzdaten).
     * Da 1 Byte Nutzdaten nicht ausreichen, um einen 16 Bit Wert zu fuellen wird die
     * Nachricht verworfen und dieser Fehler gemeldet.
     */
    BapErr_BadDataLength = 0x16,

    /**
     * Pufferueberlauf beim Empfang von Daten. 
     * Bei korrekter Konfiguration der Ringpuffergroesse darf dieser Fehler nicht auftreten.
     */
    BapErr_ReceivedDataLost = 0x17,

    /* BPL Fehlermeldungen */

    /**
     * ASG Zeitueberwachungen Heartbeat - Cache wurde ungueltig gesetzt.
     * @remarks:
     * Dieser Fehler wird in BAP 1.3 auf der Fct-Id 4 gemeldet.
     * In frueheren BAP-Versionen wurde er auf der Fct-Id 255 gemeldet.
     */
    BapErr_TimeoutHeartbeat = 0x21,

    /**
     * Alle Retry's waren erfolglos.
     */
    BapErr_RetryNotSuccessful = 0x22,

    /**
     * Die Processing Nachricht vom FSG kam nicht rechtzeitig im ASG an
     * Wird von der gleichen Funktionalitaet im BPL ausgeloest wie BapErr_RetryNotSuccessful
     */
     BapErr_ProcessingTimeout = 0x22,

    /**
     * Der Vorgang wurde nicht ausgefuehrt weil zuviele interne Vorgaenge anstehen.
     * Genauere Beschreibung bei den Rueckgabewerten der einzelnen Funktionen.
     */
    BapErr_Busy = 0x23,

#ifdef BAP_ASG
    /**
     * Die max. zu erwartende Antwortzeit wurde ueberschritten, ohne dass der Request
     * abgesendet werden konnte.
     *
     * @remarks: Dieser Fehler kann auch bei unsegmentierten Nachrichten auftreten.
     * @remarks: Der Fehler tritt nur im ASG auf.
     */
    BapErr_RequestTimeout = 0x24,
#endif /* #ifdef BAP_ASG */

    /* BAL Fehlermeldungen */

    /**
     * Die Protokollversion des FSG passt nicht mit der des ASG zusammen
     */
    BapErr_IncompatibleProtocolVersion = 0x32,

    /**
     * Die Datenfestlegung des FSG passt nicht mit der des ASG zusammen
     */
    BapErr_IncompatibleDataSpecification = 0x33,

    /* 0x34 wird hier zweimal aufgefuehrt. Ist OK, da unterschiedliche Einsatzszenarios */

#ifdef BAP_ASG
    /**
     * Die Daten im Cache sind ungueltig
     * @remarks:
     * Tritt nur im ASG als synchroner Fehler auf
     */
    BapErr_CacheInvalid = 0x34,

    /**
     * Die Daten im Cache sind ungueltig
     * @remarks:
     * Tritt nur im ASG als asynchroner Fehler auf
     */
    BapErr_GetAllMessageCorrupted = 0x34,
#endif /* #ifdef BAP_ASG */
#ifdef BAP_FSG
    /**
     * Mindestens ein Sendepuffer wurde nicht initialisiert
     * @remarks:
     * Tritt nur im FSG als synchroner Fehler auf
     */
    BapErr_SendBufferNotInitialized = 0x34,
#endif /* #ifdef BAP_FSG */
 
    /**
     * Diese Operation ist in diesem Zustand nicht erlaubt.
     * @remarks:
     * Dieser Fehler tritt haeufig auf, wenn BAP_Init und BAP_Start nicht aufgerufen wurde 
     * oder danach BAP_Shutdown aufgerufen wurde.
     * Im ASG schlagen alle Request-Aufrufe fehl, solange keine BAPCONFIG empfangen wurde.
     */
    BapErr_InvalidState = 0x35,

    /**
     * Fuer den geforderten Cache Zugriff der angegebene FunktionID gibt es kein Cache
     */
    BapErr_CacheNotAvailable = 0x36,

    /**
     * Einer der uebergebenen Parameter ist ungueltig 
     * (Null-Zeiger, ungueltige FctId oder LsgId, Requesttyp fuer diese Funktionsklasse nicht
     * erlaubt, Datentyp der Funktion und FunktionID stimmen nicht ueberein)
     * Kann auch passieren, wenn im Kombigeraet (ASG+FSG) versucht wird eine Funktion
     * aufzurufen, die nicht fuer diesen Steuergeraetetyp erlaubt ist 
     */
    BapErr_InvalidArg = 0x37,

    /* BAP-Stack Fehlermeldungen */

    /**
     * Die angegebene Funktion wurde fuer die uebergebenen Parameter und Konfiguration nicht implementiert.
     */
    BapErr_NotImplemented = 0x42
};
#endif /* BAP_DEFINES_H */

/**
 * Dieser Typ unterstuetzt die Verwendung von BapError_et anstatt "enum BapError_t" 
 * Diese Definition muss ausserhalb des BAP_DEFINES-Blocks definiert sein, da sie sonst ignoriert wird
 */
typedef enum BapError_t BapError_et;

/**
 *  Dieser Typ beschreibt alle Request-Typen, die von der Applikation genutzt
 *  werden koennen.
 *
 *  @remark (intern)
 *  Die Enum-Werte sind fest vergeben, um einen einfachen Zugriff auf die
 *  Abbildungsmatrix fuer die Abbildung f(Req/Inc, FctKlasse) => OpCode zu erreichen.
 *
 *  @remark Es werden jeweils nur die fuer ASG oder FSG gueltigen Request-Typen definiert.
 */
typedef enum BapRequest_t
{
#ifdef BAP_ASG
    /** DataSetGet_REQ */
    BapReq_DataSetGet = 0,

    /** DataSet_REQ */
    BapReq_DataSet = 1,

    /** Start_REQ */
    BapReq_Start = 5,

    /** StartResult_REQ */
    BapReq_StartResult = 6,

    /** Abort_REQ */
    BapReq_Abort = 7,

    /** DataGet_REQ */
    BapReq_DataGet = 2,
#endif /* BAP_ASG */

#ifdef BAP_FSG
    /** Data_CNF */
    BapReq_Data = 3,

#ifdef BAP_USES_ARRAYS
    /** Changed_REQ */
    BapReq_Changed = 4,
#endif /* #ifdef BAP_USES_ARRAYS */

#ifdef BAP_USES_METHODS

    /** Processing_REQ */
    BapReq_Processing = 8,

    /** Result_REQ */
    BapReq_Result = 9,

#endif /* BAP_USES_METHODS */

#endif /* BAP_FSG */

    /** Interne Kennzeichnung fuer einen ungueltigen Request, fuer InitSendBuffer und CacheGet */
    BapReq_Invalid = 0x1f
} BapRequest_et;


/**
 *  Dieser Typ beschreibt alle Acknowledge-Typen, welche von BAP an die Applikation 
 *  gemeldet werden. @see BAP_Acknowledge.
 *  
 *  Nach dem erfolgreichen Versenden eines Requests, welcher von der Applikation
 *  an BAP durch Aufruf von BAP_Request... eingeleitet wurde, wird der entsprechende
 *  Acknowledge-Typ and die Applikation gemeldet.
 *  Dieser beinhaltet im Namen die Funktionsklasse sowie den Request-Typ, der von
 *  der Applikation an BAP uebergeben wurde.
 *
 */
typedef enum BapAcknowledge_t 
{
#ifdef BAP_ASG
    /** Ein DataSetGet-Request eines Properties wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Property_DataSetGet,

    /** Ein DataSetGet-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_DataSetGet,

    /** Ein DataSet-Request eines Properties wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Property_DataSet,

    /** Ein DataSet-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_DataSet,

    /** Ein Start-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_Start,

    /** Ein StartResult-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_StartResult,

    /** Ein Abort-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_Abort,

    /** Ein DataGet-Request eines Properties wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Property_DataGet,

    /** Ein DataGet-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_DataGet,

    /** Ein DataGet-Request (GetAll) des Caches wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Cache_DataGet,
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
    /** Ein Data-Request (Status) eines Properties wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Property_Data,

    /** Ein Error-Request eines Properties wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Property_Error,
#ifdef BAP_USES_ARRAYS
    /** Ein Data-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_Data,

    /** Ein Changed-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_Changed,

    /** Ein Error-Request eines Arrays wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Array_Error,
#endif /* #ifdef BAP_USES_ARRAYS */
#ifdef BAP_USES_METHODS
    /** Ein Processing-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_Processing,

    /** Ein Result-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_Result,

    /** Ein Error-Request einer Methode wurde vollstaendig an den Bus-Treiber uebergeben */
    BapAck_Method_Error,
#endif /* BAP_USES_METHODS */
#endif /* #ifdef BAP_FSG */
    /** Dieser Acknowledge-Typ wird von BAP nur intern verwendet und nicht an die Applikation gemeldet */
    BapAck_Nothing
} BapAcknowledge_et;


/**
 *  Dieser Typ beschreibt alle Indication-Typen, welche an die Applikation
 *  gemeldet werden koennen.
 *  Die Enum-Werte sind fest vergeben, um einen einfachen Zugriff auf die
 *  Abbildungsmatrix fuer die Abbildung f(Req/Inc, FctKlasse) => OpCode zu erreichen.
 *
 *  @remark Es werden jeweils nur die fuer ASG oder FSG gueltigen Indication-Typen definiert.
 */
typedef enum BapIndication_t
{
#ifdef BAP_ASG
    /** Data_RSP */
    BapInd_Data = 3,

    /** Reset_IND */
    BapInd_Reset = 5,

    /** Result_IND */
    BapInd_Result = 10,

#ifdef BAP_USES_ARRAYS
    /** Changed_IND */
    BapInd_Changed = 4,
#endif /* #ifdef BAP_USES_ARRAYS */

    /** Processing_IND */
    BapInd_Processing = 9,
#endif /* BAP_ASG */

#ifdef BAP_FSG
    /** DataSetGet_IND */
    BapInd_DataSetGet = 0,

    /** DataSet_IND */
    BapInd_DataSet = 1,

    /** DataGet_IND */
    BapInd_DataGet = 2,

#ifdef BAP_USES_METHODS
    /** App muss Processing schicken */
    BapInd_Processing_CNF = 5,

    /** Start_IND */
    BapInd_Start = 6,

    /** StartResult_IND */
    BapInd_StartResult = 7,

    /** AbortResult_IND */
    BapInd_Abort = 8,
#endif /* BAP_USES_METHODS */
#endif /* BAP_FSG */

    /** Interne Kennzeichnung fuer eine ungueltige Indication */
    BapInd_Invalid = 0x1f
} BapIndication_et;

#ifdef BAP_USES_NAMED_IDS
/** Der Datentyp fuer die Kennung eines Logischen Steuer-Geraetes */
typedef enum BapLsg_t lsgId_t;
#else   /* #ifdef BAP_USES_NAMED_IDS */
/** Der Datentyp fuer die Kennung eines Logischen Steuer-Geraetes */
typedef uint8_t lsgId_t;
#endif  /* #ifdef BAP_USES_NAMED_IDS */

#ifdef BAP_USES_NAMED_IDS
/** Der Datentyp fuer die Kennung einer Funktion eines Logischen Steuer-Geraetes */
typedef enum BapFct_t fctId_t;
#else   /* #ifdef BAP_USES_NAMED_IDS */
/** Der Datentyp fuer die Kennung einer Funktion eines Logischen Steuer-Geraetes */
typedef uint8_t fctId_t;
#endif  /* #ifdef BAP_USES_NAMED_IDS */

#ifdef BAP_USES_16BIT_CAN_MESSAGE_HANDLE
/** Der Datentyp nimmt die ID eines Can Message Handles oder die Can Id selbst auf.
 *  Im einen Fall wird in die XML-Datei der symbolische Name der CanMessage eingetragen.
 *  Im anderen Fall wird direkt die CAN-Id (16Bit) als Zahlenwert (z.B. 0x741) eingetragen.
 */
typedef uint16_t BapCanMsgId_t;
#else
typedef uint8_t BapCanMsgId_t;
#endif /* #ifdef BAP_USES_16BIT_CAN_MESSAGE_HANDLE */

/** Der Datentyp nimmt den Can-Kanal auf. Dieser ordnet eine CanId einem
 *  physikalischen Bus zu.
 */
typedef uint8_t BapCanInterfaceChannel_t;

/* Const Deklarationen */

/* Externe globale Variablen */

/* Deklarationen von Funktionen */


#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_UTIL_BAPTYPES_H */

