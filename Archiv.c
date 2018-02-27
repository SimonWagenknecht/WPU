/* 	Task : Archivierung	  */
/* Achtung : xicbus() geaendert */

#include <stdlib.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "userdef.h"
#include "konvform.h"
#include "unoinitext.h"
#include "constext.h"

#if ARCHIV_ANL == 1

/*---------------------- Task Archivierung -----------------------------------*/
/* 					Aufruf erfolgt im Sekundenrhythmus  												      */


ULONG MinuteImJahr ( char tag, char mon, char std, char min, char jhr );
void NewPageFillBuffer ( char *buffer, ULONG data, char format );
UINT ReadPara ( char *buffer, char anzahl );
void ParameterReparieren ( void );
char ParameterPruefen ( void );


#define MAXARCH		15				// 15 Datenelemente maximal ins Archiv

#define INIT 			0x55
#define DEFPAGES	0x56
#define PREPARE		0x57
#define STORE			0x58

#define MAXTRENDS 1					// Datenelemente, die mit Intervallen < LONGINTV Minuten
#define LONGINTV	1					// archiviert werden, muss mind. 1 sein !
#define TREND_NR	32				// ab Byte 32 auf Page 2 : 
														// Indizees in den Zwischenspeicher ( DataBuffer ) fuer o.g. DaE


const char dayPerMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

char PageBuffer[64];
//char DataBuffer[MAXTRENDS][64];
char DataBuffer[MAXTRENDS][4];	// nur wenn LONGINTV = 1 ( keine Zwischenspeicherung der aktuellen Page im RAM )
ldiv_t ergLDiv;

// Struktur der Parameter im EEPROM auf Pages 0 ( ab Adresse 4 ) und 1
struct archPara {
	UINT adr;									// Komtab-Adresse					
	UINT intervall;						// in Minuten, restlos durch 60 teilbar oder Vielfache von 60
	UINT firstPage;
	char anzPages;
	char days;								// Aufzeichnungsdauer, Anzahl Tage
	};

