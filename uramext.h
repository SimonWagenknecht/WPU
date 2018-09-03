/*          uramext.h                                  */
/*  Header für nicht intialisierte User - Variablen  */

#ifndef URAMEXT_H_INCLUDED
#define URAMEXT_H_INCLUDED

#include "userdef.h"
#include "ustruct.h"
#include "archivsdm.h"	// Josch-SDM

extern int k_dummy;

extern char testClear;
extern char char_test1;
extern char char_test2;
extern int  int_test;
extern mwsp *temperatur;
extern mwsp mod_rt[2];

extern int TaErsatz[12];
extern UINT TaErsatzCtr;
extern char TaErsatzVerz;

// NLT Neue Leittechnik
extern events prim_evtab[];
extern events seku_evtab[];
extern char sms_feld[];				// Merker für Feldindex prim_evtab

/* Konfigurations-Anzeigebytes */
extern char ptinput1;
extern char ptinput2;
extern char aeinput1;
extern char aeinput2;
extern char diginput1;
extern char diginput2;
extern char digtherm1;
extern char digtherm2;
extern char digoutput1;
extern char digoutput2;
extern char anaoutput1;
extern char anaoutput2;
extern char zinput1;					// R36
extern char zinput2;					// R36
extern char zinput_r37[4];		// R37_1 und R37_2  ( R37_3 und R37_4 nicht benutzt, aber Speicherplatz reserviert )

extern char ort_idf;
extern char ort_idf_flag;

// Objekt- und Standortbezeichnungen
extern char Objektbez [16];		
extern char Objektnum [10];		
extern char Strasse   [16];		
extern char Nummer    [10];		
extern char Ort       [16];		
extern char Plz       [10];		
extern char Version_PVers_Ascii[10];	// Programmversion

//--------------------------------------------------------------------------------------------------
// M-Bus
#if ( ((IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL) )

extern MbusDynam MbusData[];
extern MbusStandard MbusPara[];
extern UINT MbusFrequ;
extern char MbusAktualisierung;
extern char MbusCtrAktiv;
extern char MbusStatus;
extern UINT Timeout;
extern char RxStatus;

extern char MbusTxCtr;
extern char MbusTxLng;
extern char MbusRxCtr;
extern char MbusRxLng;
extern char *pMbusTx;
extern char *pMbusRx;

extern char MbusTxBuf[];
extern char MbusRxBuf[];

extern char MbCtrAkt;
//extern Sichtbarkeit Sichtbar[];
extern Sichtbarkeit Sichtbar;
extern uhrzeit AktStd;
extern uhrzeit LetzteAkt;
extern char mbus_vis;
extern char zaehlerNummer;

#endif

//--------------------------------------------------------------------------------------------------
// Benutzung der Funktion Test-RS485
// Aufruf durch HandlerS1, HandlerS2 oder HandlerS3 organisiert durch UserHandler
#if ( ((IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL) || ((IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL) || ((IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL) )

extern serial485	serial[3];

#endif

//--------------------------------------------------------------------------------------------------
// Benutzung der Funktion Funkempfang
// Aufruf durch HandlerS1, HandlerS2 oder HandlerS3 organisiert durch UserHandler
#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )

extern char ucReceiverTyp_RF;		//090310 0=NEU 1=ALT
extern char ucCond_RF;
extern char ucRxFlag_RF;
extern char ucChecksum_RF;
extern char ucBcounter_RF;
extern char ucfRxBuf_RF[12];
extern char ucRF_ComPort;				// muß in den EEPROM
extern rfsens	rf_feld[12];
extern char ucTeach_idx;
extern char bedien_rf_nummer;
extern char funk_vis;

#endif

//--------------------------------------------------------------------------------------------------
// Grundfos-Pumpen mit GENIbus 
//--------------------------------------------------------------------------------------------------
#if ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )

// Neue Genibus Variablen
	extern GeniDeviceTable genibus_device_table[];		// ToDo: Const Basis Tabelle
	
	extern GeniValues Geni_Values[];
	
	extern unsigned char uc_geni_dummy_stat;
	
	extern unsigned char uc_genibus_device_nr;
	
	extern unsigned char ucf_genibus_rx_buffer[];
	extern unsigned char uc_genibus_rx_count;
	extern unsigned char uc_genibus_rx_buff_size;
	extern unsigned char uc_genibus_rx_errorflag;
	extern unsigned char uc_genibus_rx_error;
	extern unsigned char uc_genibus_rx_telegram_ready;
	extern unsigned char uc_genibus_rx_timeout_ctr;

	extern unsigned char uc_genibus_tx_count;
	extern unsigned char uc_genibus_tx_lenght;

	extern unsigned char uc_genibus_rts_prescaler; 
	extern unsigned char uc_genibus_rts_timer;
	
	extern unsigned char uc_genibus_state;
	extern unsigned char uc_genibus_special_function;
	
	extern unsigned char uc_genibus_own_adr;
	
	extern unsigned char uc_genibus_get_adress;
	extern unsigned char uc_genibus_reset_pump;
	extern unsigned char uc_genibus_reset_result;
	extern unsigned char uc_genibus_start_manual_request;	
	extern GeniManualRequest genibus_manual_request;
		
	extern unsigned char uc_genibus_request_table_idx;
	
	extern unsigned int  ui_genibus_relay_ctr;
	
	extern unsigned char uc_genibus_func_test;
