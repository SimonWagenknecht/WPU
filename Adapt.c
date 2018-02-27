
/* Task zur Adaption der Heizkurve fuer maximal 4 Heizkreise									*/
/* Task laeuft pro Heizkreis einmal pro Minute									 							*/
/* Adaptionskennwerte von maximal 4 Geraeten pro HK ( R50 ) ueber GBus empfangen 	*/
	// ulsch: praktisch können nicht 4 RIEcon auf den R36 zugreifen, was zum Zeitpunkt der Programmentwicklung nicht klar war 
	// die c-Quelle	wird trotzdem nicht geändert, in projdef.h ist ADAP_STAT_HKn maximal auf 1 zu setzen
	// bei Signalisierung der Adaptionskennwerte über Analogeingänge (bisher nicht implementiert) könnten
	// mehr als 1 Kennwert pro Heizkreis berücksichtigt werden (z.B. Adaptionskennwerte vom WRE100)

#include "struct.h"
#include "ustruct.h"
#include "sramext.h"
#include "standdef.h"
#include "uramext.h"
#include "unoinitext.h"

#include <stdlib.h>
#include <string.h>


const char KeineAdapt[] = { " KEINE ADAPTION" };
const char KeineMeldg[] = { " KEINE MELDUNG " };
const char ZuHoch[] 		= { " ZU HOCH       " };
const char ZuTief[] 		= { " ZU TIEF       " };
const char Normal[] 		= { " NORMAL        " };
const char KeineKommunikation[] = { " KEINE KOMMUNIK" };

#define TIEF 		20
#define NORMAL 	50
#define HOCH 		80

