/*            Strukturen und Funktionsprototypen im User-Bereich     */
/*            ustruct.h 				                                     */
#ifndef USTRUCT_H_INCLUDED
#define USTRUCT_H_INCLUDED

#include "projdef.h"
#include "userdef.h"

// Pumpenbus

typedef struct {
	char	Funktion; 			// deaktiviert (0), remote (1), local (2)
	char	Name[16];				// Beispiele: "PUMPE HK1       ", "ZIRK.-PUMPE     "
	char	Adresse;				// frei wählbar, Standard = Nummer
	char	Betrieb;				// Betriebsart bei Automatikbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
	char	Regelart;				// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
	UINT	Sollwert;				// Standardwert, wenn Pumpe dauernd und konstant läuft, sonst anwendungsspezifisch überschreiben [%] / 10	
												// ( z.B. in Steuer.c für Heizungspumpe )			
	char	RMverz;					// Verzögerung Rückmeldung [min]
	char	Hand;						// Hand/Automatik, Hand = 1
	char	BetriebHand;		// Betriebsart bei Handbetrieb, Stop (0), Ein (1), Minimal-Kurve (2), Maximal-Kurve (3)
	UINT	SollHand;				// [%] / 10
	char	RegelartHand;		// Konstantdruck (0), Proportionaldruck (1), Konstantfrequenz (2), Automatik (3)
	char	AlarmQuit;
	char	HoldingReg300;	// Manual (5), siehe Modbus_RS485_spec_V_1_09_en.pdf Abschnitt 5.1.8 Table 5.4
} PuBusStandard;
#define BUS_PU_LENG sizeof(PuBusStandard)

typedef struct {
	char	puAlarm;				// Zustand der roten LED der Pumpe
	char	busSm;
	char	betriebSm;	
	UINT	betriebRmCtr;

// Änderung neue Genibus-Implementierung
	unsigned char uc_betriebsart;							// Aktuelle Betriebsart <- aus AUTO bzw HAND ermittelt in PumpeParametrieren
	unsigned char uc_regelart;								// Aktuelle Regelart
	unsigned int  ui_sollwert;								// Aktueller Sollwert

// Wilo
	char betriebsArtSoll[16];	// ASCII	
	char betriebsArtIst[16];	// ASCII	
	char regelArtSoll[16];		// ASCII	
	char regelArtIst[16];			// ASCII	

} PuBusDynam;

// MBUS

typedef struct {
	char	Freigabe;
	char	Folge;					// Folge vordefinierter Telegramme bei jeder Abfrage
	char	Typ;						// Zähler-Typ
	char	Adr;						// MBus-Adresse
} MbusStandard;
#define MBUSLENG sizeof(MbusStandard)

typedef struct {
	char wrm;							// Wärmezähler
	char wrm_h2;					// Wärmezähler
	char was;							// Wasserzählerzähler
	char elt;							// Elektrozähler
	char klm;							// Klimazähler (Wärme/Kälte)
	char vol;
} Sichtbarkeit;


typedef struct {
	char	errors;
	ULONG	wmeng;
	ULONG	volume;
	ULONG	elektro;
	int		tVorlauf;
	int		tRuecklauf;
	int		tDifferenz;
	ULONG	volStrom;
	long	leistung;
	ULONG	kaelte;
	struct {
			char wmeng 	: 4;
			char volume : 4;
		} komma;
	ULONG long zwertAkt;	// letzter erfasster Zählwert
	char	kommaLT;
	char	dim_idx;
} MbusDynam;

// Struktur für Skalierung von Analogausgängen
typedef struct {
	int	NullP;						// bei   0%: in Volt [V] * 100  
	int HundP;						// bei 100%: in Volt [V] * 100
}aask;	

// Struktur für externes Erweiterungsmodul
typedef struct iob{
	char iodev;						// Gerätekennzeichen								(R37, R38, ...)
	char ionum;						// Gerätenummer											(1..4)
	char iobel;						// Nummer des IO-Belegungssatzes		(1..255)
}ioblock;

typedef struct {
	char min;
	char std;
} uhrzeit;

/*--------------------------------------------------------------------*/
// Modbus Allgemein - joja
/*--------------------------------------------------------------------*/
#if MODBUS_UNI > 0
	#include "Modbus/modbusstruct.h"
#endif	
//---------------------------------------------------------------------

//#####ulsch: skalierbare Analogeingänge
typedef struct {
	int		Skal0;										// Skalierung des Spannungs-Messwerts  0 V	
	int		Skal10;										// Skalierung des Spannungs-Messwerts  10,00 V		
	UINT	ZkFilt;										// Glaettungszeitkonstante der AE [s]	( Sprungantwort: 66 % nach ZK )
	int		UGW;											// Grenzwert unten (Dimension wie Skalierung), Null: kein Grenzwert !
	int		OGW;											// Grenzwert oben (Dimension wie Skalierung), Null: kein Grenzwert !
	int		Hyst;											// Hysterese der Grenzwerte (Dimension wie Skalierung)
} sAnaInpPara;

typedef struct {
	UINT	mwFilt;										// geglaetteter Messwert							[%] * 10
	float	fl_mwFilt;								// geglaetteter Messwert		
	mwsp	mwSkal;										// skalierter und geglaetteter Messwert
	char ugwSM;											// SM UGW
	char ogwSM;											// SM OGW
} sAnaInp;

typedef struct {
	int		Skal0;										// Skalierung LEISTUNG 0 %  [V]	
	int		Skal10;										// Skalierung des Messwerts  10V [%]		
	UINT	ZkFilt;										// Glaettungszeitkonstante der AE [s]	( Sprungantwort: 66 % nach ZK )
} sPowInpPara;

typedef struct {
	UINT	mwFilt;										// geglaetteter Messwert							[%] * 10
	float	fl_mwFilt;								// geglaetteter Messwert		
	mwsp	mwSkal;										// skalierter und geglaetteter Messwert
} sPowInp;

typedef struct datum {
	char jahr;
	char monat;
	char tag;
} datum;

struct sKes {
	char ke1 : 1;
	char ke2 : 1;
	char ke3 : 1;
	char ke4 : 1;
	char ke5 : 1;
	char ke6 : 1;
	char ke7 : 1;
	char ke8 : 1;
};

/*--------------------------------------------------------------------*/
/* Struktur von RIEcon36 - Heizungsanlagen														*/
/*--------------------------------------------------------------------*/
typedef struct anl{			
	// Typ und Version
	char Typ[15+1];				// 15 ASCII-Zeichen und \0
	char Vers[3];					// Jahr Monat Tag (hex)
	// Kategoriebezeichnung für variable Projekte
	char KatVar[15+1];		// 15 ASCII-Zeichen und \0
	
	// Steuercode für Heizkreise 
	char hkcode[HKMAX];		// Nummer des Kaltstartparametersatzes für jeden Heizkreis			
	char abscode[HKMAX];	// Nummer des Absenkprofils für jeden Heizkreis	

	// Steuercode für Wärmepumpe 
	char wpcode[WPMAX];		// Nummer des Kaltstartparametersatzes für jeden Warmwasserkreis	
	
	// Steuercode für Warmwasserkreise 
	char wwcode[WWMAX];		// Nummer des Kaltstartparametersatzes für jeden Warmwasserkreis	

	// Steuercode für Kesselkreise 
	char kecode[KEMAX];		// Nummer des Kaltstartparametersatzes für jeden Kesselkreis

	// Steuercode für Fernwärmevorregelkreis (Netztrennumg)
	char ntcode[NTMAX];		// Nummern der Kaltstartparametersätze für jeden Netztr.kreis [NT_PROFILE]

	// Steuercode für Solarregelung
	char socode[SOLMAX];	// Nummern der Kaltstartparametersätze für jeden Solar-Kreis [SO_PROFILE]

	// Steuercode für MBUS
	char mbcode[MBUSANZ > 0 ? MBUSANZ : 1 ];	// Nummern der Kaltstartparametersätze für jeden MBUS-Zähler [MB_PROFILE]

	// Steuercode für SteuerUni
	char unicode[1];			// Nummern der Kaltstartparametersätze für jede UNI-Steuerung [UNI_PROFILE]

	// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
	// Aus den Namen werden Steuercodes generiert
	unsigned int in1;			// 				Eingang IN1		Pt1000 / ----- / -------	
	unsigned int in2;			// 				Eingang IN2		Pt1000 / ----- / -------	
	unsigned int in3;			// Block  Eingang IN3		Pt1000 / 0-10V / Digital
	unsigned int in4;			// Block  Eingang IN4		Pt1000 / 0-10V / Digital
	unsigned int in5;			// Block  Eingang IN5		Pt1000 / 0-10V / Digital
	unsigned int in6;			// 				Eingang IN6		Pt1000 / 0-10V / Digital
	unsigned int in7;			// 				Eingang IN7		Pt1000 / 0-10V / Digital
	unsigned int in8;			// 				Eingang IN8		Pt1000 / 0-10V / Digital
	unsigned int in9;			// 				Eingang IN9		------ / 0-10V / Digital
	unsigned int in10;		// 				Eingang IN10	------ / 0-10V / Digital

	unsigned int aa1;			// Analog-Ausgang  AA1
	unsigned int aa2;			// Analog-Ausgang  AA2

	unsigned int da1;			// Digital-Ausgang DA1
	unsigned int da2;			// Digital-Ausgang DA2
	unsigned int da3;			// Digital-Ausgang DA3
	unsigned int da4;			// Digital-Ausgang DA4
	unsigned int da5;			// Digital-Ausgang DA5
	unsigned int da6;			// Digital-Ausgang DA6
	
	// Externe Erweiterungen mit R37, R38...
	ioblock	 iobl[4];			// maximal 4 Erweiterungsmodule
	
	// NAMUR-Kennzeichnung für Digitale Eingänge R36
	char namur;						// Bit=1...NAMUR-Eingang,	BIT0...7 = IN3...IN10

}Anlage;
	

/*--------------------------------------------------------------------*/
/* 				Struktur R37  IN/OUT-Modul 																	*/
/*--------------------------------------------------------------------*/
	// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
	// Aus den Namen werden Steuercodes generiert
