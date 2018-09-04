/*    UserConst.c															*/

/*    Konstanten im Flash-Bereich des Users		*/

#include "struct.h"
#include "ustruct.h"
#include "standdef.h"
#include "userdef.h"
#include "uramext.h"

/* User-Compilierungszeit und Datum	*/
const char user_compile_time[15] = __TIME__;
const char user_compile_date[15] = __DATE__;

	/* Typ und Version des gesamten Projektes												*/
//									             				 "---------------"
	const char Kategorie_Typ[16] 				= {"WPU-STEUERUNG  "};  //genau 15 Zeichen
	const char Kategorie_Vers[3] 				= {18,2,27};           	//Jahr, Monat, Tag

	const char Proj_Typ = 1;		// konstantes Projekt, für variable Proj. auf 0 setzen

// Unterschriften in der 2.Zeile
const char Zsync[16]								= {"SYNCHRONISIEREN"};
const char Leitstr[16]							= {"RS485, MODEM   "};
const char Ein_Ausg[16]							= {"EIN-/AUSGAENGE "};
const char KesBegr[16]							= {"ANHEBUNG       "};
const char KesAllg[16]							= {" allg.Parameter"};
const char KesPu[16]								= {" Drehzahl      "};
const char KesTyp[8][16]	= {
	{"               "},
	{"               "},
	{"               "},
	{"               "},
	{" modulierend   "},
	{" Sollwert      "},
	{" 1-stufig      "},
	{" 2-stufig      "},
};	

// SiWa
const char Wpmod[16]								= {"Steuerung      "};

const char Ntmod[16]								= {"VOR-REGELUNG   "};
const char Hkmod_fuss[16]						= {"FUSSBODEN      "};

const char Hkmod[4][16]	= {
	{"               "},
	{"               "},
	{"               "},
	{"               "},
};	

const char Wwmod[2][16]	= {
	{"               "},
	{"               "},
};	

const char Monatswerte[16] 					= {" Monatswerte   "};		//MBus

const char Stoermeld[16]						=	{"STOERMELDUNGEN "};
const char ModBusMasterSlave[16]		=	{"MASTER/SLAVE   "};
const char ModBusMaster[16]					= {"MASTER         "};	
const char ModBusSlave[16]					= {"SLAVE          "};	

/***** ulsch : Heizgradtage *****/
const char HzGrdTg[16]						  =	{"G{15} VDI3807  "};

const char Leistung[16]						  =	{"LEISTUNG       "};

//----------------------------------------------------------------------------------------------------
// Objekt- und Standortbezeichnungen            	Anzahl von ASCII-Zeichen        Beispiel
//----------------------------------------------------------------------------------------------------
const char c_Objektbez [16]         = {"Juri Maxx WPU E"};  // 15              = {"Realschule     "}; 
const char c_Objektnum [10]         = {"08_2018_J"};        //  9              = {"LU1-Klass"};       
const char c_Strasse   [16]         = {"Schwelmer Str. "};  // 15              = {"A67 LU1 Klassen"}; 
const char c_Nummer    [10]         = {"20-24    "};        //  9              = {"         "};       
const char c_Ort       [16]         = {"Berlin         "};  // 15              = {"Koesching      "}; 
const char c_Plz       [10]         = {"12207    "};        //  9              = {"         "};       

// Voreinstellung BUS-Betrieb
const char c_Service_Idf [16]       = {"42843152       "};  // 15 
const char c_ModulAdr               =  11;                   // 0= dummy
const char c_StationsAdr            =  0;                   // 0= die Stationsadresse folgt der Moduladresse automatisch (wenn Station-Initadresse 254)

// Voreinstellung Schnittstelle und Funktionen
const char c_Mode_S1                = SLAVE;            // NULL_MODE = dummy,  MASTER, SLAVE
const UINT c_Baudrate_S1            = 9600;                    // 0         = dummy,  z.B. 9600
const char c_Funktion_S1            = GBUS_FU;              // NULL_FU   = dummy,  GBUS_FU, MODBUS1_FU, MBUS1_FU, GENI1_FU...siehe projdef.h
const char c_Parity_S1              = NULL_PARI;            // NULL_PARI = dummy,  NOPAR, EVEN, ODD 
const char c_Stopbits_S1            = NULL_STOP;            // NULL_STOP = dummy,  1, 2 

const char c_Mode_S2                = NULL_MODE;            // NULL_MODE = dummy,  MASTER, SLAVE
const UINT c_Baudrate_S2            = 0;                    // 0         = dummy
const char c_Funktion_S2            = NULL_FU;              // NULL_FU   = dummy   GBUS_FU, MODBUS1_FU, MBUS1_FU, GENI1_FU...siehe projdef.h
const char c_Parity_S2              = NULL_PARI;            // NULL_PARI = dummy,  NOPAR, EVEN, ODD 
const char c_Stopbits_S2            = NULL_STOP;            // NULL_STOP = dummy,  1, 2 

const char c_Mode_S3                = NULL_MODE;            // NULL_MODE = dummy,  MASTER, SLAVE
const UINT c_Baudrate_S3            = 0;                    // 0         = dummy
const char c_Funktion_S3            = NULL_FU;              // NULL_FU   = dummy,  GBUS_FU, MODBUS1_FU, MBUS1_FU, GENI1_FU...siehe projdef.h
const char c_Parity_S3              = NULL_PARI;            // NULL_PARI = dummy,  NOPAR, EVEN, ODD 
const char c_Stopbits_S3            = NULL_STOP;            // NULL_STOP = dummy,  1, 2 

//------------------------------------------------------------------------------------------

//#define VERS_DATUM	{8,3,18}	// neu: UserHandlerS3, R3X_AADAT, R3X_DADAT
//#define VERS_DATUM	{8,5,5}		// SYNC-Gruppe gelöscht. Synchr. erfolgt in zin3-10.h
//#define VERS_DATUM	{8,8,8}		// Kommando WR_KASKADE: Antwort erweitert mit Hk-Informationen
	
//#define VERS_DATUM	{8,12,9}	// Uhr.c: Resetzeit wird gespeichert, Anzeige in parli.h hinter SYS;80; 
//#define VERS_DATUM	{8,12,22} // Änderung des Versionsdatums bewirkt Init_Leittab(); (entspricht Kaltstart 223)
//#define VERS_DATUM	{9,1,8} 	// UserReply.c: Funkempfang implementiert
//#define VERS_DATUM	{9,1,19}	// SteuerWW.c: BM Tauscherpumpe überwachen
																// RegelHk.c: im Stützbetrieb abgesenkten Vorlauf benutzen
																// Steuer.c: PUMIN auch bei Bedarfsabsenkung: Minimaldrehzahl in der Absenkphase, nicht bei Aufheizphase													
//#define VERS_DATUM	{9,2,10}	// GENIbus (Grundfos-Pumpen-Bus)
//#define VERS_DATUM	{9,2,11}	// UserReply.c: in SysRTC_InitUser() Löschen Zähler Vorjahr eingebaut, R37_1 u. R37_2 berichtigt
//#define VERS_DATUM	{9,2,13}	// UserReply.c: in get_zpointer() Zählerdummy untersuchen 
//#define VERS_DATUM	{9,3,12}	// Fehler in Absenk.c beseitigt, Aufheizzeiten bei Zeitabsenkung verlängert
//#define VERS_DATUM	{9,3,24}	// Zählernummer für Leittechnik sichtbar, neue R36CLib (LzKonv.c erweitert) vom 24.03.09 verwenden
//#define VERS_DATUM	{9,4,15}	// Monza.c: Fehler korrigiert in Clear_Verbr(..., UINT monats_spadr,...) , vorher char monats_spadr
																// U_Funkempf.c: SRC_RS485_EVC 1 (neues EnOcean Modul)
//#define VERS_DATUM	{9,4,16}	// MBUS-Implementierung	
//#define VERS_DATUM	{9,5,7}		// Korrekturen: MBus,	Neu: Regelabw.alarm NT
//#define VERS_DATUM	{9,5,25}	// Implementierung: Archivierung mit SD-Card
//#define VERS_DATUM	{9,6,3}		// Implementierung: Speicherreservierung (mit malloc und free) 	Heapsize 2K
																// Korrekturen: MBus in U_Mbus.c , im System: BicMove.c erweitert auf maximal 6 Zähler
//#define VERS_DATUM	{9,6,10}	// Korrekturen: ArchivSD.c	
//#define VERS_DATUM	{9,6,22}	// MBus-Erweiterung auf 6 Zähler, Parametererweiterung in get_feldadresse(..., char *attr) (auch Systemänderung),
																// Korrekturen: ArchivSD.c	
//#define VERS_DATUM	{9,7,1}		// Korrekturen: ArchivSD.c, Alarme.c (Quit-Taste)
//#define VERS_DATUM	{9,7,9}		// Korrekturen: ArchivSD.c, UserRam.c(struct DFILE afile;)
//#define VERS_DATUM	{9,9,29}	// Erweiterung: Anschluss Ni1000 mit Parallelwiderstand 7,5kOhm,  Typauswahl in parli SYS: (hid2) 0=Pt1000 1=Ni1000
//#define VERS_DATUM	{9,10,9}	// UserReply.c: Erweiterung UserPrimReply(...) entsprechend Systemänderung vom 09.10.2009
																//              Einbindung CoolBC und HeatBC in Countdownbehandlung, Anzeige hinter SYS: 40: (hid2)
	
//#define VERS_DATUM	{10,1,5}	// Erweiterung: Neues Kommando RD_ALARMLIST (Lesen der akt. Alarme mit WinRFB V.3.4)
																//              Änderung in Alarme.c, UserReply.c, UserRam.s, uramext.h

//#define VERS_DATUM	{10,3,26}	// Korrekturen: ArchivSD.c (mit System-Änderung: R36CCard) ***josch
//#define VERS_DATUM	{10,4,6}	// Korrekturen: AnalogEA.c (***ulsch)

//#define VERS_DATUM	{10,4,28} // Korrekturen: ArchivSD.c (mit System-Änderung: R36CCard) ***josch
                                // Neu: SPL-Modul-Kennung wird nach Reset gelesen und in Gruppe ZLT (S3-Schnittstelle) angezeigt
                                // Neu: parli.h: Änderung Testgruppe auf hid4 (Service-Code 139), betrifft auch UserReply.c
                                // Neu: Flash-Bereich um 64K vergrößert
//#define VERS_DATUM	{10,8,12} // parli.h (ZLT: KENNUNG-S3 SIMU): Simulation einer SPL-Modul-Kennung, falls Modul nicht lesbar (System ab 10,8,12)
//#define VERS_DATUM	{10,12,7} // UserTcb.c: (void *) in tcblist_con_user	eingeführt (für Compiler_V542 )
//
//#define VERS_DATUM	{10,12,10} // neu: WILO_MODBUS-Pumpen
//#define VERS_DATUM	{11,1,14}  // MBus: Zeit für letzten korrekt erfassten Wert und des ersten und letzten Fehlers
                                 // Darstellung der MBus-Zähler geändert (ZMB mit Expansion, ZMM entfällt, Anpassung an MBus-Master)
 //#define VERS_DATUM	{11,7,21}  // Adapt.c: Adaption akzeptiert außer 2,5,8 auch den Wert 0 (keine Adaption, aber ungestörte Datenübertragung)
                                 // UserReply.c, Steuer.c, ustruct.h: RD_STATIONS_INFO signalisiert Heizbetrieb (Winter)
                                 // Absenk.c, Bedarf.c : laufende Absenkphase wird abgebrochen, wenn Ta < -15°C

//#define VERS_DATUM	{11,8,8}   // neu: Datenmanager zum Transport von Daten zwischen R66-Geräten. Funktion Master (DM_MODE) oder Slave (DS_MODE) in projdef.h einstellbar.
                                 //      System {11,8,8}

//#define VERS_DATUM	{11,9,8}   // InOutR.c: Fehler beim R39 beseitigt (die obersten 4 Bits wurden nicht richtig dargestellt)
//#define VERS_DATUM	{11,9,14}  // InOutR.c: Fehler beim R39 beseitigt (bstat wurde nicht regeneriert)
//#define VERS_DATUM	{11,11,21}  // Modbus Wilo: Holding-Register 300 wird behandelt
//#define VERS_DATUM	{11,11,28} 	// Betriebs-/Regelart in ASCII für Leittechnik
//#define VERS_DATUM	{12,02,21} 	// KaskMode 2 (keine Kaskadierung in der Zeitabsenkung trotz HEIZ-BEDARF)
  																// VorrangZeitabsenkung: in der Absenkzeit wird trotz HEIZ-BEDARF abgesenkt
//#define VERS_DATUM  {12,5,30} // neues Betriebssystem {12,5,30} Verbesserung HEXBUF_FORM; parli.h: Attribute für HEXBUF_FORM angepasst
//#define VERS_DATUM  {12,7,4}		// MBus: Klimazähler (Molline Energy INT 8); Dimensionen 0,1 cbm, 0,1 cbm/h, 0,1 W		

//#define VERS_DATUM	{12,8,20}  // neue Funktion: usertext_erste_zeile(...) in UserKonv.c für multiple Texte in der Meldegruppe. C-System ab {12,8,20}

//#define VERS_DATUM	{12,9,4}   // Neue Komtab-Copy Organisation: #include "copyparli.h" in parli.h hinter BETRIEBSSTUNDEN, Update Komtabparser.exe (ab 04.09.2012) erforderlich.
                                 // Einstellungen in projdef.h, betrifft UserRam.c, uramext.h, CopyCheck.c, usertask.h
//#define VERS_DATUM	{12,9,5}   // Neu: Einbindung Anybus. Bei Bedarf wird aus dem Pool der Ordner "Anybus" in den Projektordner kopiert und in der HEW die C-Quellen hinzugefügt. 
//#define VERS_DATUM	{12,9,13}  // Änderung in WR_KASKADE  
//#define VERS_DATUM	{12,11,20} // Systemänderung {12,11,20}: Test auf Drahtbruch bei 0-10V-Eingängen kann unterdrückt werden
                                 // parli.h: Parameter 65 in der Testgruppe. Bit=0 : Testunterdrückung; 1.Bit links = IN10; Kaltstart 67 bzw.99 setzt alle auf 1

//#define VERS_DATUM	{13,1,24}  // BM-Überwachung HK/WW parametrierbar. ZirkPu-Laufzeit; LadePu läuft bei Legio ständig (parametrierbar)
//#define VERS_DATUM	{13,2,12}  // MBus: Leistung bei Elektro-Zählern    
  
//#define VERS_DATUM	{13,5,14}  // Neu: Objekt- und Standortbezeichnungen können vorbelegt werden (s.o.). Ausgeführt bei Kaltstart 67 bzw. 99
//#define VERS_DATUM	{13,5,27}  // Neu: Freitags-Ventilöffnung
//#define VERS_DATUM	{13,5,30}  // Parli.c: #include "standdef.h" zugefügt für variable Texte in 1. Zeile der Anzeige
                                 // Beispiel für Alarmtext in UserKonv.c (AnFre) 
//#define VERS_DATUM	{13,8,7}   // U_Mbus.c geändert (vorhergehende Version 08.04.13), in Userconst.c Voreinstellungen der MBus-Zähler
//#define VERS_DATUM  {13,8,8}   // Einführung von DYN_ASCII_KONV ( Betriebssystem {13,8,8} ) für dynamische Texte (z.B. Betriebsstatus, Betriebsregie) 
//#define VERS_DATUM  {13,10,9}	 // Fehler bei MBus beseitigt (betrifft Leistung von Berg-Elektrozählern, UserKonv.c und U_Mbus.c geändert)
//#define VERS_DATUM  {13,11,5}	 // Ergänzung in parli.h und Komtab.c: SSTM Alarme und SSTM Alarme+Fühler

//#define VERS_DATUM  {13,11,26} // Ab Betriebssystem {13,11,26}  
                                 // Erweiterung: Anschluss Ni1000TK5000 ( nach L&S ) mit Parallelwiderstand 7,5k 
                                 // Typauswahl in parli SYS: (hid2) 0=Pt1000 1=Ni1000 2=Ni1000TK5000
                                 
//#define VERS_DATUM  {13,11,27} // AnFre: U_ModBusSio_Wilo.c statt U_ModBusSio.c.
                                 //        S2-Schnittstelle für WILO_MODBUS-Pumpen möglich. (Fehler beseitigt)
                                 // AnFre: U_Wilo.c und UserConst.c: Für Wilo-Pumpen "Keine Meldung !" in Anzeige zugefügt
                                 //        U_WILO.c S2-Schnittstelle freigeschaltet	 
                                 // JoSch: DTimer korrigiert

//#define VERS_DATUM  {14,2,11}  // ulsch: für RIEconMis UserReply.c geändert (MBus-Zähler wie bei MBus-Master behandelt)
                                 //        (UserReply.c, RD_INI_ZAEHLER,WR_INI_ZAEHLER,RD_KONSUM,WR_KONSUM,RD_SYNC_KONSUM,WR_SYNC_KONSUM) 
                                 // ulsch: interne Wärmemengen-Zähler zählerobjekt-kompatibel gemacht (Wmeng.c, UserReply.c, UserRam.c)   

