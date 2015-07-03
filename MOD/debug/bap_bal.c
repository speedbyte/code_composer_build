/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BAL
 *
 * $Archive: /dev/BAP/bal/bap_bal.c $
 * $Revision: 232 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 * Dr. Jochen Kreuzinger, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Implementierung der BAL Schicht im BAP
 *  Projekt.
 *
 *  Diese C-Datei enthaelt nicht die Funktionen BAP_Indication*,
 *  da diese von der Applikation, die BAP nutzt, implementiert
 *  werden muessen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-06-16   1.4.0.1     ALA         Bugfix: Im ASG loeste die interne Anfrage BapReq_DataGet BAPConfig ein BAP_Acknowledge aus
 * 2006-06-02   1.4         ALA         Unterstuetzung von Bap_Acknowledge
 * 2006-06-01   1.4         ALA         Optimierung Speicherplatzbedarf: Zusammenfassung Init/Shutdown
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-11-15   1.3.1.3     ALA         Bugfix: BAP_RequestVoid mit BapReq_Processing lieferte immer BapErr_InvalidArg
 * 2005-10-24   1.3.1.2     ALA         pu8SendData nach aru8SendData umbenannt. Ueberfluessiges const innerhalb eines Cast entfernt.
 * 2005-04-29   1.3         CRI         InitPLsgBuffer durch GetLsgRomRow ersetzt
 * 2005-04-18   1.3         CRI         Asymmetrische DT: RequestVoid Unterstuetzung
 * 2005-04-13   1.3         ALA         Optimierung der Datenstrukturen, Optimierung Parameterliste BAP_BAL_CheckRequest 
 * 2005-04-12   1.3         CRI         ASG&FSG zusammen
 * 2005-04-11   1.3         CRI         Processing_CNF loest IndicationVoid aus (nur FSG)
 * 2005-04-11   1.3         CRI         CacheGetBS prueft auf NULL Pointer
 * 2005-04-10   1.3         CRI         Beim Setzen einer neuen Fct.list (initsendbuffer) muessen
 *                                      Fct. 2+3 dabei sein
 * 2005-04-08   1.3         CRI         Error Indications werden anhand der Fct.list ueberprueft
 * 2005-04-08   1.3         CRI         Testen auf NULL Pointer Uebergabe bei Requests
 * 2005-04-06   1.3         CRI         FSG kann bereits mit Subset der Fct.list starten und spaeter 
 *                                      zu einem Superset umkonfigurieren
 * 2005-04-04   1.3         CRI         In BAP_BPL_DataReceived klare Trennung zwischen ifdefs fuer FSG/ASG 
 * 2005-04-04   1.3         CRI         Zustandsmaschine fuer Einzelcachegueltigkeit und HB Indications
 * 2005-04-01   1.3         CRI         Einzelcachegueltigkeit und Gesamtcachegueltigkeit (StatusAll) 
 * 2005-03-30   1.3         CRI         Init,Shutdown und InitSendBufferByteSequence sind
 *                                      an dynamische Fct.lists angepasst
 * 2005-03-18   1.3         CRI         Wertaenderung im Cache durch HB loest Indication aus
 * 2005-03-17   1.3         CRI         RequestByteSequence auch mit Arrays & Opcode Get/Changed moeglich,
 *                                      dafuer nicht mehr in RequestVoid erlaubt
 * 2005-03-17   1.3         CRI         Get/Changed bei Array loest keine IndicationVoid 
 *                                      mehr aus sondern IndicationByteSequence
 * 2005-03-17   1.2.1       CRI         Bugfix: Bei asymmetrischen Properties (eigentlich erst ab BAP1.3 erlaubt) wird bei 
 *                                      einem Get im Zusammenhang mit dem Cache statt Typ des Empfangs der Typ des Sendens benutzt.
 * 2004-11-17   1.2         JOK         Ungueltige FctList und BAPConfig wird nicht mehr an Appl gemeldet
 *                                      sondern nur noch der entsprechende Fehler.
 * 2004-11-04   1.2         ALA         CacheSetRequest nach InitSendBuffer umbenannt. 
 *                                      BAP_RequestError Funktion und Aufrufe BAP_IndicationError hinzugefuegt.
 * 2004-10-21   1.2         JOK         Hilfsfunktionen aus bap_bal in bap_balutil verschoben
 * 2004-10-21   1.2         JOK         Asymetrische Konfiguration der Datentypen/DatenSize in RX/TX moeglich
 * 2004-10-21   1.2         JOK         Unterstuetzung von Methoden
 * 2004-10-21   1.2         JOK         Verzicht auf FctId FunctionList moeglich
 * 2004-10-21   1.2         JOK         Verzicht auf FctId Heartbeat moeglich
 * 2004-10-21   1.2         JOK         GetAll-Block Verarbeitung im ASG
 * 2004-10-21   1.2         JOK         FunctionList nicht mehr relevant fuer Versionspruefung
 * 2004-09-23   1.2         JOK         FunctionList vom FSG kann ein Subset vom ASG sein
 * 2004-09-23   1.2         JOK         Cache-Gueltigkeit abhaengig von FunctionList des FSG
 * 2004-09-23   1.2         JOK         Kompatible Erweiterung der FunctionList nicht mehr erlaubt
 * 2004-09-17   1.1.1       ALA         Anpassung type const * const nach const type * const wegen
 *                                      Problemen mit CVI-Compiler
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         BAP_BAL_CheckCacheSetRequest() entfernt und Funktionalitaet
 *                                      in BAP_BAL_CheckRequest() mit aufgenommen
 * 2004-06-07   1.1         JOK         Laufzeitoptimierungen in Funktionen durch ersetzen der aLsgId
 *                                      durch den Zeiger apoLsgRomRow in die LSG-Tabelle
 * 2004-06-07   1.1         JOK         Einfuehrung der BapDt_FixedByteSequence fuer nicht-segmentierte
 *                                      ByteSequenzen
 ******************************************************************/


/* Systemincludes mit <...> */
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_util.h"           /* Allgemeine Hilfsfunktionen fuer BAP */
#include "bap_bal.h"            /* BAP interne BAL Schnittstelle */
#include "bap_balconfig.h"      /* Struktur der Konfigtabellen */
#include "bap_balutil.h"        /* Hilfsfunktionen fuer die BAL Schicht */
#include "bap_bpl.h"            /* darunterliegende Schicht */
#include "bap_debug.h"          /* Diagnoseschnittstelle */

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */


/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

static BAP_IMPL_FAR BapError_et
BAP_BAL_Init(bool_t abIsShutdown, lsgId_t aLsgId);

/* Definition (Implementierung) von statischen Funktionen */

/**
 *  Diese Funktion initialisiert die darunterliegenden Schichten.
 *
 *  @param abIsShutdown - Entscheidet darueber, welche Zustaende erlaubt sind.
 *
 *  @param aLsgId - gibt an, welches Lsg initialisiert werden soll. 
 *
 *  @remarks: Wird sowohl von BAP_Init als auch BAP_Shutdown verwendet.
 */
