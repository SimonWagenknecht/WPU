/*	Task:					SteuerKe		*/
/*	Kesselsteuerung						*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"

void SollwertAnhebung ( mwsp* );
void Abgasmessung ( char ke );
void KesselLaufzeit ( char ke );
void KesselPumpe ( char ke );
void KesselKlappe ( char ke );
void KesselBM ( char ke );
void PumpeBM ( char ke );
void Pumpenbus_KE ( char ke );
void BrennerModulation ( char ke );
char Stufe2 ( char ke, mwsp *tv );
void SollwertAusgabe ( char ke );
void RlBegrenzung ( char ke );
void PumpenDrehzahl ( char ke );

char KesselFolge ( void );
char FolgePlausibel ( ULONG kesselfolge );
void FolgeAendernLaufzeit ( void );
void KesStoer ( void );
void KesGesperrt ( void );
void ModulationMW ( void );
void FolgekesselSchalten ( mwsp*, mwsp*, mwsp* );
void PuEin ( char ke );		
void PuAus ( char ke );	
void KlAuf ( char ke );		
void KlZu ( char ke );	
void KesselStart ( char ke );
void KesselFreigaben ( void );
char KesselSoll ( char ke );
char KesselMod ( char ke );
char KesselStuf ( char ke );
char KesselPuDz ( char ke );
void SummeRegelabweichung ( mwsp *VL_STRATEG );
char SM_TempAbweichung ( mwsp *VL_STRATEG ); 

	
/*---------------------- Task SteuerKe -----------------------------------*/
// Takt: 1 s

#if KEANZ == 1

void SteuerKe ( void )
{	
	static char sec = 0;
	int tvsb;
	mwsp *TV_STRAT;			// Strategiefühler, i.a. Weiche/Verteiler/gesamt
	mwsp *TV_ST2;
	char soll, mod, stuf;

	if ( ++sec >= 10 )
		sec = 0;

	TV_STRAT = ( TVGKE[KE1]->stat == 0 ) ? TVGKE[KE1] : TVKE[KE1];
	TV_ST2 = TVKE[KE1];

	#if SW_ANH_VAR
	if ( TV_STRAT->stat == 0 )
		SollwertAnhebung ( TV_STRAT );
	else
		ked[KE1].y_rel_anh = 0;	
	#else
	ked[KE1].y_rel_anh = 0;
	#endif
	
	ked[0].tvsb_anl = maxAnford;		// ked[0].tvsb_anl wird in aufgerufenen Funktionen benötigt
	if ( ked[0].tvsb_anl == 0 )
		tvsb = 0;
	else
	{
		tvsb = ( kes[KE1].Tvpa ) ? ked[0].tvsb_anl + kes[KE1].Tvpa : ked[0].tvsb_anl + ked[KE1].y_rel_anh;
		if ( tvsb > kes[KE1].Tvma )
			tvsb = kes[KE1].Tvma;		
		else if ( tvsb < kes[KE1].Tvmi )
			tvsb = kes[KE1].Tvmi;
	}

	#if SSF	
	Abgasmessung ( KE1 );
	ssfEinAnl = ked[KE1].ssfEin;		
	
	if ( ked[KE1].ssfEin == TRUE )
		tvsb = kes[KE1].Tvma;	
	#endif

	ked[KE1].tvsb = tvsb;	

	#if SM_TABW
	ked[0].abwAlarm = SM_TempAbweichung ( TV_STRAT );
	#endif
	
	if ( kes[KE1].Haut == TRUE )
	{	
		RVENTKE[KE1]->awert	= kes[KE1].Kestell;
		ked[KE1].y_rel = kes[KE1].Kestell;
		ked[KE1].fl_y_rel = (float)kes[KE1].Kestell / 10;		// PID-Wert durch Handwert überschreiben 
		KE[KE1]->wert = kes[KE1].Kesea;	
		ked[KE1].kesEin = kes[KE1].Kesea;
		KESTU2[KE1]->wert = kes[KE1].Kestufe2;	
		PUKEEA[KE1]->wert = kes[KE1].Puea;
		KLKEAZ[KE1]->wert = kes[KE1].Kesklaz;
				
		if ( ked[KE1].vzu )	
		{	 
			--ked[KE1].vzu;	
			ked[KE1].vauf = 0;					// Vorrang vor Auffahren 
		}                               					
		else if ( ked[KE1].vauf )			// Ausgabe für Zufahren nicht aktiv
			--ked[KE1].vauf;
		MKEZU[KE1]->wert = ked[KE1].vzu;
		MKEAUF[KE1]->wert = ked[KE1].vauf;
		
		RVENTKR[KE1]->awert	= kes[KE1].Kestell_rl;
		ked[KE1].y_rel_rl = kes[KE1].Kestell_rl;
		ked[KE1].fl_y_rel_rl = (float)kes[KE1].Kestell / 10;		// PID-Wert durch Handwert überschreiben 
		if ( ked[KE1].rvzu )	
		{	 
			--ked[KE1].rvzu;	
			ked[KE1].rvauf = 0;					// Vorrang vor Auffahren 
		}                               					
		else if ( ked[KE1].rvauf )			// Ausgabe für Zufahren nicht aktiv
			--ked[KE1].rvauf;
		RKEZU[KE1]->wert = ked[KE1].rvzu;
		RKEAUF[KE1]->wert = ked[KE1].rvauf;

		DZPUKE[KE1]->awert	= kes[KE1].DzPuHand;
		ked[KE1].dzPu = kes[KE1].DzPuHand;

	}

	else
	{	
		kes[KE1].Haut = FALSE;
		ked[KE1].vauf = 0;
		ked[KE1].vzu = 0;

		soll = SOLLWERT_KE1;
		mod = MODUL_KE1;
		stuf = STUFIG_KE1;

		ked[KE1].sollwert = 0;
		if ( stuf || ( mod  && soll == 0 ) || ( mod && soll && kes[KE1].Modulation ) )
		{
			if ( TV_STRAT->stat == 0 )
			{
				if ( ( TV_STRAT->messw >= ked[KE1].tvsb + kes[KE1].KesSchaltDiff ) || ( TVKE[KE1]->messw >= kes[KE1].TvMax ) )
					ked[KE1].kesEin = FALSE;
				else if ( TV_STRAT->messw <= ked[KE1].tvsb )
					ked[KE1].kesEin = TRUE;
					
			}
			else
				ked[KE1].kesEin = FALSE;
		}
		else if ( ( mod == 0 && soll ) || ( mod && soll && kes[KE1].Modulation == 0 ) )
		{
			SollwertAusgabe ( KE1 );
			ked[KE1].kesEin = ked[KE1].sollwert ? 1 : 0;
		}
		else
			ked[KE1].kesEin = FALSE;

		#if ( MODUL_KE1 )
		if ( soll == 0 || kes[KE1].Modulation == 1 )		
			BrennerModulation ( KE1 );
		#elif ( DZ_PU_KE1 )
		BrennerModulation ( KE1 );
		#endif		

		// Ausgabe Modulation oder Sollwert
		if ( ked[KE1].kesEin )
		{
			#if ( SOLLWERT_KE1 && MODUL_KE1 )
			RVENTKE[KE1]->awert = ( kes[KE1].Modulation == 0 ) ? ( ked[KE1].sollwert * 10 ) : Gerade_YvonX ( ked[KE1].y_rel, 0, kes[KE1].ModMin*10, 1000, 1000 );		
			#elif SOLLWERT_KE1
			RVENTKE[KE1]->awert = ked[KE1].sollwert * 10; 
			#elif ( MODUL_KE1 == 2 )	
			RVENTKE[KE1]->awert = Gerade_YvonX ( ked[KE1].y_rel, 0, kes[KE1].ModMin*10, 1000, 1000 );	
			#else
			RVENTKE[KE1]->awert = 0;	
			#endif
		}
		else
			RVENTKE[KE1]->awert = 0;			

		#if ( MODUL_KE1 || STUFIG_KE1 )
		KE[KE1]->wert = ked[KE1].kesEin;
		#endif			

		#if STUFIG_KE1 == 2
		if ( 	sec == 0 )
		{																		// alle 10 Sekunden
			ked[KE1].stufe2 = Stufe2 ( KE1, TV_ST2 );
			KESTU2[KE1]->wert = ked[KE1].stufe2;
		}
		#endif
		
		#if BM_KE1
		KesselBM ( KE1 );
		#endif
		 
		#if RL_KE1
		RlBegrenzung ( KE1 );
		#endif
		
		// Nachlauf Pumpe/Klappe
		if ( ked[KE1].kesEin )
		{
			#if ( PU_KE1 || PU_BUS_KE1 )
			ked[KE1].puna = kes[KE1].Puna * 60;
			#else
			ked[KE1].puna = 0;
			#endif
			#if KL_KE1
			ked[KE1].klna = kes[KE1].Klna * 60;
			#else
			ked[KE1].klna = 0;
			#endif			
		}
		else
		{			
			if ( ked[KE1].puna )
				--ked[KE1].puna;
			if ( ked[KE1].klna )			
				--ked[KE1].klna;
		}

		#if DZ_PU_KE1
		PumpenDrehzahl ( KE1 );
		#endif
		
		#if PU_KE1
		KesselPumpe ( KE1 );
		#endif
		
		#if KL_KE1
		KesselKlappe ( KE1 );
		#endif

		KesselLaufzeit ( KE1 );
		
		#if BM_PUKE1
		PumpeBM ( KE1 );
		#endif
	
		#if ( BUS_PU_MAX > 0 )
		Pumpenbus_KE ( KE1 );											// nur im Automatik-Betrieb !!!
		#endif

	}
	
}

#endif	// if KEANZ == 1



#if KEANZ
/**********************************************************/
/***** Kessellaufzeiten ***********************************/

