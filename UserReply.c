/*		1.Spezielle Kommandodekodierung	für	RS485- und RS232-Bus	Protokoll
				im User-Bereich.
			
				Die Standard-Kommandodekodierung wird in "Reply.c" vorgenommen.
				Der spezielle Kommandoaufruf erfolgt aus Unterprogramm:
				 
					"Dekodierung eines Subkommandos	für	Riecon 36	 "
					void SubReply(char *RsTxBuf, char	*RsRxBuf,	char prot)
						...
						...(System-Kommandos)
						...
							default:
								User36Reply(RsTxBuf, RsRxBuf,	prot);	
			
			2. Spezielle Kommandos zur Beantwortung von RIEcon50-Telegrammen.
				 Aufruf von "Reply.c" bei R50_KOM:
								User50Reply(RsTxBuf, RsRxBuf,	prot);	
				  


			3.CountDown-Überwachung.
				Aufruf erfolgt von "Uhr.c" zu jeder vollen Minute
				
			4.Systeminitialisierung EEPROM im User-Bereich
				Aufruf erfolgt von SysEEP_Init()	in Main.c	 	

			5.Zählerinitialisierung
			
			6.Laden von Standardwerten aus Typenlisten
				Aufruf erfolgt von Standard() in BicMove.c
			
			7.Laden von User-Definitionen und Ein/Ausgabe-Konfiguration
			  den System- oder UserRam: Load_Userdef()
				
*/

#include "sfr32c87.h"
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "pubramext.h"
#include "constext.h"
#include "uconstext.h"
#include "kommando.h"	
#include "standdef.h"
#include "userdef.h"
#include "date.h"
#include "sdate.h"
#include "archivsdm.h"	// Josch-SDM

#if MODBUS_UNI > 0
extern void Init_Modbus(void);
extern char*  get_feldadresse_modbus(char *wert, unsigned char ucExp_num, unsigned char ucAttr);
#endif

/* Unterprogramme */
void load_eapointer36(unsigned int *ea, char idx, char ext);
void load_eapointer37(unsigned int *ea, char idx, char ext);
void load_eapointer38(unsigned int *ea, char idx, char ext);
void load_eapointer39(unsigned int *ea, char idx, char ext);
void load_eapointer33(unsigned int *ea, char idx, char ext);
char LoadPointer(char typ, char klasse, char num, char idx, char ext, char iodev, unsigned int *label);
char LoadParamText(char *liste, unsigned int param);
void ListParameter(char *list, char klasse, unsigned int label);
void Blink(char rot, char gelb, char blau, char idx);
void Print(const char *text, char idx);
void move_evetab(char rw);
void Fill_TxBuf_ZPG(char *RsTxBuf, char offs, char i, char j, char anz, int messw, char stat); 

void fill_alarmtab(char i, char alclass_num);
void move_alarmtab(char rw);
void Check_Version(void);

zaehlsp * get_zpointer(char knum, UINT *dim_adr, UINT *err_adr, UINT *init_adr, UINT *sync_adr);


/* Subkommandos User36-Anwendung	*/
/*--------------------------------*/
// Lüftungsregelung
#define	 DIFFDRU_SOLL		0x10				// Dyn.Diff.druck	Sollwertvorgabe	(Stell_Fu_Soll) in %	 
#define	 DREHZ_IST			0x11				// Lesen Drehzahl	FU 
#define	 RUN_ONOFF			0x12				// FU	Ein-/Ausschalten
#define	 VOLUM_SOLL			0x13				// Volumenstrom Sollwertvorgabe in [0.1 cbm/h]
#define  VOLUM_STATUS		0x14				// lesen Sensor+Attr, Volumenstrom, Freigabe, Hand lesen

//Heizungsregelung
#define	 WR_KASKADE					0x20		// 
#define	 RD_KASKADE					0x21		// 
#define  WR_STATIONS_INFO		0x22		// Stationsinformationen zum Heizen/Kühlen		//090922
#define  RD_STATIONS_INFO		0x23		// Stationsinformationen zum Heizen/Kühlen		//090922
#define  RW_DMANAGER				0x24		// Datenmanager Kommando

/***** ulsch : Archivspeicher *****/
#define	WR_ARCHIV				0x30				// Archiv-Page schreiben
#define RD_ARCHIV				0x31				// Archiv-Page lesen
#define WR_HGT					0x32				// Heizgradtage schreiben
#define RD_HGT					0x33				// Heizgradtage lesen

#define RD_ALARMLIST		0x3F				// Lese Liste der aktuellen Alarme 

// Josch-SDM : Archiv SD-Card-Memory (definiert in archivsdm.h)
//#define READ_SDMEM        0x60    // Test: Lese Daten von einem Speicherplatz
//#define READ_ARCHIV       0x61    // Lese archivierte Daten in Form von Datenpaketen (Zeitstempel + Daten)
//#define READ_MEMINFO      0x62    // Test: Lese Informationen zur Speicherbelegung aus dem Ferro-Ram    
//#define READ_ARCHIV_INFO  0x63    // Test: Lese SD-Speicher-Informationen zu einem gefundenen DAE
//#define CHECK_FERRO_SDHC  0x64    // Kommando für Prüfplatz: Ferro2 gesteckt ?, SDHC gesteckt ?
//#define WRITE_SDMEM       0x65    // Test: Schreibe Daten auf einen Speicherplatz

/* Subkommandos User50-Anwendung	*/
/*--------------------------------*/
#define  RD_VBR					0x11		// Zählerstände lesen     (4 Byte ZQ1...ZQ8)
																// Monatsverbräuche lesen (2 Byte ZV1...ZV8)
																
#define	 RD_ZPG					0x01		// Zeitprogramm vom Raum RR lesen
#define	 WR_ZPG					0x02		// Zeitprogramm vom Raum RR schreiben																
#define  WR_RNAME				0x04		// Raumname vom Raum RR schreiben																					
																
#define RD_INI_ZAEHLER	0x1F		// Konfigurationsdaten von einem RIEcon-Zähler lesen
#define WR_INI_ZAEHLER	0x20		// Konfigurationsdaten zu  einem RIEcon-Zähler senden
#define RD_KONSUM				0x21		// Zähler lesen
#define WR_KONSUM				0x22		// Zähler-Fehler löschen
#define RD_SYNC_KONSUM	0x29		// Synchronisationswerte: wer und wann und mit welchen Werten durchgeführt 
#define WR_SYNC_KONSUM	0x2A		// Zähler aktuell und Vorjahr beschreiben

#define RD_RFDATEN			0x2B		// Daten von einem Funkempfänger lesen
#define WR_RFDATEN			0x2C		// Daten in einen Funkempfänger schreiben (z.B. Quittung auf neues Telegramm) (0xC0 löscht den Empfangsstatus "NEUER EMPFANG" im RIEcon60)
#define RD_RF_INIT			0x2D		// Initialisierungsdaten von einem Funkempfänger lesen
#define WR_RF_INIT			0x2E		// Initialisierungsdaten in einen Funkempfänger schreiben
#define TEACH_IN_DATUE_CONNECT						0x01		// der Slave soll das TeachIn einleiten
#define TEACH_IN_DATUE_MEM								0x02		// der Slave soll das TeachIn speichern
#define TEACH_IN_DATUE_ABBRUCH						0x04		// der Slave soll das TeachIn abbrechen
#define TEACH_IN_DATUE_NACHFRAGE					0x00		// der Slave soll das TeachIn beantworten für die Dauerabfrage

#define RD_MONSUM				0x59		// Monatsverbrauch lesen

// Indexbasis für Telegramm WR_KASKADE
#define HK_BED			0		// Index für Heizkreise						ab 1 - HKANZ
#define WWLAD_BED		10	// Index für Warmwasserbereitung	ab 11
#define WWZIRK_BED	20	// Index für Zirkulationspumpe		ab 21


/*------------------------------------------------------------------------------*/
// Aufruf erfolgt durch SubReply(..) in Reply.c    ( AC-Kommandos )
void User36Reply(char *RsTxBuf, char	*RsRxBuf,	char prot)
{
	char  idx, offs, f, i, j, k, mask;
	unsigned int intwert;
	unsigned char ucKanal, ucMerken, ucAnzahl;
	 
	idx = offs = i = mask = 0;
	
	switch(RsRxBuf[1])												// subkommando
	{
      case RD_TA1:																	// Aussentemperatur lesen, wenn vorhanden
				offs = RsFrame(RsTxBuf,	6, RCOK, prot);
//     	memcpy(&RsTxBuf[offs], &ta1m.messw, 3);			// gedämpfte Ta schicken
      	memcpy(&RsTxBuf[offs], &ta.messw, 3);				// ab 10.04.2003 nicht gedämpfte Ta schicken
			#if HKANZ > 0
				RsTxBuf[offs+3] = sowi;											// Sommer (1=JA)
			#else
				RsTxBuf[offs+3] = 0;
			#endif
				RsTxBuf[offs+4] = bc_flag;									// Flag = 1 wenn ta von bcta abstammt 
				RsTxBuf[offs+5] = 0;
			#if HKANZ > 0
				mask = 0x01;															// Maske
				for(i = 0; i < HKANZ; i++)			// Über alle Heizkreise Sommerbetrieb ermitteln
				{	if(hkd[i].somme > 0)
						RsTxBuf[offs+5] |= mask;
					mask <<= 1;	
				}	
			#endif
				break;	

		case WR_KASKADE:
			idx = RsRxBuf[2];														// Heizkreis-Nr. 1 - 4
			if(idx > 0 && idx <= HKANZ)									// Indexbildung und Überwachung
			{	
				idx--;
				
				if((RsRxBuf[3] | RsRxBuf[4]) != 0)				// Anzahl der Räume in Nutzung [3] oder in der Vorheizzeit [4]
					hkd[idx].bedarf    = BEDARF_JA;					// Heizungsbedarfsanforderung JA
				else
					hkd[idx].bedarf    = BEDARF_NEIN;				// Heizungsbedarfsanforderung NEIN
					
				if(RsRxBuf[3] == 0 && RsRxBuf[4] != 0)		// nur Vorheizzeit [4] ?
					hkd[idx].vorheiz_bed = 1;								// Heizungsbedarf nur durch Räume in Vorheizung
				else
					hkd[idx].vorheiz_bed = 0;
					

				hkd[idx].tisobed   = *(int*)&RsRxBuf[5];	// Soll-Raumtemp	
				hkd[idx].tibed		 = *(int*)&RsRxBuf[7];	// Ist-Raumtemp
				hkd[idx].CountDown = (char)(RsRxBuf[9] + 2);
				// wenn der R50 keine Raumtemp. überträgt (z.B. Dummy-Raum), sondern nur den Nutzungszustand, aber ein Raumfühler	
				// für diesen Heizkreis auf den R36 aufgeschaltet ist, wird der Messwert des Fühlers benutzt, wenn sein Status ok
				if(hkd[idx].tibed == 0 && hkd[idx].tisobed > 0)
					hkd[idx].ticopy = 1;										// Ausführung der Kopie in Bedarf.c
				else
					hkd[idx].ticopy = 0;

// Ventilstellungen
				if ( RsRxBuf[-1] >= 24 )									// Ventilstellungen werden übertragen
				{
					hkd[idx].ventGesamt = *(int*)&RsRxBuf[16];
				//hkd[idx].ventNutzung= *(int*)&RsRxBuf[18];
					hkd[idx].ventOffen  = *(int*)&RsRxBuf[20];
				}	
					
				// Antwortparameter
				offs = RsFrame(RsTxBuf,	4, RCOK, prot);
				RsTxBuf[offs+0] = hks[idx].Haut;
				RsTxBuf[offs+1] = hkd[idx].somme;
				*(int*)(RsTxBuf + offs +2) = hkd[idx].tvsb;
				
			}
			else
			{
				RsFrame(RsTxBuf, 0,	RCOK,	prot);
				
				if(idx > WWLAD_BED && idx <= WWLAD_BED + WWANZ)	// WWLAD_BED = 10
				{
					idx--;
					idx -= WWLAD_BED;
					if((RsRxBuf[3]) != 0)											// Anzahl der genutzen Räume (Dummy-Raum)
						wwd[idx].wwladBedarf    = BEDARF_JA;		// Warmwasserbedarfsanforderung JA
					else
						wwd[idx].wwladBedarf    = BEDARF_NEIN;	// Warmwasserbedarfsanforderung NEIN
	
					wwd[idx].wwladCountDown = (char)(RsRxBuf[9] + 2);
				}
				else	
				{
					if(idx > WWZIRK_BED && idx <= WWZIRK_BED + WWANZ)	// WWZIRK_BED = 20
					{
						idx--;
						idx -= WWZIRK_BED;
						if((RsRxBuf[3]) != 0)											// Anzahl der genutzen Räume (Dummy-Raum)
							wwd[idx].wwzirkBedarf    = BEDARF_JA;		// Zirkulationspumpe_Bedarfsanforderung JA
						else
							wwd[idx].wwzirkBedarf    = BEDARF_NEIN;	// Zirkulationspumpe_Bedarfsanforderung NEIN

						wwd[idx].wwzirkCountDown = (char)(RsRxBuf[9] + 2);
					}
					else
						RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
				}		
			}		
			break;
				
		case WR_ADAKENNZ:																// Adaptionskennzeichen (0, 2, 5 oder 8) empfangen
			// AC E8 KANAL ADAKZ TOUT
			RsFrame(RsTxBuf, 0,	RCOK,	prot);
			idx = RsRxBuf[2];															// Heizkreis-Nr.
			if(idx > 0 && idx <= HKANZ)
			{	idx--;
				mwAdapt[idx][0]        = RsRxBuf[3];				// uebermittelte Werte 0,2,5,8			
				adapt_countdown[idx]   = (char)(RsRxBuf[4] + 2);		// in min										
			}
			else
				RsFrame(RsTxBuf, 0,	ERANZ, prot);						// Fehler
			break;	

		//-------------------------------------------------------------------------------------------------------
#if ARCHIV_ANL == 1
/***** ulsch : Archivspeicher *****/			
		case RD_ARCHIV :
			ReadPageFromArchiv ( RsTxBuf, RsRxBuf, prot );
			break;
			
		case WR_ARCHIV :
			WritePageToArchiv ( RsTxBuf, RsRxBuf, prot );
			break;		
/***** ulsch *****/		
#endif

/***** Josch-SDM :  Archiv mit SD-Card-Memory *****/
#if ARCHIV_SDM == 1
    case READ_SDMEM:        // Test: Lese Inhalt von einem Speicherplatz
    case READ_ARCHIV:       // Lese archivierte Daten in Form von Datenpaketen (Zeitstempel + Daten)
    case READ_MEMINFO:      // Test: Lese Informationen zur Speicherbelegung aus dem Ferro-Ram    
    case READ_ARCHIV_INFO:  // Test: Lese SD-Speicher-Informationen zu einem gefundenen DAE
    case CHECK_FERRO_SDHC:  // Kommando für Prüfplatz: Ferro2 gesteckt ?, SDHC gesteckt ?
    case WRITE_SDMEM:       // Test: Schreibe Daten auf einen Speicherplatz
      ArchSDMReply(RsTxBuf, RsRxBuf, prot); // in ArchivSDM.c
      break;
#endif
			 
#if HEIZGRD	== 1
		case WR_HGT:										// Heizgradtage schreiben
			// AC 32 Monat Heizgradtage(2) AnzahlTage(1) BewerteteTage(1) + Vorjahr
			idx = RsRxBuf[2];							// Monat
			if(idx == 0 || idx > 12)
				RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Fehler
			else
			{	idx--;
				bicbus( EEPADR, &RsRxBuf[3], HEIZGRADTAGE_ADR + idx*8, 8, BICWR);
				RsFrame(RsTxBuf, 0,	RCOK,	prot);
			}
			break;
			
		case RD_HGT:										// Heizgradtage lesen
			// AC 33 Monat
			idx = RsRxBuf[2];							// Monat
			if(idx == 0 || idx > 12)
				RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Fehler
			else
			{	idx--;
				offs = RsFrame(RsTxBuf,	8, RCOK, prot);			// 8 Bytes
				bicbus( EEPADR, &RsTxBuf[offs], HEIZGRADTAGE_ADR + idx*8, 8, BICRD);
			}	 
			break;
			
#endif

		case R3X_AADAT:									// Analogausgabe setzen im R36,R37 (Kommando vom R50)
			// AC F3 Kanal(1), Wert(1), Default(1), Countdown(1)
			idx = RsRxBuf[2];							// Kanal
			k   = RsRxBuf[3];							// Wert 0-255
			//    RsRxBuf[4];							// default
			offs = (char)(RsRxBuf[5] + 2);// Countdown
			f   = 0;											// Merker für Speicherfehler
			RsFrame(RsTxBuf, 0,	RCOK,	prot);
			if(idx > 0 && idx < 3)				// Kanalnummer 1-2: R36
			{ idx -= 1;
				aadat[idx] = k;
				aadat_cd = offs;						// Countdown	
			}
			else 
			{															//							jeweils 4 Ausgänge
				if(idx > 2 && idx < 19)			// Kanalnummer 3-18: R37_1(3,4,5,6), R37_2(7,8,9,10), R37_3(11,12,13,14), R37_4(15,16,17,18)
				{	
					idx -= 3;
					i 	= (char)(idx / 4);		// mod37-Geräte-Index      				(0,1,2,3)		4 = Elementanzahl pro Gerät
					j   = (char)(idx % 4);		// mod37-Analogkanal-Index(Rest)  (0,1,2,3)		4 = Elementanzahl pro Gerät
					if(i < R37_MODMAX)				// Speicherüberprüfung ok
					{
						mod37[i].aadat[j] = k;	// Wert
						aadat_cd = offs;				// Countdown	
					}
					else
						RsFrame(RsTxBuf, 0,	ERDAT, prot);					// Fehler
				}
				else
					RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Fehler
			}		
			break;
					

		case R3X_DADAT:									// Digitalausgabe setzen im R36,R37,R33 (Kommando vom R50)
			// AC F7 Kanal(1), Wert(1), Default(1), Countdown(1)
			idx = RsRxBuf[2];							// Kanal
			k   = RsRxBuf[3];							// Wert 0,1
			//    RsRxBuf[4];							// default
			offs = (char)(RsRxBuf[5] + 2);// Countdown
			f   = 0;											// Merker für Speicherfehler
			RsFrame(RsTxBuf, 0,	RCOK,	prot);
			
			if(idx > 0 && idx < 7)				// Kanalnummer 1-6: R36
			{ 
				idx -= 1;
				mask = (char)(1 << idx);		// Maske
				if(k > 0)										// Wert 0 / 1
					dadat |= mask;
				else
					dadat &= (char)~mask;
				dadat_cd = offs;						// Countdown	
			}
			else
			{															//									jeweils 3 Relais
				if(idx > 6 && idx < 19)			// Kanalnummer 7-18: R37_1(7,8,9), R37_2(10,11,12), R37_3(13,14,15), R37_4(16,17,18)
				{ idx -= 7;
					i 	= (char)(idx / 3);		// mod37-Geräte-Index   			(0,1,2,3)		3 = Elementanzahl pro Gerät
					j   = (char)(idx % 3);		// mod37-Relais-Index(Rest)		(0,1,2)			3 = Elementanzahl pro Gerät
					if(i < R37_MODMAX)				// Speicherüberprüfung ok
					{
						mask = (char)(1 << j);	// Maske
						if(k > 0)
							mod37[i].dadat |= mask;
						else
							mod37[i].dadat &= (char)~mask;
						dadat_cd = offs;				// Countdown	
					}
					else
						f = 1;									// Speicherplatzfehler
				}
				else
				{
					if(idx > 18 && idx < 43)		// Kanalnummer 19-42: R33_1 - R33_4
					{ idx -= 19;
						i		= (char)(idx / 6);		// mod33-Geräte-Index
						j   = (char)(idx % 6);		// mod33-Relais-Index
						if(i < R33_MODMAX)				// Speicherüberprüfung ok
						{
							mask = (char)(1 << j);	// Maske
							if(k > 0)
								mod33[i].dadat |= mask;
							else
								mod33[i].dadat &= (char)~mask;
							dadat_cd = offs;				// Countdown	
						}
						else
							f = 1;									// Speicherplatzfehler
					}
					else
						RsFrame(RsTxBuf, 0,	ERDAT, prot);						// Fehler
				}
				if(f > 0)
					RsFrame(RsTxBuf, 0,	ERDAT,	prot);
			}
			break;			

		// Kommando zum Lesen von 8 Pt1000 nur vom R36
		case R38_IODAT:											// IN/OUT-Datenverkehr R38 - R36
			offs = RsFrame(RsTxBuf, 42,	RCOK,	prot);
			for(idx=0;idx<8;idx++) {
				*((signed int *)&RsTxBuf[offs]) = pt_mewe[idx].messw;
				offs += 2;
				RsTxBuf[offs++] = pt_mewe[idx].stat;
			}
			for(idx=8;idx<14;idx++) {	// auf 14 Messstellen (siehe RIEcon38)
				RsTxBuf[offs++] = 0;
				RsTxBuf[offs++] = 0;
				RsTxBuf[offs++] = NICHTV;
			}
			break;

		// Kommando zum Lesen der aktuellen Alarmliste für WinRFB ab Version 3.4
		case RD_ALARMLIST:
			// Kommando: AC, 3F, Alarmlistenindex(1 Byte , beginnt mit 1)
			// Antwort:  alarmanzahl(1), ZweiZeilen(40 Byte)
			idx = RsRxBuf[2];
			if(idx == 0 || idx > SETAB_TABL)
				RsFrame(RsTxBuf, 0,	ERDAT,	prot);
			else
			{	idx--;
				offs = RsFrame(RsTxBuf, 41,	RCOK,	prot);
				RsTxBuf[offs++] = alarmanz_dae;
				intwert = alarmlist_dae[idx];							// DAE-Nummer
				i = KomTab[intwert].gnum;
				j = KomTab[intwert].pnum;
				ZweiZeilen(&RsTxBuf[offs], i, j, 0);
			}	
			break;
							 
		case  WR_PARAM_SAVE:													// AC D4	 Kommando: Parameter sichern
			RsFrame(RsTxBuf, 0,	RCOK,	prot);
			bicmove_wunsch = 1;
			break;

		case RD_STATIONS_INFO:	//091002	RIEcon36-MMZ kann das Kommando beantworten, wenn die Funktion freigeschaltet ist, sonst keine Antwort.
      ucKanal = RsRxBuf[2];											// den entsprechenden Kanal selektieren (0=keiner), in RsRxBuf[2] steht die Kanalnummer
			if((ucKanal == 0) || (ucKanal > HKANZ)) { 	// Kanalnummer falsch
				RsFrame(RsTxBuf, 0,	ERANZ,	prot);
				break;
			}
			else {
				ucKanal--;														// Kanalnummer an Feldnummer berichtigen
				offs=RsFrame(RsTxBuf, 5, RCOK, prot);	// bei RS485=offs=6= PSYNC, STX8, MasterAdr, Leng, ModulAdr, Rcode
				ucMerken=offs;
				RsTxBuf[offs++] = 94;													// Modultyp der Daten (RIEcon66_STATIONSINFO_READ)
				RsTxBuf[offs++] = hkd[ucKanal].ucCool;
				RsTxBuf[offs++] = 0;													//	hkd[ucKanal].ucCoolerAktiv : no;
				RsTxBuf[offs++] = hkd[ucKanal].ucHeat;
				RsTxBuf[offs++] = 1;													//	hkd[ucKanal].ucHeaterAktiv;
				
			 	*(int *)( RsTxBuf + offs ) = Checksum ( RsTxBuf+ucMerken, (unsigned char)(offs-ucMerken) );
				ucAnzahl = (unsigned char)(offs-ucMerken+2);		// richtige Protokoll-Länge ermitteln und eintragen
				offs=RsFrame(RsTxBuf, ucAnzahl, RCOK, prot);	// bei RS485=ucOffs=6= PSYNC, STX8, MasterAdr, Leng, ModulAdr, Rcode
			}
			break;

		// ***** josch: Datenmanager: Kommando zum Austausch von Daten zwischen Master und Slave
		#if DS_MODE == 1
		case RW_DMANAGER:															// AC 24 Datenmanager
			RW_dmanager_reply(RsTxBuf, RsRxBuf, prot);	// in DReply.c
			break;
		#endif					

		default:
			if(prot == PROT485)
				RsFrame(RsTxBuf, 0,	ERUK,	prot);			// unbekanntes Kommando
			else
				RsFrame(RsTxBuf, 0,	TYPE_ERR,	prot);	// unbekanntes Kommando
				
			break;
	}
}						
/*------------------------------------------------------------------------------*/