static BAP_IMPL_FAR BapError_et
BAP_BAL_Init(bool_t abIsShutdown, lsgId_t aLsgId)
{
    /* Zum Zwischenspeichern des Funktionsergebnisses */
    DBGVAR BapError_et eResult;

    /* Bedingung muss sein: in pLsgRomRowTable keine NULL Eintraege */    
    BapLsgRomRow_pot poLsgRomRow = BAP_GetLsgRomRow(aLsgId);

    if (NULL == poLsgRomRow)
    {
        BAP_DBG_InvalidArgError(aLsgId);
        eResult = BapErr_InvalidArg;
    }
    else 
    /* Ueberpruefe Zustandsmaschine */
    if (abIsShutdown != (bool_t)(BapLayerLsgStat_NoInit != poLsgRomRow->poLsgRamRow->eLsgStatus))
    {
        BAP_DBG_InvalidStateError(aLsgId);
        eResult = BapErr_InvalidState;
    }
    else
    {                  
        /* Die darunterliegende Schicht initialisieren */
        eResult = BAP_BPL_Init(poLsgRomRow);
    }

    if(eResult == BapErr_OK)
    { 
        /*NULLpointer wird ueber eResult oben abgefangen */
        /*lint -esym(613, poLsgRomRow) */
        poLsgRomRow->poLsgRamRow->eLsgStatus = BapLayerLsgStat_Initialized;
        
#ifdef BAP_FSG
#if defined(BAP_ASG) && defined(BAP_FSG)
        if (BapSG_FSG == poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
        {
            /* Das FSG kopiert die default ROM Fct.list in den RAM Buffer */
            FAR_MEMCPY(poLsgRomRow->poLsgRamRow->BAP_aru8FunctionList
                 , poLsgRomRow->BAP_aru8FunctionList
                 , BAP_FCTID_FCTLIST_SIZE);
            /* Das FSG muss die internen FctIds im Sendepuffer setzen */
            eResult = BAP_BAL_InitFSGBuffers(poLsgRomRow);
        }
#endif /* #ifdef BAP_FSG */
#ifdef BAP_ASG
#if defined(BAP_ASG) && defined(BAP_FSG)
        else 
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
        {
            /* Das ASG muss den Cache initialisieren */
            /* Fct.list aus ROM wird in das RAM uebernommen */
            BAP_BAL_InvalidateCache(poLsgRomRow);
        }
#endif /* #ifdef BAP_ASG */                
        BAP_DBG_SetBalState(aLsgId, BapLayerLsgStat_Initialized);
        /*lint +esym(613, poLsgRomRow) */
    }

    return eResult;
}


/* Definition (Implementierung) von globalen Funktionen */

/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_RequestVoid(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    /* oPars.pData = NULL - wird schon von BAP_InitInternalParameters gemacht */
    /* oPars.u16Length = VOID_SIZE; - wird schon von BAP_InitInternalParameters gemacht */
    
    eResult = BAP_BAL_CheckRequest(&oPars, aeRequest, BapChkDt_VoidIn);

    /* Fuer das Kombigeraet muessen hier keine SG Unterscheidungen gemacht werden, da
       eOpCode bereits in CheckRequest mit dem SG Type bestimmt wird. Passt der Request
       nicht zum SG Type so gibt es BapOpInvalid und keine Aktion ausgefuehrt
       Die #ifdefs bleiben drin fuer die Einzelvarianten*/
    if((BapErr_OK == eResult) && (BapOp_Invalid != oPars.eOpCode))
    {
        /* Aktionen je nach RequestType ausfuehren */
        switch(aeRequest)
        {
#ifdef BAP_FSG
            case BapReq_Data:       /* speziell fuer VOID Datentyp */
                eResult = BAP_BPL_Send(&oPars);
                break;
#ifdef BAP_USES_METHODS
            case BapReq_Processing: /* speziell fuer VOID Datentyp */
            case BapReq_Result:     /* speziell fuer VOID Datentyp */
                eResult = BAP_BPL_Send(&oPars);
                break;
#endif /* #ifdef BAP_USES_METHODS */
#endif /* #ifdef BAP_FSG */
#ifdef BAP_ASG
            case BapReq_Abort:
            case BapReq_Start:      /* speziell fuer VOID Datentyp */
            case BapReq_StartResult:/* speziell fuer VOID Datentyp */
            case BapReq_DataSetGet: /* speziell fuer VOID Datentyp */
            case BapReq_DataSet:    /* speziell fuer VOID Datentyp */
                eResult = BAP_BPL_Send(&oPars);
                break;
            case BapReq_DataGet: 
                eResult = BAP_BAL_RequestVoidGet(&oPars);
                break;            
#endif /* #ifdef BAP_ASG */
            /* alle anderen RequestTypes sind nicht erlaubt bei Void */
            /* BapReq_Invalid, BapReq_Changed */
            default:
                BAP_DBG_InvalidArgError(oPars.lsgId);
                eResult = BapErr_InvalidArg;
                break;
        }
    }
    return eResult;
}


#ifdef BAP_USES_DATATYPE_8
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint8_t au8Data)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT8_SIZE;
    oPars.pData = CAST_TO_PTR( &au8Data );

    eResult = BAP_BAL_CheckRequest(&oPars, aeRequest, BapChkDt_ScalarIn);

    /* Fuer das Kombigeraet muessen hier keine SG Unterscheidungen gemacht werden, da
       eOpCode bereits in CheckRequest mit dem SG Type bestimmt wird. Passt der Request
       nicht zum SG Type so gibt es BapOpInvalid und keine Aktion ausgefuehrt
       Die #ifdefs bleiben drin fuer die Einzelvarianten*/
    if((BapErr_OK == eResult) && (BapOp_Invalid != oPars.eOpCode))
    {
        /* Aktionen je nach RequestType ausfuehren */
        switch(aeRequest)
        {
#ifdef BAP_ASG
            case BapReq_DataSetGet:
            case BapReq_DataSet:
            case BapReq_Start:
            case BapReq_StartResult:
#endif /* BAP_ASG */
#ifdef BAP_FSG
            case BapReq_Data:
#ifdef BAP_USES_METHODS
            case BapReq_Result:
            case BapReq_Processing:
#endif /* BAP_USES_METHODS */
#endif /* BAP_FSG */
                eResult = BAP_BPL_Send(&oPars);
                break;
            /* alle anderen RequestTypes sind nicht erlaubt bei Int8 */
            /* BapReq_DataGet, BapReq_Abort, BapReq_Changed, BapReq_Processing, BapReq_Invalid */
            default:
                BAP_DBG_InvalidArgError(oPars.lsgId);
                eResult = BapErr_InvalidArg;
                break;
        }
    } 
    return eResult;
}
#endif /* BAP_USES_DATATYPE_8 */


