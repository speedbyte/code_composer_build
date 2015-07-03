/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| P R O J E C T    I N F O R M A T I O N                                       |
+------------------------------------------------------------------------------+

Diese Datei beinhaltet die wichtigsten Informationen zum ausgelieferten Software-
stand. 


+------------------------------------------------------------------------------+
| C O M P I L E R    I N F O R M A T I O N                                     |
+------------------------------------------------------------------------------+
Der ADLATUS(R) wurde mit folgendem Compiler �bersetzt:

Code Composer Studio V3.3.0 / Code Generation Tools V2.71

Nur mit dem hier spezifizierten Compiler und den gew�hlten Compiler- und Linker-
einstellungen garantiert SMART die gew�nschte Funktionalit�t.


+------------------------------------------------------------------------------+
| P R O J E C T    I N F O R M A T I O N                                       |
+------------------------------------------------------------------------------+
Die vorliegende Software Version beinhaltet den ADLATUS(R) f�r den Texas 
Instruments TMS470R1 (Derivat: TMS470R1VF55BB) mit ISO-TP und 'Unified diagnostic
services' (UDS).
interne Versionsnummer: V1.00
Refenz Implementierung: Audi_TMS470_UDS_V107

Die Versionnummer der SW, die �ber den 0x22-Service abgefragt werden kann,
wird nicht von SMART gepflegt, sondern muss projektspezifisch angepasst werden.
Dies erfolgt mit Hilfe der Konastanten "c_CCcusint_AdlatusVersion_AUB" in der 
Datei "ADLATUS_CusInterfaceAudi.c".


+------------------------------------------------------------------------------+
| P R O J E C T    S T R U C T U R E                                           |
+------------------------------------------------------------------------------+
 
 | 
 | 
 +-- 01_adlatus      --> ADLATUS(R), paramterierbare Projektspezifika
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files 
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Hex-Files, Map-Files
 |
 |
 +-- 03_fsw          --> Referenz-Fahrsoftware
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Hex-Files, Map-Files
 | 
 |
 +-- 05_header       --> Header-Dateien
 |    |
 |    +-- ext        --> Headerdateien, die in kofigurierbaren Modulen 
 |    |                  benoetigt werden
 |    |
 |    +-- int        --> adlatusinterne Headerdateien
 | 
 | 
 +-- 06_hallib       --> prozessorspezifische Module
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Libraries
 | 
 | 
 +-- 07_chcklib      --> Module der Checksummenberechnung
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Libraries
 | 
 | 
 +-- 08_corelib      --> projektunabhaengige ADLATUS-Module
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Libraries
 | 
 | 
 +-- 09_audilib      --> kundenabhaengige Module
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Libraries
 | 
 | 
 +-- 10_his          --> Module fuer den HIS-Flash-Container
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Hex-Files, Map-Files
 |
 |
 +-- 11_parrot       --> Module fuer den Parrot Download Treiber
 |    |
 |    +-- 01_prj     --> Projektumgebung
 |    |
 |    +-- 02_src     --> Quelldateien
 |    |
 |    +-- 03_obj     --> erzeugte Object-Files
 |    |
 |    +-- 04_lst     --> erzeugte Listings
 |    |
 |    +-- 05_out     --> erzeugte Hex-Files, Map-Files
 | 
 | 
 +-- 12_odx          --> Downloadfiles, ODX/SOX Container inkl. Batches zur Erstellung


+------------------------------------------------------------------------------+
| N O T E :   P R O J E C T   D E P E N D E N C I E S                          |
+------------------------------------------------------------------------------+
Alle Verzeichnissse und Pfadangaben beziehen sich auf den Laufwerksbuchstaben R:
Mit "Subst R: ." l�sst sich der Pfad anpassen.


+------------------------------------------------------------------------------+
| N O T E :   R E F E R E N C E   A P P L I C A T I O N   S O F T W A R E      |
+------------------------------------------------------------------------------+
Die Referenz-Fahrsoftware ist ausschliesslich zu dem Zweck erstellt worden, 
den Flashvorgang verifizieren zu koennen und den Abstieg bzw. Aufstieg zwischen
Applikation und ADLATUS testen zu koennen. Fuer diese Zwecke steht auch ein
ODX-Container zur Verfuegung, der die Referenz-Fahrsoftware, den HIS-Container 
und einen Bedatungsblock enthaelt.


+------------------------------------------------------------------------------+
| N O T E :   Data length EEPROM data and response                             |
+------------------------------------------------------------------------------+
Pro Block gibt es einen L�schz�hler und einen Z�hler f�r erfolgreiche 
Programmierung. 
Die Z�hler sind 2 Byte gro� und werden inkrementiert bevor der entsprechende 
Block gel�scht wird bzw. wenn nach erfolreicher Checksummenpr�fung der Block als
g�ltig gekennzeichnet wird. Au�erdem gibt es pro Block einen 
2 Byte gro�en Sperrwert. Dieser begrenzt die Anzahl der L�schvorg�nge. Der 
maximale Sperrwert (maximale Anzahl der L�schvorg�nge) liegt damit bei 0xFFFF
pro Block. Die L�schz�hler und die Sperrwerte werden beide im EEPROM abgelegt.


