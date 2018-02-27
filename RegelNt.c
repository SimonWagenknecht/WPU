/*	Task:															RegelNt			*/
/*	Fernwärmevorregelung (Netztrennung)						*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"

void Pumpenbus_Nt ( char nt );
void PuNtBm ( char nt );


/*-------------------------------- Task RegelNt -----------------------------------*/
// Takt: 1 sek
void RegelNt(void)
{
	float fl_ei = 0;
	float fl_dy_rel = 0;	
	int tvsb = 0;
	int eir;
	char i;

	PuNtBm ( NT1 );

	// Abfrage Handbetrieb
	if(nts[NT1].Haut == TRUE)			
	{	RNTZU[NT1]->wert = ntd[NT1].vzu;	// Handwert[s] für Zufahren ausgeben
		if(ntd[NT1].vzu > 0)							// Ausgabe aktiv ?
		{	 ntd[NT1].vzu -= 1;							// Zeit decrementieren
			 ntd[NT1].vauf = 0;							// Vorrang vor Auffahren 
		}
		else															// Ausgabe für Zufahren nicht aktiv
		{	RNTAUF[NT1]->wert = ntd[NT1].vauf;// Handwert[s] für Auffahren ausgeben
			if(ntd[NT1].vauf > 0)						// Ausgabe aktiv ?
				 ntd[NT1].vauf -= 1;					// Zeit decrementieren
		}
		RVENTNT[NT1]->awert = nts[NT1].Vstell;	// Analogventil 0-100,0% 
		#if PU_NT1 > 0
			PUNTEA[NT1]->wert = nts[NT1].Puea;		// Pumpenrelais Handwert eintragen		
		#endif
		
		return;		
	}
	else																// Automatikbetrieb
	{	nts[NT1].Haut = 0;								// evtl. Fehleingaben löschen
		ntd[NT1].vzu	= 0;								// Handwerte löschen
		ntd[NT1].vauf	= 0;
	}

	if ( ntd[NT1].tvsb > 0 )
		ntd[NT1].pu = 1;

	#if HKANZ == 0
	else if ( ntd[NT1].blockPuEin > 0 )
	{
		--ntd[NT1].blockPuEin;
		ntd[NT1].pu = 1;		
	} 
	#endif
	#if HKANZ > 0
	else if ( PU[HK1]->func == STATISCH && PU[HK1]->wert == 1 )
		ntd[NT1].pu = 1;
	#endif
	#if HKANZ > 1	
	else if ( PU[HK2]->func == STATISCH && PU[HK2]->wert == 1 )
		ntd[NT1].pu = 1;	
	#endif		
	#if HKANZ > 2
	else if ( PU[HK3]->func == STATISCH && PU[HK3]->wert == 1 )
		ntd[NT1].pu = 1;
	#endif		
	#if HKANZ > 3
	else if ( PU[HK4]->func == STATISCH && PU[HK4]->wert == 1 )
		ntd[NT1].pu = 1;
	#endif


	else
		ntd[NT1].pu = 0;

		
// Blockierschutz: wenn im Projekt mindestens ein Heizkreis existiert, läuft die Haupt- gemeinsam mit der Heizkreispumpe
	#if HKANZ == 0
	if ( ntd[NT1].pu == 0 )
	{
		if ( ++ntd[NT1].blockSec >=  3600 )
		{
			ntd[NT1].blockSec = 0;
			++ntd[NT1].blockStd;
			if ( ntd[NT1].blockStd >= 169 )		// 7 Tage plus eine Stunde
				ntd[NT1].blockPuEin = 120;			// zwei Minuten Pumpe ein
		}		
	}
	else
	{
		ntd[NT1].blockSec = 0;	
		ntd[NT1].blockStd = 0;	
	}	
	#endif		


	#if PU_NT1 > 0
	PUNTEA[NT1]->wert	= ntd[NT1].pu;
	#endif
	
	// Regler-Tastzeit
	if(++ntd[NT1].zts < nts[NT1].Ts/10)
		 return;													// Tastzeit noch nicht erreicht
	ntd[NT1].zts = 0;
	
	
	// Sollwertermittlung
	// Maximale Solltemp. ermitteln (Anhebung wirkt nur wenn Anforderung > 0)
	tvsb = ( maxAnford > 0 ) ? ( maxAnford + nts[NT1].Tvpa ) : 0;	
	
	// Rücklaufbegrenzung
	ntd[NT1].rlbegr = 0;							// Voreinstellung
	if ( TRP[NT1]->stat == 0 && tvsb != 0 )						// wenn Rücklauffühler Primär ok und tvsb ungleich 0
	{	// Abweichung RÜCKLAUF - RÜCKLAUF.MAX
		eir = TRP[NT1]->messw - nts[NT1].Trma;
		if(eir >= 0)										// nur wenn MAX überschritten
		{	ntd[NT1].rlbegr = 1;					// Statusanzeige Rücklaufbegrenzung setzen
			tvsb -= (int) ( (float)eir * ((float)nts[NT1].Kpr / 100) );
			if ( tvsb < nts[NT1].Tvmi )
				tvsb = nts[NT1].Tvmi;				
		}	
	}
	
//#####ulsch
// Begrenzung
	if ( tvsb != 0 )													// im Sommer bleibt es bei 0°C Anforderung -> Ventil ZU	
	{																					
		if ( tvsb < nts[NT1].Tvmi )
			tvsb = nts[NT1].Tvmi;		
		else if ( tvsb > nts[NT1].Tvma )
			tvsb = nts[NT1].Tvma;
	}

	// Sollwert speichern	
	ntd[NT1].tvsb = tvsb;

	if(tvsb > 0)
	{
		// Dämpfung des berechneten Sollwertes: tvsb --> gedämpfte tsol
		if(ntd[NT1].regstart == 0)												// nach Reset
		{	 ntd[NT1].tsol			= ntd[NT1].tvsb;						// keine Filterung
			 ntd[NT1].fl_tsol		= (float)ntd[NT1].tsol / 10;
			 ntd[NT1].regstart	= 1;
		}	 
		else
		{
			ntd[NT1].fl_tsol = g_filter(ntd[NT1].fl_tsol, ntd[NT1].tvsb, nts[NT1].Ts, nts[NT1].Fzk);
			ntd[NT1].tsol 	 = (int)(ntd[NT1].fl_tsol * 10);						// für Anzeige
		}
		
		//	----------------------------------------------------------------------
		//	Reglertyp: P- / PID-Regler
		//	----------------------------------------------------------------------
		// Test Sekundärfühler
		if ( TVSEK[NT1]->stat == 0 )	// Status: 0...Ok, 41H...Überlauf,  21H...Unterlauf
		{	
			// Regelabweichung Soll - Ist
			fl_ei	= ntd[NT1].fl_tsol - (float)TVSEK[NT1]->messw / 10;
			
			// -------------- PID-Regelalgorithmus ---------------------------------
			// Berechnung der relativen Stellgrößen
			fl_dy_rel = Dy_rel ( nts[NT1].Kp, nts[NT1].Kd, nts[NT1].Ts, nts[NT1].Tn, 
															fl_ei, ntd[NT1].fl_ei1, ntd[NT1].fl_ei2 );
			
			ntd[NT1].fl_y_rel += fl_dy_rel;											// errechnete PID-Stellgröße
					
			//---------------------------------------------------------------------
			// Regelabweichungen merken
			ntd[NT1].fl_ei2 = ntd[NT1].fl_ei1;
			ntd[NT1].fl_ei1 = fl_ei;						
			
			// Berechnung und Ausgabe der absoluten Stellgrößen
			// --------------------------------------------------------------------
			// Ausgabe an 3-Punkt Ventil
			if ( nts[NT1].Vst > 0 )
			{	
				ntd[NT1].stellzeit = (int)( fl_dy_rel * (float)nts[NT1].Hub * 60 / (float)nts[NT1].Vst ); // [0.01 s]
				ntd[NT1].stellsum += ntd[NT1].stellzeit;
				if(abs(ntd[NT1].stellsum) >= 100)									// ab 1 Sekunde ausgeben
				{
					ntd[NT1].fahren		= ntd[NT1].stellsum / 100;		// nur ganze Sekunden fahren
					ntd[NT1].stellsum	= ntd[NT1].stellsum % 100; 		// Rest merken
					if(ntd[NT1].fahren > 0)		
						RNTAUF[NT1]->wert = (char)ntd[NT1].fahren;
					else
						RNTZU[NT1]->wert  = (char)abs(ntd[NT1].fahren);	
				}
			}	
			
			// Ausgabe an 0-10V Ventil

			// Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
			ntd[NT1].fl_y_rel = anti_windup( ntd[NT1].fl_y_rel, nts[NT1].Kp, nts[NT1].Wup, fl_ei );		// in Funktion.c
			ntd[NT1].si_y_rel = (int)(ntd[NT1].fl_y_rel * 10);	// debug
			 
			// Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
			ntd[NT1].y_rel = y_stell(ntd[NT1].fl_y_rel);				// in Funktion.c
			
			RVENTNT[NT1]->awert = ntd[NT1].y_rel;								// Ausgabe an Stellventil
			
			// Parameterumwandlung float to int
			ntd[NT1].ei = (int)(fl_ei * 10);										// [0,1%] zur Anzeige
		}
	}
	else	// 	tvsb == 0
	{	
		ntd[NT1].fl_ei1	  = 0;
		ntd[NT1].ei				= 0;
		RNTAUF[NT1]->wert	= 0;
		RNTZU[NT1]->wert	= 255;				// Regelventil ständig zufahren
		ntd[NT1].y_rel		= 0;
		RVENTNT[NT1]->awert = 0;				// Ausgabe an Stellventil
	}	

		#if ( BUS_PU_MAX > 0 )
		Pumpenbus_Nt ( NT1 );								// nur im Automatikbetrieb
		#endif	
}
/*-------------------------------- Task RegelNt Ende -----------------------------------*/

	


