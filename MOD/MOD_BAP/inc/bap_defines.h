 /******************************************************************
 *
 *     Copyright (c) 2005 Volkswagen AG, D-38436 Wolfsburg
 *
 ******************************************************************
 *
 * Projekt:    BAP (P0263)
 * Komponente: Konfiguration
 *
 ******************************************************************
 *
 * Beschreibung
 *
 *  Diese Datei enthaelt alle #defines zur Anpassung der BAP-SSW
 *  an den jeweiligen Steuergeraetetyp. Mit Hilfe von Makro-Schaltern
 *  werden einzelne Funktionalitaeten an- bzw. ausgeschaltet. Damit
 *  wird die SW optimal an die zur Verfuegung stehenden Ressourcen
 *  angepasst.
 *
 ******************************************************************
 * Generiert am:    30.11.2010
 *           um:    10:03:35
 * Quelldatei:      C:\Programme\Eclipse-CPP\eclipse\Workspace\UHV-NAR\MOD\MOD_BAP\xml\BAP-SW_UHVhigh_NAR_Phone_PB_MDI.xml
 * Zieldatei:       C:\Programme\Eclipse-CPP\eclipse\Workspace\UHV-NAR\MOD\MOD_BAP\xml\bap_defines.h
 *
 * BAPgen Version:  1.4.0
 * BAPgen Stand:    09.06.2006
 ******************************************************************/

#ifndef BAP_DEFINES_H
#define BAP_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Makro-Definitionen */

#define BAP_USES_CAN

#define BAP_SGTYPE_IS_LARGE_FSG


/**
 * Anzahl der Zeilen in der BAP_CanTxRomTable
 */
#define BAP_CAN_TX_ROM_TABLE_ROWS (uint16_t)76

/**
 * Anzahl der Zeilen in der BAP_InhibitRamTable
 */
#define BAP_INHIBIT_ROWS (uint16_t)5

/**
 * Anzahl der Zeilen in BAP_CanTxSegmentationChannels
 */
#define BAP_CAN_TX_SEGMENTATION_CHANNELS (uint16_t)5

/**
 * Anzahl der Zeilen in BAP_CanRxSegmentationChannels
 */
#define BAP_CAN_RX_SEGMENTATION_CHANNELS (uint16_t)3

/**
 * Enthaelt die Anzahl der Elemente von BAP_CanRxRingBuffer.
 */
#define BAP_CAN_RX_RING_BUFFER_SIZE (uint8_t)8

/**
 * Anzahl der Zeilen in der BAP_u16InterTelegramTimerTable
 */
#define BAP_INTER_TELEGRAM_TIMER_ROWS (uint16_t)14

/**
 *  Haelt die Anzahl an Zeilen, die die Tabelle BAP_LsgRomTables enthaelt.
 */
#define BAP_LSG_ROM_TABLE_ROWS (uint8_t)4

/**
 *  Haelt die Anzahl an Zeilen, die die Tabelle BAP_pLsgRomTable enthaelt.
 */
#define BAP_P_LSG_ROM_TABLE_ROWS (uint8_t)4