#ifdef BAP_USES_DATATYPE_16
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint16_t au16Data)
{
    DBGVAR uint8_t aru8SendData[2];
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    /* Wert vor dem Senden ins Little-Endian-Format konvertieren */
    BAP_BAL_Int16toDataStream(au16Data, aru8SendData);

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT16_SIZE;
    oPars.pData = CAST_TO_PTR( aru8SendData );

    eResult = BAP_BAL_CheckRequest(&oPars, aeRequest, BapChkDt_ScalarIn);

    /* Fuer das Kombigeraet muessen hier keine SG Unterscheidungen gemacht werden, da
       eOpCode bereits in CheckRequest mit dem SG Type bestimmt wird. Passt der Request
       nicht zum SG Type so gibt es BapOpInvalid und keine Aktion ausgefuehrt
       Die #ifdefs bleiben drin fuer die Einzelvarianten*/
    if((BapErr_OK == eResult) && (BapOp_Invalid != oPars.eOpCode))
    {
        /* Aktionen je nach RequestType ausfuehren */
        switch(aeRequest)
        {
#ifdef BAP_ASG
            case BapReq_DataSetGet:
            case BapReq_DataSet:
            case BapReq_Start:
            case BapReq_StartResult:
#endif /* BAP_ASG */
#ifdef BAP_FSG
            case BapReq_Data:
#ifdef BAP_USES_METHODS
            case BapReq_Result:
            case BapReq_Processing:
#endif /* BAP_USES_METHODS */
#endif /* BAP_FSG */
                eResult = BAP_BPL_Send(&oPars);
                break;
            /* alle anderen RequestTypes sind nicht erlaubt bei Int16 */
            /* BapReq_DataGet, BapReq_Abort, BapReq_Changed, BapReq_Processing, BapReq_Invalid */
            default:
                BAP_DBG_InvalidArgError(oPars.lsgId);
                eResult = BapErr_InvalidArg;
                break;
        }
    } 
    return eResult;
}
#endif /* BAP_USES_DATATYPE_16 */


#ifdef BAP_USES_DATATYPE_32
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_RequestInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const uint32_t au32Data)
{
    DBGVAR uint8_t aru8SendData[4];
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    /* Wert vor dem Senden ins Little-Endian-Format konvertieren */
    BAP_BAL_Int32toDataStream(au32Data, aru8SendData);

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT32_SIZE;
    oPars.pData = CAST_TO_PTR( aru8SendData );

    eResult = BAP_BAL_CheckRequest(&oPars, aeRequest, BapChkDt_ScalarIn);

    /* Fuer das Kombigeraet muessen hier keine SG Unterscheidungen gemacht werden, da
       eOpCode bereits in CheckRequest mit dem SG Type bestimmt wird. Passt der Request
       nicht zum SG Type so gibt es BapOpInvalid und keine Aktion ausgefuehrt
       Die #ifdefs bleiben drin fuer die Einzelvarianten*/
    if((BapErr_OK == eResult) && (BapOp_Invalid != oPars.eOpCode))
    {
        /* Aktionen je nach RequestType ausfuehren */
        switch(aeRequest)
        {
#ifdef BAP_ASG
            case BapReq_DataSetGet:
            case BapReq_DataSet:
            case BapReq_Start:
            case BapReq_StartResult:
#endif /* BAP_ASG */
#ifdef BAP_FSG
            case BapReq_Data:
#ifdef BAP_USES_METHODS
            case BapReq_Result:
            case BapReq_Processing:
#endif /* BAP_USES_METHODS */
#endif /* BAP_FSG */
                eResult = BAP_BPL_Send(&oPars);
                break;
            /* alle anderen RequestTypes sind nicht erlaubt bei Int32 */
            /* BapReq_DataGet, BapReq_Abort, BapReq_Changed, BapReq_Processing, BapReq_Invalid */
            default:
                BAP_DBG_InvalidArgError(oPars.lsgId);
                eResult = BapErr_InvalidArg;
                break;
        }
    } 
    return eResult;
}
#endif /* BAP_USES_DATATYPE_32 */


#ifdef BAP_USES_DATATYPE_BS
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_RequestByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapRequest_et aeRequest
    , const ptr_t apData
    , const uint16_t au16InLength)
{
    DBGVAR BapError_et eResult = BapErr_OK;
    BapInternalParameters_ot oPars;

    if ( (NULL == apData) && (au16InLength > (uint16_t)0) )
    {
        eResult = BapErr_InvalidArg;
    }  

    if (BapErr_OK == eResult)
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.pData = apData;
        oPars.u16Length = au16InLength;

        eResult = BAP_BAL_CheckRequest(&oPars, aeRequest, BapChkDt_VariableIn);
    }

    /* Fuer das Kombigeraet muessen hier keine SG Unterscheidungen gemacht werden, da
       eOpCode bereits in CheckRequest mit dem SG Type bestimmt wird. Passt der Request
       nicht zum SG Type so gibt es BapOpInvalid und keine Aktion ausgefuehrt
       Die #ifdefs bleiben drin fuer die Einzelvarianten*/
    /*lint -esym(644,oPars)*/
    if((BapErr_OK == eResult) && (BapOp_Invalid != oPars.eOpCode))
    /*lint +esym(644,oPars)*/
    {
        /* Aktionen je nach RequestType ausfuehren */
        switch(aeRequest)
        {
#ifdef BAP_ASG
            case BapReq_DataSetGet:
            case BapReq_DataSet:
            case BapReq_Start:
            case BapReq_StartResult:
#endif /* BAP_ASG */
#ifdef BAP_FSG
            case BapReq_Data:
#ifdef BAP_USES_METHODS
            case BapReq_Result:
            case BapReq_Processing:
#endif /* BAP_USES_METHODS */
#endif /* BAP_FSG */
                eResult = BAP_BPL_Send(&oPars);
                break;
#ifdef BAP_ASG          
            case BapReq_DataGet: /* Ist nur fuer Arrays im ASG erlaubt */
#endif /* BAP_ASG */
#ifdef BAP_USES_ARRAYS
#ifdef BAP_FSG  
            case BapReq_Changed: /* Ist nur fuer Arrays im FSG erlaubt */
#endif /* BAP_FSG */
                if(BapFctCls_Array == oPars.poFctRomRow->eFunctionClass)
                {
                    eResult = BAP_BPL_Send(&oPars);                   
                }
                else 
                { 
                    BAP_DBG_InvalidArgError(oPars.lsgId);
                    eResult = BapErr_InvalidArg;
                }
                break;
#endif /* #ifdef BAP_USES_ARRAYS */
            /* alle anderen RequestTypes sind nicht erlaubt bei ByteSequence: */
            /* BapReq_Abort, BapReq_Processing, BapReq_Invalid */
            /* BapReq_DataGet und BapReq_Changed sind nur bei Arrays erlaubt */
            default:
                BAP_DBG_InvalidArgError(oPars.lsgId);
                eResult = BapErr_InvalidArg;
                break;
        }
    }
    return eResult;
}
#endif /* BAP_USES_DATATYPE_BS */


