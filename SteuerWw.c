/*	Task:					SteuerWw			*/
/*	Warmwassersteuerung						*/

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "uconstext.h"

void puwlcnt(char ww);													
void puwllaufz(char ww);
void puwl_ein(char ww);
void CheckVorrang ( char ww, mwsp *p_wwIst, int wwSoll );
void puwBm(char ww);
void puwz_einaus(char ww);
void Pumpenbus_WW ( char ww );
void ZirkPuLaufzeit ( char ww );

/*-------------------------------- Task SteuerWw -----------------------------------*/
// Takt: 30 sek

#if WWANZ

void SteuerWw(void)
{
	char flag;
	int twwlbeg, twwlend;
	int temp1, temp2, toben, tmin, tmax;
	char stat1, stat2;
	static char legAl[2] = {0,0};		// Merker für Legionellen-Alarm
	char sp, reg;
	char puwlMode;
	char ww;
	
	for ( ww = WW1; ww < WWANZ; ww++ )
	{
		if ( ww > WW2 )
			return;
		if ( Projekte[proj_typ].wwcode[ww] == 0 )
			return;											// WW-Kreis nicht aktiv
			
		sp = ( ww == WW1 ) ? SP_WW1 : SP_WW2;
		reg = ( ww == WW1 ) ? REGEL_WW1 : REGEL_WW2;


		// Vorbereitung der Ermittlung des Ladebetriebs. Die Temperaturen werden auch beim Legiobetrieb benutzt 
		stat1 = TWW1L[ww]->stat;		// Status   Speichertemp. 1 Oben
		stat2 = TWW2L[ww]->stat;		// Status   Speichertemp. 2 Unten
		temp1 = TWW1L[ww]->messw;		// Messwert Speichertemp. 1 Oben
		temp2 = TWW2L[ww]->messw;		// Messwert Speichertemp. 2 Unten
			
		toben = 1000;	// 100,0°C vorbelegen
		if ( stat1 == 0 )
			toben = temp1;
		else if ( stat2 == 0 )
			toben = temp2;

		tmax  = 1000;														// Vorbelegung 100,0 °C
		tmin  = 1000;														// Vorbelegung 100,0 °C
		
		// Ermitteln der LadeBeginntemp u. LadeEndetemp
		twwlbeg = wws[ww].Twwlbeg;							// akt. LadeBeginntemp
		twwlend = wws[ww].Twwlend;							// akt. LadeEndetemp
		if ( wwd[ww].legio == TRUE )						// wenn Legionellenschaltung
		{	
			twwlend = wws[ww].Twwleg;							// Endtemp. = Legiotemp.
			twwlbeg = twwlend - 20;								// Beginntemp - 2,0 °C
		}	

		// wenn zwei Fühler existieren, kann durch Parameter bestimmt werden, ob nur einer für die Speicherladung benutzt wird
		// dazu wird der Status des anderen Fühlers als gestört gekennzeichnet, danach alles wie bisher
		if ( ( ww == WW1 && SPTP_WW1 == 2 ) || ( ww == WW2 && SPTP_WW2 == 2 ) )
		{			
			if ( wws[ww].AuswahlFuehler == 1 && stat1 == 0 )
				stat2 = 1;
			else if ( wws[ww].AuswahlFuehler == 2 && stat2 == 0 )
				stat1 = 1;			
		}		
				
		// Ermitteln der momentanen max. und min. Temperatur über 2 Speicherfühler
		if ( (stat1 | stat2 ) == 0 )						// beide Fühler ok
		{	
			if ( temp1 >= temp2 )
			{	
				tmax = temp1;												// tmax: Überwachung Ladebeginn
				tmin = temp2;												// tmin: Überwachung Ladeende
			}
			else
			{	
				tmax = temp2;
				tmin = temp1;
			}
		}
		else																		// mindestens 1 Fühler defekt
		{	
			if ( stat1 == 0 )
				tmax = tmin = temp1;
			else if ( stat2 == 0 )
				tmax = tmin = temp2;
		}

		// Betriebsmeldung Ladepumpen und Zirkpumpe überwachen
		if ( wws[ww].Haut == FALSE )
			puwBm ( ww );
		
		if ( sp )
		{
			//----------------------- Ladepumpen ---------------------------------
			// Abfrage Handbetrieb 
			if ( wws[ww].Haut == TRUE )
			{
				PUWLEA[ww]->wert = wws[ww].Lpea;
				PUWTEA[ww]->wert = wws[ww].Tpea;
				wwd[ww].puwlBm 	= 0;												// kein Soft-Alarm
			}		
			else	// Automatik
			{	
				wws[ww].Haut = FALSE;
		
				// LadePumpenlaufzeiten
				puwllaufz ( ww );
		
				// Ladepumpe ein/aus	
				if ( wwd[ww].wwlad )	//------------------------ im Ladebetrieb ?
				{
					PUWTEA[ww]->wert = 1;											// Tauscherpumpe einschalten
			
					puwlMode = ( ww == WW1 ) ? PUWL_MODE_WW1 : PUWL_MODE_WW2;
					if ( puwlMode == 0 )
					{														
						puwl_ein ( ww) ;												// Ladepumpe einschalten ohne Temperaturabfrage
						CheckVorrang ( ww, TVKE[KE1], wwd[ww].legio ? wws[ww].TwwladLegio : wws[ww].Twwlad );
					}
					else if ( puwlMode == 1 )
					{
						if ( TVKE[KE1]->stat || TVKE[KE1]->messw > toben )		// wenn Kesseltemp. höher als Speichertemp. oben
							puwl_ein ( ww );											// Ladepumpe einschalten
						CheckVorrang ( ww, TVKE[KE1], wwd[ww].legio ? wws[ww].TwwladLegio : wws[ww].Twwlad );
					}
					else if ( puwlMode == 2 )
					{
						if ( TWWC[ww]->stat || TWWC[ww]->messw > toben )			// wenn TWWC höher als Speichertemp. oben
							puwl_ein ( ww );											// Ladepumpe einschalten
						CheckVorrang ( ww, TWWC[ww], wwd[ww].legio ? wws[ww].TwwladLegio : wws[ww].Twwlad );
					}
					else if ( puwlMode == 3 )
					{
						if ( TVSEK[NT1]->stat || TVSEK[NT1]->messw > toben )	// wenn TVSEK höher als Speichertemp. oben
							puwl_ein ( ww );											// Ladepumpe einschalten
						CheckVorrang ( ww, TVSEK[NT1], wwd[ww].legio ? wws[ww].TwwladLegio : wws[ww].Twwlad );
					}
				}
				else	//-------------------------------------- nicht im Ladebetrieb
				{	
					wwd[ww].zpuwtna ? ( wwd[ww].zpuwtna-- ) : ( PUWTEA[ww]->wert = 0 );	// Tauscherpumpe nach Nachlauf abschalten	
					wwd[ww].zpuwlna ? ( wwd[ww].zpuwlna-- ) : ( PUWLEA[ww]->wert = 0 );	// Ladepumpe nach Nachlauf abschalten
				
					wwd[ww].wwvor = FALSE;
					wwd[ww].zvorrad = 0;
					wwd[ww].vorrVerz = wws[ww].TvorrZeit * 2;	// Zaehler Abweich-Dauer setzen		
				}
			}

			// Ladebetrieb ermitteln mit 2 Speicherfühlern:
			// Der Ladebetrieb beginnt, wenn beide Speichertemp. die LadeBeginntemp. unterschreiten.
			// Der Ladebetrieb endet,   wenn beide Speichertemp. die LadeEndetemper. überschreiten.
			// Der Ausfall eines Fühlers wird durch den zweiten Fühler kompensiert.
			// Beim Ausfall beider Fühler gibt es keinen Ladebetrieb.
		
			// Ladebetrieb ermitteln
			flag = 1;
			if ( WWAUS[ww]->bstat != NICHTV && WWAUS[ww]->bwert == 1 )
				flag = 0;															// Anlagenschalter 'Nichtnutzung WWB' ist aktiv
			else
			{
				if ( wwd[ww].legio == 0 )							// wenn kein Legionellenbetrieb
				{	// Freigabe für Schalten des Ladebetriebs nach Bedarfsanforderung und Datue ok ?
					if ( wws[ww].FrLadBedarf && wwd[ww].wwladBedarf != BEDARF_UNBEKANNT && wwd[ww].wwladBedarf == BEDARF_NEIN )
						flag = 0;													// z.Z. kein Bedarf ( über Kommando WR_KASKADE )
				}		
			}
	
			if ( flag )
			{
				if ( tmax >= 900 )
					wwd[ww].wwlad = FALSE;							// vorsichtshalber ist bei Speicher >= 90,0 °C immer Schluss !
				else
				{
					if ( wwd[ww].legio && wws[ww].LadNieAus )
					{																		// immer Speicherladung wenn Legio-Betrieb
						if ( wwd[ww].wwlad	== FALSE)
							wwd[ww].zpuwtna = wws[ww].Puwtna << 1;	// Tauscherpumpe Nachlaufzeit setzen (x min * 2 * 30s-Task)
						wwd[ww].wwlad = TRUE;							// Ladebetrieb setzen						
					}
					else
					{
						if ( tmin > twwlend )							// größer als LadeEndetemp. ? oder beide Fühler defekt
							wwd[ww].wwlad = FALSE;					// Ladebetrieb löschen	
						else if ( tmax < twwlbeg )				// kleiner als LadeBeginntemp. ?	
						{																	// Ladebetrieb setzen
							if ( wwd[ww].wwlad == FALSE )		// Anfangsinit.
							{	
								wwd[ww].wwlad = TRUE;					// Ladebetrieb setzen
								wwd[ww].zpuwtna	= wws[ww].Puwtna << 1;	// Zähler f. Tauscherpu Nachlaufzeit setzen (x min * 2 * 30s-Task)
							}
						}
					}
				}
			}
			else
			{																				// Anlagenschalter 'Nichtnutzung WWB' ist aktiv oder BEDARF_NEIN
				wwd[ww].wwlad = FALSE;								// Ladebetrieb löschen	
				wwd[ww].wwvor = FALSE;								// Vorrang löschen
			}
			
			if ( wwd[ww].wwvor == FALSE )						// wenn im Zustand Speicherladung die Ladepumpe nicht eingeschaltet wird
			{																				// (weil z.B. die Regeltemperatur nicht ansteigt), wird ein von einem früheren Vorrang
				wwd[ww].zvorrad = 0;									// stehengebliebenes ei_ww zur HK-Absenkung herangezogen. Da der Counter bei nicht eingeschalteter
				wwd[ww].ei_ww = 0;										// Ladepumpe nicht dekrementiert wird, geht der Vorrang nicht zu Ende
			}	
		
		}			// kein Speicher, keine Lade-, keine Tauscherpumpe
	
		else
			CheckVorrang ( ww, TWWC[ww], wwd[ww].twwsb );


		// --------------------- Zirkulationspumpe --------------------------
		if ( ( ww == WW1 && ( ZPU_WW1 == 1 || PU_BUS_ZPU_WW1 > 0 ) ) || ( ww == WW2 && ( ZPU_WW2 == 1 || PU_BUS_ZPU_WW2 > 0 ) ) )
		// Abfrage Handbetrieb 
		{
			if ( wws[ww].Haut == TRUE )
			{	
				PUWZEA[ww]->wert = wws[ww].Zpea;
				wwd[ww].puwzBm = 0;													// kein Soft-Alarm
			}	
			else	// Automatik
			{
				if ( WWAUS[ww]->bstat != NICHTV && WWAUS[ww]->bwert == 1) // Anlagenschalter 'Nichtnutzung WWB' ist aktiv
					wwd[ww].zirkPu = 0;
				else
				{
					// Freigabe für Schalten der Zirkulationspumpe nach Bedarfsanforderung und Datue ok ?
					if ( ( wws[ww].FrZirkBedarf > 0 ) && ( wwd[ww].wwzirkBedarf != BEDARF_UNBEKANNT ) )
					{	
						if ( wwd[ww].wwzirkBedarf == BEDARF_JA )	// Bedarf ?
							puwz_einaus ( ww );
						else
						{ 
							if ( wwd[ww].wwzirkBedarf == BEDARF_NEIN )	// kein Bedarf ?
							{
								if(wwd[ww].legio == TRUE)							// wenn Legionellenschaltung	
									wwd[ww].zirkPu = 1;
								else
									wwd[ww].zirkPu = 0;
							}
						}
						if ( wwd[ww].zpuwz > 0 )
							wwd[ww].zpuwz--;
					}					
					else	// Schalten nach Abschaltzeiten 
					{
						if ( wws[ww].Puwzdau == 0 )								// Abschaltdauer 0 h
						{
								wwd[ww].zpuwz = 0;										// Zähler für ZirkPu Abschaltdauer
								puwz_einaus ( ww );
						}
						else
						{	
							if ( wwd[ww].zpuwz == 0 )								// wenn Zähler nicht gesetzt
							{	
								if ( wws[ww].Puwzab[1] == Std &&  wws[ww].Puwzab[0] == Min )	// Abschaltzeit abfragen
									wwd[ww].zpuwz = wws[ww].Puwzdau * 120;	// Zähler setzen (1h bei 30s-Task)
								else																	// zur Unzeit
									puwz_einaus ( ww );
							}
							else	// Abschaltung läuft
							{	
								wwd[ww].zpuwz--;
								if ( wwd[ww].legio == TRUE )						// wenn Legionellenschaltung
									wwd[ww].zirkPu = 1;
								else
									wwd[ww].zirkPu = 0;
							}
						}	
					}
				}
				PUWZEA[ww]->wert = wwd[ww].zirkPu;
			}
			if ( ( ww == WW1 && ZPU_LZ_WW1 == 1 ) || ( ww == WW2 && ZPU_LZ_WW2 == 1 ) )
				ZirkPuLaufzeit ( ww );											// Laufzeit der Zirkulationspumpe berechnen / anzeigen
		}
		
		// --------------- Legionellenschaltung --------------------------
		flag = 1;
		if ( WWAUS[ww]->bstat != NICHTV && WWAUS[ww]->bwert == 1 ) // Anlagenschalter 'Nichtnutzung WWB' ist aktiv
			flag = 0;
		else
		{	
			if ( wws[ww].Legjn == TRUE )
			{	
				if ( wws[ww].Legtag & wotbit() )						// gültigen Wochentag abfragen
				{	
					if ( wwd[ww].zleg == 0 )									// Zähler läuft nicht
					{	
						if ( wws[ww].Legzeit[1] == Std && wws[ww].Legzeit[0] == Min )				// Einschaltzeit abfragen
						{	
							wwd[ww].zleg = wws[ww].Legdau * 120;	// Zähler setzen (1h bei 30s-Task)
							wwd[ww].legio = 1;										// Temperaturerhöhung ein
							legAl[ww] = 1;												// muss während Legionellenschaltung rückgesetzt werden
						}
						else
					 		flag = 0;
					}	 
					else																		// Zähler läuft
					{
						if ( ( sp && ( wwd[ww].wwlad == FALSE || tmin > twwlend ) ) || ( reg && TWWZ[ww]->messw >= wws[ww].TwwzLegio ) )	
						{
							legAl[ww] = 0;											// Legio-Temp. erreicht - alles ist gut
							wwd[ww].legioAl = FALSE;						// auch ein von vorhergehender Desinfektion generierter Alarm wird gelöscht
						}
						if ( wwd[ww].zleg == 1 && legAl[ww] == 1 )
							wwd[ww].legioAl = TRUE;							// Ende der therm.Desinf. - Legio-Temp. nicht erreicht
					
						if ( --wwd[ww].zleg == 0 )						// wenn abgelaufen
							flag = 0;
					}		
				}
				else	
					flag = 0;
			}
			else
			{
				flag = 0;
				wwd[ww].legioAl = FALSE;
				set_alspeicher( (char*)&wwd[ww].legioAl, 0 );	// gespeicherten Alarm löschen
			}					
		}		
	
		if ( flag == 0 )
		{
				wwd[ww].legio = 0;
				wwd[ww].zleg  = 0;
				legAl[ww] = 0;		
		}

		#if ( BUS_PU_MAX > 0 )
		Pumpenbus_WW ( ww );
		#endif
		
	}	// 	for ( ww = WW1; ww < WWANZ; ww++ )	
}	
/*-------------------------------- Ende Task SteuerWw -----------------------------------*/



