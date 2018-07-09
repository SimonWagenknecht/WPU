/***** 					Diagnose 						 *****/
/***** speziell fuer FW-R-0112/K-CHO *****/

#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"


/* Task "Diagnose" laeuft all 10 Sekunden */
/* es gibt 6 Programm-Zweige, die minuetlich aufgerufen werden */

void Diagnose ( void )
{
#if DIAG == 1
//	char i, n;
//	static char part = 5;
//	static char neustart = 1;
//	static char leistgBereich_min[4];
//	static char flowBereich_min[4];
//	static char wwlad = 0;
//	static char wwlad_min;
//	static char wwvor = 0;
//	static char wwvor_min;
//	static char geloescht = 0;
//	
//		
//	if ( neustart )
//	{																																			// aktuelle Ergebnisse zurückholen
//		xicbus ( XEEPADR,	(char *)( &did ), DIAG_ADR, sizeof(did), BICRD );	
//		
//		if ( did.tvpMax.tag.tag == 0 || did.tvpMax.tag.monat == 0 )					// Tag oder Monat darf nicht 0 sein (Leittechnik-Problem)
//		{
//			did.tvpMax.tag.tag = 1; did.tvpMax.tag.monat = 1; did.tvpMax.tag.jahr = 0;
//			xicbus ( XEEPADR,	(char *)( &did.tvpMax), TVPMAXDIAG_ADR, sizeof( did.tvpMax ), BICWR );	
//		}						
//		if ( did.tvpMin.tag.tag == 0 || did.tvpMin.tag.monat == 0 )					// Tag oder Monat darf nicht 0 sein (Leittechnik-Problem)
//		{
//			did.tvpMin.tag.tag = 1; did.tvpMin.tag.monat = 1; did.tvpMin.tag.jahr = 0;
//			xicbus ( XEEPADR,	(char *)( &did.tvpMin), TVPMINDIAG_ADR, sizeof( did.tvpMin ), BICWR );	
//		}							
//			
//		neustart = 0;
//		return;		
//	}
//	
//	if ( ++part > 5 )
//		part = 0;
//	
//	// Testen, ob Diagnose-Beginn und -Ende plausible Datums-Angaben sind
//	// und der aktuelle Tag im Bereich
//	// begin.jahr = 0 wird zum Löschen benutzt		
//	if ( 		dis.begin.jahr > 0 && dis.begin.jahr < 99 && dis.end.jahr > 0 && dis.end.jahr < 99	
//			&&  dis.begin.tag > 0 && dis.begin.tag <= 31 && dis.end.tag > 0 && dis.end.tag <= 31 
//			&&	dis.begin.monat > 0 && dis.begin.monat <= 12 && dis.end.monat > 0 && dis.end.monat <= 12 
//			&& ( ( (ULONG)dis.begin.jahr * 10000 + dis.begin.monat * 100 + dis.begin.tag ) <=
//					 ( (ULONG)Jahr * 10000 + Mon * 100 + Tag ) )
//			&& ( ( (ULONG)dis.end.jahr * 10000 + dis.end.monat * 100 + dis.end.tag ) >=
//					 ( (ULONG)Jahr * 10000 + Mon * 100 + Tag ) ) )					 
//	{
//		geloescht = 0;
//		switch ( part )
//		{
//			case 0 :
//			// Maximaler Volumenstrom
//				if ( wmengCtr.flow > did.flow_max )
//				{
//					did.flow_max = wmengCtr.flow;		
//					xicbus ( XEEPADR,	(char *)( &did.flow_max ), FLOWMAXDIAG_ADR, sizeof(did.flow_max), BICWR );
//				}		
//				
//				// Maximale Leistung
//				if ( wmengCtr.wmLeistg > did.wmLeistg_max)
//				{
//					did.wmLeistg_max = wmengCtr.wmLeistg;		
//					xicbus ( XEEPADR,	(char *)( &did.wmLeistg_max ), LEISTMAXDIAG_ADR, sizeof(did.wmLeistg_max), BICWR );
//				}
//				return;
//
//			case 1 :		
//			// Leistungs-Bereiche checken
//				for ( i = 0; i < 4; i++ )
//				{
//					if ( wmengCtr.wmLeistg < dis.leistgBereich[i] )
//					{
//						if ( ++leistgBereich_min[i]	>= 60 )
//						{
//							leistgBereich_min[i] = 0;
//							++did.leistgBereich_h[i];
//							xicbus ( XEEPADR,	(char *)( &did.leistgBereich_h[i]), LEISTDIAG_ADR + 2*i, 2, BICWR );								
//						}
//						break;		
//					}
//				}
//	
//			// Volumenstrom-Bereiche checken
//				for ( i = 0; i < 4; i++ )
//				{
//					if ( wmengCtr.flow < dis.flowBereich[i] )
//					{
//						if ( ++flowBereich_min[i]	>= 60 )
//						{
//							flowBereich_min[i] = 0;
//							++did.flowBereich_h[i];
//							xicbus ( XEEPADR,	(char *)( &did.flowBereich_h[i]), FLOWDIAG_ADR + 2*i, 2, BICWR );								
//						}
//						break;		
//					}
//				}
//				return;	
//
//			case 2:		
//			// Speicher-Ladung
//				if ( wwlad ) 
//				{
//					if ( ++wwlad_min > did.wwlad_time )
//					{
//						did.wwlad_time = wwlad_min;				// max. Dauer einer Speicherladung
//						xicbus ( XEEPADR,	(char *)( &wwlad_min), LADZEITDIAG_ADR, 2, BICWR );
//					}
//				}	
//				else
//					wwlad_min = 0;					
//				
//				if ( wwlad != wwd[WW1].wwlad )			// Zustand hat gewechselt
//				{
//					wwlad = wwd[WW1].wwlad;
//					if ( wwlad )											// Ladung hat gerade begonnen, Anzahl inkrementiern
//					{
//						++did.wwlad_anz;
//						xicbus ( XEEPADR,	(char *)( &did.wwlad_anz), LADANZDIAG_ADR, 2, BICWR );
//					}
//				}	
//				return;
//
//			case 3 :
//			// WW-Vorrang
//				if ( wwvor )
//				{
//					if ( ++wwvor_min > did.wwvor_time )
//					{
//						did.wwvor_time = wwvor_min;				// max. Dauer einer Speicherladung
//						xicbus ( XEEPADR,	(char *)( &wwvor_min), WWVORZEITDIAG_ADR, 2, BICWR );
//					}
//				}	
//				else
//					wwvor_min = 0;					
//				
//				if ( wwvor != wwd[WW1].wwvor )			// Zustand hat gewechselt
//				{
//					wwvor = wwd[WW1].wwvor;
//					if ( wwvor )											// Ladung hat gerade begonnen, Anzahl inkrementiern
//					{
//						++did.wwvor_anz;
//						xicbus ( XEEPADR,	(char *)( &did.wwvor_anz), WWVORANZDIAG_ADR, 2, BICWR );
//					}
//				}	
//				return;	
//
////#####ulsch: 27.08.04  case 4 und 5 geändert: Messwert-Status muss auch ok sein, wenn noch nichts gespeichert ( .jahr = 0 )
//			case 4 :
//			// max. Primaer-Vorlauf	
//				if ( TVP[ANL]->stat == 0 && ( TVP[ANL]->messw > did.tvpMax.temp || did.tvpMax.tag.jahr == 0 ) )	
//				{																													// auch wenn Speicher noch geloescht !
//					did.tvpMax.temp = TVP[ANL]->messw;
//					did.tvpMax.tag.tag = Tag;	did.tvpMax.tag.monat = Mon;	did.tvpMax.tag.jahr = Jahr;	
//					did.tvpMax.ta.stat = ta1m.stat; did.tvpMax.ta.messw = ta1m.messw;	
//					xicbus ( XEEPADR,	(char *)( &did.tvpMax), TVPMAXDIAG_ADR, sizeof( did.tvpMax ), BICWR );							
//				}
//				return;
//				
//			case 5 :
//			// min. Primaer-Vorlauf
//				if ( TVP[ANL]->stat == 0 && ( TVP[ANL]->messw < did.tvpMin.temp || did.tvpMin.tag.jahr == 0 ) )
//				{																													// auch wenn Speicher noch geloescht !
//					did.tvpMin.temp = TVP[ANL]->messw;
//					did.tvpMin.tag.tag = Tag;	did.tvpMin.tag.monat = Mon;	did.tvpMin.tag.jahr = Jahr;	
//					did.tvpMin.ta.stat = ta1m.stat; did.tvpMin.ta.messw = ta1m.messw;
//					xicbus ( XEEPADR,	(char *)( &did.tvpMin), TVPMINDIAG_ADR, sizeof( did.tvpMin ), BICWR );									
//				}
//				return;
//
//		}				
//	}
//	
//	else if ( geloescht == 0 && dis.begin.jahr == 0 )
//	{																			// alles loeschen
//		for ( i = 0, n = sizeof( did ); i < n; i++ )
//			*( (char *)(&did) + i ) = 0;
//		did.tvpMax.tag.tag = 1; did.tvpMax.tag.monat = 1; did.tvpMax.tag.jahr = 0;
//		did.tvpMin.tag.tag = 1; did.tvpMin.tag.monat = 1; did.tvpMin.tag.jahr = 0;
//			
//		xicbus ( XEEPADR,	(char *)( &did ), DIAG_ADR, sizeof(did), BICWR );	
//		wwlad = 0; wwvor = 0;								// damit eine nach Loeschen bereits aktive Ladung auch gezaehlt wird
//		geloescht = 1;
//	}			
#endif



}