#ifdef BAP_FSG
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et 
BAP_RequestError(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const BapError_et aeErrorCode)
{
    DBGVAR BapError_et eResult = BapErr_OK;
    BapInternalParameters_ot oPars;
    DBGVAR uint8_t u8ErrorCode = (uint8_t) aeErrorCode;

    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;

    oPars.poLsgRomRow = BAP_GetLsgRomRow(oPars.lsgId);
    oPars.poFctRomRow = BAP_GetLsgFctRomRow(oPars.poLsgRomRow, oPars.fctId);

    /* Pruefen ob die Funktion existiert und der ErrorCode erlaubt ist (kein interner Fehlerwert) */
    if(    (NULL == oPars.poFctRomRow) 
#if defined(BAP_ASG) && defined(BAP_FSG) 
        /*lint !e613 wenn poLsgRomRow = NULL dann ist auch poFctRomRow = NULL */
        || (BapSG_FSG != oPars.poLsgRomRow->eSGType) /* Error darf nur fuer FSG aufgerufen werden */
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
        || (BINARY_ID(oPars.fctId) > MAX_FCT_ID) /*lint !e650 enums haben mindestens 8 bit */ 
        || ((0x40 > (uint8_t)aeErrorCode) && (BapErr_OK != aeErrorCode))) 
    {
        BAP_DBG_InvalidArgError(oPars.lsgId);
        eResult = BapErr_InvalidArg;
    }
    /* Ueberpruefe Zustandsmaschine */
    else if(BapLayerLsgStat_Running != oPars.poLsgRomRow->poLsgRamRow->eLsgStatus)
    {
        BAP_DBG_InvalidStateError(oPars.lsgId);
        eResult = BapErr_InvalidState;
    }
    /* Request ist erlaubt */
    else
    {
        oPars.eOpCode = BapOp_PropError;
        oPars.pData = (ptr_t) &u8ErrorCode;
        oPars.u16Length = 1u;
        eResult = BAP_BPL_SendError(&oPars);
    }
    
    return eResult;
}
#endif /* #ifdef BAP_FSG */


#if defined(BAP_ASG)  && defined (BAP_USES_DATATYPE_8)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint8_t * const apu8OutData)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    /* Pointer muss existieren */
    if (NULL == apu8OutData)
    {
        eResult = BapErr_InvalidArg;
    }
    else
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.u16Length = INT8_SIZE;

        eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarOut);
    }
    /*lint -esym(644,oPars)*/ /* Wurde durch BAP_BAL_CheckRequest gesetzt */
    /*lint -esym(613,oPars)*/    
    BAP_ASSERT(oPars);   
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* CacheGet darf nur fuer ASG aufgerufen werden! */
    /* Erste bedingung stellt sicher das LSG&FCT auch existieren (CheckRequest)! */
    if ((BapErr_OK == eResult) && (BapSG_ASG != oPars.poLsgRomRow->eSGType))
    {
        eResult = BapErr_InvalidArg;
    }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */ 

    if(BapErr_OK == eResult)
    {
        /* Cache muss vorhanden sein */
        if(NULL == oPars.poFctRomRow->poBalRamRow)      
        {
            BAP_DBG_CacheNotAvailableError(oPars.lsgId);
            eResult = BapErr_CacheNotAvailable;
        }
        else
        {
            /* Einzelcache muss gueltig sein */
            
            if(   (BapBalDataStat_Valid == oPars.poFctRomRow->poBalRamRow->eDataStatus) 
                ||(BapBalDataStat_GetAll == oPars.poFctRomRow->poBalRamRow->eDataStatus) )
            {
                /*lint -esym(613,apu8OutData)*/ /* Wird schon am Anfang geprueft */
                *apu8OutData = oPars.poFctRomRow->poBalRamRow->xValue.u8Value;
                /*lint +esym(613,apu8OutData)*/
            }
            else
            {
                BAP_DBG_CacheInvalidError(oPars.lsgId);
                eResult = BapErr_CacheInvalid;
            }
            /*lint +esym(644,oPars)*/
        }
    } 
    /*lint +esym(613,apu8OutData)*/ 
    /*lint +esym(644,oPars)*/
    return eResult;
}
#endif /* ASG && BAP_USES_DATATYPE_8 */


#if defined(BAP_ASG)  && defined (BAP_USES_DATATYPE_16)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint16_t * const apu16OutData)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    /* Pointer muss existieren */
    if (NULL == apu16OutData)
    {
        eResult = BapErr_InvalidArg;
    }     
    else
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.u16Length = INT16_SIZE;
    
        eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarOut);
    }
    /*lint -esym(644,oPars)*/ /* wird durch BAP_BAL_CheckRequest initialisiert*/    
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* CacheGet darf nur fuer ASG aufgerufen werden! */
    /* Erste bedingung stellt sicher das LSG&FCT auch existieren (CheckRequest)! */
    if ( (BapErr_OK == eResult) && (BapSG_ASG != oPars.poLsgRomRow->eSGType) )  /*lint !e644 (in BAP_BAL_CheckRequest) */
    {
        eResult = BapErr_InvalidArg;
    }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */ 
    
    if(BapErr_OK == eResult)
    {
        /* Cache muss vorhanden sein */      
        if(NULL == oPars.poFctRomRow->poBalRamRow)  /*lint !e644 (in BAP_BAL_CheckRequest) */
        {
            BAP_DBG_CacheNotAvailableError(oPars.lsgId);
            eResult = BapErr_CacheNotAvailable;
        }
        else
        {
            /* Einzelcache muss gueltig sein */
            if(   (BapBalDataStat_Valid == oPars.poFctRomRow->poBalRamRow->eDataStatus) 
                ||(BapBalDataStat_GetAll == oPars.poFctRomRow->poBalRamRow->eDataStatus) )
            {
                /*lint -esym(613,apu16OutData)*/ /* Wird durch Abfrage zu Beginn abgefangen */
                *apu16OutData = oPars.poFctRomRow->poBalRamRow->xValue.u16Value;
                /*lint +esym(613,apu16OutData)*/ 
            }
            else
            {
                BAP_DBG_CacheInvalidError(oPars.lsgId);
                eResult = BapErr_CacheInvalid;
            }
        }
    } 
    /*lint +esym(644,oPars)*/
    return eResult;
}
#endif /* ASG && BAP_USES_DATATYPE_16 */


