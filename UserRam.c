/*						UserRam.c														 */
/*	Nicht	intialisierte	User - Variablen					 */
/*	Bereich wird nach Reset gelöscht							 */

#include "defins.h"
#include "struct.h"
#include "ustruct.h"
#include "userdef.h"
#include "projdef.h"
#include "uconstext.h"
#include "evetab.h"
#include "archivsdm.h"	// Josch-SDM

int k_dummy;

char testClear;
char char_test1;
char char_test2;
int  int_test;
mwsp *temperatur;
mwsp mod_rt[2];

int TaErsatz[12];
UINT TaErsatzCtr;
char TaErsatzVerz;

// NLT Neue Leittechnik
events prim_evtab[PETAB_DEFL];
events seku_evtab[SETAB_DEFL];
char sms_feld[PETAB_DEFL];				// Merker für Feldindex prim_evtab

/* Konfigurations-Anzeigebytes */
char ptinput1;
char ptinput2;
char aeinput1;
char aeinput2;
char diginput1;
char diginput2;
char digtherm1;
char digtherm2;
char digoutput1;
char digoutput2;
char anaoutput1;
char anaoutput2;
char zinput1;					// R36
char zinput2;					// R36
char zinput_r37[4];		// R37_1 und R37_2  ( R37_3 und R37_4 nicht benutzt, aber Speicherplatz reserviert )

char ort_idf;
char ort_idf_flag;

// Objekt- und Standortbezeichnungen
char Objektbez [16];		
char Objektnum [10];		
char Strasse   [16];		
char Nummer    [10];		
char Ort       [16];		
char Plz       [10];		
char Version_PVers_Ascii[10];	// Programmversion

//--------------------------------------------------------------------------------------------------
// MBUS
#if ( ((IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL) )

MbusDynam MbusData[MBUSANZ];
MbusStandard MbusPara[MBUSANZ];
UINT MbusFrequ;
char MbusAktualisierung;
char MbusCtrAktiv;
char MbusStatus;
UINT Timeout;
char RxStatus;

char MbusTxCtr;
char MbusTxLng;
char MbusRxCtr;
char MbusRxLng;
char *pMbusTx;
char *pMbusRx;

char MbusTxBuf[16];
char MbusRxBuf[256];

char MbCtrAkt;
//Sichtbarkeit Sichtbar[MBUSANZ];
Sichtbarkeit Sichtbar;
uhrzeit AktStd;
uhrzeit LetzteAkt;
char mbus_vis;
char zaehlerNummer;

#endif

//--------------------------------------------------------------------------------------------------
// Benutzung der Funktion Test-RS485
// Aufruf durch HandlerS1, HandlerS2 oder HandlerS3 organisiert durch UserHandler
#if ( ((IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL) || ((IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL) || ((IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL) )

serial485	serial[3];

#endif

//--------------------------------------------------------------------------------------------------
// Benutzung der Funktion Funkempfang
// Aufruf durch HandlerS1, HandlerS2 oder HandlerS3 organisiert durch UserHandler
#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )

char ucReceiverTyp_RF;		//090310 0=NEU 1=ALT
char ucCond_RF;
char ucRxFlag_RF;
char ucChecksum_RF;
char ucBcounter_RF;
char ucfRxBuf_RF[12];
char ucRF_ComPort;				// muß in den EEPROM
rfsens	rf_feld[12];
char ucTeach_idx;
char bedien_rf_nummer;
char funk_vis;

#endif

//--------------------------------------------------------------------------------------------------
// Grundfos-Pumpen mit GENIbus 
//--------------------------------------------------------------------------------------------------
#if ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )

// Neue Genibus Variablen
	GeniDeviceTable genibus_device_table[GENI_DEVICE_MAX];
	
	GeniValues Geni_Values[GENI_DEVICE_MAX];
		
	unsigned char uc_geni_dummy_stat;		// Dummy für Statusspeicher
	
	unsigned char uc_genibus_device_nr;
	
	unsigned char ucf_genibus_rx_buffer[GENIBUS_RX_BUFFER_SIZE];
	unsigned char uc_genibus_rx_count;
	unsigned char uc_genibus_rx_buff_size;
	unsigned char uc_genibus_rx_errorflag;		// ToDo: Geordnete Fehlerbehandlung / Anzeige
	unsigned char uc_genibus_rx_error;
	unsigned char uc_genibus_rx_telegram_ready;
	unsigned char uc_genibus_rx_timeout_ctr;
	
	unsigned char uc_genibus_tx_count;
	unsigned char uc_genibus_tx_lenght;

	unsigned char uc_genibus_rts_prescaler; 
	unsigned char uc_genibus_rts_timer;
	
	unsigned char uc_genibus_state;
	unsigned char uc_genibus_special_function;
	
	unsigned char uc_genibus_own_adr;
	
	unsigned char uc_genibus_get_adress;
	unsigned char uc_genibus_reset_pump;			// Adresse des zu resetenden Gerätes
	unsigned char uc_genibus_reset_result;
	unsigned char uc_genibus_start_manual_request;
	GeniManualRequest genibus_manual_request;
	
	unsigned char uc_genibus_request_table_idx;
	
	unsigned int  ui_genibus_relay_ctr;
	
	unsigned char uc_genibus_func_test;
#endif

//----------------------------------------------------------------------------------------------------
// Wilo-Pumpen mit MODBUS
//----------------------------------------------------------------------------------------------------
#if WILO_MODBUS == 1