void KesselLaufzeit ( char ke )
{
	if ( ked[ke].kesEin )
	{	
		if ( ++ked[ke].kesSec >= 3600 )
		{	
			ked[ke].kesSec = 0;
			ked[ke].kesh++;							// Laufzeit h erhöhen
			bicbus ( EEPADR, (char*)&ked[ke].kesh, KESH_ADR+(ke*2), 2, BICWR );
		}
	}
	
#if STUFIG_KE1 == 2	// nur wenn 2-stufig
	if ( KESTU2[ke]->wert )				// Abfrage Relais Stufe 2: Kessel ist ein
	{	
		if ( ++ked[ke].kes2Sec >= 3600 )
		{	
			ked[ke].kes2Sec = 0;
			ked[ke].kes2h++;						// Laufzeit h erhöhen
			bicbus(EEPADR, (char*)&ked[ke].kes2h, KES2H_ADR+(ke*2), 2, BICWR);
		}
	}
#endif			
}	


/********************************************************/
/***** Integration der Istwert-Sollwert-Differenz *******/
/***** steuert das Ein- und Ausschalten der 2.Stufe	*****/

char Stufe2 ( char ke, mwsp *tv )
{
	int abw;
	char st2 = ked[ke].stufe2;

	abw = tv->messw - ked[ke].tvsb;		// aktuelle Istwert-Sollwert-Differenz, Kesselfuehler
	ked[ke].abw2 = abw;
	
	if ( ked[ke].kesEin == 0 || tv->stat )
	{
		ked[ke].abwAlt2 = 0;
		ked[ke].abwsumme2 = 0;
		st2 = 0;
	}
		
	else
	{	
		if ( abw >= 0)															// Abweichung positiv ? ( Messwert > Sollwert )
		{																						// Ausschalten ?
			if ( ked[ke].abwAlt2 >= 0 )
			{																					// alte Abweichung auch positiv ? nur integr., wenn Kriterium nicht erreicht
				if ( ked[ke].abwsumme2 - kes[ke].Auskrit2 <= 0 )
				{
					ked[ke].abwsumme2 += abw / 6;
				}
			}			
			else																			// bei Vorzeichenwechsel
				ked[ke].abwsumme2 = +1;									// Summe zurücksetzen
		}
		else																				// Abweichung ist negativ
		{	
			if ( ked[ke].abwAlt2 < 0 )	
			{	
				if ( ked[ke].abwsumme2 + kes[ke].Einkrit2 >= 0	)																		
				{																				// alte Abweichung auch negativ ? nur integr., wenn Kriterium nicht erreicht
					ked[ke].abwsumme2 += abw / 6;					// Vollwert addieren
				}
			}
			else																			// bei Vorzeichenwechsel
				ked[ke].abwsumme2 = -1;									// Summe zurücksetzen
		}
	}
	ked[ke].abwAlt2 = abw;											// beim nächsten Mal Altwert
	
	if ( st2 == 1 && ( ked[ke].abwsumme2 - kes[ke].Auskrit2 > 0 ) )
		st2 = 0;							// Stufe 2 ausschalten
	else if ( st2 == 0 && ( ked[ke].abwsumme2 + kes[ke].Einkrit2 < 0 )  )
		st2 = 1;							// Stufe 2 einschalten ( Integral ist negativ  )

	return ( st2 );	
}


/****************************/
/***** Sollwert-Ausgabe *****/

void SollwertAusgabe ( char ke )
{
	if ( ked[ke].tvsb < kes[ke].TvsolX1 )
		ked[ke].sollwert = 0;
	else if ( ked[ke].tvsb > kes[ke].TvsolX2 )
		ked[ke].sollwert = kes[ke].SollY2;
	else
		ked[ke].sollwert = Gerade_YvonX ( ked[ke].tvsb, kes[ke].TvsolX1, kes[ke].SollY1, kes[ke].TvsolX2, kes[ke].SollY2 ); 	

//	ked[ke].kesEin = ked[ke].sollwert ? 1 : 0;

	ked[ke].fl_ei2 = (ked[0].tvsb_anl - TVKE[ke]->messw) / 10;
	ked[ke].fl_ei1 = ked[ke].fl_ei2;		
	ked[ke].ei = 0;
	ked[ke].si_y_rel = 0;
	ked[ke].y_rel = 0;
	ked[ke].fl_y_rel = 0;
	ked[ke].stellsum = 0;
	MKEZU[ke]->wert = 0;
	MKEAUF[ke]->wert = 0;

}


/***********************************************/
/***** Regelalgorithmus Brenner-Modulation *****/

void BrennerModulation ( char ke )
{
	float fl_ei = 0;
	float fl_dy_rel = 0;

	if ( ked[ke].kesEin == FALSE || TVKE[ke]->stat != 0 ) 
		ked[ke].modVerz = sowi ? ( kes[0].ModVerzSomm * 60 ) : ( kes[0].ModVerzWint * 60 );
	else
	 if ( ked[ke].modVerz )
	 	--ked[ke].modVerz;
	 			
	// Regler-Tastzeit
	if ( ( ked[ke].zts += 10 ) < kes[ke].Ts )
		 return;														// Tastzeit ( in 0,1 sec ) noch nicht erreicht
	ked[ke].zts = 0;
		
	if ( ked[ke].kesEin == FALSE || ked[ke].modVerz || TVKE[ke]->stat != 0 ) 
	{
//-		ked[ke].fl_ei2 = 0;
//-		ked[ke].fl_ei1 = 0;	
		ked[ke].fl_ei2 = (ked[0].tvsb_anl - TVKE[ke]->messw) / 10;
		ked[ke].fl_ei1 = ked[ke].fl_ei2;		
		ked[ke].ei = 0;
		ked[ke].si_y_rel = 0;
		ked[ke].y_rel = 0;
		ked[ke].fl_y_rel = 0;
//		RVENTKE[ke]->awert = 0;
		ked[ke].stellsum = 0;
		MKEZU[ke]->wert = ( kes[ke].Ts / 10 ) + 1;
		MKEAUF[ke]->wert = 0;
		return;		
	}
			
	else	
	{	
		// Regelabweichung Soll - Ist
		fl_ei	= ( (float)ked[ke].tvsb - (float)(TVKE[ke]->messw ) ) / 10;
		
		// -------------- PID-Regelalgorithmus ---------------------------------
		// Berechnung der relativen Stellgrößen
		fl_dy_rel = Dy_rel ( kes[ke].Kp, kes[ke].Kd, kes[ke].Ts, kes[ke].Tn, 
														fl_ei, ked[ke].fl_ei1, ked[ke].fl_ei2 );
		
		ked[ke].fl_y_rel += fl_dy_rel;
		
		//---------------------------------------------------------------------
		// Regelabweichungen merken
		ked[ke].fl_ei2 = ked[ke].fl_ei1;
		ked[ke].fl_ei1 = fl_ei;						
		ked[ke].ei = (int)(fl_ei * 10);			// [0,1%] zur Anzeige		

		// Berechnung und Ausgabe der absoluten Stellgrößen
		// --------------------------------------------------------------------
		// Ausgabe an 3-Punkt Ventil
		if ( kes[ke].Stellzeit > 0 )
		{	
			ked[ke].stellsum += (int)( fl_dy_rel * kes[ke].Stellzeit ); // [0.01 s]				
			if ( abs(ked[ke].stellsum) >= 100 )									// ab 1 Sekunde ausgeben
			{
				ked[ke].fahren		= ked[ke].stellsum / 100;				// nur ganze Sekunden fahren
				ked[ke].stellsum	= ked[ke].stellsum % 100; 			// Rest merken
				if ( ked[ke].fahren > 0 )		
					MKEAUF[ke]->wert = (char)ked[ke].fahren;
				else
					MKEZU[ke]->wert  = (char)( abs( ked[ke].fahren ) );	
			}
		}	

		// Ausgabe an Analogventil	
		// Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
		ked[ke].fl_y_rel = anti_windup ( ked[ke].fl_y_rel, kes[ke].Kp, kes[ke].Wup, fl_ei );		// in Funktion.c
		ked[ke].si_y_rel = (int)(ked[ke].fl_y_rel * 10);		// debug
		
		// Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
		ked[ke].y_rel = y_stell ( ked[ke].fl_y_rel );				// in Funktion.c
		
//		RVENTKE[ke]->awert = Gerade_YvonX ( ked[ke].y_rel, 0, kes[ke].ModMin*10, 1000, 1000 );
	}	
}


/***************************/
/***** Pumpen-Drehzahl *****/
// Bedingung: P1ModDz <= P2ModDz !!!

void PumpenDrehzahl ( char ke )
{
	if ( ked[ke].kesEin || ked[ke].puna )
	{
		if ( kes[ke].P1ModDz > kes[ke].P2ModDz )
			ked[ke].dzPu = 500;
		else
		{		
			if ( ked[ke].y_rel <= kes[ke].P1ModDz )	
				ked[ke].dzPu = kes[ke].P1Dz;
			else if ( ked[ke].y_rel >= kes[ke].P2ModDz )
				ked[ke].dzPu = kes[ke].P2Dz;
			else
				ked[ke].dzPu = Gerade_YvonX ( ked[ke].y_rel, kes[ke].P1ModDz, kes[ke].P1Dz, kes[ke].P2ModDz, kes[ke].P2Dz );
		}			
	}
	else
		ked[ke].dzPu = 0;	
	DZPUKE[ke]->awert	= ked[ke].dzPu;	
}


/*******************************/
/***** Rücklauf-Begrenzung *****/