#endif

//----------------------------------------------------------------------------------------------------
// Wilo-Pumpen mit MODBUS
//--------------------------------------------------------------------------------------------------
#if WILO_MODBUS == 1

	extern char cWiloPuCount; // Anzeigecounter für Anzuzeigende Pumpe in parli-Anzeige

	extern PuBusStandard *wiloPuBusPara;						// Modbus Struktur für Pumpenparameter (Adresse usw)
	extern PuBusDynam *wiloPuBusData;

	extern ModBusWiloPumpen modWiloPuData; 					// Modbus Struktur für Modbustabelle (Datenanzeige und Daten holen getrennt halten!!!)
	extern ModBusWiloPumpen modWiloPu[BUS_PU_MAX];	// Struktur für Pumpenparamter aus Kommunikation übertragen auf die jeweiligen Wilo Pumpen 

#endif	

//----------------------------------------------------------------------------------------------------
// Grundfos-Pumpen mit MODBUS
//----------------------------------------------------------------------------------------------------
#if GRUNDFOS_MODBUS == 1


#endif

//----------------------------------------------------------------------------------------------------
// für alle Pumpen
//----------------------------------------------------------------------------------------------------
#if ( GENI == 1 || WILO_MODBUS == 1 || GRUNDFOS_MODBUS == 1)

	extern PuBusStandard BusPuPara[BUS_PU_MAX];
	extern PuBusDynam BusPuData[BUS_PU_MAX];
	extern char ssmBusPu;
	extern char ssmPuCtr;
#endif

//----------------------------------------------------------------------------------------------------
// Modbus 
//--------------------------------------------------------------------------------------------------
#if MODBUS_UNI > 0

extern unsigned int ModBusKonvertError;                  // Modbus.c  + modbusparli.h
extern unsigned int modbusDeviceNr;                      // Modbus.c  + modbusparli.h
extern int activeRow[3];                                 // Modbus.c  + modbusparli.h
extern unsigned char modbusSlaveAddresses[3];            // Modbus.c  + modbusparli.h  + modbusstandard.h
extern unsigned char modbusSioParity[3];                 // Modbus.c  + modbusparli.h  + modbusstandard.h
extern unsigned char modbusSioStopBits[3];               // Modbus.c  + modbusparli.h  + modbusstandard.h
extern unsigned char modbusConvertBuffer[3][256];        // Modbus.c   

extern unsigned char modbusDeviceAddresses[];            // (nicht im UserRam.c) Modbus.c  + ModbusTabelle.c + modbusparli.h  + modbusstandard.h 
extern modbusTableRowMasterData modbusTableData[];       // (nicht im UserRam.c) Modbus.c  + ModbusTabelle.c
extern modbusTableRowSlaveData modbusSlaveTableData[];   // (nicht im UserRam.c) Modbus.c  + ModbusTabelle.c

extern char modb_leng_tx[3];                             // ModBusSio.c
extern char modb_rts_prescaler[3];                       // ModBusSio.c
extern char modb_rts_timer[3];                           // ModBusSio.c
extern char modb_rx_buff_size;                           // ModBusSio.c
extern char modb_rx_status;                              // Modbus.c                   
extern char modb_tx_count[3];                            // ModBusSio.c
extern char modb_rx_count[3];                            // ModBusSio.c              
extern char modb_rx_buffer_ready[3];                     // ModBusSio.c              
extern char modb_rx_int_state[3];                        // ModBusSio.c              
extern char modb_sio_errorflag[3];                       // ModBusSio.c
extern unsigned int EoF_timer[3];                        // ModBusSio.c
extern char timer_function[3];                           // ModBusSio.c
extern char func_code[3];                                // ModBusSio.c  + Modbus.c  
extern unsigned char byte_count_or_exception[3];         // ModBusSio.c  + Modbus.c
extern unsigned int first_reg[3];                        // ModBusSio.c  + Modbus.c  
extern unsigned int cnt_regs[3];                         // ModBusSio.c  + Modbus.c  
extern void* reg_address[3];                             // ModBusSio.c  + Modbus.c
extern char modb_curr_adr[3];                            // ModBusSio.c  + Modbus.c  

/*--------------- Modbus-Datenpunkte -----------------------------*/
#if MODBUS_EXT == 1       // bei Verwendung von READ_MULTIPLE_COILS in der ModbusTabelle
extern ULONG ul_dig32[];  // READ_MULTIPLE_COILS für 32 Bit
extern char  uc_dig[];    // Extraktion 32Bit auf Einzelbytes im Task ModbusExtract
#endif

#endif  // MODBUS_UNI

//----------------------------------------------------------------------------------------------------
// Variabler Kategorie-Typ
extern char KatVar_Typ[16];
// Info über Handschalter in ASCCI
extern char sw_stat_info[16];

// Index-Feld für Namenslisten R36, R37, R38, R33
extern unsigned int nlist[18];									// 18 Ein/Ausgänge				R36
extern unsigned int n37list[R37_MODMAX][13];		// 13 Ein/Ausgänge				R37 
extern unsigned int n38list[R38_MODMAX][14];		// 14 Analogeingänge			R38
extern unsigned int n39list[R39_MODMAX][12];		// 12 Digitaleingänge			R39
extern unsigned int n33list[R33_MODMAX][6];			//  6 Digitalausgänge			R33

