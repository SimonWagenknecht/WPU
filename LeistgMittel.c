/***** Berechnung der mittleren Leistung für Wärmemengen-Zaehleingänge *****/
// auch für Gaszähler mit Volumen-Kontakt, mittl.Leistung in Kubikmeter pro Stunde ändern (Parli)

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "wmeng.h"


// Task läuft einmal pro Minute für die Zähleingänge ZIN7, ZIN8, ZIN9, ZIN10
#define Z7			0
#define Z8			1	
#define Z9			2
#define Z10			3

// C-System: Zählwert immer Wh (bzw. Liter)
ULONG Wmeng_kWh ( zaehlsp *pZaehlsp );
void DefInpPtr ( char z, mwsp **ppVl, mwsp **ppRl );

void LeistgMittel ( void )
{
#if LEIST_BER == 1

	maxLeistung *pMaxLeistg;
	zaehlsp *pZaehlsp;	
	char i, n;
	static ULONG long lastWmeng[4] = {0, 0, 0, 0};
	ULONG long aktWmeng;
	static char min = 0xFF;					// taskeigener Minutenzähler wegen Sprüngen der Uhrzeit bei Synchr. durch R50
	mwsp *pVl;
	mwsp *pRl;
		
	if ( min == 0xFF )
		min = Min;										// Anfangs-Initialisierung
	else if ( ++min >= 60 )
		min = 0;		
			
	for ( i = Z7; i <= Z10; i++ )
	{
		if ( ( i == Z7 && LEIST_BER_ZE7 == 1 && ZE7WM == 0 ) || ( i == Z8 && LEIST_BER_ZE8 == 1 && ZE8WM == 0 ) 
			|| ( i == Z9 && LEIST_BER_ZE9 == 1 && ZE9WM == 0 ) || ( i == Z10 && LEIST_BER_ZE10 == 1 && ZE10WM == 0 ) )
		{
	
			pZaehlsp = ZE[i+4];			
			if ( pZaehlsp->zdim.dim_idx < 4 || pZaehlsp->zdim.dim_idx > 6 )
				continue;
			// Anzeige in MWh: Impuls-Wertigkeit max. 1 MWh, Anzeige in kWh: Impuls-Wertigkeit max. 1 kWh
			if ( ( pZaehlsp->zdim.dim_idx == 4 && pZaehlsp->zdim.exponent > 6 )
					|| ( ( pZaehlsp->zdim.dim_idx == 5 || pZaehlsp->zdim.dim_idx == 6 ) && pZaehlsp->zdim.exponent > 3 ) )
				continue;
	
			pMaxLeistg = &MaxLeistung[i]; 	
	// Maxima loeschen
			if ( pMaxLeistg->resMax == 1 )
			{
				pMaxLeistg->mittelMax = 0;
				pMaxLeistg->mittel = 0;
				pMaxLeistg->tag.jahr = 0;
				pMaxLeistg->tag.monat = 1;
				pMaxLeistg->tag.tag = 1;
				pMaxLeistg->zeit.min = 0;
				pMaxLeistg->zeit.std = 0;
				pMaxLeistg->vl.stat = NICHTV;
				pMaxLeistg->vl.messw = 0;	
				pMaxLeistg->rl.stat = NICHTV;
				pMaxLeistg->rl.messw = 0;																			
				pMaxLeistg->ta.stat = NICHTV;
				pMaxLeistg->ta.messw = 0;																			
				bicbus ( EEPADR, (char *)pMaxLeistg, MITT_LEIST + i * SIZE_MITT_LEIST, sizeof( maxLeistung ), BICWR );
				lastWmeng[i] = 0;																						
				pMaxLeistg->resMax = 0;
				continue;	
			}
			else
				pMaxLeistg->resMax = 0;						// Fehleingaben loeschen
	
	// Berechnung der mittleren Leistung
			n = pMaxLeistg->MittelnLeistg;
			if ( n == 0 )
				continue;														// keine Berechnung bei MittelnLeistg = 0
	
		// Test und Korrektur
			if ( n != 60 && n != 30 && n != 20 && n != 15 && n != 10 && n != 5 )		// zugelassene Mittelungen in min 
			{
				n = 60;												// Standardwert
				pMaxLeistg->MittelnLeistg = n;
			}
	
			if ( lastWmeng[i] == 0 )
			{
	//-			lastWmeng[i] = Wmeng_kWh ( pZaehlsp );
				lastWmeng[i] = pZaehlsp->zwert;
				continue;			
			}
	
			if ( ( min % n ) == 0 )													// in dieser Minute ist zu berechnen
			{
	//-			aktWmeng = Wmeng_kWh ( pZaehlsp );			
				aktWmeng = pZaehlsp->zwert;			
				if ( aktWmeng >= lastWmeng[i] )
				{
	//-				pMaxLeistg->mittel = (UINT)( ( aktWmeng - lastWmeng[i] ) * 60 / n );
					pMaxLeistg->mittel = (UINT)( ( aktWmeng - lastWmeng[i] ) * (60/n) / 1000 );		// zwert -> wmeng in Wh, Leistung in kW
					if ( pMaxLeistg->mittel > pMaxLeistg->mittelMax )
					{
						pMaxLeistg->mittelMax = pMaxLeistg->mittel;
						pMaxLeistg->tag.jahr = Jahr;
						pMaxLeistg->tag.monat = Mon;
						pMaxLeistg->tag.tag = Tag;
						pMaxLeistg->zeit.min = Min;
						pMaxLeistg->zeit.std = Std;
						DefInpPtr ( i, &pVl, &pRl );
						if ( pVl != 0 )
						{
							pMaxLeistg->vl.stat = pVl->stat;
							pMaxLeistg->vl.messw = pVl->messw;
						}	
						if ( pRl != 0 )
						{
							pMaxLeistg->rl.stat = pRl->stat;
							pMaxLeistg->rl.messw = pRl->messw;	
						}																		
						pMaxLeistg->ta.stat = ta1m.stat;
						pMaxLeistg->ta.messw = ta1m.messw;																			
						bicbus ( EEPADR, (char *)pMaxLeistg, MITT_LEIST + i * SIZE_MITT_LEIST, sizeof( maxLeistung ), BICWR );					
					}
				}	
				lastWmeng[i] = aktWmeng;
			}
				
			if ( Std == 4 && Min == 1 )
			{
				min = 1;																			// tägliche Synchronisation, kein Problem, wenn es mal nicht klappt
	//-			lastWmeng[i] = Wmeng_kWh ( pZaehlsp );
				lastWmeng[i] = pZaehlsp->zwert;
			}
		}		
	}
	
#endif	
}