typedef struct io37{			
	unsigned int xde1;		// Digital-Eingang DE1 
	unsigned int xde2;		// Digital-Eingang DE1 
	unsigned int xde3;		// Digital-Eingang DE1 
	unsigned int xde4;		// Digital-Eingang DE1 
	unsigned int xde5;		// Digital-Eingang DE1 
	unsigned int xde6;		// Digital-Eingang DE1
	
	unsigned int xaa1;		// Analog-Ausgang  AA1
	unsigned int xaa2;		// Analog-Ausgang  AA2
	unsigned int xaa3;		// Analog-Ausgang  AA3
	unsigned int xaa4;		// Analog-Ausgang  AA4

	unsigned int xda1;		// Digital-Ausgang DA1
	unsigned int xda2;		// Digital-Ausgang DA2
	unsigned int xda3;		// Digital-Ausgang DA3
}Anl37;
	
// Struktur des RAM-Bereiches für ein R37  IN/OUT-Modul
typedef struct ra37{
	// Datenübertragung
	char					bus;				// Busverbindung: 0=XBUS 1=GBUS
	char					adr;				// DeviceAdresse bzw. Moduladresse
	char					errcnt;			// Fehlerzähler Datenübertragung
	char					error;			// Anzeige des Datenübertragungsfehlers
	// Skalierungen
	aask					AASkal[4];	// Grundwerte (EEPROM gestützt, Voreinstellung in UserReply.c bei Kaltstart 66)	
		
	// Datue senden an R37
	char					Digout;			// 1 Digital-Ausgabebyte
	char					anout[4];		// 4 Analog-Ausgabebyte  0-255 Digit
	char					znum;				// aktueller Zähler
	char					zaufnum;		// Auftragsnummer für Zählerinformationen lesen
	
	// Datue empfangen vom R37
	char					Diginp;			// Digitaleingabe
	char 					sw_stat[3];	// Status für jeden Schalter 
	zaehlsp				zaehler[6];	// Zaehlerspeicher, wird stetig gefüllt
	
	// Arbeitsspeicher des Auftraggebers
	dinpsp				di_mewe[6];	// Digitaleingabe, extrahiert aus Diginp
	aasp					Aout[4];		// Analogausgabe:  0 - 100,0% vom Auftraggeber
	unsigned int	AoutSk[4];	// aktuelle skalierte Ausgabewerte; Umrechnung Aout[i]->awert nach AoutSk[i] in Output.c
	doutsp 				Dout[3];		// Digitalausgabe: (char)Ausgabe + (char)Funktion
	char					wpause[3];	// Wechselpause bei verriegelten Ausgängen
	char					dadat;			// vom R50 R3X_DADAT-Kommando
	char					aadat[4];		// vom R50 R3X_AADAT-Kommando
	char 		sw_stat_info[16];	// Info über Handschalter in ASCCI
	
}Ram37;	


/*--------------------------------------------------------------------*/
/* 				Struktur R38  PT1000-Modul 																	*/
/*--------------------------------------------------------------------*/
	// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
	// Aus den Namen werden Steuercodes generiert
typedef struct io38{			
	unsigned int xae1;		// Pt1000-Eingang AE1 
	unsigned int xae2;		// Pt1000-Eingang AE2 
	unsigned int xae3;		// Pt1000-Eingang AE3 
	unsigned int xae4;		// Pt1000-Eingang AE4 
	unsigned int xae5;		// Pt1000-Eingang AE5 
	unsigned int xae6;		// Pt1000-Eingang AE6 
	unsigned int xae7;		// Pt1000-Eingang AE7 
	unsigned int xae8;		// Pt1000-Eingang AE8 
	unsigned int xae9;		// Pt1000-Eingang AE9 
	unsigned int xae10;		// Pt1000-Eingang AE10 
	unsigned int xae11;		// Pt1000-Eingang AE11 
	unsigned int xae12;		// Pt1000-Eingang AE12 
	unsigned int xae13;		// Pt1000-Eingang AE13 
	unsigned int xae14;		// Pt1000-Eingang AE14 
}Anl38;

// Struktur des RAM-Bereiches für ein R38  PT1000-Modul
typedef struct ra38{
	// Datenübertragung
	char					bus;				// Busverbindung: 0=XBUS 1=GBUS
	char					adr;				// DeviceAdresse bzw. Moduladresse
	char					errcnt;			// Fehlerzähler Datenübertragung
	char					error;			// Anzeige des Datenübertragungsfehlers
	
	// Lifetest
	unsigned int  life;				// 14 Bit, Bit0=1 -> Fühler IN 1 aktiviert usw.
	
	// Datue senden an R38
	unsigned int inpUsed;
	
	// Datue empfangen vom R38
	mwsp pt_mewe[14];					// (int)Messwert[0,1°C]	+	(char)Status 
	
}Ram38;

/*--------------------------------------------------------------------*/
/* 				Struktur R39  INP-Modul		 																	*/
/*--------------------------------------------------------------------*/
	// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
	// Aus den Namen werden Steuercodes generiert
typedef struct io39{			
	unsigned int xde1;		// Eingang DE1 
	unsigned int xde2;		// Eingang DE2 
	unsigned int xde3;		// Eingang DE3 
	unsigned int xde4;		// Eingang DE4 
	unsigned int xde5;		// Eingang DE5 
	unsigned int xde6;		// Eingang DE6 
	unsigned int xde7;		// Eingang DE7 
	unsigned int xde8;		// Eingang DE8 
	unsigned int xde9;		// Eingang DE9 
	unsigned int xde10;		// Eingang DE10 
	unsigned int xde11;		// Eingang DE11 
	unsigned int xde12;		// Eingang DE12
	// Signalart: 0=Schließer 1=Öffner  Bit0=DE1
	unsigned int art;

}Anl39;

// Struktur des RAM-Bereiches für ein R39  INP-Modul
typedef struct ra39{
	// Datenübertragung
	char					bus;				// Busverbindung: 0=XBUS 1=GBUS
	char					adr;				// DeviceAdresse bzw. Moduladresse
	char					errcnt;			// Fehlerzähler Datenübertragung
	char					error;			// Anzeige des Datenübertragungsfehlers
	
	// Datue senden an R39
	unsigned int Digart;			// Maske für Kontaktart: 0=Schließer 1=Öffner
	
	// Datue empfangen vom R39
	unsigned int Diginp2;			// Bit0=DE1....Bit11=DE12
	char				 Derror;			// Fehler Kontaktspannung UK24
	
	// Arbeitsspeicher des Auftraggebers
	dinpsp			 di_mewe[12];	// Digitaleingabe, extrahiert aus Diginp2

}Ram39;

/*--------------------------------------------------------------------*/
/* 				Struktur R33  OUT-Modul		 																	*/
/*--------------------------------------------------------------------*/
	// Namen für Prozess Ein- und Ausgänge (Namensvereinbarung in "userdef.h")
	// Aus den Namen werden Steuercodes generiert
typedef struct io33{			
	unsigned int xda1;		// Digital-Ausgang DA1
	unsigned int xda2;		// Digital-Ausgang DA2
	unsigned int xda3;		// Digital-Ausgang DA3
	unsigned int xda4;		// Digital-Ausgang DA4
	unsigned int xda5;		// Digital-Ausgang DA5
	unsigned int xda6;		// Digital-Ausgang DA6
}Anl33;
	
// Struktur des RAM-Bereiches für ein R33  OUT-Modul
typedef struct ra33{
	// Datenübertragung
	char					bus;				// Busverbindung: 0=XBUS 1=GBUS
	char					adr;				// DeviceAdresse bzw. Moduladresse
	char					errcnt;			// Fehlerzähler Datenübertragung
	char					error;			// Anzeige des Datenübertragungsfehlers
		
	// Datue senden an R33
	char					Digout;			// 1 Digital-Ausgabebyte
	
	// Datue empfangen vom R33
	char 					sw_stat[6];	// Status für jeden Schalter 
	
	// Arbeitsspeicher des Auftraggebers
	doutsp 				Dout[6];		// Digitalausgabe: (char)Ausgabe + (char)Funktion
	char					wpause[6];	// Wechselpause bei verriegelten Ausgängen
	char					dadat;			// vom R50 R3X_DADAT-Kommando
	char 		sw_stat_info[16];	// Info über Handschalter in ASCCI
}Ram33;	

/*--------------------------------------------------------------------*/
/*				Regelparameter 																							*/
/*--------------------------------------------------------------------*/
	
/*--------------------------------------------------------------------*/