void RlBegrenzung ( char ke )
{
	float fl_ei = 0;
	float fl_dy_rel = 0;

	if ( ke == KE1 )
	{
		if ( RL_KE1 == 0 ) 
			return;
	}
	else if ( ke == KE2 )
	{
		if ( RL_KE2 == 0 ) 
			return;
	}
		
	// Regler-Tastzeit
	if ( ( ked[ke].zts_rl += 10 ) < kes[ke].Ts_rl )
		 return;														// Tastzeit ( in 0,1 sec ) noch nicht erreicht
	ked[ke].zts_rl = 0;
	
		
//	if ( ( ked[ke].kesEin == FALSE && ked[ke].puna == 0 && ked[ke].klna == 0 ) || ( kes[ke].BegrenzVlRl == 0 && TVKE[ke]->stat != 0 ) || ( kes[ke].BegrenzVlRl && TRKE[ke]->stat != 0 ) )
	if ( ( ked[ke].kesEin == FALSE && ked[ke].puna == 0 && ked[ke].klna == 0 ) || TRKE[ke]->stat != 0 )
	{
		ked[ke].soll_rl = 0;
		ked[ke].fl_ei2_rl = 0;
		ked[ke].fl_ei1_rl = 0;	
		ked[ke].ei_rl = 0;
		ked[ke].y_rel_rl = 0;
		ked[ke].fl_y_rel_rl = 0;
		ked[ke].stellsum_rl = 0;
		if ( kes[ke].RlAnheb == 0 )
		{
			RKEAUF[ke]->wert = ( kes[ke].Ts_rl / 10 ) + 1;
			RKEZU[ke]->wert = 0;
			RVENTKR[ke]->awert = 1000;
		}
		else
		{
			RKEZU[ke]->wert = ( kes[ke].Ts_rl / 10 ) + 1;
			RKEAUF[ke]->wert = 0;			
			RVENTKR[ke]->awert = 0;
		}
		return;		
	}

	else
 	{	
		ked[ke].soll_rl = kes[ke].Ttaup;
		// Regelabweichung Soll - Ist
//-		if ( kes[ke].BegrenzVlRl == 0 )
//-			fl_ei	= ( (float)kes[ke].Ttaup - (float)TVKE[ke]->messw ) / 10;
//-		else
//-			fl_ei	= ( (float)kes[ke].Ttaup - (float)TRKE[ke]->messw ) / 10;
		fl_ei	= ( (float)kes[ke].Ttaup - (float)TRKE[ke]->messw ) / 10;
		
		// -------------- PID-Regelalgorithmus ---------------------------------
		// Berechnung der relativen Stellgrößen
		fl_dy_rel = Dy_rel ( kes[ke].Kp_rl, kes[ke].Kd_rl, kes[ke].Ts_rl, kes[ke].Tn_rl, 
														fl_ei, ked[ke].fl_ei1_rl, ked[ke].fl_ei2_rl );
		if ( kes[ke].RlAnheb == 1 )
			fl_dy_rel *= -1;	// Anhebung

		ked[ke].fl_y_rel_rl += fl_dy_rel;
				
		//---------------------------------------------------------------------
		// Regelabweichungen merken
		ked[ke].fl_ei2_rl = ked[ke].fl_ei1_rl;
		ked[ke].fl_ei1_rl = fl_ei;						
		ked[ke].ei_rl = (int)( fl_ei * 10 );									// [0,1%] zur Anzeige	
		
		// Berechnung und Ausgabe der absoluten Stellgrößen
		// --------------------------------------------------------------------
		// Ausgabe an 3-Punkt Ventil
		if ( kes[ke].Stellzeit_rl > 0 )
		{	
			ked[ke].stellsum_rl += (int)( fl_dy_rel * kes[ke].Stellzeit_rl ); // [0.01 s]				
			if ( abs(ked[ke].stellsum_rl ) >= 100 )									// ab 1 Sekunde ausgeben
			{
				ked[ke].fahren_rl	= ked[ke].stellsum_rl / 100;				// nur ganze Sekunden fahren
				ked[ke].stellsum_rl	= ked[ke].stellsum_rl % 100; 			// Rest merken
				if ( ked[ke].fahren_rl > 0 )		
					RKEAUF[ke]->wert = (char)ked[ke].fahren_rl;
				else
					RKEZU[ke]->wert  = (char)( abs( ked[ke].fahren_rl ) );	
			}
		}	
				
		// Ausgabe an Analogventil	
		// Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
		ked[ke].fl_y_rel_rl = anti_windup ( ked[ke].fl_y_rel_rl, kes[ke].Kp_rl, kes[ke].Wup_rl, fl_ei );		// in Funktion.c
		ked[ke].si_y_rel_rl = (int)(ked[ke].fl_y_rel_rl * 10);		// debug
		
		// Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
		if ( ked[ke].kesEin )																	// Kessel EIN ?
			ked[ke].y_rel_rl = y_stell ( ked[ke].fl_y_rel_rl );				// in Funktion.c
		else																									// Kessel ist AUS
			ked[ke].y_rel_rl = ( kes[ke].RlAnheb == 0 ) ? (int)( 1000 ) : 0;					// Begrenzung/Anhebung -> Ventil AUF/ZU
	
		RVENTKR[ke]->awert = ked[ke].y_rel_rl;								// Ausgabe an Stellventil
	}
}


/************************************************************************************/
/***** Funktion : Anhebung der Kessel-Solltemperatur				 										*****/
/***** der Kessel-Sollwert ist nach Siefke um einen variablen Betrag anzuheben, *****/
/***** der von der Sollwert-Istwert-Differenz am Anlagenfühler abhängt  				*****/
/***** und nach dem Standard-Regelalgorithmus zu ermitteln ist			 						*****/

void SollwertAnhebung ( mwsp* pT_ist )
{
	float fl_ei = 0;
	
	if ( ( ked[0].zts_anh += 10 ) >= kes[0].Ts_anh )
	{																
		ked[0].zts_anh = 0;												// Tastzeit in s * 10 
		
	// Berechnung der Kesselanhebung
	// Sollwert entspr. Anforderung 
	// Istwert : Vorlauf Verteiler
	
	
		// Regelabweichung Soll - Ist
		fl_ei	= ( (float)ked[0].tvsb_anl - (float)pT_ist->messw ) / 10;

//-		if ( fl_ei > 20 )
//-			fl_ei = 20;														// Begrenzung
			
	// -------------- PID-Regelalgorithmus ---------------------------------
	// Berechnung der relativen Stellgrößen
		
		if ( kes[0].Kp_anh > 0 )
			ked[0].fl_y_rel_anh += Dy_rel ( kes[0].Kp_anh, kes[0].Kd_anh, kes[0].Ts_anh, kes[0].Tn_anh, 
												fl_ei, ked[0].fl_ei1_anh, ked[0].fl_ei2_anh );
		else
			ked[0].fl_y_rel_anh	= 0;
						
	// Regelabweichungen merken
		if ( ked[0].fl_y_rel_anh > 0 )
		{
			if ( ked[0].fl_y_rel_anh > 20 )
				ked[0].fl_y_rel_anh = 20;						// max. Anhebung 20 K				
			ked[0].fl_ei2_anh = ked[0].fl_ei1_anh;
			ked[0].fl_ei1_anh = fl_ei;
		}

		else
		{																				// keine Absenkung
			ked[0].fl_y_rel_anh = 0;
			ked[0].fl_ei2_anh = 0;
			ked[0].fl_ei1_anh = 0;				
		}
		ked[0].y_rel_anh = (int)( ked[0].fl_y_rel_anh * 10 );
		if ( ked[0].y_rel_anh > kes[0].AnhebMax )
			ked[0].y_rel_anh = kes[0].AnhebMax;
	}
}


/**************************************/
/***** Schornsteinfeger-Funktion ******/
void Abgasmessung ( char ke )
{
	if ( SSFKE[ke]->bstat != NICHTV )
	{
		if ( SSFKE[ke]->bwert )										// Schornsteinfeger-Taste gedrückt ?
		{
			if ( ked[ke].ctrSSF == 0 )
				ked[ke].ctrSSF = kes[ke].SSFdauer * 60;
			else if ( ked[ke].ctrSSF <= ( ( kes[ke].SSFdauer - 1 ) * 60 ) )
				ked[ke].ctrSSF = 60;									// vorzeitiges löschen, noch 1 Minuten
		}
		else if ( ked[ke].ctrSSF )
			--ked[ke].ctrSSF;		
	}
	else 
		ked[ke].ctrSSF = 0;
			
	ked[ke].ssfEin = ked[ke].ctrSSF ?	1 : 0;		// Funktion ist aktiv, solange der Zähler läuft 
}



/**********************************************/
/***** Kesselbetriebsmeldung-Überwachung ******/

void KesselBM ( char ke )
{
	if ( BMKES[ke]->bstat != NICHTV && kes[ke].KeBm_verz > 0 )			// Nur wenn Betriebsmelde-Kontakt vorhanden
	{	
		if ( BMKES[ke]->bwert || ked[ke].kesEin == 0 )
		{
			ked[ke].keBm_sAl = 0;
			ked[ke].keBm_cnt = 0;
		}		
		else if ( ked[ke].keBm_sAl == 0 )	
		{	
			++ked[ke].keBm_cnt;
			if ( ked[ke].keBm_cnt >= kes[ke].KeBm_verz * 60 )		
			{	
				ked[ke].keBm_sAl = 1;
				ked[ke].keBm_cnt = 0;	
			}	
		}
	}
	else
	{
		ked[ke].keBm_sAl = 0;
		ked[ke].keBm_cnt = 0;
	}
}		


/**********************************************/
/***** Kesselpumpe schalten *******************/

void KesselPumpe ( char ke )
{
	#if PU_KE1
	if ( ked[ke].kesEin )									// Kessel EIN ?
		PUKEEA[ke]->wert = 1;								// Kesselpumpe EIN		
	else if ( ked[ke].puna == 0 )
		PUKEEA[ke]->wert = 0;							// Kesselpumpe AUS		
	#endif
}
			

/**********************************************/
/***** Pumpenbetriebsmeldung-Überwachung ******/

void PumpeBM ( char ke )
{
	if ( BMPUKE[ke]->bstat != NICHTV && kes[ke].PuBm_verz > 0 )			// Nur wenn Betriebsmelde-Kontakt vorhanden
	{	
		if ( PUKEEA[ke]->wert )							// Pumpe EIN ?
		{	
			if ( BMPUKE[ke]->bwert )					// Betriebsmeldung EIN ?		
			{
				ked[ke].puBm_sAl = 0;						// Softalarm löschen			
				ked[ke].puBm_cnt = 0;	
			}
			else
			{	
				if ( ked[ke].puBm_cnt < 0xFFFF )	
				{	
					++ked[ke].puBm_cnt;
					if ( ked[ke].puBm_cnt >= kes[ke].PuBm_verz * 60 )			
						ked[ke].puBm_sAl = 1;				// Softalarm setzen
				}	
			}
		}
		else
			ked[ke].puBm_cnt = 0;
	}
	else
		ked[ke].puBm_sAl = 0;
}		