// Aufruf erfolgt aus SlaveReply(...) in Reply.c  	AE-Kommandos
void User50Reply(char *RsTxBuf, char	*RsRxBuf,	char prot)
{
	char	offs, i, j, k, kanal, rcode;
	unsigned int pageadr = 0;
	signed char nutzung, nachford, bedarf;
	zaehlsp *pZaehler = NULL;
	ULONG longwert = 0;
	ULONG longtime = 0;
	UINT dim_adr, err_adr, init_adr, sync_adr;
	UINT vb_jahr;
	char jahr_idx =0;
		
	switch(RsRxBuf[1])				// subkommando
	{
		case RD_VBR:
			offs = RsFrame(RsTxBuf,	66, RCOK, prot); // 64 Byte Page + 2 Byte Daten-CRC
			i = RsRxBuf[2];				// Kanalnummer 1...8 (0=Speicherdatum lesen)
			j = RsRxBuf[3];				// Seite 2 3 4 oder 40H 
			k = 0;
			switch(j)
			{
				case 2:							// Seite 2,3,4:	Zählerstände lesen
				case 3:
				case 4:
					//pageadr = ZQ0_ADR + i*0x100 + (j-2)*64;
					k=1; //Fehler
					break;
				case 0x40:					// Verbrauchsdaten lesen
					pageadr = ZMV01_ADR + (i-1)*64;
					break;
				default:
					k = 1;						// Fehler
			}
			if(k == 1)
				RsFrame(RsTxBuf, 0, ERDAT, prot);	// falsche Daten
			else
			{ i = bicbus(EEPADR,  &RsTxBuf[offs], pageadr, 64, BICRD);
				if(i == 0)
					RsFrame(RsTxBuf, 0, ERDAT, prot);
				else
					*(int *)( RsTxBuf + offs + 64 ) = Checksum ( (char *)( RsTxBuf+offs ), 64 );
			}
			break;  


		case RD_ZPG:																// für Regler Heizung, Warmwasser
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{
				offs = RsFrame(RsTxBuf,	0, RCOK, prot); 											// xx Daten + CRC
				j = offs;
				i = RsRxBuf[2];																								// Kanalnummer

				// Indexbildung und Überwachung: Heizkreise 
				//------------------------------------------
				if(i > 0 && i <= HKANZ)														// Basiswert = 0, Erster Index = 1
				{	
					i--;
					Fill_TxBuf_ZPG(RsTxBuf, offs, i, j, HKANZ, TI[i]->messw, TI[i]->stat); 
				}
				else
				{	// Indexbildung und Überwachung: Warmwasserladebetrieb 
					//----------------------------------------------------
					if(i > WWLAD_BED && i <= WWLAD_BED + WWANZ)			// WWLAD_BED = 10 (Basiswert), Erster Index = 11
					{
						i--;
						i -= WWLAD_BED;
						Fill_TxBuf_ZPG(RsTxBuf, offs, i, j, WWANZ, 0, NICHTV); 
					}
					else
					{	// Indexbildung und Überwachung: Zirkulationspumpe 
						//--------------------------------------------------
						if(i > WWZIRK_BED && i <= WWZIRK_BED + WWANZ)	// WWZIRK_BED = 20 (Basiswert), Erster Index = 21
						{
							i--;
							i -= WWZIRK_BED;
							Fill_TxBuf_ZPG(RsTxBuf, offs, i, j, WWANZ, 0, NICHTV);
						}	 
						else
							RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
					}		
				}	
			}
			break;		
						

		case WR_ZPG:															// für Regler Heizung, Warmwasser
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )	// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	RsFrame(RsTxBuf, 0,	RCOK,	prot);
				i = RsRxBuf[2];												// Kanalnummer
				nutzung  = (signed char)RsRxBuf[23];	// Nutzung 0 oder >0
				nachford = (signed char)RsRxBuf[22];	// Zeitnachforderung +/-
				
				// bedarf bestimmen
				if(nachford != 0)											// Zeitnach- oder Zeitabforderung
					bedarf = nachford;
				else																	// keine Nachforderung
					bedarf = nutzung;		

				// Indexbildung und Überwachung: Heizkreise 
				//------------------------------------------
				if(i > 0 && i <= HKANZ)								// Basiswert = 0, Erster Index = 1
				{	i--;
					// Raumtemperatur
					hkd[i].tibed = *(int*)&RsRxBuf[4];		
					if(RsRxBuf[6] != 0x80)							// Status der Raumtemp. nicht ok ?
					hkd[i].tibed = 0;

					if(bedarf > 0)
						hkd[i].bedarf = BEDARF_JA;			// Heizungsbedarfsanforderung JA
					else
						hkd[i].bedarf = BEDARF_NEIN;		// Heizungsbedarfsanforderung NEIN				

					// Solltemperatur plus/minus Temperaturnachforderung
					if(bedarf > 0)
						hkd[i].tisobed  = *(int*)&RsRxBuf[11] + (signed char)RsRxBuf[18];	// Solltemp.
					else
						hkd[i].tisobed  = *(int*)&RsRxBuf[15];	// Spartemp.
					
					hkd[i].CountDown = 20;						// mindestens in 20 Minuten Telegrammwiederholung  
				}
				else	
				{	// Indexbildung und Überwachung: Warmwasserladebetrieb 
					//----------------------------------------------------
					if(i > WWLAD_BED && i <= WWLAD_BED + WWANZ)	// WWLAD_BED = 10 (Basiswert), Erster Index = 11
					{
						i--;
						i -= WWLAD_BED;
						if(bedarf > 0)
							wwd[i].wwladBedarf  = BEDARF_JA;		// Warmwasserbedarfsanforderung JA
						else
							wwd[i].wwladBedarf  = BEDARF_NEIN;	// Warmwasserbedarfsanforderung NEIN
		
						wwd[i].wwladCountDown = 20;						// mindestens in 20 Minuten Telegrammwiederholung
					}
					else
					{	// Indexbildung und Überwachung: Zirkulationspumpe 
						//--------------------------------------------------
						if(i > WWZIRK_BED && i <= WWZIRK_BED + WWANZ)	// WWZIRK_BED = 20 (Basiswert), Erster Index = 21
						{
							i--;
							i -= WWZIRK_BED;
							if(bedarf > 0)
								wwd[i].wwzirkBedarf  = BEDARF_JA;		// Zirkulationspumpe_Bedarfsanforderung JA
							else
								wwd[i].wwzirkBedarf  = BEDARF_NEIN;	// Zirkulationspumpe_Bedarfsanforderung NEIN
	
							wwd[i].wwzirkCountDown = 20;					// mindestens in 20 Minuten Telegrammwiederholung
						}
						else
							RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
					}		
				}	
			}
			break;	

		
		case WR_RNAME:
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	RsFrame(RsTxBuf, 0,	RCOK,	prot);
				i = RsRxBuf[2];															// Kanalnummer
				if(i > 0 && i <= HKANZ)											// Indexbildung und Überwachung
				{	i--;
					memcpy(&hkd[i].raumname, &RsRxBuf[4], 15);
				}	
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}
			break;

		case	RD_INI_ZAEHLER:														//	0x1F		Konfigurationsdaten von einem RIEcon-Zähler lesen
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{
				offs = RsFrame(RsTxBuf,	0, RCOK, prot); 												// Vorläufiger Rahmen: xx Daten + CRC
				j = offs;                                                     	// offset merken	
				i = RsRxBuf[2];																									// Kanalnummer
				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					RsTxBuf[offs++] = i;																					// Zählerkanal
					*(int*)&RsTxBuf[offs]	= pZaehler->zdim.faktor;								// Einheitenzählung:		Anzahl der Zwischenzählerschritte
					offs += 2;
					RsTxBuf[offs++] = pZaehler->zdim.komma;												// Anzeigedarstellung:	Kommastelle
					k = pZaehler->zdim.dim_idx;																		// über die Dimensions-Nummer zur
					RsTxBuf[offs++] = ze_dim_namen[k].zdeinh;											// Dimensions-Einheit
					RsTxBuf[offs++] = i;																					// Zählerkanal
					RsTxBuf[offs++] = pZaehler->zdim.konfig;											// Konfiguration des Zählers
					RsTxBuf[offs++] = pZaehler->zdim.exponent;										// Einheitenzählung:	Exponent zum Errechnen der Speicherwerte

					#if MBUSANZ > 0 
					if((i > 30) && (i <= 30 + MBUSANZ )) {	//100526 diese Daten hat nur ein MBus-Zähler
						switch(MbusPara[i-31].Typ) {
							case KWS:
								pZaehler->zdim.medium = KALTWASSER;
								break;
							case WWS:
								pZaehler->zdim.medium = WARMWASSER;
								break;
							case ELT:
								pZaehler->zdim.medium = ELEKTRO;
								break;
							case WRM:
								pZaehler->zdim.medium = WAERMEMENGE;
								break;
							default:
								break;
						}
					}
					#endif

					RsTxBuf[offs++] = pZaehler->zdim.medium;											// Information: 			Definitionsbyte
					// INIT Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Initialisierung
					if(init_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	init_adr , 5,	BICRD);
						offs += 5;
					}
					
					else {
						#if MBUS_MASTER == 0
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
						#else
							pFehler =  &Z_Fehler[i-31];															// Pointer auf MBus-Zusatzspeicher
							memcpy(&RsTxBuf[offs], &pFehler->clearDongle, 5);
							offs += 5;
						#endif
					}
					
					memcpy(&RsTxBuf[offs], pZaehler->zdim.zname, 16);							// Raumname (wie Zählername ) 
					offs += 16;
					memcpy(&RsTxBuf[offs], pZaehler->zdim.znumm, 16);							// Zählernummer
					offs += 16;
					memcpy(&RsTxBuf[offs], pZaehler->zdim.zname, 16);							// Zählername
					offs += 16;
					memcpy(&RsTxBuf[offs], pZaehler->zdim.stich_tag, 3);					// Stichtag
					offs += 3;
					
					#if MBUSANZ > 0
					if((i > 30) && (i <= 30 + MBUSANZ )) {	//100526 Telegrammlänge erweitert (diese Daten hat nur ein MBus-Zähler)
						RsTxBuf[offs++] = MbusPara[i-31].Adr;
						RsTxBuf[offs++] = MbusPara[i-31].Freigabe;
					}
					else 
					#endif
					{
						RsTxBuf[offs++] = 0;
						RsTxBuf[offs++] = 0;
					}

					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge
					
				}
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}
			break;
			
		case	WR_INI_ZAEHLER:															//	0x20		Konfigurationsdaten zu  einem RIEcon-Zähler senden
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
				RsFrame(RsTxBuf, 0,	RCOK,	prot);
				i = RsRxBuf[2];																									// Kanalnummer

				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					offs = 3;
					pZaehler->zdim.faktor = *(int*)&RsRxBuf[offs];								// Einheitenzählung:		Anzahl der Zwischenzählerschritte
					offs += 2;

					pZaehler->zdim.komma  = RsRxBuf[offs++];											// Anzeigedarstellung:	Kommastelle
					
					k = 0;
					for(j = 0; j < 10; j++)																				// aus der Dimensions-Einheit die Dimensions-Nummer ermitteln
					{	if( ze_dim_namen[j].zdeinh == RsRxBuf[offs] )
						{	k = j;
							break;
						}	
					}		
					pZaehler->zdim.dim_idx = k;																		// Dimensions-Nummer eintragen
					offs++;
					
					k												= RsRxBuf[offs++];										// Zählerkanal (Feldnummer für Zählernamen) ignorieren
					pZaehler->zdim.konfig   = RsRxBuf[offs++];										// Konfiguration des Zählers
					pZaehler->zdim.exponent = RsRxBuf[offs++];										// Einheitenzählung:	Exponent zum Errechnen der Speicherwerte
					pZaehler->zdim.medium   = RsRxBuf[offs++];										// Information: 			Definitionsbyte		
					
					#if MBUSANZ > 0
					if((i > 30) && (i <= 30 + MBUSANZ )) {
						switch(pZaehler->zdim.medium) {
							case KALTWASSER:
								MbusPara[i-31].Typ 			= KWS;
								break;
							case WARMWASSER:
								MbusPara[i-31].Typ 			= WWS;
								break;
							case ELEKTRO:
								MbusPara[i-31].Typ 			= ELT;
								break;
							case WAERMEMENGE:
								MbusPara[i-31].Typ 			= WRM;
								break;
							default:
								MbusPara[i-31].Typ 			= OTHER;
								break;
						}
					}
					#endif
					
					if(init_adr > 0) {	//100519
						// INIT Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Initialisierung
						bicbus(EEPADR,	 (char	*)&RsRxBuf[offs],	init_adr, 5,	BICWR);
					}
					else {
						#if MBUS_MASTER != 0
							pFehler =  &Z_Fehler[i-31];															// Pointer auf MBus-Zusatzspeicher
							pFehler->clearDongle = *(int*)&RsRxBuf[offs];
							pFehler->clearTag = RsRxBuf[offs+2];
							pFehler->clearMon = RsRxBuf[offs+3];
							pFehler->clearJahr = RsRxBuf[offs+4];
						#endif
					}
					
					offs += 5;
																																				// Raumname ignorieren
					offs += 16;
					memcpy(pZaehler->zdim.znumm, &RsRxBuf[offs], 16);							// Zählernummer
					offs += 16;
					memcpy(pZaehler->zdim.zname, &RsRxBuf[offs], 16);							// Zählername
					offs += 16;
					memcpy(pZaehler->zdim.stich_tag, &RsRxBuf[offs], 3);					// Stichtag
					offs += 3;
					
					#if MBUSANZ > 0
					if(RsRxBuf[-1] >= 0x47) {	//100526 Telegrammlänge erweitert mit Adresse des MBus-Zählers und ob der Zähler aktiv ist
						if((i > 30) && (i <= 30 + MBUSANZ )) {
							MbusPara[i-31].Adr 			= RsRxBuf[offs++];
							MbusPara[i-31].Freigabe = RsRxBuf[offs++];
							#if MBUS_MASTER == 0
								bicbus(EEPADR,	 (char	*)&MbusPara[i-31],	MBUS_PARA_ADR+((i-31)*MBUS_PARA_LENG), MBUSLENG,	BICWR);
							#else
								ZaehlerDatenLoeschen ( i-31 );
							#endif
						}
					}
					#endif
					
					if(init_adr > 0) {	//100519
						// Dimensionierung sichern im EEPROM (Ferro):
						bicbus(EEPADR, (char *)&pZaehler->zdim.komma,	dim_adr     , 16,	BICWR); // 16 Byte Dimension   (ZAEHLxx_DIM_ADR)
						bicbus(EEPADR, (char *)&pZaehler->zdim.zname,	dim_adr + 16, 16,	BICWR); // 16 Byte Zählername  (ZAEHLxx_NAME_ADR)
						bicbus(EEPADR, (char *)&pZaehler->zdim.znumm,	dim_adr + 32, 16,	BICWR); // 16 Byte Zählernummer(ZAEHLxx_NUMM_ADR)
					}
				}	
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}		
			break;
			
		case	RD_KONSUM			:														//	0x21		Zähler lesen
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{
				offs = RsFrame(RsTxBuf,	0, RCOK, prot); 												// Vorläufiger Rahmen: xx Daten + CRC
				j = offs;                                                     	// offset merken	
				i = RsRxBuf[2];																									// Kanalnummer
				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					RsTxBuf[offs++] = i;																					// Zählerkanal
					longwert = (ULONG)pZaehler->zwert;														// Zähler aktuell nur LOW-Teil !!
					*(long*)&RsTxBuf[offs] 	= longwert;													
					offs += 4;
					longwert = (ULONG)pZaehler->zwert_vj;													// Zähler Vorjahr nur LOW-Teil !!
					*(long*)&RsTxBuf[offs] 	= longwert;													
					offs += 4;

					if(err_adr > 0) {	//100519
						// Fehlerspeicher + CLEAR Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Fehlerlöschung
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	err_adr, 31,	BICRD);
					}
					else {
						#if MBUS_MASTER == 0
						for(k=0;k<31;k++) {
							RsTxBuf[offs+k] = 0;	// Puffer füllen
						}
						#else
							pFehler =  &Z_Fehler[i-31];															// Pointer auf MBus-Zusatzspeicher
							memcpy(&RsTxBuf[offs], &pFehler->ok1Time, 31);
						#endif
					}
					#if MBUS_MASTER == 0
					if((pZaehler->ucZstatus & LO_HI_FLANKE) == LO_HI_FLANKE)	{	//081117 eingefügt
						RsTxBuf[offs+0] = Tag;
						RsTxBuf[offs+1] = Mon;
						RsTxBuf[offs+2] = Jahr;
						RsTxBuf[offs+3] = Std;
						RsTxBuf[offs+4] = Min;
						RsTxBuf[offs+5] = Sek;
					}
					if((pZaehler->ucZstatus & HI_LO_FLANKE) == HI_LO_FLANKE)	{	//081117 eingefügt
						RsTxBuf[offs+6] = Tag;
						RsTxBuf[offs+7] = Mon;
						RsTxBuf[offs+8] = Jahr;
						RsTxBuf[offs+9] = Std;
						RsTxBuf[offs+10] = Min;
						RsTxBuf[offs+11] = Sek;
					}
					#endif
					offs += 31;

					*(int*)&RsTxBuf[offs]	= pZaehler->zdim.faktor;								// Einheitenzählung:		Anzahl der Zwischenzählerschritte
					offs += 2;
					RsTxBuf[offs++] = pZaehler->zdim.komma;												// Anzeigedarstellung:	Kommastelle
					k = pZaehler->zdim.dim_idx;																		// über die Dimensions-Nummer zur
					RsTxBuf[offs++] = ze_dim_namen[k].zdeinh;											// Dimensions-Einheit
					RsTxBuf[offs++] = i;																					// Zählerkanal
					RsTxBuf[offs++] = pZaehler->zdim.konfig;											// Konfiguration des Zählers
					RsTxBuf[offs++] = pZaehler->zdim.exponent;										// Einheitenzählung:	Exponent zum Errechnen der Speicherwerte
					RsTxBuf[offs++] = pZaehler->zdim.medium;											// Information: 			Definitionsbyte
					// INIT Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Initialisierung
					if(init_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	init_adr , 5,	BICRD);
						offs += 5;
					}
					else {
						#if MBUS_MASTER == 0
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
							RsTxBuf[offs++] = 0;
						#else
							pFehler =  &Z_Fehler[i-31];															// Pointer auf MBus-Zusatzspeicher
							memcpy(&RsTxBuf[offs], &pFehler->clearDongle, 5);
							offs += 5;
						#endif
					}
					
					// neu
					*(long long *)&RsTxBuf[offs] = pZaehler->zwert;								// Hauptzähler aktuell					
					offs += 8;
					*(long long *)&RsTxBuf[offs] = pZaehler->zwert_vj;						// Hauptzähler Vorjahr					
					offs += 8;
					
					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge

				}
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}
			break;
			
		case	WR_KONSUM			:														//	0x22		Zähler-Fehler löschen
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
				RsFrame(RsTxBuf, 0,	RCOK,	prot);
				i = RsRxBuf[2];																									// Kanalnummer

				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					if(err_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[3],	err_adr, 31,	BICWR);
					}
					else {
						#if MBUS_MASTER != 0
							pFehler =  &Z_Fehler[i-31];															// Pointer auf MBus-Zusatzspeicher
							memcpy(&pFehler->ok1Time, &RsRxBuf[3], 31);
							MbusData[i-31].errors = 0;
						#endif
					}
				}	
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}		
			break;
			
		case	RD_SYNC_KONSUM:														//	0x29		Synchronisationswerte: wer und wann und mit welchen Werten durchgeführt 
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{
				offs = RsFrame(RsTxBuf,	0, RCOK, prot); 												// Vorläufiger Rahmen: xx Daten + CRC
				j = offs;                                                     	// offset merken	
				i = RsRxBuf[2];																									// Kanalnummer
				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					if(sync_adr > 0) {	//100519
						//RsTxBuf[offs++] = i;																					// Zählerkanal ****josch: steht nicht in der Telegrammliste !
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	sync_adr  , 4,	BICRD);	// Synchronisationswert aktuell nur LOW-Teil
						offs += 4;
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	sync_adr+8, 4,	BICRD);	// Synchronisationswert Vorjahr nur LOW-Teil
						offs += 4;
						// SYNC Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Synchronisierung
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	sync_adr+SDONGLE_OFF, 5,	BICRD);
						offs += 5;
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	sync_adr  , 8,	BICRD);	// Synchronisationswert aktuell long long
						offs += 8;
						bicbus(EEPADR,	(char	*)&RsTxBuf[offs],	sync_adr+8, 8,	BICRD);	// Synchronisationswert Vorjahr long long
						offs += 8;
					}
					else {
						for(k=0;k<29;k++) {
							RsTxBuf[offs++] = 0;	// Puffer füllen
						}
					}
					
					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge

				}
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);

			}
			break;
			
		case	WR_SYNC_KONSUM:														//	0x2A		Zähler aktuell und Vorjahr beschreiben
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
				RsFrame(RsTxBuf, 0,	RCOK,	prot);
				i = RsRxBuf[2];																									// Kanalnummer

				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					offs = 3;
					if(sync_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[offs],	sync_adr  , 4,	BICWR);	// Synchronisationswert aktuell nur LOW-Teil
					}
					offs += 4;
					if(sync_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[offs],	sync_adr+8, 4,	BICWR);	// Synchronisationswert Vorjahr nur LOW-Teil
					}
					offs += 4;
					// SYNC Dongle-Nummer des PC-Bedieners (2) , Tag, Monat und Jahr der Synchronisierung
					if(sync_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[offs],	sync_adr+SDONGLE_OFF, 5,	BICWR);
					}
					offs += 5;
					
					pZaehler->syncw = *(long long *)&RsRxBuf[offs]; 								// Synchronisationswert aktuell long long
					if(sync_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[offs],	sync_adr  , 8,	BICWR);
					}
					offs += 8;

					pZaehler->syncw_vj = *(long long *)&RsRxBuf[offs]; 							// Synchronisationswert Vorjahr long long
					if(sync_adr > 0) {	//100519
						bicbus(EEPADR,	(char	*)&RsRxBuf[offs],	sync_adr+8, 8,	BICWR);	
					}
					offs += 8;

					pZaehler->syncfl = 4;			 					// Flag für Synchronisieroption 1 (aktuell) und 3 (Vorjahr)
					// Zähler	sichern
					bicmove_count = 0;				 					//	Counter	init
					bicmove_flag	= 2;				 					//	Freigabe (Zählerspeicher)	im Task	BicMove

				}	
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}		
			break;
			
		case RD_MONSUM:														// 0x59 Monatsverbrauch lesen (neu 27.05.2015)
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{
				offs = RsFrame(RsTxBuf,	0, RCOK, prot); 												// Vorläufiger Rahmen: xx Daten + CRC
				j = offs;                                                     	// offset merken	
				i = RsRxBuf[2];																									// Kanalnummer (Modulnummer)
				vb_jahr = *(int*)&RsRxBuf[3];																		// Verbrauchsjahr
				k = RsRxBuf[5];																									// 1= 1.Jahreshälfte 2= 2.Jahreshälfte
				if(k==0 || k>2)
				{	
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);														// ungültige Jahreshälfte
					break;
				}	
				k--;	
				
				if( (vb_jahr - 2000) == Jahr )
					jahr_idx = 0;																									// aktuelles Jahr
				else if( (vb_jahr - 2000) == (Jahr - 1) )
					jahr_idx = 1;																									// Vorjahr
				else
				{	
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);														// ungültiges Jahr
					break;
				}	
				
				pZaehler = get_zpointer(i, &dim_adr, &err_adr, &init_adr, &sync_adr);	// Pointer auf den gewünschten Zähler ermitteln
				if( pZaehler != NULL )												
				{
					RsTxBuf[offs++] = i;																					// Zählerkanal (Modulnummer)
					RsTxBuf[offs++] = 1;																					// Datentyp: 1=Verbräuche 2=Zählerstände
					RsTxBuf[offs++] = pZaehler->zdim.konfig;											// Konfiguration des Zählers
					for(i = 0; i < 6; i++)
					{
						if(jahr_idx == 0)
						{
							longwert = pZaehler->verbr_akt[i + k*6];
						//longtime =
						}	
						else
						{
							longwert = pZaehler->verbr_vor[i + k*6];
						//longtime =
						}
						
						*(unsigned long long *)(RsTxBuf + offs) = (unsigned long long)longwert;
						offs+=8;
						*(ULONG *)(RsTxBuf + offs) = longtime;
						offs+=4;
					}
					
					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge
				}
				else
					RsFrame(RsTxBuf, 0,	ERANZ	,	prot);
			}
			break;
					
						

		
