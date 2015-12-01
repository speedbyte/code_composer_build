/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAP Configuration
 *
 * $Archive: /dev/BAP/util/bap_config.h $
 * $Revision: 90 $
 * $Modtime: 29.04.05 8:36 $
 * $Author: Clausr $
 *
 * Ersteller: Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Datentypen und Schnittstellen auf
 *  die konfigurierten Daten des Bedien- und Anzeigeprotokolls
 *  welche alle Schichten betreffen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-03-14   1.4         ALA         Unterstuetzung bis zu 255 kompatiblen BAPConfig Versionen
 * 2004-11-17   1.2         JOK         Anpassung der Strukturen an die optimierten Konfigurationstabellen
 * 2004-09-17   1.1.1       ALA         Defines von internen FctIds nach bap_types.h verschoben, da diese
 *                                      auch in der Applikation benoetigt werden.
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Datentyp BapDt_FixedByteSequence eingefuegt
 * 2004-06-07   1.1         JOK         Enumeration BapSGType_t entfernt
 * 2004-06-07   1.1         JOK         Variable u8TaskTimeMs zur Berechnung der Anzahl-Tasks
 *                                      anhand der Absolutzeit eingefuegt
 * 2004-06-07   1.1         JOK         Variable fuer die Heartbeat-Zeit von 16 auf 8 Bit geaendert.
 *                                      Die Zeit ist nun statt in Anzahltasks in 100ms angegeben.
 *                                      In kleinen SGs kann damit auf den Datentyp 16 Bit verzichtet werden.
 ******************************************************************/

#ifndef BAP_CONFIG_H
#define BAP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_privatetypes.h"

/* Makro-Definitionen */

/** Konstante fuer die maximale Laenge einer ByteSequenz */
#define MAX_BYTE_SEQUENCE_LENGTH           4094

/** Konstante fuer die Laenge von BAPCONFIG */
#define BAP_FCTID_BAPCONFIG_SIZE ((uint16_t)6)

/** Konstante fuer die Position von BAPVersion Major */
#define BAP_BAPCONFIG_POS_PV_MAJOR ((uint8_t)0)

/** Konstante fuer die Position von BAPVersion Minor */
#define BAP_BAPCONFIG_POS_PV_MINOR ((uint8_t)1)

/** Konstante fuer die Position von SG-Klasse Major */
#define BAP_BAPCONFIG_POS_SG_MAJOR ((uint8_t)2)

/** Konstante fuer die Position von SG-Klasse Minor */
#define BAP_BAPCONFIG_POS_SG_MINOR ((uint8_t)3)

/** Konstante fuer die Position von Datenfestlegung Major */
#define BAP_BAPCONFIG_POS_DF_MAJOR ((uint8_t)4)

/** Konstante fuer die Position von Datenfestlegung Minor */
#define BAP_BAPCONFIG_POS_DF_MINOR ((uint8_t)5)

/** Konstante fuer die Laenge von FUNCTIONLIST */
#define BAP_FCTID_FCTLIST_SIZE ((uint16_t)8)


/* Typ-Definitionen */

/* definiert und dokumentiert in bap_balconfig.h */
struct BapBalRamRow_t;

/* definiert und dokumentiert in bap_bplconfig.h */
struct BapBplRamRow_t;

/* definiert und dokumentiert in bap_bclconfig.h */
struct BapCanRxRomRow_t;

/* definiert und dokumentiert in bap_bclconfig.h */
struct BapCanTxRomRow_t;

/* definiert und dokumentiert in bap_bclconfig.h */
struct BapBusIndirectionRxRomRow_t;

/* definiert und dokumentiert in bap_bclconfig.h */
struct BapBusIndirectionTxRomRow_t;

/**
 *  Zeiger auf BapBalRamRow_ot
 */
typedef struct BapBalRamRow_t * BapBalRamRow_pot;

/**
 *  Definiert einen Zeiger auf BapBplRamRow_ot im RAM
 */
typedef DBGVAR struct BapBplRamRow_t * BapBplRamRow_pot;

/**
 *  Definiert einen Zeiger auf BapCanTxRomRow_ot im ROM
 */
typedef BAP_ROM_DATA_FAR const struct BapCanTxRomRow_t * BapCanTxRomRow_pot;

/**
 * Definiert einen Zeiger auf BapCanRxRomRow_ot im ROM
 */