//---------------- Unterprogramme -------------------------------------

// PUWL Einschalten zählen
//------------------------
void puwlcnt ( char ww )													
{
	if ( ww > WW2 )
		return;
	wwd[ww].puwlsta++;
	bicbus ( EEPADR, (char*)&wwd[ww].puwlsta, PUWLSTA1_ADR+(ww*2), 2, BICWR );
}

// PUWL Laufzeit erhöhen
//-----------------------
void puwllaufz ( char ww )
{
	static char puwl_sec[2];
	
	if ( ww > WW2 )
		return;
	if ( PUWLEA[ww]->func != NICHTV )
	{	
		if ( PUWLEA[ww]->wert )										// Abfrage Relais: Ladepumpe ein ?
		{	
			if ( ++puwl_sec[ww] >= 120 )							// 30-Sekunden Zähler hochzählen
			{	
				puwl_sec[ww] = 0;
				wwd[ww].puwlh++;											// Laufzeit h erhöhen
				bicbus ( EEPADR, (char*)&wwd[ww].puwlh, PUWLH1_ADR+(ww*2), 2, BICWR );
			}
		}
	}			
}	

// PUWL einschalten
//-----------------------
void puwl_ein ( char ww )
{
	if ( ww > WW2 )
		return;
	if ( wwd[ww].zpuwlna == 0 )
		wwd[ww].zpuwlna	= wws[ww].Puwlna * 2;			
	if (PUWLEA[ww]->func != NICHTV )
	{	
		if ( PUWLEA[ww]->wert == 0 )							// Ladepumpe aus ?
			puwlcnt ( ww );													// Einschalten zählen
		PUWLEA[ww]->wert = 1;											// Ladepumpe einsch.
	}	
}	


