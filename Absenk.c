/*	Task: Absenk							*/

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "unoinitext.h"
#include "standdef.h"
#include "userdef.h"	
#include <string.h>

// Unterprogramme
void  absen(char abx);
UINT  get_aufheizz(int ta, int tvab);

// #####ulsch : Ferienprogramm #####
void TestFerien ( char hk );
void TestSchalter(char hk);

/*---------------------- Task Absenk -----------------------------------*/
// Gemeinsamer Code Absenkung f�r bis zu 4 alternierende Regelkreise
// Zeitbasis f�r einen Absenk-Regelkreis : 10,0 Sekunden
// Zeitbasis f�r den Taskaufruf:				   10,0 Sekunden / HKMAX

void Absenk(void)
{
	char i;
	
	if(++abx >= HKMAX)								// Regelkreis-Index erh�hen
		abx = 0;
	
	// Projektabfrage, ob Absenk-Regelkreis aktiv
	if(Projekte[proj_typ].abscode[abx] == 0)
		return;													// Absenkung Heizkreis nicht aktiv

	
	// Abfrage: Absenkprofil "Woche" auf alle Tage kopieren ?
	if(setwoche[abx] == 1)
	{	for(i = 0; i < 7; i++)
			memcpy( (void *)&(abs_tab[abx][i+1].begin) , (void *)&(abs_tab[abx][0].begin), ABSTABLENG);
		setwoche[abx] = 0;
	}

	// Test auf Datenerhalt im Noinit-Rambereich
	if(abs_ram[abx].ramex != RAMEX)
	{	 clear_abs_ram(abx);							// L�sche Absenk-Arbeitsbereich
		 return;													// Ende
	}

	
	TestFerien(abx);				
	TestSchalter(abx );				
	
	// Absenkung ermitteln (f�llen des Absenk-Arbeitsbereiches)
	// Neue Werte entstehen nur beim Reinlaufen in einen neuen Absenkzeitbeginn
	absen(abx);		
	
	if(abs_tab[abx][Wotag].dauer == 0)	// wenn heute keine Absenkung,
	{	clear_abs_ram(abx);								// L�sche Absenk-Arbeitsbereich										
		return;														// Ende
	}
	
	if(abs_ram[abx].zasd > 0)												// Z�hler Absenkzeit noch l�uft noch
	{	abs_ram[abx].zasd--;
		
		if(TI[abx]->stat == 0)												// Raumtemperaturf�hler ok ?
		{	if(TI[abx]->messw <= hks[abx].Timi - hks[abx].TimiHyst )					// kleiner als Mindesttemp.- Hysterese ?
				abs_ram[abx].stuetz = 1;									// Zustand St�tzbetrieb setzen (f�r RegelHk.c)
			else
			{	if(TI[abx]->messw >= hks[abx].Timi )
					abs_ram[abx].stuetz = 0;								// Zustand St�tzbetrieb l�schen
			}
		}
		else																					// wenn keine Raumtemperatur vorhanden oder F�hler defekt,
			abs_ram[abx].stuetz = 0;										// Zustand St�tzbetrieb immer l�schen
			
		// Heizunterbrechung ?
		if(abs_ram[abx].hunt == 1)
			abs_ram[abx].zAbsch = 10;										// Abschaltz�hler nachladen
		
		// Abschaltphase ?	
		if ( abs_ram[abx].zAbsch > 0 )
	 	{
 	 		abs_ram[abx].zAbsch--;
	 		abs_ram[abx].absch = 1;											// Zustand Abschaltphase setzen

			// Frostgefahr in der Abschaltphase
			if(abs_ram[abx].stuetz == 1)								// im St�tzbetrieb
				abs_ram[abx].frost = 0;										// Zustand Frostschutzbetrieb immer l�schen
			else
			{
				if(ta1m.messw < hks[abx].TaFrostgrenze)
					abs_ram[abx].frost = 1;									// Zustand Frostschutzbetrieb setzen (f�r RegelHk.c)
				else if(ta1m.messw > hks[abx].TaFrostgrenze)
					abs_ram[abx].frost = 0;									// Zustand Frostschutzbetrieb l�schen
			}		
		}
		else
	 	{	abs_ram[abx].absch = 0;											// Zustand Abschaltphase l�schen
			abs_ram[abx].frost = 0;											// Zustand Frostschutzbetrieb l�schen
		}
		
	}
	else																// Absenkzeit ist abgelaufen
	{	
		abs_ram[abx].tvab		= 0;					// Absenkwert l�schen
		abs_ram[abx].absen	= 0;					// Zustand  Absenkphase l�schen					
	 	abs_ram[abx].absch 	= 0;					// Zustand 	Abschaltphase l�schen
		abs_ram[abx].hunt		= 0;					// Zustand  Heizunterbrechung l�schen
		abs_ram[abx].stuetz = 0;					// Zustand 	St�tzbetrieb l�schen
		abs_ram[abx].frost 	= 0;					// Zustand	Frostschutzbetrieb l�schen
		
		if(abs_ram[abx].zahd == 0)				// Z�hler 	Aufheizzeit abgelaufen ?
			 abs_ram[abx].aufhe = 0;				// Zustand  Aufheizphase l�schen		
		else
		{	abs_ram[abx].zahd--;
			abs_ram[abx].aufhe = 1;					// Zustand  Aufheizphase setzen	
		}
	}
		
}
/*---------------------- Ende Task Absenk -----------------------------------*/


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 		L�schen der Absenk-Arbeitsbereiches eines Heizkreises				*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//	IN:	Heizkreis (HK1..HK2)
void clear_abs_ram(char hk)
{
	char *adr;
	char i;
	
	adr = (char *)&abs_ram[hk].ramex;			// Adresse des 1. Parameters
	
	for(i = 0; i < ABSRAMLENG; i++)				// Bereich l�schen
		*adr++	= 0;
	
	abs_ram[hk].ramex = RAMEX;						// Kennzeichen eintragen
}	


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 										Absenkung ermitteln												 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//	IN:	Heizkreis (HK1..HK2)	
//	Zeitbasis ist der 10-Sekunden-Aufruf des Absenk-Task's (360 pro h)
void absen(char hk)
{
	UINT  aufheizz;			// Aufheizzeit aus Tabelle
	
//##### 110720 ulsch
	if ( ta1m.messw < -150 )			// wenn kleiner als -15 �C, keine Absenkung
	{															// neu: laufende Absenkung abbrechen
		abs_ram[hk].tvab = 0;		
		abs_ram[hk].zasd = 0;
		abs_ram[hk].zAbsch = 0;
		abs_ram[hk].zahd = 0;		
		return;
	}

	// Test auf Absenkdauer
	if(abs_tab[hk][Wotag].dauer == 0)
		return;												// Heute keine Absenkung
	else
	{	// Test auf Absenkbeginn
		if(abs_tab[hk][Wotag].begin_h != Std)
			return;
		if(abs_tab[hk][Wotag].begin != Min)
			return;
		else
		{ // Absenkbeginn erreicht
			// Au�entemperatur betrachten
//-			if(ta1m.messw < -150)			// wenn kleiner als -15 �C,
//-				return;									// keine Absenkung
			
			// Schnellaufheizung ?
			if(hks[hk].Sauf == 0)			// wenn nicht gesetzt,
				aufheizz = 0;						// prozentuale Aufheizzeit = 0
			else
			{	// Schnellaufheizung gew�nscht
				if(ta1m.messw > 100)		// wenn Ta gr��er als +10 �C,
					aufheizz = 0;					// prozentuale Aufheizzeit = 0
				else
				{	// prozentuale Aufheizzeit ermitteln (360 = 100%)
					aufheizz = get_aufheizz(ta1m.messw, abs_tab[hk][Wotag].abwert); 
				}
			}
			// Laden des Zeitz�hlers f�r Absenken
			abs_ram[hk].zasd = (360 - aufheizz) * abs_tab[hk][Wotag].dauer;
			
			// Laden des Zeitz�hlers f�r Aufheizen
			abs_ram[hk].zahd = aufheizz * abs_tab[hk][Wotag].dauer;
			
			// Absenkwert in den Arbeitsbereich laden
			abs_ram[hk].tvab = abs_tab[hk][Wotag].abwert;
			if(abs_ram[hk].tvab >= 500)			// gr��er 50,0 K
				abs_ram[hk].hunt = 1;					// Schalter Heizunterbrechung setzen
			
			// Zustandsbyte setzen
			abs_ram[hk].aufhe = 0;					// keine Aufheizphase
			abs_ram[hk].absen = 1;					// Absenkphase

			// Abschaltdauer ermitteln
			if ( ta1m.messw <= hks[hk].TaFrostgrenze	)
				abs_ram[hk].zAbsch = 0;					// bei Frost nicht abschalten
			else
			{
				abs_ram[hk].zAbsch = (UINT)(ta1m.messw - hks[hk].TaFrostgrenze) * hks[hk].AbschZeitKonst * 36 / 10;
			}
			
			// Abschaltzeit begrenzen durch Absenkdauer
			if ( abs_ram[hk].zAbsch > abs_ram[hk].zasd )
				abs_ram[hk].zAbsch = abs_ram[hk].zasd;

		}
	}
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Ermitteln der prozentualen Aufheizzeit aus der Tabelle tabse		*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//	IN:		Au�entemp.	[�C] * 10
//				Absenkwert	[K]  * 10
//	OUT:	Aufheizzeit	[%]  * 3,6    (360 = 100%)
UINT get_aufheizz(int ta, int tvab)
{
	char i, j;
	int wert;
	
	// Index f�r Tabelle ermitteln
	wert = -100;							// ta -10,0 �C
	for(i = 0; i < 6; i++)		// 6 Temperaturkurven
	{	if(ta < wert)
			break;
		else
			wert += 50;						// 5,0 �C
	}

//#####090312 UlSch auf Wunsch von ThoSi:
// unver�nderte Tabelle, aber bei Absenkungen < 20 K mehr St�tzpunkte
//-	wert = 200;								// 	tvab <= 20,0 K		
	wert = 50;								// 	tvab <= 5,0 K		
	for(j = 0; j < 5; j++)		// 5 Absenkwerte
	{	if(tvab <= wert)
			break;
		else
//-			wert += 100;					// 10,0 K
			wert += 50;					// 5,0 K
	}
	
//#####101208 ulsch Korrektur, weil sich bei der �nderung 090312 ein Fehler eingeschlichen hat
// ( j = 5 wenn tvab = 50.0, damit Zugriff auf den ersten Wert der n�chsten Zeile von tabse[] )
	if ( i > 5 )
		i = 5;
	if ( j > 4 )
		j = 4;
		
	return(tabse[i][j]);
}					
		
		
// #####ulsch : Ferienprogramm #####
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 										Ferienprogramm    												 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/		

/* bei Neustart und zu Beginn jeder Stunde wird getestet, ob Ferien sind */
void TestFerien ( char hk )
{

#if FER == 1

	ULONG aktDatum;
	static char testWar = 0xFF;
	static char ferienTag = 0;
	char i;

	if ( Min == 59 )
		testWar = 0;
	if ( testWar == 0xFF || ( Min == 0 && testWar == 0 ) )
	{
		aktDatum = Jahr * 10000 + Mon * 100 + Tag;		
		for ( i = 0; i < 10; i++ )
		{
			if ( ( aktDatum >= ferien[i].ferienBeg.jahr * 10000 +	ferien[i].ferienBeg.monat * 100 + ferien[i].ferienBeg.tag ) && 
					 ( aktDatum <= ferien[i].ferienEnd.jahr * 10000 +	ferien[i].ferienEnd.monat * 100 + ferien[i].ferienEnd.tag ) )
			{
				ferienTag = 1;
				break;
			}
		}
		if ( i > 9 )
			ferienTag = 0;					
		testWar = 1;
	}

	if ( (FERINP[0]->bstat == 0 && FERINP[0]->bwert == 1) || ferienTag == 1)
		ferienStat = 1;																	// es sind Ferien, aber noch nicht Ferienbetrieb des Heizkreises
	else 
		ferienStat = 0;
			
	if ( hks[hk].FerienAutom && ferienStat == 1 && hks[hk].FerienAbsenk > 0)
	{
		hkd[hk].absenFerien = 1;												// Zustand  Absenkphase setzen ( = Ferienbetrieb )
		hkd[hk].tvabFerien = hks[hk].FerienAbsenk;			// Absenkwert laden
		
		if(TI[hk]->stat == 0)														// Raumtemperaturf�hler ok ?
		{	if(TI[hk]->messw <= hks[hk].Timi - hks[hk].TimiHyst )		// kleiner als Mindesttemp.- Hysterese ?
				hkd[hk].stuetzFerien = 1;										// Zustand St�tzbetrieb setzen (f�r RegelHk.c)
			else
			{	if(TI[hk]->messw >= hks[hk].Timi )
					hkd[hk].stuetzFerien = 0;									// Zustand St�tzbetrieb l�schen
			}
		}
		else																						// wenn keine Raumtemperatur vorhanden oder F�hler defekt,
			hkd[hk].stuetzFerien = 0;											// Zustand St�tzbetrieb immer l�schen
			
		if(hks[hk].FerienAbsenk >= 500)									// >= 50,0K Abschaltbetrieb
		{
			hkd[hk].abschFerien = 1;											// Abschaltbetrieb setzen

			// Frostgefahr in der Abschaltphase
			if(hkd[hk].stuetzFerien == 1)									// im St�tzbetrieb,
				hkd[hk].frostFerien = 0;										// Zustand Frostschutzbetrieb immer l�schen
			else
			{
				if(ta1m.messw < hks[hk].TaFrostgrenze)
					hkd[hk].frostFerien = 1;									// Zustand Frostschutzbetrieb setzen (f�r RegelHk.c)
				else if(ta1m.messw > hks[hk].TaFrostgrenze)
					hkd[hk].frostFerien = 0;									// Zustand Frostschutzbetrieb l�schen
			}
		}
		else
	 	{	hkd[hk].abschFerien = 0;											// Zustand Abschaltphase l�schen
			hkd[hk].frostFerien = 0;											// Zustand Frostschutzbetrieb l�schen
		}
	}		// kein Ferienbetrieb
	else
	{
		hkd[hk].tvabFerien 		= 0;											// Ferienabsenkert l�schen
		hkd[hk].absenFerien		= 0;											// Zustand  Absenkphase ( = Ferienbetrieb ) l�schen					
	 	hkd[hk].abschFerien 	= 0;											// Zustand 	Abschaltphase l�schen
		hkd[hk].stuetzFerien 	= 0;											// Zustand 	St�tzbetrieb l�schen
		hkd[hk].frostFerien 	= 0;											// Zustand	Frostschutzbetrieb l�schen
	}
							
#else
	ferienStat = 0;
#endif
		
	
	return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 										Absenkung durch Anlagenschalter						 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/		
void TestSchalter(char hk)
{

	if( HKAUS[hk]->bstat == 0 && HKAUS[hk]->bwert == 1 && hks[hk].SchalterAbsenk > 0)
	{
		hkd[hk].absenSchalter = 1;												// Zustand  Absenkphase setzen ( = Schalterbetrieb )
		hkd[hk].tvabSchalter = hks[hk].SchalterAbsenk;		// Absenkwert laden
		
		if(TI[hk]->stat == 0)														// Raumtemperaturf�hler ok ?
		{	if(TI[hk]->messw <= hks[hk].Timi - hks[hk].TimiHyst )					// kleiner als Mindesttemp.- Hysterese ?
				hkd[hk].stuetzSchalter = 1;										// Zustand St�tzbetrieb setzen (f�r RegelHk.c)
			else
			{	if(TI[hk]->messw >= hks[hk].Timi )
					hkd[hk].stuetzSchalter = 0;									// Zustand St�tzbetrieb l�schen
			}
		}
		else																						// wenn keine Raumtemperatur vorhanden oder F�hler defekt,
			hkd[hk].stuetzSchalter = 0;											// Zustand St�tzbetrieb immer l�schen
			
		if(hks[hk].SchalterAbsenk >= 500)									// >= 50,0K Abschaltbetrieb
		{
			hkd[hk].abschSchalter = 1;											// Abschaltbetrieb setzen

			// Frostgefahr in der Abschaltphase
			if(hkd[hk].stuetzSchalter == 1)									// im St�tzbetrieb,
				hkd[hk].frostSchalter = 0;										// Zustand Frostschutzbetrieb immer l�schen
			else
			{
				if(ta1m.messw < hks[hk].TaFrostgrenze)
					hkd[hk].frostSchalter = 1;									// Zustand Frostschutzbetrieb setzen (f�r RegelHk.c)
				else if(ta1m.messw > hks[hk].TaFrostgrenze)
					hkd[hk].frostSchalter = 0;									// Zustand Frostschutzbetrieb l�schen
			}
		}
		else
	 	{	hkd[hk].abschSchalter = 0;											// Zustand Abschaltphase l�schen
			hkd[hk].frostSchalter = 0;											// Zustand Frostschutzbetrieb l�schen
		}
	}		// kein Schalterbetrieb

	else if ( FRGHK[hk]->bstat == 0 && FRGHK[hk]->bwert == 0 ) 
	{
		hkd[hk].absenSchalter = 1;												// Zustand  Absenkphase setzen ( = Schalterbetrieb )
		hkd[hk].abschSchalter = 1;												// Abschaltbetrieb setzen

		// Frostgefahr in der Abschaltphase
		if ( ta1m.messw < hks[hk].TaFrostgrenze )
			hkd[hk].frostSchalter = 1;											// Zustand Frostschutzbetrieb setzen (f�r RegelHk.c)
		else if ( ta1m.messw > hks[hk].TaFrostgrenze )
			hkd[hk].frostSchalter = 0;											// Zustand Frostschutzbetrieb l�schen
	}

	else
	{
		hkd[hk].tvabSchalter 		= 0;											// Schalterabsenkert l�schen
		hkd[hk].absenSchalter		= 0;											// Zustand  Absenkphase ( = Schalterbetrieb ) l�schen					
	 	hkd[hk].abschSchalter 	= 0;											// Zustand 	Abschaltphase l�schen
		hkd[hk].stuetzSchalter 	= 0;											// Zustand 	St�tzbetrieb l�schen
		hkd[hk].frostSchalter 	= 0;											// Zustand	Frostschutzbetrieb l�schen
	}

}

	 