typedef BAP_ROM_DATA_FAR const struct BapCanRxRomRow_t * BapCanRxRomRow_pot;

/**
 *  Definiert einen Zeiger auf BapBusIndirectionRxRomRow_ot im ROM
 */
typedef BAP_ROM_DATA_FAR const struct BapBusIndirectionRxRomRow_t * BapBusIndirectionRxRomRow_pot;

/**
 *  Definiert einen Zeiger auf BapBusIndirectionTxRomRow_ot im ROM
 */
typedef BAP_ROM_DATA_FAR const struct BapBusIndirectionTxRomRow_t * BapBusIndirectionTxRomRow_pot;

/**
 * Enthaelt eine Bitmaske welche Arten von OpCodes beim Senden erlaubt sind.
 * Die Bedeutung der einzelnen Bits sind die Zweierpotenz der
 * Werte der Enumeration BapOpCodes_t.
 */
typedef uint8_t BapOpCodeSendMask_t;

/**
 * Enthaelt eine Bitmaske welche Arten von OpCodes beim Empfangen erlaubt sind.
 * Die Bedeutung der einzelnen Bits sind die Zweierpotenz der
 * Werte der Enumeration BapOpCodes_t.
 */
typedef uint8_t BapOpCodeReceiveMask_t;


/**
 * Zustand einer Schicht des BAP-Stacks pro LSG
 */
typedef enum BapLayerLsgStatus_t
{
    /**
     * Initial-Zustand eines LSG nach dem Hochfahren
     */
    BapLayerLsgStat_NoInit = 0,

    /**
     * Zustand des LSG nach BAP_Init() oder BAP_Shutdown()
     */
    BapLayerLsgStat_Initialized = 1,

#ifdef BAP_ASG
    /**
     * Zustand des LSG nach BAP_Start() und noch nicht erfolgter Versionpruefung.
     * Diesen Zustand gibt es nur im ASG, da hier die Versionspruefung stattfindet.
     * Das FSG wechselt nach BAP_Start() sofort in BapLayerLsgStat_Running.
     */
    BapLayerLsgStat_WaitForConfig = 2,
#endif  /* #ifdef BAP_ASG */

    /**
     * Zustand des LSG im laufenden Betrieb
     */
    BapLayerLsgStat_Running = 3
} BapLayerLsgStatus_et;


#if defined(BAP_ASG) && defined(BAP_FSG)
/**
 * Gibt fuer die BAP-Konfiguration ASG+FSG den Steuergeraetetyp des jeweiligen LSG an.
 */
typedef enum BapSGType_t
{
    /**
     * Bei dem Steuergeraet handelt es sich um ein Anzeigesteuergeraet
     */
    BapSG_ASG = 0,

    /**
     * Bei dem Steuergeraet handelt es sich um ein Funktions- oder Bediensteuergeraet
     */
    BapSG_FSG
} BapSGType_et;
#endif /* #if defined(BAP_ASG) && defined(BAP_FSG) */


/**
 * Enthaelt ein Bytearray mit u16Length Bytes
 */
typedef struct BapBufferWithLength_t
{
    /**
     * Die Anzahl an gueltigen Bytes in aru8Buffer.
     * Muss kleiner sein als der vom Konfigurationstool
     * reservierte Speicher, der kleiner oder gleich
     * MAX_BYTE_SEQUENCE_LENGTH ist.
     */
    uint16_t u16Length;

    /**
     * Enthaelt die Bytes der Byte-Sequenz. Gueltig sind
     * allerdings nur u16Length Bytes. Auf nachfolgende Bytes
     * darf nicht lesend zugegriffen werden.
     * Schreibender Zugriff ist nur unter Pruefung der konfigurierten
     * reservierten Laenge erlaubt.
     */
    uint8_t aru8Buffer[MAX_BYTE_SEQUENCE_LENGTH];
} BapBufferWithLength_ot;

/** Zeiger auf das Bytearray BapBufferWithLength_t */
typedef DBGVAR struct BapBufferWithLength_t * BapBufferWithLength_pot;


/**
 *  Definiert die Daten eines Datenelements
 */