void CheckVorrang ( char ww, mwsp *p_wwIst, int wwSoll )
{
	static float flEiWW[2];
	int wwIst;
	
	if ( ww > WW2 ) 
		return;
	if ( p_wwIst->stat != 0 )
		return;
	else
		wwIst = p_wwIst->messw;
		
	if ( wwd[ww].wwvor == FALSE )
	{
		wwd[ww].ei_ww = 0;
		flEiWW[ww] = 0;
		if ( wwSoll - wwIst >= wws[ww].TvorrDiff )	// Abweichung groesser als parametriert
		{
			if ( --wwd[ww].vorrVerz <= 0 )
			{																					// Zeit erreicht
				wwd[ww].wwvor = TRUE;
				wwd[ww].zvorrad = wws[ww].Vorrad * 2;		// Zähler f. Vorrang-Dauer setzen(x min * 2 * 30s-Task)
			}
		}
		else
			wwd[ww].vorrVerz = wws[ww].TvorrZeit * 2;	// Zaehler Abweich-Dauer wieder setzen		
	}
	else
	{
		if ( wwSoll - wwIst > wws[ww].TvorrDiff ) 
		{
			if ( wwd[ww].zvorrad > 0 ) 
			{																					// Abweichung zu gross, maximale Zeit fuer WW-Vorrang noch nicht abgelaufen
				--wwd[ww].zvorrad;
				flEiWW[ww] = g_filter ( flEiWW[ww] , wwSoll - wwIst, 300, 1200 );
																								// Ts = 30 sec ( Taskzeit SteuerWw ), Fzk = 120 sec
				wwd[ww].ei_ww =	(int)( flEiWW[ww] * 10	);			
			}
			else
				wwd[ww].ei_ww = 0;											// Zeit fuer Vorrang ist abgelaufen													
		}
		else																	// Status "Vorrang" wird erst beendet, wenn wwSoll - wwIst <= wws[ww].TvorrDiff																		
		{																			// erst dann ist erneut Absenkung im Heizkreis moeglich
			wwd[ww].wwvor = FALSE;	
			wwd[ww].vorrVerz = wws[ww].TvorrZeit * 2;	// Zaehler Abweich-Dauer setzen
		}
	}
}
									 