extern char ntext[18][21];											// 18 E/A je 20 ASCII
extern char n37text[R37_MODMAX][13][21];				// 13 Ein/Ausgänge				R37 
extern char n38text[R38_MODMAX][14][21];				// 13 Ein/Ausgänge				R37 
extern char n39text[R39_MODMAX][12][21];				// 12 Digitaleingänge			R39
extern char n33text[R33_MODMAX][6][21];					//  6 Digitalausgänge			R33

//Sichtbarkeitsspeicher für Parametergruppen
extern	char	anl_vis;					// Anlage
extern	char	ke_vis[KEMAX];		// Kessel
extern	char	hk_vis[HKMAX];		// Heizung
extern	char	ww_vis[WWMAX];		// Warmwasser
extern  char	nt_vis[NTMAX];		// Netztrennung
extern	char	so_vis[SOLMAX];		// Solar

// #####ulsch
extern char	fer_vis;						// Ferienprogramm
extern char qsm_vis;						// Quittierbare Gespeicherte Störmeldungen

extern	char	ze_vis[];					// Zähler R36
extern	char	x1ze_vis[];				// Zähler R37_1
extern	char	x2ze_vis[];				// Zähler R37_2
extern	char	zlt_vis;					// Datue
extern	char	syc_vis;					// Synchronisation Zähler R36
extern	char	tes_vis;					// Test
extern	char	sys_vis;					// System
extern  char	r37vis[];
extern  char	r38vis[];
extern  char	r39vis[];
extern  char	r33vis[];

/***** ulsch : Heizgradtage *****/
extern char	hgt_vis;						// Heizgradtage

//Sichtbarkeitsspeicher für Parameter
extern	char	vis;							// Normal sichtbare Parameter
extern	char	hid1;							// Mit Service-Code 1	sichtbare Parameter	(135)
extern	char	hid2;							// Mit Service-Code 2	sichtbare Parameter	(137)
extern	char	hid3;							// Mit Service-Code 3	sichtbare Parameter	(143)
extern	char	hid4;							// Mit Service-Code 4	sichtbare Parameter	(139)
extern	char	kom;							// Nur für Komtab sichtbar

// Projekttyp 
extern char	proj_typ;					// Auswahl eines implemetierten Anlagen-Projektes
// Projekthilfsgrößen, Auswertung in "Output.c"
extern char temp_proj_typ;		// temporär (für Bedien) mit Prüfung und Auswertung 
extern char	newproj;					// neues Projekt aktiviert (Zählzelle down bis Reset)
extern char	wpause[];					// Wechselpause zwischen AUF/ZU bei verriegelten Ausgängen
extern char	dadat;						// vom R50 R3X_DADAT-Kommando
extern char	dadat_cd;					// 						""		-Countdown
extern char	aadat[];					// vom R50 R3X_AADAT-Kommando
extern char	aadat_cd;					// 						""		-Countdown

// Typ und Version (wird geladen entsprechend proj_typ)
extern char Version_PTyp[];
extern char Version_PVers[];
extern char namur;

// Raumtemperaturen für verschiedene Lüftungskreise, Übertragung vom R50
extern mwsp raumtemp[];			
extern int  zuluft;
extern char ablufter;					// Anzahl der Ablüfter im Projekt

// ------------------- Pointer-Felder -------------------------------------------

// --------------------- Temperaturen Pt1000	(Felder von Zeigern auf pt_mewe[i])
extern mwsp *TA[];					// Aussentemp
extern mwsp *TVP[];					// Vorlauf  Primär
extern mwsp *TRP[];					// Rücklauf Primär

extern mwsp *TVS[];					// Vorlauf  Sekundär
extern mwsp *TRS[];					// Rücklauf Sekundär
extern mwsp *TI[];					// Raumtemperaturen

extern mwsp *TWWC[];				// Temperatur nach PWÜ
extern mwsp *TWW1L[];				// Speichertemp. 1 oben
extern mwsp *TWW2L[];				// Speichertemp. 2 unten
extern mwsp *TWWV[];				// Warmwassertemp.
extern mwsp *TWWP[];				// Temperatur vor PWÜ
extern mwsp *TWWZ[];				// Zirkulationstemperatur

extern mwsp *TVKE[];				// Vorlauf  Kessel
extern mwsp *TRKE[];				// Rücklauf  Kessel
extern mwsp *TVGKE[];				// Gemeinsamer Kesselvorlauf
extern mwsp *TRGKE[];				// Gemeinsamer Kesselrücklauf

extern mwsp *TVSEK[];				// Vorlauf Sek. Netztrennung
extern mwsp *TRSEK[];				// Rücklauf Sek. Netztrennung
extern mwsp *TVSGNT[];			// Vorlauf Sekundär gesamt
extern mwsp *TRSGNT[];			// Rücklauf Sekundär WT gesamt
extern mwsp *TRPNT[];				// Rücklauf prim. Netztrennung

extern mwsp *TKOLL[];				// Temperatur am Solar-Kollektor
extern mwsp *TPUF1L[];			// Solar-Puffer-Temp. 1 oben
extern mwsp *TPUF2L[];			// Solar-Puffer-Temp. 2 unten
extern mwsp *TWWCSO[];			// Regeltemp.am Solar-Übertrager

extern mwsp *TP_UNI[];			// universeller Pt1000-Eingang