char cWiloPuCount; // Anzeigecounter für Anzuzeigende Pumpe in parli-Anzeige

PuBusStandard *wiloPuBusPara;						// Modbus Struktur für Pumpenparameter (Adresse usw)
PuBusDynam *wiloPuBusData;

ModBusWiloPumpen modWiloPuData; 				// Modbus Struktur für Modbustabelle (Datenanzeige und Daten holen getrennt halten!!!)
ModBusWiloPumpen modWiloPu[BUS_PU_MAX];	// Struktur für Pumpenparamter aus Kommunikation übertragen auf die jeweiligen Wilo Pumpen 

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

	PuBusStandard BusPuPara[BUS_PU_MAX];
	PuBusDynam BusPuData[BUS_PU_MAX];
	char ssmBusPu;
	char ssmPuCtr;

#endif

//----------------------------------------------------------------------------------------------------
// Modbus 
//--------------------------------------------------------------------------------------------------
#if MODBUS_UNI > 0

unsigned int ModBusKonvertError;									// Modbus.c  + modbusparli.h
unsigned int modbusDeviceNr;                      // Modbus.c  + modbusparli.h
int activeRow[3];                                 // Modbus.c  + modbusparli.h
unsigned char modbusSlaveAddresses[3];            // Modbus.c  + modbusparli.h  + modbusstandard.h
unsigned char modbusSioParity[3];                 // Modbus.c  + modbusparli.h  + modbusstandard.h
unsigned char modbusSioStopBits[3];               // Modbus.c  + modbusparli.h  + modbusstandard.h
unsigned char modbusConvertBuffer[3][256];        // Modbus.c   

char modb_leng_tx[3];                             // ModBusSio.c
char modb_rts_prescaler[3];                       // ModBusSio.c
char modb_rts_timer[3];                           // ModBusSio.c
char modb_rx_buff_size;                           // ModBusSio.c
char modb_rx_status;                              // Modbus.c                   
char modb_tx_count[3];                            // ModBusSio.c
char modb_rx_count[3];                            // ModBusSio.c              
char modb_rx_buffer_ready[3];                     // ModBusSio.c              
char modb_rx_int_state[3];                        // ModBusSio.c              
char modb_sio_errorflag[3];                       // ModBusSio.c
unsigned int EoF_timer[3];                        // ModBusSio.c
char timer_function[3];                           // ModBusSio.c
char func_code[3];                                // ModBusSio.c  + Modbus.c  
unsigned char byte_count_or_exception[3];         // ModBusSio.c  + Modbus.c
unsigned int first_reg[3];                        // ModBusSio.c  + Modbus.c  
unsigned int cnt_regs[3];                         // ModBusSio.c  + Modbus.c  
void* reg_address[3];                             // ModBusSio.c  + Modbus.c
char modb_curr_adr[3];                            // ModBusSio.c  + Modbus.c  

/*--------------- Modbus-Datenpunkte -----------------------------*/
#if MODBUS_EXT == 1   // bei Verwendung von READ_MULTIPLE_COILS in der ModbusTabelle
ULONG ul_dig32[4];    // READ_MULTIPLE_COILS für 32 Bit
char  uc_dig[128];    // Extraktion 32Bit auf Einzelbytes im Task ModbusExtract
#endif

#endif  // MODBUS_UNI

//----------------------------------------------------------------------------------------------------
// Variabler Kategorie-Typ
char KatVar_Typ[16];
// Info über Handschalter in ASCCI
char sw_stat_info[16];

// Index-Feld für Namenslisten R36, R37, R38, R33
unsigned int nlist[18];									// 18 Ein/Ausgänge				R36
unsigned int n37list[R37_MODMAX][13];		// 13 Ein/Ausgänge				R37 
unsigned int n38list[R38_MODMAX][14];		// 14 Analogeingänge			R38
unsigned int n39list[R39_MODMAX][12];		// 12 Digitaleingänge			R39
unsigned int n33list[R33_MODMAX][6];		//  6 Digitalausgänge			R33

char ntext[18][21];											// 18 E/A je 20 ASCII
char n37text[R37_MODMAX][13][21];				// 13 Ein/Ausgänge				R37 
char n38text[R38_MODMAX][14][21];				// 13 Ein/Ausgänge				R37 
char n39text[R39_MODMAX][12][21];				// 12 Digitaleingänge			R39
char n33text[R33_MODMAX][6][21];				//  6 Digitalausgänge			R33


//Sichtbarkeitsspeicher für Parametergruppen
char	anl_vis;					// Anlage
char	ke_vis[KEMAX];		// Kessel
char	hk_vis[HKMAX];		// Heizung
char	ww_vis[WWMAX];		// Warmwasser
char	nt_vis[NTMAX];		// Netztrennung
char	so_vis[SOLMAX];		// Solar

// #####ulsch
char fer_vis;						// Ferienprogramm
char qsm_vis;						// Quittierbare Gespeicherte Störmeldungen

char	ze_vis[8];				// Zähler R36
char	x1ze_vis[6];			// Zähler R37_1
char	x2ze_vis[6];			// Zähler R37_2
char	zlt_vis;					// Datue
char	syc_vis;					// Synchronisation Zähler R36
char	tes_vis;					// Test
char	sys_vis;					// System
char	r37vis[R37_MODMAX];
char	r38vis[R38_MODMAX];
char	r39vis[R39_MODMAX];
char	r33vis[R33_MODMAX];