// Betriebsmeldung Ladepumpe, Zirkpumpe und Tauscherpumpe überwachen
void puwBm ( char ww )
{
	if ( ww > WW2 )
		return;
	if ( ( ww == WW1 && LPUBM_WW1 == 1 ) || ( ww == WW2 && LPUBM_WW2 == 1 ) )	// Betriebsmeldung Ladepumpe vorhanden
	{
		if ( BMPUWL[ww]->bstat != NICHTV && wws[ww].PuwlBmVerz > 0 )	
		{																				// Nur wenn Betriebsmelde-Kontakt vorhanden und Verzögerung nicht 0
			if( PUWLEA[ww]->wert == 1 )						// Pumpe EIN ?
			{
				if ( BMPUWL[ww]->bwert == 1 )				// Betriebsmeldung auch EIN ?
				{
					wwd[ww].puwlBm = 0;								// kein Alarm
					wwd[ww].puwlBmVerz = 0;
				}
				else
				{
					if ( wwd[ww].puwlBmVerz < 0xFFFF )
						++wwd[ww].puwlBmVerz;
					if ( wwd[ww].puwlBmVerz >= wws[ww].PuwlBmVerz * 2 )
						wwd[ww].puwlBm = 1;						// Soft-Alarm: 'kein Betriebsmeldungs-Signal'
				}
			}
			else
				wwd[ww].puwlBmVerz = 0;	
		}	
		else	
		{
			wwd[ww].puwlBm = 0;									// kein Alarm
			wwd[ww].puwlBmVerz = 0;	
		}
	}

	if ( ( ww == WW1 && ZPUBM_WW1 == 1 ) || ( ww == WW2 && ZPUBM_WW2 == 1	) )	// Betriebsmeldung Zirkpu vorhanden
	{
		if ( BMPUWZ[ww]->bstat != NICHTV && wws[ww].PuwzBmVerz > 0 )	
		{																				// Nur wenn Betriebsmelde-Kontakt vorhanden und Verzögerung nicht 0
			if( PUWZEA[ww]->wert == 1 )						// Pumpe EIN ?
			{
				if ( BMPUWZ[ww]->bwert == 1 )				// Betriebsmeldung auch EIN ?
				{
					wwd[ww].puwzBm = 0;								// kein Alarm
					wwd[ww].puwzBmVerz = 0;
				}
				else
				{
					if ( wwd[ww].puwzBmVerz < 0xFFFF )
						++wwd[ww].puwzBmVerz;
					if ( wwd[ww].puwzBmVerz >= wws[ww].PuwzBmVerz * 2 )
						wwd[ww].puwzBm = 1;						// Soft-Alarm: 'kein Betriebsmeldungs-Signal'
				}
			}
			else
				wwd[ww].puwzBmVerz = 0;	
		}	
		else	
		{
			wwd[ww].puwzBm = 0;									// kein Alarm
			wwd[ww].puwzBmVerz = 0;	
		}
	}
	
	if ( ( ww == WW1 && TPUBM_WW1 == 1 ) || ( ww == WW2 && TPUBM_WW2 == 1	) )	// Betriebsmeldung Tauscherpumpe vorhanden
	{
		if ( BMPUWT[ww]->bstat != NICHTV && wws[ww].PuwtBmVerz > 0 )	
		{																				// Nur wenn Betriebsmelde-Kontakt vorhanden und Verzögerung nicht 0
			if ( PUWTEA[ww]->wert == 1 )					// Pumpe EIN ?
			{
				if ( BMPUWT[ww]->bwert == 1 )				// Betriebsmeldung auch EIN ?
				{
					wwd[ww].puwtBm = 0;								// kein Alarm
					wwd[ww].puwtBmVerz = 0;
				}
				else
				{
					if ( wwd[ww].puwtBmVerz < 0xFFFF )
						++wwd[ww].puwtBmVerz;
					if ( wwd[ww].puwtBmVerz >= wws[ww].PuwtBmVerz * 2 )
						wwd[ww].puwtBm = 1;						// Soft-Alarm: 'kein Betriebsmeldungs-Signal'
				}
			}
			else
				wwd[ww].puwtBmVerz = 0;	
		}	
		else	
		{
			wwd[ww].puwtBm = 0;							// kein Alarm
			wwd[ww].puwtBmVerz = 0;	
		}
	}
}

