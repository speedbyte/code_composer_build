/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/bcl/bap_bcl.c $
 * $Revision: 165 $
 * $Modtime: 3.05.05 14:44 $
 * $Author: Clausr $
 *
 * Ersteller:
 * Klaus Neubert, Kopf GmbH, D-Landau/Pfalz
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Implementierung der BCL Schicht im BAP
 *  Projekt.
 *
 *  Diese C-Datei enthaelt nicht die Funktionen BAP_BCL_Received
 *  und BAP_BCL_ErrorIndication, da diese von der BPL Schicht
 *  implementiert werden.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2006-06-07   1.4         ALA         Falls ein Status verzoegert gesendet wird bleibt der Opcode erhalten, falls
 *                                      das Senden eines Heartbeats angetriggert wird.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer neue HB-Trigger.
 * 2006-04-12   1.4         ALA         Unterstuetzung fuer BAP_ROM_DATA_FAR.
 * 2005-04-29   1.3         ALA         Entfernung von Dummy-Code. Doppelte Abfrage (nun in bclcan) des Lsg-Zustands entfernt.
 * 2005-04-15   1.3         ALA         Reduzierung der CPU-Last bei Busruhe
 * 2005-04-13   1.3         CRI         ASG&FSG zusammen
 * 2005-04-08   1.3         ALA         Anpassung an neue Datenstrukturen, Optimierung Parameteruebergabe
 * 2005-03-14   1.2.1       CRI         BCL_Start hinzugefuegt
 * 2005-03-14   1.2.1       CRI         Errors & Nachrichten wenn nicht mehr an BPL
 *                                      uebergeben, wenn Zustand nicht running ist
 * 2004-11-17   1.2         ALA         Behandlung fuer Antwort auf GetAll als Einzelnachrichten oder
 *                                      Block hinzugefuegt.
 * 2004-11-04   1.2         ALA         BAP_BCL_SendError Funktion hinzugefuegt.
 * 2004-09-17   1.1.1       ALA         Out-Of-Bounds-Check in BAP_BCL_GeneralSend hinzugefuegt
 * 2004-06-07   1.1         JOK         Compilerschalter fuer SG-Typen zur Optimierung
 *                                      des ROM-Bedarfs eingefuegt
 * 2004-06-07   1.1         JOK         Die verschiedenen Send-Funktionen (BufferSet, Send, Abort,
 *                                      Resend, ResendWithOpcode) zu einer allgemeinen Funktion
 *                                      BAP_BCL_GeneralSend() zusammengefasst.
 * 2004-06-07   1.1         JOK         BAP_BCL_Start() und BAP_BCL_Shutdown() entfernt (siehe bap_bcl.h)
 ******************************************************************/


/* Systemincludes mit <...> */
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */

/* Eigene Header-Dateien "..." */
#include "bap_types.h"
#include "bap_config.h"
#include "bap_privatetypes.h"
#include "bap_bcl.h"
#include "bap_bclutil.h"
#include "bap_bclbus.h"
#include "bap_util.h"
#include "bap_debug.h"
#include "bap_balconfig.h"


/* Externe globale Variablen-Definitionen */

/* Diese Variable ist in bap_bcl.h dokumentiert */
#ifdef BAP_TASKTIME_LIMIT
uint16_t BAP_u16TaskTimeCounter;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

/* Interne Makro-Definitionen */

/* Interne Typ-Definitionen */
typedef enum BapSendType_t
{
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_Send() */
    BapSendType_Send,
#if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_AbortSend() */
    BapSendType_Abort,
#endif /* #if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT) */
#ifdef BAP_ASG
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_Resend() */
    BapSendType_Resend,
#endif /* #ifdef BAP_ASG */
#ifdef BAP_FSG
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_BufferSet() */
    BapSendType_BufferSet,
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_SendError() */
    BapSendType_SendError,
    /** Kennung fuer einen Sendeauftrag durch BAP_BCL_ResendWithOpcode() */
    BapSendType_ResendWithOpcode,
#endif /* #ifdef BAP_FSG */
    /*lint -esym(749, BapSendType_t::BapSendType_Unused) */
    /** Diese Kennung wird nicht verwendet. Dient als Abschluss fuer das Komma abhaengig vom Steuergeratetyp */
    BapSendType_Unused
} BapSendType_et;

