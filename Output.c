/* Task: Output	*/

#include "sfr32c87.h"
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "snoinitext.h"
#include "unoinitext.h"
#include "standdef.h"
#include "userdef.h"
#include <string.h>

char Dout_procedure(doutsp *Ausgabe, char *schalter, char *pause, char Digold, char anz, char dadat);

/*---------------------- Task Ouput -----------------------------------*/
// Aufruf erfolgt im Sekundenrhythmus
void Output(void)
{
	char obyte, mask, i, j, n;

	/* Überprüfung eines neuen Projekt-Typs				*/
	if(newproj > 0)										// wurde neues Projekt aktiviert ?
	{	if(--newproj == 0)							// wenn Cout Down abgelaufen
		{																// Reset auslösen
			noi_sekcnt = RESETPAUSE + 100;		// bewirkt löschen des Counters für Programmabstürze
			DISABLE_IRQ
			HW_STOP = 0;									// zur Sicherheit
			for(;;);											// Reset durch HWDOG
		}
	}	
	if(proj_typ != temp_proj_typ)			// temp_proj_typ kommt über Bedien (parli)
	{	if(temp_proj_typ >= PROJTYPEN)	// auf max. abtesten
			temp_proj_typ = proj_typ;			// bei Fehler auf alten Wert setzen
		else														
		{	proj_typ = temp_proj_typ;			// neue Nummer,eintragen
			bicbus(EEPADR,	(char	*)&proj_typ,	PROJ_TYP_ADR, 1, BICWR); // und sichern
	 		Kaltstart = 1;								// Kaltstart anregen
	 		bicbus(EEPADR, &Kaltstart, KALTSTART_EADR, 1, BICWR); 	// merken im EEPROM
	 		if(modem_status == 6)					// wenn Modem-Verbindung steht,
				noi_modem_status_save = modem_status;
			strcpy(LCD_Ram,	"KALTSTART AUSGELOEST");
			Line2(255);
			zout = 40;
			newproj = 3;									// nach 3 Sekunden Reset
		}
	}		

	// Alle Testpunkte löschen
	if(testClear > 0)
	{
		for(i=0;i<10;i++)
			testPoint[i] = 0;
			
		testClear = 0;
	}				

	// Überprüfen von Objekt- und Standortbezeichnungnen
	obyte = 0;
	if(ort_idf_flag != 0x80)
	{	bicbus(EEPADR, &obyte, ORT_ADR, 1, BICRD);	// nur das erste Zeichen von ORT wird überprüft
	
		if(obyte >= 'A')			// Ein Ortsname ist eingetragen
		{	ort_idf_flag = 0x80;// ok
				ort_idf = 0;
		}		
		else
			ort_idf = 1;				// Softalarm: OBJ-ORT FEHLT ? generieren
	}

	/* Handschalter für Relaisausgabe registrieren						*/
	/* sw_stat[0-5] enthält die Schalterzustände von SW1-SW6
			Schalterstellung	Zustand			Code
			Links							Hand-Ein		2
			Mitte							Automatik		0
			Rechts						Hand-Aus		1												*/
	
	obyte = Schaltgr1;		//Schalterabfrage in "Inout.c"
	for(i=0; i<3; i++)
	{
		sw_stat[i] = obyte & 0x03;
		obyte >>= 2;
	}

	obyte = Schaltgr2;
	for(i=3; i<6; i++)
	{
		sw_stat[i] = obyte & 0x03;
		obyte >>= 2;
	}

	for(i = 0; i < 6; i++)
	{	
		switch(sw_stat[i])
		{
			case 1:										// Hand AUS
				sw_stat_info[i] = '0';
				break;
			case 2:										// Hand EIN
				sw_stat_info[i] = '1';
				break;
			default:									// Automatik
				sw_stat_info[i] = '.';
				break;
		}
	}			
					
	
	// Signalisierung bei Handbetrieb eines Schalters:
	// 1. Meldung "SCHALTER HAND" wird gesetzt, 
	//		wenn ein Schalter im R36 ODER R37 ODER R33 nicht auf "Auto" steht.
	// 2. Die gelbe LED leutet nur, wenn im R36 einer der 6 Schalter nicht auf "Auto" steht.
	HardHand = 0;
	if( (Schaltgr1 != 0) || (Schaltgr2 != 0) )
	{	HardHand = 1;
		#if LED_DEBUG == 0
		LedGelb  = 1;
		#endif
	}	
	else 
	{	
		#if LED_DEBUG == 0
		LedGelb  = 0;	
		#endif
	}
	
	for(i = 0; i < R37_MODMAX; i++)							// R37_Module
	{	for(j = 0; j < 3; j++)										// je 3 Schalter
		{	HardHand |= mod37[i].sw_stat[j];
			switch(mod37[i].sw_stat[j])
			{
				case 1:
					mod37[i].sw_stat_info[j] = '0';
					break;
				case 2:
					mod37[i].sw_stat_info[j] = '1';
					break;
				default:
					mod37[i].sw_stat_info[j] = '.';
					break;
			}		
		}	
	}	
	
	for(i = 0; i < R33_MODMAX; i++)							// R33_Module
	{	for(j = 0; j < 6; j++)										// je 6 Schalter
		{	HardHand |= mod33[i].sw_stat[j];
			switch(mod33[i].sw_stat[j])
			{
				case 1:
					mod33[i].sw_stat_info[j] = '0';
					break;
				case 2:
					mod33[i].sw_stat_info[j] = '1';
					break;
				default:
					mod33[i].sw_stat_info[j] = '.';
					break;
			}		
		}			
	}	
	
	// Zähler für Schaltpause decr.
	if(schalt_count > 0)
		schalt_count--;

	// Digital- und Analogausgabe
	if(Outp_Sperre ==	0)				// nur wenn	freigegeben
	{
		// Digitalausgabe R36
		Digout = Dout_procedure(&Dout[0], &sw_stat[0], &wpause[0], Digout, 6, dadat);
		
		// Analogausgabe R36
		for(j=0; j<2; j++)
		{	
			if(Aout[j].astat == 0)				// AA_FREI
				anout[j] = aadat[j];				// aadat wird vom Kommando R3X_AADAT gesetzt
			else
			{	// Skalierung
				AoutSk[j] = Gerade_YvonX ( Aout[j].awert, 0, AASkal[j].NullP, 1000, AASkal[j].HundP );
				anout[j]  = proz_bi(AoutSk[j]);
			}	
		}
		
		// Ausgabe an IN/OUT-Modul R37
		for(i = 0; i < 4; i++)										// maximal 4 Module
		{
			if(Projekte[proj_typ].iobl[i].iodev == R37)
			{
				// Digitalausgabe
				n = (char)(Projekte[proj_typ].iobl[i].ionum - 1);					// Gerätenummer
				mod37[n].Digout = Dout_procedure(&mod37[n].Dout[0], &mod37[n].sw_stat[0],
																				 &mod37[n].wpause[0], mod37[n].Digout, 3, mod37[n].dadat);
				
				// Analogausgabe
				for(j=0; j<4; j++)
				{	
					if(mod37[n].Aout[j].astat == 0)							// AA_FREI
						mod37[n].anout[j] = mod37[n].aadat[j];		// aadat wird vom Kommando R3X_AADAT gesetzt
					else
					{
						// Skalierung
						mod37[n].AoutSk[j] = Gerade_YvonX ( mod37[n].Aout[j].awert, 0, mod37[n].AASkal[j].NullP, 1000, mod37[n].AASkal[j].HundP );
						mod37[n].anout[j] = proz_bi(mod37[n].AoutSk[j]);
					}	
				}	
			}
		}

		// Ausgabe an OUT-Modul R33
		for(i = 0; i < 4; i++)										// maximal 4 Module
		{
			if(Projekte[proj_typ].iobl[i].iodev == R33)
			{
				// Digitalausgabe
				n = (char)(Projekte[proj_typ].iobl[i].ionum - 1);					// Gerätenummer
				mod33[n].Digout = Dout_procedure(&mod33[n].Dout[0], &mod33[n].sw_stat[0],
																				 &mod33[n].wpause[0], mod33[n].Digout, 6, mod33[n].dadat);
			}
		}


		oready = TRUE;			
			
	}	
	
	// Zähler-Fehlerbits erkennen und setzen
	for(i=0, mask=0x01; i<8; i++, mask <<= 1 )
	{
		n 		= 0;
		j     = zaehler[i].ucZstatus;
		obyte = zaehler[i].zdim.konfig;

		if((obyte & Z_KONFIG_FEHLER) == Z_KONFIG_FEHLER)
		{
			// es soll ein Fehler erkannt werden
			if( (obyte & Z_KONFIG_KABELBRUCH) == Z_KONFIG_KABELBRUCH)
			{
				if((j & KABELBRUCH) == KABELBRUCH)
					n++;
			}
			
			if((obyte & Z_KONFIG_NAMUR) == Z_KONFIG_NAMUR)
			{
				if((obyte & Z_KONFIG_KURZ) == Z_KONFIG_KURZ)
				{
					if((j & KURZSCHLUSS) == KURZSCHLUSS)
						n++;
				}
			}
			
			if(n!=0)
			{
				zaehler[i].ucZstatus |= FEHLER;
				Zfehler |= mask;
			}
			else
			{
				if((j & FEHLER_GESICHERT) == FEHLER_GESICHERT)
					zaehler[i].ucZstatus &= ((unsigned char)~FEHLER);
			}
		}
	}
	
	// Parametrierbarer Triggerpunkt[%] umwandeln in ADU-Format,(944 entsprechen 100% DIGITS)
	// wird benötigt in "InOut.c" zur Kontaktbewertung 
	trigger_R56   = (unsigned int)((unsigned long)944 * Trigpunkt_R56 / 100);		
	trigger_NAMUR = (unsigned int)((unsigned long)944 * Trigpunkt_NAMUR / 100);
	kabeltrigger  = (unsigned int)((unsigned long)944 * Kabelpunkt / 100);			
	kurztrigger   = (unsigned int)((unsigned long)944 * Kurzpunkt / 100);				
	
			
}
/*---------------------- Task Ouput Ende -----------------------------------*/