/***** ulsch : Heizgradtage *****/
char	hgt_vis;					// Heizgradtage

//Sichtbarkeitsspeicher für Parameter
char	vis;							// Normal sichtbare Parameter
char	hid1;							// Mit Service-Code 1	sichtbare Parameter	(135)
char	hid2;							// Mit Service-Code 2	sichtbare Parameter	(137)
char	hid3;							// Mit Service-Code 3	sichtbare Parameter	(143)
char	hid4;							// Mit Service-Code 4	sichtbare Parameter	(139)
char	kom;							// Nur für Komtab sichtbar

// Projekttyp 
char	proj_typ;					// Auswahl eines implemetierten Anlagen-Projektes
// Projekthilfsgrößen, Auswertung in "Output.c"
char  temp_proj_typ;		// temporär (für Bedien) mit Prüfung und Auswertung 
char	newproj;					// neues Projekt aktiviert (Zählzelle down bis Reset)
char	wpause[6];				// Wechselpause zwischen AUF/ZU bei verriegelten Ausgängen
char	dadat;						// vom R50 R3X_DADAT-Kommando
char	dadat_cd;					// 						""		-Countdown
char	aadat[2];					// vom R50 R3X_AADAT-Kommando
char	aadat_cd;					// 						""		-Countdown

// Typ und Version (wird geladen entsprechend proj_typ)
char Version_PTyp[16];
char Version_PVers[3];
char namur;

// Raumtemperaturen für verschiedene Lüftungskreise, Übertragung vom R50
mwsp raumtemp[4];			
int  zuluft;
char ablufter;					// Anzahl der Ablüfter im Projekt

// ------------------- Pointer-Felder -------------------------------------

// -------------- Temperaturen Pt1000	(Felder von Zeigern auf pt_mewe[i])
mwsp *TA[ANLMAX];					// Aussentemp
mwsp *TVP[ANLMAX];				// Vorlauf  Primär
mwsp *TRP[ANLMAX];				// Rücklauf Primär

mwsp *TVS[HKMAX];					// Vorlauf  Sekundär
mwsp *TRS[HKMAX];					// Rücklauf Sekundär
mwsp *TI[HKMAX];					// Raumtemperaturen

mwsp *TWWC[WWMAX];				// Temperatur nach PWÜ
mwsp *TWW1L[WWMAX];				// Speichertemp. 1 oben
mwsp *TWW2L[WWMAX];				// Speichertemp. 2 unten
mwsp *TWWV[WWMAX];				// Warmwassertemp
mwsp *TWWP[WWMAX];				// Temperatur vor PWÜ
mwsp *TWWZ[WWMAX];				// Zirkulationstemperatur

mwsp *TVKE[KEMAX];				// Vorlauf  Kessel
mwsp *TRKE[KEMAX];				// Rücklauf  Kessel
mwsp *TVGKE[KEMAX];				// Gemeinsamer Kesselvorlauf
mwsp *TRGKE[KEMAX];				// Gemeinsamer Kesselrücklauf

mwsp *TVSEK[NTMAX];				// Vorlauf Sek. Netztrennung
mwsp *TRSEK[NTMAX];				// Rücklauf Sek. Netztrennung
mwsp *TVSGNT[NTMAX];			// Vorlauf Sekundär WT gesamt
mwsp *TRSGNT[NTMAX];			// Rücklauf Sekundär WT gesamt
mwsp *TRPNT[NTMAX];				// Rücklauf prim. Netztrennung

mwsp *TKOLL[SOLMAX];			// Temperatur am Solar-Kollektor
mwsp *TPUF1L[SOLMAX];			// Solar-Puffer-Temp. 1 oben
mwsp *TPUF2L[SOLMAX];			// Solar-Puffer-Temp. 2 unten
mwsp *TWWCSO[SOLMAX];			// Regeltemp.am Solar-Übertrager

mwsp *TP_UNI[TP_UNI_ANZ]; // universeller Pt1000-Eingang

// SiWa 27.02.18 - Vordefinierte Fühler für WPU
mwsp *TV_WP[WPMAX];			// Vorlauf Wärmepumpe zum Speicher
mwsp *TR_WP[WPMAX];			// Rücklauf Wärmepumpe zum Speicher
mwsp *TSPo_WP[WPMAX];		// Speicher oben Wärmepumpe
mwsp *TSPu_WP[WPMAX];		// Speicher unten Wärmepumpe
mwsp *TVST_WP[WPMAX];		// Vorlauf Wärmepumpe zur Station
mwsp *TRST_WP[WPMAX];		// Rücklauf Wärmepumpe zur Station
mwsp *TVQ_WP[WPMAX];			// Vorlauf Wärmepumpe quellenseitig
mwsp *TRQ_WP[WPMAX];			// Rücklauf Wärmepumpe quellenseitig
mwsp *TVH_WP[WPMAX];			// Vorlauf Heißgas
mwsp *TRH_WP[WPMAX];			// Rücklauf Heißgas

mwsp **mwpadr;						// Arbeitsspeicher


// --------------- Analogeingänge 0-10V (Felder von Zeigern auf ae_mewe[i])
mwsp *LQF[1];							// Beispiel: Luftqualitätsfühler
mwsp *AE_UNI[AE_UNI_ANZ];
mwsp *ADAPT[HKANZ ? HKANZ : 1];	// Adaptions-Kennwert