//#define VERS_DATUM  {14,2,26}  // JoSch: Erweiterte In-/Out Anzeige in ANL: Anzeige der entspr. Parameterbezeichnung aus der parli.h, mit folgenden
                                 //        Änderungen: System ab {14,2,25}, parli.h, parli_inout.h (neu), InitEA.c, UserReply.c, UserRam.c und uramext.h                                     
                                 //      - Weitere Aufräumarbeiten in der parli.h mit Bildung von "include ...". Einheitliche Namensgebung der parli-include-Dateien.
//#define VERS_DATUM  {14,4,15}  // JoSch: Erweiterte In-/Out Anzeige in ANL: hinzugefügt SSTM RELAIS (parli.h) und QUITTS[0] (qsm.h) 
//#define VERS_DATUM  {14,6,11}  // ulsch: U-Mbus.c geändert (WMZ Sensus Pollustat, u.a. mit Real-Formaten); 
                                 // Zähler-Typ nicht mehr nach Respond-Telegramm, sondern entspr.Parametrierung 
//#define VERS_DATUM  {14,10,15} // Zwei-Kessel-Anlage
//#define VERS_DATUM  {14,11,19} // JoSch: Neues System  {14,11,18} : Anpassungen für get_feldadresse und user_erste_zeile
                                 // Änderungen in allen parlis mit EXPAND_CHAR, ustruct.h, userdef.h, UserReply.c, UserKonv.c        

//#define VERS_DATUM  {14,12,9}  // UlSch: MBus-Gaszähler implementiert (U_Mbus.c, parli-mbus.h, projdef.h, UserKonv.c, ustruct.h)      

//#define VERS_DATUM  {14,12,17} // JoSch: Erweiterung: Anschluss Pt500 mit Reihenwiderstand 500 Ohm. Neues System {14,12,16} erforderlich.
                                 //        in SYS; 1-8 Anzeige verändert: Pt1000 -> SENSOR. Der Sensortyp wird in Klartext angezeigt.
                                 
                                 // JoSch: Erweiterung: universell verwendbare Benutzersteuerung mit UNI-Elementen (SteuerUni.c, parli_uni.h, UniStandard, UniDynam)       
                                 
                                 // JoSch: Änderung Regelung: Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
                                 //        betrifft RegelHk.c, RegelNt.c, RegelWw.c, Funktion.c, ustruct.h. Versteckte Zusatzinformationen in parli hinter *98; VENTILSTELLUNG   

//#define VERS_DATUM  {15,1,5}  // UlSch: parli_mbus.h nun mit ZAEHLERWERT in Gruppe MBA (zwecks Archivierung)
//#define VERS_DATUM  {15,1,20} // UlSch: MbusDataCopy entfällt (überflüssig durch Möglichkeit, expandierte Parameter auch einzeln darzustellen)
                                // betrifft parli_mbus.h, Steuer.c, UserRam.c, uramext.h
                                // Gaszähler Actaris/Itron implementiert (U_Mbus.c)
                                // Heizungs-Doppelpumpen: beide laufen, wenn eine gestört (ThoSi)
//#define VERS_DATUM  {15,2,26} // UlSch: Änderung Mbus
//#define VERS_DATUM  {15,3,24} // UlSch: Änderung Wilo
//#define VERS_DATUM  {15,3,26} // JoSch: Bugfix in UserKonv.c   Funktion: user_erste_zeile(...) ucMax_expander
//#define VERS_DATUM  {15,5,6}	// UlSch: Änderung Mbus ( VolumeVarVIFE() )  

//#define VERS_DATUM  {15,6,1}  // JoSch: Neu ArchivSDM.c:    Speicherung aller Parameter (Komtab), die sich verändert haben, auf SD-Card im 5 Minuten-Raster
                                 //       Vorraussetzungen:  - Betriebssystem mit Monitoring-Task (DMonitor.c) ab  {15,6,1}
                                 //                          - 4 GB SDHC Class 10
                                 //                          - Speicherriegel mit 2. Ferro-Ram
                                 //                          - projdef.h: #define ARCHIV_SDC  2
                                 // JoSch + AnBu: parli.h  Integration GPRS (siehe Beschreibung GPRS) bzw suche in Dateien nach "// Änderung für R66 GPRS" 
                                 // JoSch: Neues Kommando: RD_MONSUM (Lese Monatsverbräuche), betrifft UserReply.c
                                 // JoSch: Kommandoerweiterung KOMTAB_READ zur Erfassung von Expansionsparametern

//#define VERS_DATUM  {15,9,8}   // UlSch: Änderung U_Mbus.c, parli_mbus.h
//#define VERS_DATUM  {15,9,21}  // UlSch (ThoSi): 
                                 // bei RCOND_WW1=1 regeln während Nachlauf Ladepumpe
                                 // Nachlauf Ladepumpe nun 10 min Standard 
//#define VERS_DATUM  {15,9,23}  // mehrere Änderungen für Zwei-Kessel-Anlagen (betrifft auch UserRam.c)
//#define VERS_DATUM  {15,10,13} // U_Mbus.c geändert ( TempVar() 
//#define VERS_DATUM  {15,11,4}  // UlSch: Steuer2Ke.c geändert (bei Bestimmung der Ist-Kesselfolge wurde  gestört/gesperrt/Hand gleich behandelt, nun Priorisierung in dieser Reihenfolge)
//#define VERS_DATUM  {15,11,4}  // Josch: DManager.c: Bugfixing bei AOUT_FORMP

//#define VERS_DATUM  {16,1,14}  // Josch-SDM : Neues Betriebssystem {16,1,14} mit Erweiterung IrDA als IrDA-S4 Schnittstelle (über Modul RIEcon83)
                                 // ArchivSD.c mit Filesystem entfällt. ArchivSDM.c erweitert mit Testkommando für Prüfplatz.
                                 // Erforderliche Änderungen sind in "R36CPool_Anpassung_160114.txt " dokumentiert.
//#define VERS_DATUM  {16,2,9}   // ulsch: 2 WW-Bereitungen. Geändert: SteuerWw,c, RegelWw.c, parli.h, ustruct.h, UserConst.c, uconstext.h, projdef.h, Steuer.c, RegelHk.c, Alarme.c, UserReply.c
  
//#define VERS_DATUM  {16,5,19}  // Josch: Fehler in Archiv_SDM.c korrigiert.Geändert: parli.h SYS; 88; ff.

//#define VERS_DATUM  {16,5,23}  // Software für 1 Kessel komplett neu
//#define VERS_DATUM  {16,6,21}  // Fehler in SteuerWw.c beseitigt: nach Speicherladung geht der Vorrang u.U.nicht zu Ende

//#define VERS_DATUM  {16,6,29}  // Josch: Neue Implementierung Modbus (mit Pumpen (WILO_MODBUS, GENI, BELIMO), ohne Pumpen als Master oder Slave)
//#define VERS_DATUM  {16,7,26}	 // ulsch: Kessel-Implementierung neu
//#define VERS_DATUM  {16,9,29}  // AnBu: Änderung Gprs: parli.h ("work handler")
//#define VERS_DATUM  {16,11,9}  // Josch,Joja: Modbus-Ergänzung (StopBits, READ_MULTIPLE_COILS, ModbusExtract.c) 
//#define VERS_DATUM  {16,11,15} // Josch: Alarme.c, parli.h: Soft-Quittierung ohne Quittiertaste möglich.
//#define VERS_DATUM  {16,11,29} // Josch: DTimer.c : Erweiterung - Sende Broadcast-Uhr
//#define VERS_DATUM  {17,2,1}   // Josch: ArchivSDM.c: Automatische Fehlerkorrektur, betrifft: UserRam.c, uramext.h, archivsdm.h, UserReply.c, parli.h
//#define VERS_DATUM  {17,4,5}   // Josch: ArchivSDM.c bugfix,  parli.h Neuordnung siehe ARCHIV_SDM
//#define VERS_DATUM  {17,4,12}  // Josch: Neues System {17,4,12} : StationsAdresse synchronisieren mit Moduladresse.
                                 //        betrifft  parli.h ZLT; *20; und folgenden ( ->;) Parameter übernehmen   
                                 //        außerdem: Eine User-Compile-Zeit wird generiert, zu sehen in TES; *92; bis *93; 
//#define VERS_DATUM  {17,6,19}	 // Ulsch: 8 MBus-Zähler

//#define VERS_DATUM  {17,6,21}  // Josch: Neues System {17,6,21} :
                                 //        Parität und Stopbits für 3 Schnittstellen sind in UserConst.c voreinstellbar. Sichtbar in Gruppe ZLT;   
                                 //        Deaktivierung von Sensor- und Analogeingängen möglich. Zugriff in SYS: 09;SENSOR-IN LIFE und 10;ANALOG-IN LIFE  und in R38_MODULE 055;
                                 // AnBu:  GPRS zurück zur Version vom 29.09.2016                                                                                                  
                                    
//#define VERS_DATUM  {17,8,30}  // Josch: ZLT Anzeige des Funktionsnamens. Betrifft: parli.h, UserKonv.c, uskonvform.h, UserConst.c, uconstext.h
                                 // JoJa:  Modbus.c: Spezialkommando WRITE_SINGLE_COIL korrigiert

//#define VERS_DATUM  {17,9,26}  // Josch: EA-Simulation mit System {17,9,26}
                                 //        betrifft InOutR.c, UserRam.c .h, userdef.h, parli.h
//#define VERS_DATUM  {17,11,13} // Josch: R3X-Simulator mit System {17,11,1} incl. Modulkonfiguration
                                 //        betrifft InOutR.c, UserRam.c uramext.h
//#define VERS_DATUM  {18,2,27}  // SiWa: Neue WPU-Software mit System vom 01.11.2017 und User-Teil aus RiedelPool vom 13.11.2017
//#define VERS_DATUM  {18,3,9}	 //	SiWa: Temperaturfühler angelegt und BM, SM und Anforderung/Freigabe, Sammelstörung über UNIs implementiert, parli WPU hinzugefügt
//#define VERS_DATUM  {18,4,4}	 //	SiWa: Manuelle Ansteuerung der WPU via AA und Betriebszustand Quellenschutz
//#define VERS_DATUM  {18,4,10}	 //	SiWa: Betriebszustand Speicherprinzip und Frostschutz, Mindestlauf- und Sperrzeit für WPU-Freigabe (DA)
//#define VERS_DATUM  {18,8,21}	 // SiWa: Werkseinstellungen angepasst, QSM hinzugefügt, Funktion: Min Max-Begrenzung bei eingehenden Sollwert hinzugefügt. Zählen der WPU Freigabe auf Ausgang bezogen
	#define VERS_DATUM  {18,9,3}	 // SiWa:	Neue Software-Version: WP_1_000_200000, Unterscheidung zwischen Abluftwärmepumpen- und Solewärmepumpensoftware in Parli und SteuerWPU
                              	 //				Keine Ausführung von void SteuerWPU nach Neustart	
                              	 // 			Mit diesem Stand kann die Abluftwärmepumpe im Projekt Erkner betrieben werden 
/*--------------------------------------------------------------------------------*/
/* 										Liste von Anlagen																						*/
/*--------------------------------------------------------------------------------*/
const Anlage Projekte[] = {
	// 0. Projekt (Dummy-Projekt)
	{	
	 // Typ und Version
	 //"??-R-????-?????"			//Kennzeichen an fester Position !!	
		{"XX-R-????-?????"},		//genau 15 ASCII-Zeichen
		{1,1,1},	           		//Jahr, Monat, Tag
		// Kategoriebezeichnung für variable Projekte
		{"KEIN PROJEKT   "},
		
		// Steuercode für Heizkreise
		{0},	// Nummern der Kaltstartparametersätze für jeden Heizkreis [HK_PROFILE]
					// 0 = Heizkreis nicht vorhanden
		
		{0},	// Nummern der Absenkprofile für jeden Heizkreis [ABS_PROFILE]
					// 0 = nicht vorhanden
					
				// Steuercode für Wärmepumpe
		{0},	// Nummern der Kaltstartparametersätze für jede WPU-Steuerung [WP_PROFILE]
					// 0 = nicht vorhanden
		
		// Steuercode für Warmwasser
		{0},	// Nummern der Kaltstartparametersätze für jeden WWkreis [WW_PROFILE]
					// 0 = nicht vorhanden
		
		// Steuercode für Kessel
		{0},	// Nummern der Kaltstartparametersätze für jeden Kesselkreis [KE_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für Fernwärmevorregelkreis (Netztrennumg)
		{0},	// Nummern der Kaltstartparametersätze für jeden Vorregelkreis [NT_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für Solar-Regelkreis
		{0},	// Nummern der Kaltstartparametersätze für jeden Solar-Regelkreis [SO_PROFILE]
					// 0 = nicht vorhanden
		
		// Steuercode für MBUS
		{0},	// Nummern der Kaltstartparametersätze für jeden MBUS-Zähler[MB_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für SteuerUni
		{0},	// Nummern der Kaltstartparametersätze für jede UNI-Steuerung [UNI_PROFILE]
					// 0 = nicht vorhanden		
					
					
		// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
		// Aus den Namen werden Steuercodes generiert
		TP_FREI,							// 				Eingang IN1		Pt1000 / ----- / -------	
		TP_FREI,							// 				Eingang IN2		Pt1000 / ----- / -------	
		TP_FREI,							// Block  Eingang IN3		Pt1000 / 0-10V / Digital
		TP_FREI,							// Block  Eingang IN4		Pt1000 / 0-10V / Digital
		TP_FREI,							// Block  Eingang IN5		Pt1000 / 0-10V / Digital
		TP_FREI,							// 				Eingang IN6		Pt1000 / 0-10V / Digital
		TP_FREI,							// 				Eingang IN7		Pt1000 / 0-10V / Digital
		TP_FREI,							// 				Eingang IN8		Pt1000 / 0-10V / Digital
		AE_FREI,							// 				Eingang IN9		------ / 0-10V / Digital
		DE_FREI,							// 				Eingang IN10	------ / 0-10V / Digital
		
		AA_FREI,							// Analog-Ausgang  AA1
		AA_FREI,							// Analog-Ausgang  AA2
		
		DA_FREI,							// Digital-Ausgang DA1
		DA_FREI,							// Digital-Ausgang DA2
		DA_FREI,							// Digital-Ausgang DA3
		DA_FREI,							// Digital-Ausgang DA4
		DA_FREI,							// Digital-Ausgang DA5
		DA_FREI,							// Digital-Ausgang DA6
		
		// Externe Erweiterungen mit R37, R38 , siehe Liste Anl37 oder Anl38
		{	{  0, 0, 0},				// Gerätekennzeichen, Gerätenummer, Nummer des Belegungsblockes
			{  0, 0, 0},
			{  0, 0, 0},
			{  0, 0, 0},
		},

		// NAMUR-Kennzeichnung für Digitale Eingänge R36
		0x00,									// Bit=1...NAMUR-Eingang,	BIT0...7 = IN3...IN10
	},
	
	// 1. Projekt: 
	{	
	 // Typ und Version
	 //"??-R-????-?????"			//Kennzeichen an fester Position !!	
		{"WP-1-000-200000"},		//genau 15 ASCII-Zeichen
		VERS_DATUM,	           		//Jahr, Monat, Tag
		// Kategoriebezeichnung für variable Projekte
		{"TEST PROJEKT   "},
		
		// Steuercode für Heizkreise
		{0},	// Nummern der Kaltstartparametersätze für jeden Heizkreis [HK_PROFILE]
					// 0 = Heizkreis nicht vorhanden
		
		{0},	// Nummern der Absenkprofile für jeden Heizkreis [ABS_PROFILE]
					// 0 = nicht vorhanden
					
					// Steuercode für Wärmepumpe
		{2},	// Nummern der Kaltstartparametersätze für jede WPU-Steuerung [WP_PROFILE]
					// 0 = nicht vorhanden, 1 = SWP, 2 = AWP
					
		// Steuercode für Warmwasser
		{0},	// Nummern der Kaltstartparametersätze für jeden WWkreis [WW_PROFILE]
					// 0 = nicht vorhanden
		
		// Steuercode für Kessel
		{0},	// Nummern der Kaltstartparametersätze für jeden Kesselkreis [KE_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für Fernwärmevorregelkreis (Netztrennumg)
		{0},	// Nummern der Kaltstartparametersätze für jeden Vorregelkreis [NT_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für Solar-Regelkreis
		{0},	// Nummern der Kaltstartparametersätze für jeden Solar-Regelkreis [SO_PROFILE]
					// 0 = nicht vorhanden

		// Steuercode für MBUS
		{0},	// Nummern der Kaltstartparametersätze für jeden MBUS-Zähler[MB_PROFILE]
							// 0 = nicht vorhanden

		// Steuercode für SteuerUni
		{0},	// Nummern der Kaltstartparametersätze für jede UNI-Steuerung [UNI_PROFILE]
					// 0 = nicht vorhanden
					
		// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
		// Aus den Namen werden Steuercodes generiert
		TP_FREI,	// TP FREI														// 				Eingang IN1		Pt1000 / ----- / -------	
		TP_FREI,	// TP FREI														// 				Eingang IN2		Pt1000 / ----- / -------	
		BM_UNI1,	// Betriebsmeldung Wärmepumpe BM WPU	// Block  Eingang IN3		Pt1000 / 0-10V / Digital
		SM_UNI1,	// Störmeldung Wärmepumpe 		SM WPU	// Block  Eingang IN4		Pt1000 / 0-10V / Digital
		DE_FREI,	// DE FREI														// Block  Eingang IN5		Pt1000 / 0-10V / Digital
		AE_UNI1,	// Externe Anforderung								// 				Eingang IN6		Pt1000 / 0-10V / Digital
		ZIN7,			// Volumenstr.geber Heißgas		WWW			// 				Eingang IN7		Pt1000 / 0-10V / Digital
		ZIN8,			// Volumenstr.geber Sp-St			WGW			// 				Eingang IN8		Pt1000 / 0-10V / Digital
		ZIN9,			// Volumenstr.geber WPU-Sp	 	WWS			// 				Eingang IN9		------ / 0-10V / Digital
		ZIN10,		// Stromzähler WPU	 					ESW			// 				Eingang IN10	------ / 0-10V / Digital
		
		TMANF1,	// Sollwert WPU		 										// 				Analog-Ausgang  AA1
		AA_FREI,	// AA FREI					 									// Analog-Ausgang  AA2
		
		DA_UNI1,	// Freigabe/Anforderung WPU						// Digital-Ausgang DA1
		DA_FREI,	// DA FREI														// Digital-Ausgang DA2
		DA_FREI,	// DA FREI														// Digital-Ausgang DA3
		DA_FREI,	// DA FREI														// Digital-Ausgang DA4
		DA_FREI,	// DA FREI														// Digital-Ausgang DA5
		SSTM1,		// Sammelstörung											// Digital-Ausgang DA6
		
		// Externe Erweiterungen mit R37, R38... , siehe Liste Anl37 oder Anl38...
		{	{  R38, 1, 1},				// Gerätekennzeichen, Gerätenummer, Nummer des Belegungsblockes
			{  0, 0, 0},
			{  0, 0, 0},
			{  0, 0, 0},
		},			

		// NAMUR-Kennzeichnung für Digitale Eingänge R36
		0x00,									// Bit=1...NAMUR-Eingang,	BIT0...7 = IN3...IN10
	},
	
	
};
const char PROJTYPEN = sizeof(Projekte) / sizeof(Anlage);
	