//---------------------------------------------------------------------------------------------------------------------------------------------------			
	#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
	// Funktelegramme
		 case WR_RF_INIT:                 // TeachIn veranlassen schreiben
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
        // in RsRxBuf[2] steht die Kanalnummer
        kanal = RsRxBuf[2];											// den entsprechenden Kanal selektieren (0=keiner)
				if((kanal > 0) && (kanal < 13))
				{
					kanal--;		// Zeiger berichtigen

					offs=RsFrame(RsTxBuf, 4, RCOK, prot);	// bei RS485=offs=6= PSYNC, STX8, MasterAdr, Leng, ModulAdr, Rcode
					j = offs;
					switch(RsRxBuf[3])
					{
						case TEACH_IN_DATUE_CONNECT:
								ucTeach_idx = kanal+1 + 0xF0;									// Merken für TeachIn	(0xF0 für 15 Sekunden Timeout)
								rf_feld[kanal].ucRf_teach_enable = TRUE;
								rcode = RCOK;																	// für später merken
							break;
						case TEACH_IN_DATUE_MEM:
								ucTeach_idx = 0;															// Merken für TeachIn
								// Speichern der ID-in den EEPROM veranlassen !!!!
								rf_feld[kanal].ucRf_teach_enable = FALSE;
								bicbus(EEPADR,	(char	*)&rf_feld[kanal].ulRf_modul_id,		RF_MODULID1_EADR+(kanal*4), 4, BICWR);
								rcode = RCOK;														// für später merken
							break;
						case TEACH_IN_DATUE_ABBRUCH:
								ucTeach_idx = 0;															// Merken für TeachIn
								rf_feld[kanal].ucRf_teach_enable = FALSE;
								rcode = RCOK;														// für später merken
							break;
						case TEACH_IN_DATUE_NACHFRAGE:
								ucTeach_idx = (ucTeach_idx & 0x0F) + 0xF0;			//	(0xF0 für 15 Sekunden Timeout)
								rcode = RCOK;														// für später merken
							break;
						default:
								rcode = ERUK;
							break;
					}

					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id & 0xFF);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 8);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 16);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 24);
					RsTxBuf[offs++] = rf_feld[kanal].ucRf_teach_enable;

					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), rcode, prot);												// Rahmen mit richtiger Protokolllänge

				}
				else
				{	// Kanalnummer zu hoch !
					RsFrame(RsTxBuf, 0,	ERANZ,	prot);
				}
			}	
			break;
				
		case WR_RFDATEN:
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
        // in RsRxBuf[2] steht die Kanalnummer
        kanal = RsRxBuf[2];											// den entsprechenden Kanal selektieren (0=keiner)
				if((kanal > 0) && (kanal < 13))
				{
					kanal--;															// Kanalzeiger berichtigen Kanal 1 hat Index 0
					offs=RsFrame(RsTxBuf, 0, RCOK, prot);	// bei RS485=offs=6= PSYNC, STX8, MasterAdr, Leng, ModulAdr, Rcode
					j = offs;

					if(((rf_feld[kanal].ucRf_status & 0xC0) != 0) && ((RsRxBuf[3] & 0xC0) == 0xC0))
					{
						rf_feld[kanal].ucRf_status &= 0x3F;
						RsTxBuf[offs++] = RCOK;						// NEU im Empfangsstatus ist gelöscht worden
					}
					else
					{
						RsTxBuf[offs++] = 0;							// NEU wurde nicht gelöscht
					}

					rf_feld[kanal].ucRf_praesens = RsRxBuf[4];

					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge
				}
				else
				{	// Kanalnummer zu hoch !
					RsFrame(RsTxBuf, 0,	ERANZ,	prot);
				}
			}	
			break;

		case RD_RFDATEN:
			i = 2;
			if(prot == PROT232)
				i = 5;
			if ( Checksum ( &RsRxBuf[2], (char)(RsRxBuf[-1]-i) ) )						// korrektes Ergebnis : 0 
				RsFrame(RsTxBuf, 0,	CRC_ERR,	prot);			
			else
			{	
        // in RsRxBuf[2] steht die Kanalnummer
        kanal = RsRxBuf[2];											// den entsprechenden Kanal selektieren (0=keiner)
				if((kanal > 0) && (kanal < 13))
				{
					kanal--;		// Sonderfunktionszeiger berichtigen Sonder 1 hat Index 0

					offs=RsFrame(RsTxBuf, 4, RCOK, prot);	// bei RS485=offs=6= PSYNC, STX8, MasterAdr, Leng, ModulAdr, Rcode
					j = offs;
					
					RsTxBuf[offs++] = 12;					// 12 RF-Module können es sein
					RsTxBuf[offs++] = 60;					// Modultyp = RIEcon60_RF = 60
					RsTxBuf[offs++] = rf_feld[kanal].ucRf_status;
					RsTxBuf[offs++] = rf_feld[kanal].ucRf_praesens;
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].siRf_temp & 0xFF);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].siRf_temp >> 8);
					RsTxBuf[offs++] = rf_feld[kanal].scRf_temp_shift;
					RsTxBuf[offs++] = rf_feld[kanal].ucLive_check_timer;
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id & 0xFF);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 8);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 16);
					RsTxBuf[offs++] = (unsigned char)(rf_feld[kanal].ulRf_modul_id >> 24);
					RsTxBuf[offs++] = rf_feld[kanal].ucRf_teach_enable;
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[0];		// Tag
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[1];		// Monat
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[2];		// Jahr
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[3];		// Stunde
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[4];		// Minute
					RsTxBuf[offs++] = rf_feld[kanal].ucfBedienZeit[5];		// Sekunde
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[0];		// Tag
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[1];		// Monat
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[2];		// Jahr
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[3];		// Stunde
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[4];		// Minute
					RsTxBuf[offs++] = rf_feld[kanal].ucfEmpfangsZeit[5];		// Sekunde
					RsTxBuf[offs++] = rf_feld[kanal].ucSetup;								// 040811 NEU

					*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));		// CRC1 + CRC2 
					offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);													// Rahmen mit richtiger Protokolllänge
				}
				else
				{	// Kanalnummer zu hoch !
					RsFrame(RsTxBuf, 0,	ERANZ,	prot);
				}
			}
			break;
				
	#endif	// Funktelegramme


		default:
			RsFrame(RsTxBuf, 0,	ERUK,	prot);			// unbekanntes Kommando
			break;
	}

}

zaehlsp * get_zpointer(char knum, UINT *dim_adr, UINT *err_adr, UINT *init_adr, UINT *sync_adr)
{
	zaehlsp *zpointer = NULL;
	
	if(knum > 0 && knum < 9)																			// Kanalnummer 1...8
	{	if(ZE[knum-1]->zstat	!= NICHTV)														// keine Dummy-Vorbelegung ?
		{
			zpointer   = &zaehler[knum-1];															// Pointer auf R36
			*dim_adr  = ZAEHL01_DIM_ADR + (knum-1) * ZAEHL_DIM_OFFS;		// Adresse Zählerdimensionierung im EEPROM
			*err_adr  = Z01LH_ADR + (knum-1) * ZFEHLER_OFFS;						// Adresse Fehlerspeicher im EEPROM
			*init_adr = INITDONGLE01_ADR + (knum-1) * ZINIT_OFFS;				// Adresse Zählerinitialisierung im EEPROM
			*sync_adr = SYNCAKTU01_ADR + (knum-1) * SYNC_OFFS;					// Adresse Zählersynchronisierung im EEPROM
		}	
	}	
	else if(knum > 10 && knum < 17)																// Kanalnummer 11...16
	{	if(X1ZE[knum-11]->zstat	!= NICHTV)														// keine Dummy-Vorbelegung ?
		{
			zpointer =  &mod37[0].zaehler[knum-11];											// Pointer auf R37_1
			*dim_adr  = ZAEHL11_DIM_ADR + (knum-11) * ZAEHL_DIM_OFFS;		// Adresse Zählerdimensionierung im EEPROM
			*err_adr  = Z11LH_ADR + (knum-11) * ZFEHLER_OFFS;						// Adresse Fehlerspeicher im EEPROM
			*init_adr = INITDONGLE11_ADR + (knum-11) * ZINIT_OFFS;			// Adresse Zählerinitialisierung im EEPROM
			*sync_adr = SYNCAKTU11_ADR + (knum-11) * SYNC_OFFS;					// Adresse Zählersynchronisierung im EEPROM
		}	
	}
	else if(knum > 20 && knum < 27)																// Kanalnummer 21...26
	{	if(X2ZE[knum-21]->zstat	!= NICHTV)														// keine Dummy-Vorbelegung ?
		{
			zpointer =  &mod37[1].zaehler[knum-21];											// Pointer auf R37_2
			*dim_adr  = ZAEHL21_DIM_ADR + (knum-21) * ZAEHL_DIM_OFFS;		// Adresse Zählerdimensionierung im EEPROM
			*err_adr  = Z21LH_ADR + (knum-21) * ZFEHLER_OFFS;						// Adresse Fehlerspeicher im EEPROM
			*init_adr = INITDONGLE21_ADR + (knum-21) * ZINIT_OFFS;			// Adresse Zählerinitialisierung im EEPROM
			*sync_adr = SYNCAKTU21_ADR + (knum-21) * SYNC_OFFS;					// Adresse Zählersynchronisierung im EEPROM
		}	
	}
#if MBUSANZ > 0
	#if MBUS_MASTER == 0
	else if ( knum > 30 && knum <= 30 + MBUSANZ )									// Kanalnummer 31...36
	{
		zpointer =  &mbZaehler[knum-31];															// Pointer auf MBus
		*dim_adr  = ZAEHLMB1_DIM_ADR + (knum-31) * ZAEHL_DIM_OFFS;		// Adresse Zählerdimensionierung im EEPROM
		*err_adr  = ZMB1LH_ADR + (knum-31) * ZFEHLER_OFFS;						// Adresse Fehlerspeicher im EEPROM (bei MBus ungenutzt)
		*init_adr = INITDONGLEMB1_ADR + (knum-31) * ZINIT_OFFS;				// Adresse Zählerinitialisierung im EEPROM (bei MBus ungenutzt)
		*sync_adr = SYNCAKTUMB1_ADR + (knum-31) * SYNC_OFFS;					// Adresse Zählersynchronisierung im EEPROM (bei MBus ungenutzt)
	}
	#else
	else if ( knum > 30 && knum <= 30 + MBUSANZ )									// Kanalnummer 31...90
	{
		zpointer =  &mbZaehler[knum-31];															// Pointer auf MBus
		*dim_adr  = 0;																								// bei MBus-Master ungenutzt
		*err_adr  = 0;																								// bei MBus-Master ungenutzt
		*init_adr = 0;																								// bei MBus-Master ungenutzt
		*sync_adr = 0;																								// bei MBus-Master ungenutzt
	}
	#endif
#endif		


#if WMENG > 0
	else if ( knum >= 95 && knum <= 98 )																			// Kanalnummer 95...98
	{	
		if ( ZE[knum-91]->zstat	!= NICHTV )														// keine Dummy-Vorbelegung ?
		{
//			zpointer   = &wmengCtr[knum-95];															// Pointer auf R36
// so gehts nicht! Ich muss die erforderlichen Werte in eine zusätzliche Struktur zaehlsp eintragen (Wärmemenge, Dimension kWh, Komma 0, exp = 3)
//	UserRam: zaehlsp wmengCalc[4];
			zpointer   = &wmengCalc[knum-95];															// Pointer auf R36
			*dim_adr  = ZAEHL01_DIM_ADR + (knum-91) * ZAEHL_DIM_OFFS;		// Adresse Zählerdimensionierung im EEPROM
			*err_adr  = Z01LH_ADR + (knum-91) * ZFEHLER_OFFS;						// Adresse Fehlerspeicher im EEPROM
			*init_adr = INITDONGLE01_ADR + (knum-91) * ZINIT_OFFS;			// Adresse Zählerinitialisierung im EEPROM
			*sync_adr = SYNCAKTU01_ADR + (knum-91) * SYNC_OFFS;					// Adresse Zählersynchronisierung im EEPROM
		}	
	}	
#endif

	else
		zpointer = NULL;
			
	return(zpointer);
}
	
/*------------------------------------------------------------------------------*/

// Aufruf erfolgt durch DatueReply(..) in Datue.c  ( AF-Kommandos )
//           oder durch SlaveReply(..) in Reply.c  (Basis-Kommando)
void UserDatueReply(char *RsTxBuf, char	*RsRxBuf,	char prot)
{
	char i, offs;
	
	switch(RsRxBuf[0])												
	{
		#if WRE100 == 1
		case SCHR_MRC:
			SchreibeMRC(RsTxBuf, RsRxBuf, prot);			// in WreReply.c
			break;
		#endif
		
		#if WRE100 == 1
		case LESE_MRC:
			LeseMRC(RsTxBuf, RsRxBuf, prot);					// in WreReply.c
			break;
		#endif
	}
				
}


/*----------------------------------------------------------------------------------*/
/* Aufruf von "Main.c" zur Untersuchung einer Zeitmarke (DDate.exe)									*/
/*----------------------------------------------------------------------------------*/
char get_new_zm(char sys)
{
	char ret = 0;
	
	if(sys == 0)	// User - Zeitmarke
	{
		// Zeitmarken Zeit + Datum vom Header "date.h" in den RAM kopieren
		// Der Header wird aktualisiert durch Starten von "DDate.exe"
		memcpy(zm_time, ZM_TIME, 8);		// für Anzeige ZEITMARKE in Parli (Testgruppe)
		memcpy(zm_date, ZM_DATE, 8);		// 
		zm_seku = ZM_SEKU;							// long Sekunde
		
		// gespeicherte Sekunde lesen
		bicbus(EEPADR,	(char	*)&zm_seku_old,	ZM_SEKU_ADR, 4,	BICRD);
		
		if(zm_seku != zm_seku_old)			
		{	
			bicbus(EEPADR,	(char	*)&zm_seku,	ZM_SEKU_ADR, 4,	BICWR);	// Neue Zeitmarke speichern 
			ret = 1;																									// Flag setzen
		}	
	}
	else	// System - Zeitmarke
	{
		// Testen auf neue System-Zeitmarke (im Header "SDATE.H")
		sys_seku = SYS_SEKU;
		bicbus(EEPADR,	(char	*)&sys_seku_old,	SYS_SEKU_ADR, 4,	BICRD);

		if(sys_seku != sys_seku_old)
		{	
			bicbus(EEPADR,	(char	*)&sys_seku,	SYS_SEKU_ADR, 4,	BICWR);
			ret = 1;
		}
		
	}	
	
	return(ret);
}
	
/*----------------------------------------------------------------------------------*/
/* User-Kaltstart	(Aufruf von Main.c)																								*/
/*----------------------------------------------------------------------------------*/
void Kaltstart_User(void)
{

	switch(Kaltstart)
	{
		case U10_KALTST:			// User Kaltstart 10
			break;
		case U11_KALTST:			// User Kaltstart 11
			break;
		case U12_KALTST:			// User Kaltstart 12
			break;
		case U13_KALTST:			// User Kaltstart 13
			break;
		case U14_KALTST:			// User Kaltstart 14
			break;
		case U15_KALTST:			// User Kaltstart 15
			break;
	}
	// Kaltstart-Nummer wird im System gelöscht und gespeichert		

}

/*----------------------------------------------------------------------------------*/
/* Boot-Vorgang	bei	Kaltstart	bzw. nach	System_init (Aufruf von Main.c boot()				*/
/*----------------------------------------------------------------------------------*/
void Boot_User(void)
{

	#if HKANZ > 0
	char i;
	for(i = 0; i < HKMAX; i++)
	{	clear_abs_ram(i);
		clear_abs_bed(i);
	}	
	#endif
}

