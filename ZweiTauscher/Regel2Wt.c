/*	Task:	Regel2Wt															*/
/*	Steuerung und Regelung von 2 Wärmetauschern	*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

void AbweichInt ( void );
void TauscherLaufzeit ( void );
char TauscherStoerung ( char wt );
void TauscherAus ( char wt );
void Klappe ( char wt, char az );
void SollwertAnhebung ( mwsp* pT_ist );
void TauscherRegeln ( char wt, mwsp *pTvWt );
void ModulationMW ( void );


#define KEINE_ANFORD 100								// 10°C 

/*-------------------------------- Task Regel2Wt -----------------------------------*/
// Takt: 1 sek
void Regel2Wt(void)
{
	static char sec = 30;
	char wt;	
	char fuehr, folge;
	char abschFolgeWt = 0;
	char fuehrWtWechsel = 0;
	
// projektabhängige Zuweisung der Temperaturfühler Anlage und Tauscher
	mwsp *pTvAnl = TVSGNT[0];				// Vorlauf Anlage
	mwsp *pTrAnl = TRSGNT[0];				// Rücklauf Anlage
	mwsp *pTvWt[2] = {0,0};					// Vorlauf sek.Tauscher
	mwsp *pTrWt[2] = {0,0};					// Rücklauf prim.Tauscher

	pTvWt[NT1] = TVSEK[NT1];
	pTvWt[NT2] = TVSEK[NT2];
	pTrWt[NT1] = TRPNT[NT1];
	pTrWt[NT2] = TRPNT[NT2];

	if ( ++sec >= 30 )
	{
		sec = 0;

// Solltemp. ermitteln ( Anforderung )
		ntd[0].tvsb_anl = ( maxAnford > KEINE_ANFORD ) ? maxAnford : KEINE_ANFORD;	

// Fehlerkorrektur Tauscher-Folge
		if ( nts[0].FuehrWt < 1 || nts[0].FuehrWt > 2 )
			nts[0].FuehrWt = 1;										 
		if ( nts[0].FuehrWtFest > 2 )
			nts[0].FuehrWtFest = 0;								// Fuehrungstauscher nicht vorgegeben		

		fuehr = (char)(nts[0].FuehrWt - 1);		// 0 oder 1
		folge = (char)(fuehr ^ 1);						// 1 oder 0	

// der Folge-Tauscher soll abgeschaltet werden, wenn die Temperatur-Differenz Vorlauf-Rücklauf (Anlage) kleiner ist
// als ein parametrierbarer Mindestwert
// aber nur wenn der Anlagen-Sollwert erreicht ist
// der Folge-Tauscher wird ebenso bei Überschreitung der Schaltdifferenz abgeschaltet
// FolgeWtAus = 0 schaltet die Funktion aus
		if ( ( pTvAnl->stat == 0 && pTrAnl->stat == 0	&& ( pTvAnl->messw - pTrAnl->messw < nts[0].FolgeWtAus	)
									&& nts[0].FolgeWtAus != 0 && ( pTvAnl->messw > ntd[0].tvsb_anl ) ) )

		{
			abschFolgeWt = 1;
			ntd[0].wtModCtr = 0;
		}


// wenn keine Anforderung vorliegt, können beide Tauscher ausgeschaltet werden
		for ( wt = 0; wt < NTANZ; wt++ )
		{
			if ( ntd[0].tvsb_anl <= KEINE_ANFORD )
			{	
				ntd[wt].tvsb = KEINE_ANFORD;
				TauscherAus ( wt );
			}		
			else
			{
				if ( nts[0].Tvpa > 0 ) 
					ntd[wt].tvsb = ntd[0].tvsb_anl + nts[0].Tvpa;
				else
					ntd[wt].tvsb = ntd[0].tvsb_anl + ntd[0].y_rel_anh;
	
				// Rücklaufbegrenzung
				if ( pTrWt[wt]->stat == 0 )			// wenn Rücklauffühler Primär ok
				{	
					if ( pTrWt[wt]->messw > nts[wt].Trma )	// nur wenn MAX überschritten
					{	
						ntd[wt].rlbegr = 1;										// Statusanzeige Rücklaufbegrenzung setzen
						ntd[wt].tvsb -= ( pTrWt[wt]->messw - nts[wt].Trma ) * nts[wt].Kpr / 100;
					}	
					else 
						ntd[wt].rlbegr = 0;					
				}
				else 
					ntd[wt].rlbegr = 0;					

				if ( ntd[wt].tvsb < nts[wt].Tvmi )
					ntd[wt].tvsb = nts[wt].Tvmi;						// Begrenzung min.
				else if ( ntd[wt].tvsb > nts[wt].Tvma )
					ntd[wt].tvsb = nts[wt].Tvma;						// Begrenzung max.
			}
		}

// Tauscher ein/ausschalten
		// Führungstauscher
		if ( pTvWt[fuehr]->stat == 0 )
		{
			if ( ntd[fuehr].wtEin == FALSE )
			{
				TauscherAus ( folge );
				TauscherAus ( fuehr );
				if ( pTvWt[fuehr]->messw <= ntd[fuehr].tvsb )
					ntd[fuehr].wtEin = TRUE;
			}
			
			else if ( pTvWt[fuehr]->messw > ( ntd[fuehr].tvsb + (int)nts[0].WtSchaltDiff ) )
			{																			// Fuehrungstauscher ist ein, ist er auszuschalten ?
				if ( ntd[folge].wtEin == TRUE )
					TauscherAus ( folge );
				else
					TauscherAus ( fuehr );
			}
		}
		else
		{
			TauscherAus ( folge );
			TauscherAus ( fuehr );
		}

		// Folgetauscher		 
		if ( ntd[fuehr].wtEin == TRUE )
		{
			if ( pTvWt[folge]->stat == 0 )
			{
				if ( ntd[folge].wtEin == TRUE )
				{
//					if ( abschFolgeWt || ( ntd[0].abwsumme - nts[0].Auskrit > 0 ) )
					if ( abschFolgeWt || ntd[0].modMwWtAus )
					{
						ntd[folge].wtEin = FALSE;
						TauscherAus ( folge );
					}
				}
//				else if ( ntd[0].abwsumme + nts[0].Einkrit < 0 )
				else if ( ntd[0].modMwWtEin )
					ntd[folge].wtEin = TRUE;	
			}
			else
				TauscherAus ( folge );
		}			

		ntd[NT1].gestoert = TauscherStoerung ( NT1 );	
		ntd[NT2].gestoert = TauscherStoerung ( NT2 );	

//-		if ( pTvAnl->stat == 0 )
//-		{
//-			ntd[0].abw = pTvAnl->messw - ntd[0].tvsb_anl;				// aktuelle Istwert-Sollwert-Differenz, Anlagenfuehler
//-			if ( ntd[folge].gestoert )
//-				ntd[0].abwsumme = 0;
//-			else
//-				AbweichInt ( );
//-		}
		
		ModulationMW ( );
					
		TauscherLaufzeit ( );

// Wechsel des Führungs-Tauschers erforderlich ?
// gewechselt wird bei Störung ( falls der Folge-Tauscher ungesört ist )
// oder laufzeitabhaengig ( wenn parametriert, d.h. WtWechseln != 0 ) 
// wenn der Fuehrungs-Tauscher fest vorgegeben ist, soll er nach gegangener Stoerung wieder zum Fuehrungs-Tauscher werden

		if ( ntd[folge].gestoert == 0 )												// Folge-Tauscher muss ungestört sein
		{
			if ( nts[0].FuehrWtFest != 0 && ( nts[0].FuehrWtFest != nts[0].FuehrWt	) ) 
				fuehrWtWechsel = TRUE;																												// Führungs-Tauscher fest vorgegeben
			else if ( ntd[fuehr].gestoert == TRUE )	
				fuehrWtWechsel	= TRUE;																												// Führungs-Tauscher als gestört erkannt
			else if ( nts[0].FuehrWtFest == 0 && nts[0].WtWechseln 
									&& ( ntd[fuehr].lzStd > ntd[folge].lzStd )
									&& ( ntd[fuehr].lzStd - ntd[folge].lzStd >= nts[0].WtWechseln ) )
				fuehrWtWechsel	= TRUE;																												// Führungs-Tauscher-Wechsel wg. Laufzeit
			else	
				fuehrWtWechsel = FALSE;					
		} 
	
		if ( fuehrWtWechsel == TRUE )									
		{ 
			if ( ntd[folge].wtEin == 0 )
				TauscherAus ( fuehr );					// vor Wechsel Führungs-Tauscher AUS, wenn Folge-Tauscher AUS ist
											
			nts[0].FuehrWt = ( nts[0].FuehrWt == 1 ) ? 2 : 1;
			ntd[0].abwsumme = 0;
			bicbus ( EEPADR, (char*)&nts[0], NT1_ADR, 1, BICWR ); // akt. Fuehrungskessel sichern	
// FuehrWt muss der erste Parameter der Struktur sein ! ####################################################################			
		}
		
		if ( ntd[NT1].gestoert )
			TauscherAus ( NT1 );													// Tauscher aus bei Stoerung										
		if ( ntd[NT2].gestoert )
			TauscherAus ( NT2 );													// Tauscher aus bei Stoerung										

		if ( sowi == TRUE )
		{
			TauscherAus ( folge );
			ntd[0].abwsumme = 0;
		}
	}

	if ( pTvAnl->stat == 0 )
		SollwertAnhebung ( pTvAnl );							// Tauscher-Sollwert anheben, wenn die Solltemp. am Anlagenvorlauf nicht erreicht wird
	
	TauscherRegeln ( NT1, TVSEK[NT1] );
	TauscherRegeln ( NT2, TVSEK[NT2] );
	
	// Ausgabe
	for ( wt = 0; wt < NTANZ; wt++ )
	{
		if ( nts[wt].Haut != TRUE )								// Automatik
		{
			if ( wt == nts[0].FuehrWt - 1 )
				Klappe ( wt, 1 );											// Klappe des Führungs-Tauschers immer offen
			else
				Klappe ( wt, ntd[wt].wtEin );

			if ( ntd[wt].zts == 0 )									// nur nach Neuberechnung
			{
				if ( ntd[wt].fahren > 0 )
				{		
					RNTAUF[wt]->wert = (char)ntd[wt].fahren;
					RNTZU[wt]->wert = 0;
				}
				else if ( ntd[wt].fahren < 0 )
				{
					RNTZU[wt]->wert  = (char)abs(ntd[wt].fahren);	
					RNTAUF[wt]->wert = 0;
				}

				RVENTNT[wt]->awert = ntd[wt].y_rel;
			}

			nts[wt].Haut = FALSE;										// evtl. Fehleingaben löschen
			ntd[wt].vzu	= 0;												// Handwerte löschen
			ntd[wt].vauf	= 0;
		}			
		else																			// Hand									
		{
			KLNTAZ[wt]->wert = nts[wt].KlAZ;
			
			RNTZU[wt]->wert = ntd[wt].vzu;					// Handwert[s] für Zufahren ausgeben
			if ( ntd[wt].vzu > 0 )									// Ausgabe aktiv ?
			{	 
				--ntd[wt].vzu;												// Zeit decrementieren
				ntd[wt].vauf = 0;											// Vorrang vor Auffahren 
				RNTAUF[wt]->wert = 0;
			}
			else																		// Ausgabe für Zufahren nicht aktiv
			{	
				RNTAUF[wt]->wert = ntd[wt].vauf;			// Handwert[s] für Auffahren ausgeben
				if ( ntd[wt].vauf > 0 )								// Ausgabe aktiv ?
					--ntd[wt].vauf;											// Zeit decrementieren
			}
			
			RVENTNT[wt]->awert = nts[wt].Vstell;		// Analogventil 0-100,0% 
			PUNTEA[wt]->wert = nts[wt].Puea;				// Pumpenrelais Handwert eintragen		
		}			
			
	}
			
}



/******************************************************************/
/***** Funktion : Integration der Istwert-Sollwert-Differenz	*****/
/***** steuert das Ein- und Ausschalten des Folgetauschs 			*****/
/***** Läuft zweinmal pro Minute													 		*****/

void AbweichInt ( void )
{
	int abw;
	
	abw = ntd[0].abw;
	if ( abw >= 0)															// Abweichung positiv ? ( Messwert > Sollwert )
	{																						// Ausschalten ?
		if ( ntd[0].abwAlt >= 0 )
		{																					// alte Abweichung auch positiv ? nur integr., wenn Kriterium nicht erreicht
			if ( ntd[0].abwsumme - nts[0].Auskrit <= 0 )
				ntd[0].abwsumme += abw/2;						// Vollwert addieren ( 2 mal pro Minute)
		}			
		else																			// bei Vorzeichenwechsel
			ntd[0].abwsumme = +1;										// Summe zurücksetzen
	}
	else																				// Abweichung ist negativ
	{	
		if ( ntd[0].abwAlt < 0 )	
		{																					// alte Abweichung auch negativ ? nur integr., wenn Kriterium nicht erreicht
			if ( ntd[0].abwsumme + nts[0].Einkrit >= 0	)				
				ntd[0].abwsumme += abw/2;						// Vollwert addieren ( 2 mal pro Minute)
		}
		else																			// bei Vorzeichenwechsel
			ntd[0].abwsumme = -1;										// Summe zurücksetzen
	}
	ntd[0].abwAlt = abw;												// beim nächsten Mal Altwert
}					



/******************************************/
/***** Funktion : Tauscherlaufzeiten  *****/
/***** Läuft zweinmal pro Minute 			*****/

void TauscherLaufzeit ( void ) 
{
	char wt;
	
	for ( wt = 0; wt < NTANZ; wt++ )
	{
		if ( ntd[wt].wtEin == TRUE )
		{	
			if ( ++ntd[wt].lzMin >= 120 )				// Minutenzähler
			{	
				ntd[wt].lzMin = 0;
				ntd[wt].lzStd++;									// Laufzeit-Std. erhöhen
				bicbus ( EEPADR, (char*)&ntd[wt].lzStd, WTH_ADR + ( wt*2 ), 2, BICWR );
			}
		}
	}		
}


/****************************************************************/
/***** Funktion : Ermittlung des Störzustands des Tauschers	*****/
/***** Läuft zweinmal pro Minute pro Tauscher 				 		 	*****/

char TauscherStoerung ( char wt )
{
	if ( ( DRMAXNT[wt]->bstat != NICHTV && DRMAXNT[wt]->bwert ) 
			|| ( TVSEK[wt]->stat != 0 )	)	
		
		return ( TRUE );
	
	else
		return ( FALSE );
}


/******************************************/
/***** Funktion : Tauscher abschalten *****/

void TauscherAus ( char wt )
{
	ntd[wt].wtEin = FALSE;
}


/**************************************/
/***** Funktion : Klappe Stellen  *****/
void Klappe ( char wt, char az )
{
	if ( az == 1 )									// Klappe öffnen
	{
		KLNTAZ[wt]->wert = 1;
		ntd[wt].klappeCtr = nts[wt].KlappeVerz * 60;
	}
	else
	{
		if ( ntd[wt].klappeCtr > 0 )
			--ntd[wt].klappeCtr;
		if ( ntd[wt].klappeCtr == 0 )
			KLNTAZ[wt]->wert = 0;			
	}
}
	

/****************************************************************************/
/***** Funktion : Anhebung der Tauscher-Solltemperatur				 					*****/
/***** der Tauscher-Sollwert ist um einen variablen Betrag anzuheben,  	*****/
/***** der von der Sollwert-Istwert-Differenz am Anlagenfühler abhängt  *****/
/***** und nach dem Standard-Regelalgorithmus zu ermitteln ist			 		*****/

void SollwertAnhebung ( mwsp* pT_ist )
{
	float fl_ei = 0;
	
	if ( ( ntd[0].zts_anh += 10 ) >= nts[0].Ts_anh )
	{																
		ntd[0].zts_anh = 0;												// Tastzeit in s * 10 
		
	// Berechnung der Sollwertanhebung
	// Sollwert entspr. Anforderung ntd[0].tvsb_anl
	// Istwert : Vorlauf Anlage 
	
	
		// Regelabweichung Soll - Ist
		fl_ei	= ( (float)ntd[0].tvsb_anl - (float)pT_ist->messw ) / 10;

		if ( fl_ei > 20 )
			fl_ei = 20;														// Begrenzung
			
	// -------------- PID-Regelalgorithmus ---------------------------------
	// Berechnung der relativen Stellgrößen
		
		if ( nts[0].Kp_anh > 0 )
			ntd[0].fl_y_rel_anh += Dy_rel ( nts[0].Kp_anh, nts[0].Kd_anh, nts[0].Ts_anh, nts[0].Tn_anh, 
												fl_ei, ntd[0].fl_ei1_anh, ntd[0].fl_ei2_anh );
		else
			ntd[0].fl_y_rel_anh	= 0;
						
	// Regelabweichungen merken
		if ( ntd[0].fl_y_rel_anh > 0 )
		{
			if ( ntd[0].fl_y_rel_anh > 20 )
				ntd[0].fl_y_rel_anh = 20;						// max. Anhebung 20 K				
			ntd[0].fl_ei2_anh = ntd[0].fl_ei1_anh;
			ntd[0].fl_ei1_anh = fl_ei;
		}

		else
		{																				// keine Absenkung
			ntd[0].fl_y_rel_anh = 0;
			ntd[0].fl_ei2_anh = 0;
			ntd[0].fl_ei1_anh = 0;				
		}
		ntd[0].y_rel_anh = (int)( ntd[0].fl_y_rel_anh * 10 );
					
	}
}


/**********************************************************/
/***** Funktion : Tauscher-Regelalgorithmus 					*****/

void TauscherRegeln ( char wt, mwsp *pTvWt )
{
	float fl_ei = 0;
	float fl_dy_rel = 0;
	int tvsb;

	// Regler-Tastzeit
	if( ++ntd[wt].zts < nts[wt].Ts/10 )
		 return;													// Tastzeit noch nicht erreicht
	ntd[wt].zts = 0;

	tvsb = ntd[wt].tvsb;

	if ( tvsb > KEINE_ANFORD && pTvWt->stat == 0 && ntd[wt].wtEin == TRUE )
	{
		// Dämpfung des berechneten Sollwertes: tvsb --> gedämpfte tsol
		if ( ntd[wt].regstart == 0 ) 												// nach Reset
		{	 
			 ntd[wt].tsol	= ntd[wt].tvsb;						// keine Filterung
			 ntd[wt].fl_tsol = (float)ntd[wt].tsol / 10;
			 ntd[wt].regstart	= 1;
		}	 
		else
		{
			ntd[wt].fl_tsol = g_filter(ntd[wt].fl_tsol, ntd[wt].tvsb, nts[wt].Ts, nts[wt].Fzk);
			ntd[wt].tsol = (int)(ntd[wt].fl_tsol * 10);						// für Anzeige
		}
		
		// Regelabweichung Soll - Ist
		fl_ei	= ntd[wt].fl_tsol - (float)pTvWt->messw / 10;
			
		// -------------- PID-Regelalgorithmus ---------------------------------
		// Berechnung der relativen Stellgrößen
		fl_dy_rel = Dy_rel ( nts[wt].Kp, nts[wt].Kd, nts[wt].Ts, nts[wt].Tn, 
														fl_ei, ntd[wt].fl_ei1, ntd[wt].fl_ei2 );
		
		ntd[wt].fl_y_rel += fl_dy_rel;
				
		//---------------------------------------------------------------------
		// Regelabweichungen merken
		ntd[wt].fl_ei2 = ntd[wt].fl_ei1;
		ntd[wt].fl_ei1 = fl_ei;						
		
		// Berechnung und Ausgabe der absoluten Stellgrößen
		// --------------------------------------------------------------------
		// Ausgabe an 3-Punkt Ventil
		if ( nts[wt].Vst > 0 )
		{	
			ntd[wt].stellzeit = (int)( fl_dy_rel * (float)nts[wt].Hub * 60 / (float)nts[wt].Vst ); // [0.01 s]
			ntd[wt].stellsum += ntd[wt].stellzeit;
			if ( abs(ntd[wt].stellsum ) >= 100 )						// ab 1 Sekunde ausgeben
			{
				ntd[wt].fahren		= ntd[wt].stellsum / 100;		// nur ganze Sekunden fahren
				ntd[wt].stellsum	= ntd[wt].stellsum % 100; 	// Rest merken
			}
			else
				ntd[wt].fahren = 0;
		}	
		
		// Ausgabe an 0-10V Ventil
		if ( ntd[wt].fl_y_rel > 100 )											// Begrenzung max 100 %
			 ntd[wt].fl_y_rel = 100;
		if ( ntd[wt].fl_y_rel < 0 )												// Begrenzung min 0%
			 ntd[wt].fl_y_rel  = 0;
			
		ntd[wt].y_rel = (int)(ntd[wt].fl_y_rel * 10);			// Zur Anzeige und Weiterverarbeitung
		
		// Parameterumwandlung float to int
		ntd[wt].ei = (int)(fl_ei * 10);										// [0,1%] zur Anzeige
	}

	else	// 	tvsb == 0, Istwert gestört
	{	
		ntd[wt].fl_ei1	  = 0;
		ntd[wt].ei				= 0;
		ntd[wt].fahren		= -255;
		ntd[wt].fl_y_rel	= 0;
		ntd[wt].y_rel		= 0;
		ntd[wt].regstart = 0;
		ntd[wt].tsol	= 0;
	}	
	
}



/*****************************************************/
void ModulationMW ( void )
{
	char wt;
	char n = 0;
	UINT mod = 0;
		
	for ( wt = NT1; wt <= NT2; wt++ )
	{
		if ( nts[wt].Haut == 0 && ntd[wt].wtEin == 1 )
		{
			++n;
			mod += ntd[wt].y_rel;
		}
	}
	ntd[0].modulatMW = ( n > 0 ) ? mod / n : 0;

	if ( ntd[0].modulatMW > nts[0].WtAusMod	&& ntd[0].modulatMW < nts[0].WtEinMod )
		ntd[0].wtModCtr = 0;	
	
	if ( ntd[0].modulatMW <= nts[0].WtAusMod && ntd[NT1].wtEin == 1 && ntd[NT2].wtEin == 1 )
	{
		if ( ntd[0].wtModCtr > 0 )
			ntd[0].wtModCtr = 0;
		else if ( abs(ntd[0].wtModCtr) < 0x8000 )
			ntd[0].wtModCtr -= 30;	
	}		
	else if ( ntd[0].modulatMW >= nts[0].WtEinMod && ( ntd[NT1].wtEin == 1 || ntd[NT2].wtEin == 1 ) )
	{
		if ( ntd[0].wtModCtr < 0 )
			ntd[0].wtModCtr = 0;
		else if ( ntd[0].wtModCtr < 0x7FFF )
			ntd[0].wtModCtr += 30;
	}
	
	ntd[0].modMwWtAus = ( ntd[0].wtModCtr < 0 && ( abs(ntd[0].wtModCtr) >= nts[0].WtAusModVerz	* 60 ) ) ? 1 : 0;
	ntd[0].modMwWtEin = ( ntd[0].wtModCtr >= nts[0].WtEinModVerz	* 60 ) ? 1 : 0;

}