void Adapt ( void )
{
	
#if ( ADAP_STAT_HK1 > 0 || ADAP_STAT_HK2 > 0 || ADAP_STAT_HK3 > 0 || ADAP_STAT_HK4 > 0 )
	
	static char hk = 0xFF;					// Index aktueller Heizkreis
	char i;
	char *pMwAdapt;									// zeigt auf den uebermittelten Wert
	mwsp *pMwAdaption;							// zeigt auf die Kopie im eigenenen Speicher
	mwsp mwAdaptionSum = { 0 };			// Auswertung ( Zusammenfassung ) aller uebertragenen Adaptionswerte	
	char mwAda;
	char anzGeraete = 0;
	int n;
	int adaptNeu;
	char adaIndex;									// Index in Tabelle der Korrekturwerte
	int x1, y1, x2, y2;	

	if ( hk == 0xFF )
	{																// Neustart : Status-Fehler vorbelegen
		for ( n = 0; n < HKANZ; n++ )
		{
			for ( i = 0; i < ADAP_STAT_MAX; i++ )
			{
				mwAdaption[n][i].stat = NICHTV;
				mwAdaption[n][i].messw = 0;
			}				
			strcpy ( &AdaptMeldg[n][0], KeineKommunikation );
		}
	}
	
	if ( ++hk >= HKMAX )
		hk = 0;
	if ( hk >= HKANZ )
		return;

	switch ( hk )
	{
		case 0 :
			anzGeraete = ADAP_STAT_HK1;
			break;
		case 1 :
			anzGeraete = ADAP_STAT_HK2;
			break;
		case 2 :
			anzGeraete = ADAP_STAT_HK3;
			break;
		case 3 :
			anzGeraete = ADAP_STAT_HK4;
			break;	
	}

	if ( anzGeraete > ADAP_STAT_MAX )	
		anzGeraete = ADAP_STAT_MAX;

// Eingabefehler korrigieren
	if ( hks[hk].AdaptWert > 100 )
		hks[hk].AdaptWert = 0;
				
	if ( hks[hk].AdaptWert == 0 || anzGeraete == 0 )
	{																// Adaption unerwuenscht
		hkd[hk].adaptKorr = 0;
		
		for ( i = 0; i < ADAP_STAT_MAX; i++ )
		{
			mwAdaption[hk][i].stat = NICHTV;
			mwAdaption[hk][i].messw = 0;
		}			
		strcpy ( &AdaptMeldg[hk][0], KeineAdapt );
		
		return;
	}												
		
// Eingabefehler korrigieren : Messbeginn kann nur zwischen 5 und 21 Uhr liegen
	if ( hks[hk].MessBeginn < 5 )
		hks[hk].MessBeginn = 5;	
	else if ( hks[hk].MessBeginn > 21 )
		hks[hk].MessBeginn = 21;
		
		
// Berechnung des aktuellen temperaturabhaengigen Korrekturwertes zur Fahrkurve		

	if ( ta1m.stat == 0 && ta1m.messw <= 200 && ta1m.messw > -250 )			// nur zwischen -24,9 und +20,0 grd C
	// Test auf min. -24,9 ist von Bedeutung : bei -25,0 wuerde der naechsttiefere Stuetzpunkt zur Interpolation benoetigt,
	// der aber nicht existiert 
	{
		adaIndex = (char)( ( ta1m.messw + 275 ) / 50 );	
												// d.h. : ta1m = -22.6.. -24.9 => -226..-249 => adaIndex ( Index in Feld der Adaptionswerte ) = 0
	// Interpolation : Temperatur und Korrekturwert des naechstliegenden Stuetzpunkts x1, y1,
	// des zweitnaechsten x2, y2
		x1 = ( (int)adaIndex - 5 ) * 50;						// Stuetzpunkt-Temperatur in Zehntelgrad	
		y1 = AdaptKorr[hk][adaIndex];													// Korrekturwert in Zehntelgrad
		if ( ( ta1m.messw + 275 ) % 50 > 25 )									// Divisionsrest
		{																											// naechsthoeherer Stuetzpunkt
			x2 = ( (int)adaIndex - 4 ) * 50;
			y2 = AdaptKorr[hk][adaIndex+1];
		}				
		else
		{
			x2 = ( (int)adaIndex - 6 ) * 50;
			y2 = AdaptKorr[hk][adaIndex-1];
		}
		
		hkd[hk].adaptKorr = ( ( ( ta1m.messw - x1 ) *	( y2 - y1 ) ) / ( x2 - x1 ) + y1 );	
																	
		if ( hkd[hk].adaptKorr > 100 )
			hkd[hk].adaptKorr = 100;											// Begrenzung auf +-10 K
		else if ( hkd[hk].adaptKorr < -100 )
			hkd[hk].adaptKorr = -100;							
	}		

			
	pMwAdapt = &mwAdapt[hk][0];
	pMwAdaption = &mwAdaption[hk][0];

// uebertragene Adaptions-Kennwerte in den eigenen Speicherbereich kopieren, wenn sie gueltig sind
	if ( ( ADAPT[hk]->stat != 0 || ADAPT[hk]->messw < 100 ) && ( ADAPT_T[hk]->bstat != 0 || ADAPT_H[hk]->bstat != 0 ) )
												// ana.Eingang NICHTV oder offen (bzw. gestört) und dig.Eingänge NICHTV
	{	
		// kein Messwert projektiert -> Adaptions-Kennwerte vom R50
		for ( i = 0; i < anzGeraete; i++ )
		{
			mwAda = *(pMwAdapt+i);
			if ( mwAda == 2 || mwAda == 5 || mwAda == 8 || mwAda == 0 )		// ueber WR_ADAKENNZ kann 0 empfangen werden (R100WRE)
			{
				(pMwAdaption+i)->messw = mwAda * 10;			// messwertlike Darstellung			
				(pMwAdaption+i)->stat = 0;
			}
		}		
	
	// Test der Datenuebetragung
		for ( i = 0; i < anzGeraete; i++ )
		{
			if ( adapt_countdown[hk] == 1 )
			{
				(pMwAdaption+i)->messw = 0;
				(pMwAdaption+i)->stat = DATERR;		// Messwertfehler
				*(pMwAdapt+i) = 0;
			}
			else if ( adapt_countdown[hk] == 0 )
			{
				(pMwAdaption+i)->messw = 0;
				(pMwAdaption+i)->stat = NICHTV;		// Messwertfehler
				*(pMwAdapt+i) = 0;
			}		
		}
	}
	else if ( ADAPT[hk]->stat == 0 && ADAPT[hk]->messw >= 100 )
	{
		// Messwert projektiert -> Adaptions-Kennwerte vom 0..10V-Eingang (nur ein Eingang pro HK vorgesehen)	
		*pMwAdapt = 0;								// Buswert löschen	
		pMwAdaption->stat =	0;

		if ( ADAPT[hk]->messw >= 100 && ADAPT[hk]->messw <= 300 )
			pMwAdaption->messw = TIEF;
		else if ( ADAPT[hk]->messw >= 400 && ADAPT[hk]->messw <= 600 )
			pMwAdaption->messw = NORMAL;			
		else if ( ADAPT[hk]->messw >= 700 && ADAPT[hk]->messw <= 900 )
			pMwAdaption->messw = HOCH;
		else
		{
			pMwAdaption->stat = NICHTV;
			pMwAdaption->messw = 0;	
		}		
	}
	else if ( ADAPT_T[hk]->bstat == 0 || ADAPT_H[hk]->bstat == 0 )
	{
		// dig.Eingänge projektiert (nur ein Eingang pro HK vorgesehen)	
		*pMwAdapt = 0;								// Buswert löschen	
		pMwAdaption->stat =	0;

		if ( ADAPT_T[hk]->bwert == TRUE && ADAPT_H[hk]->bwert == FALSE )
			pMwAdaption->messw = TIEF;
		else if ( ADAPT_T[hk]->bwert == FALSE && ADAPT_H[hk]->bwert == TRUE )
			pMwAdaption->messw = HOCH;
		else
			pMwAdaption->messw = NORMAL;			
	}
	else
	{
		pMwAdaption->stat = NICHTV;
		pMwAdaption->messw = 0;	
	}		

// Zusammenfassung der Adaptionswerte (vom RIEcon50 kommt nur einer)
// Voreinstellungen : zu hoch, Uebertragungsfehler
// Ergebnis : zu hoch nur wenn alle zu hoch, zu tief wenn einer zu tief
	mwAdaptionSum.messw = HOCH;
	mwAdaptionSum.stat = 1;
	for ( i = 0; i < anzGeraete; i++ )
	{
		mwAdaptionSum.stat &=	(pMwAdaption+i)->stat;	// Ergebnis ist 0, wenn ein Status = 0 ( Uebertragung i.O.)
		if ( (pMwAdaption+i)->messw == TIEF )
			mwAdaptionSum.messw = TIEF;
		else if ( (pMwAdaption+i)->messw == NORMAL && mwAdaptionSum.messw != TIEF )
			mwAdaptionSum.messw = NORMAL;	
		else if ( (pMwAdaption+i)->messw == 0 && mwAdaptionSum.messw != TIEF && mwAdaptionSum.messw != NORMAL )
			mwAdaptionSum.messw = 0;	
			
	}

	if ( mwAdaptionSum.stat )
	{
		mwAdaptionSum.messw = 0;			// falls alle Werte mit Fehler markiert waren
		strcpy ( &AdaptMeldg[hk][0], KeineKommunikation );
	}
		
	else
	{	
		switch ( mwAdaptionSum.messw )
		{
			case TIEF:
				strcpy ( &AdaptMeldg[hk][0], ZuTief );
				break;
			case NORMAL:
				strcpy ( &AdaptMeldg[hk][0], Normal );
				break;
			case HOCH:
				strcpy ( &AdaptMeldg[hk][0], ZuHoch );
				break;
			default:
				strcpy ( &AdaptMeldg[hk][0], KeineMeldg );
				break;									
		}
	}

// Regler im Sommerbetrieb : keine Berechnung
	if ( hkd[hk].somme == TRUE )
		return;	
		
// in der Stunde nach Messbeginn wird die Vorlauftemperatur ueberwacht
// Adaption erfolgt nur, wenn die Vorlauftemperatur der ( adaptionskorrigierten ) Heizkurve entspricht	
	if ( Std == hks[hk].MessBeginn )
	{
		if ( Min == 0 )								// in der ersten Minute werden die Zaehler geloescht
		{
			tvsHigh[hk] = 0;
			tvsLow[hk] = 0;	
			tvsNormal[hk] = 0;					
		}	
			
		// Voraussetzung für Adaption : Bedarf, keine Absenkung durch WW-Vorrang 			
		if ( ( hkd[hk].bedarf == BEDARF_JA || ( hkd[hk].bedarf == BEDARF_UNBEKANNT && abs_ram[hk].absen == 0 ) ) 
											&& hkd[hk].vorra == FALSE && hkd[hk].wwvorKorr == 0 )
		{
			if ( TVS[hk]->stat != NICHTV )		// Fühlerfehler wird nicht speziell untersucht 
			{
				if ( TVS[hk]->messw - hkd[hk].tsol > hks[hk].EiMaxAdapt )
					++tvsHigh[hk];
				else if ( hkd[hk].tsol - TVS[hk]->messw > hks[hk].EiMaxAdapt )
					++tvsLow[hk];
				else
					++tvsNormal[hk];
			}
			else															// Fühler nicht projektiert, aber Adaption gewollt: so tun als wäre alles ok 					
				++tvsNormal[hk];
		}
	}	
	
// beginnend eine Stunde nach Messbeginn werden die von den R50	uebertragenen Werte eine Stunde lang
// auf ihre Konstanz ueberwacht. Adaptiert wird, wenn keine dauerhafte Abweichung der Vorlauftemperatur erkannt wurde
// oder bei Vorlauf zu hoch und Unterversorgung oder Vorlauf zu tief  und Überversorgung 
// und nur wenn die Adaptionswerte der R50 ueber diese Stunde konstant bleiben
	if ( Std == ( hks[hk].MessBeginn + 1 ) )
	{
		if ( Min == 0 )								// in der ersten Minute werden die Zaehler geloescht
		{
			leistgHigh[hk] = 0;
			leistgLow[hk] = 0;			
		}
		
		// Voraussetzung für Adaption : Bedarf, keine Absenkung durch WW-Vorrang 	
		if ( ( hkd[hk].bedarf == BEDARF_JA || ( hkd[hk].bedarf == BEDARF_UNBEKANNT && abs_ram[hk].absen == 0 ) )		
		 			&& mwAdaptionSum.stat == 0 && hkd[hk].vorra == FALSE && hkd[hk].wwvorKorr == 0 )
		{
			if ( mwAdaptionSum.messw == TIEF )					// Unterversorgung
				++leistgLow[hk];
			else if ( mwAdaptionSum.messw == HOCH )			// Überversorgung
				++leistgHigh[hk];
		}
	}	

// Berechnung des Adaptionswertes
// einmal taeglich, zwei Stunden nach Messbeginn
	if ( Std == ( hks[hk].MessBeginn + 2 ) && Min == 0 )
	{
		if ( ( tvsHigh[hk] + tvsLow[hk] + tvsNormal[hk] ) < 59 )		// eigentlich sollte 60 rauskommen
			return;
		if ( tvsHigh[hk] >= 5 && tvsLow[hk] >= 5 )
			return;																										// starke Vorlaufschwankungen			
		
		// Berechnung erfolgt, wenn überwiegend Unterversorgung bei überwiegend normaler oder zu hoher Vorlauftemperatur
		// bzw.wenn überwiegend Überversorgung  bei überwiegend normaler oder zu tiefer Vorlauftemperatur war
		if ( ( leistgLow[hk] > 55 && tvsLow[hk] < 5 ) || ( leistgHigh[hk] > 55 && tvsHigh[hk] < 5 ) )
		{																								// wenn zu hoch oder zu tief
			if ( ta1m.stat == 0 && ta1m.messw <= 200 && ta1m.messw > -250 )			// nur zwischen -24,9 und +20,0 grd C
					// Test auf min. -24,9 ist von Bedeutung : bei -25,0 wuerde der naechsttiefere Stuetzpunkt 
					// zur Interpolation benoetigt, der aber nicht existiert 
			{
				if ( bicbus ( EEPADR, (char *)( AdaptKorr[hk] ),	ADAPT_KORR_ADR+(hk*ADAPT_KORR_LNG), 
																					sizeof(AdaptKorr[0]), BICRD ) == 0 )				// vorsichtshalber neu einlesen
					if ( bicbus ( EEPADR, (char *)( AdaptKorr[hk] ),	ADAPT_KORR_ADR+(hk*ADAPT_KORR_LNG), 
																					sizeof(AdaptKorr[0]), BICRD ) == 0 )				// noch ein Versuch
						return;																																		// so wird das nix		
				
				adaIndex = (char)( ( ta1m.messw + 275 ) / 50 );	
					// d.h. : ta1m = -22.6.. -25.0 => -226..-250 => adaIndex ( Index in Feld der Adaptionswerte ) = 0
				if ( leistgLow[hk] > 55 )
				{
					adaptNeu = ( AdaptKorr[hk][adaIndex] + hks[hk].AdaptWert );
					if ( adaptNeu > 100 )																				// Adaption auf +-10K begrenzt 
						adaptNeu = 100;
					AdaptKorr[hk][adaIndex] = adaptNeu;
					if ( ( ta1m.messw + 275 ) % 50 > 25 )												// Divisionsrest
						++adaIndex;																								// naechsthoeherer Stuetzpunkt	
					else
						--adaIndex;	
					adaptNeu = ( AdaptKorr[hk][adaIndex] + hks[hk].AdaptWert / 2 );	// halber Adaptionswert
					if ( adaptNeu > 100 )																				// Adaption auf +-10K begrenzt 
						adaptNeu = 100;
					AdaptKorr[hk][adaIndex] = adaptNeu;															
				}
				else if ( leistgHigh[hk] > 55 )
				{
					adaptNeu = ( AdaptKorr[hk][adaIndex] - hks[hk].AdaptWert );
					if ( adaptNeu < -100 )																			// Adaption auf +-10K begrenzt 
						adaptNeu = -100;
					AdaptKorr[hk][adaIndex] = adaptNeu;
					if ( ( ta1m.messw + 275 ) % 50 > 25 )												// Divisionsrest
						++adaIndex;																								// naechsthoeherer Stuetzpunkt	
					else
						--adaIndex;	
					adaptNeu = ( AdaptKorr[hk][adaIndex] - hks[hk].AdaptWert / 2 );	// halber Adaptionswert
					if ( adaptNeu < -100 )																			// Adaption auf +-10K begrenzt 
						adaptNeu = -100;
					AdaptKorr[hk][adaIndex] = adaptNeu;															
				}	
								
				if ( bicbus ( EEPADR, (char *)( AdaptKorr[hk] ),	ADAPT_KORR_ADR+(hk*ADAPT_KORR_LNG), 
																						sizeof(AdaptKorr[0]), BICWR ) == 0 )		// Aenderungen zurueckschreiben
					bicbus ( EEPADR, (char *)( AdaptKorr[hk] ),	ADAPT_KORR_ADR+(hk*ADAPT_KORR_LNG), 
																						sizeof(AdaptKorr[0]), BICWR ); 					// noch ein Versuch				

			}
		}
	}
	
#endif		// #if  ( ADAP_STAT_HK1 > 0 || ADAP_STAT_HK2 > 0 || ADAP_STAT_HK3 > 0 || ADAP_STAT_HK4 > 0 )
	
}