/*----------------------------------------------------------------------------------*/
/* Aufruf von "Konvert.c" (relevant für R50)																				*/
/*----------------------------------------------------------------------------------*/
char* get_feldadresse(char *wert, unsigned char ucExp_num, unsigned char ucAttr, unsigned char alfunc)
{
//##### ulsch 110630
	unsigned char ucExp_num_rf   = ucExp_num;
	unsigned char ucExp_num_geni = ucExp_num;
	unsigned char ucExp_num_wilo = ucExp_num;
	unsigned char ucExp_num_mbus = ucExp_num;
	
// Beispiel RIEcon36 MMZ
//	if((wert>=(char *)&usAs[0].ucAktiv)&&(wert<(char *)&usAs[1].ucAktiv)) {
//		// Berechnung des Offset für Struktur von FBH statische Werte
//		if(ucExp_num == 0) {
//			ucExp_num = ucBedien_FBH_Nummer;
//		}
//		wert += (&usAs[1].ucAktiv-&usAs[0].ucAktiv) * (ucExp_num-1);
//	}
//	else if((wert>=(char *)&usAd[0].scVaritemp)&&(wert<(char *)&usAd[1].scVaritemp)) {
//		// Berechnung des Offset für Struktur von FBH dynamische Werte
//		if(ucExp_num == 0) {
//			ucExp_num = ucBedien_FBH_Nummer;
//		}
//		wert += (&usAd[1].scVaritemp-&usAd[0].scVaritemp) * (ucExp_num-1);
//	}
	
// Beispiel Funkempfang
	#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
	 if( (wert>=(char *)&rf_feld[0].ucRf_status) && (wert<(char *)&rf_feld[1].ucRf_status) )
	 {
		// Berechnung des Offset für Struktur von RF
		if(ucExp_num_rf == 0)
		{
			if(alfunc == IDX0_FELD)
				ucExp_num_rf = 1;
			else	
				ucExp_num_rf = bedien_rf_nummer;
		}		
			
		wert += (&rf_feld[1].ucRf_status-&rf_feld[0].ucRf_status) * (ucExp_num_rf-1);
	 }
	#endif

// Änderung neue Genibus-Implementierung mit GENI_DEVICE_MAX
	#if ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )
		if ( ( wert >= (char *)&genibus_device_table[0] ) && ( wert < (char *)&genibus_device_table[1] ) )
			{
				if ( uc_genibus_device_nr == 0 )
					uc_genibus_device_nr = 1;
				else if ( uc_genibus_device_nr > GENI_DEVICE_MAX )
					uc_genibus_device_nr = GENI_DEVICE_MAX;

				if(ucExp_num == 0)
				{
					if(alfunc == IDX0_FELD)
						ucExp_num = 1;
					else	
						ucExp_num = uc_genibus_device_nr;
				}		
	
				wert += ( (char *)&genibus_device_table[1] - (char *)&genibus_device_table[0] ) * ( ucExp_num-1 );	
			}
			
		if ( ( wert >= (char *)&Geni_Values[0] ) && ( wert < (char *)&Geni_Values[1] ) )
			{
				if ( uc_genibus_device_nr == 0 )
					uc_genibus_device_nr = 1;
				else if ( uc_genibus_device_nr > GENI_DEVICE_MAX )
					uc_genibus_device_nr = GENI_DEVICE_MAX;
					
				if(ucExp_num == 0)
				{
					if(alfunc == IDX0_FELD)
						ucExp_num = 1;
					else	
						ucExp_num = uc_genibus_device_nr;
				}		
	
				wert += ( (char *)&Geni_Values[1] - (char *)&Geni_Values[0] ) * ( ucExp_num-1 );	
			}			
	#endif	

	// Genibus-Implementierung mit BUS_PU_MAX
	#if ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )
		if ( uc_genibus_device_nr == 0 )
			uc_genibus_device_nr = 1;
		else if ( uc_genibus_device_nr > BUS_PU_MAX )
			uc_genibus_device_nr = BUS_PU_MAX;
			
		if ( ucExp_num_geni == 0 )
		{
			if(alfunc == IDX0_FELD)
				ucExp_num_geni = 1;
			else	
				ucExp_num_geni = uc_genibus_device_nr;
		}		
		else if ( ucExp_num_geni > BUS_PU_MAX )
			ucExp_num_geni = BUS_PU_MAX;

		if ( ( wert >= (char *)&BusPuPara[0] ) && ( wert < (char *)&BusPuPara[1] ) )
			wert += ( (char *)&BusPuPara[1] - (char *)&BusPuPara[0] ) * ( ucExp_num_geni-1 );	
		else if ( ( wert >= (char *)&BusPuData[0] ) && ( wert < (char *)&BusPuData[1] ) )
			wert += ( (char *)&BusPuData[1] - (char *)&BusPuData[0] ) * ( ucExp_num_geni-1 );	

	#endif

	// Wilopumpen mit Modbus
	#if WILO_MODBUS == 1

		if ( cWiloPuCount == 0 )
			cWiloPuCount = 1;
		else if ( cWiloPuCount > BUS_PU_MAX )
			cWiloPuCount = BUS_PU_MAX;
		
		if ( ucExp_num_wilo == 0 )
		{
			if(alfunc == IDX0_FELD)
				ucExp_num_wilo = 1;
			else			
				ucExp_num_wilo = cWiloPuCount;
		}		
		else if ( ucExp_num_wilo > BUS_PU_MAX )
			ucExp_num_wilo = BUS_PU_MAX;
			
		// Berechnung des Offset für Struktur von WiloParli.h	
		if ( ( wert >= (char *)&BusPuPara[0] ) && ( wert < (char *)&BusPuPara[1] ) )
			wert += ( (char *)&BusPuPara[1] - (char *)&BusPuPara[0] ) * ( ucExp_num_wilo-1 );	
		else if ( ( wert >= (char *)&BusPuData[0] ) && ( wert < (char *)&BusPuData[1] ) )
			wert += ( (char *)&BusPuData[1] - (char *)&BusPuData[0] ) * ( ucExp_num_wilo-1 );
		else if ( ( wert >= (char *)&modWiloPu[0] ) && ( wert < (char *)&modWiloPu[1] ) )
			wert += ( (char *)&modWiloPu[1] - (char *)&modWiloPu[0] ) * ( ucExp_num_wilo-1 );	

	#endif // WILO_MODBUS == 1
	
	// Grundfospumpen mit Modbus
	#if GRUNDFOS_MODBUS == 1
	
	
	#endif // GRUNDFOS_MODBUS == 1

	// M-BUS
	#if ( ( ( IMPLEMENT_S3 & MBUS1_IMPL ) ==  MBUS1_IMPL ) && MBUSANZ > 0 ) 
		if ( zaehlerNummer == 0 )
			zaehlerNummer = 1;
		else if ( zaehlerNummer > MBUSANZ )
			zaehlerNummer = MBUSANZ;
		
		if ( ucExp_num_mbus == 0 )
		{
			if(alfunc == IDX0_FELD)
				ucExp_num_mbus = 1;
			else	
				ucExp_num_mbus = zaehlerNummer;
		}		
		else if ( ucExp_num_mbus > MBUSANZ )
			ucExp_num_mbus = MBUSANZ;
			
		if ( wert == (char *)&MBZE[0] )  
		// Berechnung des Offset für Struktur zaehlsp
			wert += ( (char *)&MBZE[1] - (char *)&MBZE[0] ) * ( ucExp_num_mbus-1 );	
		else if ( ( wert >= (char *)&MbusPara[0] ) && ( wert < (char *)&MbusPara[1] ) )
			wert += ( (char *)&MbusPara[1] - (char *)&MbusPara[0] ) * ( ucExp_num_mbus-1 );	
		else if ( ( wert >= (char *)&MbusData[0] ) && ( wert < (char *)&MbusData[1] ) )
			wert += ( (char *)&MbusData[1] - (char *)&MbusData[0] ) * ( ucExp_num_mbus-1 );	
	#endif

// Modbus
#if MODBUS_UNI > 0
		wert = get_feldadresse_modbus(wert, ucExp_num, ucAttr);					// in Modbus.c
#endif

	return(wert);
}	

/*----------------------------------------------------------------------------------*/
/* Aufruf von "BicMove.c" zur Berechnung einer Zähleradresse im R37									*/
/*----------------------------------------------------------------------------------*/
zaehlsp* get_r37z_adr(char idx, char znum)
{
	zaehlsp *p37z;
	
	p37z = &mod37[idx].zaehler[znum];
	
	return(p37z);
	
}
	
	
/*----------------------------------------------------------------------------------*/
/* Aufruf von "BicMove.c" zur Berechnung einer MBus-Zähleradresse 									*/
/*----------------------------------------------------------------------------------*/
zaehlsp* get_MBusZ_adr ( char znum )
{
#if MBUSANZ > 0
	return ( &mbZaehler[znum] );
#else
	return 0;
#endif		
}
	
/*----------------------------------------------------------------------------------*/
/* Aufruf von "Uhr.c" zu jeder vollen Minute																				*/
/*----------------------------------------------------------------------------------*/
void Check_CountDown(void)
{
	char i, j;
	
	for(i = 0; i < HKMAX; i++)				// in allen Reglern
	{
		if(hkd[i].CountDown	>	0)				// wenn CountDown gesetzt worden ist
		{
			hkd[i].CountDown--;						// rückwärts zählen
			if(hkd[i].CountDown ==	1)		// wenn abgelaufen
			{	// Aktionen:
				hkd[i].bedarf	  = BEDARF_UNBEKANNT;
				hkd[i].tisobed  = 0;				
				hkd[i].tibed		= 0;
				hkd[i].vorheiz_bed = 0;
				hkd[i].ticopy		= 0;
//#####ulsch 090723
				hkd[i].ventGesamt	= 0;
				hkd[i].ventOffen	= 0;
			}
		}
		if ( adapt_countdown[i] > 1 )
			adapt_countdown[i]--; 		
	}

	for(i = 0; i < WWMAX; i++)
	{
		// Ladebetrieb nach Bedarfsanford. ( über Kommando WR_KASKADE )
		if(wwd[i].wwladCountDown	>	0)			// wenn CountDown gesetzt worden ist
		{
			wwd[i].wwladCountDown--;					// rückwärts zählen
			if(wwd[i].wwladCountDown ==	1)		// wenn abgelaufen
			{	// Aktionen:
				wwd[i].wwladBedarf	  = BEDARF_UNBEKANNT;
			}
		}
		
		// Zirkulationspumpe nach Bedarfsanford. ( über Kommando WR_KASKADE )
		if(wwd[i].wwzirkCountDown	>	0)			// wenn CountDown gesetzt worden ist
		{
			wwd[i].wwzirkCountDown--;					// rückwärts zählen
			if(wwd[i].wwzirkCountDown ==	1)	// wenn abgelaufen
			{	// Aktionen:
				wwd[i].wwzirkBedarf	  = BEDARF_UNBEKANNT;
			}
		}
	}

	// CountDown vom Broadcast BCTA1 überwachen
	if(CountDwBc > 0)
	{	if(CountDwBc == 1)				// nur beim Übergang von 1 auf 0
		{	bcta.stat = DATERR;			// Datenübertragungserror
		}
		CountDwBc--;	
	}
	if(CountDwBcStat > 0) {	
		if(CountDwBcStat == 1)		// nur beim Übergang von 1 auf 0
		{	CoolBc 		 = 0;
			CoolBcStat = 0;
			HeatBc 		 = 0;
			HeatBcStat = 0;
		}
		CountDwBcStat--;
	}
	
	// Broadcast BCTA1 simulieren, für Testzwecke
	if(bc_simu > 0)
	{	if(bc_simu == 1)
			bc_simu = 2;						
		CountDwBc = bc_simu;			// mindestens 2 min. sicherstellen
		set_bcta1();							// Broadcast Außentemp. übernehmen
		bc_simu = 0;
	}	

	// R3X_DADAT Countdown
	if(dadat_cd > 0)
	{ dadat_cd--;
		if(dadat_cd == 1)
		{
			dadat = 0;
			for(i = 0; i < R37_MODMAX; i++)
				mod37[i].dadat = 0;
			for(i = 0; i < R33_MODMAX; i++)
				mod33[i].dadat = 0;
		}
	}			
	// R3X_AADAT Countdown
	if(aadat_cd > 0)
	{ aadat_cd--;
		if(aadat_cd == 1)
		{
			aadat[0] = 0;	// R36
			aadat[1] = 0;	// R36
			for(i = 0; i < R37_MODMAX; i++)
			{
				for(j = 0; j < 4; j++)
					mod37[i].aadat[j] = 0;
			}		
		}
	}
	
#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
	for(i=0;i<12;i++) {
		if(rf_feld[i].ucLive_check_timer > 0) {
			rf_feld[i].ucLive_check_timer--;
		}
		if((rf_feld[i].ucRf_praesens > 0) && ((Min &0x01) != 0)) {
			// Zeit alle 2 Minuten in den ungeraden Minuten abziehen !
			rf_feld[i].ucRf_praesens--;
		}
	}
#endif
				
// ***** josch: Datenmanager 
#if DS_MODE == 1
	DS_CountDown();		// in DReply.c
#endif

#if DM_MODE == 1
	DM_CountDown();		// in DManager.c
#endif


}

/*----------------------------------------------------------------------------------*/
/* Aufruf von Kommando Broadcast BCTA1	aus "Reply.c"																*/
/*----------------------------------------------------------------------------------*/
void set_bcta1(void)
{
	char i;
	
		if(Ta1Bc >= -500)					// ok
		{	bcta.messw  = Ta1Bc;		// gesendeten Wert übernehmen
			bcta.stat   = 0x00;
		}
		else											// Fehler, wenn >= -50,0°C
			bcta.stat		= UNTERL;
}		



/*----------------------------------------------------------------------------------
	 Systeminitialisierung bei nicht initialisiertem	RTCRAM
	 bzw.	Zwangsauslösung	durch	Kaltstart	'55'
	 für spezielle Werte im User-Bereich.
	 Aufruf von	SysRTC_Init()
	 
	 Kaltstart '55' zum Initialisieren (Löschen) von aktuellen Zählerständen																			 										
-----------------------------------------------------------------------------------*/
void SysRTC_InitUser(void)
{
	char i;
		
	 /*	alle Hauptzähler und Monatszähler (nur vom aktuellen Monat)	im EEPROM(Ferro) löschen */
	 for(i=0;i<8;i++)
	 { zaehler[i].ae_zahl= 0;
	 	 zaehler[i].zwert = 0;
	 	 zaehler[i].vwert = 0;
	 	 zaehler[i].zwert_vj = 0;
		 hwdog(1);
	 	 bicbus(EEPADR,	(char	*)&zaehler[i].zwert, 		ZAEHLER01_ADR + i*8, 8,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&zaehler[i].vwert, 		VERBRAU01_ADR + i*4, 4,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&zaehler[i].zwert_vj, VJZAEHL01_ADR + i*8, 8,	BICWR);
	 }

	#if R37_MODULE > 0
	 for(i=0;i<6;i++)
	 { mod37[0].zaehler[i].ae_zahl= 0;
	 	 mod37[0].zaehler[i].zwert = 0;
	 	 mod37[0].zaehler[i].vwert = 0;
	 	 mod37[0].zaehler[i].zwert_vj = 0;
		 hwdog(1);
	 	 bicbus(EEPADR,	(char	*)&mod37[0].zaehler[i].zwert, 		ZAEHLER11_ADR + i*8, 8,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&mod37[0].zaehler[i].vwert, 		VERBRAU11_ADR + i*4, 4,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&mod37[0].zaehler[i].zwert_vj,	VJZAEHL11_ADR + i*8, 8,	BICWR);
	 }
	#endif

	#if R37_MODULE > 1
	 for(i=0;i<6;i++)
	 { mod37[1].zaehler[i].ae_zahl= 0;
	 	 mod37[1].zaehler[i].zwert = 0;
	 	 mod37[1].zaehler[i].vwert = 0;
	 	 mod37[1].zaehler[i].zwert_vj = 0;
		 hwdog(1);
	 	 bicbus(EEPADR,	(char	*)&mod37[1].zaehler[i].zwert, 		ZAEHLER21_ADR + i*8, 8,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&mod37[1].zaehler[i].vwert, 		VERBRAU21_ADR + i*4, 4,	BICWR);
	 	 bicbus(EEPADR,	(char	*)&mod37[1].zaehler[i].zwert_vj,	VJZAEHL21_ADR + i*8, 8,	BICWR);
	 }
	#endif

	#if MBUSANZ > 0
	 for ( i = 0; i < 31; i++ )
	 	 MbusRxBuf[i] = 0;								// MbusRxBuf zweckentfremdet, risikolos
	 MbusRxBuf[0] = 1;									// Tag, siehe standdef.h ZMB1LH_ADR ff.
	 MbusRxBuf[1] = 1;									// Monat
	 MbusRxBuf[6] = 1;
	 MbusRxBuf[7] = 1;
	 MbusRxBuf[12] = 1;
	 MbusRxBuf[13] = 1;
	 MbusRxBuf[19] = 1;
	 MbusRxBuf[20] = 1;
	 MbusRxBuf[28] = 1;
	 MbusRxBuf[29] = 1;
	 	 
	 for ( i=0; i < MBUSANZ; i++ )
	 { 
	 	 mbZaehler[i].zwert = 0;
	 	 mbZaehler[i].zwert_vj = 0;
	 	 mbZaehler[i].vwert = 0;
		#if MBUS_MASTER == 0
		 hwdog(1);
	 	 bicbus ( EEPADR,	(char	*)&mbZaehler[i].zwert, ZAEHLERMB1_ADR + i*8, 8,	BICWR );
	 	 bicbus ( EEPADR,	(char	*)&mbZaehler[i].zwert_vj, VJZAEHLMB1_ADR + i*8, 8,	BICWR );
	 	 bicbus ( EEPADR,	(char	*)&mbZaehler[i].vwert, VERBRAUMB1_ADR + i*4, 4,	BICWR );
	 	 bicbus ( EEPADR,	MbusRxBuf, ZMB1LH_ADR + i*ZFEHLER_OFFS, 31,	BICWR );
		#else
		 LetzterStichtag[0] = 0; LetzterStichtag[1] = 1; LetzterStichtag[2] = 1;
		#endif
	 }	
	#endif	

// Waermemenge: Zaehlerstaende loeschen
#if WMENG > 0	

#if ZE7WM == 1
	wmengCtr[0].wmeng = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[0].wmeng,	WMENG_ADR, sizeof(wmengCtr[0].wmeng), BICWR);
#endif

#if ZE8WM == 1
	wmengCtr[1].wmeng = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[1].wmeng,	WMENG_ADR+SIZE_WMENG, sizeof(wmengCtr[1].wmeng), BICWR);
#endif

#if ZE9WM == 1
	wmengCtr[2].wmeng = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[2].wmeng,	WMENG_ADR+2*SIZE_WMENG, sizeof(wmengCtr[2].wmeng), BICWR);
#endif

#if ZE10WM == 1
	wmengCtr[3].wmeng = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[3].wmeng,	WMENG_ADR+3*SIZE_WMENG, sizeof(wmengCtr[3].wmeng), BICWR);
#endif

#endif

}	

/*----------------------------------------------------------------------------------
		Kaltstart '56' zum Initialisieren (Löschen) von Laufzeiten
-----------------------------------------------------------------------------------*/
void Init_Laufzeiten(void)
{
	char i;
	
#if SPEED_SAVE == 1	// Josch-SDM
	asdm_test_start = 0x1F07E260;			// 01.04.2015 00:00
	bicbus ( EEPADR,	(char *)&asdm_test_start,	ASDM_TEST_START_ADR, 4, BICWR);
#endif	
	

// Heizkreis-Pumpenlaufzeiten löschen	
#if ( PULZ_HK1 > 0 || PULZ_HK2 > 0 || PULZ_HK3 > 0 || PULZ_HK4 > 0 )
	for ( i = HK1; i < HKANZ; i++ )
	{
		hwdog(1);
		hkd[i].puLz = 0;			
		bicbus ( EEPADR,	(char *)&hkd[i].puLz,	PU_HK_H_ADR + 2 * i, 2, BICWR);
		hkd[i].pudLz = 0;			
		bicbus ( EEPADR,	(char *)&hkd[i].pudLz,	PUD_HK_H_ADR + 2 * i, 2, BICWR);
	}
#endif
	
// Netztrennungs-(Haupt-)Pumpe Laufzeit löschen
#if PULZ_NT1 > 0
	ntd[NT1].puLz = 0;			
	bicbus ( EEPADR,	(char *)&ntd[NT1].puLz,	PU_NT_H_ADR, 2, BICWR);
#endif		 

// Warmwasser Ladepumpe	
#if LPU_WW1	> 0
	wwd[WW1].puwlsta = 0;		// Ladepumpenstarts
	bicbus ( EEPADR,	(char *)&wwd[WW1].puwlsta,	PUWLSTA1_ADR, 2, BICWR);
	wwd[WW1].puwlh = 0;			// Ladepumpenlaufzeit
	bicbus ( EEPADR,	(char *)&wwd[WW1].puwlh,	PUWLH1_ADR, 2, BICWR);
#endif

#if LPU_WW2	> 0
	wwd[WW2].puwlsta = 0;		// Ladepumpenstarts
	bicbus ( EEPADR,	(char *)&wwd[WW2].puwlsta,	PUWLSTA1_ADR+2, 2, BICWR);
	wwd[WW2].puwlh = 0;			// Ladepumpenlaufzeit
	bicbus ( EEPADR,	(char *)&wwd[WW2].puwlh,	PUWLH1_ADR+2, 2, BICWR);
#endif

// Warmwasser Zirkulationspumpe	
#if ZPU_WW1	> 0
	wwd[WW1].puwzh = 0;			// Zirkpumpenlaufzeit
	bicbus ( EEPADR,	(char *)&wwd[WW1].puwzh,	PUWZH1_ADR, 2, BICWR);
#endif

#if ZPU_WW2	> 0
	wwd[WW2].puwzh = 0;			// Zirkpumpenlaufzeit
	bicbus ( EEPADR,	(char *)&wwd[WW2].puwzh,	PUWZH1_ADR+2, 2, BICWR);
#endif

// Solarpumpe
#if SOLANZ > 0
	sod[SO1].lzKolPuH = 0;
	bicbus ( EEPADR,	(char *)&sod[SO1].lzKolPuH,	SOLH1_ADR, 2, BICWR);
#endif

// Kesselstarts und Laufzeit
//-#if KEANZ > 0 	
//-	hwdog(1);
//-	ked[KE1].kestarts = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE1].kestarts,	KESTARTS1_ADR, 2, BICWR);
//-	ked[KE1].kesh = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE1].kesh,			KESH1_ADR, 		 2, BICWR);
//-	ked[KE1].kes2h = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE1].kes2h,		KES2H1_ADR, 	 2, BICWR);
//-#endif
//-#if KEANZ > 1 	
//-	hwdog(1);
//-	ked[KE2].kestarts = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE2].kestarts,	KESTARTS1_ADR + 2, 2, BICWR);
//-	ked[KE2].kesh = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE2].kesh,			KESH1_ADR + 2, 		 2, BICWR);
//-	ked[KE2].kes2h = 0;
//-	bicbus ( EEPADR,	(char *)&ked[KE2].kes2h,		KES2H1_ADR + 2, 	 2, BICWR);
//-#endif

#if ( KEANZ > 0 && KEANZ <= 8 )
	for ( i = 0; i < KEANZ; i++ )
	{
		ked[i].kestarts = 0;
		bicbus ( EEPADR,	(char *)&ked[i].kestarts,	KESTARTS_ADR + 2*i, 2, BICWR);
		ked[i].kesh = 0;
		bicbus ( EEPADR,	(char *)&ked[i].kesh,			KESH_ADR + 2*i, 		2, BICWR);
		ked[i].kes2h = 0;
		bicbus ( EEPADR,	(char *)&ked[i].kes2h,		KES2H_ADR + 2*i, 	  2, BICWR);		
	}
#endif
	
// Tauscher-Laufzeit (2 Tauscher, Regel2Wt.c)	
#if NTANZ > 1
	hwdog(1);
	ntd[NT1].lzStd = 0;
	bicbus ( EEPADR,	(char *)&ntd[NT1].lzStd,		WTH_ADR, 		 			 2, BICWR);
	ntd[NT2].lzStd = 0;
	bicbus ( EEPADR,	(char *)&ntd[NT2].lzStd,		WTH_ADR + 2, 		 	 2, BICWR);
#endif
}	


