/*	Task: Alarme		*/

#include "sfr32c87.h"
#include "defins.h"
#include "struct.h"
#include "ustruct.h"
#include "constext.h"
#include "uconstext.h"
#include "sramext.h"
#include "uramext.h"
#include "snoinitext.h"
#include "unoinitext.h"
#include "konvform.h"
#include "standdef.h"
#include "userdef.h"
#include <stdlib.h>
#include <string.h>

void (*prozadr)(void);	 

void PumpenAus(void);
void quit_proc(void);
void AlarmFunktion(char alfunc);

/*---------------------------------- Task Alarme -------------------------------*/
void Alarme(void)
{
	char i, j, abyte, save_mode; 
	char gnum, pnum, konv, attr, alclass, alfunc;
	char    *pADR;
	mwsp 		*pMWSP;
	dinpsp	*pDINPSP;
	UINT dae;
	static char secLad[2] = {0,0};
	static char ladZuletzt[2] = {0,0};
	
	gnum = pnum = konv = attr = alclass = alfunc	= 0;
	pADR = 0;

	// ***** josch: Datenmanager
	#if DS_MODE == 1		// Gerät im DS_Modus (Data Slave),	Auswertung in der Funktion DReply() 
	DReply();
	#endif
	
	
	#if HKANZ > 0
	// ##### ulsch : bei laenger andauernder grosser Regelabweichung wird ein Alarm generiert
	for ( i = 0; i < HKANZ; i++ )														// Alarme laeuft jede Sekunde
	{																												// nicht bei Hand, Sommerbetrieb und wenn nicht parameriert
		if ( hks[i].Haut == 0 && hkd[i].somme == 0 && hks[i].TabwMax && hks[i].TabwZeit && ( abs( hkd[i].ei ) > hks[i].TabwMax ) )
		{
			if ( ++hkd[i].abwCtr > ( hks[i].TabwZeit*60 ) )			// Counter in Sekunden, Parameter in Minuten
				hkd[i].abwAlarm = 1;
		}
		else
		{
			hkd[i].abwAlarm = 0;						
			hkd[i].abwCtr = 0;
		}
		// Solange Alarm ansteht, Counter rücksetzen; weil nach einer Quittierung nur der Alarm gelöscht wird. 
		if(hkd[i].abwAlarm == 1)
			hkd[i].abwCtr = 0;
	}
	#endif		// #if HKANZ > 0	

	#if REGEL_WW1 == 1
		#if RCOND_WW1 == 1
		if ( wws[WW1].Haut == 0 && wws[WW1].TabwMax && wws[WW1].TabwZeit && ( abs( wwd[WW1].ei ) > wws[WW1].TabwMax ) 
										&& wwd[WW1].wwlad )
		#else 
			#if RCOND_WW1 == 2
			if ( wws[WW1].Haut == 0 && wws[WW1].TabwMax && wws[WW1].TabwZeit && ( abs( wwd[WW1].ei ) > wws[WW1].TabwMax ) 
										&& PUWZEA[WW1]->wert )
			#else		// RCOND_WW1 == 0
				if ( wws[WW1].Haut == 0 && wws[WW1].TabwMax && wws[WW1].TabwZeit && ( abs( wwd[WW1].ei ) > wws[WW1].TabwMax ) )
			#endif
		#endif																			
		{
			if ( ++wwd[WW1].abwCtr > ( wws[WW1].TabwZeit * 60 ) )
				wwd[WW1].abwAlarm = 1;
		}
		else
		{
			wwd[WW1].abwAlarm = 0;						
			wwd[WW1].abwCtr = 0;
		}
	
	// Solange Alarm ansteht, Counter rücksetzen; weil nach einer Quittierung nur der Alarm gelöscht wird. 
	if(wwd[WW1].abwAlarm == 1)
		wwd[WW1].abwCtr = 0;
		
	#endif	

#if WWANZ > 0 
	#if ( ZPU_WW1 == 1 || PU_BUS_ZPU_WW1 > 0 )
	if ( wws[WW1].Haut == 0 && wws[WW1].TempWWminZeit > 0 && wwd[WW1].zirkPu == 1
																													&& TWWV[WW1]->stat == 0 && TWWV[WW1]->messw <= wws[WW1].TempWWmin )
	{
		if ( ++wwd[WW1].wwMinCtr > ( wws[WW1].TempWWminZeit * 60 ) )
			wwd[WW1].tempAlarm = 1;
	}
	else
	{
		wwd[WW1].tempAlarm = 0;						
		wwd[WW1].wwMinCtr = 0;
	}
	
	if ( wwd[WW1].tempAlarm == 1 )
		wwd[WW1].wwMinCtr = 0;
	#endif

	#if ( SP_WW1 == 1 && ( LPU_WW1 == 1 || PU_BUS_LPU_WW1 > 0 ) )
	if ( wws[WW1].Haut == 0 && wws[WW1].LadungMax > 0 )
	{
		if (  wwd[WW1].wwlad > 0 && ladZuletzt[WW1] == 0 )
		{
			wwd[WW1].wwLadCtr = 0;																		// Zähler löschen bei Beginn der Speicherladung
			secLad[WW1] = 0;				
		}
		ladZuletzt[WW1] = wwd[WW1].wwlad;	
		
		if ( wwd[WW1].wwlad > 0 )																		// Speicherladung
		{
			++secLad[WW1];
			if ( secLad[WW1] >= 60 )
			{
				secLad[WW1] = 0;
				if ( wwd[WW1].wwLadCtr < 0xFFFF )
					++wwd[WW1].wwLadCtr;
				if ( wwd[WW1].wwLadCtr >= wws[WW1].LadungMax * 60 )
					wwd[WW1].wwLadAlarm = TRUE;
			}
		}
		else if ( wwd[WW1].wwLadCtr < wws[WW1].LadungMax * 60 )
			wwd[WW1].wwLadAlarm = FALSE;															// Ende vor Erreichen der maximalen Zeit						
	}
	else
	{
		secLad[WW1] = 0;
		wwd[WW1].wwLadCtr = 0;
		wwd[WW1].wwLadAlarm = FALSE;
	}	
	#endif	
		
#endif

	#if REGEL_WW2 == 1
		#if RCOND_WW2 == 1
		if ( wws[WW2].Haut == 0 && wws[WW2].TabwMax && wws[WW2].TabwZeit && ( abs( wwd[WW2].ei ) > wws[WW2].TabwMax ) 
										&& wwd[WW2].wwlad )
		#else 
			#if RCOND_WW2 == 2
			if ( wws[WW2].Haut == 0 && wws[WW2].TabwMax && wws[WW2].TabwZeit && ( abs( wwd[WW2].ei ) > wws[WW2].TabwMax ) 
										&& PUWZEA[WW2]->wert )
			#else		// RCOND_WW2 == 0
				if ( wws[WW2].Haut == 0 && wws[WW2].TabwMax && wws[WW2].TabwZeit && ( abs( wwd[WW2].ei ) > wws[WW2].TabwMax ) )
			#endif
		#endif																			
		{
			if ( ++wwd[WW2].abwCtr > ( wws[WW2].TabwZeit * 60 ) )
				wwd[WW2].abwAlarm = 1;
		}
		else
		{
			wwd[WW2].abwAlarm = 0;						
			wwd[WW2].abwCtr = 0;
		}
	
	// Solange Alarm ansteht, Counter rücksetzen; weil nach einer Quittierung nur der Alarm gelöscht wird. 
	if(wwd[WW2].abwAlarm == 1)
		wwd[WW2].abwCtr = 0;
		
	#endif	

#if WWANZ > 1 
	#if ( ZPU_WW2 == 1 || PU_BUS_ZPU_WW2 > 0 )
	if ( wws[WW2].Haut == 0 && wws[WW2].TempWWminZeit > 0 && wwd[WW2].zirkPu == 1
																													&& TWWV[WW2]->stat == 0 && TWWV[WW2]->messw <= wws[WW2].TempWWmin )
	{
		if ( ++wwd[WW2].wwMinCtr > ( wws[WW2].TempWWminZeit * 60 ) )
			wwd[WW2].tempAlarm = 1;
	}
	else
	{
		wwd[WW2].tempAlarm = 0;						
		wwd[WW2].wwMinCtr = 0;
	}
	
	if ( wwd[WW2].tempAlarm == 1 )
		wwd[WW2].wwMinCtr = 0;
	#endif

	#if ( SP_WW2 == 1 && ( LPU_WW2 == 1 || PU_BUS_LPU_WW2 > 0 ) )
	if ( wws[WW2].Haut == 0 && wws[WW2].LadungMax > 0 )
	{
		if (  wwd[WW2].wwlad > 0 && ladZuletzt[WW2] == 0 )
		{
			wwd[WW2].wwLadCtr = 0;																		// Zähler löschen bei Beginn der Speicherladung
			secLad[WW2] = 0;				
		}
		ladZuletzt[WW2] = wwd[WW2].wwlad;	
		
		if ( wwd[WW2].wwlad > 0 )																		// Speicherladung
		{
			++secLad[WW2];
			if ( secLad[WW2] >= 60 )
			{
				secLad[WW2] = 0;
				if ( wwd[WW2].wwLadCtr < 0xFFFF )
					++wwd[WW2].wwLadCtr;
				if ( wwd[WW2].wwLadCtr >= wws[WW2].LadungMax * 60 )
					wwd[WW2].wwLadAlarm = TRUE;
			}
		}
		else if ( wwd[WW2].wwLadCtr < wws[WW2].LadungMax * 60 )
			wwd[WW2].wwLadAlarm = FALSE;															// Ende vor Erreichen der maximalen Zeit						
	}
	else
	{
		secLad[WW2] = 0;
		wwd[WW2].wwLadCtr = 0;
		wwd[WW2].wwLadAlarm = FALSE;
	}	
	#endif	
		
#endif

#if NTANZ > 0
	// ##### ToPu : bei laenger andauernder grosser Regelabweichung wird ein Alarm generiert
	for ( i = 0; i < NTANZ; i++ )														// Alarme laeuft jede Sekunde
	{																												// nicht bei Hand und wenn nicht parameriert
		if ( nts[i].Haut == 0 && nts[i].TabwMax && nts[i].TabwZeit && ( abs( ntd[i].ei ) > nts[i].TabwMax ) )
		{
			if ( ++ntd[i].abwCtr > ( nts[i].TabwZeit*60 ) )			// Counter in Sekunden, Parameter in Minuten
				ntd[i].abwAlarm = 1;
		}
		else
		{
			ntd[i].abwAlarm = 0;						
			ntd[i].abwCtr = 0;
		}
		// Solange Alarm ansteht, Counter rücksetzen; weil nach einer Quittierung nur der Alarm gelöscht wird. 
		if(ntd[i].abwAlarm == 1)
			ntd[i].abwCtr = 0;
	}
#endif			
	
	/*--------------------------------------------------------------------------*/
	/* 								Blaue Led blinkend für Absenkzustände											*/
	/*--------------------------------------------------------------------------*/
	absenkStat = 0;
	#if HKANZ > 0
	for(i = 0; i < HKANZ; i++)				// in allen Reglern
	{	if(hkd[i].somme == 1)						// Zustand Sommerbetrieb
			absenkStat = 1;
		else
		{	
			if(hkd[i].vorra_bed == TRUE)	// Vorrang Bedarfsabsenkung vor Zeitabsenkung
			{	if(hkd[i].bedarf == BEDARF_NEIN)	// Zustand  Kein Bedarf (vom RIEcon50)
					absenkStat = 1;
			}		
			else
			{	if(abs_ram[i].absen == 1)		// Zustand  Absenkphase
					absenkStat = 1;
				if(abs_ram[i].hunt  == 1)		// Zustand  Heizunterbrechung
					absenkStat = 1;
			}
		}
		if(HKAUS[i]->bwert == 1)				// Anlagenschalter Nichtnutzung HK
					absenkStat = 1;
	}
	
	if ( ferienStat == 1 && hkd[i].tvabFerien )
	{
		absenkStat = 1;
		FEROUT[0]->wert = 1;
	}
	else
		FEROUT[0]->wert = 0;	
	#endif
			
	#if LED_DEBUG == 0
	if(absenkStat == 1)
		LedBlau = Sek & 0x01;					// Blaue Led ein- oder ausschalten
	else
		LedBlau = 1;									// Normalzustand
	#endif
		
	/*------------------------------------------------------------------------------*/
	/*								 Rote LED = Alarmausgabe																			*/
	/*------------------------------------------------------------------------------*/
	// 	- einfach blinkend für defekte Meßstellen
	// 	- konstant leuchtend für anstehende Alarme
	// 	- doppel blinkend für gespeicherte Alarme (wenn QUIT_TASTE vorhanden)  
	
	// ------- Check Alarme ---------------------------------------------------------
	//	- Alarmquelle untersuchen
	//	- bei Alarm:	Eintrag in die Alarmtabelle für nachfolgende Anzeigeroutine und
	//								Ausführung einer Alarm-Behandlungsroutine (wenn definiert)
	
	// Soft-Quittungstaste bilden: Durch Parli-Bedienung oder externe Quittungstaste
	#if QUIT_TASTE == 1
		if(QUITTS[ANL]->bstat != NICHTV)
			quit_taste |= QUITTS[ANL]->bwert;	// Tastendruck Zustand EIN speichern

		save_mode = quit_taste;						// Zustand 0: nicht gedrückt oder 1: gedrückt
		if(quit_taste == 1)
			quit_proc();
	#else
		save_mode = 3;										// Zustand 3: keine Speicherung	
	#endif
	
	
	// Sammelstörmeldung für Anzeige und LT vorab löschen
	for(i = 0; i < S_CLASS_ANZ; i++)
		sstm[i] = 0; 

	
	// Untersuchung der AlarmTabelle zur Feststellung eines Alarmes oder Ereignisses
	//----------------------------------------------------------------------------------------------------------------------
	for(i = 0; i < SETAB_TABL; i++)		// Die Alarmtabelle hat die gleiche Länge und Ordnung wie die Sekundär-Ereignistabelle
	{
		// Abfrage, ob Alarm deaktiviert
		if(alarmtab[i].Waitlimit == 0 || alarmtab[i].Maske == 0)			// Maske=0 bei GRUP, EREIGNIS oder deaktiviert
		{																															// siehe auch in UserReply.c: void Init_Alarmtab(void)
			alarmtab[i].alstatus   = 0;
			alarmtab[i].alspeicher = 0;
		}
		else
		{
			// Alarmquelle untersuchen
			dae  = seku_evtab[i].dae;		 
			gnum = KomTab[dae].gnum;							// Gruppennummer
			pnum = KomTab[dae].pnum;							// Parameternummer
			pADR = get_parli_info(gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
			
			switch(konv)
			{
				case ANA_FORM:
					abyte = pADR[2];									// Status des Messwertes
					if(abyte == NICHTV)
						abyte = 0;
					break;	
					
				case ANA_FORMP:
					pMWSP	=	(mwsp*)*(long*)pADR;			// Extrahieren der Adresse,	auf	die	"pADR" zeigt
					abyte = pMWSP->stat;
					if(abyte == NICHTV)
						abyte = 0;
					break;
					
				case JANEIN_FORMIP:
					pDINPSP	=	(dinpsp*)*(long*)pADR;	// Extrahieren der Adresse,	auf	die	"pADR" zeigt
					abyte = pDINPSP->bwert;
					break;
		
				default:
					abyte = pADR[0];
					break;
					
			}
			if( abyte & alarmtab[i].Maske)				// Alarm steht an
			{
				if(++alarmtab[i].waitcounter >= alarmtab[i].Waitlimit)	// Wartezeit abgelaufen
				{
					alarmtab[i].waitcounter = alarmtab[i].Waitlimit;
					alarmtab[i].alstatus   = 1;														// Alarm setzen
					
					
					// Mitglied einer Sammelalarmklasse ?
					if(alclass > 0 && alclass <= S_CLASS_ANZ)							// siehe userdef.h
						sstm[alclass]++;																		// Byte für Sammelalarm der entsprechenden Klasse erhöhen
						
					// Prozedur definiert ? 
					AlarmFunktion(alfunc);
					
				}
			}
			else	// Alarm steht nicht an
			{
				alarmtab[i].alstatus    = 0;				
				alarmtab[i].waitcounter = 0;
			}
		}
		
		// QSM: Quittierbare Störmeldungen speichern oder löschen
		if( alclass == FUEHLER || alclass == ANZEIGE || alclass == ANZEIGE_ROT || alclass == EREIGNIS)	// keine Alarmspeicherung
			;
		else	
		{	
			switch(save_mode)
			{	
				case 0:																									// Speichermodus
					alarmtab[i].alspeicher |= alarmtab[i].alstatus;				// Alarm speichern
					break;
				case 1:																									// Quittierung
					alarmtab[i].alspeicher = 0;														// Alarmspeicher löschen bei Tastendruck
					if(konv == JANEIN_FORM)																// Softalarm auch löschen
						pADR[0] = 0;
					break;
				case 3:																									// Transparentmodus
					alarmtab[i].alspeicher = alarmtab[i].alstatus;				// kein Speichern, nur kopieren
					if(quit_taste == 1)																		// Wenn keine echte Taste vorhanden, Softquittierung möglich (ANL: 96:) 
					{ 
//						quit_taste = 0;
						quit_proc();
						if(konv == JANEIN_FORM)															// Softalarm auch löschen
							pADR[0] = 0;
					}	
					break;
			}	
		}			
	}
			
	// Rote LED Anzeige
	//----------------------------------------------------------------------
	//bei gedrueckter Quittungstaste soll die rote LED nicht leuchten ! ( Frenzel )
	if ( save_mode != 1 )
	{
		abyte = 0;
		j = 0;
		for(i = 0; i < SETAB_TABL; i++)		
		{
			if(alarmtab[i].alstatus > 0)
				abyte |= alarmtab[i].Ledfunc;		// LED-Funktionen odern
				
			j |= alarmtab[i].alspeicher;			// Quittierbare Störmeldungen odern
		}
		
		sstm_alarme = 0;										// Merker für Alarme, für SSTM-Relais, Eintrag in parli für KomtabCopy
		sstm_all    = 0;										// Merker für alle Alarme inclusive Fühler, Eintrag in parli für KomtabCopy
		if(abyte > 0)												// 1 = Alarm, 2 = Fühlerstörung
		{
			if( (abyte & 3) == 2)				 			// nur 2 = "blinkend" bedeutet Fühlerstörung
			{	SetLedRot(2, 1);								// Einschaltdauer 2*5 ms , 1 Periode
				sstm_all = 1;
			}	
			else
			{	SetLedRot(255, 1);							// immer an	
				sstm_alarme = 1;
				sstm_all = 1;
			}	
		}
		SSTM[ANL]->wert = sstm_alarme;
		
		// Blinkmodus für gespeicherte Alarme
		if(save_mode != 3)
		{
			if(j > 0)
			{
				un_qsm = 1;											// Unquittierte Störungen vorhanden	
				if(abyte == 0)									// wenn Rote LED nicht aktiviert
					SetLedRot(2, 2);							// Einschaltdauer 2*5 ms, 2 Perioden
			}
			else
				un_qsm = 0;
		}					
	}
	
	// Störungsanzeige auf LCD durch Anwahl des Parameters  					
	//-----------------------------------------------------------------------
	// Test, ob Anzeige noch aktiv
	abyte = 0;
	for(i = 0; i < SETAB_TABL; i++)
		abyte |= alarmtab[i].viscounter;	// Zähler für Anzeigezeit untersuchen	
			
	// Alle Anzeigezeiten neu laden, wenn abgelaufen
	if(abyte == 0)
	{	for(i = 0; i < SETAB_TABL; i++)
		{
			if(alarmtab[i].alstatus > 0)
				alarmtab[i].viscounter = alarmtab[i].Visloader;	// Anzeigezeit neu laden
		}		
	}
	
	// Parameter anzeigen 
	for(i = 0; i < SETAB_TABL; i++)
	{
		if(alarmtab[i].viscounter > 0)			// Zähler für Anzeigezeit	
		{
			alarmtab[i].viscounter--;
			if(autoalarm == 0)								// nur wenn gerade nicht bedient wird
			{	
				dae     = seku_evtab[i].dae;		 
				// neue Parameteranzeige wird realisiert durch Bedien.c
				grupnum = KomTab[dae].gnum;			// Gruppennummer
				paranum = KomTab[dae].pnum;			// Parameternummer

				if(grupnum == 0 && paranum == 0)
					unbek_Alarm = i;
			}	
			break;
		}
	}
	
	// DAE der aktuellen Alarme in einer Liste merken (für RFB)
	j = 0;
	for(i = 0; i < SETAB_TABL; i++)
	{
		if(alarmtab[i].alstatus > 0)	
		{	
			alarmlist_dae[j] = seku_evtab[i].dae;
			j++;
		}
	}
	alarmanz_dae = j;	
			
	
	#if QUIT_TASTE == 1
		quit_taste =  QUITTS[ANL]->bwert;		// Zustand der Taste anzeigen
	#else
		quit_taste = 0;	
	#endif	
				 		

}			
/*---------------------------------- Ende Task Alarme -------------------------------*/

// Aufruf der Alarmfunktion
// Definitionen in userdef.h  	
void AlarmFunktion(char alfunc)
{
	switch(alfunc)
	{
		case PUMP_AUS:
			PumpenAus();
			break;
	}
	
}


void PumpenAus(void)
{
	char h = HKANZ;
	char k = KEANZ;
	char i;

	PUWLEA[WW1]->wert = 0;					// Ladepumpe ausschalten
	PUWZEA[WW1]->wert = 0;					// ZirkPu ausschalten	
	PUWTEA[WW1]->wert = 0;					// TauscherPu ausschalten		
		
	for ( i = 0; i < k; i++ )
	{
		KE[i]->wert			= 0;					// Kesselfreigabe aus
		KESTU1[i]->wert = 0;					// Stufe1 ausschalten
		KESTU2[i]->wert = 0;					// Stufe2 ausschalten
	}

	for ( i = 0; i < h; i++ )
	{	
		PU[i]->wert			= 0;					// Heizungspumpe ausschalten
		hk_abschalten ( i, 1 );				// mit Starttemp. init
	}
}	

// Behandungsroutine für Quit-Taste
void quit_proc(void)
{
	noi_reanimate			    = 0;
	noi_sys_tasknum_copy	= 0;
	noi_user_tasknum_copy	= 0;
}
	