/* Struktur der Regelparameter eines Heizkreises										*/
// Teil1: Statische Parameter (mit Standdardwerten)
typedef struct rs{
	int		Ts;								// Tastzeit (>= 1 s)						[s] * 10
	UINT	Tn;								// Nachstellzeit								[s] * 10
	int		Kp;								// P-Verstärkung							[%/K] * 100
	int		Kd;								// D-Verstärkung							[%/K] * 100
	int		Kpk;							// P-Verstärkung Kaskade			[%/K] * 100
	int		Kpr;							// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	int		Anst;							// Steigung									 		[1] * 100	
	int		Tvpa;							// Niveau											 [°C] * 10		
	int		Tvma;							// max. Vorlauftemp.					 [°C] * 10		
	int		Tvmi;							// min. Vorlauftemp.					 [°C] * 10		
	int		Trma;							// max. Rücklauftemp.					 [°C] * 10		
	int		Taha;							// Temp. am Heizkurvenanfang	 [°C] * 10	
	int		Tiso;							// Raumsolltemp. für Kaskade	 [°C] * 10		
	int		Timi;							// Mindest-Raumtemp.					 [°C] * 10	
	int		Tsolu;						// Start-Sollwert Heizung			 [°C] * 10	
	int		Beabs;						// Bedarfs-Absenk.keine Nutzung [K] * 10	
	int		Fzk;							// Filterzk. f. tsol						[s] * 10
	char	Hub;							// Ventilhub									 [mm]
	char	Vst;							// Stellgeschwindigkeit		 [mm/min]
	char	Haut;							// HAND/AUTO-Umschaltung
	char	Puea;							// Hand: Pumpe	ein/aus
	char	Pu2ea;						// Hand: bei Doppelpumpe Pumpe 2	ein/aus	
	int		Vstell;						// Hand: Ventil stellen 0-10V Ventil	[%] * 10
	char	Puna;							// Pumpennachlaufzeit										[min]	
	char	Swhk;							// Schalter Sommer/Winter-Automatik 		ja/nein
	char	Wwvh[2];					// Schalter WW-Vorrang 									ja/nein
	char	Sauf;							// Schalter Schnellaufheizung 					ja/nein
	char	Psbegr;						// Schalter Primär/Sekundär Rückl.begr.	Prim=1 Sek=0
	int	  Kpww[2];					// P-Verstärkung für gleitenden Warmwasservorrang [%/K] * 100 (in Wwsteu behandelt)
	char	TabwMax;					// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	char	TabwZeit;					// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]
	char	FerienAutom;			// Schalter
	UINT	FerienAbsenk;			// Absenkwert
	UINT	SchalterAbsenk;		// Anlagenschalter-Absenk. keine Nutzung [K] * 10	
	UINT	Y_rel_min;				// Minimale Stellausgabe an 0-10V Ventil	[%] * 10
	UINT	Y_rel_beg;				// Öffnungsbeginn des 0-10V Ventils				[%] * 10

	int		TaFrostgrenze;		// Ta-FrostGrenze bei Absenkung				 	[°C] * 10		
	int		TaHeizgrenze;			// Ta-HeizGrenze beim Heizbetrieb			 	[°C] * 10		
	int		AbschZeitKonst;		// Zur Berechnung der Abschaltdauer in der Absenkzeit		[h/K] * 10
	char 	SoWiExtra;				// Schalter Auswahl Sommer/Winter Extrawerte ja/nein 
	int		Tae;							// Temp. Winter ein						 					[°C] * 10	
	int		Taa;							// Temp. Sommer aus						 					[°C] * 10	

	char	MessBeginn;				// Beginn der Adaption, Stunde
	char	AdaptWert;				// [K] * 10
	int		EiMaxAdapt;				// max. Regelabweichung, bei der Adaptionswert berechnet wird [K] * 10

	char 	FuehrPu;					// Führunspumpe
	UINT 	PuWechseln;				// laufzeitabh. Umschalten der Führungspumpe  [h]
	char 	FuehrPuFest;			// Führungspumpe fest vorgegeben, wenn ungestoert	(1,2); 3=beide Pu laufen gleichzeitig, 0=laufzeitabh.
	char	KaskMode;					// Modus der Raum-Kaskade bei gleichzeitiger Adaption : 0 -> nur anhebend und bei Unterversorgung, 1 -> immer
	int		TvpAnh;						// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		

	// Estrich-Trocknung
	datum	StartAufheiz;
	char	EstrichTemp[ESTR_STUFEN];	
	int		TimiHyst;					// Hysterese Raum-Mindesttemp. [K] * 10
	UINT	XpLeistBegr;			// [K/% * 100]
	int		KaskBegr;					// Begrenzung des Raum-Sollwerts für Kaskade	[°C] * 10
//#####090119													
	char	PuMin;						// Hand: Pumpe auf Mindestdrehzahl 													
													// 119 Byte Parameter !!!
// Pumpenbus
	UINT	BusPuSollwert;					// 0...100,0 %
	UINT	BusPuSollwertAbsenk;		// 0...100,0 %

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
	char	AbschRaumanf;		// Funktion freigegeben ?
	char	VentilAuswert;	// Ventilstellung berücksichtigen ? (sonst nur Raumtemperatur)
	char	VentOffenMax;		// HK abschalten, wenn offene Ventile (%) kleiner VentOffenMax
	int 	VorlaufMaxSommer;	// wenn VL > VorlaufMaxSommer, wird ucHeat = 1 trotz Sommer (R50 öffnet dann nicht die Heizungs-Ventile)

	char	VorrangZeitabsenkung;
	char 	PuBmVerz;				// [min]

// Freitags-Ventilöffnung	
	char VentiloeffngTag;				// Wochentag
	uhrzeit VentiloeffngBeg;		// Beginn
	uhrzeit VentiloeffngEnd;		// Ende
	int VentiloeffngAbs;				// Absenkwert (>= 50,0 K führt zum Abschalten)
	int GrenztempAbschalten;		// untere Grenze der Außentemperatur
	
	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	int		Wup;									// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	
}HkStandard;
#define HKSLENG sizeof(struct rs)

/* Struktur der Regelparameter eines Heizkreises										*/
// Teil2: Dynamische Parameter	
typedef struct rd{
	int		zts;					// Zaehler fuer Tastzeit
	char	regstart;			// Reglerstart nach Reset
	int		ei;						// Regelabweichung							[K] * 10
	float	fl_ei1;				// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2;				// Regelabweichung zum Tastzeitpunkt i-2
	int		tsol;					// Vorlaufsolltemp. , nach der geregelt wird
	float fl_tsol;			//		"								(für Filterberechnung)
	int		tvsb;					// berechnete Vorlaufsolltemp. total
	int		tvsb_hkl;			// berechnete Vorlaufsolltemp. nach Heizkennlinie (für debug)							
	int		stellzeit;		// berechnete Zeit für Stellausgabe
	int		stellsum;			// Summenzähler für Stellausgabe
	int 	fahren;				// Fahrzeit Stellausgabe in ganzen Sekunden
	float fl_y_rel;			// PID-Stellgröße		
	int   si_y_rel;			// PID-Stellgröße für debug [%] * 10
	int		y_rel;				// Stellausgabe an 0-10V Ventil	[%] * 10
	int		dy_rel;				// für debug
	UINT	zpu;					// Zähler für Pumpenachlaufzeit
	char	vzu;					// Hand Ventil zufahren			[s]
	char	vauf;					// Hand Ventil auffahren		[s]
	char	sowi;					// Ermittelter Zustand: 1 = Sommerbetrieb
	char	somme;				// Statusanzeige Sommerbetrieb
	char	heiz_grenz;		// Merker für Außentemperatur-Heizgrenze
	char	vorra;				// Statusanzeige Warmwasservorrang
	char	rlbegr;				// Statusanzeige Rücklaufbegrenzung
	char	vorra_bed;		// Zustand  1=Bedarfsabsenkung hat Vorrang vor Zeitabsenkung	
	char	bedarf;				// Statusanzeige Heizungsbedarfsanforderung (vom RIEcon50) 1=JA 2=NEIN 0=UNBEKANNT
	char	vorheiz_bed;	// Heizungsbedarf nur durch Räume in Vorheizung
	int		tisobed;			// RaumSolltemp. Bedarf 			 (vom RIEcon50)
	int		tibed;				// RaumIsttemp.  Bedarf 			 (vom RIEcon50)
	char	ticopy;				// kopiert TI nach tibed, wenn tibed = 0 (Dummy-Raum)
	char 	CountDown;		// Kommandoüberwachung	
	UINT  abwCtr;				// (Sekunden)-Counter der Regelabweichung
	int		kaskaKorr;		// aktueller temperaturabh. Kaskade-Korrekturwert zur berechn.Vorlaufsolltemp. nach Heizkennlinie tvsb_hkl	
	int		adaptKorr;		// aktueller temperaturabh. Adaptions-Korrekturwert zur berechn.Vorlaufsolltemp. nach Heizkennlinie tvsb_hkl	
	int	  wwvorKorr;		// Absenkwert für gleitenden Warmwasser-Vorrang
	
	UINT puLz;					// Pumpen-Laufzeit [h]	
	UINT pudLz;					// Laufzeit [h] 2.Pumpe	bei Doppelpumpe
	char pu1min;				// Minutenzähler Laufzeit
	char pu2min;
	
	char raumname[16];

	// Ferienabsenkung
	UINT	tvabFerien;			// Absenkwert bei Ferien, immer 0 wenn keine Ferien	
	char	absenFerien;		// Zustand  Absenkphase 				
	char	abschFerien;		// Zustand 	Abschaltphase
	char	stuetzFerien;		// Zustand 	Stützbetrieb 
	char	frostFerien;		// Zustand	Frostschutzbetrieb 
	// Schalterabsenkung
	UINT	tvabSchalter;		// Absenkwert bei Schalter, immer 0 wenn keine Schalter	
	char	absenSchalter;	// Zustand  Absenkphase 				
	char	abschSchalter;	// Zustand 	Abschaltphase
	char	stuetzSchalter;	// Zustand 	Stützbetrieb 
	char	frostSchalter;	// Zustand	Frostschutzbetrieb 

	// Gemeinsame Anzeige von Parametern aus verschiedenen Quellen:
	// Schalterabsenkung oder Bedarfsabsenkung oder Ferienabsenkung oder Zeitabsenkung 
	char	regie;					// Absenk-Regie aktuell
	char	regie_txt[16];	// Textanzeige Absenk-Regie
	char	absen;					// Zustand  Absenkphase
	char	aufhe;					// Zustand  Aufheizphase
	char	absch;					// Zustand	Abschaltphase
	char	hunt;						// Zustand  Heizunterbrechung
	char	stuetz;					// Zustand 	Stützbetrieb
	char	frost;					// Zustand	Frostschutzbetrieb
	
	// Softalarme
	char	abwAlarm;			// Regelabweichung ueberschreitet fuer eine definierte Zeit einen vorgegebenen Betrag
	char  puBm;					// Softalarm für 'Betriebsmeldung Heizungspumpe' kommt nicht

	// Estrich-Trocknung
	char	estrichProg;
	int		estrichTemp;
	char	estrichTag;	

// Heizkreis abschalten mangels Bedarf 
// Bedingungen: Funktion freigegeben, Raum ext Ist > Soll sowie (wenn parametriert) offene Ventile <= VentOffenMax
	UINT	ventOffen;		// Anzahl offener Ventile (von R50)
	UINT	ventGesamt;		// Gesamt-Anzahl Ventile (von R50)
	char 	ventOffenBer;	// Prozentsatz offener Ventile (gestörte Räume nicht berücksichtigt)
	char 	raumAbsch;		// Ergebnis der Berechnungen

	char	busPuSm;			// Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char	busPuEin;

	char	ucCool;					// Meldung Kühlen
	char 	ucHeat;					// Meldung Heizen

	UINT	leistBegrAbsenk;	// [K] * 10	
	UINT	puBmVerz;				// [min/2]
	
	char	ventiloeffng;
}HkDynam;	
 