/*----------------------------------------------------------------------------------
	 Zwangsauslösung	durch	Kaltstart	'67'
	 für spezielle Werte im User-Bereich.
	 Aufruf von	SysEEP_Init()																			 										
-----------------------------------------------------------------------------------*/
void SysEEP_InitUser_67(void)
{	
	char i;
	char profil;
	
//-----------------------------------------------------------------------------------
// Objekt- und Standortbezeichnungen (sind im System gelöscht worden).
//-----------------------------------------------------------------------------------
// Überschreiben mit Standardwerten aus UserConst.c
	bicbus(EEPADR,	(char *)&c_Objektbez,	OBJBEZ_ADR,		15, BICWR);
	bicbus(EEPADR,	(char *)&c_Objektnum,	OBJNUM_ADR,		 9, BICWR);
	hwdog(1);
	bicbus(EEPADR,	(char *)&c_Strasse,		STRASSE_ADR,	15, BICWR);
	bicbus(EEPADR,	(char *)&c_Nummer,		NUMMER_ADR,		 9, BICWR);
	hwdog(1);
	bicbus(EEPADR,	(char *)&c_Ort,				ORT_ADR,			15, BICWR);
	bicbus(EEPADR,	(char *)&c_Plz,				PLZAHL_ADR,		 9, BICWR);
	hwdog(1);

// Voreinstellung BUS-Betrieb
//---------------------------------------------------------------------------------------------
	memcpy( (void *)&Service_Idf, (void *)&c_Service_Idf, 15 );
	bicbus(EEPADR,	(char *)&c_Service_Idf,	SERVICE_IDF_ADR,		15, BICWR);
	
	// Moduladresse wurde im System auf 236 eingestellt
	if(c_ModulAdr != 0)							// überschreiben, wenn ein Wert in UserConst.c eingetragen ist
	{	ModulAdr = c_ModulAdr;
		bicbus(EEPADR,	(char *)&ModulAdr,	MODULADR_ADR,		1, BICWR);
	}	
	// Stationsadresse wurde im System auf 254 eingestellt
	if(c_StationsAdr != 0)					// überschreiben, wenn ein Wert in UserConst.c eingetragen ist
	{	StationsAdr = c_StationsAdr;
		bicbus(EEPADR,	(char *)&StationsAdr,	STATIONS_ADR,		1, BICWR);
	}	

// Schnittstelle und Funktionen (sind im System voreingestellt worden)
//---------------------------------------------------------------------------------------------
	if(c_Mode_S1 != NULL_MODE)
	{	Mode_S1 = c_Mode_S1;
		bicbus(EEPADR,	(char *)&Mode_S1,	S1_MODE_EADR,		1, BICWR);
	}	
	if(c_Mode_S2 != NULL_MODE)
	{	Mode_S2 = c_Mode_S2;
		bicbus(EEPADR,	(char *)&Mode_S2,	S2_MODE_EADR,		1, BICWR);
	}	
	if(c_Mode_S3 != NULL_MODE)
	{	Mode_S3 = c_Mode_S3;
		bicbus(EEPADR,	(char *)&Mode_S3,	S3_MODE_EADR,		1, BICWR);
	}	
		
	if(c_Baudrate_S1 != 0)
	{	Baudrate_S1 = c_Baudrate_S1;
		bicbus(EEPADR,	(char *)&Baudrate_S1,	BAUDRATE_S1_EADR,		2, BICWR);
	}	
	if(c_Baudrate_S2 != 0)
	{	Baudrate_S2 = c_Baudrate_S2;
		bicbus(EEPADR,	(char *)&Baudrate_S2,	BAUDRATE_S2_EADR,		2, BICWR);
	}	
	if(c_Baudrate_S3 != 0)
	{	Baudrate_S3 = c_Baudrate_S3;
		bicbus(EEPADR,	(char *)&Baudrate_S3,	BAUDRATE_S3_EADR,		2, BICWR);
	}	

	if(c_Funktion_S1 != NULL_FU)
	{	Funktion_S1 = c_Funktion_S1;
		bicbus(EEPADR,	(char *)&Funktion_S1,	FUNKTION_S1_EADR,		1, BICWR);
	}	
	if(c_Funktion_S2 != NULL_FU)
	{	Funktion_S2 = c_Funktion_S2;
		bicbus(EEPADR,	(char *)&Funktion_S2,	FUNKTION_S2_EADR,		1, BICWR);
	}	
	if(c_Funktion_S3 != NULL_FU)
	{	Funktion_S3 = c_Funktion_S3;
		bicbus(EEPADR,	(char *)&Funktion_S3,	FUNKTION_S3_EADR,		1, BICWR);
	}	

	if(c_Parity_S1 != NULL_PARI)
	{	Parity_S1 = c_Parity_S1;
		bicbus(EEPADR,	(char *)&Parity_S1,	PARITY_S1_EADR,		1, BICWR);
	}	
	if(c_Parity_S2 != NULL_PARI)
	{	Parity_S2 = c_Parity_S2;
		bicbus(EEPADR,	(char *)&Parity_S2,	PARITY_S2_EADR,		1, BICWR);
	}	
	if(c_Parity_S3 != NULL_PARI)
	{	Parity_S3 = c_Parity_S3;
		bicbus(EEPADR,	(char *)&Parity_S3,	PARITY_S3_EADR,		1, BICWR);
	}	

	if(c_Stopbits_S1 != NULL_STOP)
	{	Stopbits_S1 = c_Stopbits_S1;
		bicbus(EEPADR,	(char *)&Stopbits_S1,	STOPBITS_S1_EADR,		1, BICWR);
	}	
	if(c_Stopbits_S2 != NULL_STOP)
	{	Stopbits_S2 = c_Stopbits_S2;
		bicbus(EEPADR,	(char *)&Stopbits_S2,	STOPBITS_S2_EADR,		1, BICWR);
	}	
	if(c_Stopbits_S3 != NULL_STOP)
	{	Stopbits_S3 = c_Stopbits_S3;
		bicbus(EEPADR,	(char *)&Stopbits_S3,	STOPBITS_S3_EADR,		1, BICWR);
	}	

//---------------------------------------------------------------------------------------------

// ***** josch: Datenmanager 
#if DM_MODE == 1
	for(i = 0; i < 4; i++)
		Slave[i] = 0;										// Slave-Adressen werden gelöscht
	bicbus(EEPADR,	(char *)&Slave[0],	SLAVE_ADR, 4, BICWR);
#endif	

// Funkempfang
#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
	
	for(i = 0; i < 12; i++)
	{
		rf_feld[i].ulRf_modul_id = 0;
		rf_feld[i].ucSetup = 0;
	}
	
	bicbus(EEPADR,	(char *)&Null64,	RF_MODULID1_EADR, 12*4, BICWR);
	hwdog(1);
	bicbus(EEPADR,	(char *)&Null64,	RF_SETUP1_EADR, 	12*1, BICWR);
		
#endif

// MBUS
#if ( MBUSANZ > 0 && MBUS_MASTER == 0 )
	// Zählerdimensionierungen MB_Z01..MB_Z06
	for ( i = 20; i < 20+MBUSANZ; i++)
	{//													
		bicbus ( EEPADR,	(char *)&ze_dimsys[i],	ZAEHLMB1_DIM_ADR + (i-20)*48,	48, BICWR );
		hwdog(1);
	}
#endif

#if MBUSANZ > 0
	#if MBUS_MASTER == 0
	if(Proj_Typ > 0)			// bei konstanten Projekten
		proj_typ = Proj_Typ;
	else									// bei variablen Projekten	
	{ 
		bicbus(EEPADR,	(char	*)&proj_typ,	PROJ_TYP_ADR, 1, BICRD);	// lese akt. proj_typ
		
		if(proj_typ <= PROJTYPEN)		// Test auf max. Projektanzahl
			;													// ok, nicht verändern
		else	
			proj_typ = 0;	// Projekttyp  auf 0 (nicht initialisiert) setzen 
	}		
	
	bicbus(EEPADR,	(char	*)&proj_typ,	PROJ_TYP_ADR, 1, BICWR);

	// Standardwerte für MBUS
	for ( i = 0; i < MBUSANZ; i++ )
	{	
		profil = Projekte[proj_typ].mbcode[i];			// Laden der Profil Nummer
		if ( profil < MB_PROFILE )											// auf max abtesten
		{ // Speicherbereich für Standardwerte laden
			memcpy( (void *)&MbusPara[i], (void *)&Mbus_Standparam[profil], MBUSLENG );
			bicbus (EEPADR,	(char *)&MbusPara[i],	MBUS_PARA_ADR + i*MBUS_PARA_LENG, MBUSLENG,	BICWR );		
		}
	}		
	#else
	proj_typ = Proj_Typ;															// konstantes Projekt
	// Standardwerte für MBUS
	for ( i = 0; i < MBUSANZ; i++ )
	{	
		profil = Projekte[proj_typ].mbcode[i];			// Laden der Profil Nummer
		if ( profil < MB_PROFILE )									// auf max abtesten
		{ // Speicherbereich für Standardwerte laden
			memcpy( (void *)&MbusPara[i], (void *)&Mbus_Standparam[profil], MBUSLENG );
			memcpy ( (void *)&mbZaehler[i].zdim, (void *)&ze_dimsys[i+20], sizeof(zaehldim) );
		}
	}
	LetzterStichtag[0] = 0; LetzterStichtag[1] = 0; LetzterStichtag[2] = 0;	
	#endif
#endif

}

/*----------------------------------------------------------------------------------
	 Zwangsauslösung	durch	Kaltstart	'66'
	 für spezielle Werte im User-Bereich.
	 Aufruf von	SysEEP_Init()																			 										
-----------------------------------------------------------------------------------*/
void SysEEP_InitUser_66(void)
{	
	unsigned int adr;
	char i, anz;


	if(Proj_Typ > 0)			// bei konstanten Projekten
		proj_typ = Proj_Typ;
	else									// bei variablen Projekten	
	{ 
		bicbus(EEPADR,	(char	*)&proj_typ,	PROJ_TYP_ADR, 1, BICRD);	// lese akt. proj_typ
		
		if(proj_typ <= PROJTYPEN)		// Test auf max. Projektanzahl
			;													// ok, nicht verändern
		else	
			proj_typ = 0;	// Projekttyp  auf 0 (nicht initialisiert) setzen 
	}		
	
	bicbus(EEPADR,	(char	*)&proj_typ,	PROJ_TYP_ADR, 1, BICWR);

	//#####ulsch:Archivierung
	arch_par = 0;	
	hwdog(1);
	bicbus ( EEPADR,	&arch_par, ARCH_PAR_ADR, 1, BICWR );
	xicbus ( XEEPADR, &arch_par, 0x0000,	2, BICWR );				// Kennung zerstoert -> keine Archivierung

#if WMENG > 0

#if ZE7WM == 1
	wmengCtr[0].monInVj = 1;						// Kaltstart-Wert
	bicbus ( EEPADR, &wmengCtr[0].monInVj, MON_IN_VJ, 1, BICWR );
	wmengCtr[0].flow_max = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[0].flow_max,	FLOWMAX_ADR, sizeof(wmengCtr[0].flow_max), BICWR);				
	wmengCtr[0].wmLeistg_max	= 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[0].wmLeistg_max,	LEISTMAX_ADR, sizeof(wmengCtr[0].wmLeistg_max), BICWR);		
#endif

#if ZE8WM == 1
	wmengCtr[1].monInVj = 1;						// Kaltstart-Wert
	bicbus ( EEPADR, &wmengCtr[1].monInVj, MON_IN_VJ+SIZE_WMENG, 1, BICWR );
	wmengCtr[1].flow_max = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[1].flow_max,	FLOWMAX_ADR+SIZE_WMENG, sizeof(wmengCtr[1].flow_max), BICWR);				
	wmengCtr[1].wmLeistg_max	= 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[1].wmLeistg_max,	LEISTMAX_ADR+SIZE_WMENG, sizeof(wmengCtr[1].wmLeistg_max), BICWR);		
#endif

#if ZE9WM == 1
	wmengCtr[2].monInVj = 1;						// Kaltstart-Wert
	bicbus ( EEPADR, &wmengCtr[2].monInVj, MON_IN_VJ+2*SIZE_WMENG, 1, BICWR );
	wmengCtr[2].flow_max = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[2].flow_max,	FLOWMAX_ADR+(UINT)2*SIZE_WMENG, sizeof(wmengCtr[2].flow_max), BICWR);				
	wmengCtr[2].wmLeistg_max	= 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[2].wmLeistg_max,	LEISTMAX_ADR+(UINT)2*SIZE_WMENG, sizeof(wmengCtr[2].wmLeistg_max), BICWR);		
#endif

#if ZE10WM == 1
	wmengCtr[3].monInVj = 1;						// Kaltstart-Wert
	bicbus ( EEPADR, &wmengCtr[3].monInVj, MON_IN_VJ+3*SIZE_WMENG, 1, BICWR );
	wmengCtr[3].flow_max = 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[3].flow_max,	FLOWMAX_ADR+(UINT)3*SIZE_WMENG, sizeof(wmengCtr[3].flow_max), BICWR);				
	wmengCtr[3].wmLeistg_max	= 0;
	bicbus ( EEPADR,	(char *)&wmengCtr[3].wmLeistg_max,	LEISTMAX_ADR+(UINT)3*SIZE_WMENG, sizeof(wmengCtr[3].wmLeistg_max), BICWR);		
#endif

#endif

#if LEIST_BER == 1
	for ( i = 0; i <= 3; i++ )
	{
		MaxLeistung[i].MittelnLeistg = 5;
		MaxLeistung[i].mittelMax = 0;
		MaxLeistung[i].mittel = 0;
		MaxLeistung[i].tag.jahr = 0;
		MaxLeistung[i].tag.monat = 1;
		MaxLeistung[i].tag.tag = 1;
		MaxLeistung[i].zeit.min = 0;
		MaxLeistung[i].zeit.std = 0;
		MaxLeistung[i].vl.stat = NICHTV;
		MaxLeistung[i].vl.messw = 0;	
		MaxLeistung[i].rl.stat = NICHTV;
		MaxLeistung[i].rl.messw = 0;																			
		MaxLeistung[i].ta.stat = NICHTV;
		MaxLeistung[i].ta.messw = 0;																			
		MaxLeistung[i].flow = 0;
		MaxLeistung[i].resMax = 0;
		hwdog(1);
		bicbus ( EEPADR, (char *)&MaxLeistung[i], MITT_LEIST + (UINT)i * SIZE_MITT_LEIST, sizeof( maxLeistung ), BICWR );																						
	}
#endif

#if LEIST_BEGR == 1
	GrenzLeistung.GrenzLeistg = 0;
	GrenzLeistung.LeistgHyst = 0;
	GrenzLeistung.Daempfung = 0;
	GrenzLeistung.BegrAkt = 0;
	bicbus ( EEPADR, (char *)&GrenzLeistung, GRENZ_LEIST, sizeof( grenzLeistungStat ), BICWR );																						
#endif


#if DIAG == 1
	/***** ulsch : Diagnose *****/
	hwdog(1);
	dis.begin.jahr = 0; dis.begin.monat = 1;  dis.begin.tag = 1;
	dis.end.jahr = 0; dis.end.monat = 1;  dis.end.tag = 1;
	bicbus ( EEPADR, (char *)&dis, DIAG_PARA, sizeof(diagParam), BICWR );
	did.tvpMax.tag.tag = 1; did.tvpMax.tag.monat = 1; did.tvpMax.tag.jahr = 0;
	xicbus ( XEEPADR,	(char *)( &did.tvpMax), TVPMAXDIAG_ADR, sizeof( did.tvpMax ), BICWR );	
	did.tvpMin.tag.tag = 1; did.tvpMin.tag.monat = 1; did.tvpMin.tag.jahr = 0;
	xicbus ( XEEPADR,	(char *)( &did.tvpMin), TVPMINDIAG_ADR, sizeof( did.tvpMin ), BICWR );	
#endif

	/***** ulsch : Heizgradtage *****/
#if HEIZGRD == 1	
	hwdog(1);
	ta7 = 0x7FFF; ta14 = 0x7FFF; indexMon = 0; merkerStd = 0;		// Vorbelegung, Temperaturen unsinnig
	bicbus ( EEPADR,	(char	*)&ta7, TA7_ADR, 2, BICWR );
	bicbus ( EEPADR,	(char	*)&ta14, TA14_ADR, 2, BICWR );	
	bicbus ( EEPADR,	&indexMon, INDEX_MON_ADR, 1, BICWR );	
	bicbus ( EEPADR,	&merkerStd, MERKER_STD_ADR, 1, BICWR );	
#endif

	/***** ulsch : Adaption und Ferienprogramm *****/	
#if ( HKANZ >= 1 && ADAP_STAT_HK1 > 0 )
	hwdog(1);
	bicbus ( EEPADR, (char *)&Null64,	ADAPT_KORR_ADR, 								 sizeof(AdaptKorr[HK1]), BICWR );
#endif

#if ( HKANZ >= 2 && ADAP_STAT_HK2 > 0 )
	hwdog(1);
	bicbus ( EEPADR, (char *)&Null64,	ADAPT_KORR_ADR+ADAPT_KORR_LNG, 	 sizeof(AdaptKorr[HK2]), BICWR );
#endif
	
#if ( HKANZ >= 3 && ADAP_STAT_HK3 > 0 )
	hwdog(1);
	bicbus ( EEPADR, (char *)&Null64,	ADAPT_KORR_ADR+ADAPT_KORR_LNG*2, sizeof(AdaptKorr[HK3]), BICWR );
#endif
	
#if ( HKANZ >= 4 && ADAP_STAT_HK4 > 0 )
	hwdog(1);
	bicbus ( EEPADR, (char *)&Null64,	ADAPT_KORR_ADR+ADAPT_KORR_LNG*3, sizeof(AdaptKorr[HK4]), BICWR );
#endif

#if FER == 1
	for ( i = 0; i < 10; i++ )
	{
		ferien[i].ferienBeg.jahr = 0;	ferien[i].ferienBeg.monat = 1; ferien[i].ferienBeg.tag = 1; 
		ferien[i].ferienEnd.jahr = 0;	ferien[i].ferienEnd.monat = 1; ferien[i].ferienEnd.tag = 1;
	}
	hwdog(1);
	bicbus ( EEPADR, (char *)ferien,	FERIEN_ADR, (char)( 60 ), BICWR );							// 10 Ferientermine ( -anfang, -ende )
#endif

#if HKANZ > 0
	//*****josch : Raumnamen
	for(i = 0; i < HKANZ; i++)
	{
		hwdog(1);
		memcpy((void *)hkd[i].raumname, (void *)Hkmod[i], 16);
		bicbus ( EEPADR,	(char	*)&hkd[i].raumname, RAUMNAME1_ADR + i*16, 16, BICWR );
	}	
#endif

	// Skalierungswerte für Analogausgabe R36
	memcpy( (void *)&AASkal[0], (void *)&R36_37_Standparam[0], 	8 );					// 2 * 4 Byte
	bicbus ( EEPADR,	(char	*)&AASkal[0], AA_SKAL_ADR, 8, BICWR );


	// Vergabe der Standard-Adressen für IN/OUT-Modul R37
	//-----------------------------------------------------------------