#if defined(BAP_ASG)  && defined (BAP_USES_DATATYPE_32)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , uint32_t * const apu32OutData)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;

    /* Pointer muss existieren */
    if (NULL == apu32OutData)
    {
        eResult = BapErr_InvalidArg;
    } 
    else
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.u16Length = INT32_SIZE;
        
        eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarOut);
    }
    /*lint -esym(644,oPars)*/ /* wird durch BAP_BAL_CheckRequest initialisiert*/
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* CacheGet darf nur fuer ASG aufgerufen werden! */
    /* Erste bedingung stellt sicher das LSG&FCT auch existieren (CheckRequest)! */
    if ( (BapErr_OK == eResult) && (BapSG_ASG != oPars.poLsgRomRow->eSGType) )  /*lint !e644 (in BAP_BAL_CheckRequest) */
    {
        eResult = BapErr_InvalidArg;
    }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */     
    
    if(BapErr_OK == eResult)
    {
        /* Cache muss vorhanden sein */
        if(NULL == oPars.poFctRomRow->poBalRamRow)  /*lint !e644 (in BAP_BAL_CheckRequest) */
        {
            BAP_DBG_CacheNotAvailableError(oPars.lsgId);
            eResult = BapErr_CacheNotAvailable;
        }
        else
        {
            /* Einzelcache muss gueltig sein */
            if(   (BapBalDataStat_Valid == oPars.poFctRomRow->poBalRamRow->eDataStatus) 
                ||(BapBalDataStat_GetAll == oPars.poFctRomRow->poBalRamRow->eDataStatus) )
            {
                /*lint -esym(613,apu32OutData)*/ /* Wird durch Abfrage zu Beginn abgefangen */
                *apu32OutData = oPars.poFctRomRow->poBalRamRow->xValue.u32Value;
                /*lint +esym(613,apu32OutData)*/
            }
            else
            {
                BAP_DBG_CacheInvalidError(oPars.lsgId);
                eResult = BapErr_CacheInvalid;
            }
        }
    } 
    /*lint +esym(644,oPars)*/  
    return eResult;
}
#endif /* ASG && BAP_USES_DATATYPE_32 */


#if defined(BAP_ASG)  && defined (BAP_USES_DATATYPE_BS)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_CacheGetRequestByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , ptr_t * const appOutData
    , uint16_t * const apu16OutLength)
{
    DBGVAR BapError_et eResult = BapErr_OK;
    BapBufferWithLength_pot pBufWLen;
    BapInternalParameters_ot oPars;

    /* Pointer muessen existieren */
    if ( (NULL == appOutData) || (NULL == apu16OutLength) )
    {
        eResult = BapErr_InvalidArg;
    }

    if (BapErr_OK == eResult) 
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.u16Length = VOID_SIZE;    /* Bei CacheGet ist es egal welcher Parameter */
    
        eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_VariableOut);
    }   
    /*lint -esym(644,oPars)*/ /* wird durch BAP_BAL_CheckRequest initialisiert*/ 
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* CacheGet darf nur fuer ASG aufgerufen werden! */
    /* Erste bedingung stellt sicher das LSG&FCT auch existieren (CheckRequest)! */
    if ( (BapErr_OK == eResult) && (BapSG_ASG != oPars.poLsgRomRow->eSGType) )  /*lint !e644 (in BAP_BAL_CheckRequest) */
    {
        eResult = BapErr_InvalidArg;
    }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */  
    
    if(eResult == BapErr_OK)
    {
        /* Cache muss vorhanden sein */
        if(NULL == oPars.poFctRomRow->poBalRamRow)  /*lint !e644 (in BAP_BAL_CheckRequest) */
        {
            BAP_DBG_CacheNotAvailableError(oPars.lsgId);
            eResult = BapErr_CacheNotAvailable;
        }
        else
        {
            /* Einzelcache muss gueltig sein */
            if(   (BapBalDataStat_Valid == oPars.poFctRomRow->poBalRamRow->eDataStatus) 
                ||(BapBalDataStat_GetAll == oPars.poFctRomRow->poBalRamRow->eDataStatus) )
            {
                pBufWLen = &oPars.poFctRomRow->poBalRamRow->xValue.oBufferWithLength; 
                /*lint -esym(613,appOutData)*/ /* Wird durch Abfrage zu Beginn abgefangen */
                *appOutData = (ptr_t) pBufWLen->aru8Buffer;
                /*lint +esym(613,appOutData)*/
                /*lint -esym(613,apu16OutLength)*/
                *apu16OutLength = pBufWLen->u16Length;
                /*lint +esym(613,apu16OutLength)*/
            }
            else
            {
                BAP_DBG_CacheInvalidError(oPars.lsgId);
                eResult = BapErr_CacheInvalid;
            }
        }
    } 
    /*lint +esym(644,oPars)*/ 
    return eResult;
}
#endif /* ASG && BAP_USES_DATATYPE_BS */


#if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_8)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt8(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint8_t au8Data)
{
    BapInternalParameters_ot oPars;
    DBGVAR BapError_et eResult;

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT8_SIZE;
    oPars.pData = CAST_TO_PTR( &au8Data );

    eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarInit);
    
    if(eResult == BapErr_OK)
    {
#if defined(BAP_ASG) && defined(BAP_FSG) 
        /* InitSendBuffer darf nur fuer FSG aufgerufen werden! */
        if (BapSG_FSG != oPars.poLsgRomRow->eSGType)
        {
            eResult = BapErr_InvalidArg;
        }
        else
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */ 
        {
            /* Schreibt den Wert runter in den BCL - der OpCode ist hier nur ein Dummy-Wert ist */
            BAP_BPL_BufferSet(&oPars);
        }
    } 
    return eResult;
}
#endif /* #if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_8) */


#if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_16)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt16(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint16_t au16Data)
{
    BapInternalParameters_ot oPars;
    DBGVAR BapError_et eResult;
    DBGVAR uint8_t aru8SendData[2];

    BAP_BAL_Int16toDataStream(au16Data, aru8SendData);

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT16_SIZE;
    oPars.pData = CAST_TO_PTR( aru8SendData );

    eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarInit);
    
    if(eResult == BapErr_OK)
    {  
#if defined(BAP_ASG) && defined(BAP_FSG) 
        /* InitSendBuffer darf nur fuer FSG aufgerufen werden! */
        if (BapSG_FSG != oPars.poLsgRomRow->eSGType)
        {
            eResult = BapErr_InvalidArg;
        }
        else
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */ 
        {
            /* Schreibt den Wert runter in den BCL - der OpCode ist hier nur ein Dummy-Wert ist */
            BAP_BPL_BufferSet(&oPars);
        }
    } 
    return eResult;
}
#endif /* #if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_16) */


#if defined(BAP_FSG) && defined (BAP_USES_DATATYPE_32)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferInt32(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const uint32_t au32Data)
{
    BapInternalParameters_ot oPars;
    DBGVAR BapError_et eResult;
    DBGVAR uint8_t aru8SendData[4];
    
    BAP_BAL_Int32toDataStream(au32Data, aru8SendData);
    
    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.u16Length = INT32_SIZE;
    oPars.pData = CAST_TO_PTR( aru8SendData );

    eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_ScalarInit);
    
    if(eResult == BapErr_OK)
    {
#if defined(BAP_ASG) && defined(BAP_FSG) 
        /* InitSendBuffer darf nur fuer FSG aufgerufen werden! */
        if (BapSG_FSG != oPars.poLsgRomRow->eSGType)
        {
            eResult = BapErr_InvalidArg;
        }
        else
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */ 
        {
            /* Schreibt den Wert runter in den BCL - der OpCode ist hier nur ein Dummy-Wert */
            BAP_BPL_BufferSet(&oPars);
        }
    } 
    return eResult;
}
#endif /* #if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_32) */