// Unterprogramm: Zusammenstellen des Ausgabebytes
// maximal 8 Relais !
char Dout_procedure(doutsp *Ausgabe, char *schalter, char *pause, char Digold, char anz, char dadat)
{
		char obyte, mask, i, j;

		obyte				=	0;
		mask				= 1;
		for(i=0; i<anz;	i++)
		{
			if(schalter[i] == 0)							// wenn Schalter auf Automatik
			{			
				switch(Ausgabe[i].func)					// Funktion untersuchen
				{
					case STATISCH:
						if(Ausgabe[i].wert > 0)					// Soll was ausgegeben werden ?
						{																
							if((Digold & mask) == mask)  	// Alter Zustand auch EIN ?
							{
								obyte |= mask;							// ja, nur Ausgangsbit aktivieren
							} 
							else													// Alter Zustand war AUS
							{
								if(schalt_count == 0)				// Schaltpause abgelaufen ?
								{
									obyte |= mask;						// ja, Ausgangsbit aktivieren
									schalt_count = Schalt_Pause;
								}
							}
						}		
						break;
						
					case DYNAMISCH:							
						if(Ausgabe[i].wert > 0)			// Soll was ausgegeben werden ?
						{	obyte |= mask;						// ja, Ausgangsbit aktivieren
							Ausgabe[i].wert -= 1;			// Wert decrementieren
						}		
						break;
							
					case VERRIEG1:
						if(Ausgabe[i].wert > 0)			// Soll was ausgegeben werden ?
						{	if(Ausgabe[i+1].wert > 0)	// ist der andere Ausgang auch aktiv ?
							{	//beide Ausgänge sind aktiviert, welcher ist der neuere ?
								if((Digold & mask) == 0)
									j = 1;								// VERRIEG1 ist neu, VERRIEG2 löschen			
								else
									j = 0;								// VERRIEG2 ist neu, VERRIEG1 löschen
								Ausgabe[i+j].wert = 0;	// älteren Ausgabewert löschen
								pause[i] = 1;						// Wechsel kennzeichnen
							}	
							else											// nur VERRIEG1 ist aktiv
							{	if(pause[i] > 0)
									pause[i]--;						// noch nichts ausgeben
								else
								{	obyte |= mask;				// Ausgangsbit aktivieren
									Ausgabe[i].wert -= 1;	// Wert decrementieren
								}
							}
						}						
						break;
								
					case VERRIEG2:
						if(Ausgabe[i].wert > 0)			// Soll was ausgegeben werden ?
						{	if(pause[i-1] > 0)
								pause[i-1]--;						// noch nichts ausgeben
							else
							{	obyte |= mask;					// Ausgangsbit aktivieren
								Ausgabe[i].wert -= 1;		// Wert decrementieren
							}
						}			
						break;

					case 0:												// DA_FREI
						if( dadat & mask )					// dadat wird vom Kommando R3X_DADAT gesetzt
							obyte |= mask;
						break;	 	
				} 
			}		
			else														// wenn Schalter nicht auf Automatik
			{
				if(schalter[i] == 2)					// wenn Schalter auf Hand Ein
					obyte |= mask;
			}		
			mask <<= 1;
		}	//for(i=0; i<6;	i++)			 
 
 	return(obyte);
}
 	