void Archiv ( void )
{
	char i = 0;
	char n, ret_komma;
	char kennung;
	char anzahl;
	static char minAlt;
	static UINT pageToClear;
	struct archPara *pArchPara;
	UINT page;
	char gnum, pnum;
	static UINT resCnt;
//	long lData = 0;
	ULONG lData = 0;
	char format; 	
	char konv = 0;
	char attr = 0;
	char alclass, alfunc; 
	char *pAdrData;
	static char daeNr;							// das n-te Datenelement entsprechend Parameter-Pages
	static char daeTrend;						// das n-te Trend-Datenelement ( Intervall-Parameter < 60 )
	mwsp *pAnaWert;
	aasp *pAoutWert;								// *****josch: Analogausgang
	UINT ergDiv;
	ULONG min1, min2;
	static char wrPtr;
	static UINT PtrBuffer[MAXARCH];
	static char subStatus; 
	static char subStat1;
	static char TrendNr[MAXARCH];
	zaehlsp *pZAEHLSP;
	
//	lData = konv = attr = i = 0;

	if ( xicbus ( XEEPADR, ParaBuffer, 0x0000, 4, BICRD ) == 0 )
		return;															// Antwort fehlerhaft

// 1003
	if ( ( ParaBuffer[0] == 0 && ParaBuffer[1] == 0 ) || ( ParaBuffer[0] == 0x66 && ParaBuffer[1] == 0x99 ) )
	{
		archivAktiv = 0;		
		anzahl = 0; 	
		return;	
	}											// Archiv nicht initialisiert oder reparieren nicht möglich
	else if ( ParaBuffer[0] < INIT || ParaBuffer[0] > STORE || ( ParaBuffer[1] != ( (char)(~INIT) ) )
				|| ( ParaBuffer[2] != (char)(~ParaBuffer[3] ) ) || ParaBuffer[2] == 0 || ParaBuffer[2] > MAXARCH )
	// Parameter vermutlich zerstört
	{
		ParameterReparieren ( );
		return;		
	}
			
	kennung = ParaBuffer[0];
	archivAktiv = ( kennung == 0x58 && ParaBuffer[1] == 0xAA ) ? 1 : 0;
	anzahl = ParaBuffer[2];

	switch ( kennung )
	{
// Archivierung ist neu parametriert worden
		case INIT :	
			PageBuffer[0] = DEFPAGES;			
			xicbus ( XEEPADR, PageBuffer, 0x0000, 1, BICWR );
			subStatus = 0;
			arch_par = anzahl;								// kann spaeter eingeschraenkt werden
	 		bicbus(EEPADR,	&arch_par, ARCH_PAR_ADR, 1, BICWR);
//#####ulsch:Timer2	 
//	 		ms_delay(5); 			
			break;	


		case DEFPAGES :
			switch ( subStatus )
			{
				case 0 :
					if ( ReadPara ( ParaBuffer,  anzahl ) )
					{
						subStatus = 1;
						subStat1 = 0;
					}			
					break;
					
				case 1 :
					switch ( subStat1 )
					{
						case 0 :
							pArchPara = (struct archPara *)( ParaBuffer + 4 );
							page = 3;															// erste Page der Archivdaten				
							for ( i = 0; i < anzahl; i++, pArchPara++ )
							{
								gnum = KomTab[pArchPara->adr].gnum;
								pnum = KomTab[pArchPara->adr].pnum;
								if ( ( pArchPara->adr > 0 ) && ( gnum == 0 ) && ( pnum == 0 ) )
									format = 0;												// Datenelement existiert nicht
								else
								{																 
									pAdrData = get_parli_info ( gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
						// ausser bei ANA_FORM liefert die Funktion einen Zeiger auf einen Zeiger auf eine Struktur !	
									switch ( konv )
									{
										case ANA_FORM :
										case ANA_FORMP :
										case US_INT :
//									case US_INTP :		*****josch: wird ersetzt durch AOUT_FORMP
										case AOUT_FORMP:
										case S_INT :
											format = 2;
											break;
																	
										case ZE_FORMP :
											format =  attr ? 0 : 4;				// nur Absolutwert ist implementiert 
											break;

										case US_LONG :
											format = 4;
											break;
											
										case JANEIN_FORM :
										case JANEIN_FORMOP :
										case JANEIN_FORMIP :
										case US_CHAR :
										case S_CHAR :																
											format = 1;
											break;
		
										default :
											format = 0;				// Format 0 kennzeichnet Fehler in der Parametrierung								
											break;						// es wird nichts aufgezeichnet
									}
								}
								
								if ( format != 0 )
								{
// ***** 07.11.02									
/*									pArchPara->firstPage = ( i == 0 ) ? 3 :	
																					(pArchPara-1)->firstPage + (pArchPara-1)->anzPages;
*/
									pArchPara->firstPage = page;
		// Anzahl Pages ergibt sich aus 1440 (Minuten / Tag) / Intervall (Minuten) * Anzahl Tage
		// * Format (1..4 Bytes) / 60 (Bytes pro Page) + 1 ( oder 2 )
// ***** 07.11.02		
									ergDiv	= (1440 / 60) * pArchPara->days	* format;	
/***** 20.11.02									pArchPara->anzPages = ( ergDiv % pArchPara->intervall ) ? 
															(char)( ergDiv / pArchPara->intervall + 2 ) :
												 			(char)( ergDiv / pArchPara->intervall + 1 );
*****/
									pArchPara->anzPages = (char)( ergDiv / pArchPara->intervall + 2 );			 			
									if ( ergDiv % pArchPara->intervall == 0 )
										--pArchPara->anzPages;		// immer mindestens eine Page zusaetzlich zuweisen										
												 			
									page += pArchPara->anzPages;		// naechste Firstpage																																					
								}
							}
							
							if ( ( n = (char)( anzahl * sizeof( struct archPara ) ) ) > 60 )
								n = 60;								// Anzahl zu schreibender Bytes					
							if ( xicbus ( XEEPADR, ParaBuffer+4, 0x0004,	n, BICWR ) )																
								subStat1 = ( anzahl > 7 ) ? 1 : 2;		
										
							break;
							
						case 1 :
							if ( anzahl > 7 )				// muss eigentlich immer wahr sein
							{
								n = (char)( ( anzahl - 7 ) * sizeof(struct archPara ) - 4 );
								if ( xicbus ( XEEPADR, ParaBuffer + 0x40, 0x0040,	n, BICWR ) )																
									subStat1 = 2;
							}
							else
								subStat1 = 2;						
						
							break;
							
						case 2 :							// Loeschen der benutzten Pages vorbereiten
							pageToClear = 2;		// Pages 0.1 werden nicht geloescht ( Parameter )
							PageBuffer[0] = PREPARE;			
							if ( xicbus ( XEEPADR, PageBuffer, 0x0000, 1, BICWR )	)
								subStatus = 0;				
							break;
						
					}					// switch ( subStat1 )
					break;		// case 1
			}							// switch ( subStatus )

			break;
					

		case PREPARE :	
// nach Parametrierung Aufzeichnung vorbereiten :
// alle Bytes aller benutzten Seiten werden mit 0 vorbelegt, 
// bei Beginn des Schreibens auf eine Page wird diese mit 0x88 beschrieben 
// ( 0x88888888 als Zaehlwert = 2290649224 und 0x8888 = -3058,4 °C als Messwert
// werden damit ausgeschlossen, dient beim Auslesen als Endekennung )
// es wird bei jedem Task-Lauf nur eine Page beschrieben ! 
// bei Fehlern bei der Datenerfassung wird das letzte Nibble des entsprechende Wertes mit 9
// ueberschrieben - Fehlermarkierung fuer die Auswertung

			switch ( subStatus )
			{
				case 0 :
					if ( ReadPara ( ParaBuffer,  anzahl ) )
						subStatus = 1;			
					break;
					
				case 1 :
					if ( pageToClear == 2 )
					{
// Page 2 ( Pointer-Page ) wird mit den FirstPages vorbelegt
// ab Byte TREND_NR stehen die Indizees in den Zwischenspeicher DataBuffer, 
// 0xFF wenn dieser nicht benutzt wird ( wird nur bei kleinen Intervallen der Trends verwendet )
						for ( i = 0; i < 64; i++ )
							PageBuffer[i] = 0xFF;																		
						pArchPara = (struct archPara *)( ParaBuffer + 4 );
						n = 0;
						for ( daeNr = 0; daeNr < anzahl; daeNr++, pArchPara++ )
						{
							*((UINT *)PageBuffer+daeNr) = pArchPara->firstPage;
							if ( ( pArchPara->intervall < LONGINTV ) && ( n < MAXTRENDS ) )
								PageBuffer[TREND_NR+daeNr] = n++;
						}
						if ( xicbus ( XEEPADR, (char *)PageBuffer, 0x0080,	64, BICWR ) )	
						{	
							pageToClear = 3;
							for ( i = 0; i < 64; i++ )
								PageBuffer[i] = 0;			// Vorbereitung fuer Loeschen ( ..else if )
						}				
					}
					
					else if ( pageToClear > 2 && pageToClear < 512 )
					{
/***** 07.11.02						
						pArchPara = (struct archPara *)( ParaBuffer + 4 +	
																						(anzahl-1)*sizeof(struct archPara )	);
*/
// bei der Bestimmung der letzten benutzten Seite werden nun fehlerhafte Parametrierungen
// ( anzPages = 0 ) beruecksichtigt
						pArchPara = (struct archPara *)( ParaBuffer + 4 );
						for ( i = 0, n = 0xFF; i < anzahl; i++ )
							if ( (pArchPara+i)->anzPages )
								n = i;
						if ( n < 0xFF )							// mind. ein korrekter Parameter																									
							page = (pArchPara+n)->firstPage	+ (pArchPara+n)->anzPages - 1; // allerletzte Seite
						else
							page = 0;

						if ( pageToClear <= page )
						{			
							if ( xicbus ( XEEPADR, PageBuffer, pageToClear << 6, 64, BICWR ) )
								++pageToClear;
						}
						else											// alle benutzten Pages sind geloescht
						{
							pageToClear = 512;						// beim naechsten Mal wird dieser case gecancelt
		
// Loeschen des benutzten Teils des Zwischenspeichers DataBuffer
// dorthin werden Trend-Daten ( Intervall < LONGINTV Minuten ) geschrieben und erst in den
// EEPROM uebernommen, wenn eine Page voll ist 
							for ( daeNr = 0, n = 0; daeNr < anzahl; daeNr++ )
							{
					pArchPara = (struct archPara *)( &ParaBuffer[4 + sizeof(struct archPara) * daeNr] );		
//					pArchPara = (struct archPara *)( ParaBuffer+(4 + sizeof(struct archPara) * daeNr) );		
								if ( pArchPara->intervall < LONGINTV && ( 60 % pArchPara->intervall == 0 ) )
								{
									for ( i = 0; i < 64; i++ )
										DataBuffer[n][i] = 0;
										
// 20.11.02									if ( n < ( MAXTRENDS - 1 ) )
									if ( ( n + 1 ) < MAXTRENDS )
										++n;
								}								
							}
							daeTrend = MAXTRENDS;		// Durchnumerierung der Trend-Datenelemente ( Intervall < LONGINTV min )
						}													
					}
							
					else
					{
						ParaBuffer[0] = STORE;
						if ( xicbus ( XEEPADR, ParaBuffer, 0x0000, 1, BICWR ) )
						{			
// 1003							subStatus = 0;			
//							daeNr = 0;
							n = (char)( anzahl * sizeof( struct archPara) ) + 4;
							ParaBuffer[n] = 0;
							for ( i = 0; i < n; i++ )
								ParaBuffer[n] += ParaBuffer[i];
							
							if ( xicbus ( XEEPADR, &ParaBuffer[n], (UINT)n, 1, BICWR )	)
							{
								bicbus ( EEPADR,	ParaBuffer, ARCH_PAR_FERRO, 0x40, BICWR);
								if ( anzahl > 7 )
									bicbus ( EEPADR,	ParaBuffer+0x40, ARCH_PAR_FERRO+0x40, 0x40, BICWR);
								subStatus = 0;
								daeNr = 0;														
							}

						}
					}
					break;
			}
					
			break;			
				
// Daten aufzeichnen			
		case STORE :
			if ( arch_par > anzahl )
				arch_par = anzahl;
			if ( arch_par < anzahl )
				anzahl = arch_par;							// nicht alle parametrierten DaE werden aufgezeichnet		

			if ( Resetcnt != resCnt )					// falls gerade archiviert wird, wird dieser Vorgang 
			{																	// nach einemReset abgebrochen. Die Page-Pointer werden 
				daeNr = (char)( anzahl + 1 );		// NICHT auf den EEPROM zurueckgeschrieben															
				if ( ReadPara ( ParaBuffer,  anzahl ) )
					resCnt = Resetcnt;						
				break;
			}
			
			if ( minAlt == Min )
			{
				if  ( daeNr > anzahl )
					break;												// fuer diese Minute sind alle DaE behandelt
																				// => warten auf Minutenwechsel
				else if ( daeNr == anzahl ) 
				{
					if ( wrPtr )
						if ( xicbus ( XEEPADR, (char *)PtrBuffer, 0x0080, (char)( 2*anzahl ), BICWR ) )
						{
							wrPtr = 0;								// Kennung "Page-Pointer in EEPROM schreiben" loeschen
							++daeNr;
						}
					break;
				}
			}
																						
			else												// Minute hat gewechselt
			{
				minAlt = Min;
				daeNr = 0;
				daeTrend = MAXTRENDS;
				subStatus = 0;
			}

			switch ( subStatus )
			{
				case 0 :									// Parameter lesen
					pArchPara = (struct archPara *)( ParaBuffer + 4 );
					if ( minAlt != 0 )
						for ( i = 0; i < anzahl; i++ )
							if ( minAlt % (pArchPara+i)->intervall == 0 )
								break;

					if ( minAlt == 0 || i < anzahl )
					{												// in der akt.Minute ist irgendetwas zu archivieren
						if ( ( n = (char)( anzahl * sizeof(struct archPara ) ) ) > 60 )
							n = 60;							// Anzahl zu lesender Bytes
						if ( xicbus ( XEEPADR, ParaBuffer+4, 0x0004, n, BICRD )	)
							subStatus = ( anzahl > 7 ) ? 1 : 2;
					}
					else
						subStatus = 0xFF;			// sonst jede Sekunde case 0 !
						
					break;
				
				case 1 :									// Parameter benoetigen zwei Pages
					n = (char)( ( anzahl - 7 ) * sizeof(struct archPara ) - 4 );					
					if ( xicbus ( XEEPADR, ParaBuffer+64, 0x0040, n, BICRD ) )
						subStatus = 2;				
					break;
					
				case 2 :									// Page-Pointer und Trend-Nr lesen ( alles auf Page 2 )
// 1003 Parameter prüfen
					if ( ParameterPruefen ( ) == FALSE )
					{
						ParaBuffer[1]	= 0x88;
						xicbus ( XEEPADR, ParaBuffer, 0x0000, 2, BICWR );
						return;											// Reparieren beim nächsten Start der Task ( 1 sek )
					}
					
					if ( xicbus ( XEEPADR, (char *)PtrBuffer, 0x0080, (char)( 2*anzahl ), BICRD )	&&
							 xicbus ( XEEPADR, TrendNr, 0x0080+TREND_NR, anzahl, BICRD ) )
					{
						subStatus = 3;					
						daeNr = 0xFF;					// wird beim naechsten Minutenwechsel zu 0 !
					}
					
					break;
					
				case 3 :									// Daten archivieren ( nur zur parametrierten Zeit )
					pArchPara = (struct archPara *)( &ParaBuffer[4 + sizeof(struct archPara) * daeNr] );		
//					pArchPara = (struct archPara *)( ParaBuffer+(4 + sizeof(struct archPara) * daeNr) );		
					if ( pArchPara->intervall < 60 )
					{
						if ( 60 % pArchPara->intervall )				
							break;												// Teilbarkeit ist Bedingung
						if ( minAlt != 0 && ( minAlt % pArchPara->intervall ) )
							break;												// nicht in dieser Minute
						if ( pArchPara->firstPage < 3 )
							break;
					}
					else if ( minAlt != 0 || ( pArchPara->intervall % 60 ) )	
						break;			// Stunden-Intervall : Parameter ist Vielfaches von 60, immer zur Minute 0 

					min2 = MinuteImJahr ( Tag, Mon, Std, minAlt, Jahr );
					
					gnum = KomTab[pArchPara->adr].gnum;
					pnum = KomTab[pArchPara->adr].pnum;
					if ( ( pArchPara->adr > 0 ) && ( gnum == 0 ) && ( pnum == 0 ) )
						format = 0;											// Datenelement existiert nicht
					else
					{																 
						pAdrData = get_parli_info ( gnum, pnum, &konv, &attr, &alclass, &alfunc, 0);
						// bei den auf P endenden cases liefert die Funktion 
						// einen Zeiger auf einen Zeiger auf eine Struktur !	
						switch ( konv )
						{
							case US_CHAR :
							case S_CHAR :
								format = 1;
								lData = *pAdrData;								
								break;
							
							case ANA_FORM :
							case US_INT :
							case S_INT :
								format = 2;
								lData = *(int *)pAdrData;						
								break;
								
/*****josch		case US_INTP :				wird ersetzt durch AOUT_FORMP
								format = 2;
//								lData = *( (int *)( *( (int *)pAdrData) ) ); entspricht :
								lData = **( (UINT** )pAdrData);
								break;										
*/														
							case AOUT_FORMP :		// *****josch: Analogausgang
								format = 2;
								pAoutWert =(aasp *)(*(long *)pAdrData);
								lData = pAoutWert->awert;
								break;										

							case ANA_FORMP :
								format = 2;
								pAnaWert = (mwsp *)(*(long *)pAdrData);
								lData = pAnaWert->stat ? 0x8889 : pAnaWert->messw;	// bei Fehler 0x8889
								break;
										
							case ZE_FORMP :
								format = 4;
//-								lData = ((zaehlsp *)(*(long *)pAdrData))->zwert;
								pZAEHLSP = (zaehlsp *)(*(long *)pAdrData);
								lData = basiswert_to_displaywert ( pZAEHLSP->zdim.komma, pZAEHLSP->zdim.dim_idx, pZAEHLSP->zwert, &ret_komma );
								break;
								
							case US_LONG :
								format = 4;
								lData = *(ULONG *)pAdrData;						
								break;

							case JANEIN_FORM :
								format = 1;
								lData =	*pAdrData ? 1 : 0;					
								break;
								
							case JANEIN_FORMOP :
								format = 1;
								lData = ((doutsp *)(*(long *)pAdrData))->wert ? 1 : 0;
								break;
								
							case JANEIN_FORMIP :
								format = 1;
								lData = ((dinpsp *)(*(long *)pAdrData))->bwert ? 1 : 0;						
								break;	
								
							default :
								format = 0;									// Format 0 kennzeichnet Fehler in der Parametrierung								
								break;											// es wird nichts aufgezeichnet
						}
					}
					
					if ( format == 0 )
						break;
		
// nun ist der Platz im EEPROM zu suchen, auf den die Daten geschrieben werden muessen
					if ( ( page = *(PtrBuffer+daeNr ) ) < 3 )
// ***** 07.11.02 aktuelle Page abtesten					
						break;													// Parameterfehler - keine Abfrage, keine Speicherung
					if ( page >= pArchPara->firstPage + pArchPara->anzPages )
					{
						page = pArchPara->firstPage;
						*(PtrBuffer+daeNr) = page;
						wrPtr = 1;
					}
					
					if ( pArchPara->intervall < LONGINTV && TrendNr[daeNr] < MAXTRENDS )
// Trend-Daten mit sehr kurzen Intervallen werden erstmal zwischengespeichert			
					{
// 20.11.02						daeTrend = ( daeTrend < ( MAXTRENDS - 1 ) ) ? daeTrend + 1 : 0;	
						if ( ++daeTrend >= MAXTRENDS )
							daeTrend = 0;
														
						if ( DataBuffer[daeTrend][0] == 0 || DataBuffer[daeTrend][0] > 31
												|| DataBuffer[daeTrend][1] == 0 || DataBuffer[daeTrend][1] > 12
												|| DataBuffer[daeTrend][2] > 23 || DataBuffer[daeTrend][3] > 59 )	
							NewPageFillBuffer ( &DataBuffer[daeTrend][0], lData, format );
						else if ( min2 == 0 )
						{																			// neues Jahr
							xicbus ( XEEPADR, &DataBuffer[daeTrend][0], page << 6, 64, BICWR );
							NewPageFillBuffer ( &DataBuffer[daeTrend][0], lData, format );																																								
							page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																									page + 1 : pArchPara->firstPage;
							*(PtrBuffer+daeNr) = page;
							wrPtr = 1;													// Aenderung merken	
						}
						else
						{
							min1 = MinuteImJahr ( DataBuffer[daeTrend][0], DataBuffer[daeTrend][1], 
																		DataBuffer[daeTrend][2], DataBuffer[daeTrend][3], Jahr );
							if ( min2 >= min1 ) 
							{
								ergDiv = ( (UINT)( min2 - min1 ) ) / pArchPara->intervall;
								if ( ergDiv < ( 60 / format )	)
								{
									if ( format == 2 )
										*( ( UINT * )&DataBuffer[daeTrend][4 + 2 * ergDiv] ) = (UINT)lData;								
									else if ( format == 4 )
										*( ( ULONG * )&DataBuffer[daeTrend][4 + 4 * ergDiv] ) = lData;
									else
										DataBuffer[daeTrend][4 + ergDiv] = (char)lData;														
								}
								else
								{											// Puffer voll
									xicbus ( XEEPADR, &DataBuffer[daeTrend][0], page << 6, 64, BICWR );
									NewPageFillBuffer ( &DataBuffer[daeTrend][0], lData, format );																																								
									page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																															page + 1 : pArchPara->firstPage;
									*(PtrBuffer+daeNr) = page;
									wrPtr = 1;					// Aenderung merken
								}
							}
							else										// mögliche Ursache : Uhr rueckgestellt, sonst Fehler 
							{
								xicbus ( XEEPADR, &DataBuffer[daeTrend][0], page << 6, 64, BICWR );
								NewPageFillBuffer ( &DataBuffer[daeTrend][0], lData, format );																																								
								page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																																	page + 1 : pArchPara->firstPage;
								*(PtrBuffer+daeNr) = page;
								wrPtr = 1;						// Aenderung merken						
							}																							
						}				
					}
					
					else
					{
		// gleich in den EEPROM : Zeitstempel der aktuellen Seite lesen
						if ( xicbus ( XEEPADR, PageBuffer, page << 6, 4, BICRD )	== 0 )
							return;		
						min1 = MinuteImJahr ( PageBuffer[0], PageBuffer[1], PageBuffer[2], PageBuffer[3], Jahr );
						if ( page == pArchPara->firstPage && PageBuffer[0] == 0 )
		// Tag = 0 : Beginn der Aufzeichnung nach Parametrierung			
						{	
							NewPageFillBuffer ( PageBuffer, lData, format );															
							xicbus ( XEEPADR, PageBuffer, pArchPara->firstPage << 6, 64, BICWR );
						}
							
						else if ( min2 == 0 )		// aktuelle Minute
		// Prost Neujahr !	neue Page beginnen, weil sonst Speicherplatz-Berechnung schwierig		
						{
							page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																															page + 1 : pArchPara->firstPage;
							*(PtrBuffer+daeNr) = page;
							wrPtr = 1;								// Aenderung merken
							NewPageFillBuffer ( PageBuffer, lData, format );															
							xicbus ( XEEPADR, PageBuffer, page << 6, 64, BICWR );					
						}	
							
						else 
						{
							if ( min2 >= min1 ) 
							{
								ergLDiv = ldiv ( min2-min1, (long)pArchPara->intervall );
								if ( ergLDiv.rem != 0 )
									break;								// hier wird vorausgesetzt, dass die Archivierung 
																				// aller Datenelemente in einer Minute abgeschlossen ist !
								if ( ergLDiv.quot < ( 60 / format )	)																
									xicbus ( XEEPADR, (char *)&lData,	
																(UINT)((page << 6 ) + 4 + format * ergLDiv.quot), format, BICWR );
								else
								{
									page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																															page + 1 : pArchPara->firstPage;
									*(PtrBuffer+daeNr) = page;
									wrPtr = 1;
									NewPageFillBuffer ( PageBuffer, lData, format );															
									xicbus ( XEEPADR, PageBuffer, page << 6, 64, BICWR );					
								}
							}
							else											// mögliche Ursache : Uhr rueckgestellt, sonst Fehler 
							{
								page = ( page < ( pArchPara->firstPage + pArchPara->anzPages - 1 ) ) ? 
																															page + 1 : pArchPara->firstPage;
								*(PtrBuffer+daeNr) = page;
								wrPtr = 1;
								NewPageFillBuffer ( PageBuffer, lData, format );															
								xicbus ( XEEPADR, PageBuffer, page << 6, 64, BICWR );					
							}																	
						}
					}
					break;	// case subStatus 3
					
			}						// switch ( subStatus )
			
			if ( subStatus  == 3 )
				++daeNr;
			
			break;			// case STORE			

	}								// switch ( kennung )	

	return; 	
}



ULONG MinuteImJahr ( char tag, char mon, char std, char min, char jhr )
{																	
	ULONG x;
	char i;
	
	for ( i = 0, x = tag-1; i < mon - 1; i++ )
		x += dayPerMonth[i];				// vergangene Tage
	if ( ( jhr & 3 ) == 0 && mon > 2 )
		++x;												// Schaltjahr, Monat > 2
	return ( ( x * 24 + std ) * 60 + min );	
}


void NewPageFillBuffer ( char *buffer, ULONG data, char format )
{
	UINT i;

	buffer[0] = Tag;
	buffer[1] = Mon;
	buffer[2] = Std;
	buffer[3] = Min;
	*(ULONG *)( buffer + 4 )	= data;
	if ( format == 1 )
		i = 5;
	else
		i = ( format == 2 ) ? 6 : 8;	
	for ( ; i < 64; i ++ )
		buffer[i] = 0x88;
	return;			
}

/*********************************************************************/
/* Parameter auf Page 0 und ggf. Page 1 lesen und im Puffer ablegen, */
/* um schnelle Zugriffe ohne I2C-Bus-Verzoegerungen zu ermoeglichen  */

UINT ReadPara ( char *buffer, char anzahl )
{
	char n;
	UINT ret;
		
	if ( ( n = (char)( anzahl * sizeof(struct archPara ) ) ) > 60 )
		n = 60;															// Anzahl zu lesender Bytes
	ret = xicbus ( XEEPADR, buffer+4, 0x0004, 60, BICRD );
	if ( ret && ( anzahl > 7 ) )
	{																// Parameter benoetigen zwei Pages
		n = (char)( ( anzahl - 7 ) * sizeof(struct archPara ) - 4 );
		ret = xicbus ( XEEPADR, buffer+64, 0x0040, n, BICRD );
	}	
	return ( ret );									// bei Fehler wird 0 zurueckgegeben
}

#endif


void WritePageToArchiv ( char *TxBuf, char *RxBuf, char prot )
{

#if ARCHIV_ANL == 1

	UINT page;	
	char i, lng;

// 20.11.02	lng = ( prot == PROT485 ) ? *( RxBuf - 1 ) - 2 : *( RxBuf - 1 ) - 5;
	
	lng = (char)( *( RxBuf - 1 ) - 2 );
	if ( prot != PROT485 )
		lng -= 3;
	
	if ( Checksum ( RxBuf+2, lng ) )	// korrektes Ergebnis : 0, wenn CRC in Berechnung eingeschl. 
		RsFrame ( TxBuf, 0,	CRC_ERR,	prot );
	else
	{
		page = *(UINT *)( &(RxBuf[2]) );
		if ( page > 1 || lng < 5 || lng > 68 )		// Schreiben ist nur auf Page 0 und 1 zugelassen
			RsFrame ( TxBuf, 0,	ERANZ,	prot );		
		else
		{
// 1003 Schreibtelegramme auf Page 0 untersuchen
			if ( page == 0 )
			{
				if ( ( RxBuf[4] == 0 && RxBuf[5] == 0 ) || ( RxBuf[4] == 0x55 && RxBuf[5] == 0xAA ) )
					;
				else
				{
					if ( archivWriteError < 0xFF )
						archivWriteError++;
					RsFrame ( TxBuf, 0,	IFU_ERR,	prot );
					return;
				}
			}				
			if ( page == 0 && RxBuf[4] == 0 )
			// erstes Parametriertel.( Page 0 loeschen ) : nur Kennung zerstoeren ( Zeit sparen ) 
				lng = 2;
			else 
				lng -= 4;									
			if ( xicbus ( XEEPADR, &(RxBuf[4]), page << 6, lng, BICWR ) )
				RsFrame ( TxBuf, 0,	RCOK,	prot );	
			else
				RsFrame ( TxBuf, 0,	IFU_ERR,	prot );
		}
	}		

#endif
}

		
void ReadPageFromArchiv ( char *TxBuf, char *RxBuf, char prot )
{
#if ARCHIV_ANL == 1

	char i, n, offs, buffered;
	UINT page;

	if ( xicbus ( XEEPADR, PageBuffer, 0x0080, (char)(TREND_NR+MAXARCH), BICRD ) == 0 )
		RsFrame ( TxBuf, 0,	IFU_ERR,	prot );	
	else if ( archivAktiv == 0 )								// Initialisierung der Archivierung noch nicht beendet
		RsFrame ( TxBuf, 0,	BUSY,	prot );		// PageBuffer wird noch benoetigt, nicht veraendern !
	else
	{
		page = *(UINT *)( &(RxBuf[2]) );
		if ( page > 511 )
			RsFrame ( TxBuf, 0,	ERANZ,	prot );		
		else
		{	
			buffered = 0;
			offs = RsFrame ( TxBuf,	68,	RCOK,	prot );
			*(UINT *)( &(TxBuf[offs]) ) = page;				// Page ins Telegramm	
			offs += 2;
			
			for ( i = 0; i < arch_par && i < MAXARCH; i++ )
			{
				if ( *((UINT *)PageBuffer + i) == page )
				{
					n = *(PageBuffer + TREND_NR + i );
					if ( n < MAXTRENDS )					// diese Seite wird z.Zt.im RAM gespeichert
					{
						for ( i = 0; i < 64; i++ )
							TxBuf[offs++]	= DataBuffer[n][i];							
						buffered = 1;	
					}																							
				}
			}

			if ( buffered == 0 )
			{
				if ( xicbus ( XEEPADR, &TxBuf[offs], page << 6, 64, BICRD )	== 0 )
				{
					RsFrame ( TxBuf, 0,	IFU_ERR,	prot );
					return;
				}	
				offs += 64;			
			}

			*(int *)(TxBuf + offs ) = Checksum ( TxBuf + offs - 66, 66 );
		}
	}

#endif
}


#if ARCHIV_ANL == 1

void ParameterReparieren ( void )
{
	char i, n, chs;
	
	bicbus ( EEPADR,	ParaBuffer, ARCH_PAR_FERRO, 0x40, BICRD );
	if ( ParaBuffer[2] > 7 )
		bicbus ( EEPADR,	ParaBuffer+0x40, ARCH_PAR_FERRO+0x40, 0x40, BICRD );

	n = (char)( ParaBuffer[2] * sizeof( struct archPara) ) + 4;
	for ( i = 0, chs = 0; i < n; i++ )
		chs += ParaBuffer[i];
	
	if ( ParameterPruefen ( ) )
	// Checksumme ok
	{
		xicbus ( XEEPADR, ParaBuffer, 0, 0x40, BICWR );
		xicbus ( XEEPADR, &ParaBuffer[64], 0x40, 0x40, BICWR );
		if ( archivParRep < 0xFF )
			++archivParRep;
	}
	else
	{		
		ParaBuffer[0] = 0x66;		
		ParaBuffer[1] = 0x99;
		xicbus ( XEEPADR, ParaBuffer, 0, 2, BICWR );		
	}	
}


char ParameterPruefen ( void )
{
	char i, n, chs;

	n = (char)( ParaBuffer[2] * sizeof( struct archPara) ) + 4;
	for ( i = 0, chs = 0; i < n; i++ )
		chs += ParaBuffer[i];
		
	return ( ( ParaBuffer[n] == chs ) ? TRUE : FALSE );
}

#endif