/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/bal/bap_balutil.c $
 * $Revision: 161 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
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
 * 2006-06-02   1.4         ALA         Unterstuetzung von Bap_Acknowledge
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer kompatible Erweiterung.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer neue Versionspruefung.
 * 2006-04-12   1.4         ALA         Fehler behoben: InitSendBuffer kann nun fuer jedes Property aufgerufen werden
 *                                      unabhaengig von der dynamischen FctList
 * 2006-03-24   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2006-03-24   1.4         ALA         Nur in bap_balutil.c verwendete Funktionen statisch definiert
 * 2005-11-14   1.3.1.3     ALA         C++ Kommentar entfernt
 * 2005-10-24   1.3.1.2     ALA         Kommentarzeichen hinter #endif ergaenzt, Lint-Meldung im FSG entfernt.
 * 2005-08-02   1.3.1.1     ALA         Bugfix: BAP_CacheGetRequestIntXYZ fuer Properties mit Tx-Datentyp void
 *                                      lieferte zuvor InvalidArg.
 * 2005-05-03   1.3         CRI         Entfernung von BAP_BAL_SetLsgRomRow, da jetzt direkt
 * 2005-05-02   1.3         CRI         Bugfix: Status des Einzelcaches zwischenspeichern
 *                                      vor Aufruf von UpdateCache
 * 2005-04-29   1.3         CRI         InitpLsgRomRow() entfernt
 * 2005-04-29   1.3         CRI         Gesamt Cache Status in globales Flag lsgState.tCacheValid
 * 2005-04-25   1.3         CRI         BUGFIX: Wenn ASG im WaitForConfig Zustand ist,
 *                                      muss der Erhalt einer BAPCONFIG eine IndBS ausloesen
 * 2005-04-18   1.3         CRI         Gesamt Cache Status in globale LSG Variable
 * 2005-04-18   1.3         CRI         Ext. Fct.list Anpassung in CheckIfFctValid
 * 2005-04-13   1.3         ALA         Optimierung der Datenstrukturen
 * 2005-04-12   1.3         CRI         ASG&FSG zusammen
 * 2005-04-11   1.3         CRI         FixedByteSequence muss exakt Laenge 6 haben (CheckDatatype)
 * 2005-04-11   1.3         CRI         BapInd_Processing_CNF fuer das FSG hinzugekommen
 * 2005-04-11   1.3         CRI         Eine StatusAll Indication deaktiviert Zeitueberwachung fuer GetAll
 * 2005-04-10   1.3         CRI         Im FSG darf kein Request auf Fct-Id 0-7 gemacht werden
 * 2005-04-08   1.3         CRI         Hinzufuegen von BAP_BPL_AbortSend (Aufruf von BCL und loeeschen Retry)
 * 2005-04-08   1.3         CRI         Man kann jetzt bereits mit einem Fct.list Subset im ASG starten
 * 2005-04-04   1.3         CRI         Erweiterung von CheckDatatype um Funktionstyp VOID fuer asym. Prop.
 * 2005-04-04   1.3         CRI         Fuer das FSG werden beim Fct.list Zugriff statt der ROM Table
 *                                      die RAM-Table benutzt
 * 2005-04-01   1.3         CRI         Hinzufuegen der CheckIfFunctionValid Funktion
 * 2005-03-31   1.3         ALA         Interruptsperre entfernt
 * 2005-03-20   1.3         CRI         InitFSGBuffers, CheckDataReceived, CheckRequest, InvalidateCache
 *                                      an dynamische Fct.list angepasst
 *                                      (Benutzung des RAM Eintrags bzw. sperren nicht unterstuetzter Fcts)
 * 2005-03-24   1.2.1       CRI         In CheckBapConfigAsg: Bei Wechseln der Konfiguration durch einen
 *                                      Reset muessen die BPL/BCL Schicht neu initialisiert werden
 * 2005-03-18   1.3         CRI         Rueckgabewert bei BAP_BAL_UpdateCache entsprechend einer Wertaenderung
 * 2004-03-14   1.2.1       CRI         Bugfix: Illegaler Arrayzugriff auf Fct-Id 255 entfernt (2mal)
 * 2004-11-17   1.2         JOK         Auswertung der StatusAll-Block Nachricht eingebaut
 * 2004-10-21   1.2         JOK         Hilfsfunktionen aus bap_bal in bap_balutil verschoben
 * 2004-09-03   1.2         JOK         Inhalt von BAP_OpCode2IndicationMatrix
 *                                      an Funktionsklasse Methode angepasst
 * 2004-09-03   1.2         JOK         Inhalt von BAP_Request2OpCodeMatrix
 *                                      an Funktionsklasse Methode angepasst
 * 2004-08-25   1.2         JOK         Cache-Gueltigkeit abhaengig von FunctionList des FSG
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Laufzeitoptimierungen in Funktionen durch ersetzen der aLsgId
 *                                      durch den Zeiger apoLsgRomRow in die LSG-Tabelle
 * 2004-06-07   1.1         JOK         Includes bap_bal.h entfernt
 * 2004-06-07   1.1         JOK         BAP_OpCode2IndicationMatrix je nach SG-Type definiert, um
 *                                      Opcode-Doppeldeutigkeit aufzuloesen.
 * 2004-06-07   1.1         JOK         FctId 4 (Heartbeat) wird nicht mehr fuer Cache-Gueltigkeit
 *                                      ausgewertet.
 ******************************************************************/


/* Systemincludes mit <...> */
#include <string.h>  /* fuer memcpy() */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_balconfig.h"
#include "bap_balutil.h"
#include "bap_debug.h"
#include "bap_util.h"
#include "bap_bpl.h"
#include "bap_bal.h"
#include "bap_config.h"


/* Externe globale Variablen-Definitionen */

/* Fuer die 3 verschiedenen Konfigurationen (ASG, FSG, ASG & FSG)
   werden im folgenden die entsprechenden Matrizen fuer die
   Gueltigkeitpruefungen OPC-->IND und REQ-->OPC angelegt */

/* ---------------------------------------------------------------------------------------------- */
#if defined(BAP_ASG) && !defined(BAP_FSG)
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,Request) auf OpCode
 * Diese Tabelle ist im ASG bzw. FSG unterschiedlich
 */
const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][10] =
{
    /* Funktionsklasse Property */
    {
        BapOp_PropSetGet,       BapOp_PropReset_Set,    BapOp_PropGet,        BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    },
    /* Funktionsklasse Cache */
    {
        BapOp_CacheSetGetAll,   BapOp_CacheSetAll,      BapOp_CacheGetAll,    BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapOp_ArrSetGet,        BapOp_ArrSet,           BapOp_ArrGet,         BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_Invalid,
        BapOp_Invalid,          BapOp_MethStart,        BapOp_MethStartResult,
        BapOp_MethAbort,        BapOp_Invalid,          BapOp_Invalid
    }
#endif /* BAP_USES_METHODS */
};

/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Indication
 */
const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][7] =
{
    /* Funktionsklasse Property */
    {
        BapInd_Reset,       BapInd_Invalid,     BapInd_Invalid,  BapInd_Invalid,
        BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
    },
    /* Funktionsklasse Cache */
    {
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Invalid,
        BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Changed,
        BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Processing,
        BapInd_Result,      BapInd_Invalid,     BapInd_Invalid
    }
#endif /* BAP_USES_METHODS */
};

#ifdef BAP_USES_ACKNOWLEDGE
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Acknowledge
 */
const BapAcknowledge_et BAP_ROM_CONST BAP_OpCode2AcknowledgeMatrix[][8] =
{
    /* Funktionsklasse Property */
    {
        BapAck_Property_DataSet,    BapAck_Property_DataGet,    BapAck_Property_DataSetGet, BapAck_Nothing,
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
    },
    /* Funktionsklasse Cache */
    {
        BapAck_Nothing,     BapAck_Cache_DataGet,               BapAck_Nothing,             BapAck_Nothing,
        BapAck_Nothing,     BapAck_Nothing,                     BapAck_Nothing,             BapAck_Nothing
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapAck_Array_DataSet,       BapAck_Array_DataGet,       BapAck_Array_DataSetGet,    BapAck_Nothing,
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapAck_Method_Start,        BapAck_Method_Abort,        BapAck_Method_StartResult,  BapAck_Nothing,
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
    }
#endif /* BAP_USES_METHODS */
};
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */


/* ---------------------------------------------------------------------------------------------- */
#elif defined(BAP_FSG) && !defined(BAP_ASG)
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,Request) auf OpCode
 * Diese Tabelle ist im ASG bzw. FSG unterschiedlich
 */
const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][10] =
{
    /* Funktionsklasse Property */
    {
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_PropStatus,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    },
    /* Funktionsklasse Cache */
    {
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_CacheStatusAll,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_ArrStatus,
        BapOp_ArrChanged,       BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_Invalid,
        BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
        BapOp_Invalid,          BapOp_MethProcessing,   BapOp_MethResult
    }
#endif /* BAP_USES_METHODS */
};

/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Indication
 */
const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][7] =
{
    /* Funktionsklasse Property */
    {
        BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
    },
    /* Funktionsklasse Cache */
    {
        BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapInd_Start,       BapInd_Abort,       BapInd_StartResult, BapInd_Processing_CNF,
        BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
    }
#endif /* BAP_USES_METHODS */
};


#ifdef BAP_USES_ACKNOWLEDGE
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Acknowledge
 */
const BapAcknowledge_et BAP_ROM_CONST BAP_OpCode2AcknowledgeMatrix[][8] =
{
    /* Funktionsklasse Property */
    {
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,
        BapAck_Property_Data,     BapAck_Nothing,               BapAck_Nothing,             BapAck_Property_Error
    },
    /* Funktionsklasse Cache */
    {
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
    }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
    ,
    /* Funktionsklasse Array */
    {
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Array_Changed,
        BapAck_Array_Data,          BapAck_Nothing,             BapAck_Nothing,             BapAck_Array_Error
    }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
    ,
    /* Funktionsklasse Methode */
    {
        BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Method_Processing,
        BapAck_Method_Result,       BapAck_Nothing,             BapAck_Nothing,             BapAck_Method_Error
    }
#endif /* BAP_USES_METHODS */
};
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */

/* ---------------------------------------------------------------------------------------------- */
#elif defined(BAP_ASG) && defined(BAP_FSG)
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,Request) auf OpCode
 * Dabei muss unterschieden werden ob ein ASG oder FSG den Aufruf macht
 * Erster Index: 0==ASG;1==FSG
 */
#ifdef BAP_USES_METHODS
const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][4][10] =
#else
#ifdef BAP_USES_ARRAYS
const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][3][10] =
#else
const BapOpCodes_et BAP_ROM_CONST BAP_Request2OpCodeMatrix[][2][10] =
#endif /* BAP_USES_ARRAYS */
#endif /* BAP_USES_METHODS */
{
    { /* ASG Teil */
        /* Funktionsklasse Property */
        {
            BapOp_PropSetGet,       BapOp_PropReset_Set,    BapOp_PropGet,        BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        },
        /* Funktionsklasse Cache */
        {
            BapOp_CacheSetGetAll,   BapOp_CacheSetAll,      BapOp_CacheGetAll,    BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapOp_ArrSetGet,        BapOp_ArrSet,           BapOp_ArrGet,         BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_Invalid,
            BapOp_Invalid,          BapOp_MethStart,        BapOp_MethStartResult,
            BapOp_MethAbort,        BapOp_Invalid,          BapOp_Invalid
        }
#endif /* BAP_USES_METHODS */
    },
    { /* FSG Teil */
        /* Funktionsklasse Property */
        {
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_PropStatus,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        },
        /* Funktionsklasse Cache */
        {
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_CacheStatusAll,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_ArrStatus,
            BapOp_ArrChanged,       BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,        BapOp_Invalid,
            BapOp_Invalid,          BapOp_Invalid,          BapOp_Invalid,
            BapOp_Invalid,          BapOp_MethProcessing,   BapOp_MethResult
        }
#endif /* BAP_USES_METHODS */
    }
};
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Indication
 * Unterschiedlich fuer ASG & FSG
 * Erster Index: 0==ASG;1==FSG
 */
#ifdef BAP_USES_METHODS
const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][4][7] =
#else
#ifdef BAP_USES_ARRAYS
const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][3][7] =
#else
const BapIndication_et BAP_ROM_CONST BAP_OpCode2IndicationMatrix[][2][7] =
#endif /* BAP_USES_ARRAYS */
#endif /* BAP_USES_METHODS */
{
    { /* ASG Teil */
        /* Funktionsklasse Property */
        {
            BapInd_Reset,       BapInd_Invalid,     BapInd_Invalid,  BapInd_Invalid,
            BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
        },
        /* Funktionsklasse Cache */
        {
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Invalid,
            BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Changed,
            BapInd_Data,        BapInd_Invalid,     BapInd_Invalid
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid,  BapInd_Processing,
            BapInd_Result,      BapInd_Invalid,     BapInd_Invalid
        }
#endif /* BAP_USES_METHODS */
    },
    { /* FSG Teil */
        /* Funktionsklasse Property */
        {
            BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
        },
        /* Funktionsklasse Cache */
        {
            BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapInd_DataSet,     BapInd_DataGet,     BapInd_DataSetGet,  BapInd_Invalid,
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapInd_Start,       BapInd_Abort,       BapInd_StartResult, BapInd_Processing_CNF,
            BapInd_Invalid,     BapInd_Invalid,     BapInd_Invalid
        }
#endif /* BAP_USES_METHODS */
    }
};

#ifdef BAP_USES_ACKNOWLEDGE
/**
 * Tabelle zur Abbildung eines Paars (Funktionsklasse,OpCode) auf Acknowledge
 * Unterschiedlich fuer ASG & FSG
 * Erster Index: 0==ASG;1==FSG
 */
#ifdef BAP_USES_METHODS
const BapAcknowledge_et BAP_ROM_CONST BAP_OpCode2AcknowledgeMatrix[][4][8] =
#else
#ifdef BAP_USES_ARRAYS
const BapAcknowledge_et BAP_ROM_CONST BAP_OpCode2AcknowledgeMatrix[][3][8] =
#else
#endif /* BAP_USES_ARRAYS */
const BapAcknowledge_et BAP_ROM_CONST BAP_OpCode2AcknowledgeMatrix[][2][8] =
#endif /* BAP_USES_METHODS */
{
    { /* ASG Teil */
        /* Funktionsklasse Property */
        {
            BapAck_Property_DataSet,    BapAck_Property_DataGet,    BapAck_Property_DataSetGet, BapAck_Nothing,
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
        },
        /* Funktionsklasse Cache */
        {
            BapAck_Nothing,     BapAck_Cache_DataGet,               BapAck_Nothing,             BapAck_Nothing,
            BapAck_Nothing,     BapAck_Nothing,                     BapAck_Nothing,             BapAck_Nothing
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapAck_Array_DataSet,       BapAck_Array_DataGet,       BapAck_Array_DataSetGet,    BapAck_Nothing,
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapAck_Method_Start,        BapAck_Method_Abort,        BapAck_Method_StartResult,  BapAck_Nothing,
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
        }
#endif /* BAP_USES_METHODS */
    }
    ,
    { /* FSG Teil */
        /* Funktionsklasse Property */
        {
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,
            BapAck_Property_Data,       BapAck_Nothing,             BapAck_Nothing,             BapAck_Property_Error
        },
        /* Funktionsklasse Cache */
        {
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing
        }
#if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS)
        ,
        /* Funktionsklasse Array */
        {
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Array_Changed,
            BapAck_Array_Data,          BapAck_Nothing,             BapAck_Nothing,             BapAck_Array_Error
        }
#endif /* #if defined(BAP_USES_ARRAYS) || defined(BAP_USES_METHODS) */
#ifdef BAP_USES_METHODS
        ,
        /* Funktionsklasse Methode */
        {
            BapAck_Nothing,             BapAck_Nothing,             BapAck_Nothing,             BapAck_Method_Processing,
            BapAck_Method_Result,       BapAck_Nothing,             BapAck_Nothing,             BapAck_Method_Error
        }
#endif /* BAP_USES_METHODS */
    }
};
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */

#endif  /* #elif defined(BAP_ASG) && defined(BAP_FSG) */





/* ---------------------------------------------------------------------------------------------- */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */


/* Vorwaerts-Deklarationen von statischen Funktionen */

static BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_CheckDatatype(DBGVAR BapDataType_et aeDataType
    , DBGVAR uint16_t au16BSSize
    , DBGVAR uint16_t au16DataSize
    , DBGVAR bool_t aIsSend
    , DBGVAR BapBalCheckDataType_et eScalarType
    );