// SiWa 27.02.18 - Vordefinierte Fühler für WPU
extern mwsp *TV_WP[];					// Vorlauf Wärmepumpe zum Speicher
extern mwsp *TR_WP[];			// Rücklauf Wärmepumpe zum Speicher
extern mwsp *TSPo_WP[];		// Speicher oben Wärmepumpe
extern mwsp *TSPu_WP[];		// Speicher unten Wärmepumpe
extern mwsp *TVST_WP[];		// Vorlauf Wärmepumpe zur Station
extern mwsp *TRST_WP[];		// Rücklauf Wärmepumpe zur Station
extern mwsp *TVQ_WP[];		// Vorlauf Wärmepumpe quellenseitig
extern mwsp *TRQ_WP[];		// Rücklauf Wärmepumpe quellenseitig
extern mwsp *TVH_WP[];		// Vorlauf Heißgas
extern mwsp *TRH_WP[];		// Rücklauf Heißgas
extern mwsp *TSPm_WP[];		// Speicher Mitte Wärmepumpe

extern mwsp **mwpadr;				// Arbeitsspeicher



// ---------------------- Analogeingänge 0-10V (Felder von Zeigern auf ae_mewe[i])
extern mwsp *LQF[];						// Beispiel: Luftqualitätsfühler
extern mwsp *AE_UNI[];
extern mwsp *ADAPT[];					// Adaptions-Kennwert

#if TAE_ANZ == 1
extern mwsp *TAE[];						// Außentemperatur
#endif

extern mwsp **aepadr;							// Arbeitsspeicher

// ---------------------- Digitaleingänge			(Felder von Zeigern auf di_mewe[i])
extern dinpsp	*STBHK[];
extern dinpsp	*STBWW[];
extern dinpsp	*STWHK[];
extern dinpsp	*STWWW[];
extern dinpsp	*WWUW[];				
extern dinpsp	*WWOW[];
extern dinpsp	*KEAL[];					// Kesselalarm Si-Kette
extern dinpsp	*BRAL[];					// Brenneralarm 
extern dinpsp	*NACHSP[];				// Nachspeisung
extern dinpsp	*PUAL[];					// Pumpenstörung 
extern dinpsp* PUWZAL[];				// Pumpenstoerung Zirkulationspumpe (Sammelmeldung)
extern dinpsp* PUWLAL[];				// Stoermeldung (Ent-)Ladepumpe
extern dinpsp* STBWWSP[];				// Stoermeldung STB Speicher
extern dinpsp	*BMKES[];					// Betriebsmeldung Kessel
extern dinpsp	*PUSAMAL[];				// Sammelmeldung Pumpenstörung
extern dinpsp	*STBNT[];					// Sicherh.temp.begrenzer Netztrennung
extern dinpsp	*STWNT[];					// Sicherh.temp.wächter Netztrennung
extern dinpsp	*RKZUNT[];				// RM Klappe ZU NT
extern dinpsp	*RKAUFNT[];				// RM Klappe AUF NT
extern dinpsp	*DRMAXNT[];				// Max-Druck NT
extern dinpsp	*DRMASNT[];				// Max-Druck NT sek.
extern dinpsp	*DRMISNT[];				// Min-Druck NT sek.
extern dinpsp	*RVZUNT[];				// RM Ventil ZU NT
extern dinpsp	*BMPUNT[];				// Betriebsmeldung Hauptpumpe
extern dinpsp	*SAMAL[];					// Sammelstörmeldung allgemein
extern dinpsp	*PUWTAL[];				// Stoermeldung Tauscherpumpe
extern dinpsp	*FRGHK[];					// Freigabe Regelung Heizkreis
extern dinpsp	*FERINP[];				// Ferienschalter
extern dinpsp	*DE_UNI[];
extern dinpsp	*SM_UNI[];				// universeller Digitaleingang für Störmeldungen
extern dinpsp	*BM_UNI[];				// universeller Digitaleingang für Betriebsmeldungen
extern dinpsp	*PUKEAL[];
extern dinpsp	*BMPU[];			// Betriebsmeldung Heizungspumpe
extern dinpsp	*BMPUWL[];		// Betriebsmeldung Ladepumpe
extern dinpsp	*BMPUWZ[];		// Betriebsmeldung Zirkulationspumpe
extern dinpsp	*BMPUWT[];		// Betriebsmeldung Tauscherpumpe
extern dinpsp	*HKAUS[];			// Anlagenschalter Nichtnutzung HK 
extern dinpsp	*WWAUS[];			// Anlagenschalter Nichtnutzung WWB
extern dinpsp	*QUITTS[];		// Quittierungstaste
extern dinpsp	*SSFKE[];			// Schornsteinfegertaste
extern dinpsp	*STBKE[];			// Sicherh.temp.wächter Kessel
extern dinpsp	*NOTAUS[];		// Not-Aus Kessel
extern dinpsp	*DRKE[];			// SM Max-Druck Kessel
extern dinpsp	*BMPUKE[];		// BM Kesselpumpe
extern dinpsp	*ADAPT_T[];		// Adaptionsmeldung "zu tief"
extern dinpsp	*ADAPT_H[];		// Adaptionsmeldung "zu hoch"
#if ( PUDO == 1 )
	extern dinpsp	*PUDAL[HKMAX];	// Pumpenstörung, Doppelpumpe
#endif
extern dinpsp	*PUALNT[];			// Pumpenstörung Hauptpumpe