/**********************************************/
/***** Kesselklappe schalten ******************/

void KesselKlappe ( char ke )
{
	if ( ked[ke].kesEin )									// Kessel EIN ?
	{
		KLKEAZ[ke]->wert = 1;								// Kesselklappe AUF		
//-		KLKEZU[ke]->wert = 0;
//-		KLKEAUF[ke]->wert = 1;
	}
	else if ( ked[ke].klna == 0 )
	{
		KLKEAZ[ke]->wert = 0;								// Kesselklappe ZU	
//-		KLKEZU[ke]->wert = 1;
//-		KLKEAUF[ke]->wert = 0;
	}	
}

/***************************************************/
/***** Genibus / Wilo-modbus / Grundfos-modbus******/

void Pumpenbus_KE ( char ke )
{
#if BUS_PU_MAX > 0
	char pu = 0;
	char dz = 0;
	
	if ( ke == KE1 )
	{
		pu = PU_BUS_KE1;
		dz = DZ_PU_KE1;
	}
	else if ( ke == KE2 )
	{
		pu = PU_BUS_KE2;
		dz = DZ_PU_KE2;
	}		

	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
		
		if ( BusPuPara[pu].Funktion == 1 )
		{
			if ( ked[ke].kesEin || ked[ke].pu  )
			{
				BusPuPara[pu].Betrieb = 1;
				if ( dz )
					BusPuPara[pu].Sollwert = ked[ke].puDz;
			}
			else	if ( ked[ke].puna == 0 )
			{
				BusPuPara[pu].Betrieb = 0;
				if ( dz )
					BusPuPara[pu].Sollwert = 0;
			}	
		}

	#if GENI == 1	
		ked[ke].busPuEin = BusPuData[pu].uc_betriebsart;	
	#endif

	#if WILO_MODBUS == 1	
		ked[ke].busPuEin = modWiloPu[pu].output_value1_temp;	
	#endif

	#if GRUNDFOS_MODBUS == 1

	#endif
	}
#endif
}



/*****************************************************/
char SM_TempAbweichung ( mwsp *VL )
{
	char sm = ked[0].abwAlarm;
	int ei;
	
	ei =  ( VL->stat == 0 && ked[0].tvsb_anl > 0 ) ? ( ked[0].tvsb_anl - VL->messw ) : 0;
		
	
	if ( kes[0].TabwMax && kes[0].TabwZeit && ( abs( ei ) > kes[0].TabwMax ) )
	{
		if ( sm == 0 && ( ++ked[0].tabwCnt > ( kes[0].TabwZeit*60 ) ) )			// Counter in Sekunden, Parameter in Minuten
			sm = 1;
	}
	else	
	{
		sm = 0;
		ked[0].tabwCnt = 0;		
	}
	if ( sm == 1 )
		ked[0].tabwCnt= 0;
		
	return sm;
} 

#endif //#if KEANZ
				


#if KEANZ > 1
/*---------------------- Task SteuerKe -----------------------------------*/
// Takt: 1 s