#if R37_MODULE > 0
	hwdog(1);
	mod37[0].bus = 0;			// XBUS
	mod37[0].adr = 0x20;	// 1.Adresse 32D
	bicbus(EEPADR,	(char	*)&mod37[0].bus,		R37_1_BUS_ADR,	2, BICWR);		// bus und adr stehen hintereinander
	// Skalierung
	memcpy( (void *)&mod37[0].AASkal[0], (void *)&R36_37_Standparam[1], 	16 );					// 4 * 4 Byte
	bicbus ( EEPADR,	(char	*)&mod37[0].AASkal[0], R37_1_AA_SKAL_ADR, 16, BICWR );
		
#endif

#if R37_MODULE > 1
	hwdog(1);
	mod37[1].bus = 0;			// XBUS
	mod37[1].adr = 0x22;	// 2.Adresse 34D
	bicbus(EEPADR,	(char	*)&mod37[1].bus,		R37_2_BUS_ADR,	2, BICWR);
	// Skalierungswerte für Analogausgänge
	memcpy( (void *)&mod37[1].AASkal[0], (void *)&R36_37_Standparam[2], 	16 );					// 4 * 4 Byte
	bicbus ( EEPADR,	(char	*)&mod37[1].AASkal[0], R37_2_AA_SKAL_ADR, 16, BICWR );	// 4 * 4 Byte
#endif

#if R37_MODULE > 2
	hwdog(1);
	mod37[2].bus = 0;			// XBUS
	mod37[2].adr = 0x24;	// 3.Adresse 36D
	bicbus(EEPADR,	(char	*)&mod37[2].bus,		R37_3_BUS_ADR,	2, BICWR);
	// Skalierungswerte für Analogausgänge
	memcpy( (void *)&mod37[2].AASkal[0], (void *)&R36_37_Standparam[3], 	16 );					// 4 * 4 Byte
	bicbus ( EEPADR,	(char	*)&mod37[2].AASkal[0], R37_3_AA_SKAL_ADR, 16, BICWR );	// 4 * 4 Byte
#endif

#if R37_MODULE > 3
	hwdog(1);
	mod37[3].bus = 0;			// XBUS
	mod37[3].adr = 0x26;	// 4.Adresse 38D
	bicbus(EEPADR,	(char	*)&mod37[3].bus,		R37_4_BUS_ADR,	2, BICWR);
	// Skalierungswerte für Analogausgänge
	memcpy( (void *)&mod37[3].AASkal[0], (void *)&R36_37_Standparam[4], 	16 );					// 4 * 4 Byte
	bicbus ( EEPADR,	(char	*)&mod37[3].AASkal[0], R37_4_AA_SKAL_ADR, 16, BICWR );	// 4 * 4 Byte
#endif

	// Vergabe der Standard-Adressen für PT1000-Modul R38
	//-----------------------------------------------------------------
#if R38_MODULE > 0
	hwdog(1);
	mod38[0].bus = 0;			// XBUS
	mod38[0].adr = 0x30;	// 1.Adresse 48D
	bicbus(EEPADR,	(char	*)&mod38[0].bus,		R38_1_BUS_ADR,	2, BICWR);		// bus und adr stehen hintereinander
	mod38[0].life = 0xFFFF; // alles lebt
	bicbus(EEPADR,	(char	*)&mod38[0].life,		R38_1_LIFE_ADR,	2, BICWR);
#endif

#if R38_MODULE > 1
	hwdog(1);
	mod38[1].bus = 0;			// XBUS
	mod38[1].adr = 0x32;	// 2.Adresse 50D
	bicbus(EEPADR,	(char	*)&mod38[1].bus,		R38_2_BUS_ADR,	2, BICWR);
	mod38[1].life = 0xFFFF;
	bicbus(EEPADR,	(char	*)&mod38[1].life,		R38_2_LIFE_ADR,	2, BICWR);
#endif

#if R38_MODULE > 2
	hwdog(1);
	mod38[2].bus = 0;			// XBUS
	mod38[2].adr = 0x34;	// 3.Adresse 52D
	bicbus(EEPADR,	(char	*)&mod38[2].bus,		R38_3_BUS_ADR,	2, BICWR);
	mod38[2].life = 0xFFFF;
	bicbus(EEPADR,	(char	*)&mod38[2].life,		R38_3_LIFE_ADR,	2, BICWR);
#endif

#if R38_MODULE > 3
	hwdog(1);
	mod38[3].bus = 0;			// XBUS
	mod38[3].adr = 0x36;	// 4.Adresse 54D
	bicbus(EEPADR,	(char	*)&mod38[3].bus,		R38_4_BUS_ADR,	2, BICWR);
	mod38[3].life = 0xFFFF;
	bicbus(EEPADR,	(char	*)&mod38[3].life,		R38_4_LIFE_ADR,	2, BICWR);
#endif

	// Vergabe der Standard-Adressen für INP-Modul R39
	//-----------------------------------------------------------------
#if R39_MODULE > 0
	hwdog(1);
	mod39[0].bus = 0;			// XBUS
	mod39[0].adr = 0x40;	// 1.Adresse 64D
	bicbus(EEPADR,	(char	*)&mod39[0].bus,		R39_1_BUS_ADR,	2, BICWR);		// bus und adr stehen hintereinander
	mod39sysinit = TRUE;	// Init Digart bei Load_pointer
#endif

#if R39_MODULE > 1
	hwdog(1);
	mod39[1].bus = 0;			// XBUS
	mod39[1].adr = 0x42;	// 2.Adresse 66D
	bicbus(EEPADR,	(char	*)&mod39[1].bus,		R39_2_BUS_ADR,	2, BICWR);
#endif

#if R39_MODULE > 2
	hwdog(1);
	mod39[2].bus = 0;			// XBUS
	mod39[2].adr = 0x44;	// 3.Adresse 68D
	bicbus(EEPADR,	(char	*)&mod39[2].bus,		R39_3_BUS_ADR,	2, BICWR);
#endif

#if R39_MODULE > 3
	hwdog(1);
	mod39[3].bus = 0;			// XBUS
	mod39[3].adr = 0x46;	// 4.Adresse 70D
	bicbus(EEPADR,	(char	*)&mod39[3].bus,		R39_4_BUS_ADR,	2, BICWR);
#endif

	// Vergabe der Standard-Adressen für OUT-Modul R33
	//-----------------------------------------------------------------
#if R33_MODULE > 0
	hwdog(1);
	mod33[0].bus = 0;			// XBUS
	mod33[0].adr = 0x50;	// 1.Adresse 80D
	bicbus(EEPADR,	(char	*)&mod33[0].bus,		R33_1_BUS_ADR,	2, BICWR);		// bus und adr stehen hintereinander
#endif

#if R33_MODULE > 1
	hwdog(1);
	mod33[1].bus = 0;			// XBUS
	mod33[1].adr = 0x52;	// 2.Adresse 82D
	bicbus(EEPADR,	(char	*)&mod33[1].bus,		R33_2_BUS_ADR,	2, BICWR);
#endif

#if R33_MODULE > 2
	hwdog(1);
	mod33[2].bus = 0;			// XBUS
	mod33[2].adr = 0x54;	// 3.Adresse 84D
	bicbus(EEPADR,	(char	*)&mod33[2].bus,		R33_3_BUS_ADR,	2, BICWR);
#endif

#if R33_MODULE > 3
	hwdog(1);
	mod33[3].bus = 0;			// XBUS
	mod33[3].adr = 0x56;	// 4.Adresse 86D
	bicbus(EEPADR,	(char	*)&mod33[3].bus,		R33_4_BUS_ADR,	2, BICWR);
#endif


	// Leittechnik Primär- und Sekundär- Ereignistabellen (Evetab) aus "KomTab.c" initialisieren
	Init_Leittab();
							
#if MODBUS_UNI > 0
	Init_Modbus();				// Adressen aus ModbusTabelle.c laden und im EEPROM sichern
#endif							

}


/*----------------------------------------------------------------------------------*/
/* Zählerinitialisierung bei Kaltstart '222'																				*/
/* Löschen des gesamten Monatsspeichers	aktuell und Vorjahr													*/
/*----------------------------------------------------------------------------------*/
void Init_Zaehler(void)
{	unsigned int adr;
	char i;
	
//	// Monatliche Speicherung für Zählerstände löschen (Struktur nach R50, wird im R36 nicht angezeigt)
//	adr = ZQ0_ADR;
//	for(i = 0; i < 36; i++)					// 36 Pages
//	{
//		hwdog(1);
//		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
//		adr += 64;
//	}
	
	// Monatliche Verbräuche aktuell und Vorjahr löschen
	//-------------------------------------------------------
	adr = ZMV01_ADR;
	for(i = 0; i < 16; i++)					// 16 Pages
	{
		hwdog(1);
		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
		adr += 64;
	}

	#if R37_MODULE > 0
	adr = ZMV11_ADR;
	for(i = 0; i < 16; i++)					// 16 Pages
	{
		hwdog(1);
		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
		adr += 64;
	}
	#endif

	#if R37_MODULE > 1
	adr = ZMV21_ADR;
	for(i = 0; i < 16; i++)					// 16 Pages
	{
		hwdog(1);
		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
		adr += 64;
	}
	#endif
	
	#if MBUSANZ > 0 
		#if MBUS_MASTER == 0
	adr = ZMVMB1_ADR;
	for ( i = 0; i < 2 * MBUSANZ; i++ )
	{																// pro Zähler 1 Page für aktuelles und 1 Page für Vorjahres-Monate löschen
		hwdog(1);
		bicbus (EEPADR,	(char *)&Null64,	adr, ZMV_VJOFFS, BICWR );
		adr += ZMV_VJOFFS;
	}
		#else
	for ( i = 0; i < MBUSANZ; i++ )
	{
		MonatNeu = 0;
		JahrNeu = 0;
		
		for ( k = 0; k < 12; k++ )
		{
			mbZaehler[i].verbr_akt[k] = 0;
			mbZaehler[i].verbr_vor[k] = 0;
			mbZaehler[i].vwert = 0;
		}		
	}
		#endif
	#endif

	//-------------------------------------------------------

// Waermemenge: Zaehlerstaende ( Monatswerte ) loeschen
#if WMENG > 0	

#if ZE7WM == 1	
	// Waermemenge: Zaehlerstaende ( Monatswerte ) loeschen
	for ( i = 0; i < 12; i++ )
	{
		wmengCtr[0].verbr[i] = 0;
		wmengCtr[0].verbr_vj[i] = 0;				
	}
	hwdog(1);
	bicbus ( EEPADR,	(char *)&wmengCtr[0].verbr,	WMENG_MON_ADR, sizeof(wmengCtr[0].verbr), BICWR);	
	bicbus ( EEPADR,	(char *)&wmengCtr[0].verbr_vj, WMENG_VJMON_ADR, sizeof(wmengCtr[0].verbr_vj), BICWR);					
#endif

#if ZE8WM == 1	
	// Waermemenge: Zaehlerstaende ( Monatswerte ) loeschen
	for ( i = 0; i < 12; i++ )
	{
		wmengCtr[1].verbr[i] = 0;
		wmengCtr[1].verbr_vj[i] = 0;				
	}
	hwdog(1);
	bicbus ( EEPADR,	(char *)&wmengCtr[1].verbr,	WMENG_MON_ADR+1*SIZE_WMENG, sizeof(wmengCtr[1].verbr), BICWR);	
	bicbus ( EEPADR,	(char *)&wmengCtr[1].verbr_vj, WMENG_VJMON_ADR+1*SIZE_WMENG, sizeof(wmengCtr[1].verbr_vj), BICWR);					
#endif

#if ZE9WM == 1	
	// Waermemenge: Zaehlerstaende ( Monatswerte ) loeschen
	for ( i = 0; i < 12; i++ )
	{
		wmengCtr[2].verbr[i] = 0;
		wmengCtr[2].verbr_vj[i] = 0;				
	}
	hwdog(1);
	bicbus ( EEPADR,	(char *)&wmengCtr[2].verbr,	WMENG_MON_ADR+2*SIZE_WMENG, sizeof(wmengCtr[2].verbr), BICWR);	
	bicbus ( EEPADR,	(char *)&wmengCtr[2].verbr_vj, WMENG_VJMON_ADR+2*SIZE_WMENG, sizeof(wmengCtr[2].verbr_vj), BICWR);					
#endif

#if ZE10WM == 1	
	// Waermemenge: Zaehlerstaende ( Monatswerte ) loeschen
	for ( i = 0; i < 12; i++ )
	{
		wmengCtr[3].verbr[i] = 0;
		wmengCtr[3].verbr_vj[i] = 0;				
	}
	hwdog(1);
	bicbus ( EEPADR,	(char *)&wmengCtr[3].verbr,	WMENG_MON_ADR+3*SIZE_WMENG, sizeof(wmengCtr[3].verbr), BICWR);	
	bicbus ( EEPADR,	(char *)&wmengCtr[3].verbr_vj, WMENG_VJMON_ADR+3*SIZE_WMENG, sizeof(wmengCtr[3].verbr_vj), BICWR);					
#endif

#endif
	
}		

/*----------------------------------------------------------------------------------*/
/* Initialisierung Leittechnik Primär- und Sekundär- Ereignistabellen (Evetab)			*/
/* Initialisierung Alarmtabelle																											*/
/* Kaltstart '223'																																	*/
/*----------------------------------------------------------------------------------*/
void Init_Leittab(void)
{
	Init_Evetab();
	Init_Alarmtab();
}

/*----------------------------------------------------------------------------------*/
/* Initialisierung Heizgradtage bei Kaltstart '224'																	*/
/*----------------------------------------------------------------------------------*/
void Init_Hgt_User(void)
{
	// Heizgradtage fuer 24 Monate, 12 * 8 ( sizeof monHzGrd ) = 96 Byte = 1,5 Pages
	hwdog(1);
	bicbus(EEPADR,	(char *)&Null64,	HEIZGRADTAGE_ADR, 	 64, BICWR);		// 64 Bytes (  1 Page) löschen
	bicbus(EEPADR,	(char *)&Null64,	HEIZGRADTAGE_ADR+64, 32, BICWR);		// 32 Bytes (1/2 Page) löschen
}

/*----------------------------------------------------------------------------------*/
/* Transport Standardwerte aus Typenlisten (UserConst.c) in den UserRam							*/
/* Aufruf gestartet von Main(Reset) über BicMove.c in der Funktion Standard()				*/
/*----------------------------------------------------------------------------------*/

void Typelist_copy(void)
{
	char profil, i;
	
//#####ulsch: skalierbare Analogeingänge
	memcpy ( (void *)&AnaInpPara, (void *)&AnaInpStandardPara, sizeof(AnaInpPara) );
#if RM_POWER_ANZ	
	memcpy ( (void *)&RmPowerPara, (void *)&RmPowerStandardPara, sizeof(RmPowerPara) );
#endif
#if AE_DRUCK_ANZ	
	memcpy ( (void *)&DruckPara, (void *)&DruckStandardPara, sizeof(DruckPara) );
#endif

	// Auswahl nach Projekt Typ
	if(proj_typ >= PROJTYPEN)	// auf max. abtesten
		proj_typ = 0;						// bei Fehler, Dummy-Projekt

// MBUS verlagert nach Kaltstart 67 wegen Kompatibilität zu Impulszählern
//-#if MBUSANZ > 0
//-	// Standardwerte für MBUS
//-	for ( i = 0; i < MBUSANZ; i++ )
//-	{	
//-		profil = Projekte[proj_typ].mbcode[i];			// Laden der Profil Nummer
//-		if ( profil < MB_PROFILE )											// auf max abtesten
//-			// Speicherbereich für Standardwerte laden
//-			memcpy( (void *)&MbusPara[i], (void *)&Mbus_Standparam[profil], MBUSLENG );
//-	}		
//-#endif
//-
	// Standardwerte für Heizkreise
	for(i = 0; i < HKMAX; i++)
	{	profil = Projekte[proj_typ].hkcode[i];			// Laden der Profil Nummer
		if(profil < HK_PROFILE)											// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&hks[i], (void *)&Hk_Standparam[profil], HKSLENG );
	}		
	
	// Absenkprofile für Heizkreise
	for(i = 0; i < HKMAX; i++)
	{	profil = Projekte[proj_typ].abscode[i];			// Laden der Profil Nummer
		if(profil < ABS_PROFILE)										// auf max abtesten
			// Speicherbereich für Standardwerte laden
		memcpy( (void *)&abs_tab[i], (void *)&abs_standard[profil], 8*ABSTABLENG );
	}		


	// Standardwerte für Warmwasserkreise
	for(i = 0; i < WWMAX; i++)
	{	profil = Projekte[proj_typ].wwcode[i];			// Laden der Profil Nummer
		if(profil < WW_PROFILE)											// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&wws[i], (void *)&Ww_Standparam[profil], WWSLENG );
	}		

	// Standardwerte für Kesselkreise
	for(i = 0; i < KEMAX; i++)
	{	profil = Projekte[proj_typ].kecode[i];			// Laden der Profil Nummer
		if(profil < KE_PROFILE)											// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&kes[i], (void *)&Ke_Standparam[profil], KESLENG );
	}
	

	// Standardwerte für Vorregelkreise (Netztrennung)
	for(i = 0; i < NTMAX; i++)
	{	profil = Projekte[proj_typ].ntcode[i];			// Laden der Profil Nummer
		if(profil < NT_PROFILE)											// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&nts[i], (void *)&Nt_Standparam[profil], NTSLENG );
	}

	// Standardwerte für Solarkreise
	for ( i = 0; i < SOLMAX; i++ )
	{	
		profil = Projekte[proj_typ].socode[i];			// Laden der Profil Nummer
		if ( profil < SO_PROFILE )									// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&sos[i], (void *)&So_Standparam[profil], SOSLENG );
	}

	// Standardwerte für Benutzersteuerung mit UNI-Elementen
	#if STEUER_UNI == 1
		i = 0;
		profil = Projekte[proj_typ].unicode[i];			// Laden der Profil Nummer
		if ( profil < UNI_PROFILE )									// auf max abtesten
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&unis[i], (void *)&Uni_Standparam[profil], UNISLENG );
	#endif

	// Standardwerte für Bus-Pumpen
	#if ( GENI == 1 || WILO_MODBUS == 1 )
		#if BUS_PU_MAX > 0 
		for ( i = 0; i < BUS_PU_MAX; i++ )
		{	
			// Speicherbereich für Standardwerte laden
			memcpy( (void *)&BusPuPara[i], (void *)&PuBus_Standparam[i], BUS_PU_LENG );
	// Änderung neue Genibus-Implementierung
			#if ( ( ( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) || ( ( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )
			genibus_device_table[i].uc_adr = PuBus_Standparam[i].Adresse;
			#endif
		}
		#endif
	#endif

	
}


