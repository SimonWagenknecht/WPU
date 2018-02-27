/*	Task:					SteuerSol		*/
/*	Solar-Puffer-Steuerung		*/

#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"


void PuSolLaufz ( char sk );
void PuEin ( char sk );
void PuAus ( char sk );
void GenibusSol ( char sk );

/*---------------------- Task SteuerSol -----------------------------------*/
// Takt: 10 s

void SteuerSol(void)
{	
#if SOLANZ > 0

	static char sk = 0;
	int tmax, tmin;

	if ( ++sk >= SOLMAX )
		sk = 0;
	if ( sk >= SOLANZ )
		return;
		
// Handbetrieb 
	if ( sos[sk].Haut == TRUE )
		PUKOLEA[sk]->wert = sos[sk].Kpea;					// Kollektorpumpe ein/aus

// Automatik
	else
	{	
		sos[sk].Haut = FALSE;											// evtl. Fehleingabe korrigieren
		PuSolLaufz ( sk );	

	
	// Ermitteln der momentanen max. und min. Temperatur über 2 Speicherfühler
	// im Normalfall liefert der obere Fuehler max, der untere min
	// das Programm arbeitet auch korrekt, wenn die Fuehler vertauscht, einer defekt oder nur ein Fuehler existiert
	
		tmax  = tmin = sos[SO1].TPufMax + 1;			// Vorbelegung 
				
		if ( TPUF1L[sk]->stat == 0 && TPUF2L[sk]->stat == 0 )
		{																					// beide Fuehler in Ordnung
			if ( TPUF1L[sk]->messw > TPUF2L[sk]->messw )
			{
				tmax = TPUF1L[sk]->messw;
				tmin = TPUF2L[sk]->messw;			
			}	
			else
			{
				tmax = TPUF2L[sk]->messw;
				tmin = TPUF1L[sk]->messw;			
			}				
		}
		else if ( TPUF1L[sk]->stat == 0 )
			tmax = tmin = TPUF1L[sk]->messw;
		else if ( TPUF2L[sk]->stat == 0 )
			tmax = tmin = TPUF2L[sk]->messw;

	// Pufferladung ein- oder ausschalten ?
		if ( sod[sk].solLad == 0 && TKOLL[sk]->stat == 0						// keine Pufferladung : einschalten, wenn
					&& TKOLL[sk]->messw > sos[sk].TKolBeg 								// die Temp. am Kollektor > SOLAR-BEGINN 
					&& TKOLL[sk]->messw - tmin > sos[sk].TDiffBeg 				// UND Diff.(Koll. - Sp.unt.) > T-DIFF K-PU EIN 
					&& tmax < sos[sk].TPufMax - 20 												// UND Temp. Sp.oben < T-MAX PUFFER - 2 K ( Hyst. )
					&& ( SAMAL[0]->bstat == NICHTV || ( SAMAL[0]->bstat != NICHTV && SAMAL[0]->bwert == 0 ) ) )
			sod[sk].solLad = 1;

		else if ( sod[sk].solLad &&																	// Zustand Pufferladung :	beenden, wenn
						( tmax >= sos[sk].TPufMax														// Fuehler Speicher oben >= T-MAX PUFFER  
					|| TKOLL[sk]->messw < sos[sk].TKolEnd									// ODER die Temp. am Kollektor < SOLAR-ENDE 
					|| TKOLL[sk]->messw - tmin < sos[sk].TDiffEnd					// ODER Diff.( Koll. - Sp.unt. ) < T-DIFF K-PU AUS
					|| ( SAMAL[0]->bstat != NICHTV && SAMAL[0]->bwert )	)	)// ODER	der SPW-Alarm aktiv ist
			sod[sk].solLad = 0;

		sod[sk].solLad ? PuEin ( sk ) : PuAus ( sk );								// Pumpe entspr. Lade-Zustand schalten		

	}

	#if ( BUS_PU_MAX > 0 )
	GenibusSol ( sk );
	#endif
	
#endif
}


/*****************************************************/
/***** Funktion : Pumpenlaufzeit 								 *****/
/***** laeuft alle 10 sek fuer jeden Solarkreis  *****/	 

void PuSolLaufz ( char sk )
{
	static char min;
	
	if ( sk == SO1 && ( ++min >= 6 ) )
		min = 0;
	if ( min == 0 )
	{
		if ( PUKOLEA[sk]->func == STATISCH && PUKOLEA[sk]->wert )	
		{
			if ( ++sod[sk].lzKolPuMin >= 60 )
			{
				sod[sk].lzKolPuMin = 0;
				++sod[sk].lzKolPuH;							// Stundenzaehler inkr.	
				bicbus(EEPADR, (char*)&sod[sk].lzKolPuH, SOLH1_ADR+(sk*2), 2, BICWR);
			}
		}
	}
}				
	

/****************************************/
/***** Funktion : Pumpe einschalten *****/
		
void PuEin ( char sk )		
{
	sod[sk].zpu = (char)( sos[sk].Puna + 1 );	
	PUKOLEA[sk]->wert = 1;								// Pumpenrelais einschalten			
}



/*****************************************************/
/***** Funktion : Pumpe ausschalten mit Nachlauf *****/
/***** laeuft alle 10 sek fuer jeden Solarkreis  *****/	 

void PuAus ( char sk )
{
	static char min;
	
	if ( sk == SO1 && ++min >= 6 )
		min = 0;
	if ( min == 0 )
	{
		if ( sod[sk].zpu )
			sod[sk].zpu--;
		if ( sod[sk].zpu == 0 )	
			PUKOLEA[sk]->wert = 0;						// Pumpenrelais ausschalten
	}
}	


void GenibusSol ( char sk )
{
#if BUS_PU_MAX > 0
	char pu = 0;
	
	if ( sk == SO1 )
		pu = PU_BUS_SO1;
	else if ( sk == SO2 )
		pu = PU_BUS_SO2;
		
	if ( pu > 0 && pu <= BUS_PU_MAX )
	{
		pu -= 1;
		
		if ( BusPuPara[pu].Funktion == 1 )
		{		
			if ( sod[sk].solLad == TRUE || sod[sk].zpu )
			{
				BusPuPara[pu].Betrieb = 1;
//					GeniPara[pu].Sollwert = ??? falls nicht konstant
			}
			else
			{
				BusPuPara[pu].Betrieb = 0;
//					GeniPara[pu].Sollwert = 0; falls nicht konstant
			}
		}

		#if GENI == 1	
		sod[sk].busPuEin = BusPuData[pu].uc_betriebsart;	
		#endif

		#if WILO_MODBUS == 1	
		sod[sk].busPuEin = modWiloPu[pu].output_value1_temp;	
		#endif

		#if GRUNDFOS_MODBUS == 1

		#endif
					
	}
#endif
}