extern dinpsp	**dipadr;				// Arbeitsspeicher

// ---------------------- Zähleingänge R36			(Felder von Zeigern auf zaehler[i])
extern zaehlsp *ZE[8];
// ---------------------- Zähleingänge R37_1		(Felder von Zeigern auf zaehler[i])
extern zaehlsp *X1ZE[6];
// ---------------------- Zähleingänge R37_2		(Felder von Zeigern auf zaehler[i])
extern zaehlsp *X2ZE[6];
 
// MBus: 8 Zähler
#if MBUSANZ > 0
extern zaehlsp	mbZaehler[];								// Zähleingänge
extern zaehlsp *MBZE[];
#endif

// ---------------------- Analogausgänge				(Felder von Zeigern auf Aout[i])
extern aasp *AA_UNI[];
extern aasp *RVENT[];						// Regelventil für Heizkreise
extern aasp *RVENTWW[];					// Regelventil für Warmwasser
extern aasp *RVENTKE[];					// Regelwert für Kessel
extern aasp *RVENTKR[];					// Regelventil für Kesselrücklauf-Anhebung
extern aasp *DZPUKE[];					// Drehzahl Kesselpumpe
extern aasp *AASSTM[];					// Sammelstörmeldung Analogausgabe
extern aasp *PUWZ[];						// Drehzahl Warmwasser-Zirkulationspumpe
extern aasp *PUWL[];						// Drehzahl Ladepumpe (HAST-AKKU : Entladepumpe )
extern aasp *RVENTSP[];					// Strahlpumpe Warmwasser
extern aasp *RVENTMI[];					// Mischer Warmwasser
extern aasp *RVENTNT[];					// Regelventil Netztrennung
extern aasp *RVENTSO[];					// Regelventil WW-Solar

#if TAA_ANZ > 0
extern aasp *TAA[];								// Außentemperatur
#endif

#if ANFORD > 0
extern aasp *TMANF[];
#endif

extern aasp **aapadr;			// Arbeitsspeicher

// ---------------------- Relaisausgänge				(Felder von Zeigern auf Dout[i])
extern doutsp *RZU[];						// HK Stellventile zufahren
extern doutsp *RAUF[];					// HK Stellventile auffahren
extern doutsp *RWWZU[];					// WW Stellventil zufahren
extern doutsp *RWWAUF[];				// WW Stellventil auffahren
extern doutsp *PU[];						// Heizungspumpen
extern doutsp *PUMIN[];					// Minimaldrehzahl
extern doutsp *PUWLEA[];				// Ladepumpen 
extern doutsp *PUWZEA[];				// Zirkulationspumpen
extern doutsp *ELVENT[];
extern doutsp *PUGWW[];
extern doutsp *SSTM[];					// Sammelstörmeldung
extern doutsp *KESTU1[];				// Kessel Brennerstufe 1
extern doutsp *KESTU2[];				// Kessel Brennerstufe 2
extern doutsp *KE[];						// Kessel Freigabe
extern doutsp	*KLKEAZ[];				// Kesselabsperrklappe AUF/ZU
extern doutsp	*PUWTEA[];				// Tauscherpumpe
extern doutsp	*RNTZU[];					// Netztrennung Regelventil zufahren
extern doutsp	*RNTAUF[];				// Netztrennung Regelventil auffahren
extern doutsp	*KLNTAZ[];				// Tauscher-Absperrklappe AUF/ZU
extern doutsp	*FEROUT[];				// Ferienstatus
//ulsch
extern doutsp	*DA_UNI[];
extern doutsp	*MKEZU[];					// Kessel Modulation ZU
extern doutsp	*MKEAUF[];				// Kessel Modulation AUF
extern doutsp	*RKEZU[];					// Rücklauf-Anhebung: Kesselkreis ZU
extern doutsp	*RKEAUF[];				// Rücklauf-Anhebung: Kesselkreis AUF
extern doutsp	*PUKEEA[];				// Kesselpumpe
extern doutsp	*PUKOLEA[];				// Solar-Kollektor-Pumpe
extern doutsp	*PUSOEA[];				// Solar-Pumpe				
//ulsch
extern doutsp	*KLKEZU[];				// Kesselabsperrklappe ZU
extern doutsp	*KLKEAUF[];				// Kesselabsperrklappe AUF

#if PUDO == 1 
	extern doutsp	*PUD[];					// Heizungspumpen, Doppelpumpe
#endif

extern doutsp	*PUNTEA[];				// Pumpe Netztrennung (Hauptpumpe)

extern doutsp	**dopadr;					// Arbeitsspeicher

//--------------------------------------------------------------
// Speicherplätze für In/Out-Module
//-------------------------------------------------------
extern Ram37	mod37[];			// maximal 4 (dann keine weiteren Module)
extern Ram38	mod38[];			// maximal 4 (dann keine weiteren Module)
extern Ram39	mod39[];			// maximal 4 (dann keine weiteren Module)
extern Ram33	mod33[];			// maximal 4 (dann keine weiteren Module)
extern char mod39sysinit;		// Merkmal für SysEEP_InitUser  (Kaltstart 66 wurde ausgeführt)	
																	
