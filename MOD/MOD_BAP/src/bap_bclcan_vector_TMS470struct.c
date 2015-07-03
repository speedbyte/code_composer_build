/******************************************************************
 *
 *     Copyright (c) 2004 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: BCL
 *
 * $Archive: /dev/BAP/integration/bap_bclcan_vector_iav.c $
 * $Revision: 32 $
 * $Modtime: 05-04-21 16:01 $
 * $Author: Andreasl $
 *
 * Ersteller:
 * Andreas Laag, Xcc Software AG, D-76137 Karlsruhe
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt die Anpassung der BCL CANUBS Implementierung
 *  an den CAN-Treiber von Vector Software.
 *
 *  Laut Auskunft von Herrn Maurice Muecke (Volkswagen) vom 2004-10-11
 *  sind die Funktionen CanGetRxDataPtr und CanGetTxDataPtr nicht
 *  teil der offiziellen Vector CAN Treiber Schnittstelle.
 *
 *  Es kann nicht garantiert werden, dass dies mit allen Treibern funktioniert.
 *
 *  Die Verwendung wird gestattet, erfordert aber eventuell
 *  Anpassungen bei anderen Versionen des Treibers.
 *
 *  Ebenso ist die direkte Verwendung der CanConfirmationMask und
 *  CanConfirmationFlags._c nicht Teil der offiziellen Schnittstelle.
 *
 *  In der Version des CAN Treibers fuer den TMS470 von TI sind diese
 *  Funktionen vorhanden. Falls diese fuer andere CPU Typen nicht
 *  vorhanden sind (Erkennung: Compilierfehler), dann muessen
 *  entsprechende Anpassungen in dieser Datei vorgenommen werden.
 *
 *  Die Makros CAN_CONFIRMATION_FLAG und CAN_CONFIRMATION_MASK
 *  kapseln den Zugriff auf die Confirmation-Flags. Es wird
 *  davon ausgegangen, dass der CAN Treiber nach dem Versenden
 *  das entsprechende Bit setzt.
 *
 *  CanGetRxDataPtr liefert zu einem CanMsgHandle einen Zeiger
 *  auf den Can-Puffer der empfangenen Nachricht.
 *
 *  CanGetTxDataPtr liefert zu einem CanMsgHandle einen Zeiger
 *  auf den Can-Puffer der zu sendenden Nachricht.
 *
 *  @todo ???
 *  Diese Implementierung unterstuetzt nicht die Verwendung von
 *  Can-Interface-Channels. Daher muss in der Xml-Konfiguration immmer 1
 *  als Can-Interface-Channel eingetragen sein, da diese Implementierung
 *  diesen Wert als Konstante beim Empfang verwendet.
 *  Ansonsten werden im BCL alle empfangenen Daten verworfen.
 *
 ******************************************************************
 *
 * Versionsgeschichte (ohne SCM)
 *
 * Datum        Version     Autor       Beschreibung
 * ----------------------------------------------------------------
 * 2005-04-21   1.3         ALA         Aufsplittung nach Prozessortyp,
 * 2004-09-09   1.2         ALA         Initiale Version
 ******************************************************************
 * 2008-07-24    1.3       Hajo Butz      Adaption for tms470 from TI   Initiale Version
 ******************************************************************/

/* Systemincludes mit <...> */
#include <stdlib.h>
#include <string.h>

/* Bibliotheken von externen Herstellen mit <...> */
#include "can_inc.h"

/* Eigene Header-Dateien "..." */
#include "bap_canubs.h"

/*
#error Dies ist nur ein Beispiel und darf nicht fuer den Serieneinsatz verwendet werden! Bus-Off/NWM muss speziell behandelt werden.
*/

#ifndef BAP_USES_CAN
#error Die mit BAPgen konfigurierte Datenfestlegung enthaelt kein CAN.
#endif

/* Externe globale Variablen-Definitionen */

/* Interne Makro-Definitionen */
#define MEMCPY(dest,src,count) (void)memcpy((void*)(dest),(void*)(src),(const size_t)(count))

/**
 * Aufgrund von Problemen bzgl. Bitfeldern mit dem Metrowerks Compiler
 * wird ueber dieses Makro direkt auf das CAN-Tx-Confirmation-Bitfeld
 * des CAN Treibers zugegriffen um festzustellen, ob das Versenden einer
 * CAN Botschaft moeglich ist bzw. erfolgreich war.
 */
#define CAN_CONFIRMATION_FLAG(CanTxHandle) \
        CanConfirmationFlags._c[CanConfirmationOffset[(CanTxHandle)]]