#if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_BS)
/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_InitSendBufferByteSequence(const lsgId_t aLsgId
    , const fctId_t aFctId
    , const ptr_t apData
    , const uint16_t au16Length)
{
    DBGVAR BapError_et eResult = BapErr_OK;
    BapInternalParameters_ot oPars;

    BAP_ASSERT(apData);
    /*lint -esym(613,apData) siehe nachfolgende Pruefung */
    /*lint -esym(644,poLsgRomRow)*/ /* Ist durch BAP_BAL_CheckRequest spaeter sichergestellt */  
    if ( (NULL == apData) && ((uint16_t)0 < au16Length) )
    {
        eResult = BapErr_InvalidArg;
    }  

    if (BapErr_OK == eResult)
    {
        BAP_InitInternalParameters(&oPars);
        oPars.lsgId = aLsgId;
        oPars.fctId = aFctId;
        oPars.u16Length = au16Length;
        oPars.pData = apData;
    
        eResult = BAP_BAL_CheckRequest(&oPars, BapReq_Invalid, BapChkDt_VariableInit);
    }
    /*lint -esym(644,oPars)*/ /* Durch BAP_BAL_CheckRequest sichergestellt */
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* InitSendBuffer darf nur fuer FSG aufgerufen werden! */
    /* Erste bedingung stellt sicher das LSG&FCT auch existieren (CheckRequest)! */
    if ( (BapErr_OK == eResult) && (BapSG_FSG != oPars.poLsgRomRow->eSGType) )  /*lint !e644 (in BAP_BAL_CheckRequest) */
    {
        eResult = BapErr_InvalidArg;
    }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */   
    
    if(eResult == BapErr_OK)
    {
        if (BAP_FCTID_FCTLIST == oPars.fctId) 
        {
            uint8_t u8CurrentFctId;
        
            /* Neue Fct.list muss auf alle Faelle Fct.ID 2+3 beinhalten */
            if ( (apData[0] & 0x30) != 0x30)
            {
                eResult = BapErr_IncompatibleDataSpecification;
            } 
        
            /* Eine neue Fct.list darf ein Superset der initalen im ROM gespeicherten sein */
            /* Man kann dann bereits mit einem Subset anfangen! Allerdings muessen fuer das Superset */
            /* entsprechende XML Eintraege vorgenommen worden sein, welche RomRows generiert haben */
            for(u8CurrentFctId = BINARY_ID(BAP_FCTID_FCTLIST); u8CurrentFctId < 64; u8CurrentFctId++)
            {
                /* Bit gesetzt in neuer Fct.list, aber existiert auch eine RomRow dafuer? */
                if (   (0 != (apData[u8CurrentFctId / 8] & (0x80 >> (u8CurrentFctId % 8))))
                    /*lint -esym( 960, 33 ) Funktion hat keinen Seiteneffekt */
                    && (NULL == BAP_GetLsgFctRomRow(oPars.poLsgRomRow, ((const fctId_t)u8CurrentFctId))) )
                    /*lint +esym( 960, 33 ) */
                {
                    eResult = BapErr_IncompatibleDataSpecification;
                }
            }       
            if(BapErr_OK == eResult)
            {
                /* FunctionList uebernehmen */
                /*lint -e668*/ /* (void*)apData != NULL */
                MEMCPY(oPars.poLsgRomRow->poLsgRamRow->BAP_aru8FunctionList, apData
                       , BAP_FCTID_FCTLIST_SIZE);
                /*lint +e668*/
            }
        }           
    } 
    
    /* Wenn alles geklappt hat den Puffer setzen */
    if (BapErr_OK == eResult) 
    {
        /* Schreibt den Wert runter in den BCL - der OpCode ist hier nur ein Dummy-Wert ist */
        BAP_BPL_BufferSet(&oPars);     
    }       
    return eResult;
    /*lint +esym(644,oPars)*/ 
    /*lint +esym(644,poLsgRomRow)*/ 
    /*lint +esym(613,apData) */
}
#endif /* #if defined(BAP_FSG)  && defined (BAP_USES_DATATYPE_BS) */



/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_Init(lsgId_t aLsgId)
{
    /* Zum Zwischenspeichern des Funktionsergebnisses */
    DBGVAR BapError_et eResult;

    /*  Wenn Compiler meldet: Bedingung immer falsch,
        dann darf das Makro im Makefile definiert werden */
#ifndef BAP_DISABLE_TYPE_CHECKING

    /* Teste, ob die Compilereinstellungen und die
       Konfiguration aufeinander passen, d.h. die
       Standardtypen die richtige Groesse haben.
       Diese Zeilen werden typischerweise, falls
       die Typen stimmen, vom Compiler wegoptimiert */
    if(/* sizeof(int8_t)  != 1 || */ (sizeof(uint8_t)  != 1u) ||   /*lint !e912 !e506 !e774 */
         /* sizeof(int16_t) != 2 || */ (sizeof(uint16_t) != 2u) || /*lint !e912 !e506 !e774 */
         /* sizeof(int32_t) != 4 || */ (sizeof(uint32_t) != 4u))   /*lint !e912 !e506 !e774 */
    /* muss immer FALSE sein */
    {
        eResult = BapErr_InvalidArg;    /* DBG wird mit || (eResult != BapErr_OK) gesetzt */
    }
    else
#endif /* #ifndef BAP_DISABLE_TYPE_CHECKING */
    {
        eResult = BAP_BAL_Init(BAP_FALSE, aLsgId);
    }
    return eResult;
}


