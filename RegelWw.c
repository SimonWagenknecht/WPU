/*	Task:					RegelWw			*/
/*	Warmwasserregelung						*/

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"


/*-------------------------------- Task RegelWw -----------------------------------*/
// Takt: 1 sek

#if WWANZ

void RegelWw ( void )
{
	float fl_ei = 0;
	float fl_dy_rel = 0;	
	char regel;
	char rcond;
	char sp;
	char lpu, tpu;
	
	char ww;
	
	for ( ww = WW1; ww < WWANZ; ww++ )
	{
		if ( Projekte[proj_typ].wwcode[ww] == 0 )
			return;														// WW-Kreis nicht aktiv

		// Abfrage Handbetrieb
		if ( wws[ww].Haut == TRUE )	
		{	
			RWWZU[ww]->wert = wwd[ww].vzu;		// Handwert[s] für Zufahren ausgeben
			if ( wwd[ww].vzu > 0 )						// Ausgabe aktiv ?
			{	 
				wwd[ww].vzu -= 1;								// Zeit dekrementieren
				wwd[ww].vauf = 0;								// Vorrang vor Auffahren 
			}
			else															// Ausgabe für Zufahren nicht aktiv
			{	
				RWWAUF[ww]->wert = wwd[ww].vauf;// Handwert[s] für Auffahren ausgeben
				if ( wwd[ww].vauf > 0 )					// Ausgabe aktiv ?
					wwd[ww].vauf -= 1;						// Zeit dekrementieren
			}
			RVENTWW[ww]->awert = wws[ww].Vstell;	// Analogventil 0-100,0% 
			return;		
		}
		else																// Automatikbetrieb
		{	
			wws[ww].Haut = 0;									// evtl. Fehleingaben löschen
			wwd[ww].vzu	= 0;									// Handwerte löschen
			wwd[ww].vauf	= 0;
		}
	
		// Regler-Tastzeit
		if ( ( wwd[ww].zts += 10 ) < wws[ww].Ts )
			 return;													// Tastzeit noch nicht erreicht
		wwd[ww].zts = 0;

		// Regelbedingungen abfragen
		regel = FALSE;
		rcond = ( ww == WW1 ) ? RCOND_WW1 : RCOND_WW2;
		lpu = ( ww == WW1 ) ? ( LPU_WW1 || PU_BUS_LPU_WW1 ) : ( LPU_WW2 || PU_BUS_LPU_WW2 );
		tpu = ( ww == WW1 ) ? ( TPU_WW1 || PU_BUS_TPU_WW1 ) : ( TPU_WW2 || PU_BUS_TPU_WW2 );
		if ( rcond == 0 )
		{
			wwd[ww].twwsb = ( wwd[ww].legio ) ? wws[ww].TwwsolLegio : wws[ww].Twwsol;	
	
			if ( wwd[ww].regstart == 0 )									// nach Reset
			{
				wwd[ww].fl_tsol	= (float)wwd[ww].twwsb / 10;
				wwd[ww].regstart = 1;		
			}
			else
			{	// Dämpfung des berechneten Sollwertes: twwsb --> gedämpfte tsol		
				wwd[ww].fl_tsol = g_filter(wwd[ww].fl_tsol, wwd[ww].twwsb, wws[ww].Ts, wws[ww].Fzk);
			}
			wwd[ww].tsol = (int)( wwd[ww].fl_tsol * 10 );	// für Anzeige
			regel = TRUE;
		}
		else if ( rcond == 1 )
		{
//			if ( wwd[ww].wwlad || ( PUWLEA[ww]->func != NICHTV && PUWLEA[ww]->wert ) || ( PUWTEA[ww]->func != NICHTV && PUWTEA[ww]->wert ) )
// Ansteuerung der Pumpe über Bus wird nun berücksichtigt
			if ( wwd[ww].wwlad || ( lpu && wwd[ww].zpuwlna ) || ( tpu && wwd[ww].zpuwtna ) )
			{																							// ja	
				wwd[ww].tsol = wws[ww].Twwlad;							// Sollwert ohne Dämpfung
				
				if ( wwd[ww].legio && wws[ww].TwwladLegio > wws[ww].Twwlad )
					wwd[ww].tsol = wws[ww].TwwladLegio;		
	
				wwd[ww].fl_tsol	= (float)wwd[ww].tsol / 10;
				regel = TRUE;	
			}	
			else
				wwd[ww].tsol = 0;					
		}
		else if ( rcond == 2 )				// Regelung nur wenn die Zirkulationspumpe eingeschaltet ist
		{
			if ( PUWZEA[ww]->wert )
			{	
				sp = ( ww == WW1 ) ? SP_WW1 : SP_WW2;
				if ( sp )
					wwd[ww].twwsb = ( wwd[ww].wwlad ) ? wws[ww].Twwlad : wws[ww].Twwsol;
			 	else
					wwd[ww].twwsb = wws[ww].Twwsol;
			
				if ( wwd[ww].legio && ( wws[ww].TwwsolLegio > wwd[ww].twwsb ) )
					wwd[ww].twwsb = wws[ww].TwwsolLegio;	
			
				if ( wwd[ww].regstart == 0 )								// nach Reset
				{
					wwd[ww].fl_tsol	= (float)wwd[ww].twwsb / 10;
					wwd[ww].regstart = 1;		
				}
				else
				{	// Dämpfung des berechneten Sollwertes: twwsb --> gedämpfte tsol		
					wwd[ww].fl_tsol = g_filter(wwd[ww].fl_tsol, wwd[ww].twwsb, wws[ww].Ts, wws[ww].Fzk);
				}
				wwd[ww].tsol 	  = (int)( wwd[ww].fl_tsol * 10 );	// für Anzeige
				regel = TRUE;
			}				
		}
		else
			continue;

	//---------------------------------------------------------------------------------------------
	
		if ( regel == TRUE )
		{	
			//	----------------------------------------------------------------------
			//	Reglertyp: P- / PID-Regler
			//	----------------------------------------------------------------------
			// Test Regelfühler
			if (TWWC[ww]->stat == 0 )		// Status: 0...Ok, 41H...Überlauf,  21H...Unterlauf
			{	
				// Regelabweichung Soll - Ist
				fl_ei	= wwd[ww].fl_tsol - (float)TWWC[ww]->messw / 10;
			
				// -------------- PID-Regelalgorithmus ---------------------------------
				// Berechnung der relativen Stellgrößen
				fl_dy_rel = Dy_rel ( wws[ww].Kp, wws[ww].Kd, wws[ww].Ts, wws[ww].Tn, 
															fl_ei, wwd[ww].fl_ei1, wwd[ww].fl_ei2 );
			
				wwd[ww].fl_y_rel += fl_dy_rel;
			
				//---------------------------------------------------------------------
				// Regelabweichungen merken
				wwd[ww].fl_ei2 = wwd[ww].fl_ei1;
				wwd[ww].fl_ei1 = fl_ei;																		
			
				// Berechnung und Ausgabe der absoluten Stellgrößen
				// --------------------------------------------------------------------
				// Ausgabe an 3-Punkt Ventil
				if ( wws[ww].Vst > 0 )
				{	
					wwd[ww].stellzeit = (int)( fl_dy_rel * (float)wws[ww].Hub * 60 / (float)wws[ww].Vst ); // [0.01 s]
					wwd[ww].stellsum += wwd[ww].stellzeit;
					if(abs(wwd[ww].stellsum) >= 100)									// ab 1 Sekunde ausgeben
					{
						wwd[ww].fahren		= wwd[ww].stellsum / 100;		// nur ganze Sekunden fahren
						wwd[ww].stellsum	= wwd[ww].stellsum % 100; 		// Rest merken
						if(wwd[ww].fahren > 0)		
							RWWAUF[ww]->wert = (char)wwd[ww].fahren;
						else
							RWWZU[ww]->wert  = (char)abs(wwd[ww].fahren);	
					}
				}	
				
				// Ausgabe an 0-10V Ventil
				// Begrenzung der PID-Stellgröße (anti windup) auf Kp * ei (gleitend) oder fest
				wwd[ww].fl_y_rel = anti_windup( wwd[ww].fl_y_rel, wws[ww].Kp, wws[ww].Wup, fl_ei );		// in Funktion.c
				wwd[ww].si_y_rel = (int)(wwd[ww].fl_y_rel * 10);	// debug
	
				// Ausgabe der PID-Stellgröße als Stellsignal an den Systemausgang mit Begrenzung auf 0-100%
				wwd[ww].y_rel = y_stell(wwd[ww].fl_y_rel);				// in Funktion.c
					
				RVENTWW[ww]->awert = wwd[ww].y_rel;							// Ausgabe an Stellventil

				// Parameterumwandlung float to int
				wwd[ww].ei = (int)(fl_ei * 10);					// [0,1%] zur Anzeige
			}
		}
		else
		{
			wwd[ww].fl_ei1	  = 0;
			wwd[ww].ei				= 0;
			RWWAUF[ww]->wert	= 0;
			RWWZU[ww]->wert	= 255;				// Regelventil ständig zufahren
			wwd[ww].y_rel		= 0;
			RVENTWW[ww]->awert = 0;					// Ausgabe an Stellventil
			wwd[ww].fl_ei2		= 0;
			wwd[ww].fl_y_rel = 0;				
		}
	}																			// Ende der for-Schleife		
}

#endif	//if WWANZ	