/* Struktur der Absenktabelle für jeden Wochentag	eines Heizkreises	*/
typedef struct abs_wo{
	char begin;				// Beginn Min
	char begin_h;			// Beginn Std
	char dauer;				// Dauer  [h]
	int  abwert;			// Absenktemperaturwert [K] * 10
}absenktab;
#define ABSTABLENG	sizeof(struct abs_wo)


/* Struktur des Absenk-Arbeitsbereiches für einen Heizkreis	
	 wird im UserNoinit-Ram verwendet mit 2 Instanzen: Zeitabsenkung und Bedarfsabsenkung
*/
typedef struct abs_ti{ 
	UINT	ramex;			// RAM-Überwachung
	UINT	tvab;				// Absenkwert
	UINT	zasd;				// Zähler		Absenkzeit
	UINT	zahd;				// Zähler 	Aufheizzeit
	UINT	zAbsch;			// Zähler		Abschaltzeit
	char	absen;			// Zustand  Absenkphase
	char	aufhe;			// Zustand  Aufheizphase
	char	absch;			// Zustand	Abschaltphase
	char	hunt;				// Zustand  Heizunterbrechung
	char	stuetz;			// Zustand 	Stützbetrieb
	char	frost;			// Zustand	Frostschutzbetrieb
}absenkram;
#define ABSRAMLENG	sizeof(struct abs_ti)
	
	/* Struktur der Regelparameter einer Wärmepumpe										*/
// Teil1: Statische Parameter (mit Standdardwerten)
typedef struct wps{
	char	Para_Manu_Sollwert; 						// Manuellen Sollwert aktivieren
	UINT	T_manu_Sollwert; 								// Manuell vorgegebener Sollwert in [°C] *10
	UINT	T_Sollwert_Offset; 							// Sollwertoffset in [K] *10
	UINT	T_Ersatz_Sollwert; 							// Ersatz-Sollwert in [°C] *10
	char	WPU_Freigabe_Haut;							// Manueller Betrieb aktivieren zur Steuerung der WPU-Freigabe
	char	WPU_Freigabe_Hand_stellen;			// Steuerung der WPU-Freigabe im manuellen Betrieb
	 int	intPa_Quellentemperaturminimum;	// Minimale Quellentemperatur in [°C] *10
	 int	intPa_Speicherminimum;					// Minimale Speichertemperatur in [°C] *10
	 int	intPa_T_Speicherladung_on; 			// Speicherladung aktivieren: delat T in [K] *10
	 int	intPa_T_Speicherladung_off; 		// Speicherladung deaktivieren: delat T in [K] *10
	 char chPa_Mindestlaufzeit_min;				// Mindestlaufzeit der WPu in [min]
	 char chPa_Sperrzeit_min;							// Sperrzeitzeit der WPu in [min]
}WpStandard;
#define WPSLENG sizeof(struct wps)

// Teil2: Dynamische Parameter 
typedef struct wpd{
	char	Status_SW_DM_aktiv; 										// Sollwert von DM ist aktiviert
	char	Status_SW_AE_aktiv; 										// Sollwert von AE ist aktiviert
	char	Status_Quellenschutz;								 		// Betriebszustand Quellenschutz: Quellenschutz wird aktiviert, wenn die Quellentemperatur den Grenzwert unterschreitet
	char	Status_Frostschutz;								 			// Betriebszustand Frostschutz: Frostschutz wird aktiviert, wenn die Speichertemperatur den Grenzwert unterschreitet
	 int	intT_Quellentemperatur_aktiv;						// Verwendete Quellentemperatur für den Quellenschutz
	 int	intT_Speichertemperatur_aktiv;					// Verwendete Speichertemperatur für den Quellenschutz
	char	Status_Speicherladung;								 	// Betriebszustand Speicherladung: Speicherladeprinzip
	 int	intT_Ladebginn;													// Ladebeginn in [°C] *10
	 int	intT_Ladeende;													// Ladeende in [°C] *10
	char	Status_WPU_Freigabe_oZeit;							// Status der WPU-Freigabe ohne Zeitlimitierung
	 int	Sollwert_oZeit;													// Sollwert der WPU ohne Zeitlimitierung
	 int	Mindestlaufzeit_Cnt;										// Counter in Sekunden für die Mindestlaufzeit der Wärmepumpe [s]
	 int	Sperrzeit_Cnt;													// Counter in Sekunden für die Sperrzeit der Wärmepumpe [s]
	 int	Eingehender_Sollwert;										// Eingehender Sollwert: Wird in Steuer.c ausgewertet und an SteuerWPU übergeben
	 int	WPU_Starts_BM;													// Anzahl der Starts der WPU bzgl BM
	 int	WPU_Starts_Freigabe;										// Anzahl der Starts der WPU bzgl feiagbe
	 int	WPU_Freigabe_Laufzeit_sec;							// WPU-Laufzeit bzgl der Freigabe in [s]
	 int	WPU_BM_Laufzeit_sec;										// WPU-Laufzeit bzgl der BM in [s]
	 int	WPU_Freigabe_Laufzeit_h;								// WPU-Laufzeit bzgl der Freigabe in [h]
	 int	WPU_BM_Laufzeit_h;											// WPU-Laufzeit bzgl der BM in [h]
}WpDynam;



//-------------------------------------------------------------
/* Struktur der Regelparameter eines Warmwasserkreises				*/
// Teil1: Statische Parameter (mit Standdardwerten)
typedef struct ws{
	int	 Twwsol;		// WW-Solltemp
	int  Twwlad;		// Speicher-Ladetemperatur
	int  TwwladLegio;		// Speicher-Ladetemperatur bei therm.Desinf.
	int  Twwlbeg;		// Speicherlade-Beginn-Temperatur
	int  Twwlend;		// Speicherlade-Ende-Temperatur
	char Puwlna;		// Ladepumpe Nachlaufzeit
//#####ulsch	
	char Puwtna;		// Tauscherpumpe Nachlaufzeit
	char Puwzdau;		// Zirkulationspumpe Abschaltdauer [std]
	char Puwzab[2];	// Zirkulationspumpe Abschaltzeit [min][std]
	char Legjn;			// Legionellentod
	char Legtag;		// Legionellentage
	char Legzeit[2];// Legionellen Einschaltzeit			[min][std]
	char Legdau;		// Legionellen Einschaltdauer			[std]
	int  Twwleg;		// Legionellen Temperatur
	char Vorrad;		// Vorrangdauer	max								[min]

	int		Ts;				// Tastzeit (>= 1 s)						[s] * 10
	UINT	Tn;				// Nachstellzeit								[s] * 10
	int		Kp;				// P-Verstärkung							[%/K] * 100
	int		Kd;				// D-Verstärkung							[%/K] * 100
	char	Hub;			// Ventilhub									 [mm]
	char	Vst;			// Stellgeschwindigkeit		 [mm/min]
	char 	Haut;			// Hand/Automatik
	int  	Vstell;		// Hand Ventil stellen			0-10V Ventil	[%] * 10
	char 	Lpea;			// Hand Ladepumpe ein/aus 
	char 	Zpea;			// Hand Zirkpumpe ein/aus
	char 	Tpea;			// Hand Tauscherpumpe ein/aus	
	int		Fzk;			// Filterzk. f. tsol						[s] * 10	
	char	TabwMax;	// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	char	TabwZeit;	// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]	
	int 	TvorrDiff;// Regelabweichung zur Aktivierung des WW-Vorrangs [K] * 10	
	char	TvorrZeit;// Mindestzeit zur Aktivierung des WW-Vorrangs [min]	
	char	ZpuAusLad;// ZirkPu bei Ladung ausschalten
	char	FrLadBedarf;	// Freigabe für Schalten des Ladebetriebs nach Bedarfsanford. ( über Kommando WR_KASKADE )
	char	FrZirkBedarf;	// Freigabe für Schalten der Zirk.pumpe   nach Bedarfsanford. ( über Kommando WR_KASKADE )	
	char	AuswahlFuehler;	// Speicherladung: Auswahl Fühler ( 1 = oben, 2 = unten, 0 = beide )																															
	int		TvpAnh;		// Anhebung der Anforderung an den Wärmeerzeuger		[K] * 10		
	int		TempWWmin;			// minimale Gebäude-Vorlauftemp. zur Bildung eines Alarms [°C] * 10
	char	TempWWminZeit;	// Zeitdauer der minimalen Gebäude-Vorlauftemp. bis zum Alarm [min]	
	int 	LadungMax;			// [h]		

	char 	PuwlBmVerz;			// [min]
	char 	PuwzBmVerz;			// [min]
	char 	PuwtBmVerz;			// [min]
	char	LadNieAus;			// Speicherladung immer (temperatur-unabhängig) während Legio-Betrieb
	int	 	TwwsolLegio;		// WW-Solltemp.während Legionellenabtötung

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	int		Wup;						// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	int	 	TwwzLegio;	// Zirkulations-Solltemp.bei Legio-Betrieb

}WwStandard;	
#define WWSLENG sizeof(struct ws)