/**
 *  Diese Auflistung enthaelt alle Fehlercodes von BAP.
 *  Diese werden als Rueckgabewert bei Funktionsaufrufen zurueckgegeben
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
     * @remarks
     * Dieser Fehlercode tritt in BAP 1.4 nur dann auf, wenn nach dem Ausloesen
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
     */
     BapErr_IllegalSequence = 0x12,

     /**
     * Nicht erwartete Sequenznummer empfangen
     */
     BapErr_SequenceNumber = 0x13,

     /**
     * Folgebotschaft nicht rechtzeitig empfangen
     * Die Intertelegrammzeit wurde verletzt.
     */
     BapErr_TimeoutSegmentation = 0x14,

     /**
     * Datenelement passt nicht in BCL Rx Puffer
     */
     BapErr_OversizeSegmentation  = 0x15,

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
     * ASG Zeitueberwachungen Heartbeat - Cache wurde ungueltig gesetzt
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

     /**
     * Die max. zu erwartende Antwortzeit wurde ueberschritten, ohne dass der Request
     * abgesendet werden konnte.
     *
     * @remarks Dieser Fehler kann auch bei unsegmentierten Nachrichten auftreten.
     * @remarks Der Fehler tritt nur im ASG auf.
     */
     BapErr_RequestTimeout = 0x24,


     /* BAL Fehlermeldungen */
     /**
     * Die Protokollversion des FSG passt nicht mit der des ASG zusammen
     */
     BapErr_IncompatibleProtocolVersion = 0x32,

     /**
     * Die Datenfestlegung des FSG passt nicht mit der des ASG zusammen
     */
     BapErr_IncompatibleDataSpecification = 0x33,

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

     /**
     * Mindestens ein Sendepuffer wurde nicht initialisiert
     * @remarks:
     * Tritt nur im FSG als synchroner Fehler auf
     */
     BapErr_SendBufferNotInitialized = 0x34,

     /**
     * Diese Operation ist in diesem Zustand nicht erlaubt.
     * @remarks
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
     */
     BapErr_InvalidArg = 0x37,


     /* BAP-Stack Fehlermeldungen */

     /**
     * Die angegebene Funktion wurde fuer die uebergebenen Parameter und Konfiguration nicht implementiert.
     */
     BapErr_NotImplemented = 0x42,
    reserved = 0x00,
    no_error = 0x00,
    AppErr_Unknown = 0x80,
    AppErr_NotSuccessful = 0x81,
    AppErr_OutOfMemory = 0x82,
    AppErr_DataInvalid = 0x83,
    AppErr_NoUserActive = 0x88,
    AppErr_NoNetwork = 0x89,
    AppErr_NotSupportedByNetwork = 0x8A,
    AppErr_NotRegistered_EmergencyCallOnly = 0x8B,
    AppErr_NoCodeRequired = 0x8E,
    AppErr_NoRedialnumber = 0x8F,
    AppErr_ActiveCallPresent_Dialing = 0x90,
    AppErr_HeldCallPresent = 0x91,
    AppErr_NoCall = 0x93,
    AppErr_NoCallRelatedToCallID = 0x94,
    AppErr_NoActiveCall = 0x95,
    AppErr_NoCallOnHold = 0x96,
    AppErr_NoIncoming_WaitingCall = 0x97,
    AppErr_NoConference = 0x9A,
    AppErr_NoMemberOfConference = 0x9B,
    AppErr_MaximumNumberForMembersOfConferenceReached = 0x9C,
    AppErr_FunctionNotSupportedForConferenceCalls = 0x9D,
    AppErr_PhonebookDownloadInProgress = 0x9E,
    AppErr_NotSupportedByMobilePhone_HFP = 0xA0,
    AppErr_NoHeadsetConnected = 0xA1,
    AppErr_NoPairedHeadset = 0xA2
};

/**
 * Symbolische Namen fuer Function und LSG werden verwendet.
 */
#define BAP_USES_NAMED_IDS

/**
 * Benamte Bezeichner fuer logische Steuergeraete
 */
enum BapLsg_t
{
  BapLsg_Telephone = 0x28,
  BapLsg_Phonebook = 0x29,
  BapLsg_SD = 0x2b,
  BapLsg_MDI = 0x30
};

/**
 * Benamte Bezeichner fuer die Funktionen der verschiedenen logischen Steuergeraeten
 */