void Pumpenbus_Nt ( char nt )
{
#if BUS_PU_MAX > 0
	char pu = 0;
	
	if ( nt == NT1 )
		pu = PU_BUS_NT1;
	else if ( nt == NT2 )
		pu = PU_BUS_NT2;


	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
		
		if ( BusPuPara[pu].Funktion == 1 )
		{
			if ( 	ntd[NT1].pu > 0 )
			{
				BusPuPara[pu].Betrieb = 1;
// 				GeniPara[pu].Sollwert = ??? falls nicht konstant

			}
			else
			{
				BusPuPara[pu].Betrieb = 0;
//					GeniPara[pu].Sollwert = 0; falls nicht konstant
			}
		}

		#if GENI == 1	
		ntd[nt].busPuEin = BusPuData[pu].uc_betriebsart;	
		#endif

		#if WILO_MODBUS == 1	
		ntd[nt].busPuEin = modWiloPu[pu].output_value1_temp;	
		#endif

		#if GRUNDFOS_MODBUS == 1
		
		#endif
	}
#endif

}



// Betriebsmeldung der Hauptpumpe überwachen
void PuNtBm ( char nt )
{
	if ( BMPUNT[nt]->bstat != NICHTV && nts[nt].PuBmVerz > 0 && nts[NT1].Haut == 0 )	
	{																	// Nur wenn Betriebsmelde-Kontakt vorhanden und Verzögerung nicht 0
		if( PUNTEA[nt]->wert == 1 )					// Pumpe EIN ?
		{
			if ( BMPUNT[nt]->bwert == 1 )			// Betriebsmeldung auch EIN ?
			{
				ntd[nt].puBm = 0;								// kein Alarm
				ntd[nt].puBmVerz = 0;
			}
			else
			{
				if ( ntd[nt].puBmVerz < 0xFFFF )
					++ntd[nt].puBmVerz;
				if ( ntd[nt].puBmVerz >= nts[nt].PuBmVerz * 60 )
					ntd[nt].puBm = 1;					// Soft-Alarm: 'kein Betriebsmeldungs-Signal'
			}
		}
		else
			ntd[nt].puBmVerz = 0;	
	}	
	else
	{	
		ntd[nt].puBm = 0;								// kein Alarm
		ntd[nt].puBmVerz = 0;
	}	
}