#if TAE_ANZ == 1
mwsp *TAE[1];							// Außentemperatur
#endif

mwsp **aepadr;							// Arbeitsspeicher

// --------------- Digitaleingänge			(Felder von Zeigern auf di_mewe[i])
dinpsp	*STBHK[HKMAX];
dinpsp	*STBWW[WWMAX];
dinpsp	*STWHK[HKMAX];
dinpsp	*STWWW[WWMAX];
dinpsp	*WWUW[WWMAX];				
dinpsp	*WWOW[WWMAX];
dinpsp	*KEAL[KEMAX];				// Kesselalarm Si-Kette
dinpsp	*BRAL[KEMAX];				// Brenneralarm 
dinpsp	*NACHSP[KEMAX];			// Nachspeisung
dinpsp	*PUAL[HKMAX];				// Pumpenstörung 
dinpsp	*PUWZAL[WWMAX];			// Pumpenstoerung Zirkulationspumpe (Sammelmeldung)
dinpsp	*PUWLAL[WWMAX];			// Stoermeldung (Ent-)Ladepumpe
dinpsp	*STBWWSP[WWMAX];		// Stoermeldung STB Speicher
dinpsp	*BMKES[KEMAX];			// Betriebsmeldung Kessel
dinpsp	*PUSAMAL[HKMAX];		// Sammelmeldung Pumpenstörung
dinpsp	*STBNT[NTMAX];			// Sicherh.temp.begrenzer Netztrennung
dinpsp	*STWNT[NTMAX];			// Sicherh.temp.wächter Netztrennung
dinpsp	*RKZUNT[NTMAX];			// RM Klappe ZU NT
dinpsp	*RKAUFNT[NTMAX];		// RM Klappe AUF NT
dinpsp	*DRMAXNT[NTMAX];		// Max-Druck NT
dinpsp	*DRMASNT[NTMAX];		// Max-Druck NT sek.
dinpsp	*DRMISNT[NTMAX];		// Min-Druck NT sek.
dinpsp	*RVZUNT[NTMAX];			// RM Ventil ZU NT
dinpsp	*BMPUNT[NTMAX];			// Betriebsmeldung Hauptpumpe
dinpsp	*SAMAL[ANLMAX];			// Sammelstörmeldung allgemein
dinpsp	*PUWTAL[WWMAX];			// Stoermeldung Tauscherpumpe
dinpsp	*FRGHK[HKMAX];			// Freigabe Regelung Heizkreis
dinpsp	*FERINP[ANLMAX];		// Ferienschalter
dinpsp	*DE_UNI[DE_UNI_ANZ];
dinpsp	*SM_UNI[SM_UNI_ANZ];// universeller Digitaleingang für Störmeldungen
dinpsp	*BM_UNI[BM_UNI_ANZ];// universeller Digitaleingang für Betriebsmeldungen
dinpsp	*PUKEAL[KEMAX];			// Störmeldung Kesselpumpe
dinpsp	*BMPU[HKMAX];				// Betriebsmeldung Heizungspumpe
dinpsp	*BMPUWL[WWMAX];			// Betriebsmeldung Ladepumpe
dinpsp	*BMPUWZ[WWMAX];			// Betriebsmeldung Zirkulationspumpe
dinpsp	*BMPUWT[WWMAX];			// Betriebsmeldung Tauscherpumpe
dinpsp	*HKAUS[HKMAX];			// Anlagenschalter Nichtnutzung HK 
dinpsp	*WWAUS[WWMAX];			// Anlagenschalter Nichtnutzung WWB
dinpsp	*QUITTS[ANLMAX];		// Quittierungstaste
dinpsp	*SSFKE[KEMAX];			// Schornsteinfegertaste
dinpsp	*STBKE[KEMAX];			// Sicherh.temp.wächter Kessel
dinpsp	*NOTAUS[KEMAX];			// Not-Aus Kessel
dinpsp	*DRKE[KEMAX];				// SM Max-Druck Kessel
dinpsp	*BMPUKE[KEMAX];			// BM Kesselpumpe
dinpsp	*ADAPT_T[HKANZ ? HKANZ : 1];		// Adaptionsmeldung "zu tief"
dinpsp	*ADAPT_H[HKANZ ? HKANZ : 1];		// Adaptionsmeldung "zu hoch"
#if ( PUDO == 1 )
	dinpsp	*PUDAL[HKMAX];		// Pumpenstörung, Doppelpumpe
#endif
dinpsp	*PUALNT[NTMAX];			// Pumpenstörung Hauptpumpe

dinpsp	**dipadr;						// Arbeitsspeicher

// ---------------- Zähleingänge R36			(Felder von Zeigern auf zaehler[i])
zaehlsp *ZE[8];
// ---------------- Zähleingänge R37_1		(Felder von Zeigern auf zaehler[i])
zaehlsp *X1ZE[6];
// ---------------- Zähleingänge R37_2		(Felder von Zeigern auf zaehler[i])
zaehlsp *X2ZE[6];

// MBus: max.6 Zähler
#if MBUSANZ > 0
zaehlsp	mbZaehler[MBUSANZ];								// Zähleingänge
zaehlsp *MBZE[MBUSANZ];
#endif

