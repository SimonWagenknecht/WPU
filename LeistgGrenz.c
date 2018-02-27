/***** Berechnung der momentanen Leistung für Wärmemengen-Zaehleingänge *****/
// auch für Gaszähler mit Volumen-Kontakt, mittl.Leistung in Kubikmeter pro Stunde ändern (Parli)

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"


// Task läuft alle 100ms für die Zähleingänge ZIN7, ZIN8, ZIN9, ZIN10
#define Z7			0
#define Z8			1	
#define Z9			2
#define Z10			3

#define BEGR_MITTEL		1
#define BEGR_AKTUELL	2
#define BEGR_MBUS			3

void LeistgGrenz ( void )
{
#if ( LEIST_BEGR == 1 )

	zaehlsp *pZaehlsp;	
	char i;
	static UINT sec10 = 0;											// Zähler für Minuten-Takt
	static UINT leistg = 0;									// berechnete Ist-Leistung
	static char neustart = 3;
	char berechnet = FALSE;
	static float leistBegr;
	static char sec60 = 0;

	if ( ++sec10 >= 100 )
		sec10 = 0;

	for ( i = Z7; i <= Z10; i++ )
	{
		if ( berechnet == TRUE )
			break;
			
		if ( ( i == Z7 && LEIST_BEGR_ZE7 != 1 ) || ( i == Z8 && LEIST_BEGR_ZE8 != 1 ) 
			|| ( i == Z9 && LEIST_BEGR_ZE9 != 1 ) || ( i == Z10 && LEIST_BEGR_ZE10 != 1 ) )
			continue;

 		berechnet = TRUE;
 		
		pZaehlsp = ZE[i+4];			
		
		// Anzeige in MWh: Impuls-Wertigkeit max. 1 MWh, Anzeige in kWh: Impuls-Wertigkeit max. 1 kWh
		if ( ( pZaehlsp->zdim.dim_idx == 4 && pZaehlsp->zdim.exponent > 6 )
				|| ( ( pZaehlsp->zdim.dim_idx == 5 || pZaehlsp->zdim.dim_idx == 6 ) && pZaehlsp->zdim.exponent > 3 ) )
		{
			grenzLeistung.begrenzung = 0;
			continue;
		}

		if ( neustart == 3 )
		{
			grenzLeistung.lastWert = pZaehlsp->zwert;
			grenzLeistung.begrenzung = 0;
			grenzLeistung.leistung = 0;
			grenzLeistung.leistBegr = 0;
			leistBegr = 0;
			neustart = 2;
			continue;
		}
		else if ( neustart ==2 )
		{
			if ( grenzLeistung.lastWert != pZaehlsp->zwert )
			{
				grenzLeistung.lastWert = pZaehlsp->zwert;
				neustart = 1;
			}
			continue;
							
		}	
		else if ( neustart == 1 ) 
		{
			if ( grenzLeistung.lastWert != pZaehlsp->zwert )
			{
				grenzLeistung.msec = 0;	
				grenzLeistung.altMsec = 0;
				grenzLeistung.lastWert = pZaehlsp->zwert;
				neustart = 0;
			}
			continue;
		}		
		
// Momentan-Leistung aus Zeit zwischen 2 Impulsen, Mindestabstand 1 Sekunde !
		if ( grenzLeistung.msec < 36000 )													// < 1 h
		{
			grenzLeistung.msec++;
			if ( grenzLeistung.lastWert < pZaehlsp->zwert )
			{
				if ( grenzLeistung.msec >= 10 )
				{
					leistg = (UINT)( ( ( (ULONG )360 * ( (ULONG)( pZaehlsp->zwert - grenzLeistung.lastWert ) ) / (ULONG)grenzLeistung.msec ) + 5 ) / 10 );// gerundet !
					grenzLeistung.lastWert = pZaehlsp->zwert;
					grenzLeistung.altMsec = grenzLeistung.msec;
					grenzLeistung.msec = 0;
				}
			}
			else		
			{
				if ( grenzLeistung.lastWert == pZaehlsp->zwert )
				{																// vergangene Zeit größer als zuletzt, kein neuer Impuls -> berechnete Leistung wird schrittweise reduziert
					if ( grenzLeistung.altMsec < grenzLeistung.msec )
					{	
						if ( grenzLeistung.altMsec * 5 < grenzLeistung.msec )
							leistg =	0;
						else if ( grenzLeistung.altMsec < grenzLeistung.msec && ( grenzLeistung.msec % grenzLeistung.altMsec == 0 ) )		
																															// Reduzierung beginnt, wenn die verstrichene Zeit doppelt so groß wie die letzte ist
							leistg = (UINT)((ULONG)leistg * grenzLeistung.altMsec / grenzLeistung.msec);
					}
				}
				else														// lastWert > zwert : kann nur ein Fehler sein
				{
					leistg = 0;					
					grenzLeistung.lastWert = pZaehlsp->zwert;
					grenzLeistung.altMsec = grenzLeistung.msec;
					grenzLeistung.msec = 0;
				}			
			}
		}
		else
		{
			leistg = 0;
			if ( grenzLeistung.lastWert != pZaehlsp->zwert )
			{
				grenzLeistung.lastWert = pZaehlsp->zwert;
				grenzLeistung.altMsec = grenzLeistung.msec;
				grenzLeistung.msec = 0;
			}
		}

		grenzLeistung.leistung = leistg;			// für Anzeige

	}
	
	// -------------  ab hier 10-Sekunden-Takt -------------------------------

	 
	if ( sec10 == 0 )
	{
		if ( GrenzLeistung.BegrAkt == BEGR_AKTUELL )
		{
			if ( grenzLeistung.leistBegr == 0 || GrenzLeistung.Daempfung < 2 )
			{
				grenzLeistung.leistBegr = grenzLeistung.leistung;				// Momentanleistung
				leistBegr = (float)grenzLeistung.leistBegr; 
			}
			else
			{
				leistBegr = ( leistBegr * ( GrenzLeistung.Daempfung - 1 ) + (float)grenzLeistung.leistung ) / GrenzLeistung.Daempfung;
				grenzLeistung.leistBegr = (UINT)( leistBegr + 0.5 );
			}
		}	

		else if ( GrenzLeistung.BegrAkt == BEGR_MITTEL )
		{
			if ( LEIST_BEGR_ZE7 == 1 )
				grenzLeistung.leistBegr = MaxLeistung[Z7].mittel;
			else if ( LEIST_BEGR_ZE8 == 1 )
				grenzLeistung.leistBegr = MaxLeistung[Z8].mittel;
			else if ( LEIST_BEGR_ZE9 == 1 )
				grenzLeistung.leistBegr = MaxLeistung[Z9].mittel;
			else if ( LEIST_BEGR_ZE10 == 1 )
				grenzLeistung.leistBegr = MaxLeistung[Z10].mittel;
			else
				grenzLeistung.leistBegr = grenzLeistung.leistung;
		}

#if LEIST_BEGR_MBUS
		else if ( GrenzLeistung.BegrAkt == BEGR_MBUS && MbusPara[0].Freigabe == 1 && MbusPara[0].Typ == WRM )
		{
			grenzLeistung.leistBegr = MbusData[0].leistung / 1000;
		}
#endif
	
		else
		{
			grenzLeistung.leistBegr = 0;
		}
			

		if ( GrenzLeistung.GrenzLeistg > 0 && GrenzLeistung.LeistgHyst > 0 )
		{
			if ( grenzLeistung.leistBegr >= ( GrenzLeistung.GrenzLeistg - GrenzLeistung.LeistgHyst ) )
			{
				if ( grenzLeistung.begrenzung == 0 )
				{
					AnzahlBegrenzung++;
					bicbus ( EEPADR,	(char *)&AnzahlBegrenzung,	LEISTUNG_ANZBEG,	2, BICWR );
				}
				if ( ++sec60 >= 6 )
				{
					sec60 = 0;
					DauerBegrenzung++;
					bicbus ( EEPADR,	(char *)&DauerBegrenzung,	LEISTUNG_DAUBEG,	2, BICWR );	
				}										
			
				grenzLeistung.begrenzung = ( grenzLeistung.leistBegr >= GrenzLeistung.GrenzLeistg ) ? 2 : 1;
			}
			else
				grenzLeistung.begrenzung = 0;
		}
		else
			grenzLeistung.begrenzung = 0;
	}
	
#endif	
}