#if LEIST_BER == 1

ULONG Wmeng_kWh ( zaehlsp *pZaehlsp )
{
	ULONG lastWmeng = 0;
//-	
//-	if ( pZaehlsp->zdim.dim_idx == 4 )						// MWh			
//-	{
//-		if ( pZaehlsp->zdim.komma  == 0 ) 		
//-			lastWmeng = pZaehlsp->zwert * 1000;		// Anfangs-Init.
//-		else if ( pZaehlsp->zdim.komma  == 1 )
//-			lastWmeng = pZaehlsp->zwert * 100;	
//-		else if ( pZaehlsp->zdim.komma  == 2 )
//-			lastWmeng = pZaehlsp->zwert * 10;	
//-		else 
//-			lastWmeng = pZaehlsp->zwert;
//-	}
//-	else if ( pZaehlsp->zdim.dim == 5 )				// kWh				
//-	{
//-		if ( pZaehlsp->zdim.komma  == 0 ) 		
//-			lastWmeng = pZaehlsp->zwert;					// Anfangs-Init.
//-		else if ( pZaehlsp->zdim.komma  == 1 )
//-			lastWmeng = pZaehlsp->zwert / 10;	
//-		else if ( pZaehlsp->zdim.komma  == 2 )
//-			lastWmeng = pZaehlsp->zwert / 100;	
//-		else 
//-			lastWmeng = pZaehlsp->zwert / 1000;
//-	}
//-	else if ( pZaehlsp->zdim.dim == 6 )				// Kubikmeter				
//-	{
//-		if ( pZaehlsp->zdim.komma  == 0 ) 		
//-			lastWmeng = pZaehlsp->zwert;					// Anfangs-Init.
//-		else if ( pZaehlsp->zdim.komma  == 1 )
//-			lastWmeng = pZaehlsp->zwert / 10;	
//-		else if ( pZaehlsp->zdim.komma  == 2 )
//-			lastWmeng = pZaehlsp->zwert / 100;	
//-		else 
//-			lastWmeng = pZaehlsp->zwert / 1000;
//-	}	
//-	return ( lastWmeng );	
//-
	if ( pZaehlsp->zdim.dim_idx == 4 || pZaehlsp->zdim.dim_idx == 5 )
		lastWmeng = pZaehlsp->zwert / 1000;
	else 	if ( pZaehlsp->zdim.dim_idx == 6 )
		lastWmeng = pZaehlsp->zwert;
	return ( lastWmeng );
}

#endif


void DefInpPtr ( char z, mwsp **ppVl, mwsp **ppRl )
{	
	*ppVl = 0;	
	*ppRl = 0;
	
	if ( z == Z7 )
	{
		*ppVl = ZE7WMVL;
		*ppRl = ZE7WMRL;
	}		
	else if ( z == Z8 )
	{
		*ppVl = ZE8WMVL;
		*ppRl = ZE8WMRL;
	}		
	else if ( z == Z9 )
	{
		*ppVl = ZE9WMVL;
		*ppRl = ZE9WMRL;
	}		
	else if ( z == Z10 )
	{
		*ppVl = ZE10WMVL;	
		*ppRl = ZE10WMRL;
	}		
}