// Zirkulationspumpe Ein-/Ausschalten
void puwz_einaus ( char ww )
{			
	if ( ww > WW2 )
		return;
	if ( wwd[ww].wwlad && wwd[ww].legio == FALSE && wws[ww].ZpuAusLad )	
	{		
		PUWZEA[ww]->wert = 0;
		wwd[ww].zirkPu = 0;
	}
	else
	{			
		PUWZEA[ww]->wert = 1;									// ZirkPu einschalten
		wwd[ww].zirkPu = 1;
	}
}



void Pumpenbus_WW ( char ww )
{
#if BUS_PU_MAX > 0
	char pu = 0;
	
	if ( ww > WW2 )
		return;

	pu = ( ww == WW1 ) ? PU_BUS_TPU_WW1 : PU_BUS_TPU_WW2;
	
	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
	
		if ( BusPuPara[pu].Funktion == 1 )
		{
			if ( wwd[ww].wwlad || wwd[ww].zpuwtna )
			{
				BusPuPara[pu].Betrieb = 1;
//-				BusPuPara[pu].Sollwert = ??? ;		Sollwert variabel / geregelt ?
			}
			else
			{
				BusPuPara[pu].Betrieb = 0;
//-				BusPuPara[pu].Sollwert = 0;
			}				
		}

		#if GENI == 1	
		wwd[ww].busTPuEin = BusPuData[pu].uc_betriebsart;	
		#endif

		#if WILO_MODBUS == 1	
		wwd[ww].busTPuEin = modWiloPu[pu].output_value1_temp;	
		#endif
		
		#if GRUNDFOS_MODBUS == 1

		#endif

	}

	pu = ( ww == WW1 ) ? PU_BUS_LPU_WW1 : PU_BUS_LPU_WW2;
	
	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
	
		if ( BusPuPara[pu].Funktion == 1 )
		{
			if ( wwd[ww].wwlad || wwd[ww].zpuwlna )
			{
				BusPuPara[pu].Betrieb = 1;
//-				BusPuPara[pu].Sollwert = ??? ;		Sollwert variabel / geregelt ?
			}
			else
			{
				BusPuPara[pu].Betrieb = 0;
//-				BusPuPara[pu].Sollwert = 0;
			}				
		}

		#if GENI == 1	
		wwd[ww].busLPuEin = BusPuData[pu].uc_betriebsart;	
		#endif

		#if WILO_MODBUS == 1
		wwd[ww].busLPuEin = modWiloPu[pu].output_value1_temp;	
		#endif

		#if GRUNDFOS_MODBUS == 1

		#endif
	}
	
	pu = ( ww == WW1 ) ? PU_BUS_ZPU_WW1 : PU_BUS_ZPU_WW2;
	
	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
	
		if ( BusPuPara[pu].Funktion == 1 )
		{
			if ( wwd[ww].zirkPu == 1 )
			{
				BusPuPara[pu].Betrieb = 1;
//- Sollwert variabel ? Pumpe geregelt ?
//-				BusPuPara[pu].Sollwert = ;
			}
			else
			{
				BusPuPara[pu].Betrieb = 0;
//- Sollwert variabel ? Pumpe geregelt ?
//-				BusPuPara[pu].Sollwert = 0;
			}	
		}

		#if GENI == 1	
		wwd[ww].busZPuEin = BusPuData[pu].uc_betriebsart;	
		#endif

		#if WILO_MODBUS == 1
		wwd[ww].busZPuEin = modWiloPu[pu].output_value1_temp;	
		#endif

		#if GRUNDFOS_MODBUS == 1

		#endif
	}
#endif
}


void ZirkPuLaufzeit ( char ww )
{		
	static char min2[2] = {0,0};
	
	if ( ww > WW2 )
		return;
	if ( PUWZEA[ww]->func != NICHTV )
	{	
		if ( PUWZEA[ww]->wert && ++min2[ww] >= 120 )	// 30-sec-Zaehler inkr., wenn Zirkulationspumpe ein
		{	
			min2[ww] = 0;
			wwd[ww].puwzh++;						// Laufzeit h erhöhen
			bicbus ( EEPADR, (char*)&wwd[ww].puwzh, PUWZH1_ADR+(ww*2), 2, BICWR );
		}
	}	
}

#endif	//if WWANZ