/* Struktur der Regelparameter eines Warmwasserkreises				*/
// Teil2: Dynamische Parameter	
typedef struct wd{
	char vzu;				// Hand Ventil zufahren			[s]
	char vauf;			// Hand Ventil auffahren		[s]
	int	 zts;				// Zaehler fuer Tastzeit
	char regstart;	// Reglerstart nach Reset
	char wwvor;			// Warmwasservorrang
	char wwlad;			// Ladebetrieb
	int  zpuwlna;		// Zähler für Ladepumpennachlaufzeit
// ##### ulsch #####	
	UINT zpuwtna;		// Zähler für Tauscherpumpennachlaufzeit	
	UINT puwlsta;		// Anzahl der Ladepumpenstarts
	UINT puwlh;			// Ladepumpenlaufzeit							[h]
	int  zpuwz;			// Zähler für ZirkPu Abschaltdauer
	char legio;			// Legionellenbetrieb
	int  zleg;			// Zähler für Legionellenbetrieb
	int  zvorrad;		// Zähler für Vorrangdauer
	int  ei_ww;			// Regelabweichung für gleitenden Vorrang, zur Auswertung in RegelHk

	int		ei;				// Regelabweichung							[K] * 10
	float	fl_ei1;		// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2;		// Regelabweichung zum Tastzeitpunkt i-2
	int		tsol;			// Solltemp. , nach der geregelt wird
	float fl_tsol;	//		"								(für Filterberechnung)
	int		stellzeit;// berechnete Zeit für Stellausgabe
	int		stellsum;	// Summenzähler für Stellausgabe
	int 	fahren;		// Fahrzeit Stellausgabe in ganzen Sekunden
	float fl_y_rel;	// PID-Stellgröße		
	int   si_y_rel;	// PID-Stellgröße für debug [%] * 10
	int		y_rel;		// Stellausgabe an 0-10V Ventil	[%] * 10

	int		twwsb;		// berechneter Sollwert ( Zirkulations-, Ladungs-, Legionellentemp. )
	UINT 	abwCtr;		// (Sekunden)-Counter der Regelabweichung
	int	vorrVerz;	// Zähler für Vorrang-Ein-Verzoegerung

	char wwladBedarf;			// Ladebetrieb nach Bedarfsanford. ( über Kommando WR_KASKADE )
	char wwladCountDown;
	char wwzirkBedarf;		// Zirkulationspumpe ein nach Bedarfsanford. ( über Kommando WR_KASKADE )
	char wwzirkCountDown;

	UINT 	wwMinCtr;	// (Sekunden)-Counter für SM Gebäude-Vorlauf-Temp.

	// Softalarme
	char tempAlarm;	// Gebäude-Vorlauftemp. TWWV überschreitet fuer eine definierte Zeit einen vorgegebenen Parameter
	char abwAlarm;	// Regelabweichung ueberschreitet fuer eine definierte Zeit einen vorgegebenen Betrag
	char puwlBm;		// Softalarm für 'Betriebsmeldung Ladepumpe' kommt nicht
	char puwzBm;		// Softalarm für 'Betriebsmeldung Zirk.pumpe' kommt nicht
	char puwtBm;		// Softalarm für 'Betriebsmeldung Tauscherpumpe' kommt nicht
	char legioAl;
	UINT wwLadCtr;	// min		
	char wwLadAlarm;		

	char zirkPu;		// EIN = TRUE	
	
	char busZPuSm;	  // Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char busZPuEin;
	char busTPuSm;	  // Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char busTPuEin;
	char busLPuSm;	  // Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char busLPuEin;

	UINT puwlBmVerz;	// [min/2]
	UINT puwzBmVerz;	// [min/2]
	UINT puwtBmVerz;	// [min/2]
	UINT puwzh;				// Zirkpumpenlaufzeit							[h]
}WwDynam;	

//----------------------------------------------------

/* Struktur der Parameter einer Kesselsteuerung			*/
// Teil1: Statische Parameter (mit Standdardwerten)
typedef struct ks{
// Parameter fuer Kesselfolgesteuerung, gueltig mit Index 0 ( KE1 )
//################################################################################ FuehrKes muss der erste Parameter sein !!!
	ULONG FolgeSoll;

	char FuehrKes;	// Führungskessel
	int KesWechseln;		// laufzeitabh. Umschalten des Führungskessels  [h]
	UINT KesSchaltDiff;	// Schaltdifferenz zum Abschalten des Kessels
	char FuehrKesFest;	// Führunskessel fest vorgegeben, wenn ungestoert	

	int	 Tvma;			// max. Kesseltemp.					 			[°C] * 10
	int  Tvmi;			// min. Kesseltemp.					 			[°C] * 10
	int  Ttaup;			// Kessel-Taupunkttemp.			 			[°C] * 10
	int  Tvpa;			// Kesselanhebung					 				[°C] * 10
	int	 Einkrit1;	// Einschaltkriterium 1.Stufe	[K*min] * 10
	int  Auskrit1;	// Ausschaltkriterium					[K*min] * 10
	int  Einkrit2;	// Einschaltkriterium 2.Stufe	[K*min] * 10
	int  Auskrit2;	// Ausschaltkriterium					[K*min] * 10	
//-	char Kemin;			// Kesselmindestlaufzeit				 [min]
//-	char Keanf;			// Kesselanfahrzeit							 [min]
//-	int  Ta2ein;		// Außentemp.grenze Ke2 ein 			[°C] * 10
	int  TvsolX2;		// Solltemperatur Max							[°C] * 10
	int	 SollY2;		// Sollspannung Max							  [V] * 10
	int  TvsolX1;		// Solltemperatur Min							[°C] * 10
	int	 SollY1;		// Sollspannung Min							  [V] * 10

//ulsch
	char Haut;			// Hand/Automatik
//-	char HautVent;	// Hand/Automatik Ventil
	char Kesea;			// Hand Kesselfreigabe ein/aus			
	char Kestufe2;	// Hand Kesselstufe 2  ein/aus
	UINT Kestell;		// Hand Führungswert 0-10,0 Volt
	char Puea;			// Hand: Pumpe	ein/aus
	char Puna;			// Pumpennachlaufzeit
	
	UINT Ts;				// Tastzeit (>= 1 s)							 [s] * 10
	UINT Tn;				// Nachstellzeit									 [s] * 10
	UINT Kp;				// P-Verstärkung								 [%/K] * 100
	UINT Kd;				// D-Verstärkung								 [%/K] * 100	
	UINT Stellzeit; //																 [s]

	// Rücklauf-Begrenzung, Rücklauf-Anhebung	
	UINT Ts_rl;			// Tastzeit (>= 1 s)							 [s] * 10
	UINT Tn_rl;			// Nachstellzeit									 [s] * 10
	UINT Kp_rl;			// P-Verstärkung								 [%/K] * 100
	UINT Kd_rl;			// D-Verstärkung								 [%/K] * 100		
	UINT Stellzeit_rl; //																 [s]

	char KeBm_verz;	// Verzögerung Kesselbetriebsmeldung  [min]
	
	// Sollwert-Anhebung
	UINT Ts_anh;		// Tastzeit (>= 1 s)							 [s] * 10
	UINT Tn_anh;		// Nachstellzeit									 [s] * 10
	UINT Kp_anh;		// P-Verstärkung								 [%/K] * 100
	UINT Kd_anh;		// D-Verstärkung								 [%/K] * 100		
	 
	char Kesklaz;				// Hand Kessel-Klappe
	char HautKes;				// Hand/Auto Gruppe "KES"
	char HPuea;					// Hand Hauptpumpe ein/aus
	char SSFdauer;			// Schornsteinfeger-Funktion [min]	
	char FolgeKesAus;		// Abschalten des Folge-Kessels, wenn VL - RL des Primär-Kessel zu klein [K] * 10 	
	char FolgeKesAusVerz;			// Verzögerung des Abschaltens des Folgekessels bei Unterschreitung VL-RL 		
	int	 TaFuehrKesUmschalten;	// Führungskes.ist abhängig von Aussentemp. (keine Laufzeitabh., KesWechseln = 0) [°C] * 10
	char FuehrKesTaPlus;			// Führungskessel bei Ta > TaFuehrKesUmschalten;
	char FuehrKesTaMinus;			// Führungskessel bei Ta < TaFuehrKesUmschalten;			

	UINT Kestell_rl;				// Hand Führungswert 0-10,0 Volt

//-	// Pumpe regeln
//-	char HautPu;				// Hand/Automatik
//-	int	 TsPu;			// Tastzeit (>= 1 s)						[s] * 10
//-	int	 TnPu;			// Nachstellzeit								[s] * 10
//-	int	 KpPu;			// P-Verstärkung							[%/K] * 100
//-	int	 KdPu;			// D-Verstärkung							[%/K] * 100	
//-	UINT PuStell;			// Hand: Pumpe stellen 0-10V   	[%] * 10	
//-	int  SwPu20u;			// Sollwert Pu-Regel. -20 °C	  [K] * 10
//-	int  SwPu20o;			// Sollwert Pu-Regel. +20 °C	  [K] * 10	
//-	int  DzPuMin;			// Mindest-Drehzahl der Pumpe  [%] * 10
//-	int	 TdiffMin;
//-	UINT PuDzMax;
//-	int	 TdiffMax;
//-	UINT PuDzMin;
	
	int	 ModMin;			// min.Stellausgabe der Modulation [V] * 10
	char Blockierschutz;

	char Sperre;
	char SchaltpauseZu;
	char SchaltpauseAb;
	int  VlVerteilerMax;
//-	int	 Anfahrbetrieb;
	int	 PuAbschalt;

	int	 TvsbErsatz;
	char Modulation;
	char FuehlerSSTM;				// Fühlerfehler setzt Ausgang SSTM
	char KesNachlauf;

//-	ULONG FolgeSommer;
//-	ULONG FolgeWinter;

	int	 TvMax;							// max. Vorlauftemp.					 			[°C] * 10
	
	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	int	 Wup;									// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	char Klna;			// Klappennachlaufzeit
	int	 Wup_rl;							// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )
	char RlAnheb;		// RlAnheb = 0/1 -> Begrenzung/Anhebung

	// die folgenden 4 Parameter definieren eine Gerade zur Steuerung der Drehzahl der Pumpe in Abhängigkeit von der Modulation
	int	 P1ModDz;
	int	 P1Dz;
	int	 P2ModDz;
	int	 P2Dz;
	int	 DzPuHand;

	char PuBm_verz;	// Verzögerung Kesselbetriebsmeldung  [min]

	UINT KesselAusMod;			//	[%] * 10
	char KesselAusModVerz;	// min
	UINT KesselEinMod;			//	[%] * 10
	char KesselEinModVerz;	// min
																							// 122 Parameter
	int	 AnhebMax;
	
	char ModVerzSomm;
	char ModVerzWint;

	char TabwMax;		// maximale Solltemperatur-Abweichung zur Bildung eines Alarms [K] * 10
	char TabwZeit;	// Zeitdauer der maximalen Solltemperatur-Abweichung bis zum Alarm [min]
																											
}KeStandard;	
#define KESLENG sizeof(struct ks)