void SteuerKe ( void )
{	
	static char kessel = KEMAX;
	char i;
	char n;
	char ke;
	int tvsb = 0;
	int tdiff;
	char soll, mod, stuf, puDz;
	static char sec;

	if ( ++kessel >= KEMAX )									// Kesselkreis-Index erhöhen
		kessel = 0;

	if ( kessel >= KEANZ )
		return;

	if ( kessel == 0 )
	{
		if ( ++sec >= 10 )
			sec = 0;

		VL_STRATEG = TVGKE[1];
		if ( VL_STRATEG->stat && TVGKE[0]->stat == 0 )
			VL_STRATEG = TVGKE[0];	
		VL_FOLGE_AUS = TVGKE[0]; 
		RL_FOLGE_AUS = TRGKE[0]; 
		VL_ANHEB = TVGKE[1];
		if ( VL_ANHEB->stat && TVGKE[0]->stat == 0 )
			VL_ANHEB = TVGKE[0];	

		#if SW_ANH_VAR
		if ( VL_ANHEB->stat == 0 )
			SollwertAnhebung ( VL_ANHEB );			// Kessel-Sollwert anheben, wenn die Solltemp. am Anlagenvorlauf nicht erreicht wird
		else
			ked[0].y_rel_anh = 0;		
		#else
		ked[0].y_rel_anh = 0;
		#endif
		
		KesStoer ( );
		KesGesperrt ( );
		
		if ( ked[0].pauseCtr < 0xFFFF )
			++ked[0].pauseCtr;
																								
																								//-// feste Kesselfolge für Sommer / Winter ?
																								//-	if ( kes[0].KesWechseln == 0 )
																								//-	{																// Bedingung: kein laufzeitabhängiger Wechsel
																								//-		if ( sowi == TRUE )
																								//-		{
																								//-			if ( FolgePlausibel (	kes[0].FolgeSommer ) == TRUE )	
																								//-				kes[0].FolgeSoll = kes[0].FolgeSommer;		
																								//-		}		
																								//-		else
																								//-		{
																								//-			if ( FolgePlausibel (	kes[0].FolgeWinter ) == TRUE )	
																								//-				kes[0].FolgeSoll = kes[0].FolgeWinter;		
																								//-		}	
																								//-	}
	
	
	// nach Änderung der Kesselfolge soll die Anzahl der freigegebenen Kessel gleich bleiben
		if ( KesselFolge ( ) == FALSE )				// Kessel-Folge geändert
		{
			for ( i = 0; i < KEANZ; i++ )
			{
				ke = KesFolgeIst[i] - 1;
				ked[ke].kesEin = ( i < AnzahlKesEin ) ? 1 : 0;
			}
		}
	
		for ( ke = KE1; ke < KEANZ; ke++ )
			if ( kes[ke].Sperre == TRUE || ked[ke].gestoert == TRUE || kes[ke].Haut == TRUE )
				ked[ke].kesEin = FALSE;
	
		for ( ke = KE1, n = 0; ke < KEANZ; ke++ )
			if ( ked[ke].kesEin == TRUE )
				++n;
		AnzahlKesEin = n;	
	
																										
																										//-	if ( anfExt[0].stat == 0 )
																										//-		ked[0].tvsb_anl = anfExt[0].messw;
																										//-	else if ( anaInp[U1].mwSkal.stat == 0 )
																										//-	{	
																										//-		if ( anaInp[U1].mwSkal.messw >= 200 )			// analoge Anforderung < 20 °C wird nicht akzeptiert
																										//-			ked[0].tvsb_anl = anaInp[U1].mwSkal.messw;
																										//-		else
																										//-			ked[0].tvsb_anl = 0;
																										//-	}
																										//-	else
																										//-		ked[0].tvsb_anl = kes[0].TvsbErsatz;		
																										//-	
																										
																										//-	if ( kes[0].TvsbErsatz > 0 )
																										//-		ked[0].tvsb_anl = kes[0].TvsbErsatz;	
																										//-	else if ( anaInp[U2].mwSkal.stat == 0 )
																										//-	{	
																										//-		if ( anaInp[U2].mwSkal.messw >= 200 )			// analoge Anforderung < 20 °C wird nicht akzeptiert
																										//-			ked[0].tvsb_anl = anaInp[U2].mwSkal.messw;
																										//-		else
																										//-			ked[0].tvsb_anl = 0;
																										//-	}
																										//-	else
																										//-		ked[0].tvsb_anl = 0;		
																										//-	
	
		for ( ke = KE1, n = 0; ke < KEANZ; ke++ )
		{
			Abgasmessung ( ke );
			if ( ked[ke].ssfEin )
				n = 1;
		}
		ssfEinAnl = n; 												// zur Datenübertragung auf die Heizkreis-Regler
	
		ked[0].tvsb_anl = maxAnford;
	
		for ( ke = KE1; ke < KEANZ; ke++ )
		{
			if ( ked[0].tvsb_anl > 0 && ( ( ke == KesFolgeIst[0] - 1 ) || ked[ke].kesEin == TRUE ) )
				ked[ke].tvsb = ( kes[ke].Tvpa > 0 ) ? ked[0].tvsb_anl + kes[ke].Tvpa : ked[0].tvsb_anl + ked[0].y_rel_anh;	// Anhebung addieren	
			else
				ked[ke].tvsb = 0;
				
			if ( ked[ke].ssfEin )
				ked[ke].tvsb = kes[ke].Tvma;
			else if ( ked[ke].tvsb > 0 && ked[ke].tvsb < kes[ke].Tvmi )
				ked[ke].tvsb = kes[ke].Tvmi;						// Begrenzung min.
			else if ( ked[ke].tvsb > kes[ke].Tvma )
				ked[ke].tvsb = kes[ke].Tvma;						// Begrenzung max.
		}
	
	// Führungskessel
		ke = KesFolgeIst[0] - 1;
		if ( AnzahlKesEin < 2 && ( VL_STRATEG->messw > ( ked[0].tvsb_anl + (int)kes[0].KesSchaltDiff ) ) )
		{ 
			if ( ked[0].pauseCtr > kes[0].SchaltpauseAb * 60 ) 
				ked[ke].kesEin = 0;
		}
		else if ( AnzahlKesEin < 2 && ( VL_FOLGE_AUS->stat == 0 && VL_FOLGE_AUS->messw >= kes[0].VlVerteilerMax ) )
		{ 
			if ( ked[0].pauseCtr > kes[0].SchaltpauseAb * 60 )
				ked[ke].kesEin = 0;
		}
		else if ( VL_STRATEG->messw <= ked[0].tvsb_anl && ked[0].pauseCtr > kes[0].SchaltpauseZu * 60 )
			ked[ke].kesEin = 1;
	
	// Berechnung der mittleren Modulation
		#if ( MODUL_KE1 == 2 )
		ModulationMW ( ); 
		#endif 
	
	// bei Kesseln mit Sollwertausgabe (nicht moduliert) 	
		#if ( SOLLWERT_KE1 || STUFIG_KE1 || MODUL_KE1 == 1 )
		SummeRegelabweichung ( VL_STRATEG );
		#endif
	
	// Folgekessel ein/aus
		FolgekesselSchalten ( VL_STRATEG, VL_FOLGE_AUS, RL_FOLGE_AUS );
	
		for ( ke = KE1; ke < KEANZ; ke++ )
			if ( kes[ke].Sperre == TRUE || ked[ke].gestoert == TRUE || kes[ke].Haut == TRUE )
				ked[ke].kesEin = FALSE;

		#if SM_TABW
		ked[0].abwAlarm = SM_TempAbweichung ( VL_STRATEG );
		#endif

	}

// Schalten der Regler-Ausgänge
	ke = kessel;

	if ( kes[ke].Haut == TRUE )											// Abfrage Hand-/Automatik
	{
		KE[ke]->wert = kes[ke].Kesea;
		ked[ke].kesEinKE = kes[ke].Kesea;
		ked[ke].kesEin = kes[ke].Kesea;
		KESTU2[ke]->wert = kes[ke].Kestufe2;	
		ked[ke].stufe2 = kes[ke].Kestufe2;
		PUKEEA[ke]->wert = kes[ke].Puea;							// Hand Kesselpumpe ein/aus
		KLKEAZ[ke]->wert = kes[ke].Kesklaz;

		ked[ke].fl_ei2 = 0;
		ked[ke].fl_ei1 = 0;	
		ked[ke].ei = 0;
		RVENTKE[ke]->awert = ( kes[ke].Kestell < 1000) ? kes[ke].Kestell : 1000;	
		if ( kes[ke].Kestell <= ( kes[ke].ModMin * 10 ) )
			ked[ke].y_rel = 0;
		else
			ked[ke].y_rel = (int)(( (long)(kes[ke].Kestell - kes[ke].ModMin * 10) * 1000 / (1000 - kes[ke].ModMin * 10) ) );				 
		ked[ke].fl_y_rel = ked[ke].y_rel / 10;
		
		RVENTKR[ke]->awert	= kes[ke].Kestell_rl;
		ked[ke].y_rel_rl = kes[ke].Kestell_rl;
		ked[ke].fl_y_rel_rl = (float)kes[ke].Kestell / 10;		// PID-Wert durch Handwert überschreiben 
		if ( ked[ke].rvzu )	
		{	 
			--ked[ke].rvzu;	
			ked[ke].rvauf = 0;					// Vorrang vor Auffahren 
		}                               					
		else if ( ked[ke].rvauf )			// Ausgabe für Zufahren nicht aktiv
			--ked[ke].rvauf;
		RKEZU[ke]->wert = ked[ke].rvzu;
		RKEAUF[ke]->wert = ked[ke].rvauf;

	}

	else
	{
		kes[ke].Haut = FALSE;	
		
		// bei Änderung des Schaltzustands wird der Pausen-Counter und die Abweich-Summe gelöscht
		// während des Nachlaufs beim Abschalten sind die beiden Variablen aber freigegeben 
//-//			if ( KE[ke]->wert != ked[ke].kesEin && ( ked[ke].kesEin == 1 || ked[ke].kesNachlaufCtr == kes[ke].KesNachlauf * 60 ) )
		if ( ked[ke].kesEinKE != ked[ke].kesEin )
		{
			ked[0].pauseCtr = 0;
			ked[0].abwsumme1 = 0;										// Zu- oder Abschalten eines Kessels
		}
		
		if ( ked[ke].ssfEin )
			ked[ke].kesEin = 1;
		else if ( ked[KesFolgeIst[0] - 1].kesEin == 0 )
			ked[ke].kesEin = 0;											// wenn der Führungskessel AUS ist, kann der Folgekessel nur bei Abgasmessung EIN sein 			
		
//// Fenzau: Alarm bei zu vielen Kesselstarts (impl.Ziegelhof)
//		if ( Std == 0 && Min == 0 )
//			ked[ke].startsTag = 0;						// tägliche Anzahl um 0:00 löschen
//		if ( ked[ke].smStarts == 0 && ked[ke].startsTag >= kes[ke].StartsTag )
//			ked[ke].startsTag = 0;						// SM gerade durch Quittierung gelöscht - Anzahl auch löschen	 	
		
		if ( ked[ke].kesEinKE == 0 && ked[ke].kesEin == 1 )
			KesselStart ( ke );
			
		ked[ke].kesEinKE = ked[ke].kesEin;

		
		soll = KesselSoll ( ke );
		mod = KesselMod ( ke );
		stuf = KesselStuf ( ke );
		puDz = KesselPuDz ( ke );

		ked[ke].sollwert = 0;
		if ( mod || puDz )
			BrennerModulation ( ke );
		else if ( ( mod == 0 && soll ) || ( mod && soll && kes[ke].Modulation == 0 ) )
			SollwertAusgabe ( ke );

		// Ausgabe Modulation oder Sollwert
		if ( ked[ke].kesEin )
		{
			if ( soll && mod == 2 )
				RVENTKE[ke]->awert = ( kes[ke].Modulation == 0 ) ? ( ked[ke].sollwert * 10 ) : Gerade_YvonX ( ked[ke].y_rel, 0, kes[ke].ModMin*10, 1000, 1000 );		
			else if  ( soll )
				RVENTKE[ke]->awert = ked[ke].sollwert * 10; 
			else if  ( mod == 2 )	
				RVENTKE[ke]->awert = Gerade_YvonX ( ked[ke].y_rel, 0, kes[ke].ModMin*10, 1000, 1000 );	
			else
				RVENTKE[ke]->awert = 0;	
		}
		else
			RVENTKE[ke]->awert = 0;			

		if ( mod || stuf )
			KE[ke]->wert = ked[ke].kesEin;

		if ( stuf == 2 )
		{
			if ( 	sec == 0 )
			{																		// alle 10 Sekunden
				ked[ke].stufe2 = Stufe2 ( ke, TVKE[ke] );
				KESTU2[ke]->wert = ked[ke].stufe2;
			}
		}

		// Kesselpumpe
		if ( ke == KesFolgeIst[0] - 1 )		// Führungskessel
			( ked[0].tvsb_anl > 0 || ked[ke].tvsb > 0 || ked[ke].kesEin ) ? PuEin ( ke ) : PuAus ( ke );
		else
			ked[ke].kesEin == TRUE ? PuEin ( ke ) : PuAus ( ke );		// Folgekessel	 

		// Kesselklappe
		if ( ke == KesFolgeIst[0] - 1 )		// Führungskessel
//			( ked[0].tvsb_anl > 0 || ked[ke].tvsb > 0 || ked[ke].kesEin ) ? KlAuf ( ke ) : KlZu ( ke );
			KlAuf ( ke ); 							// Klappe des Führungskessels ist IMMER offen
		else
			ked[ke].kesEin == TRUE ? KlAuf ( ke ) : KlZu ( ke );		// Folgekessel	 

		#if ( DZ_PU_KE1 || DZ_PU_KE2 )
		PumpenDrehzahl ( ke );
		#endif
	}		

	KesselBM ( ke );

	RlBegrenzung ( ke );

	KesselLaufzeit ( ke );

//-		if ( ked[ke].pu == 1 )
//-		{
//-			if ( ked[ke].kesEinKE )
//-			{
//-				tdiff = ( TRGKE[0]->stat == 0 ) ? ( ked[ke].tvsb - TRGKE[0]->messw ) : 0; 
//-				if ( tdiff <= kes[ke].TdiffMin )
//-					ked[ke].puDz = kes[ke].PuDzMax;
//-				else if ( tdiff >= kes[ke].TdiffMax )
//-					ked[ke].puDz = kes[ke].PuDzMin;	
//-				else
//-					ked[ke].puDz = Gerade_YvonX ( tdiff, kes[ke].TdiffMin, kes[ke].PuDzMax, kes[ke].TdiffMax, kes[ke].PuDzMin );		
//-			}
//-			else
//-				ked[ke].puDz = kes[ke].PuDzMin;		// Kessel AUS 
//-		}
//-		else
//-			ked[ke].puDz = 0;
//-		

//-	// Bus-Pumpendrehzahl in Abhängigkeit von der Modulation
//-	if ( ked[ke].pu == 1 )
//-	{
//-		if ( ked[ke].kesEin )
//-		{
//-				ked[ke].puDz = Gerade_YvonX ( ked[ke].y_rel, 0, kes[ke].PuDzMin, 1000, kes[ke].PuDzMax );		
//-		}
//-		else
//-			ked[ke].puDz = kes[ke].PuDzMin;		// Kessel AUS 
//-	}
//-	else
//-		ked[ke].puDz = 0;
		
	Pumpenbus_KE ( ke );
	
	KesselFreigaben ( );																	// zur Anzeige in Gruppe KES

	#if ( BM_PUKE1 || BM_PUKE2 )
	PumpeBM ( ke );
	#endif
	
}


/*****************************************************/
void KesStoer ( void )
{
	char ke;
	
	for ( ke = KE1; ke < KEANZ; ke++ )
	{
		if ( ke < KEANZ )
			ked[ke].gestoert = ( ( KEAL[ke]->bstat == 0 &&  KEAL[ke]->bwert == 1 ) 
												|| ( BRAL[ke]->bstat == 0 &&  BRAL[ke]->bwert == 1 )
												|| ( DRKE[ke]->bstat == 0 &&  DRKE[ke]->bwert == 1 )
												|| ( PUKEAL[ke]->bstat == 0 && PUKEAL[ke]->bwert == 1 )
												|| ( STBKE[ke]->bstat == 0 && STBKE[ke]->bwert == 1 )
												|| ( kes[0].FuehlerSSTM && TVKE[ke]->stat != NICHTV && TVKE[ke]->stat != 0 ) 
												) ? TRUE : FALSE;
		else
			ked[ke].gestoert = 0;		
			
	}
	
	KesGestoert.ke1 = ked[KE1].gestoert;
	KesGestoert.ke2 = ked[KE2].gestoert;
	KesGestoert.ke3 = ( KEANZ >= 3 ) ? ked[KE3].gestoert : 0;
	KesGestoert.ke4 = ( KEANZ >= 4 ) ? ked[KE4].gestoert : 0;
	KesGestoert.ke5 = ( KEANZ >= 5 ) ? ked[KE5].gestoert : 0;
	KesGestoert.ke6 = ( KEANZ >= 6 ) ? ked[KE6].gestoert : 0;
	KesGestoert.ke7 = ( KEANZ >= 7 ) ? ked[KE7].gestoert : 0;
	KesGestoert.ke8 = ( KEANZ == 8 ) ? ked[KE8].gestoert : 0;

}