/**
 * Aufgrund von Problemen bzgl. Bitfeldern mit dem Metrowerks Compiler
 * wird ueber dieses Makro direkt das CAN-Tx-Confirmation-Bitfeld
 * des CAN Treibers maskiert um festzustellen, ob das Versenden einer
 * CAN Botschaft moeglich ist bzw. erfolgreich war.
 */
/* 
#define CAN_CONFIRMATION_MASK(CanTxHandle) \
        CanConfirmationMask[(CanTxHandle)]
*/        


/* Interne Typ-Definitionen */

/* Interne Const Deklarationen */

/* Interne statische Variablen */

/* Vorwaerts-Deklarationen von statischen Funktionen */

/* Definition (Implementierung) von statischen Funktionen */

/* Definition (Implementierung) von globalen Funktionen */


/*  Diese Funktion ist in bap_canubs.h dokumentiert */
BAP_FAR bool_t
BAP_CANUBS_TxData(BapCanInterfaceChannel_t aCanInterfaceChannel
    , BapCanMsgId_t aCanMsgId
    , const ptr_t apData
    , uint8_t au8Length)
{
    DBGVAR bool_t bResult;
    volatile uint8_t* pu8ConfFlag;
    DBGVAR uint8_t u8ConfMask;
    DBGVAR uint8_t u8InterruptStatus;
    DBGVAR ptr_t pCanMsgObject;

    disableint(&u8InterruptStatus);

    /* Hole Zeiger auf CAN Message Object */
    pCanMsgObject = CanGetTxDataPtr(aCanMsgId);

    /*lint -e668*//* apData ist niemals NULL */
    MEMCPY(pCanMsgObject, apData, au8Length);
    /*lint +e668*/

    pu8ConfFlag = &(CAN_CONFIRMATION_FLAG(aCanMsgId));
    u8ConfMask = CAN_CONFIRMATION_MASK(aCanMsgId);

    /* Tx Confirmation Flag loeschen, damit weitere Sendevorgaenge blockiert werden */
    *pu8ConfFlag &= ~u8ConfMask;

#ifdef C_ENABLE_VARIABLE_DLC
    if (kCanTxOk != CanTransmitVarDLC(aCanMsgId, au8Length) )
#else
    if (kCanTxOk != CanTransmit(aCanMsgId))
#endif
    {
        /* Tx Confirmation Flag wieder setzen, sonst wird das Senden fuer immer blockiert */
        *pu8ConfFlag |= u8ConfMask;

        restoreint(u8InterruptStatus);
        bResult = BAP_FALSE;
    }
    else
    {
        /* das Tx Confirmation Flag wird vom Treiber gesetzt, wenn der CAN-Controller
           per Interrupt das erfolgreiche Versenden meldet. */

        restoreint(u8InterruptStatus);
        bResult = BAP_TRUE;

        /* Fehlercodes 'kCanTxFailed' und 'kCanTxPartOffline'
           des CAN-Treibers werden nicht weiter ausgewertet.

           Im Fehlerfall wird beim naechsten Aufruf versucht die Botschaft erneut zu senden */
    }


    return bResult;
}


/*  Diese Funktion ist in bap_canubs.h dokumentiert */
BAP_FAR bool_t
BAP_CANUBS_IsReadyForTx(BapCanInterfaceChannel_t aCanInterfaceChannel
    , BapCanMsgId_t aCanMsgId)
{
    DBGVAR bool_t bResult;

    /* Beim Lesezugriff auf Confirmationflags ist die Interruptsperre nicht erforderlich */
    bResult = (CAN_CONFIRMATION_FLAG(aCanMsgId) & CAN_CONFIRMATION_MASK(aCanMsgId)) ? BAP_TRUE : BAP_FALSE;
    return bResult;
}


/*  Diese Funktion ist in bap_canubs.h dokumentiert */
BAP_FAR void
BAP_CANUBS_InitReadyForTx(BapCanInterfaceChannel_t aCanInterfaceChannel
    , BapCanMsgId_t aCanMsgId)
{
    volatile uint8_t* pu8ConfFlag;
    DBGVAR uint8_t u8InterruptStatus;

    disableint(&u8InterruptStatus);
    pu8ConfFlag = &CAN_CONFIRMATION_FLAG(aCanMsgId);
    *pu8ConfFlag |= CAN_CONFIRMATION_MASK(aCanMsgId);
    restoreint(u8InterruptStatus);
}