typedef union BapData_t
{
    /**
     * Gueltig bei Datentyp = BapDt_Int8.
     */
    uint8_t  u8Value;

#ifdef BAP_USES_DATATYPE_16
    /**
     * Gueltig, falls Datentyp = BapDt_Int16.
     */
    uint16_t u16Value;
#endif

#ifdef BAP_USES_DATATYPE_32
    /**
     * Gueltig, falls Datentyp = BapDt_Int32.
     */
    uint32_t u32Value;
#endif

    /**
     * Gueltig, falls Datentyp = BapDt_ByteSequence
     * oder Datentyp = BapDt_FixedByteSequence.
     * u16Length enthaelt die Anzahl an gueltigen Bytes
     * in aru8Buffer.
     */
    BapBufferWithLength_ot oBufferWithLength;
} BapData_ut;

/** Zeiger auf BapData_t*/
typedef DBGVAR union BapData_t * BapData_put;

/**
 *  Zustaende der Heartbeatueberwachung bei ASG
 */
typedef enum BapBplStatus_t
{
    /**
     *  Bedeutung bei ASG:
     *  Frisch initialisiert, gestartet und noch
     *  kein Heartbeat-Takt empfangen
     *
     *  Bedeutung bei FSG:
     *  Frisch initialisiert.
     */
    BapBplStat_Stopped,

    /**
     *  Verbindung zu dem LSG ist OK
     */
    BapBplStat_OK,

    /**
     *  Es gab eine Zeitueberschreitung beim
     *  Empfangen von Nachrichten des LSG
     */
    BapBplStat_Timeout
} BapBplStatus_et;


/**
 *  Je nach Steuergeraetetyp eine unterschiedliche Bedeutung
 */
typedef union BapHeartbeatInfo_t
{
    /**
     *  Enthaelt den Zustand der Heartbeat-Ueberwachung.
     *  @remarks
     *  Nur bei ASG relevant.
     */
    BapBplStatus_et eStatus;

    /**
     *  Enthaelt den Index in der BAP_BplRomTable der Function, die zuletzt versendet wurde.
     *  Wird nach einem Durchlauf auf 0 gesetzt, da diese FctId reserviert ist,
     *  braucht sie auch nicht versendet zu werden.
     *
     *  @remarks
     *  Nur bei FSG relevant.
     */
    uint8_t u8LastSentFctIndex;
} BapHeartbeatInfo_ut;


#ifdef BAP_ASG
/**
 * Zustand einer FunktionsID (Datenzeile) im BAL Cache
 */
typedef enum BapBalDataStatus_t
{
    /**
     * Die Daten im Cache sind ungueltig
     */
    BapBalDataStat_Invalid,
    /**
     * Die Daten im Cache sind gueltig
     */
    BapBalDataStat_Valid,
    /**
     * Die Daten sind gueltig, es findet aber derzeit ein GetAll statt
     * Es ist aber moeglich durch HB & Status in den Valid Zustand
     * waehrend GetAll zu kommen
     */
    BapBalDataStat_GetAll
} BapBalDataStatus_et;
#endif /* #ifdef BAP_ASG */


/**
 *  Beschreibt die unterstuetzten Datentypen vom BAL
 */
typedef enum BapDataType_t
{
    /**
     *  Datenelement ohne Wert. Mit diesem Datenelement ist nur der
     *  Aufruf von BAP_RequestVoid erlaubt.
     *
     *  Wird u.a. verwendet, wenn bei Properties nur ein BapReq_DataGet
     *  erlaubt ist, aber z.B. kein DataReq_DataSetGet benoetigt wird.
     *
     *  @remarks Der Zahlenwert wird im Code auch als Datengroesse verwendet
     */
    BapDt_Void=0,

    /**
     *  Datenelement ist ein ganzzahliger Wert mit 8 Bit Genauigkeit
     *  ohne Vorzeichen
     *
     *  @remarks Der Zahlenwert wird im Code auch als Datengroesse verwendet
     */
    BapDt_Int8=1,

#ifdef BAP_USES_DATATYPE_16
    /**
     *  Datenelement ist ein ganzzahliger Wert mit 16 Bit Genauigkeit
     *  ohne Vorzeichen
     *
     *  @remarks Der Zahlenwert wird im Code auch als Datengroesse verwendet
     */
    BapDt_Int16=2,
#endif

#ifdef BAP_USES_DATATYPE_32
    /**
     *  Datenelement ist ein ganzzahliger Wert mit 32 Bit Genauigkeit
     *  ohne Vorzeichen
     *
     *  @remarks Der Zahlenwert wird im Code auch als Datengroesse verwendet
     */
    BapDt_Int32=4,
#endif

#ifdef BAP_USES_SEGMENTATION
    /**
     *  Datenelement ist eine untypisierte Byte-Sequenz mit Laengenangabe.
     *  Die maximale Laenge ist kleiner oder gleich MAX_BYTE_SEQUENCE_LENGTH.
     *  Es duerfen kuerzere Laengen versendet werden wie konfiguriert.
     */
    BapDt_ByteSequence=5,
#endif

    /**
     *  Datenelement ist eine untypisierte Byte-Sequenz mit Laengenangabe.
     *  Die maximale Laenge ist kleiner oder gleich MAX_BYTE_SEQUENCE_LENGTH.
     *  Im Gegensatz zu BapDt_ByteSequence muss immer exakt die konfigurierte
     *  Laenge versendet werden. Kuerzere Laengen sind nicht erlaubt.
     *  Wird fuer unsegmentierte Uebertragung von Bytesequenzen verwendet.
     */
    BapDt_FixedByteSequence=6,


    /**
     *  Datenelement ist kein Wert. Datenrichtung ist nicht konfiguriert.
     */
    BapDt_None=7
} BapDataType_et;