/* Interne Const Deklarationen */

/* Interne statische Variablen */


/* Vorwaerts-Deklarationen von statischen Funktionen */


static BAP_IMPL_FAR void
BAP_BCL_SetTxRamRowFlags(BapBusIndirectionTxRomRow_pot apoBusTxRomRow
    , DBGVAR bool_t bTxRequestFlag
    );

static BAP_IMPL_FAR void
BAP_BCL_GeneralSend(BapInternalParameters_cpot apoPars
    , DBGVAR BapSendType_et aeSendType);

#ifdef BAP_USES_SEGMENTATION
static BAP_IMPL_FAR void
BAP_BCL_SingleRxNotify(BapInternalParameters_pot apoPars);
#endif /* #ifdef BAP_USES_SEGMENTATION */

#ifdef BAP_FSG
static BAP_IMPL_FAR void
BAP_BCL_SendStatusAll(BapInternalParameters_cpot apoPars);
#endif /* #ifdef BAP_FSG */

/* Definition (Implementierung) von statischen Funktionen */


/**
 *  Setzt das TxRequestFlag entsprechend des Parameters und das
 *  TxInProgressFlag immer auf 0.
 *
 *  @param  apoBusTxRomRow  entsprechende Zeile auf der die Aktion ausgefuehrt werden soll
 *  @param  bTxRequestFlag  Wert auf den das TxRequestFlag gesetzt werden soll
 */
static BAP_IMPL_FAR void
BAP_BCL_SetTxRamRowFlags(BapBusIndirectionTxRomRow_pot apoBusTxRomRow
    , DBGVAR bool_t bTxRequestFlag
    )
{
    /*lint -esym(613,apoBusTxRomRow) */

    /* Pruefe den Interfacetyp */
    if(BapIft_None != apoBusTxRomRow->eInterfaceType)
    {
#ifdef BAP_USES_SEGMENTATION
        apoBusTxRomRow->poBclTxRamRow->flags.fTxInProgressFlag = 0;
#endif /* #ifdef BAP_USES_SEGMENTATION */

        apoBusTxRomRow->poBclTxRamRow->flags.fTxRequestFlag = bTxRequestFlag;
    }
    return;

    /*lint +esym(613,apoBusTxRomRow) */
}


/**
 *  Allgemeine Funktion zum Senden von Nachrichten. Je nach Parameterwert von aeSendType
 *  werden die entsprechenden spezialisierten Funktionen ausgefuehrt.
 *
 *  @param apoPars  Struktur mit den benoetigten Parametern:<br>
 *                  -\>poLsgRomRow - Zeigt auf das den Sendeauftrag betreffende LSG oder NULL, falls kein LSG zugeordnet werden kann<br>
 *                  -\>poFctRomRow - Zeigt auf die den Sendeauftrag betreffende FCT oder NULL, falls die gesuchte FCT nicht existiert<br>
 *                  -\>pData       - Zeigt auf die  zu sendenden Daten<br>
 *                  -\>u16Length   - Enthaelt die Laenge der zu sendenden Daten<br>
 *                  -\>eOpCode     - Enthaelt den zu sendenden OpCode
 *
 *  @param aeSendType  Typ der speziellen Sendeanforderung
 */