/*--------------------------------------------------------------------------------------*/
/* Laden von User-Definitionen und Ein/Ausgangskonfiguration in den System- oder UserRam*/
/* Aufruf gestartet von Main(nach Reset) über BicMove.c in der Funktion Load_Config()		*/
/*--------------------------------------------------------------------------------------*/
void Load_Userdef()
{
	char i, j, k, n, dev, profil;
	unsigned int *p;
	unsigned int mask;
	
	mask = 0;
	
	// Alle Pointer auf Ein-/Ausgänge mit einem Dummy-Wert belegen
	fill_dummy();
	
	//----------------------------------------------------------------
	// Konfiguration der Ein- und Ausgänge
	//----------------------------------------------------------------
	// Check proj_typ
	if(proj_typ >= PROJTYPEN)		// auf max. abtesten
		proj_typ = 0;							// bei Fehler, Dummy-Projekt
		
	temp_proj_typ = proj_typ;		// für parli zur Anzeige, und zur
															// Überprüfung bzw. Neueinstellung in "Output.c"
		
	// Prüfen und Laden der Pointer auf Ein-/Ausgänge des R36
	// Zeiger auf ersten Eingang IN1 bilden (siehe "UserConst.c")
	p = (unsigned int *)&Projekte[proj_typ].in1;	
	for(i = 0; i < 18; i++)					// insgesamt 18 Ein-/Ausgänge
	{
		load_eapointer36(p, i, 0);		// i beginnt bei IN1 und endet bei DA6
																	// ext = 0: Hauptgerät R36
		p++;	
	}
	
	// Prüfen und Laden der Pointer auf Ein-/Ausgänge der Erweiterungsmodule  R37, R38, R39..
	for(k = 0; k < 4; k++)													// maximal 4 Module
	{	hwdog(1);
		dev = Projekte[proj_typ].iobl[k].iodev;				// Gerätekennzeichen testen
		switch(dev)
		{	
			case 0:																			// kein Gerät
				break;
			
			case R37:
				j = Projekte[proj_typ].iobl[k].ionum;		  // R37-Gerätenummer 
				n =	Projekte[proj_typ].iobl[k].iobel;			// Nummer des IN/OUT-Belegungsblockes
				p = (unsigned int *)&R37_Beleg[n].xde1;		// Zeiger auf ersten Eintrag im Belegungsblock
				for(i = 0; i < 13; i++)										// insgesamt 13 Ein-/Ausgänge
				{	load_eapointer37(p, i, j);							// i: beginnt bei DE1 und endet bei DA3
																									// j: R37-Gerätenummer, bildet Index für Ram37-Arbeitsspeicher
					p++;	
				}
				r37vis[j-1] = SICHTBAR;
				break;
			
			case R38:
				j = Projekte[proj_typ].iobl[k].ionum;		  // R38-Gerätenummer 
				n =	Projekte[proj_typ].iobl[k].iobel;			// Nummer des IN/OUT-Belegungsblockes
				p = (unsigned int *)&R38_Beleg[n].xae1;		// Zeiger auf ersten Eintrag im Belegungsblock
				for(i = 0, mask = 1; i < 14; i++, mask <<= 1)					// insgesamt 14 Eingänge
				{	load_eapointer38(p, i, j);							// i: beginnt bei IN1 und endet bei IN14
																									// j: R38-Gerätenummer, bildet Index für Ram38-Arbeitsspeicher
					if(*p != TP_FREI)												// Maske aller verwendeten Pt1000-Eingänge erzeugen
						mod38[j-1].inpUsed |= mask;						// wird in InoutR.c mit übertragen
						
					p++;	
				}
				r38vis[j-1] = SICHTBAR;
				break;

			case R39:
				j = Projekte[proj_typ].iobl[k].ionum;		  // R39-Gerätenummer 
				n =	Projekte[proj_typ].iobl[k].iobel;			// Nummer des IN/OUT-Belegungsblockes
				p = (unsigned int *)&R39_Beleg[n].xde1;		// Zeiger auf ersten Eintrag im Belegungsblock
				for(i = 0; i < 12; i++)										// insgesamt 12 Eingänge
				{	load_eapointer39(p, i, j);							// i: beginnt bei DE1 und endet bei DE12
																									// j: R39-Gerätenummer, bildet Index für Ram39-Arbeitsspeicher
					p++;	
				}
				if(mod39sysinit == TRUE)									// Merkmal für SysEEP_InitUser  (Kaltstart 66 wurde ausgeführt)	
				{	j--;
					mod39[j].Digart = R39_Beleg[n].art;			// Signalart: 0=Schließer 1=Öffner
					bicbus(EEPADR,	(char	*)&mod39[j].Digart,	R39_1_DIGART_ADR + 2*j,	2, BICWR);	// speichern
					j++;
				}	
				r39vis[j-1] = SICHTBAR;
				break;

			case R33:
				j = Projekte[proj_typ].iobl[k].ionum;		  // R33-Gerätenummer 
				n =	Projekte[proj_typ].iobl[k].iobel;			// Nummer des IN/OUT-Belegungsblockes
				p = (unsigned int *)&R33_Beleg[n].xda1;		// Zeiger auf ersten Eintrag im Belegungsblock
				for(i = 0; i < 6; i++)										// insgesamt 6 Ausgänge
				{	load_eapointer33(p, i, j);							// i: beginnt bei DA1 und endet bei DA6
																									// j: R33-Gerätenummer, bildet Index für Ram33-Arbeitsspeicher
					p++;	
				}
				r33vis[j-1] = SICHTBAR;
				break;
		}
	}				
	mod39sysinit = FALSE;
	
	// Laden von Typ und Version des jeweiligen Projektes
	memcpy( (void *)&Version_PTyp[0], (void *)&Projekte[proj_typ].Typ, 15 );
	memcpy( (void *)&Version_PVers[0], (void *)&Projekte[proj_typ].Vers, 3 );
	// Laden des Kategorietyps bei variablen Projekten
	memcpy( (void *)&KatVar_Typ[0], (void *)&Projekte[proj_typ].KatVar, 15 );
	namur = Projekte[proj_typ].namur;	

	// Anzeige der Parametergruppen
	//-----------------------------
	// Standardeinstellung
	anl_vis = SICHTBAR;								// immer sichtbar
	zlt_vis = SICHTSERV1;							// sichtbar nur ab Servicecode 1 
	sys_vis = SICHTSERV1;
	tes_vis = SICHTSERV4;							// Testgruppe     sichtbar nur ab Servicecode 4 
	syc_vis = SICHTSERV3;							// Synchrongruppe sichtbar nur ab Servicecode 3
// ulsch	
	arp_vis = SICHTSERV1;							// sichtbar nur ab Servicecode 1	
	
	// Projektabhängige Einstellung
	
	// Heizkreise
	for(i = 0; i < HKMAX; i++)
	{	if(Projekte[proj_typ].hkcode[i] > 0)	
		{	hk_vis[i] = SICHTBAR;
			hwdog(1);
			bicbus ( EEPADR,	(char	*)&hkd[i].raumname, RAUMNAME1_ADR + i*16, 16, BICRD );
		}	
	}
	
	// Warmwasserkreise
	for(i = 0; i < WWMAX; i++)
	{	if(Projekte[proj_typ].wwcode[i] > 0)	
		ww_vis[i] = SICHTBAR;
	}

	// Kesselkreise	
	for(i = 0; i < KEMAX; i++)
	{	if(Projekte[proj_typ].kecode[i] > 0)	
		ke_vis[i] = SICHTBAR;
	}

	// Vorregelkreise	
	for(i = 0; i < NTMAX; i++)
	{	if(Projekte[proj_typ].ntcode[i] > 0)	
		nt_vis[i] = SICHTBAR;
	}
	
	// Solarkreise	
	for ( i = 0; i < SOLMAX; i++ )
		if ( Projekte[proj_typ].socode[i] > 0 )	
			so_vis[i] = SICHTBAR;

#if MBUSANZ > 0
	#if MBUS_MASTER == 0
	for ( i = 0; i < MBUSANZ; i++ )
		MBZE[i] = &mbZaehler[i];
	#else 
	for ( i = 0; i < MBUSANZ; i++ )
	{
		MBZE[i] = &mbZaehler[i];
//		memcpy ( (void *)&mbZaehler[i].zdim, (void *)&ze_dimsys[i+20], sizeof(zaehldim) );
	}
	#endif
#endif	

// #####ulsch : Ferienprogramm #####
#if FER	
	fer_vis = SICHTBAR;
#else
	fer_vis = UNSICHTB;
#endif

/***** ulsch : Heizgradtage *****/
#if HEIZGRD	
	hgt_vis = SICHTBAR;
#else
	hgt_vis = UNSICHTB;
#endif

// Quittierbare Gespeicherte Störmeldungen
#if QUIT_TASTE	
	qsm_vis = SICHTBAR;
#else
	qsm_vis = UNSICHTB;
#endif

	// Anzeige sichtbarer und versteckter Parameter in einer Gruppe
	vis  = SICHTBAR;			// immer sichtbar	
	hid1 = SICHTSERV1;		// sichtbar nur ab Servicecode 1 (135)
	hid2 = SICHTSERV2;		// sichtbar nur ab Servicecode 2 (137)
	hid3 = SICHTSERV3;		// sichtbar nur ab Servicecode 3 (143)
	hid4 = SICHTSERV4;		// sichtbar nur ab Servicecode 4 (139)

	// Überprüfen des Versionsdatum	
	Check_Version();			// bei einer Veränderung wird Init_Leittab() aufgerufen (entspricht Kaltstart 223) 
	
	// Laden der Evetab vom EEPROM in den RAM
	bicbus(EEPADR,	(char	*)&mask,	EVETABKZ_ADR, 2, BICRD);	// Überprüfen des Initkennzeichens
	if(mask == EVETAB_KZ)
		move_evetab(BICRD);		


	// Laden der Alarmtab vom EEPROM in den RAM
	bicbus(EEPADR,	(char	*)&mask,	ALATABKZ_ADR, 2, BICRD);	// Überprüfen des Initkennzeichens
	if(mask == ALATAB_KZ)
		move_alarmtab(BICRD);		

	hwdog(1);

// Laden der Objekt- und Standortbezeichnungen vom EEPROM in den RAM
	bicbus(EEPADR,	(char *)&Objektbez,	OBJBEZ_ADR,		15, BICRD);
	bicbus(EEPADR,	(char *)&Objektnum,	OBJNUM_ADR,		 9, BICRD);
	hwdog(1);
	bicbus(EEPADR,	(char *)&Strasse,		STRASSE_ADR,	15, BICRD);
	bicbus(EEPADR,	(char *)&Nummer,		NUMMER_ADR,		 9, BICRD);
	hwdog(1);
	bicbus(EEPADR,	(char *)&Ort,				ORT_ADR,			15, BICRD);
	bicbus(EEPADR,	(char *)&Plz,				PLZAHL_ADR,		 9, BICRD);
	hwdog(1);

// Programmversion als String speichern	
	ctoa(&Version_PVers_Ascii[0], Version_PVers[2]);		// Tag
	Version_PVers_Ascii[2] = ':';
	ctoa(&Version_PVers_Ascii[3], Version_PVers[1]);		// Monat
	Version_PVers_Ascii[5] = ':';
	ctoa(&Version_PVers_Ascii[6], Version_PVers[0]);		// Jahr

}
	


/*----------------------------------------------------------------------------------*/
/*---------------               Unterprogramme                ----------------------*/
/*----------------------------------------------------------------------------------*/


// Unterprogramm zu Load_Userdef
// InOut-Check und Laden der Pointeradresse für einen Ein-/Ausgang R36
void load_eapointer36(unsigned int *ea, char idx, char ext)
{
	char typ, klasse, num, retnum;
	unsigned int eatyp;
	unsigned int label = 0;
	
	// Extrahieren der Definition des Ein/Ausgangs (Definitionen in "userdef.h")
	eatyp 	= *ea;
	typ			= (char)(eatyp >> 12);
	klasse	= (char)(eatyp >> 4);
	num			= (char)(eatyp & 0x000F);
	

	// 1. InOut-Konfigurations-Check mit Bildung der Steuerworte für die Hardware
	//---------------------------------------------------------------------------
	// 		bei Fehler: LED- und LCD-Anzeige und Prozess-Stop !  
	switch(idx)		// bezieht sich auf Prozess-E/A
								// (definiert in Anlage Projekte[PROJNUM] in "UserConst.c")
	{
		case 0:													// IN1
		case 1:													// IN2
			if(typ != 1)									// nur Pt1000
				Fehler(1, idx);	
			else
				ptinput1 = 0x03;						// Konfigurationsanzeige
			break;
			
		case 2:													// IN3
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 1)
				{	aswcontrol = 0x10;					// IN3,IN4,IN5 als Block
					ptcontrol  = 0x07;					// 3 x Pt1000
					ptinput1	|= 0x1C;					// Konfigurationsanzeige
				}
				else
				{ if(typ == 2)
					{	aecontrol = 0x01;					// 0-10V
						aeinput1 |= 0x04;					// Konfigurationsanzeige
					}
					else // Typ 3 oder 4
					{	diginput1 |= 0x04;	
						if(typ == 4)
						{	cntcontrol |= 0x01;
							zinput1    |= 0x04;
						}	
					}		
				}
			}						
			break;
			
		case 3:													// IN4	
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else	
			{	if(aswcontrol == 0x10)				// wenn Block gesetzt,
				{	if(typ != 1)								// muss Pt1000 sein
						Fehler(3, idx);
				}
				else
				{ if(typ == 2)
					{	aecontrol |= 0x02;				// 0-10V
						aeinput1  |= 0x08;				// Konfigurationsanzeige
					}
					else // Typ 3 oder 4
					{	diginput1 |= 0x08;				// Konfigurationsanzeige
						if(typ == 4)
						{	cntcontrol |= 0x02;
							zinput1    |= 0x08;
						}
					}		
				}
			}						
			break;
			
		case 4:													// IN5	
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else if(aswcontrol == 0x10)				// wenn Block gesetzt,
			{	if(typ != 1)								// muss Pt1000 sein
					Fehler(3, idx);
			}
			else
			{ if(typ == 2)
				{	aecontrol |= 0x04;				// 0-10V
					aeinput1  |= 0x10;				// Konfigurationsanzeige
				}
				else // Typ 3 oder 4
				{	diginput1 |= 0x10;				// Konfigurationsanzeige
					if(typ == 4)
					{	cntcontrol |= 0x04;
						zinput1    |= 0x10;
					}
				}		
			}					
			break;
		
		case 5:													// IN6	
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 1)
				{	aswcontrol |= 0x20;
					ptcontrol  |= 0x08;					// Pt1000
					ptinput1	 |= 0x20;					// Konfigurationsanzeige
				}	
				else
				{ if(typ == 2)
					{	aecontrol |= 0x08;				// 0-10V
						aeinput1  |= 0x20;				// Konfigurationsanzeige
					}
					else // Typ 3 oder 4
					{	diginput1 |= 0x20;				// Konfigurationsanzeige
						if(typ == 4)
						{	cntcontrol |= 0x08;
							zinput1    |= 0x20;
						}	
					}		
				}
			}						
			break;
				
		case 6:													// IN7	
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else	
			{	if(typ == 1)
				{ aswcontrol |= 0x40;	
					ptcontrol  |= 0x10;					// Pt1000
					ptinput1	 |= 0x40;					// Konfigurationsanzeige
				}	
				else
				{ if(typ == 2)
					{	aecontrol |= 0x10;				// 0-10V
						aeinput1  |= 0x40;				// Konfigurationsanzeige
					}
					else // Typ 3 oder 4
					{	diginput1 |= 0x40;				// Konfigurationsanzeige
						if(typ == 4)
						{	cntcontrol |= 0x10;
							zinput1    |= 0x40;
						}	
					}		
				}
			}						
			break;
				
		case 7:													// IN8	
			if(typ == 0 || typ > 4)
				Fehler(2, idx);
			else	
			{	if(typ == 1)
				{ aswcontrol |= 0x80;	
					ptcontrol  |= 0x20;					// Pt1000
					ptinput1	 |= 0x80;					// Konfigurationsanzeige
				}	
				else
				{ if(typ == 2)
					{	aecontrol |= 0x20;				// 0-10V
						aeinput1  |= 0x80;				// Konfigurationsanzeige
					}
					else // Typ 3 oder 4
					{	diginput1 |= 0x80;				// Konfigurationsanzeige
						if(typ == 4)
						{	cntcontrol |= 0x20;
							zinput1    |= 0x80;
						}	
					}		
				}
			}						
			break;
				
		case 8:													// IN9	
			if(typ < 2 || typ > 4)
				Fehler(1, idx);
			else	
			{	if(typ == 2)
				{		aecontrol |= 0x40;				// 0-10V
						aeinput2  |= 0x01;				// Konfigurationsanzeige
				}
				else // Typ 3 oder 4
				{	diginput2 |= 0x01;				// Konfigurationsanzeige
					if(typ == 4)
					{	cntcontrol |= 0x40;
						zinput2    |= 0x01;
					}	
				}
			}			
			break;
				
		case 9:													// IN10	
			if(typ < 2 || typ > 4)
				Fehler(1, idx);
			{	
				if(typ == 2)
				{		aecontrol |= 0x80;				// 0-10V
						aeinput2  |= 0x02;				// Konfigurationsanzeige
				}
				else // Typ 3 oder 4
				{	diginput2 |= 0x02;				// Konfigurationsanzeige
					if(typ == 4)
					{	cntcontrol |= 0x80;
						zinput2    |= 0x02;
					}	
				}
			}			
			break;
				
		case 10:												// Analog-Ausgang  AA1
		case 11:												// Analog-Ausgang  AA2
			if(typ != 5)
				Fehler(4, (char)(idx-10));
			else
				anaoutput1 = 0x03;					// Konfigurationsanzeige
			break;			

		case 12:												// DA1
		case 13:												// DA2
		case 14:												// DA3
		case 15:												// DA4
		case 16:												// DA5
		case 17:												// DA6
			if(typ != 6)
				Fehler(5, (char)(idx-12));
			else
				digoutput1 = 0x3F;					// Konfigurationsanzeige
			break;
			
		default:
			Fehler(6, 0);
			break;
	}					

	if(bs_error == 0)		// wenn Fehler() nicht aufgerufen wurde
	{
		// 2. Pointer laden
		retnum = LoadPointer(typ, klasse, num, idx, ext, R36, &label);
		
		// 3. Laden der Indizes für Stringfelder (Ein-/Ausgangsnamen)
		//------------------------------------------------------------------------
		nlist[idx]			= (eatyp & 0xFFF0) | retnum;
		
		// 4. Parameter suchen und Textliste laden
		ListParameter(ntext[idx], klasse, label);

	}
}


//-----------------------------------------------------------------------------	
// Unterprogramm zu Load_Userdef
// InOut-Check und Laden der Pointeradresse für einen Ein-/Ausgang des R37
//-----------------------------------------------------------------------------	
void load_eapointer37(unsigned int *ea, char idx, char ext)
{
	char typ, klasse, num, retnum;
	unsigned int eatyp;
	unsigned int label = 0;
	
	// Extrahieren der Definition des Ein/Ausgangs (Definitionen in "userdef.h")
	eatyp 	= *ea;
	typ			= (char)(eatyp >> 12);
	klasse	= (char)(eatyp >> 4);
	num			= (char)(eatyp & 0x000F);
	
	// 1. InOut-Konfigurations-Check
	//---------------------------------------------------------------------------
	// 		bei Fehler: LED- und LCD-Anzeige und Prozess-Stop !  
	switch(idx)		// bezieht sich auf Prozess-E/A
								// (definiert in Anl37 R37_Beleg[R37SETS] in "UserConst.c")
	{
		case 0:											// IN1 bis IN 6
			if(typ < 3 || typ > 4)
				Fehler(2, idx);					// nur Digital- oder Zähleingang zugelassen
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x01;
			}		
			break;		
		case 1:
			if(typ < 3 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x02;
			}		
			break;		
		case 2:
			if(typ < 3 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x04;
			}		
			break;		
		case 3:
			if(typ < 3 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x08;
			}		
			break;		
		case 4:
			if(typ < 3 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x10;
			}		
			break;		
		case 5:
			if(typ < 3 || typ > 4)
				Fehler(2, idx);
			else
			{	if(typ == 4)
					zinput_r37[ext-1] |= 0x20;
			}		
			break;		

		
		case 6:											// Analog-Ausgang  AA1 bis AA4
		case 7:
		case 8:
		case 9:
			if(typ != 5)
				Fehler(4, (char)(idx-6));
			break;	
				
		case 10:										// DA1
		case 11:										// DA2
		case 12:										// DA3
			if(typ != 6)
				Fehler(5, (char)(idx-10));
			break;
	}
				
	if(bs_error == 0)		// wenn Fehler() nicht aufgerufen wurde
	{
		// 2. Pointer laden
		retnum = LoadPointer(typ, klasse, num, idx, ext, R37, &label);
		
		// 3. Laden der Indizes für Stringfelder (Ein-/Ausgangsnamen)
		//------------------------------------------------------------------------
		n37list[ext-1][idx] = (eatyp & 0xFFF0) | retnum;

		// 4. Parameter suchen und Textliste laden
		ListParameter(n37text[ext-1][idx], klasse, label);

	}
}

//-----------------------------------------------------------------------------	
// Unterprogramm zu Load_Userdef
// InOut-Check und Laden der Pointeradresse für einen Eingang des R38
//-----------------------------------------------------------------------------	
void load_eapointer38(unsigned int *ea, char idx, char ext)
{
	char typ, klasse, num, retnum;
	unsigned int eatyp;
	unsigned int label = 0;
	
	// Extrahieren der Definition des Ein/Ausgangs (Definitionen in "userdef.h")
	eatyp 	= *ea;
	typ			= (char)(eatyp >> 12);
	klasse	= (char)(eatyp >> 4);
	num			= (char)(eatyp & 0x000F);
	
	// 1. InOut-Konfigurations-Check
	//---------------------------------------------------------------------------
	// 		bei Fehler: LED- und LCD-Anzeige und Prozess-Stop !  
	switch(idx)		// bezieht sich auf Prozess-E/A
								// (definiert in Anl38 R38_Beleg[R38SETS] in "UserConst.c")
	{
		case 0:											// IN1 bis IN 14
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:													
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			if(typ != 1)							// nur Pt1000
				Fehler(1, idx);	
			break;	
	}
				
	if(bs_error == 0)		// wenn Fehler() nicht aufgerufen wurde
	{
		// 2. Pointer laden
		retnum = LoadPointer(typ, klasse, num, idx, ext, R38, &label);
		
		// 3. Laden der Indizes für Stringfelder (Ein-/Ausgangsnamen)
		//------------------------------------------------------------------------
		n38list[ext-1][idx] = (eatyp & 0xFFF0) | retnum;

		// 4. Parameter suchen und Textliste laden
		ListParameter(n38text[ext-1][idx], klasse, label);

	}
}

//-----------------------------------------------------------------------------	
// Unterprogramm zu Load_Userdef
// InOut-Check und Laden der Pointeradresse für einen Eingang des R39
//-----------------------------------------------------------------------------	
void load_eapointer39(unsigned int *ea, char idx, char ext)
{
	char typ, klasse, num, retnum;
	unsigned int eatyp;
	unsigned int label = 0;
	
	// Extrahieren der Definition des Ein/Ausgangs (Definitionen in "userdef.h")
	eatyp 	= *ea;
	typ			= (char)(eatyp >> 12);
	klasse	= (char)(eatyp >> 4);
	num			= (char)(eatyp & 0x000F);
	
	// 1. InOut-Konfigurations-Check
	//---------------------------------------------------------------------------
	// 		bei Fehler: LED- und LCD-Anzeige und Prozess-Stop !  
	switch(idx)		// bezieht sich auf Prozess-E/A
								// (definiert in Anl39 R39_Beleg[R39SETS] in "UserConst.c")
	{
		case 0:											// DE1 bis DE12
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:													
		case 8:
		case 9:
		case 10:
		case 11:
			if(typ != 3)
				Fehler(2, idx);					// nur Digitaleingang zugelassen
			break;
	}
				
	if(bs_error == 0)		// wenn Fehler() nicht aufgerufen wurde
	{
		// 2. Pointer laden
		retnum = LoadPointer(typ, klasse, num, idx, ext, R39, &label);
		
		// 3. Laden der Indizes für Stringfelder (Ein-/Ausgangsnamen)
		//------------------------------------------------------------------------
		n39list[ext-1][idx] = (eatyp & 0xFFF0) | retnum;

		// 4. Parameter suchen und Textliste laden
		ListParameter(n39text[ext-1][idx], klasse, label);

	}
}