/*----------------------------------------------------------------------------*/
/*  							Liste von Ein-/Ausgangs-Belegungssätzen für R37-Module			*/
/*----------------------------------------------------------------------------*/
const Anl37 R37_Beleg[] = {
	// 0. Belegungssatz (Dummy-Set)
	{	
		DE_FREI,		// Digital-Eingang DE1 
		DE_FREI,		// Digital-Eingang DE2
		DE_FREI,		// Digital-Eingang DE3 
		DE_FREI,		// Digital-Eingang DE4 
		DE_FREI,		// Digital-Eingang DE5 
		DE_FREI,		// Digital-Eingang DE6
		
		AA_FREI,		// Analog-Ausgang  AA1
		AA_FREI,		// Analog-Ausgang  AA2
		AA_FREI,		// Analog-Ausgang  AA3
		AA_FREI,		// Analog-Ausgang  AA4
	
		DA_FREI,		// Digital-Ausgang DA1
		DA_FREI,		// Digital-Ausgang DA2
		DA_FREI,		// Digital-Ausgang DA3
	},

				
	// 1. Belegungssatz Anwendung für R37_1
	{	
		DE_FREI,		// 														// Digital-Eingang DE1	(ZIN11)	
		DE_FREI,		//														// Digital-Eingang DE2	(ZIN12)	
		DE_FREI,		// 														// Digital-Eingang DE3	(ZIN13)	
		DE_FREI,		// 														// Digital-Eingang DE4	(ZIN14)  
		DE_FREI,		// 														// Digital-Eingang DE5	(ZIN15)  
		DE_FREI,		// 														// Digital-Eingang DE6	(ZIN16) 
		
		RVENTWW1,		//														// Analog-Ausgang  AA1
		AA_FREI,		//														// Analog-Ausgang  AA2
		AA_FREI,		//														// Analog-Ausgang  AA3
		AA_FREI,		//														// Analog-Ausgang  AA4
	
		DA_FREI,		// 														// Digital-Ausgang DA1
		DA_FREI,		// 														// Digital-Ausgang DA2
		DA_FREI,		// 														// Digital-Ausgang DA3
	},
		
};	
const char R37SETS = sizeof(R37_Beleg) / sizeof(Anl37);

/*----------------------------------------------------------------------------*/
/*  							Liste von Ein-/Ausgangs-Belegungssätzen für R38-Module			*/
/*----------------------------------------------------------------------------*/
const Anl38 R38_Beleg[] = {
	// 0. Belegungssatz (Dummy-Set)
	{	
		TP_FREI,		// Pt1000 Eingang IN1
		TP_FREI,		// Pt1000 Eingang IN2
		TP_FREI,		// Pt1000 Eingang IN3
		TP_FREI,		// Pt1000 Eingang IN4
		TP_FREI,		// Pt1000 Eingang IN5
		TP_FREI,		// Pt1000 Eingang IN6
		TP_FREI,		// Pt1000 Eingang IN7
		TP_FREI,		// Pt1000 Eingang IN8
		TP_FREI,		// Pt1000 Eingang IN9
		TP_FREI,		// Pt1000 Eingang IN10
		TP_FREI,		// Pt1000 Eingang IN11
		TP_FREI,		// Pt1000 Eingang IN12
		TP_FREI,		// Pt1000 Eingang IN13
		TP_FREI,		// Pt1000 Eingang IN14
	},

	// 1. Belegungssatz 
	{	
		TV_WP1,			// Vorlauf  Wärmepumpe zum Speicher  TV-WWP 	// Pt1000 Eingang IN1
		TR_WP1,			// Rücklauf Wärmepumpe zum Speicher  TR-WWP 	// Pt1000 Eingang IN2
		TSPo_WP1,		// Speicher oben Wärmepumpe 				 TSPo-WWP	// Pt1000 Eingang IN3
		TSPu_WP1,		// Speicher unten Wärmepumpe 				 TSPu-WWP // Pt1000 Eingang IN4
		TVST_WP1,		// Vorlauf WPU-Speicher zur Station  TVST-WWP	// Pt1000 Eingang IN5
		TRST_WP1,		// Rücklauf WPU-Speicher zur Station TVST-WWP	// Pt1000 Eingang IN6
		TVQ_WP1,		// Vorlauf Wärmepumpe quellenseitig  TVQ-WWP	// Pt1000 Eingang IN7
		TRQ_WP1,		// Rücklauf Wärmepumpe quellenseitig TRQ-WWP	// Pt1000 Eingang IN8
		TVH_WP1,		// Vorlauf Heißgas					         TVH-WWP	// Pt1000 Eingang IN9
		TRH_WP1,		// Rücklauf Heißgas					         TRH-WWP	// Pt1000 Eingang IN10
		TSPm_WP1,		// Speicher Mitte Wärmepumpe 				 TSPm-WWP // Pt1000 Eingang IN11
		TP_FREI,		// 										  											// Pt1000 Eingang IN12
		TP_FREI,		//																						// Pt1000 Eingang IN13
		TP_FREI,		//																						// Pt1000 Eingang IN14
	},

};
const char R38SETS = sizeof(R38_Beleg) / sizeof(Anl38);
			
/*----------------------------------------------------------------------------*/
/*  							Liste von Ein-/Ausgangs-Belegungssätzen für R39-Module			*/
/*----------------------------------------------------------------------------*/
const Anl39 R39_Beleg[] = {
	// 0. Belegungssatz (Dummy-Set)
	{	
		DE_FREI,		// Eingang DE1
		DE_FREI,		// Eingang DE2
		DE_FREI,		// Eingang DE3
		DE_FREI,		// Eingang DE4
		DE_FREI,		// Eingang DE5
		DE_FREI,		// Eingang DE6
		DE_FREI,		// Eingang DE7
		DE_FREI,		// Eingang DE8
		DE_FREI,		// Eingang DE9
		DE_FREI,		// Eingang DE10
		DE_FREI,		// Eingang DE11
		DE_FREI,		// Eingang DE12
		// Signalart: 0=Schließer 1=Öffner  Bit0=DE1
		0x0000,			// .... 0000 0000 0000
	},

	{	
		STWNT1,			// SM STW1 Fernwärme						// Eingang DE1
		SM_UNI1,		// SM STW2 Fernwärme						// Eingang DE2
		SM_UNI2,		// SM Max-Druck									// Eingang DE3
		PUAL1,			// SM Pumpe HK1									// Eingang DE4
		DE_FREI,		// Eingang DE5
		DE_FREI,		// Eingang DE6
		DE_FREI,		// Eingang DE7
		DE_FREI,		// Eingang DE8
		DE_FREI,		// Eingang DE9
		DE_FREI,		// Eingang DE10
		DE_FREI,		// Eingang DE11
		DE_FREI,		// Eingang DE12
		// Signalart: 0=Schließer 1=Öffner  Bit0=DE1
		0x0000,			// .... 0000 0000 0000
	},

};
const char R39SETS = sizeof(R39_Beleg) / sizeof(Anl39);

/*----------------------------------------------------------------------------*/
/*  							Liste von Ausgangs-Belegungssätzen für R33-Module						*/
/*----------------------------------------------------------------------------*/
const Anl33 R33_Beleg[] = {
	// 0. Belegungssatz (Dummy-Set)
	{	
		DA_FREI,		// Digital-Ausgang DA1
		DA_FREI,		// Digital-Ausgang DA2
		DA_FREI,		// Digital-Ausgang DA3
		DA_FREI,		// Digital-Ausgang DA4
		DA_FREI,		// Digital-Ausgang DA5
		DA_FREI,		// Digital-Ausgang DA6
	},


};	
const char R33SETS = sizeof(R33_Beleg) / sizeof(Anl33);

/*----------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für MBUS														*/
/*----------------------------------------------------------------------------*/
const MbusStandard  Mbus_Standparam[] = {
 // Profil 0 (dummy)
 {
	0,															// Freigabe
	0,															// Telegramm-Folge
	0,															// Zähler-Typ: Wärme 4, Kaltwasser 22, Warmwasser 6, Elektro 2
	0,
 },

 // Profil 1: Wärmezähler ISTA Sensonic II, freigegeben
 {1,1,4,2},

 // Profil 2: Wasserzähler ISTA, freigegeben
 {1,1,6,22},

 // Profil 3: Elektrozähler PadPuls M2, freigegeben
 {
	1,1,2,6},

 // Profil 4: Kaltwasserzähler PadPuls M2, freigegeben
 {1,1,0x16,8},

 // Profil 5 (dummy)
 {0,1,0,0},

 // Profil 6 (Standard: Wärmezähler, nicht freigegeben, Adresse 0)
 {0,1,4,0},

 {1,1,4,1},
 {1,1,4,2},


}; 	
const char MB_PROFILE = sizeof ( Mbus_Standparam ) / sizeof ( MbusStandard ); 	