extern char proc_IO;				// Auftragsmerker
extern char iocnt;					// Counter über maximal 4 Module       (ohne EA-Simulation)
extern char iocnt_max;			// Counter über maximal 4 Module + R66 (mit  EA-Simulation)
extern char oready;					// Flag: wenn Output-Task abgelaufen
// EA-Simulation
extern char names_anford;
extern char projekt_anford;
extern char r37text_cnt[];
extern char r38text_cnt[];
extern char r39text_cnt[];
extern char r33text_cnt[];
extern char r66text_cnt;
//--------------------------------------------------------------

// Sammelstörmeldung für Anzeige und LT
extern char sstm[];				// Feld mit Zählerständen der jeweiligen Sammelalarmklasse, Index entsprechend userdef.h 
extern char sstm_old[];		// Merker für Event.c (Erhöhung des Zählerstandes bei Sammelalarmen (neuer Alarm) )

// Schaltpause für statische Relais, wenn mehrere Relais gleichzeitig einschalten wollen
extern char Schalt_Pause;		// Sekunden
extern char schalt_count;

/*-------------- Absenkung nach Zeit für mehrere Heizkreise --------------*/
extern char	 abx;									// Reglerindex für Absenkung
extern char	 setwoche[];			// Schalter Absenkzeiten"Woche" kopieren auf alle Tage
extern absenktab abs_tab[][8];	// Absenkzeiten und Temperaturen
/*-------------- Absenkung bei Bedarf für mehrere Heizkreise -------------*/
extern char	 bex;								// Reglerindex für Bedarfs-Absenkung


/*-------------------- Steuer ----------------------------------------*/

// Skalierungswerte für Analogausgabe R36
extern aask		AASkal[];		// Grundwerte (EEPROM gestützt, Voreinstellung in UserReply.c bei Kaltstart 66)
extern UINT		AoutSk[];		// aktuelle skalierte Ausgabewerte; Umrechnung Aout[i]->awert nach AoutSk[i] in Output.c

// gedämpfte Außenlufttemperatur 
extern mwsp		ta1m;				// gedämpfte Außentemp.
//extern float	fl_ta1m;		//		"								(für Filterberechnung) ist im Noinit-Rambereich
extern char		Ta1mzk;			// Filterzeitkonstante 				[min]
extern char		steustart;	// Steuerstart nach Reset

extern mwsp  bcta;				// Broadcast Außentemp. + Status
extern char  bc_vorra;		// Vorrang Broadcast-Außentemp.
extern char  bc_flag;			// Flag = 1 wenn ta von bcta abstammt 
extern char  bc_simu;			// Simulation eines BCTA1-Kommandos
extern mwsp  ta;					// Arbeitsspeicher für Außentemperatur

// gemittelte Außenlufttemperatur über Stunden
extern mwsp		ta1mh;			// gedämpfte Außentemp. + Status
//extern float	fl_ta1mh;		//		"			(für Filterberechnung) ist im Noinit-Rambereich
extern int		ta1mh_temp;	// Eingebbarer Parameter zum Einstellen der gedämpften Ta 

extern char		Ta1mhzk;		// Filterzeitkonstante 				[h]
extern char		zzta1mh;		// Zähler für Tastzeit
extern char		zzsave;			// Zeitzähler für Ablage im EEPROM 

// Sommer Winter,	zentrale Parameter in ANL
extern int		Tae;			// Temp. Winter ein						 [°C] * 10	
extern int		Taa;			// Temp. Sommer aus						 [°C] * 10	
extern char		sowi;			// 1 = Sommer

#if ANF_EXT > 0
extern mwsp anfExt[];							// Anforderung über Bus
#endif

extern int maxAnford;

/*--------------- Reglerparameter für alle Heizkreise ----------------------*/
//	Die Parameternamen sind in den Strukturdefinitionen festgelegt (ustruct.h) 

// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
extern HkStandard	hks[];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
extern HkDynam		hkd[];

/*--------------- Reglerparameter für mehrere Warmwasserkreise -------*/
// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
extern WwStandard	wws[];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
extern WwDynam		wwd[];

/*--------------- Reglerparameter für mehrere Kesselkreise -------*/
// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
extern KeStandard	kes[];

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
extern KeDynam		ked[];

/*--------------- Reglerparameter für mehrere Vorregelkreise (Netztrennung)------*/
//	Die Parameternamen sind in den Strukturdefinitionen festgelegt (ustruct.h) 

// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
extern NtStandard	nts[NTMAX];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Vorregelprogramme
extern NtDynam			ntd[NTMAX];

/*--------------- Reglerparameter für Wärmepumpe -------------*/
extern WpStandard	wps[WPMAX];
extern WpDynam	wpd[WPMAX];

/*--------------- Reglerparameter für Solar-Regelung -------------*/
extern SoStandard	sos[SOLMAX];
extern SoDynam	sod[SOLMAX];

/* Regelparameter für Benutzersteuerung mit UNI-Elementen --------*/
extern UniStandard unis[1];
extern UniDynam    unid[1];


/*--------------- Alarmtabelle -----------------------------------*/
extern AlarmVar	alarmtab[];

extern char quit_taste;
extern char un_qsm;							// 1 = Unquittierte Störungen vorhanden
extern char unbek_Alarm;				// unbekannter Alarm (alarmtab - Index)
extern char sstm_alarme;				// Merker für Alarme, für SSTM-Relais, Eintrag in parli für KomtabCopy
extern char sstm_all;						// Merker für alle Alarme inclusive Fühler, Eintrag in parli für KomtabCopy