/*****************************************************/
void KesGesperrt ( void )
{
	KesSperre.ke1 = kes[KE1].Sperre;	
	KesSperre.ke2 = kes[KE2].Sperre;	
	KesSperre.ke3 = ( KEANZ >= 3 ) ? kes[KE3].Sperre : 0;
	KesSperre.ke4 = ( KEANZ >= 4 ) ? kes[KE4].Sperre : 0;
	KesSperre.ke5 = ( KEANZ >= 5 ) ? kes[KE5].Sperre : 0;
	KesSperre.ke6 = ( KEANZ >= 6 ) ? kes[KE6].Sperre : 0;
	KesSperre.ke7 = ( KEANZ >= 7 ) ? kes[KE7].Sperre : 0;
	KesSperre.ke8 = ( KEANZ == 8 ) ? kes[KE8].Sperre : 0;
}


/*****************************************************/
void ModulationMW ( void )
{
	char ke;
	char n = 0;
	UINT mod = 0;
		
	for ( ke = KE1; ke <= KEANZ; ke++ )
	{
		if ( kes[ke].Haut == 0 && ked[ke].kesEin == 1 && kes[ke].Sperre == 0 && ked[ke].gestoert == 0 )
		{
			++n;
			mod += ked[ke].y_rel;
		}
	}
	ked[0].modulatMW = ( n > 0 ) ? mod / n : 0;

	if ( ked[0].modulatMW > kes[0].KesselAusMod	&& ked[0].modulatMW < kes[0].KesselEinMod )
		ked[0].kesselModCtr = 0;	
	
	if ( ked[0].modulatMW <= kes[0].KesselAusMod && AnzahlKesEin > 1 )
	{
		if ( ked[0].kesselModCtr > 0 )
			ked[0].kesselModCtr = 0;
		else if ( abs(ked[0].kesselModCtr) < 0x8000 )
			--ked[0].kesselModCtr;	
	}		
	else if ( ked[0].modulatMW >= kes[0].KesselEinMod && AnzahlKesEin >= 1 )
	{
		if ( ked[0].kesselModCtr < 0 )
			ked[0].kesselModCtr = 0;
		else if ( ked[0].kesselModCtr < 0x7FFF )
			++ked[0].kesselModCtr;
	}
	
	ked[0].modMwKesAus = ( ked[0].kesselModCtr < 0 && ( abs(ked[0].kesselModCtr) >= kes[0].KesselAusModVerz	* 60 ) ) ? 1 : 0;
	ked[0].modMwKesEin = ( ked[0].kesselModCtr >= kes[0].KesselEinModVerz	* 60 ) ? 1 : 0;

}


/*******************************************************************************************/
/***** das Zu- und Abschalten der Folgekessel wird von der Regelabweichung *****************/
/***** (Anlagen-Solltemperatur minus Temperatur am Strategiefühler) bestimmt ***************/ 
void SummeRegelabweichung ( mwsp *VL_STRATEG )
{
	static char sec;

	if ( VL_STRATEG->stat )
	{
		ked[0].abw1 = 0;
		ked[0].abwsumme1 = 0;
	}
	
	else if ( ++sec >= 6 )
	{
		sec = 0;

		ked[0].abw1 = ked[0].tvsb_anl - VL_STRATEG->messw;
		// wenn das Schaltkriterium erreicht ist, wird nicht weiter summiert

		if ( ( ked[0].abw1 > 0 && ked[0].abwsumme1 < kes[0].Einkrit1 )  || ( ked[0].abw1 < 0 && ( kes[0].Auskrit1 + ked[0].abwsumme1 > 0 ) ) )
		{																					// abwsumme1 in K*min, also /10, da alle 6 sec gerechnet wird
			if ( ked[0].abwsumme1 >= 0 )
				ked[0].abwsumme1 = ( ked[0].abw1 >= 0 ) ? ked[0].abwsumme1 + ked[0].abw1 / 10 : ked[0].abw1 / 10;
			else 
				ked[0].abwsumme1 = ( ked[0].abw1 <= 0 ) ? ked[0].abwsumme1 + ked[0].abw1 / 10 : ked[0].abw1 / 10;
		}
		
		if ( ked[0].abwsumme1 > 0 && ked[0].pauseCtr < kes[0].SchaltpauseZu * 60 )
			ked[0].abwsumme1 = 0;				// Folgekessel Zuschalten noch nicht möglich
		if ( ked[0].abwsumme1 < 0 && ked[0].pauseCtr < kes[0].SchaltpauseAb * 60 )
			ked[0].abwsumme1 = 0;				// Folgekessel Abschalten noch nicht möglich
	}
}


/*****************************************************/
void FolgekesselSchalten  ( mwsp *VL_STRATEG, mwsp *VL_FOLGE_AUS, mwsp *RL_FOLGE_AUS )
{
	char ein = FALSE;
	char aus = FALSE;
	char ke, i;
	
// Schaltkriterium Regelabweichung
	if ( ked[0].abwsumme1 >= kes[0].Einkrit1 )
		ein = TRUE;
	else if ( kes[0].Auskrit1 + ked[0].abwsumme1 <= 0 )
		aus = TRUE;
		
// Abschalten wegen Schaltdifferenz ( Vorlauf Führungskessel )
//	ke = KesFolgeIst[0] - 1;				// Führungskessel
//	if ( TVKE[ke]->messw > ( ked[ke].tvsb + (int)kes[0].KesSchaltDiff ) ) 
	if ( VL_STRATEG->messw > ( ked[0].tvsb_anl + (int)kes[0].KesSchaltDiff ) )
		aus = TRUE;

// Abschalten wegen geringer Abnahme
	if ( VL_FOLGE_AUS->stat == 0 && RL_FOLGE_AUS->stat == 0 
			&& VL_STRATEG->messw > ked[0].tvsb_anl && ( VL_FOLGE_AUS->messw - RL_FOLGE_AUS->messw < kes[0].FolgeKesAus ) )
		aus = TRUE;

// maximale Temperatur am Verteiler überschritten
	if ( VL_FOLGE_AUS->stat == 0 && VL_FOLGE_AUS->messw >= kes[0].VlVerteilerMax )
		aus = TRUE;

// Abschalten wegen geringer Modulation
	if ( ked[0].modMwKesAus > 0 )
		aus = TRUE;
	else if ( ked[0].modMwKesEin > 0 )
		ein = TRUE;
		
	if ( aus == TRUE )
		ein = FALSE;

// aus hat Vorrang
	if ( aus == TRUE && AnzahlKesEin > 1 && ked[0].pauseCtr > kes[0].SchaltpauseAb * 60 )
	{
		for ( i = 0; i < KEANZ; i++ )
		{
			ke = KesFolgeIst[i] - 1;
			ked[ke].kesEin = ( i < AnzahlKesEin - 1 ) ? 1 : 0;
			ked[0].modMwKesAus = 0;
			ked[0].kesselModCtr = 0;
		} 
	}
	else if ( ein == TRUE && AnzahlKesEin > 0 && ked[0].pauseCtr > kes[0].SchaltpauseZu * 60)
	{
		for ( i = 0; i < KEANZ; i++ )
		{
			ke = KesFolgeIst[i] - 1;
			ked[ke].kesEin = ( i <= AnzahlKesEin ) ? 1 : 0;
			ked[0].modMwKesEin = 0;
			ked[0].kesselModCtr = 0;
		} 
	}
}



/******************************************/
/***** Pumpe einschalten ******************/
		
void PuEin ( char ke )		
{
	ked[ke].puna = kes[ke].Puna * 60;	
	PUKEEA[ke]->wert = 1;									// Pumpenrelais einschalten	
	ked[ke].pu = 1;
}



/******************************************/
/***** Pumpe ausschalten mit Nachlauf *****/
/***** Blockierschutz                 *****/

void PuAus ( char ke )
{
	static UINT KesLaufzeit[] = {0,0,0,0,0,0,0,0};
	
	if ( ked[ke].puna )
		ked[ke].puna--;
		
// Blockierschutz : Freitags 03:03 (03:06) aber nur wenn Kessellaufzeit sich in der vergangenen Woche nicht verändert hat	
	if ( Wotag == 5 && Std == 3 && Min == 3*(ke+1) )
	{
		if ( KesLaufzeit[ke] != ked[ke].kesh )
			KesLaufzeit[ke] = ked[ke].kesh;
		else if ( kes[ke].Blockierschutz == TRUE )
		{
			ked[ke].puna = 120;							
			PUKEEA[ke]->wert = 1;				// Pumpe für 2 Minuten einschalten	
			ked[ke].pu = 1;
		}
	}

	if ( ked[ke].puna == 0 )
	{
		PUKEEA[ke]->wert = 0;					// Pumpenrelais ausschalten
		ked[ke].pu = 0;
	}
}	



/**************************************/
/***** Klappe öffnen ******************/
		
void KlAuf ( char ke )		
{
	ked[ke].klna = kes[ke].Klna * 60;	
	KLKEAZ[ke]->wert = 1;
}


/******************************************/
/***** Klappe schliessen mit Nachlauf *****/
/***** Blockierschutz                 *****/