/**
 *  Enthaelt die ROM Konfiguration fuer alle Schichten je Funktions-ID
 */
typedef DBGVAR BAP_ROM_DATA_FAR const struct BapFctRomRow_t
{
    /**
     * Die LsgID auf die sich diese ROM Zeile bezieht.
     */
    lsgId_t lsgId;

    /**
     * Die FunctionID auf die sich diese ROM Zeile bezieht.
     */
    fctId_t fctId;

    /**
     * Die Funktionsklasse zu der die FunctionID gehoert.
     */
    BapFunctionClass_et eFunctionClass;

    /**
     * Maske der erlaubten OpCodes beim Senden.
     */
    BapOpCodeSendMask_t u8OpCodeSendMask;

    /**
     * Maske, der erlaubten OpCodes beim Empfangen.
     */
    BapOpCodeReceiveMask_t u8OpCodeReceiveMask;

    /**
     * Datentyp in Empfangsrichtung, der in dieser FunctionID verarbeitet wird.
     */
    BapDataType_et eRxDataType;

    /**
     * Groesse des Datentyps in Bytes in Empfangsrichtung. Fuer die Datentypen
     * BapDt_Int8 immer 1, BapDt_Int16 immer 2, BapDt_Int32 immer 4,
     * und BapDt_FixedByteSequence oder BapDt_ByteSequence
     * kleiner gleich MAX_BYTE_SEQUENCE_LENGTH
     */
    uint16_t u16RxSize;

    /**
     * Datentyp in Senderichtung, der in dieser FunctionID verarbeitet wird.
     */
    BapDataType_et eTxDataType;

    /**
     * Groesse des Datentyps in Bytes in Senderichtung. Fuer die Datentypen
     * BapDt_Int8 immer 1, BapDt_Int16 immer 2, BapDt_Int32 immer 4,
     * und BapDt_FixedByteSequence oder BapDt_ByteSequence
     * kleiner gleich MAX_BYTE_SEQUENCE_LENGTH
     */
    uint16_t u16TxSize;

    /**
     * Zeigt auf den RAM Bereich fuer diese FunctionID.
     * NULL zeigt an, dass fuer diese FunctionID kein Cache vorhanden ist.
     */
    BapBalRamRow_pot poBalRamRow;

    /** True und eDirection = BapDir_Source:
     *  Datenelement nimmt am zyklischen HB Senden teil
     */
    bool_t bHeartbeatOn;

    /** Maximale Anzahl an Wiederholungen fuer dieses LSG */
    uint8_t nMaxRetryCounter;

    /** Taskzaehler fuer Zeitueberwachung, rueckwaerts zaehlend */
    uint16_t nMaxRetryOrProcessingTime;

    /** zeigt auf eine Zeile im RAM */
    BapBplRamRow_pot poBplRamRow;

    /** Enthaelt die Anzahl der Zeilen der Tabelle poBusTxTable */
    uint8_t u8TxTableSize;

    /** Zeiger auf eine Tabelle mit allen Tx Eintraegen fuer diese FctId. */
    BapBusIndirectionTxRomRow_pot poBusTxTable;

    /** Enthaelt die Anzahl der Zeilen der Tabelle poBusRxTable */
    uint8_t u8RxTableSize;

    /** Zeiger auf eine Tabelle mit allen Rx Eintraegen fuer diese FctId. */
    BapBusIndirectionRxRomRow_pot poBusRxTable;
} BapFctRomRow_ot;