// ---------------- Analogausgänge				(Felder von Zeigern auf Aout[i])
aasp *AA_UNI[AA_UNI_ANZ];
aasp *RVENT[HKMAX];						// Regelventil für Heizkreise
aasp *RVENTWW[WWMAX];					// Regelventil für Warmwasser
aasp *RVENTKE[KEMAX];					// Regelwert für Kessel
aasp *RVENTKR[KEMAX];					// Regelventil für Kesselrücklauf-Anhebung
aasp *DZPUKE[KEMAX];					// Drehzahl Kesselpumpe
aasp *AASSTM[2];							// Sammelstörmeldung Analogausgabe
aasp *PUWZ[WWMAX];						// Drehzahl Warmwasser-Zirkulationspumpe
aasp *PUWL[WWMAX];						// Drehzahl Ladepumpe (HAST-AKKU : Entladepumpe )
aasp *RVENTSP[WWMAX];					// Strahlpumpe Warmwasser
aasp *RVENTMI[WWMAX];					// Mischer Warmwasser
aasp *RVENTNT[NTMAX];					// Regelventil Netztrennung
aasp *RVENTSO[SOLMAX];				// Regelventil WW-Solar

#if TAA_ANZ > 0
aasp *TAA[TAA_ANZ];						// Außentemperatur
#endif

#if ANFORD > 0
aasp *TMANF[ANFORD];
#endif

aasp **aapadr;								// Arbeitsspeicher

// ---------------- Relaisausgänge				(Felder von Zeigern auf Dout[i])
doutsp	*RZU[HKMAX];					// HK Stellventile zufahren
doutsp	*RAUF[HKMAX];					// HK Stellventile auffahren
doutsp	*RWWZU[WWMAX];				// WW Stellventil zufahren
doutsp	*RWWAUF[WWMAX];				// WW Stellventil auffahren
doutsp	*PU[HKMAX];						// Heizungspumpen
doutsp	*PUMIN[HKMAX];				// Minimaldrehzahl
doutsp	*PUWLEA[WWMAX];				// Ladepumpen 
doutsp	*PUWZEA[WWMAX];				// Zirkulationspumpen
doutsp	*ELVENT[WWMAX];
doutsp	*PUGWW[WWMAX];
doutsp	*SSTM[2];							// Sammelstörmeldung
doutsp	*KESTU1[KEMAX];				// Kessel Brennerstufe 1
doutsp	*KESTU2[KEMAX];				// Kessel Brennerstufe 2
doutsp	*KE[KEMAX];						// Kessel Freigabe
doutsp	*KLKEAZ[KEMAX];				// Kesselabsperrklappe AUF/ZU
doutsp	*PUWTEA[WWMAX];				// Tauscherpumpe
doutsp	*RNTZU[NTMAX];				// Netztrennung Regelventil zufahren
doutsp	*RNTAUF[NTMAX];				// Netztrennung Regelventil auffahren
doutsp	*KLNTAZ[NTMAX];				// Tauscher-Absperrklappe AUF/ZU
doutsp	*FEROUT[1];						// Ferienstatus
//ulsch
doutsp	*DA_UNI[DA_UNI_ANZ];
doutsp	*MKEZU[KEMAX];				// Kessel Modulation ZU
doutsp	*MKEAUF[KEMAX];				// Kessel Modulation AUF
doutsp	*RKEZU[KEMAX];				// Rücklauf-Anhebung: Kesselkreis ZU
doutsp	*RKEAUF[KEMAX];				// Rücklauf-Anhebung: Kesselkreis AUF
doutsp	*PUKEEA[KEMAX];				// Kesselpumpe
doutsp	*PUKOLEA[SOLMAX];			// Solar-Kollektor-Pumpe
doutsp	*PUSOEA[SOLMAX];			// Solar-Pumpe
//ulsch
doutsp	*KLKEZU[KEMAX];				// Kesselabsperrklappe ZU
doutsp	*KLKEAUF[KEMAX];			// Kesselabsperrklappe AUF

#if PUDO == 1 
	doutsp	*PUD[HKMAX];				// Heizungspumpen, Doppelpumpe
#endif

doutsp	*PUNTEA[NTMAX];				// Pumpe Netztrennung (Hauptpumpe)

doutsp	**dopadr;							// Arbeitsspeicher

//---------------------------------------------------------------------------------------------
// Speicherplätze für In/Out-Module
//---------------------------------------------------------------------------------------------
Ram37	mod37[R37_MODMAX];			// maximal 4 (dann keine weiteren Module)
Ram38	mod38[R38_MODMAX];			// maximal 4 (dann keine weiteren Module)
Ram39	mod39[R39_MODMAX];			// maximal 4 (dann keine weiteren Module)
Ram33	mod33[R33_MODMAX];			// maximal 4 (dann keine weiteren Module)
char mod39sysinit;						// Merkmal für SysEEP_InitUser  (Kaltstart 66 wurde ausgeführt)	

char proc_IO;									// Auftragsmerker
char iocnt;										// Counter über maximal 4 Module       (ohne EA-Simulation)
char iocnt_max;								// Counter über maximal 4 Module + R66 (mit  EA-Simulation)
char oready;									// Flag: wenn Output-Task abgelaufen
// EA-Simulation
char names_anford;
char projekt_anford;
char r37text_cnt[4];
char r38text_cnt[4];
char r39text_cnt[4];
char r33text_cnt[4];
char r66text_cnt;
//---------------------------------------------------------------------------