/*----------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für alle Bus-Pumpen						   */
/*								Zuordnung für Pumpen 1 bis 8 entsprechend projdef.h		      */
/*----------------------------------------------------------------------------*/
const PuBusStandard  PuBus_Standparam[] = {

	// Profil 1 Pumpe 1 (Index 0)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"PUMPE KESSEL 1 "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		1,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		2,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert = 80,0 %			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 2 Pumpe 2 (Index 1)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"PUMPE KESSEL 2 "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		2,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		2,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10				
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 3 Pumpe 3 (Index 2)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"PUMPE HK1      "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		3,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		1,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 4 Pumpe 4 (Index 3)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"HEIZUNGSPUMPE  "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		4,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		1,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 5 Pumpe 5 (Index 4)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"TAUSCHERPUMPE  "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		5,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		2,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 6 Pumpe 6 (Index 5)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"LADEPUMPE      "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		6,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		2,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

	// Profil 7 Pumpe 7 (Index 6)
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"ZIRK-PUMPE     "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		7,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10			
		3,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

 // Profil 8 Pumpe 8 (Index 7) 
 {
		0,														// char	Funktion; 			// deaktiviert (0), remote (1), local (2)
		{"               "},					// Name[16];						// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
		0,														// char Adresse;				// frei wählbar, Standard = Nummer
		0,														// char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// UINT	Sollwert;				// [%] / 10				
		0,														// char	RMverz;					// Verzögerung Rückmeldung [min]
		0,														// char	Hand;						// Hand/Automatik, Hand = 1
		0,														// char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
		0,														// char	SollHand;				// [%] / 10
		0,														// char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
		0,														// char	AlarmQuit;
		2,														// char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
 },

};

			
/*----------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für verschiedene Heizkreise				*/
/*----------------------------------------------------------------------------*/
const HkStandard  Hk_Standparam[] = {
 // Profil 0 (dummy)
 {
	  0,	//int		Ts;						= 0		// Tastzeit (>= 1 s)						[s] * 10
	  0,	//UINT	Tn;						= 0		// Nachstellzeit								[s] * 10
	  0,	//int		Kp;						= 0		// P-Verstärkung							[%/K] * 100
	  0,	//int		Kd;						= 0		// D-Verstärkung							[%/K] * 100
	  0,	//int		Kpk;					= 0		// P-Verstärkung Kaskade			[%/K] * 100
	  0,	//int		Kpr;					= 0		// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	  0,	//int		Anst;					= 0		// Steigung									 		[1] * 100	
	  0,	//int		Tvpa;					= 0		// Niveau											 [°C] * 10		
	  0,	//int		Tvma;					= 0		// max. Vorlauftemp.					 [°C] * 10		
	  0,	//int		Tvmi;					= 0		// min. Vorlauftemp.					 [°C] * 10		
	  0,	//int		Trma;					= 0		// max. Rücklauftemp.					 [°C] * 10		
	  0,	//int		Taha;					= 0		// Temp. am Heizkurvenanfang	 [°C] * 10	
	  0,	//int		Tiso;					= 0		// Raumsolltemp. für Kaskade	 [°C] * 10		
	  0,	//int		Timi;					= 0		// Mindest-Raumtemp.					 [°C] * 10	
	  0,	//int		Tsolu;				= 0		// Start-Sollwert Heizung			 [°C] * 10	
	  0,	//int		Beabs;				= 0		// Bedarfs-Absenk.keine Nutzung [K] * 10	
	  0,	//int		Fzk;					= 0		// Filterzk. f. tsol						[s] * 10
	  0,	//char	Hub;					= 0		// Ventilhub									 [mm]
	  0,	//char	Vst;					= 0		// Stellgeschwindigkeit		 [mm/min]
	  0,	//char	Haut;					= 0		// HAND/AUTO-Umschaltung
	  0,	//char	Puea;					= 0		// Hand: Pumpe	ein/aus
	  0,	//char	Pu2ea;				= 0		// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	  0,	//int		Vstell;				= 0		// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	  0,	//char	Puna;					= 0		// Pumpennachlaufzeit										[min]		
	  0,	//char	Swhk;					= 0		// Schalter Sommer/Winter-Automatik 		ja/nein
	  {0,0},	//char	Wwvh;					= 0		// Schalter WW-Vorrang 									ja/nein
	  0,	//char	Sauf;					= 0		// Schalter Schnellaufheizung 					ja/nein
	  0,	//char	Psbegr;				= 0		// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	  {0,0},	//int	  Kpww[];					= 0		// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	  0,	//char	TabwMax;			= 0		// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	  0,	//char	TabwZeit;			= 0		// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	  0,	//char	FerienAutom;	= 0		// Schalter
	  0,	//UINT	FerienAbsenk;	= 0		// Absenkwert
	  0,	//UINT	SchalterAbsenk= 0		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	  0,	//UINT	Y_rel_min;		= 0		// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	  0,	//UINT	Y_rel_beg;		= 0		// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	  0,	//int		TaFrostgrenze;= 0		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	  0,	//int		TaHeizgrenze;	= 0		// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
		0,	//int		AbschZeitKonst;= 0	// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	  0,	//char 	SoWiExtra;		= 0		// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	  0,	//int		Tae;					= 0		// Temp. Winter ein						 					[°C] * 10	
	  0,	//int		Taa;					= 0		// Temp. Sommer aus						 					[°C] * 10	

	  0,	//char	MessBeginn;		= 0		// Beginn der Adaption, Stunde
	  0,	//char	AdaptWert;		= 0		// [K] * 10
	  0,	//int		EiMaxAdapt;		= 0		// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	  0,	//char 	FuehrPu;			= 0		// Führunspumpe
	  0,	//UINT 	PuWechseln;		= 0		// laufzeitabh. Umschalten der Führungspumpe  [h]
	  0,	//char 	FuehrPuFest;	= 0		// Führunspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	  0,	//char	KaskMode;			= 0		// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	  0,	//int 	TvpAnh;				= 0		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
// Estrich-Programm
		{ 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

	  0,	// TimiHyst;					= 0		// Hysterese Raum-Mindesttemp. [K] * 10
		0,	// XpLeistBegr;				= 0		// [K/% * 100]
	  0,	// KaskBegr;					= 0		// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	  0,	// PuMin							= 0		// Pumpe auf Mindestdrehzahl													

// Pumpenbus
		0,	// BusPuSollwert = 0				// 0...100,0 %
		0,	// BusPuSollwertAbsenk = 0	// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
		0,	//AbschRaumanf;		// Funktion freigegeben ?
		0,	//VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
		0,	//VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
		0,	//VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

		0,	//VorrangZeitabsenkung;
		0,	//PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
		0,	// char VentiloeffngTag;				// Wochentag
		{0,0},	// uhrzeit VentiloeffngBeg;		// Beginn
		{0,0},	// uhrzeit VentiloeffngEnd;		// Ende
		0,	// int VentiloeffngAbs;				// Absenkwert (>= 50,0 K führt zum Abschalten)
		0,	// int GrenztempVentiloeffng;	// untere Grenze der Außentemperatur

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
		0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },
 
 // Profil 1 (Radiatoren, Wohnhaus)
 {
	100,	//int		Ts;						= 10,0		// Tastzeit (>= 1 s)						[s] * 10
 1200,	//UINT	Tn;						= 120,0		// Nachstellzeit								[s] * 10
	200,	//int		Kp;						=  2,00		// P-Verstärkung							[%/K] * 100
	  0,	//int		Kd;						=  0,00		// D-Verstärkung							[%/K] * 100
	300,	//int		Kpk;					=  3,00		// P-Verstärkung Kaskade			[%/K] * 100
	150,	//int		Kpr;					=  1,50		// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	140,	//int		Anst;					=  1,40		// Steigung									 		[1] * 100	
	300,	//int		Tvpa;					= 30,0		// Niveau											 [°C] * 10		
	800,	//int		Tvma;					= 80,0		// max. Vorlauftemp.					 [°C] * 10		
	400,	//int		Tvmi;					= 40,0		// min. Vorlauftemp.					 [°C] * 10		
	990,	//int		Trma;					= 99,0		// max. Rücklauftemp.					 [°C] * 10		
	200,	//int		Taha;					= 20,0		// Temp. am Heizkurvenanfang	 [°C] * 10	
	210,	//int		Tiso;					= 21,0		// Raumsolltemp. für Kaskade	 [°C] * 10		
	150,	//int		Timi;					= 15,0		// Mindest-Raumtemp.					 [°C] * 10	
	400,	//int		Tsolu;				= 40,0		// Start-Sollwert Heizung			 [°C] * 10	
	100,	//int		Beabs;				= 10,0		// Bedarfs-Absenk.keine Nutzung [K] * 10	
	600,	//int		Fzk;					= 60,0		// Filterzk. f. tsol						[s] * 10
	 20,	//char	Hub;					= 20			// Ventilhub									 [mm]
	 10,	//char	Vst;					= 10			// Stellgeschwindigkeit		 [mm/min]
	  0,	//char	Haut;					=  0			// HAND/AUTO-Umschaltung
	  1,	//char	Puea;					=  1			// Hand: Pumpe	ein/aus
	  1,	//char	Pu2ea;				=  1			// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	  0,	//int		Vstell;				=  0,0		// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	  5,	//char	Puna;					=  5			// Pumpennachlaufzeit										[min]		
	  1,	//char	Swhk;					=  1			// Schalter Sommer/Winter-Automatik 		ja/nein
	  {0,0},	//char	Wwvh;					=  1			// Schalter WW-Vorrang 									ja/nein
	  0,	//char	Sauf;					=  0			// Schalter Schnellaufheizung 					ja/nein
	  0,	//char	Psbegr;				=  0			// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	  {0,0},	//int	  Kpww;					=  1,00		// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	100,	//char	TabwMax;			= 10,0		// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	 60,	//char	TabwZeit;			= 60			// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	  0,	//char	FerienAutom;	=  0			// Schalter
	200,	//UINT	FerienAbsenk;	= 20,0		// Absenkwert
	200,	//UINT	SchalterAbsenk= 20,0		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	  0,	//UINT	Y_rel_min;		=  0,0		// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	  0,	//UINT	Y_rel_beg;		=  0,0		// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	 30,	//int		TaFrostgrenze;=  3,0		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	220,	//int		TaHeizgrenze;	= 22,0		// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
	  0,	//int		AbschZeitKonst=  0,0		// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	  0,	//char 	SoWiExtra;		=  0			// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	170,	//int		Tae;					= 17,0		// Temp. Winter ein						 					[°C] * 10	
	180,	//int		Taa;					= 18,0		// Temp. Sommer aus						 					[°C] * 10	

	 18,	//char	MessBeginn;		= 10			// Beginn der Adaption, Stunde
	 10,	//char	AdaptWert;		=  0,0		// [K] * 10
	 30,	//int		EiMaxAdapt;		=  3,0		// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	  0,	//char 	FuehrPu;			=  0		// Führungspumpe
	  0,	//UINT 	PuWechseln;		=  0		// laufzeitabh. Umschalten der Führungspumpe  [h]
	  0,	//char 	FuehrPuFest;	=  0		// Führungspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	  1,	//char	KaskMode;			=  0		// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	  50,	//int 	TvpAnh;				=  0		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
// Estrich-Programm
		{ 0, 1, 1},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

	  5,	// TimiHyst;					= 2,0		// Hysterese Raum-Mindesttemp. [K] * 10
	 50,	// XpLeistBegr;				= 0,50	// Solltemp.-Absenkung [K/% * 100]
	210,	// KaskBegr;					= 21,0	// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	  0,	// PuMin							= 0		// Pumpe auf Mindestdrehzahl													

// Pumpenbus
		800,	// BusPuSollwert = 0				// 0...100,0 %
		500,	// BusPuSollwertAbsenk = 0	// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
		0,	//AbschRaumanf;		// Funktion freigegeben ?
		0,	//VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
		0,	//VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
	990,	//VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

		0,	//VorrangZeitabsenkung;
		2,	//PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
		0x20,		// char VentiloeffngTag;				// Wochentag Freitag
		{50,9},		// uhrzeit VentiloeffngBeg;			// Beginn 9:50
		{25,10},	// uhrzeit VentiloeffngEnd;			// Ende 10:25
		0,				// int VentiloeffngAbs;					// Absenkwert (>= 50,0 K führt zum Abschalten)
		-10,			// int GrenztempVentiloeffng;		// untere Grenze der Außentemperatur, -1,0°C

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
		0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },

 
 // Profil 2 (Radiatoren, Schulen/Büros/Gewerbe)
 {
	100,	//int		Ts;						= 10,0		// Tastzeit (>= 1 s)						[s] * 10
 1200,	//UINT	Tn;						= 120,0		// Nachstellzeit								[s] * 10
	200,	//int		Kp;						=  2,00		// P-Verstärkung							[%/K] * 100
	  0,	//int		Kd;						=  0,00		// D-Verstärkung							[%/K] * 100
	400,	//int		Kpk;					=  3,00		// P-Verstärkung Kaskade			[%/K] * 100
	150,	//int		Kpr;					=  1,50		// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	140,	//int		Anst;					=  1,40		// Steigung									 		[1] * 100	
	250,	//int		Tvpa;					= 25,0		// Niveau											 [°C] * 10		
	750,	//int		Tvma;					= 75,0		// max. Vorlauftemp.					 [°C] * 10		
	350,	//int		Tvmi;					= 35,0		// min. Vorlauftemp.					 [°C] * 10		
	990,	//int		Trma;					= 99,0		// max. Rücklauftemp.					 [°C] * 10		
	200,	//int		Taha;					= 20,0		// Temp. am Heizkurvenanfang	 [°C] * 10	
	210,	//int		Tiso;					= 21,0		// Raumsolltemp. für Kaskade	 [°C] * 10		
	150,	//int		Timi;					= 15,0		// Mindest-Raumtemp.					 [°C] * 10	
	400,	//int		Tsolu;				= 40,0		// Start-Sollwert Heizung			 [°C] * 10	
	150,	//int		Beabs;				= 15,0		// Bedarfs-Absenk.keine Nutzung [K] * 10	
	600,	//int		Fzk;					= 60,0		// Filterzk. f. tsol						[s] * 10
	 20,	//char	Hub;					= 20			// Ventilhub									 [mm]
	 10,	//char	Vst;					= 10			// Stellgeschwindigkeit		 [mm/min]
	  0,	//char	Haut;					=  0			// HAND/AUTO-Umschaltung
	  1,	//char	Puea;					=  1			// Hand: Pumpe	ein/aus
	  1,	//char	Pu2ea;				=  1			// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	  0,	//int		Vstell;				=  0,0		// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	  5,	//char	Puna;					=  5			// Pumpennachlaufzeit										[min]		
	  1,	//char	Swhk;					=  1			// Schalter Sommer/Winter-Automatik 		ja/nein
	  {0,0},	//char	Wwvh;					=  1			// Schalter WW-Vorrang 									ja/nein
	  0,	//char	Sauf;					=  0			// Schalter Schnellaufheizung 					ja/nein
	  0,	//char	Psbegr;				=  0			// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	  {0,0},	//int	  Kpww;					=  1,00		// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	100,	//char	TabwMax;			= 10,0		// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	 60,	//char	TabwZeit;			= 60			// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	  0,	//char	FerienAutom;	=  0			// Schalter
	200,	//UINT	FerienAbsenk;	= 20,0		// Absenkwert
	200,	//UINT	SchalterAbsenk= 20,0		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	  0,	//UINT	Y_rel_min;		=  0,0		// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	  0,	//UINT	Y_rel_beg;		=  0,0		// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	 30,	//int		TaFrostgrenze;=  3,0		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	220,	//int		TaHeizgrenze;	= 22,0		// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
	 10,	//int		AbschZeitKonst=  1,0		// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	  0,	//char 	SoWiExtra;		=  0			// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	160,	//int		Tae;					= 17,0		// Temp. Winter ein						 					[°C] * 10	
	170,	//int		Taa;					= 18,0		// Temp. Sommer aus						 					[°C] * 10	

	  8,	//char	MessBeginn;		=  8			// Beginn der Adaption, Stunde
	 10,	//char	AdaptWert;		=  0,0		// [K] * 10
	 30,	//int		EiMaxAdapt;		=  3,0		// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	  0,	//char 	FuehrPu;			=  0		// Führungspumpe
	  0,	//UINT 	PuWechseln;		=  0		// laufzeitabh. Umschalten der Führungspumpe  [h]
	  0,	//char 	FuehrPuFest;	=  0		// Führungspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	  0,	//char	KaskMode;			=  0		// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	  0,	//int 	TvpAnh;				=  0		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
// Estrich-Programm
		{ 0, 1, 1},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

	  5,	// TimiHyst;					= 2,0		// Hysterese Raum-Mindesttemp. [K] * 10
		0,	// XpLeistBegr;				= 0			// [K/% * 100]
	210,	// KaskBegr;					= 21,0	// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	  0,	// PuMin							= 0		// Pumpe auf Mindestdrehzahl													

// Pumpenbus
		800,	// BusPuSollwert = 0				// 0...100,0 %
		500,	// BusPuSollwertAbsenk = 0	// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
		0,	//AbschRaumanf;		// Funktion freigegeben ?
		0,	//VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
		0,	//VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
	990,	//VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

		0,	//VorrangZeitabsenkung;
		2,	//PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
		0x20,		// char VentiloeffngTag;				// Wochentag Freitag
		{50,9},		// uhrzeit VentiloeffngBeg;			// Beginn 9:50
		{25,10},	// uhrzeit VentiloeffngEnd;			// Ende 10:25
		0,				// int VentiloeffngAbs;					// Absenkwert (>= 50,0 K führt zum Abschalten)
		-10,			// int GrenztempVentiloeffng;		// untere Grenze der Außentemperatur, -1,0°C

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
		0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },
 
 // Profil 3 (Fußboden)
 {
	100,	//int		Ts;						= 10,0		// Tastzeit (>= 1 s)						[s] * 10
 1200,	//UINT	Tn;						= 120,0		// Nachstellzeit								[s] * 10
	250,	//int		Kp;						=  2,50		// P-Verstärkung							[%/K] * 100
	  0,	//int		Kd;						=  0,00		// D-Verstärkung							[%/K] * 100
	300,	//int		Kpk;					=  3,00		// P-Verstärkung Kaskade			[%/K] * 100
	150,	//int		Kpr;					=  1,50		// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	 80,	//int		Anst;					=  0,80		// Steigung									 		[1] * 100	
	200,	//int		Tvpa;					= 20,0		// Niveau											 [°C] * 10		
	500,	//int		Tvma;					= 50,0		// max. Vorlauftemp.					 [°C] * 10		
	250,	//int		Tvmi;					= 25,0		// min. Vorlauftemp.					 [°C] * 10		
	990,	//int		Trma;					= 99,0		// max. Rücklauftemp.					 [°C] * 10		
	200,	//int		Taha;					= 20,0		// Temp. am Heizkurvenanfang	 [°C] * 10	
	210,	//int		Tiso;					= 21,0		// Raumsolltemp. für Kaskade	 [°C] * 10		
	150,	//int		Timi;					= 15,0		// Mindest-Raumtemp.					 [°C] * 10	
	300,	//int		Tsolu;				= 30,0		// Start-Sollwert Heizung			 [°C] * 10	
	 50,	//int		Beabs;				=  5,0		// Bedarfs-Absenk.keine Nutzung [K] * 10	
	600,	//int		Fzk;					= 60,0		// Filterzk. f. tsol						[s] * 10
	 20,	//char	Hub;					= 20			// Ventilhub									 [mm]
	 10,	//char	Vst;					= 10			// Stellgeschwindigkeit		 [mm/min]
	  0,	//char	Haut;					=  0			// HAND/AUTO-Umschaltung
	  1,	//char	Puea;					=  1			// Hand: Pumpe	ein/aus
	  1,	//char	Pu2ea;				=  1			// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	  0,	//int		Vstell;				=  0,0		// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	  5,	//char	Puna;					=  5			// Pumpennachlaufzeit										[min]		
	  1,	//char	Swhk;					=  1			// Schalter Sommer/Winter-Automatik 		ja/nein
	  {0,0},	//char	Wwvh;					=  1			// Schalter WW-Vorrang 									ja/nein
	  0,	//char	Sauf;					=  0			// Schalter Schnellaufheizung 					ja/nein
	  0,	//char	Psbegr;				=  0			// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	  {0,0},	//int	  Kpww;					=  1,00		// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	 50,	//char	TabwMax;			=  5,0		// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	 60,	//char	TabwZeit;			= 60			// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	  0,	//char	FerienAutom;	=  0			// Schalter
	100,	//UINT	FerienAbsenk;	= 10,0		// Absenkwert
	100,	//UINT	SchalterAbsenk= 10,0		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	  0,	//UINT	Y_rel_min;		=  0,0		// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	  0,	//UINT	Y_rel_beg;		=  0,0		// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	 30,	//int		TaFrostgrenze;=  3,0		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	220,	//int		TaHeizgrenze;	= 22,0		// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
	 10,	//int		AbschZeitKonst=  1,0		// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	  0,	//char 	SoWiExtra;		=  0			// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	160,	//int		Tae;					= 17,0		// Temp. Winter ein						 					[°C] * 10	
	170,	//int		Taa;					= 18,0		// Temp. Sommer aus						 					[°C] * 10	

	 10,	//char	MessBeginn;		= 10			// Beginn der Adaption, Stunde
	  0,	//char	AdaptWert;		=  0,0		// [K] * 10
	 30,	//int		EiMaxAdapt;		=  3,0		// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	  0,	//char 	FuehrPu;			=  0		// Führungspumpe
	  0,	//UINT 	PuWechseln;		=  0		// laufzeitabh. Umschalten der Führungspumpe  [h]
	  0,	//char 	FuehrPuFest;	=  0		// Führungspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	  0,	//char	KaskMode;			=  0		// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	  0,	//int 	TvpAnh;				=  0		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
// Estrich-Programm
		{ 0, 1, 1},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

	  5,	// TimiHyst;					= 2,0		// Hysterese Raum-Mindesttemp. [K] * 10
 	  0,	// LeistBegrTs;				= 0 		// Ventil schliesst zur Leistungsbegrenzung um LeistBegrTs/Ts [%]
	210,	// KaskBegr;					= 21,0	// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	  0,	// PuMin							= 0		// Pumpe auf Mindestdrehzahl													

// Pumpenbus
		800,	// BusPuSollwert = 0				// 0...100,0 %
		500,	// BusPuSollwertAbsenk = 0	// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
		0,	//AbschRaumanf;		// Funktion freigegeben ?
		0,	//VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
		0,	//VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
	990,	//VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

		0,	//VorrangZeitabsenkung;
		2,	//PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
		0,	// char VentiloeffngTag;				// Wochentag
		{0,0},	// uhrzeit VentiloeffngBeg;		// Beginn
		{0,0},	// uhrzeit VentiloeffngEnd;		// Ende
		0,	// int VentiloeffngAbs;				// Absenkwert (>= 50,0 K führt zum Abschalten)
		0,	// int GrenztempVentiloeffng;	// untere Grenze der Außentemperatur

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
		0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },

 
 // Profil 4 (Zubringer Lüftung)
 {
	100,	//int		Ts;						= 10,0		// Tastzeit (>= 1 s)						[s] * 10
 1200,	//UINT	Tn;						= 120,0		// Nachstellzeit								[s] * 10
	200,	//int		Kp;						=  2,00		// P-Verstärkung							[%/K] * 100
	  0,	//int		Kd;						=  0,00		// D-Verstärkung							[%/K] * 100
	  0,	//int		Kpk;					=  0,00		// P-Verstärkung Kaskade			[%/K] * 100
	150,	//int		Kpr;					=  1,50		// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	 80,	//int		Anst;					=  0,80		// Steigung									 		[1] * 100	
	450,	//int		Tvpa;					= 20,0		// Niveau											 [°C] * 10		
	750,	//int		Tvma;					= 75,0		// max. Vorlauftemp.					 [°C] * 10		
	500,	//int		Tvmi;					= 50,0		// min. Vorlauftemp.					 [°C] * 10		
	990,	//int		Trma;					= 99,0		// max. Rücklauftemp.					 [°C] * 10		
	200,	//int		Taha;					= 20,0		// Temp. am Heizkurvenanfang	 [°C] * 10	
	210,	//int		Tiso;					= 21,0		// Raumsolltemp. für Kaskade	 [°C] * 10		
	150,	//int		Timi;					= 15,0		// Mindest-Raumtemp.					 [°C] * 10	
	300,	//int		Tsolu;				= 30,0		// Start-Sollwert Heizung			 [°C] * 10	
	  0,	//int		Beabs;				=  0,0		// Bedarfs-Absenk.keine Nutzung [K] * 10	
	600,	//int		Fzk;					= 60,0		// Filterzk. f. tsol						[s] * 10
	 20,	//char	Hub;					= 20			// Ventilhub									 [mm]
	 10,	//char	Vst;					= 10			// Stellgeschwindigkeit		 [mm/min]
	  0,	//char	Haut;					=  0			// HAND/AUTO-Umschaltung
	  1,	//char	Puea;					=  1			// Hand: Pumpe	ein/aus
	  1,	//char	Pu2ea;				=  1			// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	  0,	//int		Vstell;				=  0,0		// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	  5,	//char	Puna;					=  5			// Pumpennachlaufzeit										[min]		
	  1,	//char	Swhk;					=  1			// Schalter Sommer/Winter-Automatik 		ja/nein
	  {0,0},	//char	Wwvh;					=  1			// Schalter WW-Vorrang 									ja/nein
	  0,	//char	Sauf;					=  0			// Schalter Schnellaufheizung 					ja/nein
	  0,	//char	Psbegr;				=  0			// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	  {0,0},	//int	  Kpww;					=  1,00		// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	100,	//char	TabwMax;			= 10,0		// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	 60,	//char	TabwZeit;			= 60			// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	  0,	//char	FerienAutom;	=  0			// Schalter
	100,	//UINT	FerienAbsenk;	= 10,0		// Absenkwert
	100,	//UINT	SchalterAbsenk= 10,0		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	  0,	//UINT	Y_rel_min;		=  0,0		// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	  0,	//UINT	Y_rel_beg;		=  0,0		// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	 30,	//int		TaFrostgrenze;=  3,0		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	220,	//int		TaHeizgrenze;	= 22,0		// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
	  0,	//int		AbschZeitKonst=  0,0		// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	  0,	//char 	SoWiExtra;		=  0			// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	160,	//int		Tae;					= 17,0		// Temp. Winter ein						 					[°C] * 10	
	170,	//int		Taa;					= 18,0		// Temp. Sommer aus						 					[°C] * 10	

	 10,	//char	MessBeginn;		= 10			// Beginn der Adaption, Stunde
	  0,	//char	AdaptWert;		=  0,0		// [K] * 10
	 30,	//int		EiMaxAdapt;		=  3,0		// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	  0,	//char 	FuehrPu;			=  0		// Führungspumpe
	  0,	//UINT 	PuWechseln;		=  0		// laufzeitabh. Umschalten der Führungspumpe  [h]
	  0,	//char 	FuehrPuFest;	=  0		// Führungspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	  0,	//char	KaskMode;			=  0		// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	  0,	//int 	TvpAnh;				=  0		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
// Estrich-Programm
		{ 0, 1, 1},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

	  5,	// TimiHyst;					= 2,0		// Hysterese Raum-Mindesttemp. [K] * 10
		0,	// XpLeistBegr;				= 0			// [K/% * 100]
	210,	// KaskBegr;					= 21,0	// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	  0,	// PuMin							= 0		// Pumpe auf Mindestdrehzahl													

// Pumpenbus
		800,	// BusPuSollwert = 0				// 0...100,0 %
		500,	// BusPuSollwertAbsenk = 0	// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
		0,	//AbschRaumanf;		// Funktion freigegeben ?
		0,	//VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
		0,	//VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
	990,	//VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

		0,	//VorrangZeitabsenkung;
		2,	//PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
		0,	// char VentiloeffngTag;				// Wochentag
		{0,0},	// uhrzeit VentiloeffngBeg;		// Beginn
		{0,0},	// uhrzeit VentiloeffngEnd;		// Ende
		0,	// int VentiloeffngAbs;				// Absenkwert (>= 50,0 K führt zum Abschalten)
		0,	// int GrenztempVentiloeffng;	// untere Grenze der Außentemperatur

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
		0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },

}; 	
const char HK_PROFILE = sizeof ( Hk_Standparam ) / sizeof ( HkStandard ); 	


/*----------------------------------------------------------------------------*/
/* 						Liste von Absenktabellen für verschiedene Heizkreise						*/
/*----------------------------------------------------------------------------*/
const absenktab abs_standard[][8] = {
// begin[min], begin[h], dauer[h], absenktemp[0.1 K]
	
	{ // Absenkprofil 0 (dummy)
		{0,   0,	0,	  0},		// Alle Tage
		{0,   0,	0,	  0},		// Mo
		{0,   0,	0,	  0},		// Di
		{0,   0,	0,	  0},		// Mi
		{0,   0,	0,	  0},		// Do
		{0,   0,	0,	  0},		// Fr
		{0,   0	,	0,	  0},		// Sa
		{0,	  0,	0,	  0},		// So
	},
	
	{ // Absenkprofil 1: Radiatoren Wohnhaus
		{0,  23,	6,	100},		// Alle Tage
		{0,  23,	6,	100},		// Mo
		{0,  23,	6,	100},		// Di
		{0,  23,	6,	100},		// Mi
		{0,  23,	6,	100},		// Do
		{59, 23,	6,	100},		// Fr
		{59, 23	,	6,	100},		// Sa
		{0,	 23,	6,	100},		// So
	},
	
	{ // Absenkprofil 2: FBH Wohnhaus
		{0,  23,	6,	 50},		// Alle Tage
		{0,  23,	6,	 50},		// Mo
		{0,  23,	6,	 50},		// Di
		{0,  23,	6,	 50},		// Mi
		{0,  23,	6,	 50},		// Do
		{59, 23,	6,	 50},		// Fr
		{59, 23	,	6,	 50},		// Sa
		{0,	 23,	6,	 50},		// So
	},

	{ // Absenkprofil 3: Radiatoren Schulen/Büros/Gewerbe
		{0,  18, 12,	150},		// Alle Tage
		{0,  18, 12,	150},		// Mo
		{0,  18, 12,	150},		// Di
		{0,  18, 12,	150},		// Mi
		{0,  18, 12,	150},		// Do
		{0,  15, 24,	150},		// Fr
		{0,  15, 24,	150},		// Sa
		{0,	 15, 14,	150},		// So
	},

	{ // Absenkprofil 4: FBH Schulen/Büros/Gewerbe
		{0,  18, 12,	 50},		// Alle Tage
		{0,  18, 12,	 50},		// Mo
		{0,  18, 12,	 50},		// Di
		{0,  18, 12,	 50},		// Mi
		{0,  18, 12,	 50},		// Do
		{0,  15, 24,	 50},		// Fr
		{0,  15, 24,	 50},		// Sa
		{0,	 15, 14,	 50},		// So
	},

	
	{ // Absenkprofil 5: Turnhalle
		{0,  21, 11,	200},		// Alle Tage
		{0,  21, 11,	200},		// Mo
		{0,  21, 11,	200},		// Di
		{0,  21, 11,	200},		// Mi
		{0,  21, 11,	200},		// Do
		{0,  21, 11,	200},		// Fr
		{0,  21, 11,	200},		// Sa
		{0,	 21, 11,	200},		// So
	},               
	
	{ // Absenkprofil 6: Kita
		{0,  20, 10,	150},		// Alle Tage
		{0,  20, 10,	150},		// Mo
		{0,  20, 10,	150},		// Di
		{0,  20, 10,	150},		// Mi
		{0,  20, 10,	150},		// Do
		{0,  20, 12,	150},		// Fr
		{0,  12, 20,	150},		// Sa
		{0,	 12, 18,	150},		// So
	},

	{ // Absenkprofil 7: alles aus
		{0,   0,	0,	  0},		// Alle Tage
		{0,   0,	0,	  0},		// Mo
		{0,   0,	0,	  0},		// Di
		{0,   0,	0,	  0},		// Mi
		{0,   0,	0,	  0},		// Do
		{0,   0,	0,	  0},		// Fr
		{0,   0	,	0,	  0},		// Sa
		{0,	  0,	0,	  0},		// So
	},

	{ // Absenkprofil 8: degewo Wohnhaus
		{0,  23,	5,	 50},		// Alle Tage
		{0,  23,	5,	 50},		// Mo
		{0,  23,	5,	 50},		// Di
		{0,  23,	5,	 50},		// Mi
		{0,  23,	5,	 50},		// Do
		{0,  23,	5,	 50},		// Fr
		{0,  23	,	5,	 50},		// Sa
		{0,	 23,	5,	 50},		// So
	},
	
};	
const char ABS_PROFILE =  ( sizeof ( abs_standard ) / 8 ) / sizeof ( absenktab ) ;

/*----------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für verschiedene Kesselkreise			*/
/*----------------------------------------------------------------------------*/


const KeStandard  Ke_Standparam[] = {
 // Profil 0 (dummy)
 {
	0, 		// ULONG FolgeSoll; 

	0, 		// FuehrKes = 0	  Führunskessel
	0, 		// KesWechseln = 0		laufzeitabh. Umschalten des Führungskessels  [h]
	0, 		// KesSchaltDiff = 0	Schaltdiffernz zum Abschalten des Kessels	
	0, 		// FuehrKesFest = 0 	Führunskessel fest vorgegeben, wenn ungestoert	
 	
	0,		// Tvma	=  0,0		max. Kesseltemp.							[°C] * 10
	0,		// Tvmi =  0,0		min. Kesseltemp.							[°C] * 10
	0,		// Ttaup=  0,0		Kessel-Taupunkttemp.		 			[°C] * 10
	0,		// Tvpa =  0,0		Kesselanhebung								[°C] * 10
	0,		// Einkrit1 = 0,0	Einschaltkriterium				 [K*min] * 10
	0,		// Auskrit1 = 0,0	Ausschaltkriterium				 [K*min] * 10
	0,		// Einkrit2 = 0,0	Einschaltkriterium				 [K*min] * 10
	0,		// Auskrit2 = 0,0	Ausschaltkriterium				 [K*min] * 10	
//-	0,		// Kemin	 = 0		Kesselmindestlaufzeit				 [min]	
//-	0,		// Keanf   = 0		Kesselanfahrzeit						 [min]
//-	0,		// Ta2ein  = 0		Außentemp.grenze Ke2 ein 			[°C] * 10
	0,		// TvsolX2;				Solltemperatur Max						[°C] * 10
	0,		// SollY2;				Sollspannung Max							 [V] * 10
	0,		// TvsolX1;				Solltemperatur Min						[°C] * 10
	0,		// SollY1;				Sollspannung Min							 [V] * 10

//ulsch	
	0, 		// Haut	= 0				Hand/Automatik
//-	0, 		// HautVent	= 0		Hand/Automatik Ventil	
	0, 		// Kesea					Hand Kesselfreigabe ein/aus		
	0, 		// Kestufe2 = 0		Hand Kesselstufe 2  ein/aus
	0,  	// Kestell = 0		Hand Führungswert 0-10,0 Volt
	0, 		// Puea = 0				Hand: Pumpe	ein/aus	
	0,		// Puna = 0				Pumpennachlaufzeit [min]	
	
	0,		// Ts							Tastzeit (>= 1 s)							 [s] * 10
	0,		// Tn							Nachstellzeit									 [s] * 10
	0,		// Kp							P-Verstärkung								 [%/K] * 100
	0,		// Kd							D-Verstärkung								 [%/K] * 100	
	0,		// Stellzeit																		 [s]

	0, 		// Ts_rl					Tastzeit (>= 1 s)							 [s] * 10
	0,		// Tn_rl;					Nachstellzeit									 [s] * 10
	0, 		// Kp_rl;					P-Verstärkung								 [%/K] * 100
	0, 		// Kd_rl;					D-Verstärkung								 [%/K] * 100	
	0,		// Stellzeit_rl																		 [s]

	0,		// KeBm_verz			Verzögerung Kesselbetriebsmeldung  [min]
	
	0,		// Ts_anh					Tastzeit (>= 1 s)							 [s] * 10
	0,		// Tn_anh					Nachstellzeit									 [s] * 10
	0,		// Kp_anh					P-Verstärkung								 [%/K] * 100
	0,		// Kd_anh					D-Verstärkung								 [%/K] * 100	
	
	0, 		// Kesklaz = 0 				Hand Kessel-Klappe	
	0, 		// HautKes = 0 				Hand/Auto Gruppe "KES"
	0, 		// HPuea = 0 					Hand Hauptpumpe ein/aus	
	0, 		// SSFdauer = 0 			Schornsteinfeger-Funktion [min]		
	0, 		// FolgeKesAus = 0 		Abschalten des Folge-Kessels, wenn VL - RL des Primär-Kessel zu klein [K] * 10 	
	0, 		// FolgeKesAusVerz=0	Verzögerung des Abschaltens des Folgekessels bei Unterschreitung VL-RL [min]		

	0, 		// TaFuehrKesUmschalten = 0 Führunskes.ist abhängig von Aussentemp. (keine Laufzeitabh., KesWechseln = 0) [°C] * 10
	0, 		// FuehrKesTaPlus = 0 			Führungskessel bei Ta > TaFuehrKesUmschalten;
	0, 		// FuehrKesTaMinus = 0 			Führungskessel bei Ta < TaFuehrKesUmschalten;			

	0, 		// Kestell_begr = 0 				Hand Führungswert 0-10,0 Volt

//-	// Pumpe regeln
//-	0,		// HautPu = 0
//-	0,		// TsPu	= 0				Tastzeit (>= 1 s)						[s] * 10
//-	0,		// TnPu	= 0				Nachstellzeit								[s] * 10
//-	0,		// KpPu	= 0				P-Verstärkung							[%/K] * 100
//-	0,		// KdPu	= 0				D-Verstärkung							[%/K] * 100	
//-	0,		// PuStell = 0		Hand: Pumpe stellen 0-10V   [%] * 10	
//-	0,		// SwPu20u = 0		Sollwert Pu-Regel. -20 °C	  [K] * 10
//-	0,		// SwPu20o = 0		Sollwert Pu-Regel. +20 °C	  [K] * 10
//-	0,		// DzPuMin;			// Mindest-Drehzahl der Pumpe  [%] * 10
//-	0, 		// UINT	TdiffMin;
//-	0, 		// UINT	PuDzMax;
//-	0, 		// UINT	TdiffMax;
//-	0, 		// UINT	PuDzMin;		
	
	0,		// ModMin = 0,0		min.Stellgröße der Modulation [%]*10
	0,		// Blockierschutz;

	0,		// Sperre;
	0,		// SchaltpauseZu;
	0,		// SchaltpauseAb;
	0,		// VlVerteilerMax;
//-	0,		// Anfahrbetrieb;
	0,		// PuAbschalt;

	0,		// TvsbErsatz;
	0,		// Modulation;
	0,		// FuehlerSSTM;				// Fühlerfehler setzt Ausgang SSTM
	0,		// KesNachlauf;				// min

//-	0, 		// ULONG FolgeSommer; 
//-	0, 		// ULONG FolgeWinter; 

	0, 		// TvMax;							// max. Vorlauftemp.					 			[°C] * 10

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,		// Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	0,		// Klna = 0				Klappennachlaufzeit [min]	
	0,		// Wup_rl;						// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	0,		// char RlAnheb;		// RlAnheb = 0/1 -> Begrenzung/Anhebung

	// die folgenden 4 Parameter definieren eine Gerade zur Steuerung der Drehzahl der Pumpe in Abhängigkeit von der Modulation
	0,		// int	 P1ModDz;
	0,		// int	 P1Dz;
	0,		// int	 P2ModDz;
	0,		// int	 P2Dz;
	0,		// int	 DzPuHand;

	0,		// char PuBm_verz;	// Verzögerung Kesselbetriebsmeldung  [min]

	0,		// KesselAusMod;			//	[%] * 10
	0,		// KesselAusModVerz;	// min
	0,		// KesselEinMod;			//	[%] * 10
	0,		// KesselEinModVerz;	// min

	0,		// int AnhebMax;		

	0,		// char ModVerzSomm;
	0,		// char ModVerzWint;

	0,		// TabwMax = 0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	0,		// TabwZeit= 0			Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]	
	
 },
 
 // Profil 1 
 {
	12,	// FolgeSoll;

	1, 		// FuehrKes = 0	  Führunskessel
	100, 	// KesWechseln = 100		laufzeitabh. Umschalten des Führungskessels [h]
	100, 	// KesSchaltDiff = 0	Schaltdifferenz zum Abschalten des Kessels		[K] * 10
	1, 		// FuehrKesFest = 0 	Führunskessel fest vorgegeben, wenn ungestoert				
 	
	850,	// Tvma		= 85,0	max. Kesseltemp.							[°C] * 10
	200,	// Tvmi 	= 60,0	min. Kesseltemp.							[°C] * 10
//-	400,	// Tvmi 	= 40,0	min. Kesseltemp.							[°C] * 10			Brennwertkessel
	400,	// Ttaup	= 40,0	Kessel-Taupunkttemp.		 			[°C] * 10
	50,		// Tvpa 	=  5,0	Kesselanhebung								[°C] * 10
	1000,	// Einkrit1=100,0	Einschaltkriterium				[K*min] * 10
	100,	// Auskrit1= 50,0	Ausschaltkriterium				[K*min] * 10
	200,	// Einkrit1= 20,0	Einschaltkriterium				[K*min] * 10
	200,	// Auskrit2= 20,0	Ausschaltkriterium				[K*min] * 10	
//-	5,		// Kemin  =  5		Kesselmindestlaufzeit				 [min]	
//-	15,		// Keanf  = 30		Kesselanfahrzeit						 [min]
//-	100,	// Ta2ein = 10,0	Außentemp.grenze Ke2 ein 			[°C] * 10
	900,	// TvsolX2;				Solltemperatur Max						[°C] * 10
	100,	// SollY2;				Sollspannung Max							 [V] * 10
	200,	// TvsolX1;				Solltemperatur Min						[°C] * 10
	20,		// SollY1;				Sollspannung Min							 [V] * 10

//ulsch	
	0, 		// Haut	= 0				Hand/Automatik
//-	0, 		// HautVent	= 0		Hand/Automatik Ventil		
	0, 		// Kesea					Hand Kesselfreigabe ein/aus		
	0, 		// Kestufe2 = 0		Hand Kesselstufe 2  ein/aus
	0,  	// Kestell = 0		Hand Führungswert 0-10,0 Volt
	0, 		// Puea = 0				Hand: Pumpe	ein/aus	
	5,		// Puna = 5				Pumpennachlaufzeit [min]		
	100,	// Ts 	= 10,0 		Tastzeit (>= 1 s)							[s] * 10
	1200,	// Tn 	= 50,0 		Nachstellzeit									[s] * 10
	250,	// Kp 	=  2,00		P-Verstärkung								[%/K] * 100
	0,		// Kd							D-Verstärkung								[%/K] * 100	
	30,		// Stellzeit																		 [s]

	100, 	// Ts_rl = 10,0	Tastzeit (>= 1 s)							 [s] * 10
	500,	// Tn_rl = 50,0	Nachstellzeit									 [s] * 10
	200, 	// Kp_rlr = 2,00	P-Verstärkung								 [%/K] * 100
	0, 		// Kd_rl;				D-Verstärkung								 [%/K] * 100	
	30,		// Stellzeit_rl																		 [s]

	1,		// KeBm_verz			Verzögerung Kesselbetriebsmeldung  [min]
	
	10,		// Ts_anh = 1,0		Tastzeit (>= 1 s)							 [s] * 10
	12000,	// Tn_anh	= 1200,0 Nachstellzeit									 [s] * 10
	100,	// Kp_anh = 1,00	P-Verstärkung								 [%/K] * 100
	0,		// Kd_anh					D-Verstärkung								 [%/K] * 100	
	
	0, 		// Kesklaz;				 		Hand Kessel-Klappe		
	0, 		// HautKes = 0 				Hand/Auto Gruppe "KES"
	0, 		// HPuea = 0 					Hand Hauptpumpe ein/aus	
	15, 	// SSFdauer = 60 			Schornsteinfeger-Funktion [min]	
	70, 	// FolgeKesAus = 7,0 	Abschalten des Folge-Kessels, wenn VL - RL des Primär-Kessel zu klein [K] * 10 	
	2, 		// FolgeKesAusVerz=0	Verzögerung des Abschaltens des Folgekessels bei Unterschreitung VL-RL [min]		

	10, 	// TaFuehrKesUmschalten=0,0 Führunskes.ist abhängig von Aussentemp. (keine Laufzeitabh., KesWechseln = 0) [°C] * 10
	1, 		// FuehrKesTaPlus = 0 			Führungskessel bei Ta > TaFuehrKesUmschalten;
	1, 		// FuehrKesTaMinus = 0 			Führungskessel bei Ta < TaFuehrKesUmschalten;			

	0, 		// Kestell_rl = 0 				Hand Führungswert 0-10,0 Volt

//-	// Pumpe regeln
//-	0,		// HautPu = 0
//-	100,	// TsPu	= 10,0		Tastzeit (>= 1 s)						[s] * 10
//-	1200, // TnPu	= 1800,0	Nachstellzeit								[s] * 10
//-	1000,	// KpPu	= 5,00		P-Verstärkung							[%/K] * 100
//-	0,		// KdPu	= 0				D-Verstärkung							[%/K] * 100	
//-	0,		// PuStell = 0		Hand: Pumpe stellen 0-10V   [%] * 10	
//-	200,	// SwPu20u = 12,0	Sollwert Pu-Regel. -20 °C	  [K] * 10
//-	300,	// SwPu20o = 4,0	Sollwert Pu-Regel. +20 °C	  [K] * 10
//-	300,	// DzPuMin = 30,0	Mindest-Drehzahl der Pumpe  [%] * 10
//-	100,	// 	UINT	TdiffMin;
//-	600,	// 	UINT	PuDzMax;
//-	500,	// 	UINT	TdiffMax;
//-	400,	// 	UINT	PuDzMin; 
//-	
	0,		// ModMin = 2,0	min.Stellausgabe der Modulation [V] * 10

	1,		// Blockierschutz;
	0,		// Sperre;
	5,		// SchaltpauseZu;	[min]
	5,		// SchaltpauseAb;	[min]
	950,	// VlVerteilerMax;[°C] * 10
//-	0,		// Anfahrbetrieb;	
	600,	// 	int	 PuAbschalt;

	200,	// TvsbErsatz;
	0,		// Modulation;
	0,		// FuehlerSSTM;				// Fühlerfehler setzt Ausgang SSTM
	0,		// KesNachlauf;				// min

//-	0,		// FolgeSommer;
//-	0,		// FolgeWinter;

	900, 	// TvMax;							// max. Vorlauftemp.					 			[°C] * 10

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,	// int		Wup;								// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	5,		// Klna = 5				Klappennachlaufzeit [min]	
	0,		// Wup_rl;									// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	0,		// char RlAnheb;		// RlAnheb = 0/1 -> Begrenzung/Anhebung

	// die folgenden 4 Parameter definieren eine Gerade zur Steuerung der Drehzahl der Pumpe in Abhängigkeit von der Modulation
	200,	// int	 P1ModDz;		20,0 %
	700,	// int	 P1Dz;			70,0 %
	800,	// int	 P2ModDz;		80,0 %
	300,	// int	 P2Dz;			30,0 %
	0,		// int	 DzPuHand;

	2,		// char PuBm_verz;	// Verzögerung Kesselbetriebsmeldung  [min]

	200,	// KesselAusMod;				[%] * 10
	10,		// KesselAusModVerz;		min
	800,	// KesselEinMod;				[%] * 10
	5,		// KesselEinModVerz;		min

	50,		// int AnhebMax;		

	0,		// char ModVerzSomm;
	0,		// char ModVerzWint;

	200,	// TabwMax = 0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	60,		// TabwZeit= 0			Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]	
	
 },

}; 
const char KE_PROFILE = sizeof ( Ke_Standparam ) / sizeof ( KeStandard );


/*----------------------------------------------------------------------------*/
/*						Standardwerten für Wärmepumpe			*/
/*----------------------------------------------------------------------------*/
const WpStandard  Wp_Standparam[] = {
 // Profil 0 (dummy)
 {
 	0,	//char	Para_Manu_Sollwert; 		// Manuellen Sollwert aktivieren
	0,	//UNINT	t_manu_Sollwert; 		// Manuell vorgegebener Sollwert in [?C] *10
	0,	//UINT	T_Sollwert_Offset; 		// Sollwertoffset in [K] *10
	0,	//UINT	T_Ersatz_Sollwert; 		// Ersatz-Sollwert in [°C] *10
	0,	//UINT	iPa_T_Sollwert_IN_MIN; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten minimalen Wert [°C] *10
	0,	//UINT	iPa_T_Sollwert_IN_MAX; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten minimalen Wert [°C] *10
	0,  //char	WPU_Freigabe_Haut;						// Manueller Betrieb aktivieren zur Steuerung der WPU-Freigabe
	0,  //char	WPU_Freigabe_Hand_stellen;		// Steuerung der WPU-Freigabe im manuellen Betrieb
	0,	//UINT	intPa_Quellentemperaturminimum;	// Minimale Quellentemperatur in [°C] *10
  0, 	//UINT	intPa_Speicherminimum;					// Minimale Speichertemperatur in [°C] *10
	0,	// int	intPa_T_Speicherladung_on; 			// Speicherladung aktivieren: delat T in [K] *10
  0,	// int	intPa_T_Speicherladung_off; 		// Speicherladung deaktivieren: delat T in [K] *10
	0,	//char chPa_Mindestlaufzeit_min;				// Mindestlaufzeit der WPu in [min]
	0,	//char chPa_Sperrzeit_min;							// Sperrzeitzeit der WPu in [min]
	0,	//char chPa_Verzoegerung_min;					// verzögerte Freigabe der WPU wegen Ansteueurng der Quellenpumpe [min]
 },
  // Profil 1 Solewärmepumpe
 {
 	0,		//char	Para_Manu_Sollwert; 		// Manuellen Sollwert aktivieren
	500,	//UNINT	t_manu_Sollwert; 		// Manuell vorgegebener Sollwert in [?C] *10
	30,		//UINT	T_Sollwert_Offset; 		// Sollwertoffset in [K] *10
	550,	//UINT	T_Ersatz_Sollwert; 		// Ersatz-Sollwert in [°C] *10
	  0,	//UINT	iPa_T_Sollwert_IN_MIN; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten minimalen Wert [°C] *10
	600,	//UINT	iPa_T_Sollwert_IN_MAX; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten maximalen Wert [°C] *10
		0,	//char	WPU_Freigabe_Haut;						// Manueller Betrieb aktivieren zur Steuerung der WPU-Freigabe
	  0,	//char	WPU_Freigabe_Hand_stellen;		// Steuerung der WPU-Freigabe im manuellen Betrieb
	-50,	//UINT	intPa_Quellentemperaturminimum;	// Minimale Quellentemperatur in [°C] *10
	150,  //UINT	intPa_Speicherminimum;					// Minimale Speichertemperatur in [°C] *10
		0,	// int	intPa_T_Speicherladung_on; 			// Speicherladung aktivieren: delat T in [K] *10
	-30,	//int	intPa_T_Speicherladung_off; 		// Speicherladung deaktivieren: delat T in [K] *10
	 15,	//char chPa_Mindestlaufzeit_min;				// Mindestlaufzeit der WPu in [min]
	 	5,	//char chPa_Sperrzeit_min;							// Sperrzeitzeit der WPu in [min]
		2,	//char chPa_Verzoegerung_min;					// verzögerte Freigabe der WPU wegen Ansteueurng der Quellenpumpe [min]
 },
   // Profil 2 Abluftwärmepumpe
 {
 	0,		//char	Para_Manu_Sollwert; 		// Manuellen Sollwert aktivieren
	500,	//UNINT	t_manu_Sollwert; 		// Manuell vorgegebener Sollwert in [?C] *10
	30,		//UINT	T_Sollwert_Offset; 		// Sollwertoffset in [K] *10
	550,	//UINT	T_Ersatz_Sollwert; 		// Ersatz-Sollwert in [°C] *10
	  0,	//UINT	iPa_T_Sollwert_IN_MIN; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten minimalen Wert [°C] *10
	600,	//UINT	iPa_T_Sollwert_IN_MAX; 					// Begrenzt den eingehenden Sollwert (wpd[WP1].Eingehender Sollwert auf den eingestellten maximalen Wert [°C] *10
		0,	//char	WPU_Freigabe_Haut;						// Manueller Betrieb aktivieren zur Steuerung der WPU-Freigabe
	  0,	//char	WPU_Freigabe_Hand_stellen;		// Steuerung der WPU-Freigabe im manuellen Betrieb
	  0,	//UINT	intPa_Quellentemperaturminimum;	// Minimale Quellentemperatur in [°C] *10
	150,  //UINT	intPa_Speicherminimum;					// Minimale Speichertemperatur in [°C] *10
		0,	// int	intPa_T_Speicherladung_on; 			// Speicherladung aktivieren: delat T in [K] *10
	-30,	//int	intPa_T_Speicherladung_off; 		// Speicherladung deaktivieren: delat T in [K] *10
	 15,	//char chPa_Mindestlaufzeit_min;				// Mindestlaufzeit der WPu in [min]
	 	5,	//char chPa_Sperrzeit_min;							// Sperrzeitzeit der WPu in [min]
		0,	//char chPa_Verzoegerung_min;					// verzögerte Freigabe der WPU wegen Ansteueurng der Quellenpumpe [min]
 },
};
const char WP_PROFILE = sizeof ( Wp_Standparam ) / sizeof ( WpStandard );


/*----------------------------------------------------------------------------*/
/*						Liste von Standardwerten für verschiedene Warmwasserkreise			*/
/*----------------------------------------------------------------------------*/
const WwStandard  Ww_Standparam[] = {
 // Profil 0 (dummy)
 {
	0,		// Twwsol	= 0,0			WW-Solltemp										[°C] * 10
	0,		// Twwlad = 0,0			Speicher-Ladetemperatur				[°C] * 10
	0,		// TwwladLegio = 0,0	Speicher-Ladetemperatur bei therm.Desinf.	[°C] * 10
	0,		// Twwlbeg= 0,0			Speicherlade-Beginn-Temp.			[°C] * 10
	0,		// Twwlend= 0,0			Speicherlade-Ende-Temperatur	[°C] * 10
	0,		// Puwlna = 0				Ladepumpe Nachlaufzeit				[min]
	0,		// Puwtna = 0				Tauscherpumpe Nachlaufzeit		[min]	
	0,		// Puwzdau = 0			Zirkulationspumpe Abschaltdauer [std]
	{0,0},// Puwzab[2] =			Zirkulationspumpe Abschaltzeit [min][std]
	0,		// Legjn	= 0				Legionellentod
	0,		// Legtag = 0				Legionellentage
	{0,0},// Legzeit[2]				Legionellen Einschaltzeit			[min][std]
	0,		// Legdau = 1				Legionellen Einschaltdauer		[std]
	0,		// Twwleg = 0,0			Legionellen Temperatur				[°C] * 10
	0,		// Vorrad = 0				Vorrangdauer	max							[min]

	0,		// Ts			= 0,0			Tastzeit (>= 1 s)						[s] * 10
	0,		// Tn			= 0,0			Nachstellzeit								[s] * 10
	0,		// Kp			= 0,00		P-Verstärkung							[%/K] * 100
	0,		// Kd			= 0,00		D-Verstärkung							[%/K] * 100
	0,		// Hub		= 0				Ventilhub									 [mm]
	0,		// Vst		= 0				Stellgeschwindigkeit		 [mm/min]
	0,		// Haut		= 0				Hand/Automatik
	0,		// Vstell	= 0				Hand Ventil stellen			0-10V Ventil	[%] * 10
	0,		// Lpea		= 0				Hand Ladepumpe ein/aus 
	0,		// Zpea		= 0				Hand Zirkpumpe ein/aus
	0,		// Tpea		= 0				Hand Tauscherpumpe ein/aus	
	0,		// Fzk		= 0				Filterzk. f. tsol						[s] * 10
	0,		// TabwMax = 0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	0,		// TabwZeit= 0			Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]	
	0,		// Tvorr = 0				Regelabweichung zur Aktivierung des WW-Vorrangs [K] * 10	
	0,		// TvorrZeit = 0		Mindestzeit zur Aktivierung des WW-Vorrangs [min]			
	0,		// ZpuAusLad = 0		ZirkPu bei Ladung ausschalten		

	0,		// char	FrLadBedarf;	// Freigabe für Schalten des Ladebetriebs nach Bedarfsanford. ( über Kommando WR_KASKADE )
	0,		// char	FrZirkBedarf;	// Freigabe für Schalten der Zirk.pumpe   nach Bedarfsanford. ( über Kommando WR_KASKADE )	
	0,		// AuswahlFuehler			// Speicherladung: Auswahl Fühler ( 1 = oben, 2 = unten, 0 = beide )																															
	0,		// TvpAnh = 0				Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
	0,		// TempWWmin;			// minimale Gebäude-Vorlauftemp. zur Bildung eines Alarms [°C] * 10
	0,		// TempWWminZeit;	// Zeitdauer der minimalen Gebäude-Vorlauftemp. bis zum Alarm [min]	
	0,		// LadungMax;			// [ h ]			

	0,		// PuwlBmVerz;			// [min]
	0,		// PuwzBmVerz;			// [min]
	0,		// PuwtBmVerz;			// [min]
	0,		// LadNieAus;				// Speicherladung immer (temperatur-unabhängig) während Legio-Betrieb
	0,		// TwwsolLegio			// WW-Solltemp.während Legionellenabtötung	[°C] * 10

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,		// int		Wup;					// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	0,		// int	 	TwwzLegio;	// Zirkulations-Solltemp.bei Legio-Betrieb

 },
 
 // Profil 1 
 {
	520,	// Twwsol		= 50,0		WW-Solltemp										[°C] * 10
	650,	// Twwlad 	= 65,0		Speicher-Ladetemperatur				[°C] * 10
	700,	// TwwladLegio = 70,0	Speicher-Ladetemperatur bei therm.Desinf.	[°C] * 10
	580,	// Twwlbeg	= 58,0		Speicherlade-Beginn-Temp.			[°C] * 10
	600,	// Twwlend	= 60,0		Speicherlade-Ende-Temperatur	[°C] * 10, bei ein Fühler 63,0 °C
	10,		// Puwlna 	= 3				Ladepumpe Nachlaufzeit				[min]
	2,		// Puwtna 	= 2				Tauscherpumpe Nachlaufzeit				[min]	
	0,		// Puwzdau 	= 6				Zirkulationspumpe Abschaltdauer [std]
	{0,23},	// Puwzab[2]				Zirkulationspumpe Abschaltzeit [min][std]
	1,		// Legjn		= 1				Legionellentod
	0x12,	// Legtag 	= Mo+Do		Legionellentage
	{0,1},// Legzeit[2]					Legionellen Einschaltzeit			[min][std]
	2,		// Legdau 	= 1				Legionellen Einschaltdauer		[std]
	650,	// Twwleg 	= 65,0		Legionellen Temperatur				[°C] * 10
	30,		// Vorrad 	= 30			Vorrangdauer	max							[min]

	50,		// Ts				= 5,0			Tastzeit (>= 1 s)						[s] * 10
	1200,	// Tn				= 120,0		Nachstellzeit								[s] * 10
	200,	// Kp				= 2,00		P-Verstärkung							[%/K] * 100
	0,		// Kd				= 0,00		D-Verstärkung							[%/K] * 100
	25,		// Hub			= 25			Ventilhub									 [mm]
	25,		// Vst			= 25			Stellgeschwindigkeit		 [mm/min]
	0,		// Haut			= 0				Hand/Automatik
	0,		// Vstell		= 0				Hand Ventil stellen			0-10V Ventil	[%] * 10
	0,		// Lpea			= 0				Hand Ladepumpe ein/aus 
	0,		// Zpea			= 0				Hand Zirkpumpe ein/aus
	0,		// Tpea			= 0				Hand Tauscherpumpe ein/aus
	600,	// Fzk			= 60			Filterzk. f. tsol						[s] * 10
	100,	// TabwMax = 10,0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	60,		// TabwZeit= 60				Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	50,		// Tvorr = 5,0				Regelabweichung zur Aktivierung des WW-Vorrangs [K] * 10	
	5,		// TvorrZeit = 5			Mindestzeit zur Aktivierung des WW-Vorrangs [min]					
	0,		// ZpuAusLad = 1			ZirkPu bei Ladung ausschalten					

	0,		// char	FrLadBedarf  = 0 Freigabe für Schalten des Ladebetriebs nach Bedarfsanford. ( über Kommando WR_KASKADE )
	0,		// char	FrZirkBedarf = 0 Freigabe für Schalten der Zirk.pumpe   nach Bedarfsanford. ( über Kommando WR_KASKADE )	
	0,		// AuswahlFuehler			Speicherladung: Auswahl Fühler ( 1 = oben, 2 = unten, 0 = beide )																															
	50,		// TvpAnh = 0				Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
	500,	// TempWWmin = 50,0		minimale Gebäude-Vorlauftemp. zur Bildung eines Alarms [°C] * 10
	15,		// TempWWminZeit			Zeitdauer der minimalen Gebäude-Vorlauftemp. bis zum Alarm [min]	
	4,		// LadungMax;					[ h	]	

	2,		// PuwlBmVerz;			// [min]
	2,		// PuwzBmVerz;			// [min]
	2,		// PuwtBmVerz;			// [min]
	0,		// LadNieAus;				// Speicherladung immer (temperatur-unabhängig) während Legio-Betrieb
	680,	// TwwsolLegio	= 0,0	// WW-Solltemp.während Legionellenabtötung	[°C] * 10

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,		// int		Wup;					// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	650,	// int	 	TwwzLegio;	// Zirkulations-Solltemp.bei Legio-Betrieb

 },	

};
const char WW_PROFILE = sizeof ( Ww_Standparam ) / sizeof ( WwStandard );

/*----------------------------------------------------------------------------------*/
/*				Liste von Standardwerten für verschiedene Vorregelkreise (Netztrennung)		*/
/*----------------------------------------------------------------------------------*/
const NtStandard  Nt_Standparam[] = {
 // Profil 0 (dummy)
 {
	0,		// FuehrWt = 0			Führungs-Tauscher

	0,		// Ts 		=  0,0 		Tastzeit (>= 1 s)							[s] * 10
	0,		// Tn 		=  0,0 		Nachstellzeit									[s] * 10
	0,		// Kp		 	=  0,00		P-Verstärkung								[%/K] * 100
	0,		// Kd		 	=  0,00		D-Verstärkung								[%/K] * 100
	0,		// Kpr		=  0,00		P-Verstärkung TRS-Begrenzg	[%/K] * 100
	0,		// Tvma		=  0,0		max. Vorlauftemp.						.[°C] * 10		
	0,		// Tvmi		=  0,0		min. Vorlauftemp.						 [°C] * 10		
	0,		// Trma 	=  0,0		max. Rücklauftemp.					 [°C] * 10		
	0,		// Tvpa		=  0,0		Anhebung						 				 [°C] * 10
	0,		// Fzk		=  0,0		Filterzk. f. tsol							[s] * 10
	0,		// Hub		=  0			Ventilhub										 [mm]
	0,		// Vst		=  0			Stellgeschwindigkeit		 [mm/min]
	0,		// Haut		=  0			HAND/AUTO-Umschaltung
	0,		// Vstell	=  0			Hand Ventil stellen			0-10V Ventil	[%] * 10
	0,		// Puea		=  0				Hand: Pumpe	ein/aus	
	0,		// TabwMax = 0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	0,		// TabwZeit= 0			Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]

	0,		// FuehrWtFest	= 0    Führungs-Tauscher fest vorgegeben, wenn ungestoert	
	0,		// WtWechseln		= 0    laufzeitabh. Umschalten des Führungs-Tauschers  [h]
	0,		// FolgeWtAus		= 0,0  Abschalten des Folge-Tauschers, wenn VL - RL des Führungs-Tauschers zu klein [K] * 10 	
	0,		// WtSchaltDiff	= 0,0  Schaltdifferenz zum Abschalten des Führungs-Tauschers [K] * 10 
	0,		// Einkrit			= 0,0  Einschaltkriterium Folge-Tauscher	[K*min] * 10
	0,		// Auskrit			= 0,0  Ausschaltkriterium Folge-Tauscher	[K*min] * 10
	0,		// KlAZ					= 0    Klappenstellung bei Hand	
	0,		// KlappeVerz		= 0		 Verzögerung des Schließens der Klappe [min]
	
	0,		// Ts_anh				= 0		 Tastzeit (>= 1 s)							 [s] * 10
	0,		// Tn_anh				= 0		 Nachstellzeit									 [s] * 10
	0,		// Kp_anh				= 0		 P-Verstärkung								 [%/K] * 100
	0,		// Kd_anh				= 0		 D-Verstärkung								 [%/K] * 100	

	0,		// PuBmVerz;				// [min]

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,		// int		Wup;					// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },
 
 // Profil 1
 {
	0,		// FuehrWt = 0			Führungs-Tauscher

	100,	// Ts 		= 10,0 		Tastzeit (>= 1 s)							[s] * 10
	1500,	// Tn 		= 150,0 	Nachstellzeit									[s] * 10
	100,	// Kp 		=  1,00		P-Verstärkung								[%/K] * 100
	0,		// Kd 		=  0,00		D-Verstärkung								[%/K] * 100
	0,	// Kpr		=  2,25		P-Verstärkung TRS-Begrenzg	[%/K] * 100
	950,	// Tvma		= 95,0		max. Vorlauftemp.						.[°C] * 10	System 90/70		
	200,	// Tvmi		= 20,0		min. Vorlauftemp.						 [°C] * 10		
	990,	// Trma 	= 70,0		max. Rücklauftemp.					 [°C] * 10		
	0,		// Tvpa		=  0,0		Anhebung						 				 [°C] * 10
	600,	// Fzk		= 60,0		Filterzk. f. tsol							[s] * 10
	20,		// Hub		= 20			Ventilhub										 [mm]
	10,		// Vst		= 10			Stellgeschwindigkeit		 [mm/min]
	0,		// Haut		=  0			HAND/AUTO-Umschaltung
	0,		// Vstell	=  0			Hand Ventil stellen			0-10V Ventil	[%] * 10
	1,		// Puea		=  1			Hand: Pumpe	ein/aus	
	100,	// TabwMax = 0			maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	60,		// TabwZeit= 0			Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]

	0,		// FuehrWtFest	= 0    Führungs-Tauscher fest vorgegeben, wenn ungestoert	
	50,		// WtWechseln		= 50   laufzeitabh. Umschalten des Führungs-Tauschers  [h]
	30,		// FolgeWtAus		= 3,0  Abschalten des Folge-Tauschers, wenn VL - RL des Führungs-Tauschers zu klein [K] * 10 	
	80,		// WtSchaltDiff	= 8,0  Schaltdifferenz zum Abschalten des Führungs-Tauschers [K] * 10 
 200,		// Einkrit			= 20,0 Einschaltkriterium Folge-Tauscher	[K*min] * 10
	50,		// Auskrit			= 5,0  Ausschaltkriterium Folge-Tauscher	[K*min] * 10
	0,		// KlAZ					= 0    Klappenstellung bei Hand	
	1,		// KlappeVerz		= 0		 Verzögerung des Schließens der Klappe [min]

	10,		// Ts_anh 			= 1,0	 Tastzeit (>= 1 s)						[s] * 10
	12000,// Tn_anh				= 1200,0 	Nachstellzeit							[s] * 10
	100,	// Kp_anh 			= 1,00 P-Verstärkung								[%/K] * 100
	0,		// Kd_anh				= 0		 D-Verstärkung								[%/K] * 100	

	1,		// PuBmVerz;				// [min]

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	0,		// int		Wup;					// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
 },

};
const char NT_PROFILE = sizeof ( Nt_Standparam ) / sizeof ( NtStandard );


/*----------------------------------------------------------------------------------*/
/*				Liste von Standardwerten für Solar-Regelkreise		*/
/*----------------------------------------------------------------------------------*/
const SoStandard  So_Standparam[] = {
 // Profil 0 (dummy)
	{
		0,	// TKolBeg = 0	 		erforderliche Mindesttemp. am Kollektor-Fühler ( Solar-Lad. ein ) [°C] * 10
		0,	// TKolEnd = 0	 		Min. Temp. am Kollektor-Fühler ( Solar-Lad. aus ) [°C] * 10
		0,	// TPufMax = 0	 		Max. Temp. am oberen Puffer-Fühler ( Solar-Lad. aus ) [°C] * 10
		0,	// TDiffBeg = 0	 		erforderliche Mindesttemp.-Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. ein ) [K] * 10
		0,	// TDiffEnd = 0	 		Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. aus )	[K] * 10
		0,	// Puna = 0	 				Kollektorpumpen-Nachlaufzeit	[min]
		0,	// Haut = 0	 				HAND/AUTO-Umschaltung
		0,	// Kpea = 0	 				Hand Kollektorpumpe ein/aus
	},	
	// Profil 1	
	{
		300,// TKolBeg = 30,0	 	erforderliche Mindesttemp. am Kollektor-Fühler ( Solar-Lad. ein ) [°C] * 10
		200,// TKolEnd = 20,0	 	Temp. am Kollektor-Fühler ( Solar-Lad. aus ) [°C] * 10
		950,// TPufMax = 95,0		Max. Temp. am oberen Puffer-Fühler ( Solar-Lad. aus ) [°C] * 10
		60,	// TDiffBeg = 6,0	 	erforderl. Mindesttemp.-Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. ein ) [K] * 10
		40,	// TDiffEnd = 4,0	 	Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. aus )	[K] * 10
		2,	// Puna = 2	 				Kollektorpumpen-Nachlaufzeit	[min]
		0,	// Haut = 0	 				HAND/AUTO-Umschaltung
		0,	// Kpea = 0	 				Hand Kollektorpumpe ein/aus
	},
};
const char SO_PROFILE = sizeof ( So_Standparam ) / sizeof ( SoStandard );		 	

/*----------------------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für Benutzersteuerung mit UNI-Elementen				*/
/*----------------------------------------------------------------------------------------*/
const UniStandard  Uni_Standparam[] = {
 // Profil 0 (dummy)
	{
//		 0,		// char	Loader;			// Testparameter
		0,		// NspMax;					// min
	},
	// Profil 1
	{
//		10,		// char	Loader;			// Testparameter
		10,		// NspMax;					// min
	},
};
const char UNI_PROFILE = sizeof ( Uni_Standparam ) / sizeof ( UniStandard );		




/*----------------------------------------------------------------------------------*/

// Dimensionsnamen für Zähler
const zdname ze_dim_namen[10] = {
//   Text  , Einheit,	Basiseinheit  dim_idx
	{"      ", 0x00,		0},						// 0
	{"GJ    ", 0xE0,		8},						// 1		(E_GIGA + E_JOULE)
	{"MJ    ", 0xA0,		8},						// 2		(E_MEGA + E_JOULE)
	{"kJ    ", 0x60,		8},						// 3		(E_KILO + E_JOULE)
	{"MWh   ", 0x83,		7},						// 4		(E_MEGA + E_WATT + E_STUNDE)
	{"kWh   ", 0x43,		7},						// 5		(E_KILO + E_WATT + E_STUNDE)
	{"m    ", 0x08,		9},						// 6		(E_QMETER)
	{"Wh    ", 0x03,		0},						// 7		(E_WATT + E_STUNDE)						Basiseinheit
	{"J     ", 0x40,		0},						// 8		(E_JOULE)											Basiseinheit
	{"l     ", 0x04,		0},						// 9		(E_LITER)											Basiseinheit
}; 


// Funktionsnamen für Schnittstellen
const char fu_namen[9][7] = {
	{"      "},  // #define NULL_FU         0         // keine Funktion                
	{"GBUS  "},  // #define GBUS_FU         1         // RS485 RIEcon Gebäudebus       
	{"FUNK  "},  // #define FUNK1_FU        2         // RS485 Funkmodul EnOcean       
	{"GENI  "},  // #define GENI1_FU        3         // RS485 Genibus Pumpen          
	{"MBUS  "},  // #define MBUS1_FU        4         // RS232 MBUS                    
	{"TEST  "},  // #define GBUS1_FU        5         // RS485 Test Gebäudebus         
	{"MODBUS"},  // #define MODBUS1_FU      6         // RS485 oder RS232 MODBUS Master
	{"KNXBUS"},  // #define KNXBUS1_FU      7         // KNX/EIB über RIEcon66-SPL-KNX 
	{"ANYBUS"},  // #define ANYBUS1_FU      8         // RS232 Anybus                  
};	

/* Zählerdimensionierungen für Systemstart 	(leerer EEPROM, Kaltstart 67 oder Kaltstart 99)  */
// Stichtag 01.01.00 ( {0,1,1} ) verhindert Stichtag !
// Stichtag ausführen : konfig Bit 4 setzen (konfig = 0x10)

const zaehldim ze_dimsys[20 + MBUSANZ] = {
// Der Impulswert wird als Exponent zur Basiseinheit eingetragen.

// komma, dim_idx, faktor, exp, konfig, medium, name_idx, stich_tag, reserve, zname(Zählername),	znumm(Zählernummer)
// R36-E2
	{0, 			0,			0,			0,		0,			0,			1,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN3
	{0,				0,			0,			0,		0,			0,			2,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN4
	{0,				0,			0,			0,		0,			0,			3,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN5
	{0,				0,			0,			0,		0,			0,			4,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN6
	{3,				6,			0,			0,		0,			0,			5,			{0,1,1},	 {0},			"WMZ WPU HEISSG.", "               "},// ZIN7		// 1 Impuls = 1000000 Wh (1MWh)
	{2,				6,			0,			1,		0,			0,			6,			{0,1,1},	 {0},			"WMZ WPU-ST     ", "               "},// ZIN8		// 1 Impuls = 1000000 Wh (1MWh)
	{2,				6,			0,			1,		0,			0,			7,			{0,1,1},	 {0},			"WMZ WPU-SP     ", "               "},// ZIN9		// 1 Impuls = 1000000 Wh (1MWh)
	{0,				5,			0,			0,		0,			0,			8,		  {0,1,1},	 {0},			"STROM WPU      ", "               "},// ZIN10	// 1 Impuls = 1000 Wh (1kWh)
// R37_1
	{2, 			6,			0,			1,		0,			0,			11,			{0,1,1},	 {0},			"KALTWASSER     ", "               "},// ZIN11	// 1 Impuls =   10 l  (0,01m³)
	{0,				0,			0,			0,		0,			0,			12,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN12
	{0,				0,			0,			0,		0,			0,			13,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN13
	{0,				0,			0,			0,		0,			0,			14,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN14
	{0,				0,			0,			0,		0,			0,			15,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN15
	{0,				0,			0,			0,		0,			0,			16,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN16
// R37_2
	{0, 			0,			0,			0,		0,			0,			21,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN21
	{0,				0,			0,			0,		0,			0,			22,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN22
	{0,				4,			0,			6,		0,			0,			23,			{0,1,1},	 {0},			"WAERMEMENGE    ", "               "},// ZIN23	// 1 Impuls = 1000000 Wh (1MWh)
	{0,				0,			0,			0,		0,			0,			24,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN24
	{0,				0,			0,			0,		0,			0,			25,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN25
	{0,				0,			0,			0,		0,			0,			26,			{0,1,1},	 {0},			"UNBEKANNT      ", "               "},// ZIN26
// MBus
#if MBUSANZ > 0
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,31,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z01
#endif
#if MBUSANZ > 1
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,32,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z02
#endif
#if MBUSANZ > 2
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,33,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z03
#endif
#if MBUSANZ > 3
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,34,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z04
#endif
#if MBUSANZ > 4
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,35, {1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z05
#endif
#if MBUSANZ > 5
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,36,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z06
#endif
#if MBUSANZ > 6
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,37,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z07
#endif
#if MBUSANZ > 7
	{3,				4,			0,			0,		Z_KONFIG_STICHTAG,0,38,	{1,1,1},	 {0},			"UNBEKANNT      ", "               "},// MB_Z08
#endif
};	


/*------------------------------------------------------------------------------------*/
/*  							Liste von Standardwerten für Skalierung von Analogausgängen					*/
/*------------------------------------------------------------------------------------*/
//Skal bei 0% und 100%			Werteingabe in [Volt] * 100

const aask R36_37_Standparam[][5] = {
	{	// R36
		{   0, 1000},	// AA1
		{   0, 1000},	// AA2
		{   0,    0},	// 
		{   0,    0},	//
	},

	{	// R37_1
		{   0, 1000},	// AA1
		{   0, 1000},	// AA2
		{   0, 1000},	// AA3
		{   0, 1000},	// AA4
	},

	{	// R37_2
		{   0, 1000},	// AA1
		{   0, 1000},	// AA2
		{   0, 1000},	// AA3
		{   0, 1000},	// AA4
	},

	{	// R37_3
		{   0, 1000},	// AA1
		{   0, 1000},	// AA2
		{   0, 1000},	// AA3
		{   0, 1000},	// AA4
	},

	{	// R37_4
		{   0, 1000},	// AA1
		{   0, 1000},	// AA2
		{   0, 1000},	// AA3
		{   0, 1000},	// AA4
	},	
};

/*--------------------------------------------------------------------------------------*/
/*				Liste von Standardwerten für die Skalierung der universellen Analogeingänge		*/
/*--------------------------------------------------------------------------------------*/
// Brandenburg, Klosterquartier, Heizung
const sAnaInpPara AnaInpStandardPara[] = {
// Beispiel für 	AE_UNI1 : Druck Rücklauf Anlage: 2 V = 0,00, 10 V = 6,00 bar
//	 {-150,				// Skalierung 0 V
//		600,				// Skalierung 10 V
//		5,					// Filterzeitkonstante [s]		
//		200,				// UGW	2,00 bar
//		0,					// OGW												kein OGW
//		20},				// Hysterese Grenzwerte 0,2 bar

								// AE_UNI1
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		10,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI2 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		10,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI3 : 
	 {0,					// Skalierung 0 V
		160,				// Skalierung 10 V
		10,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		2},					// Hysterese Grenzwerte 	

								// AE_UNI4 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		5,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI5 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		5,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI6 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		5,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI7 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		5,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	

								// AE_UNI8 : 
	 {0,					// Skalierung 0 V
		1000,				// Skalierung 10 V
		5,					// Filterzeitkonstante [s]		
		0,					// UGW												keine Grenzwerte
		0,					// OGW
		0},					// Hysterese Grenzwerte 	
};

#if RM_POWER_ANZ
const sPowInpPara RmPowerStandardPara[] = {
								// RM_POW1
	 {20,					// Skalierung 0 % = 2,0 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#if RM_POWER_ANZ > 1
								// RM_POW2 : 
	 {20,					// Skalierung 0 % = 2,0 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 2
								// RM_POW3 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 3
								// RM_POW4 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 4
								// RM_POW5 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 5
								// RM_POW6 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 6
								// RM_POW7 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
#if RM_POWER_ANZ > 7
								// RM_POW8 : 
	 {0,					// Skalierung 0 % = 0,00 V
		1000,				// Skalierung 10,0 V = 100,0 %
		10},				// Filterzeitkonstante [s]		
#endif
};
#endif

#if AE_DRUCK_ANZ
const sAnaInpPara DruckStandardPara[] = {
// Beispiel: Druck Rücklauf Anlage: 0 bar = 2,0 V, 10 V = 6,00 bar
//	 {20					// Skalierung 0 bar = 2,0 V
//		600,				// Skalierung 10 V = 6,00 bar
//		10,					// Filterzeitkonstante [s]		
//		200,				// UGW	2,00 bar
//		000,				// OGW	kein OGW
//		20},				// Hysterese Grenzwerte 0,20 bar

								// AE_DRU1
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar	
#if AE_DRUCK_ANZ > 1
								// AE_DRU2 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,0 bar
		20},				// Hysterese Grenzwerte 0,20 bar	
#endif
#if AE_DRUCK_ANZ > 2
								// AE_DRU3 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar	
#endif
#if AE_DRUCK_ANZ > 3
								// AE_DRU4 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar		
#endif
#if AE_DRUCK_ANZ > 4
								// AE_DRU5 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar	
#endif
#if AE_DRUCK_ANZ > 5
								// AE_DRU6 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar		
#endif
#if AE_DRUCK_ANZ > 6
								// AE_DRU7 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar	
#endif
#if AE_DRUCK_ANZ > 7
								// AE_DRU8 : 
	 {00,					// Skalierung 0 bar = 2,0 V
		1600,				// Skalierung 10 V = 6,00 bar
		10,					// Filterzeitkonstante [s]		
		100,				// UGW = 2,00 bar
		300,				// OGW = 3,00 bar
		20},				// Hysterese Grenzwerte 0,20 bar		
#endif
};
#endif

/*----------------------------------------------------------------------------------*/

/* Tabelle der Aufheizzeiten											*/
const char tabse[6][5] = {
	//tvab	<=20,		<=30,	<=40,		<=50,		>50		[K] 
//UlSch auf Wunsch von ThoSi 090312
	//tvab	<=5,		<=10,	<=15,		<=20,		>20		[K] 
					{60,		80,		100,		120,		150},	// ta < -10 °C	
					{50,		70,		 90,		110,		140},	// ta <  -5 °C	
					{30,		50,		 70,		100,		100},	// ta <   0 °C	
					{20,		30,		 50,		 80,		 80},	// ta <   5 °C	
					{10,		20,		 30,		 50,		 50},	// ta <  10 °C	
					{ 0,		 0,		  0,		  0,		  0},	// ta >  10 °C	
};					


/* Sonderzeichen LCD */
const char CarTab[64] = {										//  Code:	Zeichen			in RFB	
	0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00,	//  0x04:	Kasten					"®"  'gefülltes Karo	
	0x00,0x01,0x03,0x05,0x09,0x11,0x1F,0x00,	//  0x05:	Delta						'8'
	0x06,0x09,0x09,0x06,0x00,0x00,0x00,0x00,	//  0x06:	Grad						'°'
	0x18,0x04,0x0C,0x04,0x18,0x00,0x00,0x00,	//  0x07:	hoch 3					'³'
	0x00,0x04,0x0E,0x15,0x04,0x04,0x00,0x00,	//  0x00:	Pfeil hoch			'0'
	0x00,0x04,0x04,0x15,0x0E,0x04,0x00,0x00,	//  0x01:	Pfeil runter		'2'
	0x10,0x18,0x1C,0x1E,0x1C,0x18,0x10,0x00,	//  0x02:	Dreieck					'4'
	0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00		//  0x03:	Mittelpunkt			'6'
};

//---------- Bus-Pumpen --------------------------------------------------
const char * pu_operation_set[]	=
	{	
		"PUMPE STOP     \0",		// 0
		"PUMPE EIN      \0",		// 1
		"MIN-DREHZAHL   \0",		// 2
		"MAX-DREHZAHL   \0",		// 3
	  "Keine Antwort !\0",		// 4	***AnFre 12.12.2012
	};

const char * pu_control_set[]	=
	{	
		"KONSTANTDRUCK  \0",		// 0
		"PROPORT.DRUCK  \0",		// 1
		"KONSTANTFREQU. \0",		// 2
		"AUTOMATIK-MODE \0",		// 3
	  "Keine Antwort !\0",		// 4	***AnFre 12.12.2012
	};


const char ClearDisplay[] =
	{ "               \0"};

const char * pu_func_set[]	=
	{	
		"DEAKTIVIERT!   \0",		// 0
		"REMOTE MODE    \0",		// 1
		"LOCAL MODE     \0",		// 2
	};

const char * wilo_servstat_set[]	=
	{	
		"Keine Meldung  \0",
		"Servicebedarf  \0",				
		"Lager wechseln \0",				
		"Lager fetten   \0",				
		"Dichtung tausch\0",				
		"Undef Meldung! \0",
	};

const char * wilo_error_set[]	=
	{	
		"Keine Meldung  \0",
		"Modul-Fehler   \0",				
		"Motor-Fehler   \0",				
		"Nicht belegt   \0",				
		"Pumpen-Fehler  \0",				
		"Versorg.Spannun\0",				
		"Undef Meldung! \0",
	};

const char * wilo_errorstat_set[]	=
	{	
		"Keine Meldung  \0",
		"Unterspannung  \0",		
		"Ueberspannung  \0",
		"Phase fehlt    \0",
		"Trockenlauf    \0",
		"Druck zu hoch  \0",
		"Druck zu niedr.\0",
		"Nicht belegt   \0",
		"Nicht belegt   \0",
		"UeberTemp Motor\0",
		"Motorfehler    \0",
		"Blockierung    \0",
		"UeberTemp Modul\0",
		"Modulfe.Warnung\0",
		"Modulfehler    \0",
		"Sensorfehler   \0",
		"Undef Meldung! \0",
	};

const char * wilo_pustat_set[]	=
	{	
		"Keine Meldung  \0",
		"Betriebsm.  EIN\0",	
		"Drehricht.Links\0",	
		"Soll-Ist A.>10%\0",
		"Extern Aus     \0",	
		"Doppelpumpe    \0",	
		"Lokale Bedien. \0",	
		"Q/H nicht ermit\0",	
		"Extern Min.    \0",	
		"Nicht belegt   \0",
		"Nicht belegt   \0",
		"Nicht belegt   \0",
		"Nicht belegt   \0",
		"Nicht belegt   \0",
		"Wink/Service   \0",	
		"Undef Meldung! \0",
	};

const char * wilo_statdiag_set[]	=
	{	
		"Keine Meldung  \0",
		"Pu o.Modul-Fehl\0",
		"Versorgung Eo.H\0",
		"Nicht belegt   \0",
		"Untere Regelgr.\0",
		"Obere Regelgr. \0",
		"Nicht belegt   \0",
		"Sollwert-Fehler\0",
		"Nicht belegt   \0",
		"Lokale Bedien. \0",
		"Nicht belegt   \0",
		"Pumpe laeuft   \0",
		"Undef Meldung! \0",
	};
//---------- Ende Bus-Pumpen --------------------------------------------------



// im System benötigte Definitionen werden in const umgewandelt. Zugriff erfolgt dann über "extern ..."
// aktuelle Zählwerte
const UINT uiZAEHLER01_ADR 		= ZAEHLER01_ADR;	// R36
const UINT uiZAEHLER11_ADR		= ZAEHLER11_ADR;	// R37_1
const UINT uiZAEHLER21_ADR 		= ZAEHLER21_ADR;	// R37_2
const UINT uiZAEHLERMB1_ADR 	= ZAEHLERMB1_ADR;	// MBus
const UINT uiZVJ_OFFSET				= ZVJ_OFFSET;			// Offset zum Vorjahres-Hauptzähler

// Verbrauchswerte des aktuellen Monats 
const UINT uiVERBRAU01_ADR 		= VERBRAU01_ADR;	// R36  
const UINT uiVERBRAU11_ADR 		= VERBRAU11_ADR;	// R37_1
const UINT uiVERBRAU21_ADR 		= VERBRAU21_ADR;	// R37_2
const UINT uiVERBRAUMB1_ADR 	= VERBRAUMB1_ADR;	// MBus

// Zählerdimensionierungen
const UINT uiZAEHL01_DIM_ADR 	= ZAEHL01_DIM_ADR;	// R36
const UINT uiZAEHL11_DIM_ADR 	= ZAEHL11_DIM_ADR;	// R37_1
const UINT uiZAEHL21_DIM_ADR 	= ZAEHL21_DIM_ADR;	// R37_2

// Monatsverbräuche
const UINT uiZMV01_ADR				=	ZMV01_ADR;			// R36  
const UINT uiZMV11_ADR				=	ZMV11_ADR;			// R37_1
const UINT uiZMV21_ADR				=	ZMV21_ADR;			// R37_2
const UINT uiZMVMB1_ADR				=	ZMVMB1_ADR;			// MBus
const UINT uiZMV_OFFSET				=	ZMV_OFFSET;			// Offset zur nächsten Zähleradresse
const UINT uiZMV_VJOFFS				=	ZMV_VJOFFS;			// Offset zum Vorjahresbereich innerhalb eines Zählerspeicherbereiches

const char ucZEX1BELEG				=	ZEX1BELEG;			// Zählerbelegung R37_1
const char ucZEX2BELEG				=	ZEX2BELEG;			// Zählerbelegung R37_2
const char ucR37_MODULE 			= R37_MODULE;
const char ucMBUSANZ					= MBUSANZ;
 
const UINT uiPE_ADR 					= PE_TAB_ADR;
const UINT uiSE_ADR 					= SE_TAB_ADR;
const UINT uiAL_ADR 					= AL_TAB_ADR;
const char ucGRUP							= GRUP; 
const char ucHKANZ						= HKANZ; 
const char ucLED_DEBUG				= LED_DEBUG; 

// Serieller Schnittstellen Index
const char ucS1 =	S1;
const char ucS2 =	S2;
const char ucS3 =	S3;
const char ucWRE100  = WRE100;



// diese allgemeinen Namen entfallen, wenn jedem Zähler individuell ein Name zugeordnet wird
//#####MBUS
const char ZlrWaerme[16]  = {"Waerme         "};														 
const char ZlrElektr[16]  = {"Elektrizitaet  "};														 
const char ZlrWarmwas[16] = {"Warmwasser     "};														 
const char ZlrKaltwas[16] = {"Kaltwasser     "};														 
const char ZlrFrei[16]    = {"nicht benutzt  "};														 
