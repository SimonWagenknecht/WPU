/*	Task: Bedarf

		Untersucht die Meldungen vom RIEcon50 hinsichtlich
		- Absenkung und Abschaltung bei 'Kein Bedarf' 
		- �berwachung der Mindesttemperatur
*/

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "unoinitext.h"

void check_absen(char bex);
void clear_abs_bed(char hk);

/*---------------------- Task Bedarf -----------------------------------*/
// Gemeinsamer Code: Bedarf f�r bis zu 4 alternierende Regelkreise
// Zeitbasis f�r einen Bedarf-Regelkreis : 10,0 Sekunden
// Zeitbasis f�r den Taskaufruf:					 10,0 Sekunden / HKMAX
void Bedarf(void)
{
	char rtOK;
		
	if(++bex >= HKMAX)																	// Index erh�hen
		bex = 0;
	
	// Projektabfrage, ob Absenk-Regelkreis aktiv
	if(Projekte[proj_typ].abscode[bex] == 0)
		return;																						// Absenkung Heizkreis nicht aktiv

	// Test auf Datenerhalt im Noinit-Rambereich
	if(abs_bed[bex].ramex != RAMEX)
	{	 clear_abs_bed(bex);															// L�sche Absenk-Arbeitsbereich
		 return;																					// Ende
	}

	// Absenkung ermitteln ( �berpr�fung der Vorrangschaltung und f�llen des Absenk-Arbeitsbereiches,
	//											 �berpr�fen auf Ende der Absenkzeit: "BEDARF JA")
	check_absen(bex); 
	
	// Absenk- und Abschaltphasen testen
	//--------------------------------------------------
	if(abs_bed[bex].zasd > 0)														// Z�hler Absenkzeit noch l�uft noch
	{	abs_bed[bex].zasd--;															// wird bei BEDARF NEIN in check_absen(bex) nachgetriggert,
																											// l�uft ab bei BEDARF_UNBEKANNT (wirkt als �berbr�ckungsz�hler bei Datenausfall)

	 	if( abs_bed[bex].zAbsch > 0)											// Z�hler Abschaltzeit wird immer decrementiert
	 	 		abs_bed[bex].zAbsch--;
		

		// Test auf Raummindesttemperatur-Unterschreitung
		// �bertragene Solltemp. ist die Spartemperatur
		if(hkd[bex].tibed > 10 &&  hkd[bex].tisobed > 10)	// nur wenn Soll- und Ist-Temp. > 1,0 �C ( sonst Fehler )
		{	
			rtOK = TRUE;																		// Raumtemperaturen ok
			if(hkd[bex].tibed <= hkd[bex].tisobed - hks[bex].TimiHyst)						// kleiner als Mindesttemp.- Hysterese ?
				abs_bed[bex].stuetz = 1;											// Zustand St�tzbetrieb setzen (f�r RegelHk.c)
			else
			{	if(hkd[bex].tibed >= hkd[bex].tisobed)
					abs_bed[bex].stuetz = 0;										// Zustand St�tzbetrieb l�schen
			}
		}
		else																							// Fehler Raumtemperatur
		{
			rtOK = FALSE;
			abs_bed[bex].stuetz = 0;												// Zustand St�tzbetrieb immer l�schen
		}

		// Heizunterbrechung ?  Raumtemp. ok ?
		if(abs_bed[bex].hunt == 1 || rtOK == TRUE)
			abs_bed[bex].zAbsch = 10;												// Abschaltz�hler nachladen
		
		// Abschaltphase ?	
		if ( abs_bed[bex].zAbsch > 0 )
	 	{
	 		abs_bed[bex].absch = 1;													// Zustand Abschaltphase setzen

			// Frostgefahr in der Abschaltphase
			if(abs_bed[bex].stuetz == 1)										// im St�tzbetrieb kann der
				abs_bed[bex].frost = 0;												// Zustand Frostschutzbetrieb immer gel�scht werden
			else
			{	
				if(ta1m.messw < hks[bex].TaFrostgrenze)
					abs_bed[bex].frost = 1;											// Zustand Frostschutzbetrieb setzen (f�r RegelHk.c)
				else if(ta1m.messw > hks[bex].TaFrostgrenze)
					abs_bed[bex].frost = 0;											// Zustand Frostschutzbetrieb l�schen
			}
		}
		else
	 	{	abs_bed[bex].absch = 0;													// Zustand Abschaltphase l�schen
			abs_bed[bex].frost = 0;													// Zustand Frostschutzbetrieb l�schen
		}
	}
	else		// Absenkzeit ist abgelaufen ( entweder durch Ablauf des �berbr�ckungsz�hlers oder durch BEDARF JA )
	{	
		abs_bed[bex].tvab		= 0;													// Absenkwert l�schen
		abs_bed[bex].absen	= 0;													// Zustand  Absenkphase l�schen					
	 	abs_bed[bex].absch 	= 0;													// Zustand 	Abschaltphase l�schen
		abs_bed[bex].hunt		= 0;													// Zustand  Heizunterbrechung l�schen
		abs_bed[bex].stuetz = 0;													// Zustand 	St�tzbetrieb l�schen
		abs_bed[bex].frost 	= 0;													// Zustand	Frostschutzbetrieb l�schen

		// Schnellaufheizung testen
		if(abs_bed[bex].zahd > 0)													// Z�hler Aufheizzeit ist gesetzt (Schalter Schnellaufheizung == 1)
		{	abs_bed[bex].zahd--;														// wird bei BEDARF JA in check_absen(bex) nachgetriggert,                      
			                      													// l�uft ab bei BEDARF_UNBEKANNT (wirkt als �berbr�ckungsz�hler bei Datenausfall)

			if(hkd[bex].vorheiz_bed == 1)										// wenn Heizungsbedarf nur durch R�ume in Vorheizung, ermittelt in WR_KASKADE
				abs_bed[bex].aufhe	= 1;											// Zustand  Aufheizphase setzen
			else                                          	
				abs_bed[bex].aufhe	= 0;											// Zustand  Aufheizphase l�schen
		}
		else
			abs_bed[bex].aufhe	= 0;												// Zustand  Aufheizphase l�schen
	}	
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 										Absenkung ermitteln												 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void check_absen(char hk)
{

//##### 110720 ulsch
	if ( ta1m.messw < -150 )			// wenn kleiner als -15 �C, keine Absenkung
	{															// neu: laufende Absenkung abbrechen
		abs_bed[hk].tvab = 0;		
		abs_bed[hk].zasd = 0;
		abs_bed[hk].zAbsch = 0;
		abs_bed[hk].zahd = 0;		
		return;
	}

	// Vorrangfeststellung: Bedarfsabsenkung oder Zeitabsenkung (wird auch in RegelHk.c ben�tigt)
	if(hks[hk].Beabs == 0)										// Kein Bedarfs-Absenkwert eintragen ?
	{	
		hkd[hk].vorra_bed = FALSE;							// Kein Vorrang vor Zeitabsenkung
		clear_abs_bed(hk);											// L�sche Absenkbereich
	}
	else																			// Absenkwert ist eingetragen
	{	// F�r die Vorrangfeststellung die Daten�bertragung testen
		if ( hks[hk].VorrangZeitabsenkung && abs_ram[hk].absen )
			hkd[hk].vorra_bed = FALSE;						// Zeitabsenkung dominiert, Bedarfsmeldung wird ignoriert		
		
		else if((hkd[hk].bedarf != BEDARF_UNBEKANNT))
			hkd[hk].vorra_bed = TRUE;							// Vorrang vor Zeitabsenkung
		else
			hkd[hk].vorra_bed = FALSE;						// Kein Vorrang vor Zeitabsenkung, wenn Daten�bertragung unklar
	}		
	
	if(hkd[hk].vorra_bed == TRUE)							// Daten�bertragung i.O.
	{	
		// wenn der R50 keine Raumtemp. �bertr�gt (z.B. Dummy-Raum), sondern nur den Nutzungszustand, aber ein Raumf�hler	
		// f�r diesen Heizkreis auf den R36 aufgeschaltet ist, wird der Messwert des F�hlers benutzt, wenn sein Status ok
		if(hkd[hk].ticopy == 1)									// ticopy wird in WR_KASKADE gebildet 
		{ 
			if(TI[hk]->stat == 0)	
				hkd[hk].tibed = TI[hk]->messw;
			else
				hkd[hk].tibed = 0;
		}			

		// Initialisierung durchf�hren, nur wenn noch keine Absenkphase l�uft
		if(abs_bed[hk].absen == FALSE)
		{
			if(hkd[hk].bedarf == BEDARF_NEIN)			// jetzt Absenkwerte laden, wenn "Kein Bedarf"
			{
				// Au�entemperatur betrachten
				if(ta1m.messw < -150)								// wenn kleiner als -15 �C,
					;																	// keine Absenkung
				else
				{
					// Laden des Zeitz�hlers f�r Absenken
					abs_bed[hk].zasd = 360;						// 3600/10 = 1 h �berbr�ckungszeit f�r Absenken
					// Das Bedarfs-Absenkprogramm l�uft bei Unterbrechung der Daten�bertragung mindestend noch 1 Stunde weiter,
					// obwohl die Zeitabsenkung solange die Regie �bernommen hat. Bei Wiederkehr der Daten�bertragung geht die
					// Bedarfsabsenkung nahtlos weiter. 
										
					abs_bed[hk].zahd = 0;							// Zeitz�hler f�r Aufheizen l�schen
					
					// Absenkwert in den Arbeitsbereich laden
					abs_bed[hk].tvab = hks[hk].Beabs;	// Bedarfs-Absenkwert eintragen
					if(abs_bed[hk].tvab >= 500)				// gr��er 50,0 K
						abs_bed[hk].hunt = 1;						// Schalter Heizunterbrechung setzen
					
					// Zustandsbyte setzen
					abs_bed[hk].aufhe = 0;						// keine Aufheizphase
					abs_bed[hk].absen = 1;						// Absenkphase
		
					// Abschaltdauer ermitteln
					if ( ta1m.messw <= hks[hk].TaFrostgrenze	)
						abs_bed[hk].zAbsch = 0;					// bei Frost nicht abschalten
					else
					{
						abs_bed[hk].zAbsch = (UINT)(ta1m.messw - hks[hk].TaFrostgrenze) * hks[hk].AbschZeitKonst * 36 / 10;
					}
				}	
			}
			else	// BEDARF_JA
			{
				abs_bed[hk].zasd = 0;								// Zeitz�hler f�r Absenken l�schen
				abs_bed[hk].zAbsch = 0;							// Zeitz�hler f�r Abschalten l�schen
				
				// Schnellaufheizung ?
				if(hks[hk].Sauf == 0)								// wenn nicht gesetzt
					abs_bed[hk].zahd = 0;							// Zeitz�hler f�r Aufheizen l�schen
				else
				{	// Laden des Zeitz�hlers f�r Aufheizen
					abs_bed[hk].zahd = 360;						// 3600/10 = 1 h �berbr�ckungszeit f�r Aufheizen nachtriggern
				}
			}	
		}
		else	// Absenkphase l�uft
		{
			if(hkd[hk].bedarf == BEDARF_JA)				// �berpr�fen auf Ende der Absenkzeit
			{
				abs_bed[hk].zasd = 0;								// Zeitz�hler f�r Absenken l�schen
				abs_bed[hk].zAbsch = 0;							// Zeitz�hler f�r Abschalten l�schen
			}
			else	// BEDARF NEIN
			{	
				abs_bed[hk].zasd = 360;							// Zeitz�hler f�r Absenken (�berbr�ckungszeit) retriggern
			}					
		}		
	}

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 		L�schen der Absenk-Arbeitsbereiches eines Heizkreises				*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//	IN:	Heizkreis
void clear_abs_bed(char hk)
{
	char *adr;
	char i;
	
	adr = (char *)&abs_bed[hk].ramex;			// Adresse des 1. Parameters
	
	for(i = 0; i < ABSRAMLENG; i++)				// Bereich l�schen
		*adr++	= 0;
	
	abs_bed[hk].ramex = RAMEX;						// Kennzeichen eintragen
}	
	
				