// Sammelstörmeldung für Anzeige und LT
char sstm[S_CLASS_ANZ];				// Feld mit Zählerständen der jeweiligen Sammelalarmklasse, Index entsprechend userdef.h 
char sstm_old[S_CLASS_ANZ];		// Merker für Event.c (Erhöhung des Zählerstandes bei Sammelalarmen (neuer Alarm) )

// Schaltpause für statische Relais, wenn mehrere Relais gleichzeitig einschalten wollen
char Schalt_Pause;		// Sekunden
char schalt_count;

/*-------------- Absenkung nach Zeit für mehrere Heizkreise --------------*/
char	 		abx;								// Reglerindex für Absenkung
char	 		setwoche[HKMAX];		// Schalter Absenkzeiten"Woche" kopieren auf alle Tage
absenktab abs_tab[HKMAX][8];	// Absenkzeiten und Temperaturen für eine Woche
/*-------------- Absenkung bei Bedarf für mehrere Heizkreise -------------*/
char	 		bex;								// Reglerindex für Bedarfs-Absenkung


/*-------------------- Steuer ----------------------------------------*/

// Skalierungswerte für Analogausgabe R36
aask		AASkal[2];		// Grundwerte (EEPROM gestützt, Voreinstellung in UserReply.c bei Kaltstart 66)
UINT		AoutSk[2];		// aktuelle skalierte Ausgabewerte; Umrechnung Aout[i] -> AoutSk[i] in Output.c

// gedämpfte Außenlufttemperatur 
mwsp	ta1m;				// gedämpfte Außentemp. + Status
//float	fl_ta1m;		//		"								(für Filterberechnung) ist im Noinit-Rambereich
char	Ta1mzk;			// Filterzeitkonstante 				[min]
char	steustart;	// Steuerstart nach Reset

mwsp  bcta;				// Broadcast Außentemp. + Status
char  bc_vorra;		// Vorrang Broadcast-Außentemp.
char  bc_flag;		// Flag = 1 wenn ta von bcta abstammt 
char  bc_simu;		// Simulation eines BCTA1-Kommandos
mwsp  ta;					// Arbeitsspeicher für Außentemperatur

// gemittelte Außenlufttemperatur über Stunden
mwsp	ta1mh;			// gedämpfte Außentemp. + Status
//float	fl_ta1mh;	//		"				(für Filterberechnung) ist im Noinit-Rambereich
int		ta1mh_temp;	// Eingebbarer Parameter zum Einstellen der gedämpften Ta 

char	Ta1mhzk;		// Filterzeitkonstante 				[h]
char	zzta1mh;		// Zähler für Tastzeit
char	zzsave;			// Zeitzähler für Ablage im EEPROM 

// Sommer Winter,	zentrale Parameter in ANL
int		Tae;				// Temp. Winter ein						 [°C] * 10	
int		Taa;				// Temp. Sommer aus						 [°C] * 10	
char	sowi;				// 1 = Sommer

#if ANF_EXT > 0
mwsp anfExt[ANF_EXT];							// Anforderung über Bus
#endif

int maxAnford;		// maximale Anforderung (HK, WW)

/*--------------- Reglerparameter für mehrere Heizkreise ---------------*/
//	Die Parameternamen sind in den Strukturdefinitionen festgelegt (ustruct.h) 

// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
HkStandard	hks[HKMAX];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
HkDynam			hkd[HKMAX];

/*--------------- Reglerparameter für mehrere Warmwasserkreise -------*/
// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
WwStandard	wws[WWMAX];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
WwDynam			wwd[WWMAX];

/*--------------- Reglerparameter für mehrere Kesselkreise -------*/
// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
KeStandard	kes[KEMAX];

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Regelprogramme
KeDynam			ked[KEMAX];

/*--------------- Reglerparameter für mehrere Vorregelkreise (Netztrennung)------*/
//	Die Parameternamen sind in den Strukturdefinitionen festgelegt (ustruct.h) 

// Statische Parameter (mit Standdardwerten)	
//   Der Bereich wird gefüllt:
//	 a)nach Kaltstart aus der Liste mit Standardwerten (in UserConst.c)
//		 (Transport mit Funktion "Typelist_copy()" in UserReply.c)
//	 b)nach Reset mit Werten aus EEPROM
//		 (Transport mit Funktion "Ram__Bicram(0)" in Main.c )
NtStandard	nts[NTMAX];	

// Dynamische Parameter
//	 Der Bereich dient als Arbeitsspeicher für die Vorregelprogramme
NtDynam			ntd[NTMAX];

/*--------------- Reglerparameter für Wärmepumpe -------------*/
WpStandard	wps[WPMAX];
WpDynam	wpd[WPMAX];

/*--------------- Reglerparameter für Solar-Regelung -------------*/
SoStandard	sos[SOLMAX];
SoDynam	sod[SOLMAX];

/* Regelparameter für Benutzersteuerung mit UNI-Elementen --------*/
UniStandard unis[1];
UniDynam    unid[1];

/*--------------- Alarmtabelle -----------------------------------*/
AlarmVar	alarmtab[SETAB_DEFL];

char quit_taste;
char un_qsm;							// 1 = Unquittierte Störungen vorhanden
char unbek_Alarm;					// unbekannter Alarm (alarmtab - Index)
char sstm_alarme;					// Merker für Alarme, für SSTM-Relais, Eintrag in parli für KomtabCopy
char sstm_all;						// Merker für alle Alarme inclusive Fühler, Eintrag in parli für KomtabCopy