void KlZu ( char ke )
{
	static UINT KesLaufzeit[] = {0,0,0,0,0,0,0,0};
	
	if ( ked[ke].klna )
		ked[ke].klna--;
		
// Blockierschutz : Freitags 04:03 (04:06) aber nur wenn Kessellaufzeit sich in der vergangenen Woche nicht verändert hat	
	if ( Wotag == 5 && Std == 4 && Min == 3*(ke+1) )
	{
		if ( KesLaufzeit[ke] != ked[ke].kesh )
			KesLaufzeit[ke] = ked[ke].kesh;
		else if ( kes[ke].Blockierschutz == TRUE )
		{
			ked[ke].klna = 120;							
			KLKEAZ[ke]->wert = 1;
		}
	}

	if ( ked[ke].klna == 0 )
	{
		KLKEAZ[ke]->wert = 0;
	}
}	



/*****************************************************/
char KesselFolge ( void )
{
	static char neustart = TRUE;
	ULONG f;
	char i, k, ke;
	char ret = TRUE;				// bei geänderter Folge wird FALSE zurückgegeben

	if ( neustart == TRUE )
	{
		if ( FolgePlausibel (	kes[0].FolgeSoll ) == FALSE )
		{
			if ( KEANZ == 1 )
				kes[0].FolgeSoll = 1;			
			else if ( KEANZ == 2 )
				kes[0].FolgeSoll = 12;			
			else if ( KEANZ == 3 )
				kes[0].FolgeSoll = 123;			
			else if ( KEANZ == 4 )
				kes[0].FolgeSoll = 1234;			
			else if ( KEANZ == 5 )
				kes[0].FolgeSoll = 12345;			
			else if ( KEANZ == 6 )
				kes[0].FolgeSoll = 123456;	
			else if ( KEANZ == 7 )
				kes[0].FolgeSoll = 1234567;	
			else if ( KEANZ == 8 )
				kes[0].FolgeSoll = 12345678;											
//			bicbus ( EEPADR, (char*)&kes[KE1], KE_ADR, 4, BICWR ); 	// Kesselfolge sichern	kes[KE1].FolgeSoll ist 1.Parameter
		}
		ked[0].folgeSollAlt = kes[0].FolgeSoll;
		ked[0].folgeIstAlt = kes[0].FolgeSoll;

//-		if ( FolgePlausibel (	kes[0].FolgeSommer ) == FALSE )
//-		{
//-			if ( KEANZ == 1 )
//-				kes[0].FolgeSommer = 1;			
//-			else if ( KEANZ == 2 )
//-				kes[0].FolgeSommer = 12;			
//-			else if ( KEANZ == 3 )
//-				kes[0].FolgeSommer = 123;			
//-			else if ( KEANZ == 4 )
//-				kes[0].FolgeSommer = 1234;			
//-			else if ( KEANZ == 5 )
//-				kes[0].FolgeSommer = 12345;			
//-			else if ( KEANZ == 6 )
//-				kes[0].FolgeSommer = 123456;			
//-		}
//-
//-		if ( FolgePlausibel (	kes[0].FolgeWinter ) == FALSE )
//-		{
//-			if ( KEANZ == 1 )
//-				kes[0].FolgeWinter = 1;			
//-			else if ( KEANZ == 2 )
//-				kes[0].FolgeWinter = 12;			
//-			else if ( KEANZ == 3 )
//-				kes[0].FolgeWinter = 123;			
//-			else if ( KEANZ == 4 )
//-				kes[0].FolgeWinter = 1234;			
//-			else if ( KEANZ == 5 )
//-				kes[0].FolgeWinter = 12345;			
//-			else if ( KEANZ == 6 )
//-				kes[0].FolgeWinter = 123456;			
//-		}
//-
		bicbus ( EEPADR, (char*)&kes[KE1], KE_ADR, ( ( KESLENG > 64 ) ? 64 : KESLENG ), BICWR ); 
		if ( KESLENG > 64 )
			bicbus ( EEPADR, (char*)&kes[KE1]+64, KE_ADR+64, KESLENG-64, BICWR ); 

		neustart = FALSE;	
	}
	else if ( ked[KE1].folgeSollAlt != kes[KE1].FolgeSoll ) 
	{
		if ( FolgePlausibel (	kes[KE1].FolgeSoll ) == FALSE )
		{
			kes[KE1].FolgeSoll = ked[KE1].folgeSollAlt;
		}
		else
		{
			ked[KE1].folgeSollAlt = kes[KE1].FolgeSoll;
		}
		bicbus ( EEPADR, (char*)&kes[KE1], KE_ADR, 4, BICWR ); 	// Kesselfolge sichern	
	}

	KesFolgeSoll[0] = kes[KE1].FolgeSoll / 10000000;
	f = kes[KE1].FolgeSoll % 10000000;
	KesFolgeSoll[1] = f / 1000000;
	f = f % 1000000;
	KesFolgeSoll[2] = f / 100000;
	f = f % 100000;
	KesFolgeSoll[3] = f / 10000;
	f = f % 10000;
	KesFolgeSoll[4] = f / 1000;
	f = f % 1000;
	KesFolgeSoll[5] = f / 100;
	f = f % 100;
	KesFolgeSoll[6] = f / 10;
	KesFolgeSoll[7] = f % 10;

// versteh ich nicht mehr. Wie kann KesFolgeSoll[0] = 0 sein, wenn die Anzahl der eingegebenen Ziffern der Kessel-Anzahl entspricht ? 
// 160720: ist wohl doch richtig. Dann kommt der Führunskessel nach KesFolgeSoll[0]
	if ( KEANZ < 8 )
	{
		while ( KesFolgeSoll[0] == 0 )
			for ( i = 0; i < 7; i++ )
			{
				KesFolgeSoll[i] = KesFolgeSoll[i+1];
				KesFolgeSoll[i+1] = 0;
			}
	}


	if ( kes[0].KesWechseln > 0 )
		FolgeAendernLaufzeit ( );

	for ( i = 0; i < KEANZ; i++ )
		KesFolgeIst[i] = 0;

// Kesselstörungen/-sperren untersuchen	
	for ( i = 0; i < KEANZ; i++ )
	{
		ke = KesFolgeSoll[i];
		if ( ke > 0 && ke <= KEANZ )
		{
			ke -= 1;
			if ( kes[ke].Sperre == 0 && ked[ke].gestoert == 0 && kes[ke].Haut == 0 ) 
			{
				for ( k = 0; k < KEANZ; k++ )
				{
					if ( KesFolgeIst[k] == 0 )
					{
						KesFolgeIst[k] = ke + 1;
						break;
					}
				}
			}
		}
	}

// 151104: nun kommen zuerst die gestörten, dann die gesperrten und dann die auf Hand gestellten Kessel
// Grund: sonst kann ein auf Hand gestellter Kessel zum Führungskessel werden, wenn alle anderen gestört/gesperrt sind
// Folge: alle Absperrklappen sind geschlossen, das muss vermieden werden (ThoSi)
//-	for ( i = 0; i < KEANZ; i++ )
//-	{
//-		ke = KesFolgeSoll[i];
//-		if ( ke > 0 && ke <= 6 )
//-		{
//-			ke -= 1;
//-			if ( kes[ke].Sperre != 0 || ked[ke].gestoert != 0 || kes[ke].Haut != 0  ) 
//-			{
//-				for ( k = 0; k < KEANZ; k++ )
//-				{
//-					if ( KesFolgeIst[k] == 0 )
//-					{
//-						KesFolgeIst[k] = ke + 1;
//-						break;
//-					}
//-				}
//-			}
//-		}
//-	}
	for ( i = 0; i < KEANZ; i++ )
	{
		ke = KesFolgeSoll[i];
		if ( ke > 0 && ke <= KEANZ )
		{
			ke -= 1;
			if ( ked[ke].gestoert != 0 ) 
			{
				for ( k = 0; k < KEANZ; k++ )
				{
					if ( KesFolgeIst[k] == 0 )
					{
						KesFolgeIst[k] = ke + 1;
						break;
					}
				}
			}
		}
	}	
	for ( i = 0; i < KEANZ; i++ )
	{
		ke = KesFolgeSoll[i];
		if ( ke > 0 && ke <= KEANZ )
		{
			ke -= 1;
			if ( kes[ke].Sperre != 0 ) 
			{
				for ( k = 0; k < KEANZ; k++ )
				{
					if ( KesFolgeIst[k] == 0 )
					{
						KesFolgeIst[k] = ke + 1;
						break;
					}
				}
			}
		}
	}	
	for ( i = 0; i < KEANZ; i++ )
	{
		ke = KesFolgeSoll[i];
		if ( ke > 0 && ke <= KEANZ )
		{
			ke -= 1;
			if ( kes[ke].Haut != 0  ) 
			{
				for ( k = 0; k < KEANZ; k++ )
				{
					if ( KesFolgeIst[k] == 0 )
					{
						KesFolgeIst[k] = ke + 1;
						break;
					}
				}
			}
		}
	}

	ked[KE1].folgeIst = KesFolgeIst[0];
	for ( k = 1;  k < KEANZ; k++ )
		ked[KE1].folgeIst = ked[KE1].folgeIst * 10 + KesFolgeIst[k];;
		
	if ( FolgePlausibel (	ked[KE1].folgeIst ) == FALSE )
	{
		if ( KEANZ == 1 )
			ked[KE1].folgeIst	= 1;
		else if ( KEANZ == 2 )
			ked[KE1].folgeIst	= 12;
		else if ( KEANZ == 3 )
			ked[KE1].folgeIst	= 123;
		else if ( KEANZ == 4 )
			ked[KE1].folgeIst	= 1234;		
		else if ( KEANZ == 5 )
			ked[KE1].folgeIst	= 12345;		
		else if ( KEANZ == 6 )
			ked[KE1].folgeIst	= 123456;		
		else if ( KEANZ == 7 )
			ked[KE1].folgeIst	= 1234567;		
		else if ( KEANZ == 8 )
			ked[KE1].folgeIst	= 12345678;		
	}

	if ( ked[KE1].folgeIst != ked[KE1].folgeIstAlt )
		ret = FALSE;
	ked[KE1].folgeIstAlt = ked[KE1].folgeIst;
	
	return ( ret );
}


