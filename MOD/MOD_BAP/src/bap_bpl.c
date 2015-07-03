/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BPL
 *
 * $Archive: /dev/BAP/bpl/bap_bpl.c $
 * $Revision: 172 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Hans-Micheal Schubert, Xcc Software AG, D-76137 Karlsruhe (Basis)
 * Andreas Laag, Xcc Software AG, D-76137 Karlsruhe (QM/Wartung)
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Implementierung der BPL Schicht im BAP
 *  Projekt.
 *
 *  Diese C-Datei enthaelt nicht die Funktionen BAP_BPL_DataReceived
 *  und BAP_BPL_ErrorIndication, da diese von der BAL Schicht
 *  implementiert wird.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-06-02   1.4         ALA         Unterstuetzung von Bap_Acknowledge
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2006-04-12   1.4         ALA         Optimierung der Task-Laufzeit (HB-Monitoring)
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer neue HB-Trigger.
 * 2006-03-14   1.4         ALA         Bugfix: BCL Fehler wurden im FSG unterdrueckt, wenn Processing-Timer aktiv war
 * 2005-10-24   1.3.1.2     ALA         const vor void Rueckgabetyp entfernt.
 * 2005-07-20   1.3.1       ALA         Bugfix: Kein erneutes Aussenden von Get, SetGet, StartResult, wenn Busy,
 *                                      doppeltes Abort entfernt.
 * 2005-04-13   1.3         CRI         StopRetries werden nun auch fuer FSG durchgefuehrt bei Init
 * 2005-04-13   1.3         ALA         Optimierung der Datenstrukturen. HB-TO Meldung nun auf Funktion 4.
 * 2005-04-10   1.3         CRI         Hinzufuegen von 2 Fkt. zur Zeitueberwachung im FSG
 * 2005-04-10   1.3         CRI         Erweitern von UpdateRetryOnOpcode, Send, SendError um Methoden
 * 2005-04-05   1.3         ALA         BapErr_Busy, falls Retry-Timeout auftritt ohne dass der Request bereits versendet wurde.
 * 2005-03-14   1.2.1       CRI         BCL_Start wird wieder aufgerufen
 * 2004-11-22   1.2         JOK         BapOp_MethAbortResultNoBusy wieder raus, da Processing und Ueberwachung von 
 *                                      Anwendung gemacht wird.
 * 2004-11-17   1.2         JOK         Interruptsperre in BAP_BPL_TaskRetryMonitoringFct() eingebaut
 * 2004-11-17   1.2         JOK         Funktion BAP_BPL_RetryAnswered() ausgegliedert
 * 2004-11-17   1.2         JOK         Globale Variable gu16HeartbeatTimerValue in LsgRam-Bereich uebernommen
 * 2004-11-04   1.2         ALA         BAP_BPL_SendError Funktion hinzugefuegt.
 * 2004-09-08   1.2         JOK         BapOp_MethAbortResult wird intern mit BapOp_MethAbortResultNoBusy verarbeitet
 *                                      damit ein AbortResult nach StartResult moeglich ist.
 * 2004-09-08   1.2         JOK         FktKlasse-Methode Aufrufe werden mittels Retry ueberwacht. Keine spezielle
 *                                      Ueberpruefung ob Start/Abort gerade laeuft.
 * 2004-09-03   1.2         JOK         Interne OpCodes werden nicht mehr an den BAL weitergegeben und
 *                                      fuehren damit nicht mehr zu einer Fehlermeldung in der Diagnose-Botschaft
 * 2004-09-03   1.2         JOK         Verarbeitung der HB-Meldung im ASG als Funktion extrahiert und
 *                                      nun auch vom BAL nutzbar
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Konfigurierte HB-Zeit ist in 100ms angegeben. Der BPL
 *                                      rechnet diese Zeit in Anzahl Task-Aufrufe fuer die
 *                                      interne Timerbehandlung um.
 * 2004-06-07   1.1         JOK         Beachtung des Sonderfalls von HB-Zeit 0 im FSG bedeutet
 *                                      kein HB-Mechanismuns im FSG
 * 2004-06-07   1.1         JOK         Beachtung des Sonderfalls von HB-Faktor 0 im ASG bedeutet
 *                                      keine HB-Ueberwachung des FSG im ASG.
 * 2004-06-07   1.1         JOK         Globale Variable gu16HeartbeatTimerValue eingefuegt
 * 2004-06-07   1.1         JOK         Funktion BAP_BPL_TaskRetryMonitoringLsg() in 
 *                                      BAP_BPL_TaskRetryMonitoringFct() umbenannt
 * 2004-06-07   1.1         JOK         Enumeration BapDir_Sink/Source entfernt
 * 2004-06-07   1.1         JOK         Funktion BAP_BPL_Shutdown() entfernt (siehe bap_bpl.h)
 ******************************************************************/
   
/* Systemincludes mit <...> */

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */

#include "bap_bpl.h"
#include "bap_bplconfig.h"
#include "bap_bplutil.h"
#include "bap_util.h"
#include "bap_bcl.h"    /* darunterliegende Schicht */
#include "bap_debug.h"
#include "bap_balutil.h"
#include "bap_balconfig.h"

/* Externe globale Variablen-Definitionen */

#ifdef BAP_FSG
/* Funktionszeiger mit Namen BAP_BPL_BufferSet, 
   der auf BAP_BCL_BufferSet zeigt */
BAP_IMPL_FAR void (* const BAP_BPL_BufferSet) 
    (BapInternalParameters_cpot apoPars) = BAP_BCL_BufferSet;
#endif /* #ifdef BAP_FSG */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