static BAP_IMPL_FAR void
BAP_BCL_GeneralSend(BapInternalParameters_cpot apoPars
    , DBGVAR BapSendType_et aeSendType)
{
    /*lint -esym(613,apoPars) */
    DBGVAR uint8_t u8TxTableIdx;
    BapFctRomRow_pot poFctRomRow = apoPars->poFctRomRow;
    DBGVAR bool_t bFlag = BAP_TRUE;
    DBGVAR uint16_t u16Length = apoPars->u16Length;

#ifdef BAP_TASKTIME_LIMIT
    apoPars->poLsgRomRow->poLsgRamRow->fState.fTxFlag = BAP_TRUE;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

    for (u8TxTableIdx=0; poFctRomRow && (u8TxTableIdx < poFctRomRow->u8TxTableSize); u8TxTableIdx++)
    {
        BapBusIndirectionTxRomRow_pot  const poBusTxRomRow
            = &poFctRomRow->poBusTxTable[u8TxTableIdx];
        BapBclTxRamRow_pot  const poBclTxRamRow
            = poBusTxRomRow->poBclTxRamRow;

        if(BapIft_None != poBusTxRomRow->eInterfaceType)
        {
            switch(aeSendType)
            {
/* Im folgenden keine Fallunterscheidungen fuer das Kombigeraet noetig */
/* da Types nur in bestimmten SGs auftreten koennen, @see BapSendType_t */
#if defined(BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)
                case BapSendType_Abort:
                    BAP_BCL_SetTxRamRowFlags(poBusTxRomRow, BAP_FALSE);

#ifdef BAP_USES_ACKNOWLEDGE
                    poBclTxRamRow->flags.fTxRequestNeedsAckFlag = BAP_FALSE;
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */

                    break;
#endif /* #if defined(BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT) */
#ifdef BAP_FSG
                case BapSendType_BufferSet:
#endif /* #ifdef BAP_FSG */
                case BapSendType_Send:
                    /* Opcode in Buffer schreiben */
                    poBclTxRamRow->eOpCode = apoPars->eOpCode;

                    /* Wert ist unzulaessig ? */
                    if (u16Length > poBusTxRomRow->u16BufferSize)
                    {
                        /* Was zuviel ist abschneiden */
                        u16Length = poBusTxRomRow->u16BufferSize;
                    }

                    /* Setze Datengroesse des Sendepuffers und kopiere die Daten */
                    poBclTxRamRow->oBufferWithLength.u16Length = u16Length;
                    MEMCPY(poBclTxRamRow->oBufferWithLength.aru8Buffer, apoPars->pData, u16Length);

#ifdef BAP_FSG
#if defined(BAP_ASG) && defined(BAP_FSG)
                    if (BapSG_FSG == apoPars->poLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
                    {
                        poBclTxRamRow->flags.fTxInitializedFlag = BAP_TRUE;
                        /* Setze Tx Flags bei Send und loesche Sie bei BufferSet */
                        bFlag = (const bool_t) ((BapSendType_BufferSet != aeSendType) ? BAP_TRUE : BAP_FALSE);
                    }
#endif /* #ifdef BAP_FSG */
                    BAP_BCL_SetTxRamRowFlags(poBusTxRomRow, bFlag);

#ifdef BAP_USES_ACKNOWLEDGE
                    poBclTxRamRow->flags.fTxRequestNeedsAckFlag = bFlag;
#endif /* #ifdef BAP_USES_ACKNOWLEDGE */
                    break;
#ifdef BAP_FSG
                case BapSendType_SendError:
                    /* Fehlerwert in Spezialpuffer speichern */
                    poBclTxRamRow->eErrorCode = (BapError_et) *apoPars->pData;
                    /* TX Flag setzen, damit Fehler ausgesendet wird */
                    poBclTxRamRow->flags.fTxErrorFlag = BAP_TRUE;

#if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE)
                    poBclTxRamRow->flags.fTxErrorNeedsAckFlag = BAP_TRUE;
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE) */

                    /* Durch das Loeschen des Progress-Flags wird zuerst der Fehler gesendet (Prioritaet).
                       Falls zuvor eine Wertuebertragung lief, wird diese nach dem Aussenden des Fehlers neu begonnen.
                       Somit geht der Werte-Request nicht verloren, aber der Fehler wird zuerst gemeldet.

                       Ein Aussenden des Fehlers bei eingeschalteter Segmentierung zwischendurch haette sonst die Wirkung,
                       dass die Start-Botschaft (des Fehlers) dazwischen im ASG zu Sequenzfehlern fuehren wuerde,
                       da keine weiteren Segmente mehr erwartet wuerden.
                    */
#if defined(BAP_FSG) && defined(BAP_USES_SEGMENTATION)
                    if (poBclTxRamRow->flags.fTxInProgressFlag)
                    {
                        poBclTxRamRow->flags.fTxRequestFlag = BAP_TRUE;
                        poBclTxRamRow->flags.fTxInProgressFlag = BAP_FALSE;
                    }
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_SEGMENTATION) */

                    break;
                case BapSendType_ResendWithOpcode:
#if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE)
                    /* Fuehre kein ResendWithOpcode durch, falls gewoehnliches Senden noch nicht erfolgt */
                    if( !(poBclTxRamRow->flags.fTxRequestNeedsAckFlag) )
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE) */
                    {
#if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE)
                        poBclTxRamRow->flags.fTxRequestNeedsAckFlag = BAP_FALSE;
#endif /* #if defined(BAP_FSG) && defined(BAP_USES_ACKNOWLEDGE) */

                        /*  Aendere den OpCode ab */
                        poBclTxRamRow->eOpCode = apoPars->eOpCode;

                        BAP_BCL_SetTxRamRowFlags(poBusTxRomRow, BAP_TRUE);
                    }
                    break;
#endif /* #ifdef BAP_FSG */
#ifdef BAP_ASG
                case BapSendType_Resend:
                    BAP_BCL_SetTxRamRowFlags(poBusTxRomRow, BAP_TRUE);
                    /* Falls noch ein Acknowledge aussteht, nichts dran aendern */
                    break;
#endif /* #ifdef BAP_ASG */
                default:
                    break;
            }
        }
    }   /* for (u8TxTableIdx... */
    return;
    /*lint +esym(613,apoPars) */
}