UINT alarmlist_dae[SETAB_DEFL];		// aktuelle Alarme in einer Liste merken (für RFB):  2 Byte DAE-Nummer
char alarmanz_dae;

/*---------------- Zählerspeicherung pro Monat ------------------------*/
char	std_old;
char	monza_first;
char	monza_znum;
// Rest verlagert nach UserNoinit.c

/***** ulsch : Archivspeicher *****/
char	arp_vis;						// Sichtbarkeit der Gruppe "Archivierung" in Parli
char arch_par;						// Anzahl , char = UCHAR !	
char archivAktiv;					// Archivierung parametriert, Anlauf beendet ( Kennung 58h )
char ParaBuffer[128];


/***** ulsch : Adaption*****/
/*---------------- Adaption --------------------------------------------------------*/
char mwAdapt[HKMAX][ADAP_STAT_MAX];			// uebermittelte Werte 2,5,8
char adapt_countdown[HKMAX];					// Wert in min, empfangen vom R50 über Kommando WR_ADAKENNZ

#if ( ADAP_STAT_HK1 > 0 || ADAP_STAT_HK2 > 0 || ADAP_STAT_HK3 > 0 || ADAP_STAT_HK4 > 0 )
mwsp mwAdaption[HKMAX][ADAP_STAT_MAX];	// Kopie von mwAdapt mit selbst gebildetem Status ( Datenuebertrag.fehler .stat = 1 )
char AdaptMeldg[HKMAX][16];
int AdaptKorr[HKMAX][10];								// 10 temperaturspez.Adaptionswerte fuer -25 grd C ...+20 grd C, in 0,1 grd
char tvsHigh[HKMAX];										// Zaehler zur Ueberwachung der Vorlauf-Temperatur
char tvsLow[HKMAX];
char tvsNormal[HKMAX];
char leistgLow[HKMAX];									// Zaehler
char leistgHigh[HKMAX];
#endif

/***** ulsch : Ferienprogramm *****/
char ferienStat;					// Zustand Ferien	
ferienZeit ferien[10];		// Feld fuer 10 Ferien-Termine ( 60 Byte )

char absenkStat;					// Zustand Absenkung

/***** ulsch : Heizgradtage *****/
#if HEIZGRD == 1
char indexMon;
int ta7;
int ta14;									// Speicherplatz unmittelbar nach ta7 !
monHzGrd sMonHzGrd;
monHzGrd sMonHzGrdAnz;
char merkerStd;
char monHzGrdAnz;
#endif

/***** ulsch : Waermemenge, Diagnose *****/
#if WMENG > 0
zaehlspWmeng wmengCtr[4];					// ZIN7 bis ZIN10 sind möglich
zaehlsp wmengCalc[4];							// für interne Wärmemengen-Zähler, für Zählerobjekt benötigt
#endif

#if ( LEIST_BER > 0 )
maxLeistung MaxLeistung[4];
#endif

#if ( LEIST_BEGR > 0 )
grenzLeistungStat GrenzLeistung;
grenzLeistungDyn grenzLeistung;
UINT AnzahlBegrenzung;
UINT DauerBegrenzung;
#endif

#if DIAG == 1
diagnose did;											// dyn. Parameter
diagParam dis;										// stat. Parameter
#endif	

//#####josch: // Komtab-Copy
copydata CopyData[COPY_RESERV_EMPF];
char		 LeerData[1];
int			 CopySend[COPY_RESERV_SEND];
int			 LeerSend[1];

char ssfEinAnl;										// Schornsteinfeger-Funktion ( KE1 OR KE2 )	

#if TAE_ANZ == 1
mwsp ta_ae;
int TaeSkalMin;
int TaeSkalMax;
#endif

#if TAA_ANZ > 0
int TaaSkalMin;
int TaaSkalMax;
#endif

#if ANFORD == 1
int TmanfSkalMin;
int TmanfSkalMax;
int TmanfSkalMinSpg;
int TmanfSkalMaxSpg;
#endif

sAnaInpPara AnaInpPara[AE_UNI_ANZ];
sAnaInp anaInp[AE_UNI_ANZ];

#if KEANZ > 1
struct sKes KesEin;
struct sKes KesSperre;
struct sKes KesGestoert;
char KesFolgeSoll[8];
char KesFolgeIst[8];
char AnzahlKesEin;
//char KesParVis[KEMAX];
mwsp *VL_STRATEG;
mwsp *VL_FOLGE_AUS; 
mwsp *RL_FOLGE_AUS; 
mwsp *VL_ANHEB;
#endif		// #if KEANZ > 1

// ***** josch: Datenmanager ***************************************************************
// Gerät im DS_Modus (Data Slave),	Auswertung in der Funktion DReply() 
#if DS_MODE == 1
char DS_RxBuf[DS_RXBUF_MAX];			// vom Master empfangene Daten
char DS_RxTout;										// Timeout in Minuten
char DS_RxStat;										// Status der Empfangsdaten
char DS_TxBuf[DS_TXBUF_MAX];			// an den Master zu sendende Daten
char DS_TxLeng;										// Datenanzahl zu senden