/* Struktur der Parameter einer Kesselsteuerung			*/
// Teil2: Dynamische Parameter	
typedef struct kd{
	char kesea;			// Hand Kesselfreigabe ein/aus
	char gestoert;		
	int	 tvsb;			// berechnete KesselVorlaufsolltemp.

//ulsch
	int	 tvsb_ber;	// berechnete KesselVorlaufsolltemp. ( inkl. Anhebung )
			
	int  y_rel;			// Stellausgabe Führungswert
	int   si_y_rel;	// PID-Stellgröße für debug [%] * 10
	char zkmin_m;		// Zähler f. Kessel Mindestlaufzeit [min]
	char zkmin_s;		// Zähler f. Kessel Mindestlaufzeit [sek]
	char zkanf_m;		// Zähler f. Kessel Anfahrzeit [min]
	char zkanf_s;		// Zähler f. Kessel Anfahrzeit [sek]
	int  abw1;			// Abweichung Kessel_Ist - Kessel_Soll
	int  abwAlt1;		// alte Abweichung
	int  abwsumme1;	// Summe der Abweichungen
	int  abw2;			// Abweichung Anlage_Ist - Anlage_Soll
	int  abwAlt2;		// alte Abweichung
	int  abwsumme2;	// Summe der Abweichungen	

	char stufe1;
	char stufe2;

	UINT zts;
	UINT zts_rl;		// Zähler f. Tastzeit der Rücklauf-Begrenzung	
	UINT zts_anh;		// Zähler f. Tastzeit der Anhebung
	
	// eventl. noinit-ram
	int  kestarts;	// Anzahl der Kesselstarts
	int  kesh;			// Kessellaufzeit
	int  kes2h;			// Laufzeit Stufe 2 

//ulsch
	char	vzu;			// Hand Ventil zufahren			[s]
	char	vauf;			// Hand Ventil auffahren		[s]

	int		ei;				// Regelabweichung							[K] * 10
	int		tsol;			// Solltemp. , nach der geregelt wird	
	float	fl_ei1;		// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2;		// Regelabweichung zum Tastzeitpunkt i-2
	float fl_y_rel;
	int		stellsum; // berechnete Zeit für Stellausgabe
	int 	fahren;
			
// Parameter fuer Kesselfolgesteuerung, gueltig mit Index 0 ( KE1 )	
	int	 tvsb_anl;	// Anlage Vorlauf Solltemp. (max. Anforderung )
	// Sollwert-Anhebung
	int y_rel_anh;								
	float	fl_ei1_anh;		// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2_anh;		// Regelabweichung zum Tastzeitpunkt i-2
	float fl_y_rel_anh;		

	int y_rel_rl;			// Stellausgabe Führungswert
	int si_y_rel_rl;	// PID-Stellgröße für debug [%] * 10
	int ei_rl;				// Regelabweichung							[K] * 10		
	float	fl_ei1_rl;	// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2_rl;	// Regelabweichung zum Tastzeitpunkt i-2
	float fl_y_rel_rl;
	int		stellsum_rl;// berechnete Zeit für Stellausgabe
	int 	fahren_rl;
	int soll_rl;
	
	// Softalarme
	char keBm_sAl;	// Softalarm Fehlende Betriebsmeldung Kessel 
	UINT keBm_cnt;	// Wartezeit [s]
	
	char ssfEin;		// Schornsteinfeger-Funktion
	UINT ctrSSF;		// Zähler Schornsteinfeger-Funktion [min]	
	char folgeKesAusVerz;	// Verzögerung des Abschaltens des Folgekessels bei Unterschreitung VL-RL 		
	
	int		tsolPu;		// aussentemperaturabh. Sollwert, auf den die Diff. Vorlauf-Rücklauf über die Pumpendrehzahl geregelt wird
	int		eiPu;			// Regelabweichung							[K] * 10
	float	fl_ei1Pu;	// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2Pu;	// Regelabweichung zum Tastzeitpunkt i-2
	UINT	ztsPu;		// Zaehler fuer Tastzeit
	int		y_relPu;	// Stellausgabe an Pumpe 0-10V	[%] * 10
	float	fl_y_relPu;	

	char	rvzu;			// Hand RL-Begrenzungs-Ventil zufahren		[s]
	char	rvauf;		// Hand RL-Begrenzungs-Ventil auffahren		[s]

	ULONG folgeIst;	
	ULONG folgeSollAlt;
	ULONG folgeIstAlt;
	char kesEin;
	UINT pauseCtr;
	UINT modulatMW;
	UINT kesselAusModCtr;
	char modMwKesAus;
	char modMwKesEin;
	UINT kesNachlaufCtr;	//	[s]
	char kesEinKE;				// ersetzt den Ausgang KE, wenn der nicht vorhanden

	char busPuSm;	  // Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char busPuEin;
	char pu;
	int	 puDz;
	
	UINT kesSec;
	UINT kes2Sec;
	UINT puna;	
	UINT klna;
	
	int	 dzPu;

	char puBm_sAl;	// Softalarm Fehlende Betriebsmeldung Kessel 
	UINT puBm_cnt;	// Wartezeit [s]
	int	 sollwert;

	int  kesselModCtr;
	
	UINT modVerz; 
	char abwAlarm;			// Regelabweichung ueberschreitet fuer eine definierte Zeit einen vorgegebenen Betrag	
	UINT tabwCnt;			
}KeDynam;	

//---------------------------------------------------------------------

/* Struktur der Regelparameter eines Vorregelkreises (Netztrennung)*/
// Teil1: Statische Parameter (mit Standdardwerten)
typedef struct ns{
	char	FuehrWt;	// Führungs-Tauscher

	int		Ts;				// Tastzeit (>= 1 s)						[s] * 10
	UINT	Tn;				// Nachstellzeit								[s] * 10
	int		Kp;				// P-Verstärkung							[%/K] * 100
	int		Kd;				// D-Verstärkung							[%/K] * 100
	int		Kpr;			// P-Verstärkung TRS-Begrenzg.[%/K] * 100
	int		Tvma;			// max. Vorlauftemp.					 [°C] * 10		
	int		Tvmi;			// min. Vorlauftemp.					 [°C] * 10		
	int		Trma;			// max. Rücklauftemp.					 [°C] * 10		
	int   Tvpa;			// Anhebung					 					 [°C] * 10
	int		Fzk;			// Filterzk. f. tsol						[s] * 10
	char	Hub;			// Ventilhub									 [mm]
	char	Vst;			// Stellgeschwindigkeit		 [mm/min]
	char	Haut;			// HAND/AUTO-Umschaltung
	int 	Vstell;		// Hand Ventil stellen			0-10V Ventil	[%] * 10
	char	Puea;			// Hand: Pumpe	ein/aus	
	char	TabwMax;	// maximale Regelabweichung zur Bildung eines Alarms [K] * 10
	char	TabwZeit;	// Zeitdauer der maximalen Regelabweichung bis zum Alarm [min]

// Parameter für 2 Tauscher (Tauscher-Folge)
	char	FuehrWtFest;	// Führungs-Tauscher fest vorgegeben, wenn ungestoert	
	int 	WtWechseln;		// laufzeitabh. Umschalten des Führungs-Tauschers  [h]
	char 	FolgeWtAus;		// Abschalten des Folge-Tauschers, wenn VL - RL des Primär-Tauschers zu klein [K] * 10 	
	UINT 	WtSchaltDiff;	// Schaltdifferenz zum Abschalten des Führungs-Tauschers [K] * 10 
	int	 	Einkrit;			// Einschaltkriterium Folge-Tauscher	[K*min] * 10
	int	 	Auskrit;			// Ausschaltkriterium Folge-Tauscher	[K*min] * 10
	char	KlAZ;					// Klappenstellung bei Hand	
	char	KlappeVerz;		// Verzögerung des Schließens der Klappe [min]
	
	// Sollwert-Anhebung
	UINT 	Ts_anh;				// Tastzeit (>= 1 s)							 [s] * 10
	UINT 	Tn_anh;				// Nachstellzeit									 [s] * 10
	UINT 	Kp_anh;				// P-Verstärkung								 [%/K] * 100
	UINT 	Kd_anh;				// D-Verstärkung								 [%/K] * 100		

	char 	PuBmVerz;				// [min]

	// Wind-Up: Begrenzung der Stellgröße des PID-Reglers auf einen gleitenden oder festen Maximalwert (anti windup)
	int		Wup;					// 0 = gleitend (Produkt aus Kp * ei),  >0 = fester +/- Maximalwert  [%] * 10. ( nur positiven Wert eingeben  )

}NtStandard;
#define NTSLENG sizeof(struct ns)

/* Struktur der Regelparameter eines Vorregelkreises									*/
// Teil2: Dynamische Parameter	
typedef struct nd{
	int		zts;			// Zaehler fuer Tastzeit
	char	regstart;	// Reglerstart nach Reset
	int		ei;				// Regelabweichung							[K] * 10
	float	fl_ei1;		// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2;		// Regelabweichung zum Tastzeitpunkt i-2
	int		tsol;			// Vorlaufsolltemp. , nach der geregelt wird
	float fl_tsol;	//		"								(für Filterberechnung)
	int		tvsb;			// berechnete Vorlaufsolltemp. total
	int		stellzeit;// berechnete Zeit für Stellausgabe
	int		stellsum;	// Summenzähler für Stellausgabe
	int 	fahren;		// Fahrzeit Stellausgabe in ganzen Sekunden
	float fl_y_rel;	// PID-Stellgröße		
	int   si_y_rel;	// PID-Stellgröße für debug [%] * 10
	int   y_rel;		// Stellausgabe an 0-10V Ventil	[%] * 10
	char	zpu;			// Zähler für Pumpenachlaufzeit
	char	vzu;			// Hand Ventil zufahren			[s]
	char	vauf;			// Hand Ventil auffahren		[s]
	char	rlbegr;		// Statusanzeige Rücklaufbegrenzung
	UINT 	puLz;			// Pumpen-Laufzeit [h]	
	UINT  abwCtr;		// (Sekunden)-Counter der Regelabweichung

// Parameter für 2 Tauscher (Tauscher-Folge)
	int	 	tvsb_anl;	// Anlage Vorlauf Solltemp. (max. Anforderung )
	char	wtEin;
	// Sollwert-Anhebung
	int 	y_rel_anh;								
	float	fl_ei1_anh;		// Regelabweichung zum Tastzeitpunkt i-1
	float	fl_ei2_anh;		// Regelabweichung zum Tastzeitpunkt i-2
	float fl_y_rel_anh;		
	UINT 	zts_anh;			// Zähler f. Tastzeit der Anhebung
	
	char 	gestoert;		
	UINT 	lzStd;				// Laufzeit Stunden
	char 	lzMin; 				// Laufzeit Minuten/2
	int 	abw;					// aktuelle Istwert-Sollwert-Differenz, Anlagenfuehler
	int  	abwAlt;				// alte Istwert-Sollwert-Differenz
	int  	abwsumme;			// Summe der Regel-Abweichungen

	UINT	klappeCtr;		// Verzögerung des Schließens der Klappe [sec]	

	//Soft-Alarme
	char abwAlarm;			// Regelabweichung ueberschreitet fuer eine definierte Zeit einen vorgegebenen Betrag	

	char pu;
	char busPuSm;				// Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char busPuEin;

	UINT blockSec;			// Hauptpumpe: Verzögerung Blockierschutz, wenn Pumpe aus [sec]
	char blockStd;			// Hauptpumpe: Verzögerung Blockierschutz, wenn Pumpe aus [std]
	char blockPuEin;		// Hauptpumpe EIN zwecks Blockierschutz [sec]

	UINT puBmVerz;			// [sec]
	char puBm;					// Softalarm für 'Betriebsmeldung Hauptpumpe' kommt nicht
}NtDynam;	
 