/**
 *  Zeiger auf ein BapFctRomRow_t im ROM
 */
typedef DBGVAR BAP_ROM_DATA_FAR const struct BapFctRomRow_t * BapFctRomRow_pot;


/**
 * Enthaelt Flags, welche angeben, ob etwas zu Senden ist oder
 * etwas empfangen wurde
 */
typedef struct BapLsgState_t
{
    /**
     * Gibt an, dass dieses logische Steuergeraet bei der Auswertung der Tx-Flags
     * beruecksichtigt werden muss.
     * @deprecated - wird in BAP 1.4 nicht mehr genutzt. Fuer CAN werden stattdessen die BapHeuristikFlags_t genutzt.
     */
    bitfield_t fTxFlag:1;

    /**
     * Wird als Zwischenspeicher bei der CAN Heuristik benoetigt.
     */
    bitfield_t fTxCanRequestFound:1;

    /**
     * Gibt an, dass dieses logische Steuergeraet bei der Auswertung der Rx-Flags
     * beruecksichtigt werden muss.
     */
    bitfield_t fRxFlag:1;

    /**
     * Gesamtgueltigkeit des Caches
     */
    bitfield_t fCacheValid:1;

    /**
     * Beschreibt, ob in diesem Lsg gerade eine Retry-Ueberwachung oder ein
     * Processing Timer laeuft.
     */
    bitfield_t fRetryProcessingActive:1;

    /** Reserviert fuer zukuenftige Erweiterungen */
    bitfield_t fUnused1:1;

    /** Reserviert fuer zukuenftige Erweiterungen */
    bitfield_t fUnused2:1;

    /** Reserviert fuer zukuenftige Erweiterungen */
    bitfield_t fUnused3:1;
} BapLsgState_ot;

/**
 *  Zeiger auf ein BapLsgState_ot
 */
typedef DBGVAR struct BapLsgState_t * BapLsgState_pot;

/**
 * Stati aller Schichten, welche das LSG betreffen
 */
typedef DBGVAR struct BapLsgRamRow_t
{
    /**
     *  Haelt den Status fuer das logische Steuergeraet
     */
    BapLayerLsgStatus_et eLsgStatus;

    /**
     *  Rueckwaerts zaehlender Heartbeat-Timer.
     *  Bei FSG: Timer zum Versenden von Heartbeat-Nachrichten
     *  Bei ASG: Ueberwachungstimer beim Empfang von HB-Nachrichten
     */
    uint16_t u16HeartbeatTimer;

    /** @see BapHeartbeatInfo_t */
    BapHeartbeatInfo_ut xHeartbeatInfo;

    /**
     * Enthaelt die momentan aktive Funktionsliste des ASG bzw. FSG
     */
    uint8_t BAP_aru8FunctionList[BAP_FCTID_FCTLIST_SIZE];

    /**
     * Enthaelt Flags, welche angeben, ob etwas zu Senden ist oder
     * etwas empfangen wurde
     */
    BapLsgState_ot fState;
} BapLsgRamRow_ot;

/**
 *  Zeiger auf eine RAM Zeile auf struct BapLsgRamRow_t
 */
typedef DBGVAR struct BapLsgRamRow_t * BapLsgRamRow_pot;

/**
 *  Konstanter Zeiger auf eine RAM Zeile auf struct BapLsgRamRow_t
 */
typedef DBGVAR const struct BapLsgRamRow_t * BapLsgRamRow_cpot;



/**
 * Liste der Elemente der Funktions-Id 2, BAP_CONFIG
 * @remarks
 * Bedeutung der Indizes:
 * 0 BAP Version Major          - BAP_BAPCONFIG_POS_PV_MAJOR
 * 1 BAP Version Minor          - BAP_BAPCONFIG_POS_PV_MINOR
 * 2 Steuergeraete Klasse Major - BAP_BAPCONFIG_POS_SG_MAJOR
 * 3 Steuergeraete Klasse Minor - BAP_BAPCONFIG_POS_SG_MINOR
 * 4 Datenfestlegung Major      - BAP_BAPCONFIG_POS_DF_MAJOR
 * 5 Datenfestlegung Minor      - BAP_BAPCONFIG_POS_DF_MINOR
 */
typedef uint8_t BapBAPConfig_t[BAP_FCTID_BAPCONFIG_SIZE];

/**
 * Zeiger auf ROM Tabelle BapBAPConfig_t
 */
typedef BAP_ROM_DATA_FAR const BapBAPConfig_t * BapBAPConfig_pt;

/**
 *  Gibt Zugriff auf ROM Tabellen und die LSG RAM Tabellen
 *  der einzelnen Schichten ueber die LSG ID.
 */
typedef DBGVAR const struct BapLsgRomRow_t
{
    /**
     * Beschreibt, fuer welches LSG diese Zeile gilt
     */
    lsgId_t lsgId;


#if defined(BAP_ASG)
    /**
     * Anzahl der Elemente in poBAPConfigTable fuer dieses LSG
     * @remarks
     * Im FSG wird diese Variable nicht benoetigt, da die Anzahl
     * konstant (=1) ist.
     */
    uint8_t u8BAPConfigTableSize;
#endif

    /**
     * Liste der unterstuetze Versionen im ASG
     * Aktuelle Version im FSG, nur der erste Eintrag ist relevant
     */
    BapBAPConfig_pt paru8BAPConfigTable;

    /**
     * Liste der unterstuetzten Function Identifier
     * FSG: zur Verfuegung gestellte FctIds
     * ASG: Defaultmaessig unterstuetze  FctIds (falls noch kein FctList/FctId 3 empfangen wurde)
     */
    const uint8_t BAP_aru8FunctionList[BAP_FCTID_FCTLIST_SIZE];

    /**
     * Zeiger auf den Wert des HeartBeat-Timers in Anzahl Task-Aufrufe.
     * Die Heartbeat-Zeit des FSG wird mit 100ms multipliziert und durch die
     * BAP_Task-Zeit (jeweils des FSGs/ASGs) in ms dividiert.
     * Beim ASG wird zusaetzlich der HB-Faktor multipliziert.
     */
    uint16_t *pu16HeartbeatTimer;

    /**
     * Zykluszeit in Millisekunden mit der ein Task-Aufruf stattfindet.
     */
    uint8_t u8TaskTimeMs;

    /**
     * Heartbeat-Zeit oder Faktor je nach SG-Typ
     * FSG: Zeit in 100ms Schritten mit der Heartbeats ausgesendet werden
     * ASG: Faktor mit dem die empfangene HB-Zeit des FSG multipliziert
     *      werden soll fuer die HB-Ueberwachung.
     */
    uint8_t u8Heartbeat;

    /**
     * Anzahl der Elemente in poFctRomTable fuer dieses LSG
     */
    uint8_t u8FctRomTableSize;

    /**
     * Anzahl der Elemente in poCanRxRomTable fuer dieses LSG,
     * 0 falls dieses Steuergeraet nicht den BCL CAN nutzt.
     */
    uint16_t u16CanRxRomTableSize;

    /**
     *  Zeiger auf die Funktionstabelle im ROM fuer dieses LSG
     */
    BapFctRomRow_pot poFctRomTable;

    /** Zeiger auf den Anfang der BCL CanRx Tabelle im ROM
     *  fuer dieses LSG bzw. NULL falls kein BCL CAN verwendet
     *  wird.
     */
    BapCanRxRomRow_pot poCanRxRomTable;

    /** Falls diese Spalte BAP_TRUE ist, dann wird beim Senden
     *  immer die Laenge 8 an BAP_CANUBS_TxData uebergeben.
     *
     *  Falls die Spalte BAP_FALSE ist, dann wird beim Senden
     *  immer die aktuelle Laenge an BAP_CANUBS_TxData uebergeben.
     *
     *  @remarks:
     *  Dadurch ist es moeglich, je Lsg festzulegen, ob es kompatibel zu BAP 1.1
     *  mit fester DLC-Laenge = 8 oder kompatibel zu BAP 1.2 mit variablem DLC sein soll.
     */
    bool_t bUseDLC8;

#if defined(BAP_ASG) && defined(BAP_FSG)
    /**
     * Gibt an, ob es sich bei dem Steuergeraet um ein ASG oder ein FSG handelt.
     * Diese Spalte wird nur verwendet, wenn ASG+FSG zusammen in einem Steuergeraet vorhanden sind.
     */
    BapSGType_et eSGType;
#endif /* #if defined(BAP_ASG) && defined(BAP_FSG) */

    /**
     * Zeiger auf die Zeile im RAM, die die Zustaende dieses LSG enthalten
     */
    BapLsgRamRow_pot poLsgRamRow;

   /**
     * Anzahl der Elemente in poCanTxRomTable fuer dieses LSG,
     * 0 falls dieses Steuergeraet nicht den BCL CAN nutzt.
     */
    uint16_t u16CanTxRomTableSize;

    /** Zeiger auf den Anfang der BCL CanRx Tabelle im ROM
     *  fuer dieses LSG bzw. NULL falls kein BCL CAN verwendet
     *  wird.
     */
    BapCanTxRomRow_pot poCanTxRomTable;
} BapLsgRomRow_ot;

/**
 *  Definiert einen Zeiger auf BapLsgRomRow_ot im ROM
 */
typedef DBGVAR BAP_ROM_DATA_FAR const struct BapLsgRomRow_t * BapLsgRomRow_pot;

/**
 *  Gemeinsame Parameterliste fuer interne Funktionen.
 *
 *  @remarks:
 *  Dient der Optimierung des Stackframes und Reduzierung von Suchvorgaengen.
 */
typedef DBGVAR struct BapInternalParameters_t
{
    /** Enthaelt die LSG-ID oder 0 */
    lsgId_t lsgId;

    /** Enthaelt die FCT-ID oder 0 */
    fctId_t fctId;

    /** Zeigt auf das aktuelle LSG oder NULL, falls kein LSG zugeordnet werden kann */
    BapLsgRomRow_pot poLsgRomRow;

    /** Zeigt auf die aktuelle FCT oder NULL, falls die gesuchte FCT nicht existiert */
    BapFctRomRow_pot poFctRomRow;

    /** Zeigt auf die aktuellen Daten */
    DBGVAR ptr_t pData;

    /** Enthaelt die Laenge der aktuellen Daten */
    uint16_t u16Length;

    /** Enthaelt den aktuellen OpCode */
    BapOpCodes_et eOpCode;
} BapInternalParameters_ot;

/**
 *  Definiert einen nicht konstanten Stack-Zeiger auf BapInternalParameters_ot;
 */
typedef DBGVAR struct BapInternalParameters_t * BapInternalParameters_pot;

/**
 *  Definiert einen Stack-Zeiger auf ein konstantes BapInternalParameters_ot;
 */
typedef DBGVAR const struct BapInternalParameters_t * BapInternalParameters_cpot;


/* Const Deklarationen */

/**
 *  Einsprungstabelle auf alle moeglichen Konfigurationen im Steuergeraet.
 *
 *  Im ASG kann zu einer LsgId mehrere Konfigurationen hinterlegt sein.
 *  Im FSG muss es zu jeder LsgId genau eine Konfiguration geben.
 */
extern BAP_ROM_DATA_FAR BapLsgRomRow_ot BAP_ROM_CONST BAP_LsgRomTables[];

/* Externe globale Variablen */

/**
 *  Einsprungtabelle auf die aktuell ausgewaehlte Konfiguration.
 *
 *  Dazu muss die LsgId in der Tabelle gesucht werden,
 *  Index und LsgId sind nicht synchron, da die Daten
 *  gepackt vorgehalten werden.
 *
 *  @remarks:
 *  Diese Tabelle liegt im RAM, die Zeiger koennen geaendert werden,
 *  nicht jedoch die Werte, auf die der Zeiger zeigt.
 *
 *  Diese Tabelle darf/sollte keine NULL-Zeiger enthalten.
 *
 *  Diese Tabelle muss vor dem Aufruf von BAP_Init initialisiert
 *  worden sein, i.d.R. macht dies der Startup-Code der C-Bibliothek.
 *
 *  Ansonsten siehe Beispiel initialization.c im Integrationsordner.
 */
extern BapLsgRomRow_pot BAP_pLsgRomTable[];

/* Deklarationen von Funktionen */

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_CONFIG_H */