//-----------------------------------------------------------------------------	
// Unterprogramm zu Load_Userdef
// InOut-Check und Laden der Pointeradresse für einen Ausgang des R33
//-----------------------------------------------------------------------------	
void load_eapointer33(unsigned int *ea, char idx, char ext)
{
	char typ, klasse, num, retnum;
	unsigned int eatyp;
	unsigned int label = 0;
	
	// Extrahieren der Definition des Ein/Ausgangs (Definitionen in "userdef.h")
	eatyp 	= *ea;
	typ			= (char)(eatyp >> 12);
	klasse	= (char)(eatyp >> 4);
	num			= (char)(eatyp & 0x000F);
	
	// 1. InOut-Konfigurations-Check
	//---------------------------------------------------------------------------
	// 		bei Fehler: LED- und LCD-Anzeige und Prozess-Stop !  
	switch(idx)		// bezieht sich auf Prozess-E/A
								// (definiert in Anl33 R33_Beleg[R33SETS] in "UserConst.c")
	{
		case 0:											// DA1 bis DA6
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			if(typ != 6)
				Fehler(5, idx);					// nur Digitalausgang zulassen
			break;
	}
				
	if(bs_error == 0)		// wenn Fehler() nicht aufgerufen wurde
	{
		// 2. Pointer laden
		retnum = LoadPointer(typ, klasse, num, idx, ext, R33, &label);
		
		// 3. Laden der Indizes für Stringfelder (Ein-/Ausgangsnamen)
		//------------------------------------------------------------------------
		n33list[ext-1][idx] = (eatyp & 0xFFF0) | retnum;

		// 4. Parameter suchen und Textliste laden
		ListParameter(n33text[ext-1][idx], klasse, label);

	}
}

// Unterprogramm zu ListParameter: Suchen und Laden der Texte für Ein/Ausgänge
char LoadParamText(char *liste, unsigned int param)
{
	char i, j;
	char ret = 0;
	unsigned int label = 0;
	char pleng = 0;
	char grtext[5] = {0};
	
	
	for(i = 0; i < PARLIMAX; i++)
	{
		// nicht relevante Gruppen übergehen
		strcpy(grtext, Pgruppe[i].grkz);
		if(		
				  strncmp(grtext, "ALK", 3) == 0
				||strncmp(grtext, "ZLT", 3) == 0
				||strncmp(grtext, "TES", 3) == 0
				||strncmp(grtext, "SYS", 3) == 0
			)
			continue;		// nächste Gruppe vergleichen	
		else
		{
			pleng = Pgruppe[i].pleng;		// Länge der Parametergruppe
			// Parameter suchen			
			for(j = 0; j < pleng; j++)
			{
				label = (unsigned int)Pgruppe[i].Param[j].label;
				if(label == param)													// Parameter gefunden
				{
					strcpy(liste, Pgruppe[i].grkz);						// Gruppenkennzeichen
					strcat(liste, Pgruppe[i].Param[j].bez);		// Bezeichnung
					ret = 1;																	// gefunden
					break;
				}	 
			}	
		}
		if(ret == 1)
			break;	
	}
	return(ret);
		
}
	
// Unterprogramm: Textliste für Ein/Ausgänge füllen
void ListParameter(char *liste, char klasse, unsigned int label)
{
		if(klasse == 0)												// wenn xx_FREI, d.h. nicht belegter Ein-/Ausgang
				strcpy(liste, "*->: UNBELEGT       ");
		else if(label == 0)															// ungültige Information
				strcpy(liste, "                    ");
		else
		{
			if(LoadParamText(liste, label) == 0)			// wenn Parameter nicht gefunden		
				strcpy(liste, "*->: NICHT GEFUNDEN ");
		}
	
}	



// Fehlertexte
//										 --------------------
const char Text1[] = {"IN   unguelt.Eing. !\0"};
const char Text2[] = {"IN   kein Eingang  !\0"};
const char Text3[] = {"IN   ungleich IN3  !\0"};
const char Text4[] = {"AA   kein Ana.Ausg.!\0"};
const char Text5[] = {"DA   kein Dig.Ausg.!\0"};
const char Text6[] = {"XX   unbekannt     !\0"};

const char Text7[]  = {"IN   Doppelbelegung!\0"};
const char Text8[]  = {"AA   Doppelbelegung!\0"};
const char Text9[]	= {"DA   Doppelbelegung!\0"};
const char Text10[] = {"ZE   Falschbelegung!\0"};

const char Text11[] = {"     Alarmtabelle  !\0"};




/* Fehlerausgabe auf LCD-Anzeige					*/
void Print(const char *text, char idx)
{
	strcpy(LCD_Ram, text);
	ctoa(&LCD_Ram[2], (char)(idx+1));
	strcpy(pub_LCD_RamZ1, LCD_Ram);					// für Übergabe an Protected System
	LCD_Anz();
}

/* Fehlerausgabe auf LCD-Anzeige					*/
void Print2(const char *text, signed char idx)
{
	char zahl;
	zahl = (char)abs(idx);
	
	strcpy(LCD_Ram, text);
	if(idx < 0)
		LCD_Ram[1] = '-';
	else
		LCD_Ram[1] = '+';
	ctoa(&LCD_Ram[2], zahl);
	strcpy(pub_LCD_RamZ1, LCD_Ram);					// für Übergabe an Protected System
	LCD_Anz();
}

/* Fehlerausgabe mit LED  								*/
void Blink(char rot, char gelb, char blau, char idx)
{
	char i;
	
	#if LED_DEBUG == 0
	LedRot   = rot;
	LedGelb  = gelb;
	LedBlau  = blau;
	#endif
	ms_delay(500);		// 500 ms

	#if LED_DEBUG == 0
	LedRot   = 0;
	LedGelb  = 0;
	LedBlau = 0;
	#endif
	ms_delay(500);
	
	for(i = 0; i < idx+1; i++)
	{
		#if LED_DEBUG == 0
		LedBlau = 1;
		#endif
		ms_delay(100);		// 100 ms
		#if LED_DEBUG == 0
		LedBlau = 0;
		#endif
		ms_delay(200);
	}
}	

/* 	Fehleranzeige bei fehlerhafter In-Out-Konfiguration	*/
//	Betriebssystem wird nicht gestartet !	
void Fehler(char code, signed char idx)		
{
	char ewig = 0;
	
	while(ewig < 2)						// 2 x Schleife
	{
		switch(code)
		{	//    rot,gelb,grün
			case 1:									// Eingang 1 oder 2 ist kein Pt1000
				Blink(1, 0, 0, idx);	// idx+1 zeigt, welcher Eingang falsch definiert
				Print(Text1, idx);
				break;
			case 2:									// Ein Eingang ist nicht als Eingang definiert
				Blink(0, 1, 0, idx);
				Print(Text2, idx);
				break;
			case 3:									// Eingang 3,4,5 sind nicht einheitlich definiert
				Blink(1, 1, 0, idx);	// (wenn Eingang 3 Pt1000, dann 4 und 5 auch Pt1000)
				Print(Text3, idx);
				break;
			case 4:									// Ein Analogausgang ist nicht als Analogausgang definiert
				Blink(0, 0, 1, idx);
				Print(Text4, idx);
				break;
			case 5:									// Ein Digitalausgang ist nicht als Digitalausgang definiert
				Blink(1, 0, 1, idx);	//  
				Print(Text5, idx);
				break;
			case 6:									// Fatal-Error
				Blink(1, 1, 1, idx);
				Print(Text6, idx);
				break;
			
			case 7:									// IN Eingang Doppelt belegt
				Blink(1, 1, 1, idx);
				Print(Text7, idx);
				break;
			case 8:									// AA Ausgang Doppelt belegt
				Blink(1, 1, 1, idx);
				Print(Text8, idx);
				break;
			case 9:									// DA Ausgang Doppelt belegt
				Blink(1, 1, 1, idx);
				Print(Text9, idx);
				break;
			case 10:								// ZIN Eingang falsch
				Blink(1, 1, 1, idx);
				Print(Text10, idx);
				break;
				
			case 11:								// alarmtab	zu klein
				Blink(1, 1, 1, 1);
				Print2(Text11, idx);
				break;
				
		}
		ms_delay(1000);		// 1 s
		ewig++;
	}
	bs_error = 1;				// Betriebssystem-Error, führt in main_func() zum Rücksprung nach protected main()
}

//---------------------------------------------------------------------------------------------
// UP: Leittechnik Primär- und Sekundär- Ereignistabellen (Evetab) aus "KomTab.c" initalisieren
//---------------------------------------------------------------------------------------------
void Init_Evetab(void)
{
	char i;
	unsigned int adr;
	
	// 1.Aktion: Speicherbereich (64 Pages) löschen
	adr = PE_TAB_ADR;
	for(i = 0; i < 64; i++)
	{
		hwdog(1);
		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
		adr += 64;
	}

	// 2.Aktion: Evetab - Tabellen aus KomTab.c(Flash) in den RAM und EEPROM kopieren	 
	memcpy(prim_evtab, Pe_Tab, PETAB_TABL * sizeof(struct evs) );	// Flash --> RAM
	memcpy(seku_evtab, Se_Tab, SETAB_TABL * sizeof(struct evs) );	// Flash --> RAM

	move_evetab(BICWR);											// RAM   --> EEPROM

	// 3.Aktion: Initkennzeichen setzen
	adr = EVETAB_KZ;
	bicbus(EEPADR,	(char *)&adr,	EVETABKZ_ADR, 2, BICWR);
}
	
// ------------------------------------------------------------------------------------------------------
// UP: Bewegen der Evetab  zwischen RAM und EEPROM
// ------------------------------------------------------------------------------------------------------
// Neue Leittechnik (NLT):  Tabellenform für ein Ereignis (Primär- und Sekundär) 
// 6  Bytes im RAM: 		stat(1), dae(2), count(1), min_kommt(1), min_geht(1)
// 											nur die ersten 4 Bytes werden übertragen

//																									1.KOMMT-		   Letztes KOMMT-	  Letztes GEHT-Ereignis		
// 16 Bytes im EEPROM:	stat(1), dae(2), count(1), long_time(4),		long_time(4),		long_time(4),

// Zeitstempel für Ereignisse werden hier nicht! übertragen
void move_evetab(char rw)
{
	char i;
		
	// Tabelle der Primärereignisse
	for(i = 0; i < PETAB_TABL; i++)
	{	
		hwdog(1);
		bicbus(EEPADR,	(char	*)&prim_evtab[i],	PE_TAB_ADR + i*16, 4, rw);	// nur die ersten 4 Bytes werden übertragen
	}
	
	// Tabelle der Sekundärereignisse
	for(i = 0; i < SETAB_TABL; i++)
	{	
		hwdog(1);
		bicbus(EEPADR,	(char	*)&seku_evtab[i],	SE_TAB_ADR + i*16, 4, rw);	// nur die ersten 4 Bytes werden übertragen
	}

}

//---------------------------------------------------------------------------------------------
// UP: Initialisierung einer Alarmtabelle aus den Informationen der
//     Sekundär- Ereignistabellen (Evetab) aus "KomTab.c"
//---------------------------------------------------------------------------------------------
// Die Alarmtabelle wird vom Task Alarme benutzt
// Sie ist wird gebildet aus der Sekundär-Ereignistabelle, in der alle Alarme und Ereignisse aufgelistet sind
void Init_Alarmtab(void)
{
	char i, gnum, pnum, konv, attr, alclass, alfunc;
	char *pADR;
	UINT adr;
	
	gnum = pnum = konv = attr = alclass = alfunc = 0;
	
	// 1.Aktion: Speicherbereich löschen (12 Pages löschen)
	adr = AL_TAB_ADR;
	for(i = 0; i < 12; i++)
	{
		hwdog(1);
		bicbus(EEPADR,	(char *)&Null64,	adr, 64, BICWR);
		adr += 64;
	}
	
	// 2.Aktion:	Standardwerte initialisieren (Maske, Visloader, Ledfunc, Waitlimit),
	//						ermittelt aus den Zusatzinformationen (Alarm-Klasse und Alarm-Funktion) in der Parli
	for(i = 0; i < SETAB_TABL; i++)
	{
		hwdog(1);
		adr  = seku_evtab[i].dae;			// Datenelementnummer
		gnum = KomTab[adr].gnum;			// Gruppennummer
		pnum = KomTab[adr].pnum;			// Parameternummer
		
		pADR = get_parli_info(gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
		fill_alarmtab(i, alclass);
	}
	
	// 3.Aktion:	gefüllte Alarmtabelle in den EEPROM übertragen
	move_alarmtab(BICWR);
	
	// 4.Aktion: Initkennzeichen setzen
	adr = ALATAB_KZ;
	bicbus(EEPADR,	(char *)&adr,	ALATABKZ_ADR, 2, BICWR);

}

// UP zu Init_Alarmtab
void fill_alarmtab(char idx, char alclass_num)
{
	
	if( (alclass_num & GRUP) == GRUP)					// Gruppenkennzeichen
	{
		// Keine Anzeigeoptionen bei Sammelalarmen selbst
		alarmtab[idx].Maske				=  0;
		alarmtab[idx].Visloader		=  0;
		alarmtab[idx].Ledfunc			=  0;
		alarmtab[idx].Waitlimit		=  0;					// keine Aktivierung
	}
	else
	{	
		switch(alclass_num)
		{
			case 0:
				alarmtab[idx].Maske				=  0;
				alarmtab[idx].Visloader		=  0;
				alarmtab[idx].Ledfunc			=  0;
				alarmtab[idx].Waitlimit		=  0;					// keine Aktivierung
				break;

			case FUEHLER:
				alarmtab[idx].Maske				=  UMASK;	// UND-Operator 													
				alarmtab[idx].Visloader		=  2;			// Anzeigezeit in Sek. für die LCD-Anzeige				
				alarmtab[idx].Ledfunc			=  2;			// Rote Led 0=aus 1=ein 2=blinkend				
				alarmtab[idx].Waitlimit		=  5;			// Aktivierung nach 5 Sekunden
				break;

			case EREIGNIS:
				alarmtab[idx].Maske				=  1;
				alarmtab[idx].Visloader		=  0;			// keine Anzeige
				alarmtab[idx].Ledfunc			=  0;			// Rote Led aus
				alarmtab[idx].Waitlimit		=  1;
				break;
				
			case ANZEIGE:
				alarmtab[idx].Maske				=  1;
				alarmtab[idx].Visloader		=  2;			// Anzeigezeit in Sek. für die LCD-Anzeige	
				alarmtab[idx].Ledfunc			=  0;			// Rote Led aus
				alarmtab[idx].Waitlimit		=  1;
				break;
			
			case ANZEIGE_ROT:
				alarmtab[idx].Maske				=  1;
				alarmtab[idx].Visloader		=  5;			// Anzeigezeit in Sek. für die LCD-Anzeige	
				alarmtab[idx].Ledfunc			=  1;			// Rote Led ein
				alarmtab[idx].Waitlimit		=  1;
				break;

			default:															// Alle Alarme, die zu einer Sammelalarmgruppe gehören sowie alle Einzelalarme
				alarmtab[idx].Maske				=  1;
				alarmtab[idx].Visloader		=  5;			// 5 Sekunden Anzeige
				alarmtab[idx].Ledfunc			=  1;			// Led ein
				alarmtab[idx].Waitlimit		=  3;			// Aktivierung nach 3 Sekunden
				break;
				
		}
	}	
}
	
//---------------------------------------------------------------------------------------------
// UP: Bewegen der Alarmtab  zwischen RAM und EEPROM
// ------------------------------------------------------------------------------------------------------
// bewegt wird nur der Initialisierungsteil der Alarmtabelle (4 Byte: Maske, Visloader, Ledfunc, Waitlimit)
void move_alarmtab(char rw)
{
	char i;
	for(i = 0; i < SETAB_TABL; i++)
	{
		hwdog(1);
		bicbus(EEPADR,	(char	*)&alarmtab[i],	AL_TAB_ADR + i * 4, 4, rw);	// nur die ersten 4 Bytes werden übertragen
	}
	
}
	
//---------------------------------------------------------------------------------------------
// UP: Überprüfen des Versionsdatums	
// bei einer Veränderung wird Init_Leittab() aufgerufen (entspricht Kaltstart 223) 
//---------------------------------------------------------------------------------------------
void Check_Version(void)
{
	char buf[3] = {0};

	bicbus(EEPADR,	(char	*)&buf,	VERSION_ADR, 3, BICRD);		// gespeicherte Version lesen
	if( memcmp( buf, Version_PVers, 3) != 0)									// vergleichen mit aktueller Version
	{	// ungleich
		hwdog(1);
		Init_Leittab();																				// Kaltstart 223
		bicbus(EEPADR,	(char	*)&Version_PVers,	VERSION_ADR, 3, BICWR); // aktuelle Version speichern
	}
}			
				

//---------------------------------------------------------------------------------------------
// UP: Sendepuffer füllen für RD_ZPG
//---------------------------------------------------------------------------------------------
void Fill_TxBuf_ZPG(char *RsTxBuf, char offs, char i, char j, char anz, int messw, char stat) 
{					
		RsTxBuf[offs++] = anz;																			// Anzahl der Kreise im Projekt	
		RsTxBuf[offs++] = 36;																				// Modultyp
		RsTxBuf[offs++] = 0x1D;																			// Modulfunktion Low: Lüftung, Regelung, ZPG, Bedienung
		RsTxBuf[offs++] = 0x00;																			// Modulfunktion High
		RsTxBuf[offs++] = 0x1D;																			// Sichtbarkeit im R50 Low
		RsTxBuf[offs++] = 0x00;																			// Sichtbarkeit im R50 High
		RsTxBuf[offs++] = 0x00;																			// Raumstatus
		RsTxBuf[offs++] = (char)messw & 0xFF;												// Low    RAUM-TEMP  AKT
		RsTxBuf[offs++] = (char)(messw >> 8);												// High   RAUM-TEMP  AKT
		if(stat   == NICHTV)
			RsTxBuf[offs++] = NICHTV;
		else	
			RsTxBuf[offs++] = (char)(stat ? stat: 0x80);							// Status RAUM-TEMP  AKT (wenn 0, dann 0x80)
		RsTxBuf[offs++] =	0;																				// Messtemperatur Low
		RsTxBuf[offs++] =	0;																				// Messtemperatur High
		RsTxBuf[offs++] =	0;																				// Sollregeltemp Low
		RsTxBuf[offs++] =	0;																				// Sollregeltemp High
		RsTxBuf[offs++] =	0;																				// Sollwarmtemp Low
		RsTxBuf[offs++] =	0;																				// Sollwarmtemp High
		RsTxBuf[offs++] =	0;																				// maximal einstellbare Temperatur Low
		RsTxBuf[offs++] =	0;																				// maximal einstellbare Temperatur High
		RsTxBuf[offs++] =	0;																				// Spartemperatur Low  (bei Nichtnutzung)
		RsTxBuf[offs++] =	0;																				// Spartemperatur High (bei Nichtnutzung)
		RsTxBuf[offs++] =	0;																				// Increment der Temperaturverstellung
		RsTxBuf[offs++] =	0;																				// Temperaturnachforderung als signed char * 10
		RsTxBuf[offs++] =	0;																				// Ventilstellung (0=zu)
		RsTxBuf[offs++] =	0;																				// Aufheizphase (0=nein)
		RsTxBuf[offs++] =	0;																				// Fensterkontakt (0=zu)
		RsTxBuf[offs++] =	0;																				// Nachforderungszeit signed char in Minuten
		RsTxBuf[offs++] =	0;//lud[i].lueftung;											// Nutzungszustand (0=inaktiv)
		RsTxBuf[offs++] =	0;																				// aktuelle Vorheizzeit in Minuten/Kelvin
		RsTxBuf[offs++] =	0;																				// minimale Vorheizzeit in Minuten/Kelvin
		RsTxBuf[offs++] =	0;																				// maximale Vorheizzeit in Minuten/Kelvin
		RsTxBuf[offs++] =	0;																				// aktuelle Zeitprogramm 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// aktuelle Zeitprogramm 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// aktuelle Zeitprogramm 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Montag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Montag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Montag 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Dienstag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Dienstag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Dienstag 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Mittwoch 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Mittwoch 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Mittwoch 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Donnerstag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Donnerstag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Donnerstag 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Freitag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Freitag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Freitag 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Samstag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Samstag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Samstag 16-23 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Sonntag 0-7 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Sonntag 8-15 Uhr
		RsTxBuf[offs++] =	0;																				// Zeitprogramm Sonntag 16-23 Uhr
		RsTxBuf[offs++] =	0x00;																			// Zeitmarke Low  in Sekunden
		RsTxBuf[offs++] =	0x00;																			// Zeitmarke High in Sekunden
		*(int *) (RsTxBuf + offs) = Checksum(RsTxBuf+j, (unsigned char) (offs-j));
		offs = RsFrame(RsTxBuf,(char)(offs-j+2), RCOK, prot);
}


/*------------------------------------------------------------------------------*/
// Behandlung von Primär-Kommandos
// Aufruf erfolgt durch SlaveReply(..) in Reply.c    
/*------------------------------------------------------------------------------*/
void UserPrimReply(char *RsTxBuf, char	*RsRxBuf,	char prot)
{
	char	ucKommando,	ucLeng,	ucOffs, ucModul;

	ucKommando	=	RsRxBuf[0];
	
	switch(ucKommando)
	{
		// protokollunabhängige Kommandos
		// case ABC:
		//	 break;

		// nicht gefunden, evtl.
		// protokollrefenzierte Kommandos
		default:
			if(prot	== PROT485)
			{	
				switch(ucKommando)
				{
					//case DEF:
					//	break;
					
					// nicht gefunden
					default:
						RsFrame(RsTxBuf, 0,	ERUK,	prot);		// unbekanntes Kommando		
						break;
				}
			}
			else 
			{
				switch(ucKommando)
				{
					//case XYZ:
					//	break;
					
					// nicht gefunden	
					default:
						RsFrame(RsTxBuf, 0,	TYPE_ERR,	prot); //	unbekanntes	Kommando	 	
						break;
				} // switch(ucKommando)
			} // ELSE if(prot	== PROT485)
			break;
	} // switch(ucKommando)
}