/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_Start(lsgId_t aLsgId)
{
    DBGVAR BapError_et eResult;
    BapInternalParameters_ot oPars;
    DBGVAR BapLayerLsgStatus_et eNewLayerStatus = BapLayerLsgStat_Initialized;
#ifdef BAP_ROM_DATA_FAR_EMPTY
	/* Werte koennen direkt aus dem ROM kopiert werden */
#else
	/* Temporaerer Speicher, da Werte aus dem ROM nicht direkt versendet werden koennen */
    DBGVAR uint8_t aru8NearRamForBAPConfig[BAP_FCTID_BAPCONFIG_SIZE];
#endif

    BAP_InitInternalParameters(&oPars);
    oPars.lsgId = aLsgId;
    oPars.poLsgRomRow = BAP_GetLsgRomRow(oPars.lsgId);

    if(NULL == oPars.poLsgRomRow)
    {
        BAP_DBG_InvalidArgError(oPars.lsgId);
        eResult = BapErr_InvalidArg;
    }
    else
    {
        /* Ueberpruefe Zustandsmaschine */
        if (BapLayerLsgStat_Initialized != oPars.poLsgRomRow->poLsgRamRow->eLsgStatus)
        {
            BAP_DBG_InvalidStateError(oPars.lsgId);
            eResult = BapErr_InvalidState;
        }
        else
        {
            /* Die darunterliegende Schicht initialisieren */
            eResult = BAP_BPL_Start(oPars.poLsgRomRow);
#ifdef BAP_FSG  /* Fuer reine ASG Variante nicht noetig, da immer BapErr_OK */
            if(eResult == BapErr_OK)
#endif
            {
                oPars.fctId = BAP_FCTID_BAPCONFIG;
                oPars.poFctRomRow = BAP_GetLsgFctRomRow(oPars.poLsgRomRow, oPars.fctId);

#ifdef BAP_ASG
#if defined(BAP_ASG) && defined(BAP_FSG)
                if (BapSG_ASG == oPars.poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
                {
                    /* Beim Start des ASG die BAPConfig vom FSG abfragen */                    
                    oPars.eOpCode = BapOp_PropGet;
                    /* Loeschen der Gesamtcachegueltigkeit */
                    oPars.poLsgRomRow->poLsgRamRow->fState.fCacheValid = BAP_FALSE;
                    BAP_DBG_SetCacheValidState(aLsgId, BAP_FALSE);
                    eNewLayerStatus = BapLayerLsgStat_WaitForConfig;
                }
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
#if defined(BAP_ASG) && defined(BAP_FSG)
                else 
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */                
                {
                    /* Beim Start des FSG dem ASG die BAPConfig mit Reset schicken */                    
                    oPars.eOpCode = BapOp_PropReset_Set;
#ifdef BAP_ROM_DATA_FAR_EMPTY
                    /* Nimm den Wert direkt aus dem ROM */
                    oPars.pData = (ptr_t) *oPars.poLsgRomRow->paru8BAPConfigTable;
#else
                    /* Kopiere den Wert vorher aus dem BAP_ROM_DATA_FAR ROM ins RAM */
                    oPars.pData = (ptr_t) aru8NearRamForBAPConfig;
                    FAR_MEMCPY(aru8NearRamForBAPConfig, *oPars.poLsgRomRow->paru8BAPConfigTable, BAP_FCTID_BAPCONFIG_SIZE);
#endif
                    oPars.u16Length = BAP_FCTID_BAPCONFIG_SIZE;
                    eNewLayerStatus = BapLayerLsgStat_Running;
                } 
#endif /* #ifdef BAP_FSG */
                /* Sende den erforderlichen Request aus */
                eResult = BAP_BPL_Send(&oPars);

                if(eResult != BapErr_OK)
                {
                    (void) BAP_BPL_Shutdown(oPars.poLsgRomRow);
                    /* Falls irgendwas in den unteren Schichten fehlschlaegt immer InvalidArg */
                    BAP_DBG_InvalidArgError(oPars.lsgId);
                    eResult = BapErr_InvalidArg;
                }
                else 
                {
                    (oPars.poLsgRomRow->poLsgRamRow->eLsgStatus) = eNewLayerStatus;
                    BAP_DBG_SetBalState(oPars.lsgId, eNewLayerStatus);
                }
            }
        }
    }
    return eResult;
}


/*
 *  Diese Funktion ist in bap.h dokumentiert
 */
BAP_IMPL_FAR BapError_et
BAP_Shutdown(lsgId_t aLsgId)
{
    return BAP_BAL_Init(BAP_TRUE, aLsgId);
}


/* CallBack-Funktionen */

/*
 *  Wird vom BPL als Callback aufgerufen. Dokumentiert in bap_bpl.h
 */
BAP_IMPL_FAR void
BAP_BPL_DataReceived(BapInternalParameters_cpot apoPars)
{
    DBGVAR BapError_et eResult;
    /* wird auf jeden Fall spaeter durch BAP_OpCode2IndicationMatrix gesetzt werden */
    DBGVAR BapIndication_et eIndication = BapInd_Invalid; 
    /*lint -esym(613,apoPars)*/
    DBGVAR volatile_ptr_t pConvertedData = apoPars->pData;
    DBGVAR BapDataType_et eDataType = BapDt_None;
#ifdef BAP_USES_DATATYPE_16
    DBGVAR uint16_t u16Data = 0u;
#endif /* #ifdef BAP_USES_DATATYPE_16 */
#ifdef BAP_USES_DATATYPE_32
    DBGVAR uint32_t u32Data = 0uL;
#endif /* #ifdef BAP_USES_DATATYPE_32 */

    eResult = BAP_BAL_CheckDataReceived(apoPars);

    /* Folgende Pruefung bringt das ASG vom WaitForConfig in den Running Zustand */
    /* Dabei wird ueberprueft ob die uebermittelte BAPCONFIG gueltig ist */
#ifdef BAP_ASG /* Behandlung muss in ASG durchgefuehrt werden */
    if ((BapErr_OK == eResult) && (BAP_FCTID_BAPCONFIG == apoPars->fctId) && ((BapOp_PropReset_Set == apoPars->eOpCode) || (BapOp_PropStatus == apoPars->eOpCode))
#if defined(BAP_ASG) && defined(BAP_FSG) /* Im Kombigeraet muss die Abfrage ob es ein ASG ist, drin sein */
         && (BapSG_ASG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
        )                 
    {
        /* ungueltige BAPConfig nicht an Appl melden */
        if (BAP_BAL_CheckBapConfigAsg(apoPars, pConvertedData))
        {
            eResult = BapErr_IncompatibleProtocolVersion;
        }
    }        
#endif /* #ifdef BAP_ASG */

    /* Wenn untere Bedingung wahr --> (poLsgRomRow != NULL) */
    if(BapErr_OK == eResult)
    {
#if defined(BAP_USES_DATATYPE_16) || defined(BAP_USES_DATATYPE32)
        /* Empfangsdaten je nach Datentyp aufbereiten */
        switch(apoPars->poFctRomRow->eRxDataType) 
        {
#ifdef BAP_USES_DATATYPE_16
            case BapDt_Int16:
                u16Data = BAP_BAL_DataStreamToInt16(pConvertedData);
                pConvertedData = (ptr_t)&u16Data;
                break;
#endif /* BAP_USES_DATATYPE_16 */
#ifdef BAP_USES_DATATYPE_32
            case BapDt_Int32:
                u32Data = BAP_BAL_DataStreamToInt32(pConvertedData);
                pConvertedData = (ptr_t)&u32Data;
                break;
#endif /* BAP_USES_DATATYPE_32 */
            default:
                break;
        } 
#endif /* #if defined(BAP_USES_DATATYPE_16) || defined(BAP_USES_DATATYPE32) */

#if defined(BAP_ASG) && defined(BAP_FSG)
        eIndication = BAP_OpCode2IndicationMatrix[apoPars->poLsgRomRow->eSGType][apoPars->poFctRomRow->eFunctionClass][apoPars->eOpCode];
#else
        eIndication = BAP_OpCode2IndicationMatrix[apoPars->poFctRomRow->eFunctionClass][apoPars->eOpCode];
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */

#if defined(BAP_ASG) && defined(BAP_FSG) /* Im Kombigeraet muss die Abfrage ob es ein ASG ist, drin sein */
        if (BapSG_ASG == apoPars->poLsgRomRow->eSGType) 
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */

#ifdef BAP_ASG /* Behandlung muss in ASG durchgefuehrt werden */
        {
            eDataType = BAP_BAL_AsgDataReceived(pConvertedData, apoPars, &eIndication);
        }
#endif /* BAP_ASG */

#if defined(BAP_ASG) && defined(BAP_FSG) /* Im Kombigeraet muss die Abfrage ob es ein FSG ist, drin sein */
        else 
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */

#ifdef BAP_FSG 
        {
            eDataType = BAP_BAL_FsgDataReceived(apoPars, eIndication);
        } 
#endif /* BAP_FSG */
    }   /* if(eResult == BapErr_OK) */

    /* Ist bei der Auswertung von FSG bzw. ASG ein Spezialfall aufgetreten der nicht nach oben */
    /* gemeldet werden darf ist der eDataType auf BapDt_None gesetzt und keine Indication wird ausgeloest*/
    
    /* In Abhaengigkeit der Datentypen die entsprechende Indication aufrufen */
    /* gilt auch fuer Array mit Changed! */
    switch(eDataType)
    {
        case BapDt_Int8:
            /*lint -esym(613,pConvertedData)*/ /* Von unterliegenden Schichten initialisiert */
            BAP_IndicationInt8(apoPars->lsgId, apoPars->fctId, eIndication, *(uint8_t*)pConvertedData);
            /*lint +esym(613,pConvertedData)*/
            break;
#ifdef BAP_USES_DATATYPE_16                        
        case BapDt_Int16:
            BAP_IndicationInt16(apoPars->lsgId, apoPars->fctId, eIndication, u16Data);
            break;
#endif  /* BAP_USES_DATATYPE_16 */
#ifdef BAP_USES_DATATYPE_32
        case BapDt_Int32:
            BAP_IndicationInt32(apoPars->lsgId, apoPars->fctId, eIndication, u32Data);
            break;
#endif  /* BAP_USES_DATATYPE_32 */
        case BapDt_FixedByteSequence:
#ifdef BAP_USES_SEGMENTATION
        case BapDt_ByteSequence:
#endif /* BAP_USES_SEGMENTATION */
            BAP_IndicationByteSequence(apoPars->lsgId, apoPars->fctId, eIndication, pConvertedData, apoPars->u16Length);
            break;
        case BapDt_Void:
            BAP_IndicationVoid(apoPars->lsgId, apoPars->fctId, eIndication);
            break;
        case BapDt_None: /* nichts zu tun! */
        default:
            break;
    }
    /*lint +esym(613,apoPars)*/
    return;
}

/*
 *  Wird vom BPL als Callback aufgerufen. Dokumentiert in bap_bpl.h
 */
BAP_IMPL_FAR void
BAP_BPL_ErrorIndication(DBGVAR lsgId_t aLsgId
    , DBGVAR fctId_t aFctId
    , DBGVAR BapError_et aeErrorCode)
{
#ifdef BAP_ASG
    DBGVAR bool_t giveNoIndication = BAP_FALSE;

    BapLsgRomRow_pot poLsgRomRow;
    BapLsgRamRow_cpot poLsgRamRow;
    BapFctRomRow_pot poFctRomRow;

    poLsgRomRow = BAP_GetLsgRomRow(aLsgId);
    if (poLsgRomRow
#if defined(BAP_ASG) && defined(BAP_FSG)
        && (BapSG_ASG == poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
        )
    {
        /* Falls lsg=0 oder fct=0 Indication nach oben melden == allgemeiner Fehler */
        if ( ((lsgId_t)0 != aLsgId) && ((fctId_t)0 != aFctId) )
        {
        
            poLsgRamRow = poLsgRomRow->poLsgRamRow;
            poFctRomRow = BAP_GetLsgFctRomRow(poLsgRomRow, aFctId);
            if (poLsgRamRow
                /*lint -esym( 960, 33 ) Funktion hat keinen Seiteneffekt */
                && !BAP_BAL_CheckIfFunctionIsValid(poFctRomRow, poLsgRamRow))
                /*lint +esym( 960, 33 ) */
            {
                /* Wenn nicht in Fct.list keine Indication melden */
                giveNoIndication = BAP_TRUE;
            }
        
        }
        /* Sonderbehandlung fuer Retry-Ueberwachung bei GetAll */
        if((BAP_FCTID_GETALL == aFctId) 
            && (BapErr_RetryNotSuccessful == aeErrorCode)
            /*lint -esym( 960, 33 ) Funktion hat keinen Seiteneffekt */
            && (BapBalDataStat_Valid == BAP_BAL_GetLsgCacheStatus(poLsgRomRow)))
            /*lint +esym( 960, 33 ) */
        {
            /* StatusAll wurde schon gemeldet, also Error ignorieren */
            giveNoIndication = BAP_TRUE;
        }

        /* Sonderbehandlung fuer Heartbeat-Ueberwachung */
        if(BapErr_TimeoutHeartbeat == aeErrorCode)
        {
            /* LSG lebt nicht mehr => Cache auf invalid setzten */
            BAP_BAL_InvalidateCache(poLsgRomRow);
        }

    }

    if(!giveNoIndication)
#endif /* BAP_ASG */
    {
        BAP_IndicationError(aLsgId, aFctId, aeErrorCode);
    } 

    return;
}

#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Diese Funktion wird vom BPL aufgerufen, wenn eine Nachricht erfolgreich verschickt wurde.
 *
 *  @param apoFctRomRow zeigt auf die Funktion 
 *  @param aeOpCode enthaelt den gesendeten OpCode
 */
BAP_IMPL_FAR void
BAP_BPL_Acknowledge(BapFctRomRow_pot apoFctRomRow, BapOpCodes_et aeOpCode)
{
    BapAcknowledge_et eAcknowledge;

    /*lint -esym(613, apoFctRomRow) */
    BAP_ASSERT(apoFctRomRow);

    eAcknowledge = BAP_BAL_GetAcknowledge(apoFctRomRow, aeOpCode);

    if (BapAck_Nothing != eAcknowledge)
    {
#ifdef BAP_ASG
        /* Die Abfrage stoert im Kombigeraet bei FSGs nicht */
        BapLsgRomRow_pot poLsgRomRow = BAP_GetLsgRomRow(apoFctRomRow->lsgId);
        if (poLsgRomRow && (BapLayerLsgStat_Running == poLsgRomRow->poLsgRamRow->eLsgStatus))
#endif /* #ifdef BAP_ASG */
        {
            BAP_Acknowledge(apoFctRomRow->lsgId, apoFctRomRow->fctId, eAcknowledge);
        }
    }

    /*lint +esym(613, apoFctRomRow) */
}
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */
