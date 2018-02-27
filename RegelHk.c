/*		Task:	RegelHk			*/

//#####ulsch: 25.02.05 bei Bedarfsabsenkung und kein Frostschutz : hk_abschalten

#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "unoinitext.h"

// Unterprogramme
char  tvsoll_ber(char hk);
void  raum_kaskade(char hk, char ada);
void  rueckl_begr(char hk, char ps);
char  test_aufhe(char hk);
char TestEstrichProg ( char hk );
char RaumAbschaltung	( char hk );

/*---------------------- Task RegelHk -----------------------------------*/
// Gemeinsamer Code für bis zu 4 alternierende Regelkreise
// Zeitbasis für einen Regelkreis : 1,0 Sekunden
// Zeitbasis für den Taskaufruf:		1,0 Sekunden / HKMAX

void RegelHk(void)
{
	static char hk = HKMAX;
	
	float fl_ei = 0;
	float fl_dy_rel = 0;	
	float fl_Y_rel_beg = 0;
	float fl_Y_rel_min = 0;
	char  hkAbschalten = 0;
	char	tvret;
	char hkAbsch;
	int wwvorKorr[2];
	
	// Ventilöffnung
	char vo, er;
	UINT abschBeg, abschEnd, aktuell;
	
	// Lokale Pointer auf Hk-Strukturen
	HkDynam    *pHkd;	
	HkStandard *pHks;

	if(++hk >= HKMAX)										// Regelkreis-Index erhöhen
		hk = 0;
	
	// Projektabfrage, ob Regelkreis aktiv
	if(Projekte[proj_typ].hkcode[hk] == 0)
		return;													// Heizkreis nicht aktiv
	
	// Strukturpointer zuweisen
	pHkd = &hkd[hk];	
	pHks = &hks[hk];

	// Ventilstellungsbegrenzungen int to float
	fl_Y_rel_beg = (float)pHks->Y_rel_beg / 10;			// Beginn der Ventilöffnung
	fl_Y_rel_min = (float)pHks->Y_rel_min / 10;			// Minimalbegrenzung
	
	//--------------------------------------------------------------------------
	// Betriebsart, Absenkanzeige
	//--------------------------------------------------------------------------
	if(pHkd->absenSchalter == 1)							
	{	// Schalterabsenkung
		pHkd->regie  = REG_SCHALTER;
		pHkd->absen  = pHkd->absenSchalter ;			// Zustand  Absenkphase
		pHkd->aufhe  = 0 ;												// Zustand  Aufheizphase
		pHkd->absch  = pHkd->abschSchalter ;			// Zustand	Abschaltphase
		pHkd->hunt   = 0;													// Zustand  Heizunterbrechung
		pHkd->stuetz = pHkd->stuetzSchalter;			// Zustand 	Stützbetrieb
		pHkd->frost  = pHkd->frostSchalter ;			// Zustand	Frostschutzbetrieb
	}
	else if(pHkd->vorra_bed == TRUE)						// bei Vorrang Bedarfsabsenkung vor Zeitabsenkung
	{	// Bedarfsabsenkung
		pHkd->regie  = REG_BEDARF;
		pHkd->absen  = abs_bed[hk].absen ;				// Zustand  Absenkphase
		pHkd->aufhe  = abs_bed[hk].aufhe ;				// Zustand  Aufheizphase
		pHkd->absch  = abs_bed[hk].absch ;				// Zustand	Abschaltphase
		pHkd->hunt   = abs_bed[hk].hunt  ;				// Zustand  Heizunterbrechung
		pHkd->stuetz = abs_bed[hk].stuetz;				// Zustand 	Stützbetrieb
		pHkd->frost  = abs_bed[hk].frost ;				// Zustand	Frostschutzbetrieb
	}
	else	// Zeitabsenkung
	{
		if(pHkd->absenFerien == 1)								// Ferienbetrieb
		{
			pHkd->regie  = REG_FERIEN;
			pHkd->absen  = pHkd->absenFerien ;			// Zustand  Absenkphase
			pHkd->aufhe  = 0 ;											// Zustand  Aufheizphase
			pHkd->absch  = pHkd->abschFerien ;			// Zustand	Abschaltphase
			pHkd->hunt   = 0;												// Zustand  Heizunterbrechung
			pHkd->stuetz = pHkd->stuetzFerien;			// Zustand 	Stützbetrieb
			pHkd->frost  = pHkd->frostFerien ;			// Zustand	Frostschutzbetrieb
		}
		else																			// Normal Zeitabsenkung
		{		
			pHkd->regie  = REG_ZEIT;
			pHkd->absen  = abs_ram[hk].absen ;			// Zustand  Absenkphase
			pHkd->aufhe  = abs_ram[hk].aufhe ;			// Zustand  Aufheizphase
			pHkd->absch  = abs_ram[hk].absch ;			// Zustand	Abschaltphase
			pHkd->hunt   = abs_ram[hk].hunt  ;			// Zustand  Heizunterbrechung
			pHkd->stuetz = abs_ram[hk].stuetz;			// Zustand 	Stützbetrieb
			pHkd->frost  = abs_ram[hk].frost ;			// Zustand	Frostschutzbetrieb
		}	
	}				
	switch(pHkd->regie)
	{
		case REG_SCHALTER:
			memcpy(pHkd->regie_txt, TXT_SCHALTER, 15);
			break;
		case REG_BEDARF:
			memcpy(pHkd->regie_txt, TXT_BEDARF, 15);
			break;
		case REG_FERIEN:
			memcpy(pHkd->regie_txt, TXT_FERIEN, 15);
			break;
		case REG_ZEIT:
			memcpy(pHkd->regie_txt, TXT_ZEIT, 15);
			break;
		default:
			memcpy(pHkd->regie_txt, TXT_DEFAULT, 15);
			break;
	}		
				


	//--------------------------------------------------------------------------
	// Abfrage Handbetrieb
	//--------------------------------------------------------------------------
	if(pHks->Haut == TRUE)			
	{	
		RZU[hk]->wert = pHkd->vzu;								// Handwert[s] für Zufahren ausgeben
		if(pHkd->vzu > 0)													// Ausgabe aktiv ?
		{	 pHkd->vzu -= 1;												// Zeit decrementieren
			 pHkd->vauf = 0;												// Vorrang vor Auffahren 
		}                               					
		else																			// Ausgabe für Zufahren nicht aktiv
		{	RAUF[hk]->wert = pHkd->vauf;						// Handwert[s] für Auffahren ausgeben
			if(pHkd->vauf > 0)											// Ausgabe aktiv ?
				 pHkd->vauf -= 1;											// Zeit decrementieren
		}                               					

		RVENT[hk]->awert = pHks->Vstell;								// Analogventil 0-100,0% 
		hkd[hk].fl_y_rel = (float)pHks->Vstell / 10;

		if(pHkd->vzu > 0 || pHkd->vauf > 0)				// return, solange Ausgabe läuft (wegen 1 Sekunden Takt) 
			return;
	}                                 					
	else																				// Automatikbetrieb
	{	pHks->Haut = 0;														// evtl. Fehleingaben löschen
		pHkd->vzu	= 0;														// Handwerte löschen
		pHkd->vauf	= 0;
	}
	//----------------------------------------------------------------------------
	
	// Regler-Tastzeit
	if(++pHkd->zts < pHks->Ts/10)
		 return;																	// Tastzeit noch nicht erreicht
	pHkd->zts = 0;	 


	// Sommer/Winter-Automatik
	//-----------------------------------------------------------------------------
	// Entscheidung: zentrale Werte aus ANL oder Extrawerte aus HK
	if(pHks->SoWiExtra == 0)
	{	// zentrale Werte auf Heizkreis kopieren
		pHks->Tae  = Tae;
		pHks->Taa  = Taa;
		pHkd->sowi = sowi;
	}
	else
	{	// Extrawerte für den Heizkreis ausgewählt
		if(ta1mh.messw <= pHks->Tae) 							// ta1mh <= Winter-Ein-Temp. ?
			pHkd->sowi = 0;													// Winterbetrieb
		else if(ta1mh.messw >= pHks->Taa)					// ta1mh >= Sommer-Ein-Temp. ?
			pHkd->sowi = 1;													// Sommerbetrieb
	}		

	pHkd->somme = pHks->Swhk;										// Sommer/Winter-Automatik nach Statusanzeige		
	if(pHks->Swhk > 0)													// Automatik aktiv ?
	{	
		// Außentemperatur-Heizgrenze abtesten
		if(ta1m.stat != 0)
			pHkd->heiz_grenz = 0;										// Merker
		else
		{ if(pHkd->heiz_grenz == 1)								// war Merker gesetzt ?
			{	if (ta1m.messw < pHks->TaHeizgrenze - 20)	// Hysterese 2,0K
					pHkd->heiz_grenz = 0;
			}
			else
			{	if (ta1m.messw >= pHks->TaHeizgrenze)
					pHkd->heiz_grenz = 1;
			}
		}			
		
		pHkd->somme = pHkd->sowi | pHkd->heiz_grenz;	// Sommer/Winter-Zustand oder Heizgrenz-Zustand nach Statusanzeige	

		if(pHkd->somme > 0)												// Sommerbetrieb ?
		{	
			if(hkAbschalten == 0)
				hkAbschalten = 1;
		}
	}
	
	// GWW-Vorrang
	//-------------------------------------------------------------------------------
//-	pHkd->wwvorKorr = 0;							// voreinstellen
//-	pHkd->vorra = pHks->Wwvh;					// Schalter Vorrang hk nach Statusanzeige
//-	if(pHks->Wwvh > 0)								// Schalter gesetzt ?
//-	{	
//-		pHkd->vorra = wwd[WW1].wwvor && wwd[WW1].zvorrad;
//-		if ( pHkd->vorra )							// Vorrangbetrieb ?
//-		{	if(pHks->Kpww >= 1000)				// wenn Kpww >= 10,00
//-			{	
//-			if(hkAbschalten == 0)
//-				hkAbschalten =2;						// Abschalten ohne Starttemp. init
//-			}
//-			else													// gleitenden Absenkwert errechnen
//-				pHkd->wwvorKorr = (int) ( (float)wwd[WW1].ei_ww * ((float)pHks->Kpww / 100) );
//-
//-			#if SP_WW1 == 0
//-				if ( pHkd->wwvorKorr > 100 )
//-					pHkd->wwvorKorr = 100;		// Siefke : bei WW-Bereitung ohne Speicher Absenkung auf 10 K begrenzt
//-			#endif												// dafuer ohne zeitl. Begrenzung moeglich
//-																		// hier : Wiedereinschalten nach TvorrZeit									
//-		}
//-	}	

	// GWW-Vorrang, zwei WW-Kreise
	if ( WWANZ && hkAbschalten == 0 )
	{

		pHkd->vorra = ( ( pHks->Wwvh[WW1] && wwd[WW1].wwvor && wwd[WW1].zvorrad )
									|| ( WWANZ > 1 && ( pHks->Wwvh[WW2] && wwd[WW2].wwvor && wwd[WW2].zvorrad ) ) );

		wwvorKorr[WW1] = 0;
		wwvorKorr[WW2] = 0;
		
		if ( pHks->Wwvh[WW1] && wwd[WW1].wwvor && wwd[WW1].zvorrad )
		{
//			pHkd->vorra = 1;
			if ( pHks->Kpww[WW1] >= 1000 )
				hkAbschalten = 2;
			else
				wwvorKorr[WW1] = (int) ( (float)wwd[WW1].ei_ww * ((float)pHks->Kpww[WW1] / 100) );			
		}
		if ( WWANZ > 1 && hkAbschalten == 0 )
		{
			if ( pHks->Wwvh[WW2] && wwd[WW2].wwvor && wwd[WW2].zvorrad )
			{
//				pHkd->vorra = 1;
				if ( pHks->Kpww[WW2] >= 1000 )
					hkAbschalten = 2;
				else
					wwvorKorr[WW2] = (int) ( (float)wwd[WW2].ei_ww * ((float)pHks->Kpww[WW2] / 100) );			
			}
		}		
		if ( hkAbschalten == 0 )
			pHkd->wwvorKorr = ( wwvorKorr[WW1] >= wwvorKorr[WW2] ) ? wwvorKorr[WW1] : wwvorKorr[WW2];	
		else
			pHkd->wwvorKorr = 0;		
		#if SP_WW1 == 0
		if ( pHkd->wwvorKorr > 100 )
			pHkd->wwvorKorr = 100;		// Siefke : bei WW-Bereitung ohne Speicher Absenkung auf 10 K begrenzt
		#endif
	}
	else
	{
		pHkd->vorra = 0;		
		pHkd->wwvorKorr = 0;
	}
	
		
	//---------------------------------------------------------------------------------------------------
	// Sonderphasen (Aufheizphase, Stützbetrieb, Frostschutzbetrieb) testen
	// --> tvsb <-- wird entsprechend gesetzt
	//---------------------------------------------------------------------------------------------------

	hkd[hk].kaskaKorr = 0;
	 
	if ( pHkd->somme == TRUE )	
		; 
	else if(test_aufhe(hk) == 1)																// 1 = Sonderphase
		hkAbschalten = 0;
	else	
	{	
		// ---------------------------- Normalphase -----------------------------------------------
		// Berechnung der Heizungsvorlauftemp.(tvsb) über die Heizkennlinie
		tvret = tvsoll_ber(hk);	// wenn ok, dann tvsb berechnet(tvret=1), sonst tvsb = Tvmi (tvret= 0)
		//-----------------------------------------------------------------------------------------

		//-----------------------------------------------------------------------------------------
		// Anlagenschalter steht auf Nichtnutzung HK 
		//-----------------------------------------------------------------------------------------
		if(pHkd->absenSchalter == 1)	
		{	//------------------------------------------------
			// Schalter-Regie
			//------------------------------------------------
			if(pHkd->abschSchalter == TRUE)										// Abschalten ermittelt in Absenk.c (TestSchalter)
			{																									// kommt bei Frostbetrieb hier nicht vorbei
				if(hkAbschalten == 0)
					hkAbschalten = 1;
			}				
			else																							// kein Abschalten
			{	if(tvret)
					pHkd->tvsb -= pHkd->tvabSchalter;							// Berechn. Absenkwert abziehen
			}		
		}
		else
		{	//-----------------------------------------------------------------------------------------
			// Regel- oder Absenkphasen
			//-----------------------------------------------------------------------------------------
			if(pHkd->vorra_bed == TRUE)													// Vorrang Bedarfsabsenkung vor Zeitabsenkung
			{	
				//------------------------------------------------
				// Bedarfs-Regie
				//------------------------------------------------
				// Kaskade mit Kaskadewerten vom RIEcon50
				if(pHkd->bedarf == BEDARF_JA)											// Nur in der Nutzungszeit
				{	
					if ( pHkd->tisobed > 10 && pHkd->tibed > 10 )  	// nur wenn Soll- und Ist-Temp.> 1,0 °C ( sonst Fehler )
						raum_kaskade(hk, 1);													// 1 = R50-Kaskadewerte benutzen
				}						
				else
				{	// Bedarfsabsenkung bei 'Kein  Bedarf'
					if(abs_bed[hk].tvab > 0)												// wenn Absenkwert eingetragen (berechnet in Bedarf.c)
					{	if(abs_bed[hk].absch == TRUE  )								// Abschalten ermittelt in Bedarf.c
						{																							// kommt bei Frostbetrieb hier nicht vorbei
							if(hkAbschalten == 0)
								hkAbschalten = 1;
						}				
						else																					// kein Abschalten
						{	if(tvret)
								pHkd->tvsb -= abs_bed[hk].tvab;						// Absenkwert benutzen
						}								
					}
				}	
			}						
			else	// Zeitabsenkung oder Ferienabsenkung
			{
				//-------------------------------------------------
				// Ferien-Regie
				//-------------------------------------------------
				if( pHkd->vorra_bed == FALSE && ferienStat == 1 && pHkd->tvabFerien )	
				{	
					if(pHkd->abschFerien == TRUE)											// Abschalten ermittelt in Absenk.c (TestFerien)
					{																									// kommt bei Frostbetrieb hier nicht vorbei
						if(hkAbschalten == 0)
							hkAbschalten = 1;
					}				
					else																							// kein Abschalten
					{	if(tvret)
							pHkd->tvsb -= pHkd->tvabFerien;								// Berechn. Absenkwert abziehen
					}		
				}
				else
				{	//-----------------------------------------------
					// Zeit-Regie
					//-----------------------------------------------
					if(abs_ram[hk].tvab == 0)													// kein Absenkwert eingetragen (berechnet in Absenk.c)
					{	// Kaskade mit Raumtemperaturfühler
						if(TI[hk]->stat == 0 && abs_ram[hk].absen == 0 )// wenn Status ok und nicht in Absenkung
							raum_kaskade(hk, 0);													// 0 = keine R50-Kaskadewerte benutzen
					}
					else
					{	// Absenkung nach Absenktabellen
						if(abs_ram[hk].absch == TRUE  )									// Abschalten ermittelt in Absenk.c
						{																								// kommt bei Frostbetrieb hier nicht vorbei
							if(hkAbschalten == 0)
								hkAbschalten = 1;
						}				
						else																						// kein Abschalten
						{	if(tvret)
								pHkd->tvsb -= abs_ram[hk].tvab;							// Absenkwert benutzen
						}
					}
				}											
			}
		}	
		
		// Gleitender Warmwasservorrang, errechneten Absenkwert abziehen
		if(tvret)
			pHkd->tvsb -= pHkd->wwvorKorr;
		
		if(pHkd->tvsb < pHks->Tvmi)				// Minimaltemp. nicht unterschreiten	
			pHkd->tvsb = pHks->Tvmi;
		
		// Rücklauftemperaturbegrenzung
		pHkd->rlbegr = 0;									// Statusanzeige Rücklauftemp.begrenzung löschen
		if(pHks->Psbegr > 0)							// Begrenzung auf Primär-Rücklauftemp. ?
		{	pHks->Psbegr = 1;								// evtl. Fehlbedienung korrigieren
			if(TRP[ANL]->stat == 0)  				// Primär-Rücklauffühler ok ?
				rueckl_begr(hk, 1);						// 1 = Begrenzung auf Primär-Rücklauftemp.
		}
		else															// Begrenzung auf Sekundär-Rücklauftemp. 
		{	if(TRS[hk]->stat == 0)					// Sekundär-Rücklauffühler ok ?
				rueckl_begr(hk, 0);						// 0 = Begrenzung auf Sekundär-Rücklauftemp.
		}		
	
	}	// ------------------------ Ende tvsb-Berechnung in der Normalphase ----------------------------------------


	if ( pHkd->somme == FALSE && FRGHK[hk]->bstat == 0 )		// Freigabe wird im Sommer ignoriert, Sommer dominiert
	{																												// Freigabesignal ist definiert
		if ( FRGHK[hk]->bwert == 0 )
		{																											// Heizkreis nicht freigegeben
			if ( ta1m.stat == 0 && ta1m.messw > pHks->TaFrostgrenze )
			{	if(hkAbschalten == 0)
					hkAbschalten = 1;
			}
			else
				pHkd->tvsb = pHks->Tvmi;
		}
	}

	// Heizkreis abschalten mangels Bedarf
	hkAbsch = RaumAbschaltung	( hk );
	if ( hkAbschalten == 0 )
		hkAbschalten = hkAbsch;	
	
	// Maximalwertbegrenzung
	if(pHkd->tvsb > pHks->Tvma)
		 pHkd->tvsb = pHks->Tvma;
	else if(pHkd->tvsb < pHks->Tvmi)
		 pHkd->tvsb = pHks->Tvmi;	


// Test: Estrich-Programm
	pHkd->estrichProg = TestEstrichProg ( hk );
	if ( pHkd->estrichProg == TRUE )
	{
		pHkd->tvsb = pHkd->estrichTemp * 10;
		if (pHkd->tvsb > pHks->Tvma )
	 		pHkd->tvsb = pHks->Tvma;
	 	hkAbschalten = 0;							// alle Abschaltbedingungen ( Sommer ! ) sind unwirksam
	}
				
#if LEIST_BEGR > 0
	if ( grenzLeistung.begrenzung > 0 && grenzLeistung.leistBegr > ( GrenzLeistung.GrenzLeistg - GrenzLeistung.LeistgHyst ) )
	{
		pHkd->leistBegrAbsenk = (UINT)( ( (ULONG)grenzLeistung.leistBegr - ( GrenzLeistung.GrenzLeistg - GrenzLeistung.LeistgHyst ) )
																			 * pHks->XpLeistBegr * 10 / ( GrenzLeistung.GrenzLeistg - GrenzLeistung.LeistgHyst ) );
		if ( pHkd->tvsb - pHks->Tvmi > pHkd->leistBegrAbsenk )
			pHkd->tvsb -= pHkd->leistBegrAbsenk;
		else
			pHkd->tvsb = pHks->Tvmi;		
	}
	else
		pHkd->leistBegrAbsenk = 0;
#endif				


// Abschalten während der von der Einzelraum-Regelung erzwungenen Ventilöffnung
// nicht wenn Ta zu gering, nicht bei Abgasmessung oder aktiviertem Estrich-Programm
	vo = FALSE;
	
	if ( hk == HK1 )
		er = ( BEDRAUM_HK1 == 1	&& FBH_HK1 == 0 ) ? 1 : 0;
	else if ( hk == HK2 )
		er = ( BEDRAUM_HK2 == 1	&& FBH_HK2 == 0 ) ? 1 : 0;
	else if ( hk == HK3 )
		er = ( BEDRAUM_HK3 == 1	&& FBH_HK3 == 0 ) ? 1 : 0;
	else if ( hk == HK4 )
		er = ( BEDRAUM_HK4 == 1	&& FBH_HK4 == 0 ) ? 1 : 0;
	else 
		er = FALSE;		


	if ( er && pHks->VentiloeffngAbs && ssfEinAnl == FALSE && pHkd->estrichProg == FALSE )
	{
		if ( pHks->VentiloeffngTag & wotbit() )
		{
			abschBeg = pHks->VentiloeffngBeg.std * 60 + pHks->VentiloeffngBeg.min;
			abschEnd = pHks->VentiloeffngEnd.std * 60 + pHks->VentiloeffngEnd.min;
			aktuell = Std * 60 + Min;	
			
			if ( aktuell >= abschBeg && aktuell < abschEnd )
			{
				vo = TRUE;
				if ( pHks->VentiloeffngAbs >= 500 && ta1m.messw > pHks->GrenztempAbschalten )
					hkAbschalten = 1;
				else
				{
					pHkd->tvsb -= pHks->VentiloeffngAbs;
					if ( pHkd->tvsb < pHks->Tvmi )
						pHkd->tvsb = pHks->Tvmi;	
				}
			}		
		}
	}
	pHkd->ventiloeffng = vo;


	//-------------------------------------------------------------------
	// Untersuchung auf hkAbschalten
	//-------------------------------------------------------------------

	#if SSF == 1
	if ( ssfEinAnl == TRUE )							// Abgasmessung
	{
		if ( hkAbschalten )
		{
			hkAbschalten = 0;									// Programm läuft nicht über Funktion hk_abschalten (),
			pHkd->tvsb  = 0;									// der Kessel wird bei Abgasmessung immer eingeschaltet. Wenn im Sommer jedoch tvsb
		}																		// verändert würde, schaltete bei Mehr-Kessel-Anlagen u.U. nicht nur der zu messende.
	}
	#endif	
	
	if(hkAbschalten != 0)
	{
		hk_abschalten ( hk, hkAbschalten )	;
		return;																		// TASK ENDE	!!!!!
	}	

	//-------------------------------------------------------------------

//Schwerin: wenn Druck Rücklauf sekundär aufgeschaltet
//-	// Heizung abschalten bei Druckalarm
//-	if ( hks[hk].HzgAusSMDruck && ( anaInp[U2].ugwSM || anaInp[U2].ogwSM ) )
//-	{
//-		hkd[hk].zpu = 0;								// Pumpe ohne Nachlauf abschalten
//-		hk_abschalten ( hk, 1 )	;
//-		return;																		// TASK ENDE	!!!!!
//-	}				

		// Schornsteinfeger-Funktion. Solltemp.wird bei laufender Estrich-Trocknung	nicht verändert !	
	#if SSF == 1
	if ( ssfEinAnl == TRUE && pHkd->estrichProg == FALSE )
	{
		pHkd->tsol = pHks->Tvma;
		pHkd->fl_tsol	= (float)pHkd->tsol / 10;
	}
	else
	#endif	

	// Dämpfung des berechneten Sollwertes: tvsb --> gedämpfte tsol
	if( (pHkd->regstart == 0) || (pHkd->wwvorKorr > 0) )	// nach Reset oder bei gleit. WW-Vorrang
	{	 pHkd->tsol			= pHkd->tvsb;												// keine Filterung
		 pHkd->fl_tsol	= (float)pHkd->tsol / 10;
		 pHkd->regstart	= 1;
	}	 
	else
	{
		pHkd->fl_tsol = g_filter(pHkd->fl_tsol, pHkd->tvsb, pHks->Ts, pHks->Fzk);
		pHkd->tsol 	  = (int)(pHkd->fl_tsol * 10);					// für Anzeige
	}
	//	----------------------------------------------------------------------
	//	Reglertyp: P- / PID-Regler
	//	----------------------------------------------------------------------
	// Test Vorlauffühler
	if(TVS[hk]->stat == 0)	// Status: 0...Ok, 41H...Überlauf,  21H...Unterlauf
	{	
		// Regelabweichung Soll - Ist
		fl_ei	= pHkd->fl_tsol - (float)TVS[hk]->messw / 10;
		
		// -------------- PID-Regelalgorithmus ---------------------------------
		// Berechnung der relativen Stellgrößen
		fl_dy_rel = Dy_rel ( pHks->Kp, pHks->Kd, pHks->Ts, pHks->Tn, 
														fl_ei, pHkd->fl_ei1, pHkd->fl_ei2 );

		pHkd->fl_y_rel += fl_dy_rel;
		
		pHkd->dy_rel	= (int)(fl_dy_rel * 1000);					// für debug

		//---------------------------------------------------------------------
		// Regelabweichungen merken
		pHkd->fl_ei2 = pHkd->fl_ei1;
		pHkd->fl_ei1 = fl_ei;						
		// Parameterumwandlung float to int
		pHkd->ei = (int)(fl_ei * 10);											// [0,1%] zur Anzeige
		
		//--------------------------------------------------------------------------
		// Abfrage Handbetrieb
		//--------------------------------------------------------------------------
		if(pHks->Haut == TRUE)			
			hkd[hk].fl_y_rel = (float)pHks->Vstell / 10;		// PID-Wert durch Handwert überschreiben
		else
		{
			// Berechnung und Ausgabe der absoluten Stellgrößen
			// --------------------------------------------------------------------
			// Ausgabe an 3-Punkt Ventil
			if ( pHks->Vst > 0 )
			{	
				pHkd->stellzeit = (int)( fl_dy_rel * (float)pHks->Hub * 60 / (float)pHks->Vst ); // [0.01 s]			
				pHkd->stellsum += pHkd->stellzeit;			
				if(abs(pHkd->stellsum) >= 100)								// ab 1 Sekunde ausgeben
				{
					pHkd->fahren		= pHkd->stellsum / 100;			// nur ganze Sekunden fahren
					pHkd->stellsum	= pHkd->stellsum % 100; 		// Rest merken
					if(pHkd->fahren > 0)		
						RAUF[hk]->wert = (char)pHkd->fahren;
					else
						RZU[hk]->wert  = (char)abs(pHkd->fahren);	
				}
			}	
			
			// Ausgabe an 0-10V Ventil
			
			// Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
			pHkd->fl_y_rel = anti_windup( pHkd->fl_y_rel, pHks->Kp, pHks->Wup, fl_ei );		// in Funktion.c
			pHkd->si_y_rel = (int)(pHkd->fl_y_rel * 10);		// debug
			
			// Abfrage Ventilstellungsbegrenzungen			
			if(pHks->Y_rel_beg > 0)													// Ventilöffnungsbeginn gesetzt ?
			{
				if(fl_dy_rel > 1)															// nur bei positiver Änderung
				{	if(pHkd->fl_y_rel < fl_Y_rel_beg)						// und Wert kleiner als Ventilöffnungsbeginn,
						 pHkd->fl_y_rel = fl_Y_rel_beg;						// dann Setzen auf Ventilöffnungsbeginn
				}
			}	
			if(pHks->Y_rel_min > 0)													// Minimalbegrenzung gesetzt ?
			{			
				if(pHkd->fl_y_rel  < fl_Y_rel_min)						// Wert kleiner als Minimalbegrenzung,								
					 pHkd->fl_y_rel  = fl_Y_rel_min;						// dann setzen auf Minimalbegrenzung
			}		 
				
			// Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
			pHkd->y_rel = y_stell(pHkd->fl_y_rel);					// in Funktion.c

			RVENT[hk]->awert = pHkd->y_rel;									// Ausgabe an Stellventil
		}
	}	
		
}

/*---------------------- Ende Task RegelHk -----------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Berechnung der Vorlauf-Soll-Temperatur über die Heizkennlinie */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  IN: hk.....Heizkreis
// OUT:	tvsb...berechnete Vorlauftemperatur [0.1°C]
char tvsoll_ber(char hk)
{
	float fl_tvsb, fl_steigung, fl_niveau, fl_tadiff;
	char ret;
	
	fl_tvsb = fl_steigung = fl_niveau = fl_tadiff = 0;
	
	fl_steigung		=  (float)hks[hk].Anst / 100;
	fl_niveau			=  (float)hks[hk].Tvpa / 10;
	fl_tadiff			=  (float)hks[hk].Taha / 10 - (float)ta1m.messw / 10;
	
	if(fl_tadiff < 0)		// wenn Ta größer als Ta am Heizkurvenanfang
	{	// Vorlauf Minimum einstellen
		hkd[hk].tvsb = hks[hk].Tvmi;
		ret = 0;					// Return not ok
	}
	else
	{	// Vorlauf Berechnung
		fl_tvsb = fl_niveau + fl_steigung * fl_tadiff + (float)0.05; //mit Rundung auf 1/10
		// float to integer
		hkd[hk].tvsb = (int)(fl_tvsb * 10);
		ret = 1;					// Return ok
	}
	hkd[hk].tvsb_hkl = hkd[hk].tvsb;	// Merken für debug
	
// ##### ulsch : Adaption = temperaturabhaengige Korrektur der Heizkurve
	hkd[hk].tvsb += hkd[hk].adaptKorr;
	
	
	return(ret);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Raumtemperatur-abhaengige TVSOLL-Verschiebung								 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  IN: hk.....Heizkreis
//			ada....1 = Soll. und Isttemp.  von R50 benutzen
//			tvsb...schon berechnete  Vorlauftemperatur (Heizkennlinie)
// OUT:	tvsb...verschobene Vorlauftemperatur
void raum_kaskade(char hk, char ada)
{
	int mwAdap = 0;
	long kas;
	int eik = 0;		

		
//ulsch: wenn eine Adaption der Heizkennlinie durchgeführt wird, soll die Kaskade nur zur Anhebung des Vorlauf-Sollwertes 
// benutzt werden (Raum-Ist < Soll) und nur, wenn der RIEcon50 Unterversorgung	(bezieht sich auf alle Räume) signalisiert	
//ulsch: 27.03.07: per Parametrierung soll die Kaskade trotz Adaption uneingeschränkt wirken ( Parameter KaskMode ) !
	switch ( hk )
	{
		case HK1 :
			#if ADAP_STAT_HK1 > 0
			mwAdap = mwAdaption[HK1][0].messw;
			#endif
			break;	
		case HK2 :
			#if ADAP_STAT_HK2 > 0
			mwAdap = mwAdaption[HK2][0].messw;
			#endif
			break;
		case HK3 :
			#if ADAP_STAT_HK3 > 0
			mwAdap = mwAdaption[HK3][0].messw;
			#endif
			break;	
		case HK4 :
			#if ADAP_STAT_HK4 > 0
			mwAdap = mwAdaption[HK4][0].messw;
			#endif
			break;												
	}


// die Parameter der Zeitabsenkung sind bisher ohne Bedeutung, wenn dem R66 der Nutzungszustand (Heizbedarf)
// von einem übergeordneten Gerät (R50, R100WRE) übermittelt wird
// neu: während dieser Zeit soll die Kaskade nicht wirken (Wunsch von M.Knispel, abgestimmt mit Dr.Riedel)
	if ( hks[hk].KaskMode == 2 )
	{
		if ( abs_ram[hk].absen == TRUE )
			return;
	}	

	if ( hks[hk].KaskMode == 0 )
	{
		if ( mwAdap == 50 || mwAdap == 80 || ( mwAdap == 20 && ( hkd[hk].tisobed < hkd[hk].tibed ) ) )
		{
			hkd[hk].kaskaKorr = 0;
			return;
		}
	}
				
	if(ada == 0)
	{	// Regelabweichung RAUM-SOLL - RAUM-IST
		// int Zeiger TI[] wird in Load_Userdef() ("UserReply.c") in Abhängigkeit vom
		// aktuellen Projekt geladen. Projektdefinitionen sind "UserConst.c" festgelegt
		
		// 090106: wirksame Raum-Soll-Temp. wird  durch Parameter KaskBegr begrenzt
		if ( hks[hk].Tiso <= hks[hk].KaskBegr )
			eik = hks[hk].Tiso - TI[hk]->messw;
		else if ( TI[hk]->messw < hks[hk].KaskBegr )	// Raumtemp. < KaskBegr : statt Tiso gilt KaskBegr 
			eik = hks[hk].KaskBegr - TI[hk]->messw;
		else if ( TI[hk]->messw > hks[hk].Tiso )			// Raumtemp. > Tiso
			eik = hks[hk].Tiso - TI[hk]->messw;
		else																					// Raumtemp. > KaskBegr, aber < Tiso
			eik = 0; 		
	}
	
	if(ada == 1)
	{ // Regelabweichung RAUM-SOLL-ADAP - RAUM-IST-ADAP
		if ( hkd[hk].tisobed <= hks[hk].KaskBegr )
			eik = hkd[hk].tisobed - hkd[hk].tibed;
		else if ( hkd[hk].tibed < hks[hk].KaskBegr )
			eik = hks[hk].KaskBegr - hkd[hk].tibed;
		else if ( hkd[hk].tibed > hkd[hk].tisobed )
			eik = hkd[hk].tisobed - hkd[hk].tibed;
		else
			eik = 0;
	}
	
	kas = (long)eik * hks[hk].Kpk / 100;				// 071012: Wirkung der Kaskade begrenzt auf +20,0/-10,0
	if ( kas > 200 )
		kas = 200;
	else if ( kas < -100 )
		kas = -100;
		
	hkd[hk].kaskaKorr = (int)kas;
	
	hkd[hk].tvsb += (int)kas;
		
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* Rücklauftemperatur-abhaengige TVSOLL-Verschiebung						 */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  IN: hk.....Heizkreis
//			ps.....Begrenzung auf Primär(1)- oder Sekundär(0)- Rücklauftemp.
//			(tvsb...berechnete  Vorlauftemperatur (Heizkennlinie + Kaskade)
// OUT:	tvsb...verschobene Vorlauftemperatur
void rueckl_begr(char hk, char ps)
{
	float fl_tvsb, fl_absenk, fl_eir, fl_tr;
	
	fl_tvsb = fl_absenk = fl_eir = fl_tr = 0;
	
	// welche Rücklauftemperatur ?
	if(ps == 1)														// Begrenzung auf Primärrücklauftemp. ?
		fl_tr = (float)TRP[ANL]->messw / 10;
	else																	// Begrenzung auf Sekundärrücklauftemp.
		fl_tr = (float)TRS[hk]->messw / 10;				
		
	// Abweichung RÜCKLAUF - RÜCKLAUF.MAX
	fl_eir = fl_tr - (float)hks[hk].Trma / 10;
	
	if(fl_eir >= 0)									// nur wenn MAX überschritten
	{	hkd[hk].rlbegr = 1;					// Statusanzeige Rücklaufbegrenzung setzen
		
		// Errechne Absenkwert.
		fl_absenk = fl_eir * (float)hks[hk].Kpr / 100;
		
		// Lade berechnete  Vorlauftemperatur nach Heizkurve + Kaskade
		fl_tvsb = (float)hkd[hk].tvsb / 10;

		// Verschiebung berechnen
		if(fl_tvsb > fl_absenk + 1)				// Sicherstellen, dass das Ergebnis positiv ist
			fl_tvsb = fl_tvsb - fl_absenk;
		else
			fl_tvsb = 1;										// mindestens 1°C
		
		// float to integer
		hkd[hk].tvsb = (int)(fl_tvsb * 10);
	}
}	


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 		Sonderphasen testen (Aufheizphase, Stützbetrieb, Frostschutzbetrieb)				*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//  IN: hk.....Heizkreis
// OUT: tvsb...Sonderphase aktiv  : return 1
char test_aufhe(char hk)
{
	char ret;
	
	ret = 0;
	
	if(hkd[hk].absenSchalter == 1)
	{
		if(hkd[hk].stuetzSchalter == 1)					// Stützbetrieb
		{	tvsoll_ber ( hk );										// nach Heizkennlinie
			ret = 1;													
		}
		else if(hkd[hk].frostSchalter == 1)			// Frostschutzbetrieb
		{	hkd[hk].tvsb =  hks[hk].Tvmi;					// Minimalwert laden
			ret = 1;													
		}
	}
	else
	{	if(hkd[hk].vorra_bed == TRUE)						// Vorrang Bedarfsabsenkung vor Zeitabsenkung
		{
			// Bedarfsabsenkung	(Phasen ermittelt in Bedarf.c)
			
			if(abs_bed[hk].aufhe == 1)						// Aufheizphase
			{	hkd[hk].tvsb =  hks[hk].Tvma;				// Maximalwert laden
				ret = 1;
			}
			else if(abs_bed[hk].stuetz == 1)			// Stützbetrieb
			{	tvsoll_ber ( hk );									// nach Heizkennlinie
//#####090116 auf Wunsch des Chefs
				hkd[hk].tvsb -= abs_bed[hk].tvab;		// Absenkwert benutzen
				ret = 1;													
			}
			else if(abs_bed[hk].frost == 1)				// Frostschutzbetrieb
			{	hkd[hk].tvsb =  hks[hk].Tvmi;				// Minimalwert laden
				ret = 1;													
			}
		}
		else		// Zeitabsenkung	(Phasen ermittelt in Absenk.c)
		{	
			if(hkd[hk].absenFerien == 1)					// Ferienbetrieb
			{
				if(hkd[hk].stuetzFerien == 1)				// Stützbetrieb
				{	tvsoll_ber ( hk );								// nach Heizkennlinie
//#####090116
					hkd[hk].tvsb -= hkd[hk].tvabFerien;		// Berechn. Absenkwert abziehen
					ret = 1;													
				}
				else if(hkd[hk].frostFerien == 1)		// Frostschutzbetrieb
				{	hkd[hk].tvsb =  hks[hk].Tvmi;			// Minimalwert laden
					ret = 1;													
				}
			}
			else	// Absenkbetrieb
			{
				if(abs_ram[hk].aufhe == 1)					// Aufheizphase
				{	hkd[hk].tvsb =  hks[hk].Tvma;			// Maximalwert laden
					ret = 1;
				}
				else if(abs_ram[hk].stuetz == 1)		// Stützbetrieb
				{	tvsoll_ber ( hk );								// nach Heizkennlinie
//#####090116
					hkd[hk].tvsb -= abs_ram[hk].tvab;			// Absenkwert benutzen
					ret = 1;													
				}
				else if(abs_ram[hk].frost == 1)			// Frostschutzbetrieb
				{	hkd[hk].tvsb =  hks[hk].Tvmi;			// Minimalwert laden
					ret = 1;													
				}
			}	
		}
	}
		
	return(ret);
}


char TestEstrichProg ( char hk )	
{
	HkDynam *pHkd;	
	HkStandard *pHks;
	char fbh;
	char tg;
	UINT start, jetzt;							// als Tag des Jahres
	datum heute;
	char ret = FALSE;

	// Strukturpointer zuweisen
	pHkd = &hkd[hk];	
	pHks = &hks[hk];

	if ( hk == HK1 )
		fbh	= FBH_HK1;
	else if ( hk == HK2 )
		fbh	= FBH_HK2;
	else if ( hk == HK3 )
		fbh	= FBH_HK3;
	else if ( hk == HK4 )
		fbh	= FBH_HK4;
	else
		fbh = 0;	

	if ( fbh > 0 )
	{
		tg = 0;
		while ( tg < ESTR_STUFEN && pHks->EstrichTemp[tg] != 0 )
			tg++;												// Anzahl Estrich-Tage 	
		if ( tg > 0 )
		{
			if (  pHks->StartAufheiz.jahr == Jahr && Mon == pHks->StartAufheiz.monat && Tag >= pHks->StartAufheiz.tag && Tag < pHks->StartAufheiz.tag + tg )
			{
				pHkd->estrichTag = Tag - pHks->StartAufheiz.tag + 1;
				ret = TRUE;			
			}
								
			else if ( pHks->StartAufheiz.jahr == Jahr && Mon == pHks->StartAufheiz.monat + 1 )
			{
				start = DayOfYear ( pHks->StartAufheiz );
				heute.tag = Tag;
				heute.monat = Mon;
				heute.jahr = Jahr;
				jetzt = DayOfYear ( heute );
				if ( jetzt < start + tg )	
				{
					pHkd->estrichTag = jetzt-start + 1;
					ret = TRUE;			
				}
			}
									
			else if ( pHks->StartAufheiz.jahr == Jahr - 1 && Mon == 1 && pHks->StartAufheiz.monat == 12 )
			{
				if ( ( 31 - pHks->StartAufheiz.tag + 1 + Tag ) <= tg )
				{
					pHkd->estrichTag = 31 - pHks->StartAufheiz.tag + Tag + 1;
					ret = TRUE;			
				}					
			}
		}
	}		

	if ( ret == TRUE )
		pHkd->estrichTemp = pHks->EstrichTemp[pHkd->estrichTag - 1];
	else
	{
		pHkd->estrichTemp = 0;
		pHkd->estrichTag = 0;
	}	
		
	return ( ret );
}


char RaumAbschaltung	( char hk )
{
	char hkAbschalten = 0;
	char raumbedarf = 0;
		
	if ( hk == HK1 )
		raumbedarf = BEDARF_HK1;
	else if  ( hk == HK2 )
		raumbedarf = BEDARF_HK2;
	else if  ( hk == HK3 )
		raumbedarf = BEDARF_HK3;
	else if  ( hk == HK4 )
		raumbedarf = BEDARF_HK4;
	
	if ( raumbedarf == TRUE )
	{
		// keine Abschaltung, wenn Funktion nicht freigegeben, Außentemp.gestört oder <= Frostgrenze,
		// externe Raum-Soll- oder -Ist-Temp.gestört (<= 1°C) oder Ist-Temp. < Soll
		if ( hks[hk].AbschRaumanf != TRUE || ta1m.stat != 0 || hkd[hk].tibed <= 10 || hkd[hk].tisobed <= 10  )
		{
			hkd[hk].raumAbsch = 0;							// nicht auswertbar
			hkd[hk].ventOffenBer = 0;
		}
		else if ( ta1m.messw <= hks[hk].TaFrostgrenze	)
		{
			hkd[hk].raumAbsch = 2;							// nicht abschalten
			hkd[hk].ventOffenBer = 0;
		}
		else
		{
			if ( hks[hk].VentilAuswert == TRUE )
			{
				// Plausibilität der Ventilstellungen prüfen
				// ventGesamt: Anzahl der Räume in Nutzung/Vorheizen. ventOffen: Anzahl der offenen Ventile
				// gestörte Räume unberücksichtigt
				if ( ( hkd[hk].ventGesamt == 0 ) || ( hkd[hk].ventOffen > hkd[hk].ventGesamt ) )
				{
					hkd[hk].raumAbsch = 0;					// nicht auswertbar
					hkd[hk].ventOffenBer = 0;
				}
				else 
				{
					hkd[hk].ventOffenBer = 	100 * hkd[hk].ventOffen  /  hkd[hk].ventGesamt;
					if ( hkd[hk].tibed < hkd[hk].tisobed )
						hkd[hk].raumAbsch = 2;				// nicht abschalten					
					else if ( hkd[hk].ventOffenBer <= hks[hk].VentOffenMax )
						hkd[hk].raumAbsch = 1;				// abschalten
					else
						hkd[hk].raumAbsch = 2;				// nicht abschalten
				}
			}
			else
			// Ventile nicht auswerten, Raum-Ist >= Soll
			{
				hkd[hk].ventOffenBer = 0;
				if ( hkd[hk].tibed < hkd[hk].tisobed )
					hkd[hk].raumAbsch = 2;				// nicht abschalten	
				else
					hkd[hk].raumAbsch = 1;				// abschalten
			}
		}
	}
	if ( hkd[hk].raumAbsch == 1 )
		hkAbschalten = 1;
	return ( hkAbschalten );
}