/*****************************************************/
char FolgePlausibel ( ULONG kesselfolge )
{
	char ret = TRUE;
	ULONG f;
	char folge[8];
	char i;
	char b;
	
	if ( KEANZ == 1 )
	{
		if ( kesselfolge != 1 )
			ret = FALSE;
	}
	else if ( KEANZ == 2 )
	{
		if ( kesselfolge < 12 || kesselfolge > 21 )
			ret = FALSE;
	}
	else if ( KEANZ == 3 )
	{
		if ( kesselfolge < 123 || kesselfolge > 321 )
			ret = FALSE;
	}
	else if ( KEANZ == 4 )
	{
		if ( kesselfolge < 1234 || kesselfolge > 4321 )
			ret = FALSE;
	}
	else if ( KEANZ == 5 )
	{
		if ( kesselfolge < 12345 || kesselfolge > 54321 )
			ret = FALSE;
	}	
	else if ( KEANZ == 6 )
	{
		if ( kesselfolge < 123456 || kesselfolge > 654321 )
			ret = FALSE;
	}
	else if ( KEANZ == 7 )
	{
		if ( kesselfolge < 1234567 || kesselfolge > 7654321 )
			ret = FALSE;
	}	
	else if ( KEANZ == 8 )
	{
		if ( kesselfolge < 12345678 || kesselfolge > 87654321 )
			ret = FALSE;
	}	
	
	else
		ret = FALSE;

	if ( ret == TRUE )
	{
		folge[0] = kesselfolge / 10000000;
		f = kesselfolge % 10000000;
		folge[1] = f / 1000000;
		f = f % 1000000;
		folge[2] = f / 100000;
		f = f % 100000;
		folge[3] = f / 10000;
		f = f % 10000;
		folge[4] = f / 1000;
		f = f % 1000;
		folge[5] = f / 100;
		f = f % 100;
		folge[6] = f / 10;
		folge[7] = f % 10;
		
	// Plausibilitätstest: die Kessel 1-(3..6) müssen jeweils einmal in der Folge enthalten sein
		for ( i = 0, b = 0; i <= 7; i++ )
			if ( folge[i] > 0 && folge[i] <= 8 )
				b =  b | (1 << ( folge[i] - 1 ) );

		if ( KEANZ == 1 )
		{
			if ( b != 1 )
				ret = FALSE;
		}
		else if ( KEANZ == 2 )
		{
			if ( b != 3 )
				ret = FALSE;
		}
		else if ( KEANZ == 3 )
		{
			if ( b != 7 )
				ret = FALSE;
		}
		else if ( KEANZ == 4 )
		{
			if ( b != 0x0F )
				ret = FALSE;
		}
		else if ( KEANZ == 5 )
		{
			if ( b != 0x1F )
				ret = FALSE;
		}
		else if ( KEANZ == 6 )
		{
			if ( b != 0x3F )
				ret = FALSE;
		}
		else if ( KEANZ == 7 )
		{
			if ( b != 0x7F )
				ret = FALSE;
		}
		else if ( KEANZ == 8 )
		{
			if ( b != 0xFF )
				ret = FALSE;
		}
	}
	
	return ( ret );
}


	
/*****************************************************/
// Laufzeitabhängigen Änderung der Kesselfolge 
void FolgeAendernLaufzeit ( void )
{
	char iLzMin = 0;
	char iLzMax = 0;
	UINT lzMin = 0xFFFF;
	UINT lzMax = 0;
	char i, ke;
	
	for ( i = 0; i < KEANZ; i++ )
	{
		ke = KesFolgeSoll[i];
		if ( ke > 0 && ke <= KEANZ )	
		{
			ke--;
//-			if ( ked[ke].kesh < lzMin )
//-			{
//-				lzMin = ked[ke].kesh;
//-				iLzMin = i;
//-			}
//-			if ( ked[ke].kesh > lzMax )
//-			{
//-				lzMax = ked[ke].kesh;
//-				iLzMax = i;
//-			}
			if ( ked[ke].gestoert == 0 )
			{			
				if ( ked[ke].kesh < lzMin )
				{
					lzMin = ked[ke].kesh;
					iLzMin = i;
				}
				if ( ked[ke].kesh > lzMax )
				{
					lzMax = ked[ke].kesh;
					iLzMax = i;
				}	
			}		
		}
	}

	if ( iLzMax < iLzMin && ( lzMax >= lzMin + kes[0].KesWechseln ) )
	// Kessel mit der niedrigsten und größten Laufzeit werden getauscht,
	// wenn der Kessel mit der größeren Laufzeit höher priorisiert ist
	{
		ke = KesFolgeSoll[iLzMin];	
		KesFolgeSoll[iLzMin] = KesFolgeSoll[iLzMax];	
		KesFolgeSoll[iLzMax] = ke;
	}	


	kes[0].FolgeSoll = KesFolgeSoll[0];
	for ( i = 1;  i < KEANZ; i++ )
		kes[0].FolgeSoll = kes[0].FolgeSoll * 10 + KesFolgeSoll[i];;

}



/******************************************/
void KesselStart ( char ke )
{
	ked[ke].kestarts++;											// Anzahl der Einschaltungen erhöhen
	bicbus(EEPADR, (char*)&ked[ke].kestarts, KESTARTS_ADR+(ke*2), 2, BICWR);

// Fenzau
//	if ( kes[ke].StartsTag )
//	{
//		++ked[ke].startsTag;
//		if ( ked[ke].startsTag >= kes[ke].StartsTag )
//			ked[ke].smStarts = 1;	 	
//	}	
}
	


/*****************************************************/
void KesselFreigaben ( void )
{
	KesEin.ke1 = ked[KE1].kesEin;				
	KesEin.ke2 = ked[KE2].kesEin;				
	KesEin.ke3 = ( KEANZ >= 3 ) ? ked[KE3].kesEin : 0;				
	KesEin.ke4 = ( KEANZ >= 4 ) ? ked[KE4].kesEin : 0;				
	KesEin.ke5 = ( KEANZ >= 5 ) ? ked[KE5].kesEin : 0;				
	KesEin.ke6 = ( KEANZ >= 6 ) ? ked[KE6].kesEin : 0;				
	KesEin.ke7 = ( KEANZ >= 7 ) ? ked[KE7].kesEin : 0;				
	KesEin.ke8 = ( KEANZ >= 8 ) ? ked[KE8].kesEin : 0;				
			
}


/*****************************************************/
char KesselSoll ( char ke )
{
	char ret = 0;
	
	if ( ke == KE1 )
		ret = SOLLWERT_KE1;
	else if ( ke == KE2 )
		ret = SOLLWERT_KE2;
	#if KEANZ > KE3 
	else if ( ke > KE3 )
		ret = SOLLWERT_KE3;
	#endif
	#if KEANZ > KE4 
	else if ( ke > KE4 )
		ret = SOLLWERT_KE4;
	#endif
	#if KEANZ > KE5 
	else if ( ke > KE5 )
		ret = SOLLWERT_KE5;
	#endif
	#if KEANZ > KE6 
	else if ( ke > KE6 )
		ret = SOLLWERT_KE6;
	#endif
	#if KEANZ > KE7 
	else if ( ke > KE7 )
		ret = SOLLWERT_KE7;
	#endif
	#if KEANZ > KE8 
	else if ( ke > KE8 )
		ret = SOLLWERT_KE8;
	#endif
	
	return ( ret );								
}


/*****************************************************/
char KesselMod ( char ke )
{
	char ret = 0;
	
	if ( ke == KE1 )
		ret = MODUL_KE1;
	else if ( ke == KE2 )
		ret = MODUL_KE2;
	#if KEANZ > KE3 
	else if ( ke == KE3 )
		ret = MODUL_KE3;
	#endif
	#if KEANZ > KE4 
	else if ( ke == KE4 )
		ret = MODUL_KE4;
	#endif
	#if KEANZ > KE5 
	else if ( ke == KE5 )
		ret = MODUL_KE5;
	#endif
	#if KEANZ > KE6 
	else if ( ke == KE6 )
		ret = MODUL_KE6;
	#endif
	#if KEANZ > KE7 
	else if ( ke == KE7 )
		ret = MODUL_KE7;
	#endif
	#if KEANZ > KE8 
	else if ( ke == KE8 )
		ret = MODUL_KE8;
	#endif
	
	return ( ret );								
}


/*****************************************************/
char KesselStuf ( char ke )
{
	char ret = 0;
	
	if ( ke == KE1 )
		ret = STUFIG_KE1;
	else if ( ke == KE2 )
		ret = STUFIG_KE2;
	#if KEANZ > KE3 
	else if ( ke > KE3 )
		ret = STUFIG_KE3;
	#endif
	#if KEANZ > KE4 
	else if ( ke > KE4 )
		ret = STUFIG_KE4;
	#endif
	#if KEANZ > KE5 
	else if ( ke > KE5 )
		ret = STUFIG_KE5;
	#endif
	#if KEANZ > KE6 
	else if ( ke > KE6 )
		ret = STUFIG_KE6;
	#endif
	#if KEANZ > KE7 
	else if ( ke > KE7 )
		ret = STUFIG_KE7;
	#endif
	#if KEANZ > KE8 
	else if ( ke > KE8 )
		ret = STUFIG_KE8;
	#endif
	
	return ( ret );								
}


/*****************************************************/
char KesselPuDz ( char ke )
{
	char ret = 0;
	
	if ( ke == KE1 )
		ret = DZ_PU_KE1;
	else if ( ke == KE2 )
		ret = DZ_PU_KE2;
	#if KEANZ > KE3 
	else if ( ke > KE3 )
		ret = DZ_PU_KE3;
	#endif
	#if KEANZ > KE4 
	else if ( ke > KE4 )
		ret = DZ_PU_KE4;
	#endif
	#if KEANZ > KE5 
	else if ( ke > KE5 )
		ret = DZ_PU_KE5;
	#endif
	#if KEANZ > KE6 
	else if ( ke > KE6 )
		ret = DZ_PU_KE6;
	#endif
	#if KEANZ > KE7 
	else if ( ke > KE7 )
		ret = DZ_PU_KE7;
	#endif
	#if KEANZ > KE8 
	else if ( ke > KE8 )
		ret = DZ_PU_KE8;
	#endif
	
	return ( ret );								
}

#endif	// if KEANZ > 1
	