extern UINT alarmlist_dae[];		// aktuelle Alarme in einer Liste merken (für RFB):  2 Byte DAE-Nummer
extern char alarmanz_dae;

/*---------------- Zählerspeicherung pro Monat ------------------------*/
extern char	std_old;
extern char	monza_first;
extern char	monza_znum;
// Rest verlagert nach unoinitext.h

/***** ulsch : Archivparameter *****/
extern char arch_par;						// char = UCHAR !	
extern char	arp_vis;
extern char archivAktiv;
extern char ParaBuffer[];

/***** ulsch Adaption*****/
/*---------------- Adaption --------------------------------------------------------*/
extern char mwAdapt[HKMAX][ADAP_STAT_MAX];		// uebermittelte Werte 2,5,8
extern char adapt_countdown[];								// Wert in min, empfangen vom R50 über Kommando WR_ADAKENNZ

#if ( ADAP_STAT_HK1 > 0 || ADAP_STAT_HK2 > 0 || ADAP_STAT_HK3 > 0 || ADAP_STAT_HK4 > 0 )
extern mwsp mwAdaption[HKMAX][ADAP_STAT_MAX];	// Kopie von mwAdapt mit selbst gebildetem Status ( Datenuebertrag.fehler .stat = 1 )
extern char AdaptMeldg[HKMAX][16];
extern int AdaptKorr[HKMAX][10];							// 10 temperaturspez.Adaptionswerte fuer -25 grd C ...+20 grd C, in 0,1 grd
extern char tvsHigh[];												// Zaehler zur Ueberwachung der Vorlauf-Temperatur
extern char tvsLow[];
extern char tvsNormal[];
extern char leistgLow[];											// Zaehler
extern char leistgHigh[];
#endif

/***** ulsch : Ferienprogramm *****/
extern char ferienStat;					// Zustand Ferien	
extern ferienZeit ferien[];			// Feld fuer 10 Ferien-Termine

extern char absenkStat;					// Zustand Absenkung

/***** ulsch : Heizgradtage *****/
#if HEIZGRD == 1
extern char indexMon;
extern int ta7;
extern int ta14;								// Speicherplatz unmittelbar nach ta7 !
extern monHzGrd sMonHzGrd;
extern monHzGrd sMonHzGrdAnz;
extern char merkerStd;
extern char monHzGrdAnz;
#endif

/***** ulsch : Waermemenge, Diagnose *****/
#if WMENG > 0
extern zaehlspWmeng wmengCtr[];
extern zaehlsp wmengCalc[4];		// für interne Wärmemengen-Zähler, für Zählerobjekt benötigt
#endif

#if ( LEIST_BER > 0 )
extern maxLeistung MaxLeistung[];
#endif

#if ( LEIST_BEGR > 0 )
extern grenzLeistungStat GrenzLeistung;
extern grenzLeistungDyn grenzLeistung;
extern UINT AnzahlBegrenzung;
extern UINT DauerBegrenzung;
#endif

#if DIAG == 1	
extern diagnose did;
extern diagParam dis;
#endif

//#####josch:
extern copydata CopyData[];
extern char		 	LeerData[];
extern int			CopySend[];
extern int			LeerSend[];

extern char ssfEinAnl;	// Schornsteinfeger-Funktion ( KE1 OR KE2 )	

#if TAE_ANZ == 1
extern mwsp ta_ae;
extern int TaeSkalMin;
extern int TaeSkalMax;
#endif

#if TAA_ANZ > 0
extern int TaaSkalMin;
extern int TaaSkalMax;
#endif

#if ANFORD == 1
extern int TmanfSkalMin;
extern int TmanfSkalMax;
extern int TmanfSkalMinSpg;
extern int TmanfSkalMaxSpg;
#endif
 
extern sAnaInpPara AnaInpPara[AE_UNI_ANZ];
extern sAnaInp anaInp[AE_UNI_ANZ];

#if KEANZ > 1
extern struct sKes KesEin;
extern struct sKes KesSperre;
extern struct sKes KesGestoert;
extern char KesFolgeSoll[];
extern char KesFolgeIst[];
extern char AnzahlKesEin;
//extern char KesParVis[KEMAX];
extern mwsp *VL_STRATEG;
extern mwsp *VL_FOLGE_AUS; 
extern mwsp *RL_FOLGE_AUS;
extern mwsp *VL_ANHEB;
#endif	// #if KEANZ > 1

// ***** josch: Datenmanager ***************************************************************
// Gerät im DS_Modus (Data Slave),	Auswertung in der Funktion DReply() 
#if DS_MODE == 1
extern char DS_RxBuf[DS_RXBUF_MAX];				// vom Master empfangene Daten
extern char DS_RxTout;										// Timeout in Minuten
extern char DS_RxStat;										// Status der Empfangsdaten
extern char DS_TxBuf[DS_TXBUF_MAX];				// an den Master zu sendende Daten
extern char DS_TxLeng;										// Datenanzahl zu senden

// Empfangene Daten vom Master (Variable müssen vom Typ mwsp sein)
extern mwsp zentrale_ea;									// Beispiel für 1 Byte: Ein/Aus              
extern mwsp zentrale_istwert;							// Beispiel für 3 Byte: Temperatur mit Status
extern mwsp zentrale_sollwert;						// Beispiel für 2 Byte: Sollwert             

#endif