#ifdef BAP_ASG
static BAP_IMPL_FAR void
BAP_BPL_DoRetry(BapBplRamRow_pot apoBplRamRow, BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_ASG
static BAP_IMPL_FAR void 
BAP_BPL_TaskRetryMonitoringFct(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_ASG
static BAP_IMPL_FAR void
BAP_BPL_UpdateGetAllRetryOnOpCode(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_ASG
static BAP_IMPL_FAR void
BAP_BPL_UpdateRetryOnOpCode(BapFctRomRow_pot apoFctRomRow
    , DBGVAR BapOpCodes_et aeOpCode);
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
#ifdef BAP_USES_METHODS
static BAP_IMPL_FAR void
BAP_BPL_UpdateProcessingDispatching(BapFctRomRow_pot apoFctRomRow
    , DBGVAR BapOpCodes_et aeOpCode);
#else
#define BAP_BPL_UpdateProcessingDispatching(apoFctRomRow, aeOpCode) ((void)0)    
#endif /* #ifdef BAP_USES_METHODS */
#endif /* #ifdef BAP_FSG */

#ifdef BAP_ASG
static BAP_IMPL_FAR void 
BAP_BPL_RetryAnswered(DBGVAR lsgId_t aLsgId
    , BapBplRamRow_pot apoBplRamRow);
#endif /* #ifdef BAP_ASG */

/* Definition (Implementierung) von statischen Funktionen */

#ifdef BAP_ASG
/**
 *  Fuehrt ein Retry durch (Retry-Timer ist abgelaufen) wenn noch ein Retry
 *  durchzufuehren ist oder meldet ein BapErr_RetryNotSuccessful oder BapErr_RequestTimeout
 *  an die Applikation.
 *
 *  @param apoPars -\>poLsgRomRow Zeiger auf die Rom-Zeile des Lsg, wo das Retry-Monitoring durchgefuehrt werden soll.<br>
 *                 -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poFctRomRow Zeiger auf die Rom-Zeile der Funktion, wo das Retry-Monitoring durchgefuehrt werden soll.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>pData muss mit NULL vorbesetzt worden sein <br>
 *                 -\>u16Length muss mit 0 vorbesetzt worden sein <br>
 *                 -\>eOpCode muss mit BapOp_Invalid vorbesetzt worden sein
 *
 *  @param apoBplRamRow muss apoPars->poFctRomRow->poBplRamRow entsprechen.
 *
 *  @remarks
 *  Die Parameter duerfen nicht NULL sein.
 */
static BAP_IMPL_FAR void
BAP_BPL_DoRetry(BapBplRamRow_pot apoBplRamRow, BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613, apoBplRamRow) */
    BAP_ASSERT(apoBplRamRow);
    /*lint -esym(613, apoPars) */
    BAP_ASSERT(apoPars);
    
    if(apoBplRamRow->fRetryOrProcessingFlags.u6RetryCounter) /* Ist Counter noch nicht abgelaufen?*/
    {
        /* Timer fuer naechsten Retry wieder auf ROMWert hochsetzen */
        apoBplRamRow->u16RetryOrProcessingTimer = apoPars->poFctRomRow->nMaxRetryOrProcessingTime;

        /* Retry-Anzahl dekrementieren */
        apoBplRamRow->fRetryOrProcessingFlags.u6RetryCounter 
            = (apoBplRamRow->fRetryOrProcessingFlags.u6RetryCounter - 1u); /* -- geht nicht wegen CVI Compiler */

        /* Retry durchfuehren */
        BAP_BCL_Resend(apoPars);

        BAP_DBG_RetryProvokedEvent(apoPars->lsgId);
    }
    else
    {
        /*Jetzt sind alle Retry's verstrichen*/
        /*Dieses Send ist schief gegangen - also nicht mehr versuchen */
        apoBplRamRow->u16RetryOrProcessingTimer=0;

        apoBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;   /* Retry ausschalten */

        if (apoBplRamRow->fRetryOrProcessingFlags.bRequestSent)
        {
            /* Falls Request versendet wurde */

            /* uebergeordnete Schicht informieren! */
            BAP_BPL_ErrorIndication(apoPars->lsgId
                , apoPars->fctId 
                , BapErr_RetryNotSuccessful
            );

            BAP_DBG_RetryTimeoutError(apoPars->lsgId);
        }
        else
        {
            /* Falls Request nicht gesendet werden konnte */
            
            /* uebergeordnete Schicht informieren! */
            BAP_BPL_ErrorIndication(apoPars->lsgId
                , apoPars->fctId 
                , BapErr_RequestTimeout                 
            );

            BAP_DBG_RequestTimeoutError(apoPars->lsgId);
        }

        /* Sendevorgang abbrechen */
        BAP_BCL_AbortSend(apoPars);
    }
    /*lint +esym(613, apoBplRamRow) */
    /*lint +esym(613, apoPars) */
}
#endif  /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Fuehrt das RetryMonitoring fuer eine FctId durch (wg. max. Schachtelungstiefe in eigene Funktion ausgeglieder)
 *
 *  @param apoPars -\>poLsgRomRow Zeiger auf die Rom-Zeile des Lsg, wo das Retry-Monitoring durchgefuehrt werden soll.<br>
 *                 -\>lsgId bezeichnet das logische Steuergeraet.<br>
 *                 -\>poFctRomRow Zeiger auf die Rom-Zeile der Funktion, wo das Retry-Monitoring durchgefuehrt werden soll.<br>
 *                 -\>fctId bezeichnet die Funktion des angegebenen logischen Steuergeraetes.<br>
 *                 -\>pData muss mit NULL vorbesetzt worden sein <br>
 *                 -\>u16Length muss mit 0 vorbesetzt worden sein <br>
 *                 -\>eOpCode muss mit BapOp_Invalid vorbesetzt worden sein
 *
 */
static BAP_IMPL_FAR void
BAP_BPL_TaskRetryMonitoringFct(BapInternalParameters_cpot apoPars)
{
    BapBplRamRow_pot poBplRamRow;
    /* interne Hilfsfunktion */

    /*lint -esym(613, apoPars)*/
    BAP_ASSERT( apoPars != NULL );
    BAP_ASSERT( apoPars->pData == NULL );
    BAP_ASSERT( apoPars->eOpCode == BapOp_Invalid );
    BAP_ASSERT( apoPars->poLsgRomRow != NULL );
    BAP_ASSERT( apoPars->poFctRomRow != NULL );
    BAP_ASSERT( apoPars->poFctRomRow->fctId == apoPars->fctId );
    BAP_ASSERT( apoPars->poLsgRomRow->lsgId == apoPars->lsgId );

    poBplRamRow = apoPars->poFctRomRow->poBplRamRow;

    /* RamTabelle existiert (=> Retry ist moeglich) und der Retry ist aktiv, d.h. noch keine Antwort gekommen. */
    if( poBplRamRow && poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive )
    {
        /* Ist Timer abgelaufen? */
        switch (poBplRamRow->u16RetryOrProcessingTimer)
        {
            case 0:
                /* 0 bedeutet, dass der Retry-Timer gestoppt wurde (StartMsg empfangen),
                 * dann warte auf Antwort oder Intertelegrammzeit- oder Segmentierungsfehler 
                 * (= an dieser Stelle nichts zu tun).
                 */
                break;
            case 1:   
                /* 1 bedeutet, dass der Retry-Timer abgelaufen ist */
                BAP_BPL_DoRetry(poBplRamRow, apoPars);
                break;
            default:
                --poBplRamRow->u16RetryOrProcessingTimer;
                break;
        }
        /* Retry-Ueberwachung des Lsgs eingeschaltet lassen */
        apoPars->poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive = BAP_TRUE;
    }

    /*lint +esym(613, apoPars)*/
    return;
}
#endif /* #ifdef BAP_ASG */



#ifdef BAP_ASG
/**
 *  Schaltet den Retry-Timer aus und aktualisiert die Diagnose-Bits.
 *
 *  @param aLsgId bezeichnet das logische Steuergeraet
 *
 *  @param *apoBplRamRow zeigt auf die Retry-Informationen.
 */
static BAP_IMPL_FAR void 
BAP_BPL_RetryAnswered(DBGVAR lsgId_t aLsgId
    , BapBplRamRow_pot apoBplRamRow)
{
    /* interne Hilfsfunktion */
    /*lint -esym(613,apoBplRamRow) */
    BAP_ASSERT(apoBplRamRow);
    
    if (apoBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive)
    {
        BAP_DBG_RetryAnsweredEvent(aLsgId);
        apoBplRamRow->u16RetryOrProcessingTimer = 0;
        apoBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = 0;
        apoBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;
    }    
    return;
    /*lint +esym(613,apoBplRamRow) */
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/**
 *  Retriggert den Retry-Timer von GetAll bei Beantwortung durch einzelne HB-Status-Nachrichten
 *
 *  @param apoPars 
 *      -\>poFctRomRow Funktion, auf den sich der OpCode bezieht (falls HB-Status-Nachricht)
 *      -\> eOpCode Der OpCode, der ausgewertet werden soll.
 */
static BAP_IMPL_FAR void
BAP_BPL_UpdateGetAllRetryOnOpCode(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_ASSERT(apoPars);

    /* war es ein Heartbeat-Status? */
    if ( (BapOp_PropHeartbeatStatus == apoPars->eOpCode) && (BapFctCls_Property == apoPars->poFctRomRow->eFunctionClass))
    {
        BapBplRamRow_pot poBplRamRowGetAll;
        BapFctRomRow_pot poFctRomRowGetAll = BAP_GetLsgFctRomRow(apoPars->poLsgRomRow, BAP_FCTID_GETALL);  

        /* Retriggern von GetAll, falls bei GetAll gerade die Retry-Ueberwachung laueft */
        
        if (poFctRomRowGetAll)  /* Falls GetAll-FctId vorhanden */
        {
            poBplRamRowGetAll = poFctRomRowGetAll->poBplRamRow;
        }
        else
        {
            poBplRamRowGetAll = NULL;
        }
    
        if (poBplRamRowGetAll   /* Falls Retry-Ueberwachung bei GetAll konfiguriert */
            && poBplRamRowGetAll->fRetryOrProcessingFlags.bRetryOrProcessingActive  /* und gerade aktiv */)
        {
            /*lint -esym(613,poFctRomRowGetAll) *//* Zweig wird nie erreicht wenn poFctRomRowGetAll == NULL */
            poBplRamRowGetAll->u16RetryOrProcessingTimer = poFctRomRowGetAll->nMaxRetryOrProcessingTime;
            /*line +esym(613,poFctRomRowGetAll) */
        }
    }
    /*lint +esym(613,apoPars) */
}
#endif  /* #ifdef BAP_ASG */

#ifdef BAP_ASG
/**
 *  Verarbeitet einen OpCode und aktualisiert die Retry-Timer abhaengig vom OpCode.
 *
 *  @param *apoFctRomRow Funktion, auf den sich der OpCode bezieht.
 *
 *  @param aeOpCode Der OpCode, der ausgewertet werden soll.
 */
static BAP_IMPL_FAR void
BAP_BPL_UpdateRetryOnOpCode(BapFctRomRow_pot apoFctRomRow
    , DBGVAR BapOpCodes_et aeOpCode)
{
    BapBplRamRow_pot poBplRamRow;
        
    /* interne Hilfsfunktion */
    /*lint -esym(613,apoFctRomRow) */
    BAP_ASSERT(apoFctRomRow);
    
    poBplRamRow = apoFctRomRow->poBplRamRow;

    /* Teste, ob Ram fuer Retry konfiguriert */
    if(poBplRamRow)
    {
        switch(aeOpCode)
        {
        case BapOp_PropStatus:
        /*case BapOp_ArrStatus:*/   /* gleicher numerischer Wert */
        /*case BapOp_MethResult:*/   /* gleicher numerischer Wert */
            /* Stoppen des Retry-timer LAH 1.0 Seite 51*/
            BAP_BPL_RetryAnswered(apoFctRomRow->lsgId, poBplRamRow);
            break;
#ifdef BAP_USES_SEGMENTATION
        case BapOp_StartMessage:
            /* Beim Erhalt einer segmentierten Nachricht wird die Retry-Zeitueberwachung
               angehalten, da schon ein Teil der Antwort da ist. Der vollstaendige Empfang
               wird ueber den BCL sichergestellt */
            poBplRamRow->u16RetryOrProcessingTimer = 0;
            /* Nicht den Counter zuruecksetzen, Retry bleibt aktiv, aber gestoppt (Timer=0) */
            /* Retry muss ja nach Intertelegrammzeitfehler wieder eingeschaltet werden. */
            BAP_DBG_RetryStoppedEvent(apoFctRomRow->lsgId);
            break;
#endif /* #ifdef BAP_USES_SEGMENTATION */
        case BapOp_MessageSent:             
            if(poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive)
            {
                /* Request wurde versendet */
                poBplRamRow->fRetryOrProcessingFlags.bRequestSent = BAP_TRUE;
                /*Retriggern des Retry-Timer Seite 52*/
                poBplRamRow->u16RetryOrProcessingTimer = apoFctRomRow->nMaxRetryOrProcessingTime;
                BAP_DBG_RetryRetriggeredEvent(apoFctRomRow->lsgId);
            }
            break;
        case BapOp_PropHeartbeatStatus:
        /* case BapOp_ArrChanged: (gleicher numerischer Wert)*/
        /* case BapOp_MethProcessing: (gleicher numerischer Wert)  */
        /*   --> eigentlich nicht gewollt, aber keine Probleme zu erwarten */

#ifdef BAP_USES_METHODS
            if (BapFctCls_Method == apoFctRomRow->eFunctionClass)
            {
                /* Reset des Counters bei Erhalt eines Processing, damit kein Resend von StartResult */
                poBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = 0;
            }
#endif /* #ifdef BAP_USES_METHODS */

            /*Retriggern des Retry-Timer Seite 52*/
            if(poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive)
            {
                /* Retriggern Retry-Timer der aktuellen Fct-Id */
                poBplRamRow->u16RetryOrProcessingTimer = apoFctRomRow->nMaxRetryOrProcessingTime;

                BAP_DBG_RetryRetriggeredEvent(apoFctRomRow->lsgId);
            }
            break;
        default:
            break;
        }
    }

    return;
    /* interne Hilfsfunktion */
    /*lint +esym(613,apoFctRomRow) */
}
#endif /* #ifdef BAP_ASG */


/* Definition (Implementierung) von globalen Funktionen */

#ifdef BAP_FSG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void 
BAP_BPL_ResendStatus(BapInternalParameters_cpot apoPars)
{
    BapOpCodes_et eOpCode;
    
    /*lint -esym(613,apoPars)*/
    BAP_ASSERT(apoPars);
    
    /* Der OpCode wird temporaer fuer die Dauer des Funktionsaufrufs geaendert
     * und ansschliessend wieder auf den urspruenglichen Wert zurueckgesetzt.
     * Spart Code, Stackspeicher und Laufzeit
     */
    eOpCode = apoPars->eOpCode;
       
    /* const_cast<>, temporaeres Aendern des OpCodes */
    ((BapInternalParameters_pot)apoPars)->eOpCode = BapOp_PropStatus;  /*lint !e613*/ /* Zeiger kann nicht durch const_cast<> zu NULL werden */

    /* Heartbeat muss immer retriggert werden, da FSG */
    BAP_BPL_RetriggerHeartbeat(apoPars->poLsgRomRow);

    BAP_BCL_ResendWithOpCode(apoPars);

    /* const_cast<>, Wiederherstellen des OpCodes */
    ((BapInternalParameters_pot)apoPars)->eOpCode = eOpCode; /*lint !e613*/ /* Zeiger kann nicht durch const_cast<> zu NULL werden */
   
    return;
    /*lint +esym(613,apoPars)*/
}
#endif /* #ifdef BAP_FSG */


/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BPL_Send(BapInternalParameters_cpot apoPars)
{
    DBGVAR BapError_et  ret = BapErr_OK;
    DBGVAR bool_t bIgnoreSending = BAP_FALSE;
#if defined(BAP_USES_METHODS) || defined(BAP_ASG)
    BapBplRamRow_pot poBplRamRow = NULL;
#endif /* #if defined(BAP_USES_METHODS) || defined(BAP_ASG) */

    /*lint -esym(613,apoPars)*/
    BAP_ASSERT(apoPars);

#ifdef BAP_USES_METHODS
    if (apoPars->poFctRomRow)
    {
        poBplRamRow = apoPars->poFctRomRow->poBplRamRow;
    }
#endif /* #ifdef BAP_USES_METHODS */

#ifdef BAP_ASG
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* Darf nur im ASG passieren, sonst Stoerung des FSG Processing */
    if ((apoPars->poLsgRomRow) && (BapSG_ASG == apoPars->poLsgRomRow->eSGType)) 
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
    {
        /* Nur im ASG, pruefen, ob gerade ein Retry laeuft */
        if ( (poBplRamRow && poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive) )
        {
#ifdef BAP_USES_METHODS
            /* Nur ein Abort darf waehrend einer laufenden Ueberwachung unterbrechen */
            /* Die vorherige Ueberwachung kann von einem StartResult oder Abort sein */        
            if (   (BapFctCls_Method == apoPars->poFctRomRow->eFunctionClass) 
                && (BapOp_MethAbort == apoPars->eOpCode) )
            {
                /* Reset des Timers */
                poBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = apoPars->poFctRomRow->nMaxRetryCounter;
                poBplRamRow->fRetryOrProcessingFlags.bRequestSent = BAP_FALSE;
                poBplRamRow->u16RetryOrProcessingTimer = apoPars->poFctRomRow->nMaxRetryOrProcessingTime;

                BAP_DBG_RetryMonitoringStartedEvent(apoPars->lsgId);
            }
            else 
#endif /* BAP_USES_METHODS */
            {
                BAP_DBG_RetryBusyError(apoPars->lsgId);
                ret = BapErr_Busy;
                /* Der Request darf im Fehlerfall nicht gesendet werden */
                bIgnoreSending = BAP_TRUE;
            }
        }
        else if (apoPars->poFctRomRow && apoPars->poLsgRomRow)
        {            
            /* Pruefen, ob ein Retry ueberwacht werden muss */
            /* Bedingungen: MaxRetryTimer>0 und richtiger Request und ASG */
            /* Ein Abort ohne dass ein Retry laeuft, darf keinen Timer starten! */
            if((apoPars->poFctRomRow->nMaxRetryOrProcessingTime > 0u)
                && poBplRamRow
                && ((BapOp_PropSetGet == apoPars->eOpCode) || (BapOp_PropGet == apoPars->eOpCode))
                /* || (BapOp_ArrSetGet == apoPars->eOpCode) || (BapOp_ArrGet == apoPars->eOpCode)
                   || (BapOp_CacheSetGetAll == apoPars->eOpCode) || (BapOp_CacheGetAll == apoPars->eOpCode)
                   || (BapOp_MethStartResult == apoPars->eOpCode) - OpCodes sind alle gleich, daher kann Pruefung entfallen */
#ifdef BAP_USES_METHODS
                && !((BapFctCls_Method == apoPars->poFctRomRow->eFunctionClass) && (BapOp_MethAbort == apoPars->eOpCode))
#endif /* #ifdef BAP_USES_METHODS */
                )
            {
                apoPars->poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive = BAP_TRUE;
                poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_TRUE;
                poBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = apoPars->poFctRomRow->nMaxRetryCounter;
                poBplRamRow->fRetryOrProcessingFlags.bRequestSent = BAP_FALSE;
                poBplRamRow->u16RetryOrProcessingTimer = apoPars->poFctRomRow->nMaxRetryOrProcessingTime;

                BAP_DBG_RetryMonitoringStartedEvent(apoPars->lsgId);
            }            
        }
        else
        {
            ret = BapErr_InvalidArg;        
            /* Der Request darf im Fehlerfall nicht gesendet werden */
            bIgnoreSending = BAP_TRUE;
        }
    }
#endif /* #ifdef BAP_ASG */

        
#ifdef BAP_FSG
#if defined(BAP_ASG) && defined(BAP_FSG) 
    /* Darf nur im FSG passieren */
    if (BapSG_FSG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
    {

        if (apoPars->poFctRomRow && apoPars->poLsgRomRow)
        {
            /* Nur im FSG beim Senden den HB retriggern, bei ASG beim Empfang retriggern */
            BAP_BPL_RetriggerHeartbeat(apoPars->poLsgRomRow);

#ifdef BAP_USES_METHODS
            /* Das Senden eines Results deaktiviert Retry */
            /* Das Senden einer Processing resettet ihn wenn aktiv, wenn inaktiv wird Processing ignoriert */
            if (poBplRamRow)
            {
                switch(apoPars->eOpCode)
                {
                    case BapOp_MethProcessing:
                        /* Ueberwachung aktiv --> kein Result versendet, Processing an BCL leiten */
                        if (poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive)
                        {
                            poBplRamRow->u16RetryOrProcessingTimer = apoPars->poFctRomRow->nMaxRetryOrProcessingTime;
                        }
                        else /* Keine Ueberwachung mehr, Processing wird ignoriert und nicht an BCL geleitet */
                        {
                            bIgnoreSending = BAP_TRUE;
                        }
                        break;
                    case BapOp_MethResult:
                        /* Nach Result Ueberwachung deaktivieren */
                        poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;
                        break;
                    default:
                        /* nichts zu tun */
                        break;
                 }
             }      
#endif /* BAP_USES_METHODS */
        }
        else
        {
            ret = BapErr_InvalidArg;        
            /* Der Request darf im Fehlerfall nicht gesendet werden */
            bIgnoreSending = BAP_TRUE;
        }
    }
#endif /* #ifdef BAP_FSG */

    if (!bIgnoreSending)
    {
        BAP_BCL_Send(apoPars);
    }
    return ret;
    /*lint +esym(613,apoPars)*/    
}

#if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT) 
/*  Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_AbortSend(DBGVAR lsgId_t aLsgId
    ,DBGVAR fctId_t aFctId)
{
    BapInternalParameters_ot oPars;
    BapBplRamRow_pot poBplRamRow = NULL;
    
    oPars.lsgId = aLsgId;
    oPars.fctId = aFctId;
    oPars.eOpCode = BapOp_Invalid;
    oPars.pData = NULL;
    oPars.u16Length = 0;
    oPars.poLsgRomRow = BAP_GetLsgRomRow(aLsgId);
    oPars.poFctRomRow = BAP_GetLsgFctRomRow(oPars.poLsgRomRow, aFctId);
    
    if (oPars.poFctRomRow)
    {
        poBplRamRow = oPars.poFctRomRow->poBplRamRow;
        if (poBplRamRow) 
        {
            /* Loeschen des Sendeauftrags (durch Retry ausgeloest) */
            BAP_BCL_AbortSend(&oPars);
            /* loeschen der Retryueberwachung */
            poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;
        }
    }
    return;
}
#endif /* #if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)  */

#ifdef BAP_FSG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BPL_SendError(BapInternalParameters_cpot apoPars)
{
    DBGVAR BapError_et  ret;
    BapBplRamRow_pot poBplRamRow;
    
     /*lint -esym(613,apoPars)*/
    BAP_ASSERT(apoPars);

    if (apoPars->poFctRomRow)
    {        
        /* retriggert HeartbeatTimer */
        BAP_BCL_SendError(apoPars);
       
        BAP_BPL_RetriggerHeartbeat(apoPars->poLsgRomRow);

        /* Ein Error einer Methode beendet die Ueberwachung */
        poBplRamRow = apoPars->poFctRomRow->poBplRamRow;
        if (poBplRamRow) 
        {
            poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE; 
        }   

        ret = BapErr_OK;
    }
    else
    {
        ret = BapErr_InvalidArg;        
    }   

    return ret;
    /*lint +esym(613,apoPars)*/
}
#endif /* #ifdef BAP_FSG */


/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BPL_Init(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR BapError_et eResult;
    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

    /* Zuerst die darunterliegende Schicht anhalten */
    eResult = BAP_BCL_Init(apoLsgRomRow);
    if ( BapErr_OK == eResult)
    {
        /* Stoppen der Heartbeats */
        apoLsgRomRow->poLsgRamRow->xHeartbeatInfo.eStatus = BapBplStat_Stopped; /* Passt sowohl bei ASG und FSG */
        apoLsgRomRow->poLsgRamRow->u16HeartbeatTimer = 0;

        /* Stoppen der Retries im ASG bzw. Processing_CNF Senden im FSG */
        BAP_BPL_StopRetries(apoLsgRomRow);
        BAP_DBG_SetHeartbeatState(apoLsgRomRow->lsgId, BAP_FALSE);
    }
    return eResult;
    /*lint +esym(613,apoLsgRomRow)*/
}

#ifdef BAP_FSG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR DBGVAR BapError_et
BAP_BPL_Start(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR BapError_et eResult;

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

    /* Zuerst die darunterliegende Schicht starten */
    eResult = BAP_BCL_Start(apoLsgRomRow); 
    if (BapErr_OK == eResult) 
    {
#if defined(BAP_ASG) && defined(BAP_FSG) 
        /* Nur FSG startet HB */
        if (BapSG_FSG == apoLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
        {
            if(apoLsgRomRow->u8Heartbeat)
            {
                /*Starten des HBs*/
                BAP_BPL_InitializeHeartbeat(apoLsgRomRow, (uint8_t) 1);
                BAP_DBG_SetHeartbeatState(apoLsgRomRow->lsgId, BAP_TRUE);
                BAP_BPL_SetNextFctIndex(apoLsgRomRow); /* Fehler darf nicht vorkommen wenn BAPGen verwendet wird */
            }
        }
    }
    return eResult;
    /*lint +esym(613,apoLsgRomRow)*/
}
#endif /* #ifdef BAP_FSG */

#ifdef BAP_ASG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_TaskHeartbeatMonitoring(void)
{
    DBGVAR uint8_t u8LsgIndex = BAP_P_LSG_ROM_TABLE_ROWS;
    BapLsgRomRow_pot poLsgRomRow = NULL;
    BapLsgRamRow_pot poLsgRamRow = NULL;

    while(u8LsgIndex--)
    {
        poLsgRomRow=BAP_pLsgRomTable[u8LsgIndex];
        poLsgRamRow = poLsgRomRow->poLsgRamRow;

        if(poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running) /* In WaitForConfig gibt es kein HB Monitoring */
        {
#if defined(BAP_ASG) && defined(BAP_FSG)  
            if (BapSG_ASG == poLsgRomRow->eSGType)
            {
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */                
                if( poLsgRamRow && (BapBplStat_OK == poLsgRamRow->xHeartbeatInfo.eStatus) ) /* Timer gestartet ?*/
                {
                    if(poLsgRamRow->u16HeartbeatTimer > 1u) /* noch nicht abgelaufen */
                    {
                        poLsgRamRow->u16HeartbeatTimer--;
                    }
                    else /* HBTimer abgelaufen */
                    {  
                        /* jetzt wird der HeartbeatTimer angehalten */
                        poLsgRamRow->xHeartbeatInfo.eStatus = BapBplStat_Stopped;
    
                        /* Meldung an uebergeordnete Schicht HeartbeatTimer abgelaufen */
                        BAP_BPL_ErrorIndication(poLsgRomRow->lsgId , BAP_FCTID_HEARTBEAT , BapErr_TimeoutHeartbeat);
                        
                        BAP_DBG_HeartbeatTimeoutError(poLsgRomRow->lsgId);
                    }
                }
#if defined(BAP_ASG) && defined(BAP_FSG)  
            }
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
        }
    }
    return;
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_FSG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_TaskHeartbeatDispatcher(void)
{
    DBGVAR uint8_t u8LsgIndex = 0;
    BapInternalParameters_ot oPars;
    BapLsgRamRow_pot poLsgRamRow = NULL;
  
    oPars.eOpCode = BapOp_PropHeartbeatStatus;
    oPars.pData = NULL;
    oPars.u16Length = 0;

    while((u8LsgIndex < BAP_P_LSG_ROM_TABLE_ROWS) )
    {
        oPars.poLsgRomRow=BAP_pLsgRomTable[u8LsgIndex];

        if((oPars.poLsgRomRow->u8Heartbeat)
#if defined(BAP_ASG) && defined(BAP_FSG)  
            && (BapSG_FSG == oPars.poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */    
            && (oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running))
        {
            oPars.lsgId = oPars.poLsgRomRow->lsgId;
            poLsgRamRow = oPars.poLsgRomRow->poLsgRamRow;

            /* Interruptsperre ist hier nicht erforderlich, da beim FSG kein Retriggern im Rx-IRQ */

            if(poLsgRamRow->u16HeartbeatTimer > 1u) /* noch nicht abgelaufen */
            {
                poLsgRamRow->u16HeartbeatTimer--;
            }
            else /* HBTimer abgelaufen */
            {                           
                /* Naechstes Element im Heartbeat-Umlauf versenden */
                oPars.poFctRomRow = &oPars.poLsgRomRow->poFctRomTable[poLsgRamRow->xHeartbeatInfo.u8LastSentFctIndex];
                oPars.fctId = oPars.poFctRomRow->fctId;


                /* retriggere HeartbeatTimer */
                BAP_BPL_RetriggerHeartbeat(oPars.poLsgRomRow);

                BAP_BCL_ResendWithOpCode(&oPars);

                BAP_DBG_HeartbeatProcessedEvent(oPars.lsgId);

                BAP_BPL_SetNextFctIndex(oPars.poLsgRomRow); /*fuer naechste fct vorbereiten*/
            }
        }
        ++u8LsgIndex;
    }
    return;
}
#endif /* #ifdef BAP_FSG */


#ifdef BAP_ASG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_TaskRetryMonitoring(void)
{
    BapInternalParameters_ot oPars;
    BapLsgRamRow_pot poLsgRamRow;

    DBGVAR uint8_t u8LsgIndex;
    DBGVAR uint8_t u8RomRowIndex;

    oPars.eOpCode = BapOp_Invalid;
    oPars.pData = NULL;
    oPars.u16Length = 0;

    for(u8LsgIndex = 0; u8LsgIndex < BAP_P_LSG_ROM_TABLE_ROWS; ++u8LsgIndex)
    {
        oPars.poLsgRomRow = BAP_pLsgRomTable[u8LsgIndex];
        oPars.lsgId = oPars.poLsgRomRow->lsgId;
        poLsgRamRow = oPars.poLsgRomRow->poLsgRamRow;

        if( 
#if defined(BAP_ASG) && defined(BAP_FSG)  
            (BapSG_ASG == oPars.poLsgRomRow->eSGType) && 
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
            ((poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running) ||
                (poLsgRamRow->eLsgStatus == BapLayerLsgStat_WaitForConfig))
            && (poLsgRamRow->fState.fRetryProcessingActive)
          )    
        {
            /* Ueberwachung fuer das Lsg abschalten. Wird bei Bedarf wieder durch BAP_BPL_TaskRetryMonitoringFct() eingeschaltet */
            poLsgRamRow->fState.fRetryProcessingActive = BAP_FALSE;

            for(u8RomRowIndex = oPars.poLsgRomRow->u8FctRomTableSize; u8RomRowIndex > 0; --u8RomRowIndex)
            {
                oPars.poFctRomRow =  &oPars.poLsgRomRow->poFctRomTable[u8RomRowIndex-1];
                oPars.fctId = oPars.poFctRomRow->fctId;

                BAP_BPL_TaskRetryMonitoringFct(&oPars);
            }
        }
    }
    return;
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_ASG
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void 
BAP_BPL_ProcessHBMsg(BapLsgRomRow_pot apoLsgRomRow, DBGVAR uint8_t au8HearbeatTime)
{
    BapLsgRamRow_pot poLsgRamRow;

    /* Hilfsfunktion, wird nur beim Empfang der HB-Nachricht vom BPL selbst oder
     * beim Empfang der Statusall-Nachricht aufgerufen, d.h. alle Aufrufenden sind bekannt.
     */
    /*lint -esym(613,apoLsgRomRow) */
    BAP_ASSERT(apoLsgRomRow);

    poLsgRamRow = apoLsgRomRow->poLsgRamRow;

    if( /* HB-Zeit vom FSG == 0 bedeutet kein HB-Mechanismus */
        (au8HearbeatTime > 0)
        /* HB-Faktor im ASG == 0 bedeutet keine HB-Ueberwachung */
        && (apoLsgRomRow->u8Heartbeat))  
    {
        /*  Heartbeat eingetroffen, dann Ueberwachung starten */
        poLsgRamRow->xHeartbeatInfo.eStatus = BapBplStat_OK;
        BAP_DBG_SetHeartbeatState(apoLsgRomRow->lsgId, BAP_TRUE);
        BAP_BPL_InitializeHeartbeat(apoLsgRomRow, au8HearbeatTime);
    }
    return;
    /*lint +esym(613,apoLsgRomRow) */
}
#endif /* #ifdef BAP_ASG */


/* CallBack-Funktionen */

/*  Wird vom BCL als Callback aufgerufen. Dokumentiert in bap_bcl.h */
BAP_IMPL_FAR void
BAP_BCL_ErrorIndication(DBGVAR lsgId_t aLsgId
    , DBGVAR fctId_t aFctId
    , DBGVAR BapError_et aeErrorCode)
{
#ifdef BAP_ASG
    BapLsgRomRow_pot poLsgRomRow;
    BapFctRomRow_pot poFctRomRow;
    BapBplRamRow_pot  poBplRamRow;

    poLsgRomRow = BAP_GetLsgRomRow(aLsgId);
    poFctRomRow = BAP_GetLsgFctRomRow(poLsgRomRow, aFctId);
    poBplRamRow = poFctRomRow ? poFctRomRow->poBplRamRow : NULL;

    /*lint -esym(613, poLsgRomRow) falls BplRamRow dann existiert auch LsgRomRow! */ 
    if (poBplRamRow 
        && poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive
#if defined(BAP_ASG) && defined(BAP_FSG) 
        && (BapSG_ASG == poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
       )
    {
        switch(aeErrorCode)
        {
            /* BCL Fehlermeldungen */
            case BapErr_IllegalSequence:
            case BapErr_SequenceNumber:
            case BapErr_TimeoutSegmentation:
            case BapErr_OversizeSegmentation:
                /* Wiedereinschalten des Retry-Mechanismus nach einem 
                 * Intertelegrammzeitfehler und allen moeglichen Segmentierungsfehlern.
                 *
                 * Retry oder Retry-Timeout beim naechsten RetryDispatcher-Lauf ausloesen
                 * durch Setzen des Timers auf "abgelaufen"=1.
                 */
                poBplRamRow->u16RetryOrProcessingTimer = 1;

                BAP_DBG_RetryMonitoringStartedEvent(aLsgId);

                /* Im Retry-Fall erfolgt keine Meldung an BAL */

                break;
            default:
                /* Stoppen des Retry-timer LAH 1.0 Seite 51*/
                BAP_BPL_RetryAnswered(aLsgId, poBplRamRow);

                /* Meldung an BAL */
                BAP_BPL_ErrorIndication(aLsgId, aFctId, aeErrorCode);
                break;
        }
    }
    else
#endif /* #ifdef BAP_ASG */
    {
        /* Meldung an BAL */
        BAP_BPL_ErrorIndication(aLsgId, aFctId, aeErrorCode);
    }

    /*lint +esym(613, poLsgRomRow) */ 

    return;
}


/*  Wird vom BCL als Callback aufgerufen. Dokumentiert in bap_bcl.h */
BAP_IMPL_FAR void
BAP_BCL_DataReceived(BapInternalParameters_cpot apoPars)
{
/*
 * wenn ASG und aFctId ==4 (HB) und aeOpCode = BapOp_PropHeartbeatStatus => Ueberwachung starten
 * wenn ASG und aFctId ==2 (BAP_Config versionsNr) und aeOpCode = BapOp_PropReset  => HB aus
 */

    /* Diese Funktion wird nur vom BCL aufgerufen, apData wird nicht NULL sein */
    /*lint -esym(613,apoPars) */
    BAP_ASSERT(apoPars);

#ifdef BAP_ASG
    if ( (apoPars->eOpCode == BapOp_MessageReceived) )
    {
        if( (NULL != apoPars->poLsgRomRow)
            && (BapBplStat_Stopped != apoPars->poLsgRomRow->poLsgRamRow->xHeartbeatInfo.eStatus) 
#if defined(BAP_ASG) && defined(BAP_FSG)  
            && (BapSG_ASG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
           )
        {
            /* retriggere Heartbeat auch bei Nachrichten einer kompatiblen Erweiterung der Funktionsliste */
            BAP_BPL_RetriggerHeartbeat(apoPars->poLsgRomRow);

            /* retriggere GetAll auch bei Nachrichten einer kompatiblen Erweiterung der Funktionsliste */
            BAP_BPL_UpdateGetAllRetryOnOpCode(apoPars);
        }
    }
    else
#endif /* #ifdef BAP_ASG */

    if(apoPars->poFctRomRow)
    {
#ifdef BAP_ASG
#if defined(BAP_ASG) && defined(BAP_FSG)  
        if (BapSG_ASG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
        {
            if( (BAP_FCTID_HEARTBEAT == apoPars->fctId) 
                /* HMS, ALA: Status nicht gemaess LAH, aber damit Get HB vom BAL auch den HB einschaltet */
                && ((BapOp_PropHeartbeatStatus == apoPars->eOpCode) ||  (BapOp_PropStatus == apoPars->eOpCode)))
            {
                BAP_BPL_ProcessHBMsg(apoPars->poLsgRomRow, *apoPars->pData);
            }
            else if( (BAP_FCTID_BAPCONFIG == apoPars->fctId) && (BapOp_PropReset_Set == apoPars->eOpCode) )
            {
                /*  Heartbeat-Ueberwachung stoppen */
                apoPars->poLsgRomRow->poLsgRamRow->xHeartbeatInfo.eStatus = BapBplStat_Stopped;
                BAP_DBG_SetHeartbeatState(apoPars->lsgId, BAP_FALSE);

                /*  Nach FSG Reset auch die laufenden Retries anhalten ohne Timeout-Meldung */
                BAP_BPL_StopRetries(apoPars->poLsgRomRow);
            }
            else
            {
                /* keine reservierte Funktion die fuer den BPL Bedeutung hat */
            }

            BAP_BPL_UpdateRetryOnOpCode(apoPars->poFctRomRow, apoPars->eOpCode);
            BAP_BPL_UpdateGetAllRetryOnOpCode(apoPars);
        }
#endif /* #ifdef BAP_ASG */

#ifdef BAP_FSG
#if defined(BAP_ASG) && defined(BAP_FSG)  
        if (BapSG_FSG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
        {
            BAP_BPL_UpdateProcessingDispatching(apoPars->poFctRomRow, apoPars->eOpCode);
        }
#endif /* #ifdef BAP_FSG */
        
        /* nur externe OpCodes verarbeiten und an BAL weitergeben */
        if(apoPars->eOpCode < BapOp_CacheError)
        {
            BAP_BPL_DataReceived(apoPars);
        }
        else
        {
            /* interne OpCodes werden nur vom BPL verarbeitet */
        }
    }
    else
    {
        /*Nachricht war nicht fuer mich!*/

        /* Bit ist zwar dem BCL zugeordnet, aber der kann es selbst nicht 
           auf allen Bussystemen auswerten ohne zusaetzlichen Speicherbedarf */
        BAP_DBG_RxDataPurgedEvent();
    }
    /*lint +esym(613,apoPars) */
    return;
}


#if defined(BAP_FSG) && defined(BAP_USES_METHODS)
/* Diese Funktion ist in bap_bpl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BPL_TaskProcessingDispatcher(void)
{   
    BapBplRamRow_pot poBplRamRow;

    DBGVAR uint8_t u8LsgIndex;
    DBGVAR uint8_t u8RomTableSize;
    DBGVAR uint8_t u8RomRowIndex;
    BapInternalParameters_ot oPars;

    BAP_InitInternalParameters(&oPars);
    oPars.eOpCode = BapOp_MethProcessing;

    /* Ueber LSGs iterieren */
    for(u8LsgIndex = 0; u8LsgIndex < BAP_P_LSG_ROM_TABLE_ROWS; ++u8LsgIndex)
    {
        oPars.poLsgRomRow = BAP_pLsgRomTable[u8LsgIndex];
        oPars.lsgId = oPars.poLsgRomRow->lsgId;
        u8RomTableSize = oPars.poLsgRomRow->u8FctRomTableSize;
        if(   (oPars.poLsgRomRow->poLsgRamRow->eLsgStatus == BapLayerLsgStat_Running)  
            && (oPars.poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive)
#if defined(BAP_ASG) && defined(BAP_FSG)  
            &&(BapSG_FSG == oPars.poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG)  */
            )
        {
            /* Dispatcher fuer dieses Lsg ausschalten, wird bei Bedarf spaeter wieder eingeschaltet */
            oPars.poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive = BAP_FALSE;
            /* Ueber Fcts des LSG iterieren */
            for(u8RomRowIndex = 0; u8RomRowIndex < u8RomTableSize; ++u8RomRowIndex)
            {
                oPars.poFctRomRow = &oPars.poLsgRomRow->poFctRomTable[u8RomRowIndex];
                oPars.fctId = oPars.poFctRomRow->fctId;
                    
                poBplRamRow = oPars.poFctRomRow->poBplRamRow;
                /* Ist ein Retry konfiguriert ?, kann nur Methode sein! */
                /* RamTabelle existiert und der Retry ist aktiv, d.h. noch keine Antwort gekommen. */
                if( poBplRamRow && poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive )
                {       
                    /* Ist Timer abgelaufen? */
                    switch (poBplRamRow->u16RetryOrProcessingTimer)
                    {
                        case 0:
                            /* 0 bedeutet, dass der Retry-Timer gestoppt wurde (StartMsg empfangen),
                             * dann warte auf Antwort oder Intertelegrammzeit- oder Segmentierungsfehler 
                             * (= an dieser Stelle nichts zu tun).
                             */
                            break;
                        case 1:
                            /* 1 bedeutet, dass der Retry-Timer abgelaufen ist */
                       
                            /* Timer fuer naechsten Retry wieder auf ROMWert hochsetzen */
                            poBplRamRow->u16RetryOrProcessingTimer = oPars.poFctRomRow->nMaxRetryOrProcessingTime;

                            /* setzte konfigurierte Laenge, sonst wird die Nachricht bei der Typpruefung verworfen */
                            oPars.u16Length = oPars.poFctRomRow->u16RxSize;

                            /* Senden einer Aufforderung eine Processing Nachricht zu verschicken in FSG Applikation */
                            BAP_BPL_DataReceived(&oPars);
                            
                            break;
                        default:
                            --poBplRamRow->u16RetryOrProcessingTimer;
                            break;
                    }
                    /* Dispatcher fuer dieses Lsg wieder einschalten */
                    oPars.poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive = BAP_TRUE;
                }
            }
        }
    }
    return;
}
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_METHODS) */

#if defined(BAP_FSG) && defined(BAP_USES_METHODS)
static BAP_IMPL_FAR void
BAP_BPL_UpdateProcessingDispatching(BapFctRomRow_pot apoFctRomRow
    , DBGVAR BapOpCodes_et aeOpCode)
{
    BapBplRamRow_pot poBplRamRow;
        
    /* interne Hilfsfunktion */
    /*lint -esym(613,apoFctRomRow) */
    BAP_ASSERT(apoFctRomRow);
    
    poBplRamRow = apoFctRomRow->poBplRamRow;

    if(poBplRamRow)
    {
        switch(aeOpCode)
        {
        case BapOp_MethAbort:
            /* Retry stoppen */ 
            poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_FALSE;
            break;
        case BapOp_MethStartResult:
            /* Im ASG werden beim Absenden Timer aktiviert, hier im FSG beim Empfang */
            /* Pruefen, ob ein Retry ueberwacht werden muss */
            /* Bedingungen: MaxRetryTimer>0 und richtiger Request und ASG */
            if(apoFctRomRow->nMaxRetryOrProcessingTime > 0u)
            {
                /*lint -esym(613,poLsgRomRow) Wenn es eine FctRomRow gibt, gibt es auch immer eine poLsgRomRow*/
                BapLsgRomRow_pot poLsgRomRow = BAP_GetLsgRomRow(apoFctRomRow->lsgId);
                poLsgRomRow->poLsgRamRow->fState.fRetryProcessingActive = BAP_TRUE; /*lint !e613 During Specific Walk poLsgRomRow might be NULL */
                /*lint +esym(613,poLsgRomRow) */
                poBplRamRow->u16RetryOrProcessingTimer = apoFctRomRow->nMaxRetryOrProcessingTime;
                poBplRamRow->fRetryOrProcessingFlags.u6RetryCounter = apoFctRomRow->nMaxRetryCounter;
                poBplRamRow->fRetryOrProcessingFlags.bRetryOrProcessingActive = BAP_TRUE;
    
                BAP_DBG_RetryMonitoringStartedEvent(apoFctRomRow->lsgId);
            }
            break;
            /* Nachricht wurde komplett versendet, entweder MethProcessing oder MethResult */
            /* kein Retrigger; Aber merken, dass versendet */
        default:
            break;
        }
    }
    else
    {
        /* keine RamRow --> Kein retry */
    }
    return;
    /* interne Hilfsfunktion */
    /*lint +esym(613,apoFctRomRow) */
}
#endif  /* #if defined(BAP_FSG) && defined(BAP_USES_METHODS) */


#ifdef BAP_USES_ACKNOWLEDGE
/**
 *  Diese Funktion wird vom BCL aufgerufen, wenn eine Nachricht erfolgreich verschickt wurde.
 *  Sie wird nur nach einem BAP_BCL_Send aufgerufen, nicht jedoch nach einem BAP_BCL_Resend oder
 *  BAP_BCL_ResendWithOpcode.
 *
 *  @param apoFctRomRow zeigt auf die Funktion 
 *  @param aeOpCode enthaelt den gesendeten OpCode
 */
BAP_IMPL_FAR void
BAP_BCL_Acknowledge(BapFctRomRow_pot apoFctRomRow, BapOpCodes_et aeOpCode)
{
    BAP_BPL_Acknowledge(apoFctRomRow, aeOpCode);
}
#endif  /* #ifdef BAP_USES_ACKNOWLEDGE */