#ifdef BAP_ASG
static BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckFunctionListAsg(DBGVAR const volatile_ptr_t apData
    , BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
static BAP_IMPL_FAR void
BAP_BAL_ProcessStatusAll(DBGVAR uint16_t au16Length
    , DBGVAR const volatile_ptr_t apData
    , BapLsgRomRow_pot apoLsgRomRow);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
static BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_UpdateCache(BapFctRomRow_pot apoFctRomRow
    , DBGVAR const volatile_ptr_t apData
    , DBGVAR const uint16_t au16Length);
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
static BAP_IMPL_FAR void
BAP_BAL_UpdateLsgCacheStatus(DBGVAR lsgId_t aLsgId);
#endif /* #ifdef BAP_ASG */


/* Definition (Implementierung) von statischen Funktionen */

/**
 *  Diese Funktion prueft ob zu einem konfigurierten Datentyp die angegebenen
 *  Werte passen und gibt BAP_FALSE im Fehlerfall zurueck.
 *
 *  @param aeDataType bezeichnet den konfigurierten Datentyp
 *
 *  @param au16BSSize gibt bei dem nicht skalaren Datentypen ByteSequenz die Groesse an
 *      Wird bei skalaren Typen ignoriert.
 *
 *  @param au16DataSize gibt die zur pruefende Datengroesse an
 *
 *  @param aIsSend True wenn Pruefung fuer Request, False bei Empfang
 *      Dadurch kann gewaehrleistet werden, dass Processing als IndVoid an App gemeldet wird
 *
 *  @param eScalarType gibt an, ob es sich bei dem Pruefling um einen skalaren Datentyp handelt
 *
 *  @retval BAP_TRUE
 *      der Datentyp passt zu den Werten
 *  @retval BAP_FALSE
 *      die Pruefung ist fehlgeschlagen
 */
static BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_CheckDatatype(DBGVAR BapDataType_et aeDataType
    , DBGVAR uint16_t au16BSSize
    , DBGVAR uint16_t au16DataSize
    , DBGVAR bool_t aIsSend
    , DBGVAR BapBalCheckDataType_et eScalarType)
{
    bool_t bIsScalar;
    DBGVAR bool_t bResult;

#ifdef BAP_FSG
    if (BapChkDt_ScalarInit == eScalarType)
    {
        eScalarType = BapChkDt_ScalarIn;
    }
    else if (BapChkDt_VariableInit == eScalarType)
    {
        eScalarType = BapChkDt_VariableIn;
    }
    else { /* andere Init Typen existieren nicht */ }
#endif /* #ifdef BAP_FSG */

    if((BapChkDt_ScalarIn != eScalarType) && (BapChkDt_ScalarOut != eScalarType))
    {
        bIsScalar = BAP_FALSE;
    }
    else
    {
        bIsScalar = BAP_TRUE;
    }

    bResult = BAP_TRUE;

    /* Pruefen ob der Datentyp erlaubt ist */
    switch(aeDataType)
    {
        /* pruefen des Void Datentyps: Ist ein Skalartyp mit Laenge=0 */
        case BapDt_Void:
        /* pruefen von skalaren Datentypen: muss exakt passen oder void sein */
        case BapDt_Int8:
#ifdef BAP_USES_DATATYPE_16
        case BapDt_Int16:
#endif /* BAP_USES_DATATYPE_16 */
#ifdef BAP_USES_DATATYPE_32
        case BapDt_Int32:
#endif /* BAP_USES_DATATYPE_32 */
            if( (((uint16_t)aeDataType != au16DataSize) || !bIsScalar) && (BapChkDt_VoidIn != eScalarType) )
            {
                bResult = BAP_FALSE;
            }
            break;
        /* pruefen von FixedBytesequenzen: Laenge muss exakt passen oder void, was immer als skalar angesehen wird */
        /* aIsSend Bedingung: Nur in Senderichtung muss alles passen */
        /* In Empfangsrichtung ist Processing mit anderer Kennung moeglich */
        case BapDt_FixedByteSequence:
            if(((au16BSSize != au16DataSize) && aIsSend && !bIsScalar)
                || bIsScalar
                || ((BapChkDt_VoidIn == eScalarType) && (VOID_SIZE != au16DataSize))
              )
            {
                bResult = BAP_FALSE;
            }
            break;
#ifdef BAP_USES_SEGMENTATION
        /* pruefen von Bytesequenzen: Laenge muss in Puffer passen oder void, was immer als skalar angesehen wird */
        case BapDt_ByteSequence:
            if(((au16BSSize < au16DataSize) && !bIsScalar)
                || bIsScalar
                || ((BapChkDt_VoidIn == eScalarType) && (VOID_SIZE != au16DataSize))
              )
            {
                bResult = BAP_FALSE;
            }
            break;
#endif /* BAP_USES_SEGMENTATION */
        default:
        /* unbekannten Datentyp verbieten */
            bResult = BAP_FALSE;
            break;
    }
    return bResult;
}


#ifdef BAP_ASG
/**
 *  Diese Funktion prueft im ASG die FunctionList Botschaft. Es wird geprueft, ob
 *  die Liste des FSG gleich oder ein Subset vom ASG ist (alle FctIds vom FSG muessen
 *  auch im ASG konfiguriert sein).
 *
 *  Aktionen:
 *      - Bei einer fehlerhaften FunctionList wird eine ErrorIndication mit dem
 *        Fehlerwert BapErr_IncompatibleDataSpecification gemeldet.
 *
 *  @param apData beinhaltet die FunctionList Botschaft selbst
 *
 *  @param apoLsgRomRow beinhaltet ein Zeiger auf das zugehoerige LSG
 *
 *  @retval BapErr_OK Funktionsliste ist zulaessig
 *  @retval BapErr_ReceivedDataLost falls ein kompatibles Superset definiert wurde (ist zulaessig)
 *  @retval BapErr_IncompatibleDataSpecification falls ein inkompatibles Superset definiert wurde.
 *
 *  @remark Der Zeiger apoLsgRomRow muss vor Aufruf dieser Funktion auf NULL
 *          geprueft sein. Ein Aufruf mit NULL-Zeiger ist verboten!
 */
static BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckFunctionListAsg(DBGVAR const volatile_ptr_t apData
    , BapLsgRomRow_pot apoLsgRomRow
)
{
    DBGVAR fctId_t u8CurrentFctId;
    DBGVAR BapError_et eFctListOk = BapErr_OK;
    DBGVAR fctId_t u8HighestASGFctId = (fctId_t)0;
    uint8_t u8CurrentFctBit = 0;
    uint8_t u8FctRomRowIdx;

    /* In diese Tabelle werden die vom ASG unterstuetzten Funktionen eingetragen */
    DBGVAR uint8_t aru8SupportedFunctionList[BAP_FCTID_FCTLIST_SIZE];

    volatile const uint8_t * pSourceData;
    DBGVAR uint8_t *pDestData = &aru8SupportedFunctionList[7];

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);
    /*lint -esym(613,apData)*/
    BAP_ASSERT(apData);

    /* Im ASG kann ein Subset der verfuegbaren Funktionsliste im ROM hinterlegt sein */
    /* Das FSG kann ebenso ein Subset der im ASG verfuegbaren Funktionen als neue Funktionsliste uebertragen */
    /* Ebenso darf das FSG ein Superset der im ASG verfuegbaren Funktionen senden, falls alle zusaetzlichen */
    /* Funktionen hoehere FctIds haben wie die hoechste FctId im ASG. In diesem Falle wird die Schnittmenge */
    /* beider Funktionslisten als neue Funktionsliste gewaehlt */

    pSourceData = &apData[7];
    MEMSET(aru8SupportedFunctionList, 0, BAP_FCTID_FCTLIST_SIZE);

    /* ermittle die vom ASG unterstuetzte Funktionsliste */
    for(u8FctRomRowIdx = 0; u8FctRomRowIdx < apoLsgRomRow->u8FctRomTableSize; u8FctRomRowIdx++)
    {
        uint8_t u8FctId = BINARY_ID(apoLsgRomRow->poFctRomTable[u8FctRomRowIdx].fctId);

        aru8SupportedFunctionList[u8FctId / 8] |= (0x80 >> (u8FctId % 8));
    }

    /* Prufe zunaechst die Zulaessigkeit der neuen FunctionList und entferne */
    /* die FctIds aus dem Empfangspuffer, welche ein Superset definieren     */
    for(u8CurrentFctId = (fctId_t)64;
        --u8CurrentFctId >= BAP_FCTID_FCTLIST;  /* beginne mit 63 und hoer nach der FctList auf */
        u8CurrentFctBit += u8CurrentFctBit      /* schiebe Bit nach Links */
       )
    {
        if (!u8CurrentFctBit)
        {
            u8CurrentFctBit++;  /* hat danach den Wert 0x01 */

            /* const_cast<> ist erforderlich. */
            /* Ungefaehrlich in BAP 1.4, da danach niemand mehr auf den Empfangspuffer zugreift. */
            pSourceData = &apData[u8CurrentFctId / (fctId_t)8];
            pDestData = &aru8SupportedFunctionList[u8CurrentFctId / (fctId_t)8];
        }

        /* existiert diese Funktion im ASG? */
        if (!(u8CurrentFctBit & (*pDestData)))
        {

            /* Bit gesetzt in neuer Fct.list, dann muss es auch im ASG unterstuetzt werden, falls die neue
             * Funktion nicht hinter der hoechsten unterstuetzten Funktion des ASG liegt
             */
            if (u8CurrentFctBit & (*pSourceData))
            {
                if (u8HighestASGFctId)
                {
                    eFctListOk = BapErr_IncompatibleDataSpecification;
                }
                else
                {
                    eFctListOk = BapErr_ReceivedDataLost;
                }
            }
        }
        else
        {
            /* Merke die hoechste FctId des ASG um inkompatible Erweiterungen zu erkennen */
            if (!u8HighestASGFctId)
            {
                u8HighestASGFctId = u8CurrentFctId;
            }

            /* Falls in neuer Funktionliste das Bit nicht gesetzt ist, Bit im ASG ausmaskieren */
            if ( !(u8CurrentFctBit & (*pSourceData)) )
            {
                *pDestData ^= u8CurrentFctBit;
                if (BapErr_OK == eFctListOk)
                {
                    eFctListOk = BapErr_ReceivedDataLost;
                }
            }
        }
    }

    if(BapErr_IncompatibleDataSpecification == eFctListOk)
    {
        /* Fehlerhafte Datenfestlegung an die Applikation melden */
        BAP_IndicationError(apoLsgRomRow->lsgId, BAP_FCTID_FCTLIST, BapErr_IncompatibleDataSpecification);
        BAP_DBG_DataDefinitionError(apoLsgRomRow->lsgId);
    }
    else
    {
        /* FunctionList uebernehmen */
        MEMCPY(apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList, &aru8SupportedFunctionList[0], BAP_FCTID_FCTLIST_SIZE);    /*lint !e668*//* (void*)apData!=NULL */
    }

    return eFctListOk;
    /*lint +esym(613,apData)*/
    /*lint +esym(613,apoLsgRomRow)*/
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Diese Funktion verarbeitet eine StatusAll-Block Nachricht. Entsprechend der
 *  FunctionList werden die einzelnen Daten ueber die Konfigurierte Daten-Groesse
 *  aus dem Block herausgeschnitten und falls ein Cache fuer die FctId vorhanden
 *  ist abgelegt. Bis eine FunctionList vom FSG da ist, wird mit der vom ASG
 *  gearbeitet.
 *  Als Sonderfall wird die FctId Heartbeat behandelt. Die Daten werden hier
 *  an den BPL geschickt, um dort die Heartbeat-Ueberwachung zu starten.
 *  Ist die GetAll Nachricht fehlerhaft (z.B. falsche Laenge) wird der Gesamtcache
 *  ungueltig und die Fehlermeldung BapErr_GetAllMessageCorrupted gemeldet.
 *
 *  @param au16Length gibt die Laenge der Daten an.
 *
 *  @param apData beinhaltet die FunctionList Botschaft selbst
 *
 *  @param apoLsgRomRow beinhaltet ein Zeiger auf das zugehoerige LSG
 *
 *  @remark Der Zeiger apoLsgRomRow muss vor Aufruf dieser Funktion auf NULL
 *          geprueft sein. Ein Aufruf mit NULL-Zeiger ist verboten!
 */
static BAP_IMPL_FAR void
BAP_BAL_ProcessStatusAll(DBGVAR uint16_t au16Length
    , DBGVAR const volatile_ptr_t apData
    , BapLsgRomRow_pot apoLsgRomRow
)
{
    DBGVAR uint8_t *pu8FctList;
    DBGVAR uint8_t u8TableSize;
    DBGVAR uint8_t u8FctIndex;
    BapFctRomRow_pot poFctRomRow;
    DBGVAR uint16_t u16GetAllOffset;
    DBGVAR BapError_et eError = BapErr_OK;
    DBGVAR BapError_et eCompatibleExtension = BapErr_OK;

    /*lint -esym(613,apoLsgRomRow)*/
    /*lint -esym(831,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);
    /*lint -esym(613,apData)*/
    BAP_ASSERT(apData);

    /* FunctionList holen */
    /* FunctionList-Eintrag immer vorhanden: entweder durch ASG FctList initial, oder durch FSG FctList */
    pu8FctList = apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList;
    u8TableSize = apoLsgRomRow->u8FctRomTableSize;

    u16GetAllOffset = 0;
    /* Schleife solange alle Fct-Caches gueltig sind und nicht Ende der Tabelle erreicht */
    for(u8FctIndex=0; (u8FctIndex<u8TableSize) && (BapErr_OK == eError); ++u8FctIndex)
    {
        poFctRomRow = &apoLsgRomRow->poFctRomTable[u8FctIndex];
        /* Ist die FunctionId im GetAll enthalten? */
        if((BINARY_ID(poFctRomRow->fctId) > 1)     /* GETALL ausschliessen */
            && ( BINARY_ID(poFctRomRow->fctId) < 64)   /*lint !e650 enums haben mindestens 8 bit */ /* keine internen FctIds (CacheStatus) */
            /* Bit in der FctList holen, welches angibt ob die FctId vom FSG relevant ist */
            && (0 != (pu8FctList[BINARY_ID(poFctRomRow->fctId) / 8] & (0x80 >> (BINARY_ID(poFctRomRow->fctId) % 8))))
            && (BapFctCls_Property == poFctRomRow->eFunctionClass) )    /* Es werden nur Properties uebertragen */

        {
            uint16_t u16Size = 0;
            /* Ermittle Laenge des Eintrages */
            if(BapDt_ByteSequence == poFctRomRow->eRxDataType)
            {
                /* Bei ByteSequence steht die Laengenangabe in den empfangenen Telegramm */

                /* Sind noch ausreichend gueltige Daten vorhanden? */
                if(au16Length < (u16GetAllOffset + (uint16_t)2))
                {
                    /* nicht genuegend Daten */
                    eError = BapErr_GetAllMessageCorrupted;
                }
                else
                {
                    /* bei ByteSequences wird die Laenge in den ersten 2 Bytes [Lo,Hi] uebertragen */
                    u16Size = (uint16_t)apData[u16GetAllOffset] +
                        (uint16_t)((uint16_t)(apData[u16GetAllOffset+1u])
                        << 8);

                    /* Offset auf Daten nach Laenge setzen */
                    u16GetAllOffset += (uint16_t)2;
                }

                if(u16Size > poFctRomRow->u16RxSize)
                {
                    /* Fehler: Laenge in GETALL ist groesser als die konfigurierte Laenge */
                    eError = BapErr_GetAllMessageCorrupted;
                }
            }
            else
            {
                /* Sonst gilt die konfigurierte Laenge */
                u16Size = poFctRomRow->u16RxSize;
            }
            /* Sind noch ausreichend gueltige Daten vorhanden? */
            if( (BapErr_OK == eError)
                && ((u16GetAllOffset + u16Size) <= au16Length)
              )
            {
                /* Sonderfall bei der Behandlung von HEARTBEAT, da hier der BPL informiert werden muss */
                if(poFctRomRow->fctId == BAP_FCTID_HEARTBEAT)
                {
                    BAP_BPL_ProcessHBMsg(apoLsgRomRow, apData[u16GetAllOffset]);
                }
                /* Uebernehme die FunctionList, falls sie gueltig ist */
                else if( (poFctRomRow->fctId == BAP_FCTID_FCTLIST)
                    /*lint -esym(960,33) Seiteneffekt ist nur gewuenscht, wenn es eine FunctionList ist */
                    && (BapErr_IncompatibleDataSpecification
                        == (eCompatibleExtension = BAP_BAL_CheckFunctionListAsg(&apData[u16GetAllOffset], apoLsgRomRow))) )
                    /*lint +esym(960,33) */
                {
                    eError = BapErr_GetAllMessageCorrupted;
                }
                else
                {
                    /* keine Sonderbehandlung erforderlich */
                }

                /* nur wenn im ASG ein Cache fuer diese FctId vorhanden ist und der */
                /* Cache nicht valid ist, ein Update durchfuehren. Verwerfen des Datums wenn Valid */
                if((NULL != poFctRomRow->poBalRamRow)
                    && (   (BapBalDataStat_Invalid == poFctRomRow->poBalRamRow->eDataStatus)
                        || (BapBalDataStat_GetAll == poFctRomRow->poBalRamRow->eDataStatus)  ) )
                {
                    /* Information ueber eine Werteaenderung ist unwichtig hier */
                    (void) BAP_BAL_UpdateCache(poFctRomRow, &apData[u16GetAllOffset], u16Size);
                }
                else
                {
                    /* wenn kein Cache vorhanden, werden die Daten ueberlesen */
                }

                /* Offset auf naechsten Wert im GetAll-Block setzen */
                u16GetAllOffset += u16Size;
            }
            else
            {
                eError = BapErr_GetAllMessageCorrupted;
            }
        } else { /* nix zu tun */ }
    }
    if( (BapErr_OK != eError)
        || (u16GetAllOffset > au16Length)
        || ((u16GetAllOffset < au16Length) && (BapErr_ReceivedDataLost != eCompatibleExtension))
    )
    {
        /* Korrupte GETALL-Nachricht an die Applikation melden */
        BAP_IndicationError(apoLsgRomRow->lsgId, BAP_FCTID_GETALL, BapErr_GetAllMessageCorrupted);
        BAP_DBG_GetAllMessageCorruptedError(apoLsgRomRow->lsgId);
        /* Nach einem korrupten STATUSALL muss der teilweise ueberschriebene Cache ungueltig werden */
        BAP_BAL_InvalidateCache(&*apoLsgRomRow);    /* &* behebt Lint-Analyse-Problem */
    }
    else
    {
        /* Gesamt-Cache-Gueltigkeit neu berechnen */
        BAP_BAL_UpdateLsgCacheStatus(apoLsgRomRow->lsgId);
    }
    return;
    /*lint +esym(613,apData)*/
    /*lint +esym(613,apoLsgRomRow)*/
    /*lint +esym(831,apoLsgRomRow)*/
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Diese Funktion aktualisiert den Cache-Speicher mit den neuen Werten fuer
 *  ein einzelnen Cache-Eintrag. Der Zustand des Gesamt LSG Caches muss anschliessend
 *  ueber die Funktion BAP_BAL_UpdateLsgCacheStatus neu berechnet werden.
 *  Die Anpassung von Little/Big-Endian muss bereits geschehen sein.
 *  Ein vorhandener Cache fuer die FunctionID muss vor dem Aufruf bereits
 *  sichergestellt sein.
 *
 *  @param apoFctRomRow gibt die BapFctRomRow der betreffenden FctId an.
 *
 *  @param apData beinhaltet ein Byte-Array auf die zu verarbeiteten Daten.
 *
 *  @param au16Length gibt die Laenge der Daten an.
 *
 *  @returns
 *      BAP_FALSE wenn sich der uebergebende Wert vom urspruenglichen unterscheidet,
 *      BAP_TRUE  wenn identisch
 *
 */
static BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_UpdateCache(BapFctRomRow_pot apoFctRomRow
    , DBGVAR const volatile_ptr_t apData
    , DBGVAR const uint16_t au16Length)
{
    /*lint -esym(613,apoFctRomRow)*/
    DBGVAR BapBalRamRow_pot pBalRamRow = apoFctRomRow->poBalRamRow;
    BapData_put const pxValue = &pBalRamRow->xValue;
    DBGVAR bool_t bValueHasChanged = BAP_FALSE;

    /*lint -esym(613,apData)*/
    BAP_ASSERT(apData);

    switch(apoFctRomRow->eRxDataType)
    {
        case BapDt_Int8:
            if (pxValue->u8Value != *((const uint8_t *)apData) )    /*lint !e613*//* falls apData!=NULL, dann ist das Ergebnis des casts auch != NULL*/
            {
                pxValue->u8Value = *((const uint8_t *)apData);      /*lint !e613*//* dto. */
                bValueHasChanged = BAP_TRUE;
            }
            break;
        case BapDt_Int16:
            if (pxValue->u16Value != *((const uint16_t *)(const void *)apData) )    /*lint !e613*/
            {
                pxValue->u16Value = *((const uint16_t *)(const void *)apData);      /*lint !e613*/
                bValueHasChanged = BAP_TRUE;
            }
            break;
        case BapDt_Int32:
            if (pxValue->u32Value != *((const uint32_t *)(const void *)apData) )    /*lint !e613*/
            {
                pxValue->u32Value = *((const uint32_t *)(const void *)apData);      /*lint !e613*/
                bValueHasChanged = BAP_TRUE;
            }
            break;
        case BapDt_FixedByteSequence:
        case BapDt_ByteSequence:
            /*lint -e668*//* (void*)apData != NULL */
            /*lint -esym(960,33) MEMCMP hat keinen Seiteneffekt */
            if (   (pxValue->oBufferWithLength.u16Length != au16Length)
                || (0 != MEMCMP(pxValue->oBufferWithLength.aru8Buffer, apData, au16Length)))
            {
                MEMCPY(pxValue->oBufferWithLength.aru8Buffer, apData, au16Length);
                pxValue->oBufferWithLength.u16Length = au16Length;
                bValueHasChanged = BAP_TRUE;
            }
            /*lint +esym(960,33) */
            /*lint +e668*/
            break;
        default:
            break;
    }
    if(BapBalDataStat_Valid != pBalRamRow->eDataStatus)
    {
        pBalRamRow->eDataStatus = BapBalDataStat_Valid;
    }

    return bValueHasChanged;
    /*lint +esym(613,apData)*/
    /*lint +esym(613,apoFctRomRow)*/
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Diese Funktion aktualisiert den Gesamt-Cache-Status des adressierten LSG,
 *  in dem alle Einzel-Caches auf valid geprueft werden und falls alle valid sind
 *  auch der Gesamt-Cache-Status valid wird. Wird ein Cache einer FunktionID
 *  als invalid gefunden, so wird der Gesamt-Cache-Status invalid.
 *  Fuer die Ueberpruefung der Cache-Gueltigkeit werden nur FctIds in Betracht
 *  gezogen, die in der derzeitigen FunctionList enthalten sind.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet.
 *
 *  @remarks
 *      Falls die LsgID nicht gefunden wird, wird der Update-Aufruf ignoriert.
 *  @remarks
 *      Die LsgID muss im Code vorher schon geprueft worden sein,
 */
static BAP_IMPL_FAR void
BAP_BAL_UpdateLsgCacheStatus(DBGVAR lsgId_t aLsgId)
{
    BapLsgRomRow_pot poLsgRomRow;
    DBGVAR uint8_t *pu8FctList;
    DBGVAR uint8_t u8TableSize;
    DBGVAR uint8_t u8FctIndex;
    DBGVAR bool_t bLsgCacheStatus = BAP_TRUE;

    /* Eintrag des LSGs holen */
    poLsgRomRow = BAP_GetLsgRomRow(aLsgId);
    if(NULL != poLsgRomRow)
    {
        /* FunctionList holen */
        /* FunctionList-Eintrag immer gueltig: entweder durch ASG FctList initial, oder durch uebertragene FSG FctList */
        pu8FctList = poLsgRomRow->poLsgRamRow->BAP_aru8FunctionList;
        u8TableSize = poLsgRomRow->u8FctRomTableSize;
        u8FctIndex = 0;
        /* Schleife solange alle Fct-Caches gueltig sind und nicht Ende der Tabelle erreicht */
        while((u8FctIndex < u8TableSize) && (bLsgCacheStatus))
        {
            BapFctRomRow_pot poFctRomRow = &poLsgRomRow->poFctRomTable[u8FctIndex];
            /* Ist der Cache fuer eine Funktion ungueltig, dann ist der ganze Cache ungueltig */
            if((NULL != poFctRomRow->poBalRamRow)
                && (BINARY_ID(poFctRomRow->fctId) > 1)     /* GETALL ausschliessen */
                && (BINARY_ID(poFctRomRow->fctId) < 64)    /*lint !e650 enums haben mindestens 8 bit *//* keine internen FctIds (CacheStatus) */
                /* Bit in der FctList holen, welches angibt ob die FctId vom FSG relevant ist */
                && (0 != (pu8FctList[BINARY_ID(poFctRomRow->fctId) / 8] & (0x80 >> (BINARY_ID(poFctRomRow->fctId) % 8))) )
                && (BapBalDataStat_Invalid == poFctRomRow->poBalRamRow->eDataStatus))
            {
                bLsgCacheStatus = BAP_FALSE;
            }
            else
            {
                ++u8FctIndex;
            }
        }
        /* Hat sich die Gueltigkeit des Caches veraendert? (! ist erforderlich, damit es fuer 1 != -1 auch funktioniert) */
        if ((bool_t)!bLsgCacheStatus != ((bool_t)!poLsgRomRow->poLsgRamRow->fState.fCacheValid))
        {
            /* nur Aenderungen melden */
            poLsgRomRow->poLsgRamRow->fState.fCacheValid = bLsgCacheStatus;
            if (bLsgCacheStatus)
            {
                BAP_IndicationVoid(aLsgId, BAP_FCTID_GETALL, BapInd_Data);
                /* Setzen der Gesamtcachegueltigkeit */
                poLsgRomRow->poLsgRamRow->fState.fCacheValid = BAP_TRUE;
                /* Beenden des Retry fuer GetAll */
                BAP_BPL_AbortSend(aLsgId, BAP_FCTID_GETALL);
                BAP_DBG_StatusAllEvent(aLsgId);
                BAP_DBG_SetCacheValidState(aLsgId, BAP_TRUE);
            }
            else
            {
                /* Loeschen der Gesamtcachegueltigkeit */
                poLsgRomRow->poLsgRamRow->fState.fCacheValid = BAP_FALSE;
                BAP_DBG_SetCacheValidState(aLsgId, BAP_FALSE);
            }
        }
        else
        {
            /* keine Meldung erforderlich */
        }
    }
    return;
}
#endif /* #ifdef BAP_ASG */

/* Definition (Implementierung) von globalen Funktionen */

/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckRequest(BapInternalParameters_pot apoPars
    , DBGVAR BapRequest_et aeRequest
    , BapBalCheckDataType_et eCheckDataType
    )
{
    DBGVAR BapError_et eResult = BapErr_OK;

    BapLayerLsgStatus_et eLsgStatus = BapLayerLsgStat_Running;
#ifdef BAP_FSG
    if ( (BapChkDt_ScalarInit == eCheckDataType) || (BapChkDt_VariableInit == eCheckDataType))
    {
        eLsgStatus = BapLayerLsgStat_Initialized;
    }
#endif /* #ifdef BAP_FSG */

    /*lint -esym(613,apoPars)*/
    BAP_ASSERT(apoPars);

    apoPars->poLsgRomRow = BAP_GetLsgRomRow(apoPars->lsgId);
    apoPars->poFctRomRow = BAP_GetLsgFctRomRow(apoPars->poLsgRomRow, apoPars->fctId);

    /* Pruefen ob die Funktion existiert */
    if (NULL == apoPars->poFctRomRow)
    {
        eResult = BapErr_InvalidArg;
    }
    /* Ueberpruefe Zustandsmaschine */
    else if(eLsgStatus != apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus)
    {
        BAP_DBG_InvalidStateError(apoPars->lsgId);
        eResult = BapErr_InvalidState;
    }
    /* Pruefen ob in Fct.list und OpCode ermitteln */
    else
    {
        bool_t bValidFct = BAP_BAL_CheckIfFunctionIsValid(apoPars->poFctRomRow, apoPars->poLsgRomRow->poLsgRamRow);
        if (!bValidFct)
        {
            eResult = BapErr_InvalidArg;
        }
        else if (BapReq_Invalid == aeRequest)
        {
            /* Requesttyp: InitSendBuffer/CacheGetRequest. Fuer InitSendBuffer richtigen OpCode eintragen.
               Bei CacheGetRequest wird der OpCode nicht benoetigt */
            apoPars->eOpCode = BapOp_PropHeartbeatStatus;
        }
        else
        {
#if defined(BAP_ASG) && defined(BAP_FSG)
            apoPars->eOpCode = BAP_Request2OpCodeMatrix[apoPars->poLsgRomRow->eSGType][apoPars->poFctRomRow->eFunctionClass][aeRequest];
#else
            apoPars->eOpCode = BAP_Request2OpCodeMatrix[apoPars->poFctRomRow->eFunctionClass][aeRequest];
#endif
            /* Pruefen ob der OpCode erlaubt ist */
            if ( (BapReq_Invalid != aeRequest)
                 && (0 == (apoPars->poFctRomRow->u8OpCodeSendMask & (uint8_t)(1u<<(uint8_t)apoPars->eOpCode)))
               )
            {
                eResult = BapErr_InvalidArg;
            }

#ifdef BAP_FSG /* IM FSG und Kombigeraet */
            /* Im FSG ist es verboten einen Request auf Fct.ID 0-7 zu schicken, im ASG ist es OK */
            else if (
                 /* Nur erlaubt im Nicht initialisierten Zustand */
#if defined(BAP_ASG) && defined(BAP_FSG) /* Nur im Kombigeraet */
                 (BapSG_FSG == apoPars->poLsgRomRow->eSGType) &&
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
                /* Nur im Initzustand duerfen BAP interne Fct-Ids gesetzt werden */
                 (8 > BINARY_ID(apoPars->fctId)) && (BapLayerLsgStat_Initialized != eLsgStatus)
               )
            {
                eResult = BapErr_InvalidArg;
            }
            else { /* kein anderer Fall */ }
#endif /* #ifdef BAP_FSG */
        }
    }

    /* Pruefen ob der Datentyp erlaubt ist */
    if (BapErr_OK == eResult)
    {
        bool_t bDataTypeCheck;

        switch(eCheckDataType)
        {
            case BapChkDt_VoidIn:
#ifdef BAP_ASG /* auch Kombigeraet */
                /* wird hier aus Hilfsvariable fuer naechste Anweisung verwendet,
                   bei Get und Abort Datentyp void verwenden statt dem konfigurierten.
                 */
                bDataTypeCheck = (bool_t) ((BapReq_DataGet == aeRequest) || (BapReq_Abort == aeRequest));
                bDataTypeCheck = BAP_BAL_CheckDatatype(
                    (const BapDataType_et)(bDataTypeCheck ? BapDt_Void : apoPars->poFctRomRow->eTxDataType)
                    , VOID_SIZE
                    , VOID_SIZE
                    , BAP_TRUE  /* Ist Senderichtung */
                    , (const BapBalCheckDataType_et) (bDataTypeCheck ? BapChkDt_VoidIn : BapChkDt_ScalarIn)
                );
#else /* reines FSG */
                bDataTypeCheck = BAP_BAL_CheckDatatype(
                    apoPars->poFctRomRow->eTxDataType
                    , VOID_SIZE
                    , VOID_SIZE
                    , BAP_TRUE  /* Ist Senderichtung */
                    , BapChkDt_ScalarIn
                );
#endif /* #ifdef BAP_ASG */
                break;
#ifdef BAP_FSG
            case BapChkDt_ScalarInit:
            case BapChkDt_VariableInit:
#endif /* #ifdef BAP_FSG */
            case BapChkDt_ScalarIn:
            case BapChkDt_VariableIn:
                bDataTypeCheck = BAP_BAL_CheckDatatype(apoPars->poFctRomRow->eTxDataType
                    ,
                        apoPars->poFctRomRow->u16TxSize /* au16BSSize */
                    , apoPars->u16Length              /* au16DataSize */
                    , BAP_TRUE /* Ist Senderichtung */
                    , eCheckDataType /* VariableIn | Skalar */
                );
            break;

#ifdef BAP_ASG
            case BapChkDt_ScalarOut:
                bDataTypeCheck = BAP_BAL_CheckDatatype(apoPars->poFctRomRow->eRxDataType
                    , apoPars->poFctRomRow->u16RxSize /* au16BSSize */
                    , apoPars->u16Length              /* au16DataSize */
                    , BAP_TRUE /* Ist Senderichtung */
                    , eCheckDataType /* VariableIn | Skalar */
                );
            break;

            /** Pruefung fuer BYTESEQUENCE bei BAP_CacheGetRequestByteSequence */
            case BapChkDt_VariableOut:
            default:
                bDataTypeCheck = BAP_BAL_CheckDatatype(apoPars->poFctRomRow->eRxDataType
                    , apoPars->poFctRomRow->u16RxSize /* au16BSSize */
                    , apoPars->poFctRomRow->u16RxSize /* au16DataSize */
                    , BAP_TRUE /* Ist Senderichtung */
                    , BapChkDt_VariableOut
                );
                break;
#else
            case BapChkDt_ScalarOut:
            case BapChkDt_VariableOut:
            default:
                bDataTypeCheck = BAP_FALSE;
                break;
#endif /* #ifdef BAP_ASG */
        }

        if (!bDataTypeCheck)
        {
            eResult = BapErr_InvalidArg;
        }
    }

    if (BapErr_InvalidArg == eResult)
    {
        BAP_DBG_InvalidArgError(apoPars->lsgId);
    }

    return eResult;
    /*lint +esym(613,apoPars)*/
}


/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_CheckDataReceived(BapInternalParameters_cpot apoPars)
{
    DBGVAR BapError_et eResult = BapErr_OK;

    DBGVAR BapDataType_et eRxDataType;

    /*lint -esym(613,apoPars)*/
    BAP_ASSERT(apoPars);

    eRxDataType = apoPars->poFctRomRow->eRxDataType;

    BAP_ASSERT(apoPars->fctId <= MAX_FCT_ID);

    /* Pruefen ob die Funktion existiert */
    if (NULL == apoPars->poFctRomRow)
    {
        eResult = BapErr_InvalidArg;
    }

    /* Pruefen ob der OpCode erlaubt ist */
    if ( (BapErr_OK == eResult)
         && (0 == (apoPars->poFctRomRow->u8OpCodeReceiveMask & (uint8_t)(1u<<(uint8_t)apoPars->eOpCode)))
       )
    {
        eResult = BapErr_InvalidArg;
    }

    /* Pruefen ob der Datentyp erlaubt ist */
    if (BapErr_OK == eResult)
    {
        bool_t bDataTypeCheck = BAP_BAL_CheckDatatype(eRxDataType
            , apoPars->poFctRomRow->u16RxSize
            , apoPars->u16Length
            , BAP_FALSE /* Ist nicht Senderichtung */
            ,   (const BapBalCheckDataType_et) (
#ifdef BAP_USES_SEGMENTATION
                ((eRxDataType == BapDt_ByteSequence) || (eRxDataType == BapDt_FixedByteSequence)) ? BapChkDt_VariableOut :
#else
                (eRxDataType == BapDt_FixedByteSequence) ? BapChkDt_VariableOut :
#endif /* #ifdef BAP_USES_SEGMENTATION */
               BapChkDt_ScalarOut) );
        if (!bDataTypeCheck)
        {
            eResult = BapErr_InvalidArg;
        }
    }

    /* Pruefen ob nicht in Fct.list */
    if (BapErr_OK == eResult)
    {
        bool_t bValidFct = BAP_BAL_CheckIfFunctionIsValid(apoPars->poFctRomRow, apoPars->poLsgRomRow->poLsgRamRow);
        if (!bValidFct)
        {
            eResult = BapErr_InvalidArg;
        }
    }

    if (BapErr_OK == eResult)
    {
        /* Normalerweise darf im Nicht Running Zustand nichts nach oben gemeldet werden, aber */
        /* Die BAP_FCTID_BAPCONFIG muss auch im WAIT_FOR_CONFIG Status an Applikation gemeldet werden! */
        if(    (BapLayerLsgStat_Running != apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus)
#ifdef BAP_ASG
            && !( (BAP_FCTID_BAPCONFIG == apoPars->fctId) && (BapLayerLsgStat_WaitForConfig == apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus) )
#endif /* #ifdef BAP_ASG */
            )
        {
            /* hier keine BAP_DBG Meldung, da der BAP-Stack noch nicht gestartet wurde */
            eResult = BapErr_InvalidState;
        }
    }
    else
    {
        BAP_DBG_InvalidArgError(apoPars->lsgId);
    }

    return eResult;
    /*lint +esym(613,apoPars)*/
}


#ifdef BAP_ASG
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR void
BAP_BAL_InvalidateCache(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR uint8_t u8FctIndex;
    DBGVAR uint8_t u8TableSize;

    /*lint -esym(613, apoLsgRomRow) */
    /*lint -esym(831, apoLsgRomRow) */
    BAP_ASSERT(apoLsgRomRow);

    u8TableSize = apoLsgRomRow->u8FctRomTableSize;
    for(u8FctIndex=0; u8FctIndex<u8TableSize; ++u8FctIndex)
    {
        BapFctRomRow_pot poFctRomRow = &apoLsgRomRow->poFctRomTable[u8FctIndex];
        if(NULL != poFctRomRow->poBalRamRow)
        {
            poFctRomRow->poBalRamRow->eDataStatus = BapBalDataStat_Invalid;
        }
    }
    /* FctList-Puffer mit ASG FctList vorbelegen */
    FAR_MEMCPY(apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList, apoLsgRomRow->BAP_aru8FunctionList, BAP_FCTID_FCTLIST_SIZE);
    BAP_BAL_UpdateLsgCacheStatus(apoLsgRomRow->lsgId);
    /*lint +esym(831, apoLsgRomRow) */
    /*lint +esym(613, apoLsgRomRow) */

    return;
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR BapBalDataStatus_et
BAP_BAL_GetLsgCacheStatus(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR BapBalDataStatus_et eReturnStatus = BapBalDataStat_Invalid;

    if(NULL != apoLsgRomRow)
    {
        eReturnStatus = (const BapBalDataStatus_et) (
            (apoLsgRomRow->poLsgRamRow->fState.fCacheValid) ?
            BapBalDataStat_Valid : BapBalDataStat_Invalid);
    }
    else
    {
        /* darf nie vorkommen! */
    }
    return eReturnStatus;
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_CheckBapConfigAsg(BapInternalParameters_cpot apoPars
    , DBGVAR const volatile_ptr_t apData
)
{
    /*
     * Bit6 = PV, Bit4 = SG/DF  (Gesamtergebnis aller Konfigurationszeilen)
     * Bit2 = PV, Bit0 = SG/DF  (Einzelergebnis der aktuellen Konfigurationszeile)
     */
    DBGVAR uint8_t u8ValidConfig = 0;
    DBGVAR uint8_t u8ListId;
    DBGVAR uint8_t u8LsgTableId;
    DBGVAR uint8_t u8RefLsgTableId = 0xff;   /* mit "-1" initialisieren damit dann mit 0 angefangen wird */
    DBGVAR lsgId_t oldLsgId = (lsgId_t)0;
    DBGVAR BAP_ROM_DATA_FAR const uint8_t *pu8BAPConfig;
    DBGVAR BapError_et bapErr;

    BapLsgRomRow_pot poLsgRomRow=NULL;

    /*lint -esym(613,apData)*/
    /*lint -esym(613,apoPars)*/ /* wurde bereits vorher benutzt */
    BAP_ASSERT(apData);

    /* Eine passende Konfiguration suchen */
    for(u8LsgTableId=0; (u8LsgTableId<BAP_LSG_ROM_TABLE_ROWS) && (0x55 != u8ValidConfig); u8LsgTableId++)
    {
        poLsgRomRow = &BAP_LsgRomTables[u8LsgTableId];
        if(oldLsgId != poLsgRomRow->lsgId)
        {
            u8RefLsgTableId++;
            oldLsgId = poLsgRomRow->lsgId;
        }
        if(poLsgRomRow->lsgId == apoPars->lsgId)
        {
            /* BAPConfig vom FSG pruefen */
            for(u8ListId=0; (u8ListId<poLsgRomRow->u8BAPConfigTableSize) && (0x55 != u8ValidConfig); u8ListId++)
            {
                /* Intialisiere das aktuelle Ergebnis mit keine Uebereinstimmung gefunden, nicht jedoch das Gesamtergebnis */
                u8ValidConfig &= 0xF0;

                pu8BAPConfig = (poLsgRomRow->paru8BAPConfigTable)[u8ListId];

                if((pu8BAPConfig[BAP_BAPCONFIG_POS_PV_MAJOR] == apData[BAP_BAPCONFIG_POS_PV_MAJOR])
                    && (pu8BAPConfig[BAP_BAPCONFIG_POS_PV_MINOR] == apData[BAP_BAPCONFIG_POS_PV_MINOR])
                   )
                {
                    u8ValidConfig |= 0x44;
                }

                if((pu8BAPConfig[BAP_BAPCONFIG_POS_SG_MAJOR] == apData[BAP_BAPCONFIG_POS_SG_MAJOR])
                    && (pu8BAPConfig[BAP_BAPCONFIG_POS_DF_MAJOR] == apData[BAP_BAPCONFIG_POS_DF_MAJOR])
                    )
                {
                    u8ValidConfig |= 0x11;
                }
            }
        }
    }

    if ( 0x55 == u8ValidConfig )
    {
        /* Eine gueltige BAPConfig gefunden */
        BAP_pLsgRomTable[u8RefLsgTableId] = poLsgRomRow;
    }
    else
    {
        /* falls keine gueltige Konfig gefunden mit der "alten" weiterarbeiten */
        poLsgRomRow = apoPars->poLsgRomRow;
    }

    /* Beim Empfang eines Reset den Cache ungueltig machen */
    if(BapOp_PropReset_Set == apoPars->eOpCode)
    {
        /* Nachdem das ASG gestartet wurde und ein Reset kommt erst mal den Zustand
           WaitForConfig einnehmen, bis die Version geprueft wurde
           (erfolgt noch in dieser Funktion, erforderlich fuer ungueltige Version) */
        /*lint -esym(771,poLsgRomRow)  da die Lsg-Tabelle immer mindestens eine Zeile hat, ist der Zeiger initialisiert */
        /*lint -esym(794,poLsgRomRow)*/
        poLsgRomRow->poLsgRamRow->eLsgStatus = BapLayerLsgStat_WaitForConfig;
        BAP_DBG_SetBalState(apoPars->lsgId, BapLayerLsgStat_WaitForConfig);
        BAP_BAL_InvalidateCache(poLsgRomRow);
        /* Es muessen die darunterliegenden Schichten neu initialisiert werden */
        /* z.B. Heartbeat, Sendeauftraege muessen an die neue Config angepasst werden */
        /* Es ist nicht moeglich die BAL Ebene neu zu starten, da sonst auch BAPConfig */
        /* Anfragen durchgefuehrt werden wuerden */
        (void)BAP_BPL_Init(poLsgRomRow);
        (void)BAP_BPL_Start(poLsgRomRow);
    }

    if(0x55 == u8ValidConfig)
    {
        /* Wenn alles ok => Zsutand auf running setzen */
        poLsgRomRow->poLsgRamRow->eLsgStatus = BapLayerLsgStat_Running;
        /*lint +esym(794,poLsgRomRow)*/
        /*lint +esym(771,poLsgRomRow)*/
        BAP_DBG_SetBalState(apoPars->lsgId, BapLayerLsgStat_Running);
        bapErr = BapErr_OK;
    }
    else
    {
        if(!(u8ValidConfig & 0x40))
        {
            /* Fehlerhafte Protokollversion an die Applikation melden */
            BAP_DBG_ProtocolVersionError(apoPars->lsgId);
            bapErr = BapErr_IncompatibleProtocolVersion;
        }
        else
        {
            /* Es ist durchaus moeglich, dass es eine passende DF und SG gibt, aber nicht in Kombination mit einer gueltigen PV */

            /* Fehlerhafte Datenfestlegung oder SG-Klasse an die Applikation melden */
            BAP_DBG_DataDefinitionError(apoPars->lsgId);
            bapErr = BapErr_IncompatibleDataSpecification;
        }
        BAP_IndicationError(apoPars->lsgId, BAP_FCTID_BAPCONFIG, bapErr);
    }

    return (bool_t) (BapErr_OK != bapErr);

    /*lint +esym(613,apoPars)*/
    /*lint +esym(613,apData)*/
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_FSG
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BAL_InitFSGBuffers(BapLsgRomRow_pot apoLsgRomRow)
{
    /* Zum Zwischenspeichern des Funktionsergebnisses */
    DBGVAR BapError_et eResult = BapErr_OK;

#ifdef BAP_ROM_DATA_FAR_EMPTY
    DBGVAR ptr_t pBapConfig;
    DBGVAR ptr_t pFctList;
#else
    DBGVAR uint8_t pBapConfig[BAP_FCTID_BAPCONFIG_SIZE];
    DBGVAR uint8_t pFctList[BAP_FCTID_FCTLIST_SIZE];
#endif /* #ifdef BAP_ROM_DATA_FAR_EMPTY */

    /*lint -esym(613,apoLsgRomRow) */
    BAP_ASSERT(apoLsgRomRow);

#ifdef BAP_ROM_DATA_FAR_EMPTY
    pBapConfig = (ptr_t) apoLsgRomRow->paru8BAPConfigTable[0];
    pFctList = (ptr_t) apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList;
#else
    FAR_MEMCPY(pBapConfig, apoLsgRomRow->paru8BAPConfigTable[0], BAP_FCTID_BAPCONFIG_SIZE);
    FAR_MEMCPY(pFctList, apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList, BAP_FCTID_FCTLIST_SIZE);
#endif /* #ifdef BAP_ROM_DATA_FAR_EMPTY */

    /* Das FSG muss die internen FctIds im Sendepuffer setzen */
    eResult = BAP_InitSendBufferByteSequence(apoLsgRomRow->lsgId
        , BAP_FCTID_BAPCONFIG
        , pBapConfig
        , BAP_FCTID_BAPCONFIG_SIZE);

    if(BapErr_OK == eResult)
    {
        /* Spezialfall ohne FctList pruefen */
        if(NULL != BAP_GetLsgFctRomRow(apoLsgRomRow, BAP_FCTID_FCTLIST))
        {
            eResult = BAP_InitSendBufferByteSequence(apoLsgRomRow->lsgId
                , BAP_FCTID_FCTLIST
                , pFctList
                , BAP_FCTID_FCTLIST_SIZE);
        }
    }
    if(BapErr_OK == eResult)
    {
        /* Spezialfall ohne Heartbeat pruefen */
        if(NULL != BAP_GetLsgFctRomRow(apoLsgRomRow, BAP_FCTID_HEARTBEAT))
        {
            eResult = BAP_InitSendBufferInt8(apoLsgRomRow->lsgId
                , BAP_FCTID_HEARTBEAT
                , apoLsgRomRow->u8Heartbeat);
        }
    }
    if(BapErr_OK != eResult)
    {
        /* Falls irgendwas in den unteren Schichten fehlschlaegt immer InvalidArg */
        BAP_DBG_InvalidArgError(apoLsgRomRow->lsgId);
        eResult = BapErr_InvalidArg;
    }
    return eResult;
    /*lint +esym(613,apoLsgRomRow) */
}
#endif /* #ifdef BAP_FSG */


#ifdef BAP_USES_DATATYPE_16
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR void
BAP_BAL_Int16toDataStream(DBGVAR uint16_t au16Value, DBGVAR uint8_t *apPtr)
{
    /*lint -esym(613,apPtr)*/
    BAP_ASSERT(apPtr);

    apPtr[0] = (uint8_t) au16Value; /* & 0xff ist implizit durch den Cast) */
    apPtr[1] = (uint8_t) (au16Value >> 8);
    return;
    /*lint +esym(613,apPtr)*/
}
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_16
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR uint16_t
BAP_BAL_DataStreamToInt16(DBGVAR volatile_ptr_t apPtr)
{
    DBGVAR uint16_t u16Return;
    if (NULL == apPtr)
    {
        u16Return = (uint16_t)0u;
    }
    else
    {
        u16Return = (uint16_t)apPtr[0] + (uint16_t)((uint16_t)apPtr[1] << 8);
    }
    return u16Return;
}
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_32
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR void
BAP_BAL_Int32toDataStream(DBGVAR uint32_t au32Value, DBGVAR uint8_t *apPtr)
{
    /*lint -esym(613,apPtr)*/
    BAP_ASSERT(apPtr);

    apPtr[0] = (uint8_t)(au32Value);       /* & 0xff ist implizit durch den Cast */
    apPtr[1] = (uint8_t)(au32Value >> 8);  /* & 0xff ist implizit durch den Cast */
    apPtr[2] = (uint8_t)(au32Value >> 16); /* & 0xff ist implizit durch den Cast */
    apPtr[3] = (uint8_t)(au32Value >> 24);
    return;

    /*lint +esym(613,apPtr)*/
}
#endif /* BAP_USES_DATATYPE_32 */


#ifdef BAP_USES_DATATYPE_32
/*
 *  Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR uint32_t
BAP_BAL_DataStreamToInt32(DBGVAR volatile_ptr_t apPtr)
{
    DBGVAR uint32_t u32Return;
    if (NULL == apPtr)
    {
        u32Return = 0u;
    }
    else
    {
        u32Return = ((uint32_t)apPtr[0] + ((uint32_t)apPtr[1] << 8) + ((uint32_t)apPtr[2] << 16) + ((uint32_t)apPtr[3] << 24));
    }
    return u32Return;
}
#endif /* BAP_USES_DATATYPE_32 */

/*
 * Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR DBGVAR bool_t
BAP_BAL_CheckIfFunctionIsValid(BapFctRomRow_pot apoFctRomRow
                             , BapLsgRamRow_cpot apoLsgRamRow)
{
    DBGVAR bool_t bIsValid;

    /*lint -esym(613,apoFctRomRow)*/
    BAP_ASSERT(apoFctRomRow);
    /*lint -esym(613,apoLsgRamRow)*/
    BAP_ASSERT(apoLsgRamRow);

    if ( (BAP_FCTID_GETALL < apoFctRomRow->fctId)    /* GetAll braucht nicht in Fct.list zu sein (wg. Kompatibilitaet zu BAP1.2) */
        && (BapLayerLsgStat_Initialized != apoLsgRamRow->eLsgStatus)
        && (0 == (apoLsgRamRow->BAP_aru8FunctionList[BINARY_ID(apoFctRomRow->fctId) / 8] & (0x80 >> (BINARY_ID(apoFctRomRow->fctId) % 8))) )
    )
    {
        /* Fct. ist nicht in aktueller Fct.list und wird demzufolge purged */
        BAP_DBG_RxDataPurgedEvent();

        /* Fct.Id ist nicht in Fct.list enthalten */
        bIsValid = BAP_FALSE;
    }
    else
    {
        bIsValid = BAP_TRUE;
    }

    return bIsValid;

    /*lint +esym(613,apoFctRomRow)*/
    /*lint +esym(613,apoLsgRamRow)*/
}


#ifdef BAP_ASG
/*
 * Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR BapError_et
BAP_BAL_RequestVoidGet(BapInternalParameters_cpot apoPars)
{
    DBGVAR BapError_et eResult = BapErr_OK;
    DBGVAR uint8_t u8FctIndex;
    DBGVAR uint8_t u8TableSize;

    /* Hier muss zwischen der Einzel Cache Gueltigkeit der Properties */
    /* und der Gesamtgueltigkeit des Cache bei BAP_FCTID_GETALL unterschieden werden */
    /*lint -esym(613,apoPars)*/ /* Ist bereits vom Aufrufer abgeprueft */
    if (BAP_FCTID_GETALL == apoPars->fctId)
    {
        if(BapBalDataStat_Valid == BAP_BAL_GetLsgCacheStatus(apoPars->poLsgRomRow))
        {
            /* Bei gueltigem GesamtCache gleich eine STATUSALL Meldung erzeugen und keine Botschaft ueber Bus senden */
            BAP_IndicationVoid(apoPars->lsgId, BAP_FCTID_GETALL, BapInd_Data);
        }
        else /* Cache Zustand von BAP_FCTID_GETALL ist ungueltig */
        {
            /* Alle Valid Cache Eintraege gehen in den GetAll Zustand ueber und Aussenden auf Bus an FSG */
            u8TableSize = apoPars->poLsgRomRow->u8FctRomTableSize;
            for(u8FctIndex=0; u8FctIndex<u8TableSize; ++u8FctIndex)
            {
                BapBalRamRow_pot poBalRamRow = apoPars->poLsgRomRow->poFctRomTable[u8FctIndex].poBalRamRow;
                if ( (NULL != poBalRamRow) && (BapBalDataStat_Valid == poBalRamRow->eDataStatus) )
                {
                    poBalRamRow->eDataStatus = BapBalDataStat_GetAll;
                }
            }
            eResult = BAP_BPL_Send(apoPars);
        }
    }
    /* Existiert ueberhaupt ein Cache fuer diese Funktion? && */
    /* Automatisches Beantworten von DataGet wenn der Einzelcache Zustand nicht Invalid ist */
    else
    if ( (NULL != apoPars->poFctRomRow->poBalRamRow)
         && (BapBalDataStat_Invalid != apoPars->poFctRomRow->poBalRamRow->eDataStatus) )
    {
        BapData_put pxValue = &apoPars->poFctRomRow->poBalRamRow->xValue;

        /* gueltiger Cache vorhanden => direkt Indication aufrufen */
        switch(apoPars->poFctRomRow->eRxDataType)
        {
            case BapDt_Int8:
                BAP_IndicationInt8(apoPars->lsgId, apoPars->fctId, BapInd_Data, pxValue->u8Value);
                break;
            case BapDt_Int16:
                BAP_IndicationInt16(apoPars->lsgId, apoPars->fctId, BapInd_Data, pxValue->u16Value);
                break;
            case BapDt_Int32:
                BAP_IndicationInt32(apoPars->lsgId, apoPars->fctId, BapInd_Data, pxValue->u32Value);
                break;
            case BapDt_FixedByteSequence:
            case BapDt_ByteSequence:
                BAP_IndicationByteSequence(apoPars->lsgId, apoPars->fctId, BapInd_Data
                    , pxValue->oBufferWithLength.aru8Buffer
                    , pxValue->oBufferWithLength.u16Length);
                break;
            default:
                break;
        }
    }
    /* Wenn Get nicht aus dem Cache beantwortet werden kann, dann an FSG schicken */
    else
    {
#ifdef BAP_USES_ARRAYS
        /* Array/Get ist als BAP_RequestVoid nicht erlaubt! */
        if(BapFctCls_Array == apoPars->poFctRomRow->eFunctionClass)
        {
            BAP_DBG_InvalidArgError(apoPars->lsgId);
            eResult = BapErr_InvalidArg;
        }
        else
#endif /* #ifdef BAP_USES_ARRAYS */
        {
            eResult = BAP_BPL_Send(apoPars);
        }
    }
    /*lint +esym(613,apoPars)*/
    return eResult;
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_FSG
/*
 * Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR BapDataType_et
BAP_BAL_FsgDataReceived(BapInternalParameters_cpot apoPars, DBGVAR const BapIndication_et aeIndication)
{
    DBGVAR bool_t giveNoIndication = BAP_FALSE;
    DBGVAR BapDataType_et eDataType = BapDt_None;

    /*lint -esym(613,apoPars)*/ /* Ist bereits vom Aufrufer abgeprueft */
    /* Spezialbehandlungen fuer FSG durchfuehren */
    if ( (BapInd_DataGet == aeIndication)
        && ( (BapFctCls_Property == apoPars->poFctRomRow->eFunctionClass) || (BapFctCls_Cache == apoPars->poFctRomRow->eFunctionClass) )
       )
    {
        /* Properties & Cache werden direkt vom Stack beantwortet, Arrays werden als Indication gemeldet */
        giveNoIndication = BAP_TRUE;
        if (apoPars->fctId == BAP_FCTID_GETALL)
        {
            BAP_DBG_GetAllEvent(apoPars->lsgId);
            /* Sonderbehandlung fuer GetAll im BAL-FSG nicht mehr erforderlich, da BCL dies nun tut. */
        }

        /* DataGet wird direkt beantwortet ohne Indication */
        /* fuer Property und Cache */
        BAP_BPL_ResendStatus(apoPars);
    }

    /* Indications nur wenn das FSG gestartet wurde */
    if( !giveNoIndication && (BapInd_Invalid != aeIndication)
        && (BapLayerLsgStat_Running == apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus))
    { /* Sonderbehandlung fuer Void-Indications */
#ifdef BAP_USES_METHODS
        /* Abort oder Processing im FSG immer Void */
        if ( (BapInd_Abort == aeIndication) || (BapInd_Processing_CNF == aeIndication) )
        {
            eDataType = BapDt_Void;
        }
        else
#endif /* #ifdef BAP_USES_METHODS */
        {
            eDataType = apoPars->poFctRomRow->eRxDataType;
        }
    }
    /*lint +esym(613,apoPars)*/
    return eDataType;
}
#endif /* BAP_FSG */


#ifdef BAP_ASG
/*
 * Diese Funktion wird in bap_balutil.h dokumentiert.
 */
BAP_IMPL_FAR BapDataType_et
BAP_BAL_AsgDataReceived(DBGVAR volatile_ptr_t apConvertedData
    , BapInternalParameters_cpot apoPars
    , DBGVAR BapIndication_et * const apeIndication)
{
    DBGVAR bool_t giveNoIndication = BAP_FALSE;
    DBGVAR BapDataType_et eDataType = BapDt_None;

    /*lint -esym(613,apoPars)*/ /* Ist bereits vom Aufrufer abgeprueft */
    /*lint -esym(613,apeIndication)*/ /* Sicher, dass bereits vom Aufrufer gesetzt */
    /* Spezialbehandlungen fuer ASG durchfuehren */
    if ( (BapInd_Data == *apeIndication) || (BapOp_PropHeartbeatStatus==apoPars->eOpCode) )
    {
        switch(apoPars->fctId)
        {
            case BAP_FCTID_FCTLIST:
                /* Sonderbehandlung fuer FunctionList im ASG */
                if(BapErr_IncompatibleDataSpecification == BAP_BAL_CheckFunctionListAsg(apConvertedData, apoPars->poLsgRomRow))
                {
                    /* ungueltige FctList nicht an Appl melden */
                    giveNoIndication = BAP_TRUE;
                }
                else
                {
                    BAP_BAL_UpdateLsgCacheStatus(apoPars->lsgId);
                }
                break;
            case BAP_FCTID_GETALL:
                /* Sonderbehandlung fuer GETALL-Block-Antwort im ASG */
                BAP_BAL_ProcessStatusAll(apoPars->u16Length, apConvertedData, apoPars->poLsgRomRow);
                break;
            default:
                /* nichts tun */
                break;
        }
    }

    /* Sonderbehandlung fuer Status und Heartbeats im ASG */
    if ( (BapOp_PropHeartbeatStatus == apoPars->eOpCode)
        && (BapLayerLsgStat_Running == apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus) )
    {
        BAP_DBG_HeartbeatProcessedEvent(apoPars->lsgId);
    }

    /* Falls HB auf Fct.Id ohne Cache kommt wird die If Bedingung nicht erfuellt! */
    if(/* Pruefung auf Property ist per Config sichergestellt ueber CacheAvailable
        (BapFctCls_Property == poFctRomRow->eFunctionClass) */
        (BapLayerLsgStat_Running == apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus)
        && ((BapOp_PropHeartbeatStatus == apoPars->eOpCode) || (BapOp_PropStatus == apoPars->eOpCode))
        && (NULL != apoPars->poFctRomRow->poBalRamRow)
        )
    {
        /* Alter Status muss zwischengespeichert werden, da UpdateCache dies ueberschreibt */
        DBGVAR BapBalDataStatus_et eOldState = apoPars->poFctRomRow->poBalRamRow->eDataStatus;
        DBGVAR bool_t bValueHasChanged = BAP_BAL_UpdateCache(apoPars->poFctRomRow, apConvertedData, apoPars->u16Length);

        if ( (BapBalDataStat_Valid == eOldState) && bValueHasChanged )
        {
            /* Im Falle einer Wertaenderung eines Heartbeat auch Indication */
            *apeIndication = BapInd_Data; /* Heartbeat erzeugt auch Indication! */
        }
        else
        {
            /* Wertaenderung spielt keine Rolle: Status macht immer Indication (BapInd_Data) */
        }

        BAP_BAL_UpdateLsgCacheStatus(apoPars->lsgId);
    }

    /* Indications nur wenn das ASG komplett gestartet wurde oder im Falle einer BAPConfig-Nachricht */
    if((BapInd_Invalid != *apeIndication)
        && !giveNoIndication
        && ((BAP_FCTID_BAPCONFIG == apoPars->fctId) || (BapLayerLsgStat_Running == apoPars->poLsgRomRow->poLsgRamRow->eLsgStatus)))
    {
        /* StatusAll unterdruecken, da dies von UpdateLsgCacheStatus gemeldet wird */
        if ((BapInd_Data == *apeIndication) && (BAP_FCTID_GETALL == apoPars->fctId))
        {
            /* nichts melden, Datentyp bleibt auf BapDt_None */
        }
        else
        {
            eDataType = apoPars->poFctRomRow->eRxDataType;
        }
    }
    /*lint +esym(613,apeIndication)*/
    /*lint +esym(613,apoPars)*/
    return eDataType;
}
#endif /* BAP_ASG */


#ifdef BAP_USES_ACKNOWLEDGE
/*  Diese Funktion ist in bap_balutil.h dokumentiert */
BAP_IMPL_FAR BapAcknowledge_et
BAP_BAL_GetAcknowledge(BapFctRomRow_pot apoFctRomRow, BapOpCodes_et aeOpCode)
{
    BapAcknowledge_et eAck = BapAck_Nothing;
#if defined(BAP_ASG) && defined(BAP_FSG)
    BapLsgRomRow_pot poLsgRomRow;
#endif /* #if defined(BAP_ASG) && defined(BAP_FSG) */

    /*lint -esym(613, apoFctRomRow) */
    BAP_ASSERT(apoFctRomRow);

#if defined(BAP_ASG) && defined(BAP_FSG)
    poLsgRomRow = BAP_GetLsgRomRow(apoFctRomRow->lsgId);
    eAck = BAP_OpCode2AcknowledgeMatrix[poLsgRomRow->eSGType][apoFctRomRow->eFunctionClass][aeOpCode];  /*lint !e613 Falls fct existiert, dann gibt es auch lsg */
#else
    eAck = BAP_OpCode2AcknowledgeMatrix[apoFctRomRow->eFunctionClass][aeOpCode];
#endif  /* #else defined(BAP_ASG) && defined(BAP_FSG) */

    return eAck;
    /*lint +esym(613, apoFctRomRow) */

}
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */

/* CallBack-Funktionen */