+------------------------------------------------------------------------------+
| N O T E :   New customer interface file                                      |
+------------------------------------------------------------------------------+
Die Datei "ADLATUS_CheckDependAudi.c" dient als Interface zu kundenspezifischen
Bestimmung der Systemkonsistenz und der Abh�ngigkeiten der SW-Teile. Das als 
Beispiel verwendete Programm durchsucht das System auf Vollst�ndigkeit d.h alle
Bl�cke m�ssen vorhanden und g�ltig sein.


+------------------------------------------------------------------------------+
| N O T E :   communication flags - fbl --- asw                                |
+------------------------------------------------------------------------------+
Beim Programmieren dienen zwei Flags im EEPROM zum Austausch von Information 
zwischen Bootloader und Applikation. Das erste Flag "..ECUProgramInfo" enth�lt
das Konsistenz-Flag, wird vor dem L�schen auf Inkonsistenz gesetzt und muss von
der Applikation nach erfolgreicher �berpr�fung des Systems auf Konsistenz 
zur�ckgesetzt werden. Das Flag kann �ber "0x22 0xF1 0xDF" abgeftagt werden.

#define d_CCkwpaudi_AppInconsistent           (UBYTE)     0x04u
#define d_CCkwpaudi_AppConsistent             (UBYTE)     0x00u


Das zweite Flag "..SysProgrammed" wird nach erfolgreicher �berpr�fung des 
Systems (Check dependencies) vom Bootloader gesetzt. Dieses Flag dient der 
Applikation zur Erkennung einer Neuprogrammierung und muss von der Applikation
nach erfolgreicher �berpr�fung des Systems zur�ckgesetzt werden.

#define d_CCkwpaudi_AppProgrammed             (UBYTE)     0x02u


+------------------------------------------------------------------------------+
| N O T E :   NVM -- EEPROM Initialisation                                     |
+------------------------------------------------------------------------------+
F�r einen vollst�ndigen Programmierzyklus muss das EEPROM g�ltige Daten 
enthalten. In der Datei "ADLATUS_NvmHandler.c" gibt es den define "NVMINIT" mit 
dem eine Grundinitalisierung des EEPROMS erreicht werden kann. 


+------------------------------------------------------------------------------+
| N O T E :   WATCHDOG Test                                                    |
+------------------------------------------------------------------------------+
In der Datei "ADLATUS_ExternalWatchdog.c" gibt es ein define "TOGGLEPIN"
mit welchem die Bedienung des Watchdogs simuliert bzw. gemessen werden
kann, indem ein Pin des Controllers im 1ms Rythmus gesetzt/zur�ckgesetzt wird.
Dieser Pin wird in der Datei "ADLATUS_Periphery.c" als Output konfiguriert.
In der Auslieferversion ist der TOGGLEPIN inaktiv und der Pin als Output 
konfiguriert. Bei der Verwendung im Steuerger�t muss diese DebugFunktionalit�t 
entfernt werden.

Der Testpin ist innerhalb des Modules ADLATUS_HalRAM.c aktiv.

Als Togglepin dient Pin36 (GIOA[4]/INT4).


+------------------------------------------------------------------------------+
| N O T E :   WATCHDOG                                                         |
+------------------------------------------------------------------------------+
Der externe Watchdog wird alle n ms �1 ms getriggert. 'n' l��t sich �ber 
'd_CCwtdog_InitExtWdogTime' im 'File ADLATUS_ExternalWatchdog.c' 
einstellen. In der Auslieferungsversion ist dieser Wert auf 100 msec gesetzt.
Inerhalb des Startupcodes erfolgt noch keine Triggerung des Watchdogs.
Der Timeout des internen Watchdogs wird im Rahmen der Initialisierung des 
ADLATUS konfiguriert.

Zus�tzlich ist ein weiterer WDT Triggerpunkt am Einsprung in die Applikation.
Dadurch hat die Applikation nach Aufstarten 100ms Zeit bis zum n�chsten WDT
Trigger.


+------------------------------------------------------------------------------+
| N O T E :   T I M E L O C K                                                  |
+------------------------------------------------------------------------------+
Der Wert 'c_CCconprj_Timelock_T16' in der Datei 'ADLATUS_ConfigProject.c' ist 
auf 600s = 10min vorkonfiguriert. Dies bedeutet eine Sperrzeit von 10min, wenn 
nacheinander 3 falsche Keys gesendet werden.
Ein Wert von 0 bedeutet eine Sperrzeit von unendlich.


+------------------------------------------------------------------------------+
| N O T E :  C H A N G E   D O U C U M E N T A T I O N                         |
+------------------------------------------------------------------------------+
Siehe im Ordner 05_Dokumentation im File �nderungsdokumentation_TMS470R1.txt
nach durchgef�hrten �nderungen der Referenzimplementierung.

+------------------------------------------------------------------------------+
| N O T E :  P A R R O T   D O W N L O A D		                       |
+------------------------------------------------------------------------------+
Dieser FBL enth�lt eine Erweiterung um das nachgeschaltete Parrot Bluetooth
Modul zu programmieren. F�r die Programmierung werden 3 logische Bl�cke ben�tigt,
Wobei nur der 3 und letzte Block im EEPROM verwaltet wird. Der Download der 
Bl�cke muss in der richtigen Reihenfolge durchgef�hrt werden. Siehe auch
086350-S1NN-ADLATUS-TMS470-MucSuc_PHSPEZ_VX.XY.pdf.

 