enum BapFct_t
{
  /* Steuergeraet Telephone */
  BapFct_Telephone_GetAll = 0x01,
  BapFct_Telephone_BAP_Config = 0x02,
  BapFct_Telephone_FunctionList = 0x03,
  BapFct_Telephone_HeartBeat = 0x04,
  BapFct_Telephone_FSG_Setup = 0x0e,
  BapFct_Telephone_FSG_OperationState = 0x0f,
  BapFct_Telephone_MobileServiceSupport = 0x10,
  BapFct_Telephone_UserList = 0x11,
  BapFct_Telephone_ActiveUser = 0x12,
  BapFct_Telephone_RegisterState = 0x13,
  BapFct_Telephone_LockState = 0x14,
  BapFct_Telephone_NetworkProvider = 0x16,
  BapFct_Telephone_SignalQuality = 0x17,
  BapFct_Telephone_LineState = 0x19,
  BapFct_Telephone_CallState = 0x1a,
  BapFct_Telephone_CallInfo = 0x1b,
  BapFct_Telephone_DialNumber = 0x1c,
  BapFct_Telephone_HangupCall = 0x1d,
  BapFct_Telephone_AcceptCall = 0x1e,
  BapFct_Telephone_CallHold = 0x1f,
  BapFct_Telephone_ResumeCall = 0x20,
  BapFct_Telephone_HandsFreeOnOff = 0x21,
  BapFct_Telephone_MicroMuteOnOff = 0x22,
  BapFct_Telephone_SendDTMF = 0x24,
  BapFct_Telephone_MPReleaseActiveCallAcceptWaitingCall = 0x26,
  BapFct_Telephone_MPSwap = 0x27,
  BapFct_Telephone_MPCallHoldAcceptWaitingCall = 0x28,
  BapFct_Telephone_CCJoin = 0x2b,
  BapFct_Telephone_RestoreFactorySettings = 0x30,
  /* Steuergeraet Phonebook */
  BapFct_Phonebook_GetAll = 0x01,
  BapFct_Phonebook_BAP_Config = 0x02,
  BapFct_Phonebook_FunctionList = 0x03,
  BapFct_Phonebook_HeartBeat = 0x04,
  BapFct_Phonebook_FSG_Setup = 0x0e,
  BapFct_Phonebook_FSG_OperationState = 0x0f,
  BapFct_Phonebook_MobileServiceSupport = 0x10,
  BapFct_Phonebook_MissedCallIndication = 0x11,
  BapFct_Phonebook_MissedCalls = 0x12,
  BapFct_Phonebook_ReceivedCalls = 0x13,
  BapFct_Phonebook_DialedNumbers = 0x14,
  BapFct_Phonebook_PbState = 0x16,
  BapFct_Phonebook_Phonebook = 0x17,
  BapFct_Phonebook_PbSpeller = 0x18,
  BapFct_Phonebook_VoiceMailboxNumber = 0x19,
  BapFct_Phonebook_PbStartDownload = 0x1a,
  BapFct_Phonebook_SMSState = 0x1b,
  BapFct_Phonebook_SMSReceived = 0x1d,
  BapFct_Phonebook_SMSSend = 0x20,
  /* Steuergeraet SD */
  BapFct_SD_GetAll = 0x01,
  BapFct_SD_BAP_Config = 0x02,
  BapFct_SD_FunctionList = 0x03,
  BapFct_SD_HeartBeat = 0x04,
  BapFct_SD_FSG_OperationState = 0x0f,
  BapFct_SD_ASG_Capabilities = 0x10,
  BapFct_SD_ASG_Config = 0x11,
  BapFct_SD_MFL_BlockDefinition = 0x12,
  BapFct_SD_FrameStatus = 0x13,
  BapFct_SD_FrameData = 0x14,
  BapFct_SD_FrameDataAck = 0x15,
  BapFct_SD_ScrollBar = 0x16,
  BapFct_SD_LsgStatus = 0x17,
  /* Steuergeraet MDI */
  BapFct_MDI_GetAll = 0x01,
  BapFct_MDI_BAP_Config = 0x02,
  BapFct_MDI_FunctionList = 0x03,
  BapFct_MDI_HeartBeat = 0x04,
  BapFct_MDI_FSG_Setup = 0x0e,
  BapFct_MDI_FSG_OperationState = 0x0f,
  BapFct_MDI_MDserviceSupport = 0x10,
  BapFct_MDI_MDactive = 0x11,
  BapFct_MDI_PlayStatus = 0x14,
  BapFct_MDI_MediaEvent = 0x16,
  BapFct_MDI_TimePosition = 0x17,
  BapFct_MDI_SkipForward = 0x19,
  BapFct_MDI_SkipBackward = 0x1a,
  BapFct_MDI_CurrentFileName = 0x1e,
  BapFct_MDI_CurrentFileInfo = 0x1f
};

#ifdef __cplusplus
}
#endif

#endif      /* #ifndef BAP_DEFINES_H */