//---------------------------------------------------------------------


/* Struktur der Regelparameter einer Solar-Regelung */
// Statische Parameter (mit Standdardwerten)

typedef struct ss {

	int		TKolBeg;	// erforderliche Mindesttemp. am Kollektor-Fühler ( Solar-Lad. ein ) [°C] * 10
	int 	TKolEnd;	// Min. Temp. am Kollektor-Fühler ( Solar-Lad. aus ) [°C] * 10
	int		TPufMax;	// Max. Temp. am oberen Puffer-Fühler ( Solar-Lad. aus ) [°C] * 10
	int		TDiffBeg;	// erforderliche Mindesttemp.-Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. ein ) [K] * 10
	int 	TDiffEnd;	// Differenz Kollektor - unterer Puffer-Fühler ( Solar-Lad. aus )	[K] * 10
	char	Puna;			// Kollektorpumpen-Nachlaufzeit	[min]
	char	Haut;			// HAND/AUTO-Umschaltung
	char 	Kpea;			// Hand Kollektorpumpe ein/aus		
																																		// Struktur 13 Byte
} SoStandard;
#define SOSLENG sizeof (struct ss)

/* Struktur der Regelparameter einer Solar-Regelung	*/
// Dynamische Parameter	
typedef struct sd {
	char	solLad;		// aktueller Ladezustand ( 0/1 )
	char	zpu;			// Zähler für Pumpenachlaufzeit
	UINT  lzKolPuH;	// Kollektorpumpenlaufzeit [h]
	UINT  lzKolPuMin;	// Kollektorpumpenlaufzeit, Zwischenzaehler [min]

	char	busPuSm;			// Zusammenfassung der über den Pumpenbus (WILO_MODBUS, GENI) signalisierten und der vom Regler gebildeten Störungen
	char	busPuEin;

} SoDynam;	
 
//---------------------------------------------------------------------

/* Struktur der Regelparameter Benutzersteuerung mit UNI-Elementen */
// Statische Parameter (mit Standdardwerten)
typedef struct uns {
	char	Loader;			// Testparameter
	
} UniStandard;
#define UNISLENG sizeof (struct uns)

// Dynamische Parameter	
typedef struct und {
	UINT counter;			// Testparameter
	char starter;

} UniDynam;	
	
//---------------------------------------------------------------------

// #####ulsch : Ferienprogramm #####
typedef struct ferienZeit {
	datum ferienBeg;
	datum ferienEnd;
} ferienZeit;


// #####ulsch : Heizgradtage #####
typedef struct {
	UINT hzGrdTage;
	char hzTage;
	char hzTageBew;	
	UINT hzGrdTageVj;
	char hzTageVj;
	char hzTageBewVj;		
} monHzGrd;												// size 8 Byte, wichtig fuer Bicbus-Zugriffe ( 64/8 ist restlos teilbar ! )

typedef struct {
	mwsp copyData;
	char copyFirst;
	UINT copyRxTimer; 
} copydata;



/* Struktur eines Wärmezählers */
typedef struct {
		ULONG wmeng;						// Zählwert absolut	( kWh )
		ULONG verbr[12];				// Monatsverbräuche Aktuell
		ULONG verbr_vj[12];			// Monatsverbräuche Vorjahr		
		ULONG flow_max;
		UINT wmLeistg_max;
		char resMax;						// Maximal-Werte in Struktur zaehlspWmeng loeschen ( flow_max, wmLeistg_max, auch wmLeistgMittelMax )
		char syncFlag;
		ULONG flow;							// Dim. 0,0 m3/h 
		UINT wmLeistg;					// kW
		zaehlsp *pZaehlsp;
		mwsp *pVl;
		mwsp *pRl;
		ULONG lastWmeng;				// für Leistungsberechnung
		ULONG wmeng_Jsum;
		ULONG wmeng_J;					// in Joule 
		char monInVj;
		ULONG long lastWert;		// aktueller Zaehlerstand

		UINT volumen;
		UINT zeitdiff;	
		UINT zeitdiffZuletzt;
		float masse;
		char monSichern;
		char monReset;
			
} zaehlspWmeng;		

/* Struktur zur Berechnung und Anzeige der maximalen Durchschnittsleistung (Standard: Stunde) */
typedef struct {
		char MittelnLeistg;			// im EEPROM gespeichert [min] (zulässig : 5, 10, 15, 20, 30, 60)
		UINT mittelMax;					// immer zuerst in dieser Struktur !!!	
		UINT mittel;						// letzter berechneter Wert
		datum tag;
		uhrzeit zeit;
		mwsp vl;								// Primär-Vorlauf	
		mwsp rl;								// Primär-Rücklauf
		mwsp ta;								// Außentemperatur
		ULONG flow;							// Volumenstrom	
		char resMax;						// alles rücksetzen
} maxLeistung;

/* Struktur zur Berechnung und Anzeige Grenz-Leistung */
typedef struct {
		UINT GrenzLeistg;				// Grenz-Leistung im EEPROM gespeichert [kW] mit Standard-Wert
		UINT LeistgHyst;				// Hysterese für Grenz-Leistung im EEPROM gespeichert [kW] mit Standard-Wert
		char BegrAkt;						// 0 : Begrenzung der mittl.Leistung (5 min) 1: Begr.der akt.Leistung
		UINT Daempfung;					// leistBegr wird gedämpft, wenn die Momentanleistung zur Begrenzung benutzt wird
} grenzLeistungStat;

typedef struct {
		char begrenzung;				// Meldung Begrenzung Ein ?
		UINT msec;							// Messung der Millisekunden ab letztem Zählimpuls
		UINT altMsec;						// Millisekunden alter Impulsabstand
		ULONG long lastWert;		// letzter Zähler-Wert
		UINT kwhImp;						// KWh pro Impuls
		UINT leistung;					// Leistung in kW
		UINT leistBegr;
} grenzLeistungDyn;


/* Diagnose */
typedef struct {
	int	temp;
	datum tag;
	mwsp ta;		
} extrTemp;												// Min- oder Max-Temperatur mit zugehoerigem Tag und Aussentemperatur

typedef struct {
	ULONG flow_max;
	UINT wmLeistg_max;
	UINT flowBereich_h[4];
	UINT leistgBereich_h[4];
	UINT wwlad_anz;
	UINT wwlad_time;
	UINT wwvor_anz;
	UINT wwvor_time;
	extrTemp tvpMax;
	extrTemp tvpMin;			
} diagnose;

typedef struct {
	datum begin;
	datum end;
	ULONG	flowBereich[4];
	UINT leistgBereich[4];
} diagParam;


// für U_Test_RS485
typedef struct {
	char Mode;							// Master (1) oder Slave (0) Mode
	char status;						// Status der Datenübertragung im Task 'SlaveS1'
	char rxtest;						// Testanzeige
	char Leng;							// Telegrammlänge im Rx-Interrupt   
	char TLeng;							// Telegrammlänge im Task 'SlaveS1'
	char Bcount;						// Sende- / Empfangspuffer Index
	char Cond;							// Empfangszustand im Rx-Interrupt 
	char bcc;								// Block Check Code
	char TelCnt;						// Teiler für Telegramm-Timeout
	char stx;								// Merker für STX oder STX8
	char ordnum;						// letzer Auftrag
	char tsk_cnt;						// Antwortverzögerung
	UINT Baudrate;
	UINT match_fall;
	UINT match_rise;
	int  MatchCount;
	int  BaudQuali;
	char tmr_count;				// Zähler für TMR_BIT Überwachung
	UINT toutl;
	UINT touth;
	char tout_ext;					// bei FLASHCONTROL-Kommandos TimeOut-Verlängerung in Rx_Init();
	DatBits cstat;
	char merkAdr;
} serial485;	

// für U_Funkempf
typedef struct rf{
	unsigned char ucRf_status;						// Status des Sensors und der Schalterstellung am Sensor
	unsigned char ucRf_praesens;
	signed int siRf_temp;
	signed char scRf_temp_shift;
	unsigned char ucLive_check_timer;
	unsigned char ucSetup;								// muß in den EEPROM
	unsigned long ulRf_modul_id;					// muß in den EEPROM
	unsigned char ucfBedienZeit[6];				// [0] = Tag, [1] = Monat, [2] = Jahr, [3] = Stunde, [4] = Minute, [5] = Sekunde
	unsigned char ucfEmpfangsZeit[6];			// [0] = Tag, [1] = Monat, [2] = Jahr, [3] = Stunde, [4] = Minute, [5] = Sekunde
	unsigned char ucRf_teach_enable;
//	char rf_switch;
}rfsens;