// Empfangene Daten vom Master (Variable müssen vom Typ mwsp sein)
mwsp zentrale_ea;									// Beispiel für 1 Byte: Ein/Aus              
mwsp zentrale_istwert;						// Beispiel für 3 Byte: Temperatur mit Status
mwsp zentrale_sollwert;						// Beispiel für 2 Byte: Sollwert             

#endif

// Gerät im DM_Modus (Data Master),	Datenbearbeitung in der Funktion DManager(), Organisation der Datenübertragung im Task DTimer() 
#if DM_MODE == 1
dm_param DM[DM_SLAVES];						// DM Struktur
char dtslave;											// DTimer aktueller Slave
char dtstate;											// DTimer aktueller Bearbeitungsstatus
char dtauftrag;										// DTimer aktueller Auftrag
char proc_TI;											// DTimer Auftragsmerker 
char Slave[4];										// Slave 1 - 4 Adressen

// Empfangene Daten von den Slaves  (Variable müssen vom Typ mwsp sein)
mwsp station1_ea;									// Beispiel für 1 Byte: Ein/Aus              
mwsp station1_istwert;						// Beispiel für 3 Byte: Temperatur mit Status
mwsp station1_sollwert;						// Beispiel für 2 Byte: Sollwert             

	#if DM_SLAVES > 1
mwsp station2_ea;			
mwsp station2_istwert;
mwsp station2_sollwert;
	#endif

#endif

// ***** Josch-SDM : Archiv mit SD-Card-Memory ********************************************************* 
#if ARCHIV_SDM == 1
UINT	werks_sdm_code;						// Code eingeben für Werkseinstellung
UINT	sdm_akt_dae;
char	a_sperre;
char	sdm_arch_stop;						// Benutzer-Stopp (1), wird gesichert in Standard.c 
char	mount_check;
char	test_start;
char	sdm_base_check;
char	sdm_alarm_check;

// Funktion: Zeitmessung (in Upro.c)
// IN:       char  onoff ...ON (1=Start), OFF ( 0=Stop)
// OUT:      ULONG Zeit.....ms
#if ZEIT_MESSUNG == 1
ULONG read_zeit;
ULONG write_zeit;	
ULONG write_zeit_max;
#endif

char	c_erg_gl;									// Ergebnis der Card-Zugriffe veröffentlichen in der parli
char  c_erg_text[16];
char	a_erg_gl;									// Ergebnis der Archivierung  veröffentlichen in der parli
char  a_erg_text[16];
char	cardType;
char	speedClass;
char	performanceMove;
char	ferro_err;
char	card_err;

char	bic_err;
char	asdm_status;
char	asdm_action;
char	sdm_ret;										// Fehleranzeige
char	asdm_minut;
ULONG asdm_time;									// Zeitmarke aktuell
ULONG read_time;									// Zeitmarke: Erster Zeitstempel der Datenpage, deren Eintrag in der Verweispage fehlt
char  asdm_buffer[64];						// Puffer für Zeitstempel und Datenwert
char	asdm_page[512];							// Puffer für eine Page
ULONG timeout_max;

UINT	current_dae;
char	current_dae_exp;
char	max_expand;
char	exp_index;
UINT	dae_append;
UINT	komtab_changed;
char	daily_init;
int   reset_dae;									// ferro Init für ein DAE,EXP

UINT	ferro_adr;
UINT	ferro_mnext;
UINT	ferro_msize;
ferro_info	dae_info[1];
ferro_info *exp_ferro_tab[256];		// Pointer auf Expanddaten-Tabelle im alloziierten Ferro-Bereich

// DAE Archivierung
ULONG hp_adr;											// aktuelle Adresse der Hauptpage.    Anzahl 1
ULONG vp_adr;											// aktuelle Adresse der Verweispage.  Anzahl 15
ULONG dp_adr;											// aktuelle Adresse der Datenpage.    Anzahl 2032
ULONG sdm_mnext;									// Expand-Allocation: nächster freier Platz
ULONG sdm_msize;									// Expand-Allocation: Größe des verfügbaren Speicherbereiches
ULONG	exp_sdm_tab[256];						// Expand-Allocation; die Expanddaten-Tabelle enthält alle alloziierten Hauptpage-Adressen

// DAE Suche
ULONG search_hp_adr;							// gefundene Adresse der Hauptpage.
UINT  search_vp_num;							// gefundene Verweispagenummer in der Hauptpage (1-15)
ULONG search_vp_adr;							// gefundene Adresse der Verweispage.
UINT  search_dp_num;							// gefundene Datenpagenummer in der Verweispage (1-2032)
ULONG search_dp_adr;							// gefundene Adresse der Datenpage.
UINT  search_ts_num;							// gefundene Zeitstempelnummer in der Datenpage
char	sd_buf[512];								// Puffer für eine Page

#if SPEED_SAVE == 1			//$ Test
ULONG asdm_test_start;
#endif

#endif // End ARCHIV_SDM == 1

#if RM_POWER_ANZ
mwsp *RM_POWER[RM_POWER_ANZ];
sPowInpPara RmPowerPara[RM_POWER_ANZ];
sPowInp rmPower[RM_POWER_ANZ];
#endif

#if AE_DRUCK_ANZ
mwsp *AE_DRUCK[AE_DRUCK_ANZ];
sAnaInpPara DruckPara[AE_DRUCK_ANZ];
sAnaInp druck[AE_DRUCK_ANZ];
#endif






																		