#ifdef BAP_USES_SEGMENTATION
/**
 *  Diese Funktion prueft, ob ein Datenelement oder ein Fehlerwert empfangen wurde
 *  und ruft ggfs. die entsprechende Callback-Funktion auf.
 *
 *  @see BAP_BCL_ErrorIndication, BAP_BCL_DataReceived
 *
 *  @param apoPars  benoetigt die Parameter lsgId und fctId sowie die Zeiger auf die entsprechenden ROM-Zeilen.
 *
 *  @remarks: Diese Funktion veraendert apoPars->eOpCode, apoPars->pData und apoPars->u16Length
 */
static BAP_IMPL_FAR void
BAP_BCL_SingleRxNotify(BapInternalParameters_pot apoPars)
{
    /*lint -esym(613,apoPars) */
    DBGVAR uint8_t u8BusRomRowIdx;
    DBGVAR uint8_t u8BusTableSize = apoPars->poFctRomRow->u8RxTableSize;
    BapBusIndirectionRxRomRow_pot poBusRxTable = apoPars->poFctRomRow->poBusRxTable;


    /* Durchsuche alle Empfangsmoeglichkeiten fuer die jeweilige FunctionId */
    for(u8BusRomRowIdx = 0; u8BusRomRowIdx < u8BusTableSize; u8BusRomRowIdx++)
    {
        BapBclRxRamRow_pot  const poBclRxRamRow = poBusRxTable[u8BusRomRowIdx].poBclRxRamRow;

        /* wenn kein Rx-Puffer vorhanden, dann wurde der Empfang direkt gemeldet, z.B. in BAP_CAN_RxUnsegmented */
        if (NULL != poBclRxRamRow)
        {
            /* Wurde ein Fehler empfangen? */
            if(poBclRxRamRow->flags.fRxErrorFlag)
            {
#ifdef BAP_TASKTIME_LIMIT
                BAP_u16TaskTimeCounter += BAP_IND_RX_TIME + sizeof(BapError_et);
                apoPars->poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_TRUE;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

                /* BPL benachrichten */
                BAP_BCL_ErrorIndication(apoPars->lsgId
                    , apoPars->fctId
                    , poBclRxRamRow->eErrorCode);

                /* Loesche RxError Flag */
                poBclRxRamRow->flags.fRxErrorFlag = BAP_FALSE;
            }

            /* Wurde das Datenelement vollstaendig empfangen? */
            if(poBclRxRamRow->flags.fRxCompleteFlag)
            {
                apoPars->eOpCode   = poBclRxRamRow->eOpCode;
                apoPars->pData     = (ptr_t) poBclRxRamRow->oBufferWithLength.aru8Buffer;
                apoPars->u16Length = poBclRxRamRow->oBufferWithLength.u16Length;

#ifdef BAP_TASKTIME_LIMIT
                BAP_u16TaskTimeCounter += BAP_IND_RX_TIME + apoPars->u16Length;
                apoPars->poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_TRUE;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

                /* BPL benachrichtigen */
                BAP_BCL_DataReceived(apoPars);

                /* Loesche RxComplete Flag */
                poBclRxRamRow->flags.fRxCompleteFlag = BAP_FALSE;
            }
        }
    }


    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #ifdef BAP_USES_SEGMENTATION */

#ifdef BAP_FSG
/**
 *  Diese Funktion behandelt eine GetAll Anfrage und sendet entwender einzelne Heartbeat-Statusnachrichten
 *  oder einen StatusAll-Block als Antwort auf die GetAll Anfrage.
 *
 *  @param apoPars Struktur mit den benoetigten Parametern:<br>
 *                 -\>poLsgRomRow - Zeigt auf das den Sendeauftrag betreffende LSG oder NULL, falls kein LSG zugeordnet werden kann <br>
 *                 -\>poFctRomRow - Zeigt auf die den Sendeauftrag betreffende FCT oder NULL, falls die gesuchte FCT nicht existiert
 *
 *  @remarks
 *  Bei Small FSG wird immer mit einzelnen Heartbeat Status Nachrichten geantwortet.
 *  Bei Large FSG wird nur dann mit einzelnen Heartbeat Status Nachrichten geantwortet, wenn kein Sendepuffer vorgesehen ist.
 */
static BAP_IMPL_FAR void
BAP_BCL_SendStatusAll(BapInternalParameters_cpot apoPars)
{
    DBGVAR uint8_t u8FctIdMask = 0x20;  /* FctId 0 (reserviert) und 1 (Cache) brauchen nicht ausgewertet zu werden */
    BapInternalParameters_ot oSingleHBStatPars =
    {   (lsgId_t)0       /* lsgId - wird spaeter auf apoPars->lsgId gesetzt. */
        ,(fctId_t)0     /* fctId - wird in Schleife gesetzt */
        , NULL  /* poLsgRomRow - wird spaeter auf apoPars->poLsgRomRow gesetzt */
        , NULL  /* poFctRomRow - wird in Schleife gesetzt */
        , NULL  /* pData */
        , 0     /* u16Length */
        , BapOp_PropHeartbeatStatus /* eOpCode */
    };
    DBGVAR uint8_t u8FctListSeg;
#ifdef BAP_USES_STATUS_ALL
    BapBufferWithLength_pot poGetAllBufferWithLength = NULL;
#endif /* BAP_USES_STATUS_ALL */

    /*lint -esym(613, apoPars) */
    BAP_ASSERT(apoPars != NULL );

#ifdef BAP_USES_STATUS_ALL
    if (NULL != apoPars->poFctRomRow->poBusTxTable)
    {
        poGetAllBufferWithLength = &apoPars->poFctRomRow->poBusTxTable->poBclTxRamRow->oBufferWithLength;

        /* Versenden des StatusAll-Blocks beim naechsten BAP_Task Aufruf, OpCode and Flags werden hiermit initialisiert. */
        BAP_BCL_GeneralSend(apoPars, BapSendType_Send);
    }
    else
    {
        /* Nutze Einzelbeantwortung */
    }
#endif /* BAP_USES_STATUS_ALL */

    oSingleHBStatPars.lsgId = apoPars->lsgId;
    oSingleHBStatPars.poLsgRomRow = apoPars->poLsgRomRow;
    u8FctListSeg = apoPars->poLsgRomRow->poLsgRamRow->BAP_aru8FunctionList[0];

    /* Suche alle Properties, in denen ein Bit in der FunctionList gesetzt ist */
    for (oSingleHBStatPars.fctId = BAP_FCTID_BAPCONFIG; BINARY_ID(oSingleHBStatPars.fctId) < 64; oSingleHBStatPars.fctId++) /*lint !e650 enums haben mindestens 8 bit */
    {
        /* Wechsel zum naechstes Byte (falls FctId % 8 == 0)? */
        if (!(BINARY_ID(oSingleHBStatPars.fctId) & 0x07))
        {
            u8FctIdMask = 0x80;

            /* Hole dieses Element aus dem Array mit Index (FctId / 8) */
            u8FctListSeg = oSingleHBStatPars.poLsgRomRow->poLsgRamRow->BAP_aru8FunctionList[BINARY_ID(oSingleHBStatPars.fctId)>>3];
        }

        /* Ist Bit gesetzt? */
        if(u8FctListSeg & u8FctIdMask)
        {
            oSingleHBStatPars.poFctRomRow = BAP_GetLsgFctRomRow(oSingleHBStatPars.poLsgRomRow, oSingleHBStatPars.fctId);

            if ( BapFctCls_Property != oSingleHBStatPars.poFctRomRow->eFunctionClass )
            {
                /* Nur Properties werden bei StatusAll versendet */
            }
            else
#ifdef BAP_USES_STATUS_ALL
            if (NULL != poGetAllBufferWithLength)
            {
                /* ja, dann fuege diese Nachricht zum Sendepuffer hinzu, falls einer konfiguiert ist */

                /* Hole Nachricht, die hinzugefuegt werden muss */
                BapBufferWithLength_pot poCurrentBufferWithLength = &oSingleHBStatPars.poFctRomRow->poBusTxTable->poBclTxRamRow->oBufferWithLength;
                oSingleHBStatPars.pData = (ptr_t) poCurrentBufferWithLength->aru8Buffer;

                /* Laenge voranstellen, falls erforderlich */
                if ( oSingleHBStatPars.poFctRomRow->poBusTxTable->bDataTypeSupportsVarLength)
                {
                    oSingleHBStatPars.u16Length = poCurrentBufferWithLength->u16Length;

                    /* Untere 8 Bit der Laenge setzen */
                    poGetAllBufferWithLength->aru8Buffer[poGetAllBufferWithLength->u16Length] = (uint8_t) (oSingleHBStatPars.u16Length);

                    /* Obere 8 Bit der Laenge setzen */
                    poGetAllBufferWithLength->aru8Buffer[poGetAllBufferWithLength->u16Length+1u] = (uint8_t) (oSingleHBStatPars.u16Length >> 8);

                    poGetAllBufferWithLength->u16Length += 2u;

                    /* Daten kopieren */
                    MEMCPY( &poGetAllBufferWithLength->aru8Buffer[poGetAllBufferWithLength->u16Length]
                        , oSingleHBStatPars.pData
                        , oSingleHBStatPars.u16Length );

                    /* Laenge der aktuellen Nachricht zur Gesamtnachricht hinzufuegen */
                    poGetAllBufferWithLength->u16Length += oSingleHBStatPars.u16Length;
                }
                else
                { /* bei fixer Laenge steht diese im ROM */
                    oSingleHBStatPars.u16Length = oSingleHBStatPars.poFctRomRow->poBusTxTable->u16BufferSize;

                    /* Daten kopieren */
                    MEMCPY( &poGetAllBufferWithLength->aru8Buffer[poGetAllBufferWithLength->u16Length]
                        , oSingleHBStatPars.pData
                        , oSingleHBStatPars.u16Length );

                    /* Laenge der aktuellen Nachricht zur Gesamtnachricht hinzufuegen */
                    poGetAllBufferWithLength->u16Length += oSingleHBStatPars.u16Length;
                }
            }
            else /* falls kein Sendepuffer fuer GetAll, dann nutze Einzelbeantwortung */
#endif /* BAP_USES_STATUS_ALL */
            {
                /* ja, dann ResendWithOpCode, Aufruf von GeneralSend vermeidet Rekursion */
                BAP_BCL_GeneralSend(&oSingleHBStatPars, BapSendType_ResendWithOpcode);
            }
        }
        else
        {
            /* Diese Funktion nicht versenden */
        }
        u8FctIdMask /= (uint8_t) 2u;
    }

    /*lint +esym(613, apoPars) */
}
#endif /* #ifdef BAP_FSG */


/* Definition (Implementierung) von globalen Funktionen */

#if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_AbortSend(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_BCL_GeneralSend(apoPars, BapSendType_Abort);
    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #if defined (BAP_ASG) || defined(WIRD_NIE_GEBRAUCHT)  */


#ifdef BAP_FSG
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_SendError(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_BCL_GeneralSend(apoPars, BapSendType_SendError);
    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #ifdef BAP_FSG */


/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_Send(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_BCL_GeneralSend(apoPars, BapSendType_Send);
    return;
    /*lint +esym(613,apoPars) */
}


#ifdef BAP_ASG
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_Resend(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_BCL_GeneralSend(apoPars, BapSendType_Resend);
    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #ifdef BAP_ASG */


#ifdef BAP_FSG
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_ResendWithOpCode(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    if (BAP_FCTID_GETALL == apoPars->fctId)
    {
        /* Sonderbehandlung fuer Funktionsklasse Cache */
        BAP_BCL_SendStatusAll(apoPars);
    }
    else
    {
        BAP_BCL_GeneralSend(apoPars, BapSendType_ResendWithOpcode);
    }
    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #ifdef BAP_FSG */


#ifdef BAP_FSG
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_BufferSet(BapInternalParameters_cpot apoPars)
{
    /*lint -esym(613,apoPars) */
    BAP_BCL_GeneralSend(apoPars, BapSendType_BufferSet);
    return;
    /*lint +esym(613,apoPars) */
}
#endif /* #ifdef BAP_FSG */


/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR BapError_et
BAP_BCL_Init(BapLsgRomRow_pot apoLsgRomRow)
{
    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

    /* Initialisierung aller dem LSG gehoerenden BCL RAM Bereiche */
    BAP_BCL_InitLsgRamAreas(apoLsgRomRow);
    /* Initialisierung des Vector CAN Treibers fuer den BAP Tx Pfad */
    BAP_BCL_InitCanDrvTxConfirmationFlags(apoLsgRomRow);

    return BapErr_OK;
    /*lint +esym(613,apoLsgRomRow)*/
}

#ifdef BAP_FSG
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR BapError_et
BAP_BCL_Start(BapLsgRomRow_pot apoLsgRomRow)
{
    DBGVAR uint8_t u8FctRomRowIdx;
    DBGVAR uint8_t u8BusTxIdx;
    DBGVAR BapError_et eResult = BapErr_OK;

    /*lint -esym(613,apoLsgRomRow)*/
    BAP_ASSERT(apoLsgRomRow);

#if defined(BAP_ASG) && defined(BAP_FSG)
    if (BapSG_FSG == apoLsgRomRow->eSGType)
#endif /* defined(BAP_ASG) && defined(BAP_FSG) */
    {
        /* Teste, ob alle Sendepuffer initialisiert sind */
        for (u8FctRomRowIdx=0; (u8FctRomRowIdx<apoLsgRomRow->u8FctRomTableSize) && (BapErr_OK == eResult); u8FctRomRowIdx++)
        {
            BapFctRomRow_pot poFctRomRow = &apoLsgRomRow->poFctRomTable[u8FctRomRowIdx];
            DBGVAR uint8_t * const pu8FctList = apoLsgRomRow->poLsgRamRow->BAP_aru8FunctionList;

            for(u8BusTxIdx=0; u8BusTxIdx<poFctRomRow->u8TxTableSize; u8BusTxIdx++)
            {
                BapBusIndirectionTxRomRow_pot  const poBusTxRow = &poFctRomRow->poBusTxTable[u8BusTxIdx];

                if (/* Nur falls Werte versendet werden koennen ist eine Initialisierung erforderlich */
                        poBusTxRow->u16BufferSize
                    /* Nur Properties muessen vorbelegt werden */
                    && (BapFctCls_Property == poFctRomRow->eFunctionClass)
                    /* Wert nicht initialisiert? */
                    && !poBusTxRow->poBclTxRamRow->flags.fTxInitializedFlag
                    /* Funktion muss in Funktionsliste sein */
                    && (0 != (pu8FctList[BINARY_ID(poFctRomRow->fctId) / 8] & (0x80 >> (BINARY_ID(poFctRomRow->fctId) % 8))))
                   )
                {
                    eResult = BapErr_SendBufferNotInitialized;
                }
            }
        }
    }

    return eResult;
    /*lint +esym(613,apoLsgRomRow)*/
}
#endif /* #ifdef BAP_FSG */


/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_TaskSend(void)
{
    /* Verwalte die Inhibit Timer */
    BAP_BCL_InhibitTimerTask();

    /* Aufruf CAN Sende Task */
    BAP_BCL_CanSendTask();

    return;
}


/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_ReadRingbuffer(void)
{
    /* Hole die Daten aus den Ringpuffern ab */
    BAP_BCL_CanReceiveTask();

    return;
}


#ifdef BAP_USES_SEGMENTATION
/*  Diese Funktion ist in bap_bcl.h dokumentiert */
BAP_IMPL_FAR void
BAP_BCL_TaskRxNotify(void)
{
    DBGVAR uint8_t u8LsgRomRowIdx;

    BapInternalParameters_ot oPars;
    oPars.eOpCode = BapOp_Invalid;
    oPars.pData = NULL;
    oPars.u16Length = 0;

    /* Verwalte die InterTelegram Timer */
    BAP_BCL_InterTelegramTimerTask();

    /* Fuer alle Lsgs: Durchsuche, ob in deren BCL Puffern vollstaendig
     * empfangene Datenelemente oder Fehlerwerte enthalten sind
     */
    for ( u8LsgRomRowIdx = 0; u8LsgRomRowIdx < BAP_P_LSG_ROM_TABLE_ROWS; u8LsgRomRowIdx++)
    {
        DBGVAR uint8_t u8FctRomRowIdx;
        DBGVAR uint8_t u8FctRomTableSize;
        BapFctRomRow_pot poFctRomTable;

        oPars.poLsgRomRow = BAP_pLsgRomTable[u8LsgRomRowIdx];
        oPars.lsgId = oPars.poLsgRomRow->lsgId;
        u8FctRomTableSize = oPars.poLsgRomRow->u8FctRomTableSize;
        poFctRomTable = oPars.poLsgRomRow->poFctRomTable;


#ifdef BAP_TASKTIME_LIMIT
        if ( (oPars.poLsgRomRow->poLsgRamRow->fState.fRxFlag)
            && (BAP_u16TaskTimeCounter<BAP_MAX_TASK_TIME)
           )
#endif /* #ifdef BAP_TASKTIME_LIMIT */
        {
#ifdef BAP_TASKTIME_LIMIT
            oPars.poLsgRomRow->poLsgRamRow->fState.fRxFlag = BAP_FALSE;
#endif /* #ifdef BAP_TASKTIME_LIMIT */

            for (u8FctRomRowIdx = 0;
#ifdef BAP_TASKTIME_LIMIT
                 (BAP_u16TaskTimeCounter<BAP_MAX_TASK_TIME) &&
#endif /* #ifdef BAP_TASKTIME_LIMIT */
                 (u8FctRomRowIdx < u8FctRomTableSize);
                 u8FctRomRowIdx++)
            {
                oPars.poFctRomRow = &poFctRomTable[u8FctRomRowIdx];
                oPars.fctId = oPars.poFctRomRow->fctId;

                BAP_BCL_SingleRxNotify(&oPars);
            }
        }
    }
#ifdef BAP_TASKTIME_LIMIT
    if (BAP_u16TaskTimeCounter>=BAP_MAX_TASK_TIME)
    {
        BAP_DBG_TaskTimeLimitEvent();
    }
#endif /* #ifdef BAP_TASKTIME_LIMIT */
    return;
}
#endif /* #ifdef BAP_USES_SEGMENTATION */