// ***** josch: Datenmanager ***************************************************************
// Gerät im DM_Modus (Data Master),	Datenbearbeitung in der Funktion DManager(), Organisation der Datenübertragung im Task DTimer() 
typedef struct {
	char aktiv;									// Datenübertragung anregen im DTimer
	char ready;									// Datenübertragung ausgeführt
	char SlaveAdr;							// Moduladresse des Slavegerätes
	char TxLeng;								// an den Slave zu sendende Datenanzahl
	char TxBuf[DM_TXBUF_MAX];		// an den Slave zu sendende Daten
	char RxTout;								// Empfangs-Timeout in Minuten
	char RxStat;								// Status der Empfangsdaten
	char RxBuf[DM_RXBUF_MAX];		// vom Slave empfangene Daten
	mwsp rdta;									// Außentemperatur vom Slave empfangen 
	char rdta_bc;								// Außentemperaturquelle (bc_flag)
	char rdta_flag;							// Flag für erfolgte Datenübertragung					
}dm_param;	
	
	
// Änderung neue Genibus-Implementierung	
	#if GENI == 1
		#include "GeniBus/genibus_struct.h"	
	#endif
//---------------------------------------------------------------

// Measure Values
typedef struct gm{
	int pu_t_w;					// (Medientemperatur)
	int pu_p;						// (akt. Leistung)
	int pu_speed;				// (Drehzahl)
	int pu_h;						// (akt. Förderhöhe)
	int pu_q;						// (akt. Förderstrom)
	int pu_led_contr;
	int pu_act_mode1;
	int pu_act_mode2;
	int pu_act_mode3;
	int pu_al_code;
//	char pu_f_act;				// (akt. Frequenz)
// werden getrennt behandelt
	UINT energy;				// (Energieverbrauch)
	UINT t2hour;				// (Betriebsstunden)
	char pu_unit_family;
	char pu_unit_type;
	char pu_unit_version;
	char pu_operation_inp;
	char pu_control_inp;
}measure_value;
//#define GMLENG sizeof(struct gm)
//---------------------------------------------------------------


/**************** Modbus-Daten-Struktur für Wilo ****************/

/*-------- Input Register für Wilo Pumpen für IF-Modul Modbus Stratos (2097808) --------*/
typedef struct {
	
	// Input Register												// Adresse		Einheit		Darstellung (Nachkomma)		Anmerkung
	mwsp mwspActualDifferentialPressure; 		// 1					m H2O			0.1 											
	mwsp mwspFlowRate; 									 		// 2					m H2O			0.1												
	mwsp mwspPowerConsumption;							// 3					kWh				1		 											
	mwsp mwspPowerRating;										// 4					W					1
	mwsp mwspOperationHours;								// 5					h					10
	mwsp mwspMainsCurrent;									// 6					A					0.1
	mwsp mwspSpeed;													// 7					min-1			1
	mwsp mwspMediumTemperature;							// 8					K					0.1
	mwsp mwspOperatingHoursDP;							// 9					h					10												Nicht verwendet
	mwsp mwspCurrentOperationMode;					// 10		
	
	mwsp mwspPumpModule;										// 16		
	mwsp mwspPumpType;											// 17		
	mwsp mwspMaxSpeed;											// 18					min-1			1
	mwsp mwspMinSpeed;											// 19					min-1			1
	
	mwsp mwspSupportedErrors;		          	// 26
	mwsp mwspSupportedServiceMessages;			// 27
	mwsp mwspMaxPowerRating;			        	// 28					W					1 
	
	mwsp mwspServiceMessage;		          	// 35
	mwsp mwspErrorType;		                	// 36
	mwsp mwspErrorMessage;		            	// 37
	mwsp mwspPumpStatus;		              	// 38
	mwsp mwspStateDiagnostics;		        	// 39

}wiloInputRegister;
#define WILO_INPUT_REG_ANZ sizeof(wiloInputRegister)/sizeof(mwsp) // Anzahl der InputRegister bei Wilo Pumpen

/*--------  Holding Register für Wilo Pumpen für IF-Modul Modbus Stratos (2097808) --------*/
typedef struct {
	// Holding Register 										// Adresse		Einheit		Darstellung (Nachkomma)		Anmerkung
	mwsp mwspSetValue;											// 1					%					0.5
	mwsp mwspPumpCommand;		                // 40				  
	mwsp mwspOperationMode;		              // 42	 
	mwsp mwspBusCommandTimer;		            // 300	 

}wiloHoldingRegister;
#define WILO_HOLDING_REG_ANZ sizeof(wiloHoldingRegister)/sizeof(mwsp)	// Anzahl der HoldingRegister bei Wilo Pumpen

/*-------- Wilo Pumpen Gesamtstruktur für IF-Modul Modbus Stratos (2097808) --------*/
typedef struct {
	
 	wiloInputRegister wiloInReg; 			// Input Register		
	 	 
	wiloHoldingRegister wiloHoldReg; 	// Holding Register

	
	char busSmWar;										// Fehlerdiagnose
                      				
	UINT SetPoint_Input;							// Sollwert 	  Ist, auf Wilo-Konvention zurück dekodiert		
	UINT Operation_Input;							// Betriebsart Ist, auf Wilo-Konvention zurück dekodiert	
	UINT Control_Input;								// Regelart Ist, auf Wilo-Konvention zurück dekodiert	
	                          				
	UINT output_value1_temp;					// Betriebsart, OutputRegister, auf Wilo-Konvention dekodiert
	UINT output_value2_temp;					// Regelart, 		OutputRegister, auf Wilo-Konvention dekodiert 
	
	ULONG op_hours;										// Input Register "mwspOperationHours" mal 10 (für Anzeige)
	signed int siMediumTemperature;		// Medium Temperatur in Celsius
	
	unsigned char ucGlobalFlag; 			// Global-Flag Doppelbelegung z. Zt. noch keine Anzeige mehrerer Zustände 
	unsigned char ucServiceMessage; 	// Service Message,   aus dem Bitmuster von mwspServiceMessage dekodierter Charwert für UserKonv.c		
	unsigned char ucErrorType;      	// Error Type,			  aus dem Bitmuster von mwspErrorType dekodierter Charwert für UserKonv.c					
	unsigned char ucErrorMessage;   	// Error Message,		  aus dem Bitmuster von mwspErrorMessage dekodierter Charwert für UserKonv.c			
	unsigned char ucPumpStatus;     	// Pump Status,			  aus dem Bitmuster von mwspPumpStatus dekodierter Charwert für UserKonv.c				
	unsigned char ucStateDiagnostics; // State Diagnostics, aus dem Bitmuster von mwspStateDiagnostics dekodierter Charwert für UserKonv.c

	unsigned char strServiceMessage[16];	// Service Message,  Text-Anzeige in parli_Wilo.h
	unsigned char strErrorType[16];   		// Error Type,			 Text-Anzeige in parli_Wilo.h
	unsigned char strErrorMessage[16];  	// Error Message,		 Text-Anzeige in parli_Wilo.h
	unsigned char strPumpStatus[16];   		// Pump Status,			 Text-Anzeige in parli_Wilo.h				
	unsigned char strStateDiagnostics[16];// State Diagnostics,Text-Anzeige in parli_Wilo.h	
	
	unsigned char ucNeustart;						// Merker für Neustart
}ModBusWiloPumpen;                                           


// ***** Josch-SDM : Archiv SD-Card-Memory ********************************************************* 
// Struktur für Ferro-Ram: Infobereich für den Schnellzugriff bei der Auswahl der Pages auf der SD-Karte
typedef struct fe_info {
	UINT	akt_DP;		// aktuelle Datenpage-Nr.
	char	num_DP;		// Anzahl der Einträge in der Datenpage
	char	off_DP;		// Offset zum nächsten Eintrag (+4 Byte Zeitstempel)
	char	akt_VP;		// aktuelle Verweispage-Nr.
	char	num_VP;		// Anzahl der Einträge in der Verweispage
	char	num_HP;		// Anzahl der Einträge in der Hauptpage
	char	ovr_HP;		// Anzahl der Datenpage-Überläufe (Ringpuffer)
}ferro_info;


// Struktur für SD-Karte: Aufbau des Bereiches für ein DAE
// nur Information !! Die Struktur kann der Compiler nicht verarbeiten, weil die SD-Card-Adressen den Speicherbereich des Prozessors überschreiten.  
//typedef struct sdm {
//	pages hp[1];				// Hauptpage
//	pages vp[16];				// Verweispages
//	pages dp[2031];			// Datenpages
//} dae_page;		

//typedef struct pag {
//	char page[0x20];
//} pages;	
	
//---------------------------------------------------------------

/* Funktionsprototypen */
//float e_filter(float alt, int neu, int ts, int zk);		
float g_filter(float alt, int neu, int ts, int zk);		
void Check_CountDown(void);
void set_bcta1(void);
void Typelist_copy(void);
void Load_Userdef(void);
void T_E_S_T___________P_U_N_K_T(void);
void PumpenAus(void);
void hk_abschalten(char hk, char stemp);
char* get_feldadresse(char *wert, unsigned char ucExp_num, unsigned char ucAttr, unsigned char alfunc); 
void Fehler(char code, signed char idx);		
void fill_dummy(void);
char TempMitt(mwsp* pT1, mwsp* pT2, mwsp* pTmitt);
char get_alspeicher_idx(char *p_wert);
char get_alspeicher(char *p_wert);
void set_alspeicher(char *p_wert, char ea);
float anti_windup(float fl_y_rel, int Kp, int Wup, float fl_ei);
int y_stell(float fl_y_rel);

/***** ulsch : Archivspeicher *****/
void WritePageToArchiv ( char *TxBuf, char *RxBuf, char prot );		
void ReadPageFromArchiv ( char *TxBuf, char *RxBuf, char prot );		
float Dy_rel ( int  kp, int kd, int ts, UINT tn, float ei, float ei1, float ei2 );
int Gerade_YvonX ( int x, int x1, int y1, int x2, int y2 );

/***** ulsch : Heizgradtage *****/
void HgtAnzeigen ( char mon );
void Init_Hgt_User(void);
void Init_Leittab(void);

// josch: in CopyCheck.c
char CopyInit(unsigned int dae_num, char *pDae);
void clear_abs_ram(char hk);
void clear_abs_bed(char hk);

UINT DayOfYear ( datum date );

// josch: Datenmanager
void DReply(void);
void DS_CountDown(void);
void DManager(void);
void DM_CountDown(void);
void RW_dmanager_reply(char *RsTxBuf, char *RsRxBuf, char prot);

// Josch-SDM : Archiv SD-Card-Memory
void ArchSDMReply(char *RsTxBuf, char *RsRxBuf, char prot);

#endif	// USTRUCT_H_INCLUDED