// Gerät im DM_Modus (Data Master),	Datenbearbeitung in der Funktion DManager(), Organisation der Datenübertragung im Task DTimer() 
#if DM_MODE == 1
extern dm_param DM[DM_SLAVES];						// DM Struktur
extern char dtslave;											// DTimer aktueller Slave
extern char dtstate;											// DTimer aktueller Bearbeitungsstatus
extern char dtauftrag;										// DTimer aktueller Auftrag
extern char proc_TI;											// DTimer Auftragsmerker 
extern char Slave[4];											// Slave 1 - 4 Adressen

// Empfangene Daten von den Slaves  (Variable müssen vom Typ mwsp sein)
extern mwsp station1_ea;									// Beispiel für 1 Byte: Ein/Aus              
extern mwsp station1_istwert;							// Beispiel für 3 Byte: Temperatur mit Status
extern mwsp station1_sollwert;						// Beispiel für 2 Byte: Sollwert             

	#if DM_SLAVES > 1
extern mwsp station2_ea;			
extern mwsp station2_istwert;
extern mwsp station2_sollwert;
	#endif

#endif

// ***** Josch-SDM : Archiv mit SD-Card-Memory ********************************************************* 
#if ARCHIV_SDM == 1
extern UINT	werks_sdm_code;						// Code eingeben für Werkseinstellung
extern UINT	sdm_akt_dae;
extern char	a_sperre;
extern char	sdm_arch_stop;						// Benutzer-Stopp (1), wird gesichert in Standard.c 
extern char	mount_check;
extern char	test_start;
extern char	sdm_base_check;
extern char	sdm_alarm_check;

// Funktion: Zeitmessung (in Upro.c)
// IN:       char  onoff ...ON (1=Start), OFF ( 0=Stop)
// OUT:      ULONG Zeit.....ms
#if ZEIT_MESSUNG == 1
extern ULONG read_zeit;
extern ULONG write_zeit;	
extern ULONG write_zeit_max;
#endif

extern char	c_erg_gl;									// Ergebnis der Card-Zugriffe veröffentlichen in der parli
extern char c_erg_text[16];
extern char	a_erg_gl;									// Ergebnis der Archivierung  veröffentlichen in der parli
extern char a_erg_text[16];
extern char	cardType;
extern char	speedClass;
extern char	performanceMove;
extern char ferro_err;
extern char card_err;

extern char	bic_err;
extern char	asdm_status;
extern char	asdm_action;
extern char	sdm_ret;										// Fehleranzeige
extern char	asdm_minut;
extern ULONG asdm_time;									// Zeitmarke aktuell
extern ULONG read_time;									// Zeitmarke: Erster Zeitstempel der Datenpage, deren Eintrag in der Verweispage fehlt
extern char asdm_buffer[64];						// Puffer für Zeitstempel und Datenwert
extern char	asdm_page[512];							// Puffer für eine Page
extern ULONG timeout_max;

extern UINT	current_dae;
extern char	current_dae_exp;
extern char	max_expand;
extern char	exp_index;
extern UINT	dae_append;
extern UINT	komtab_changed;
extern char	daily_init;
extern int  reset_dae;									// ferro Init für ein DAE

extern UINT	ferro_adr;
extern UINT	ferro_mnext;
extern UINT	ferro_msize;
extern ferro_info	dae_info[1];
extern ferro_info *exp_ferro_tab[256];			// Pointer auf Expanddaten-Tabelle im alloziierten Ferro-Bereich

// DAE Archivierung
extern ULONG hp_adr;										// aktuelle Adresse der Hauptpage.    Anzahl 1
extern ULONG vp_adr;										// aktuelle Adresse der Verweispage.  Anzahl 15
extern ULONG dp_adr;										// aktuelle Adresse der Datenpage.    Anzahl 2032
extern ULONG sdm_mnext;									// Expand-Allocation: nächster freier Platz
extern ULONG sdm_msize;									// Expand-Allocation: Größe des verfügbaren Speicherbereiches
extern ULONG exp_sdm_tab[256];					// Expand-Allocation; die Expanddaten-Tabelle enthält alle alloziierten Hauptpage-Adressen

// DAE Suche
extern ULONG search_hp_adr;							// gefundene Adresse der Hauptpage.
extern UINT  search_vp_num;							// gefundene Verweispagenummer in der Hauptpage (1-15)
extern ULONG search_vp_adr;							// gefundene Adresse der Verweispage.
extern UINT  search_dp_num;							// gefundene Datenpagenummer in der Verweispage (1-2032)
extern ULONG search_dp_adr;							// gefundene Adresse der Datenpage.
extern UINT  search_ts_num;							// gefundene Zeitstempelnummer in der Datenpage
extern char	 sd_buf[512];								// Puffer für eine Page

#if SPEED_SAVE == 1			//$ Test
extern ULONG asdm_test_start;
#endif

#endif	// ARCHIV_SDM == 1

#if RM_POWER_ANZ
extern mwsp *RM_POWER[];
extern sPowInpPara RmPowerPara[RM_POWER_ANZ];
extern sPowInp rmPower[RM_POWER_ANZ];
#endif

#if AE_DRUCK_ANZ
extern mwsp *AE_DRUCK[];
extern sAnaInpPara DruckPara[AE_DRUCK_ANZ];
extern sAnaInp druck[AE_DRUCK_ANZ];
#endif

#endif	// URAMEXT_H_INCLUDED
