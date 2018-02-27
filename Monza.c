/* Task: Monza									    														*/
/* Sichern der Zählerwerte     im Monatsspeicher								*/
/* Sichern der Verbrauchswerte im Monatsspeicher								*/

#include <string.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "unoinitext.h"
#include "standdef.h"
#include "userdef.h"	
#include "uconstext.h"

void Copy_Verbr(char znum, char device);
char Clear_Verbr(char monza_aktion, char znum, UINT monats_spadr, char device);
char Zaehler_Stichtag(char monza_aktion, char znum, char device);

unsigned long long get_max_display(char dim, char komma);

/*---------------------- Task Monza --------------------------------*/
void Monza(void)
{
	char jahroffs, monoffs, tagoffs, i, mask, status, index;
	char buf[7] = { 0 };
	
	//---------------------------------------------------------------------------------
	// Nach Reset werden alle Monatsverbräuche errechnet und in den Anzeige-Ram geladen
	// (Vorjahr komplett und akt.Jahr bis zum Vormonat)
	//---------------------------------------------------------------------------------
	if(monza_first == FALSE)				// nach Reset
	{	
		if(monza_ramex != 0x12345678)	// Noinit Ram zerstört ?
		{	
			monza_aktiv		= 0;
			monza_aktion	= 0;
			retur_aktion	= 0;
			nullg_aktion	= 0;
			monats_flag		= 0;
			jahres_flag		= 0;
			monza_ramex 	= 0x12345678;
		}
			
		if(monza_aktiv == TRUE)				// Monza war aktiv, wurde durch Reset unterbrochen					
		{	monza_first = TRUE;
			return;											// weitermachen an der unterbrochenen Stelle
		}	
		else
		{
			// Monatsverbräuche und Vorjahreszählwert transportieren in die Zählerstruktur zur Anzeige
			if(monza_znum < 8)
			{	
				// R36
				Copy_Verbr(monza_znum, 0);
				monza_znum++;
			}
			else if(monza_znum < 14)
			{	
				// R37_1
				if(R37_MODULE > 0)
				{
					Copy_Verbr(monza_znum - 8, 1);
				}	
				monza_znum++;
			}
			else if(monza_znum < 20)
			{	
				// R37_2
				if(R37_MODULE > 1)
				{
					Copy_Verbr(monza_znum - 14, 2);
				}	
				monza_znum++;
			}
#if ( MBUSANZ > 0 && MBUS_MASTER == 0 )
			else if ( monza_znum >= 20 && monza_znum < 20 + MBUSANZ )
			{	
				// MBus
				Copy_Verbr ( monza_znum - 20, 3 );
				monza_znum++;
			}
#endif						
			else
			{ 
				std_old = Std;
				monza_znum = 0;
				monza_first = TRUE;
			}
			return;
		}	
	}			
	//------------------------------------------------------------------


	buf[0] = Tag;
	buf[1] = Mon;
	buf[2] = Jahr;
	buf[3] = Std;
	buf[4] = Min;
	buf[5] = Sek;

	// Stündliche Speicherung der aktuellen Zählwerte im EEPROM
	if(monza_aktiv == FALSE)
	{	//Abfrage nach neuer Stunde
		if( std_old != Std )
		{	
			std_old = Std; 
			// Abfrage nach neuem Jahr
			if(Tag == 1 && Mon == 1 && Std == 0)
			{	
				jahres_flag = TRUE;
				monats_flag = TRUE;
			}
			else
			{// Abfrage nach neuem Monat
				if(Tag == 1 && Std == 0)
				{
					monats_flag = TRUE;
				}	
			}		
			
			// Speicheradresse für Verbrauchswerte berechnen
			monv_spadr 				= ZMV01_ADR + (Mon-1) * 4;							// neu: Verbrauchswerte = 4 Byte
			monv_spadr_r37_1	= ZMV11_ADR + (Mon-1) * 4;
			monv_spadr_r37_2	= ZMV21_ADR + (Mon-1) * 4;
			monv_spadr_MBus		= ZMVMB1_ADR + (Mon-1) * 4;
			
			monza_aktion = 0;
			monza_aktiv  = TRUE;
			return;																										// Task-Ende
		}
		
		// keine neue Stunde, Fehlerspeicherung untersuchen
		for(i = 0, mask = 0x01; i < 8; i++, mask <<= 1)
		{
			status = zaehler[i].ucZstatus;
			if( (status & FEHLER_GESICHERT) == FEHLER_GESICHERT)																	// Fehler wurde schon gesichert
			{	if((status & FEHLER) != FEHLER)																											// und es liegt kein Fehler mehr an
				{	
					buf[6] = status;																																	// die restlichen Fehlerbits einsetzen
				//bicbus(EEPADR, (char*)&buf[0], Z1LF_ADR+(zeiger*OFFS_ZXLF_ADR), 7, BICWR);				// speichern Fehler Letztes Auftreten
					status &= (unsigned char)~(FEHLER_GESICHERT | KABELBRUCH | KURZSCHLUSS);					// Merkbits löschen
					zaehler[i].ucZstatus = status;
					Zfehler &= (unsigned char)~mask;																									// Fehlerbit löschen
				}
			}
			else	// möglicher Fehler wurde noch nicht gesichert
			{
				if( (status & FEHLER) == FEHLER)
				{					
				//bicbus(EEPADR, (char	*)&buf[6], Z1EF_ADR+2+(zeiger*OFFS_ZXEF_ADR), 1, BICRD);		// lesen Fehler Erstes Auftreten
					if(buf[6] == 0)
					{	// es ist noch kein erster Fehler aufgetreten, also speichern Fehler Erstes Auftreten
						buf[6] = status;																																// die Fehlerbits einsetzen
					//bicbus(EEPADR, (char	*)&buf[0], Z1EF_ADR+(zeiger*OFFS_ZXEF_ADR), 7, BICWR);		// speichern Fehler Erstes Auftreten
					}
					else		// es gibt einen gespeicherten ersten Fehler
					{
						buf[6] = status;																																// die Fehlerbits einsetzen
					//bicbus(EEPADR, (char	*)&buf[0], Z1LF_ADR+(zeiger*OFFS_ZXLF_ADR), 7, BICWR);		// speichern Fehler Letztes Auftreten
					}
					
					status &= (unsigned char)~(FEHLER | KABELBRUCH | KURZSCHLUSS);										// Merkbits löschen
					status |= (unsigned char)FEHLER_GESICHERT;																				// Merkbit setzen
					zaehler[i].ucZstatus = status;
				}
			}
		}			
						
	}			  
	else	// Zählerspeicherung durchführen
	{	
		index = 0;

		switch(monza_aktion)
		{	
			case 0:											
				if(bicmove_flag != 2)			// Zählersicherung abwarten (in BicMove.c jede Miniute)
				{	monza_aktion++;
					retur_aktion = 0;
				}	
				break;

			// Monatszaehler 1 bis 8 schreiben
			//-----------------------------------------------------------------
			case 8:	index++;
			case 7:	index++;
			case 6:	index++;
			case 5:	index++;
			case 4:	index++;
			case 3:	index++;
			case 2:	index++;
			case 1:
				// R36
				// Monatswechsel (evtl. inclusive Jahreswechsel) ?
				if(monats_flag == TRUE)
				{	if(retur_aktion != monza_aktion)								// Überprüfen, ob Clear_Verbr nach einer Unterbrechung(Reset) schon durchgeführt wurde
						retur_aktion = Clear_Verbr(monza_aktion, index, monv_spadr, 0 );	// Zählerindex 0-7
				}		
				else	// Jede Stunde
				{	// aktuellen Verbrauchswert --> EEPROM
					bicbus(EEPADR, (char *)&zaehler[index].vwert, monv_spadr, 4, BICWR);
				}
				monv_spadr += ZMV_OFFSET;													// Offset zur nächsten Zähleradresse im Verbrauchsbereich
				monza_aktion++;
				break;
			
				// R37_1
			case 14:	index++;
			case 13:	index++;
			case 12:	index++;
			case 11:	index++;
			case 10:	index++;
			case 9:
				if(R37_MODULE > 0)
				{
					// Monatswechsel (evtl. inclusive Jahreswechsel) ?
					if(monats_flag == TRUE)
					{	if(retur_aktion != monza_aktion)	
							retur_aktion = Clear_Verbr(monza_aktion, index, monv_spadr_r37_1, 1 );	// Zählerindex 0-5
					}		
					else	// Jede Stunde
					{	// aktuellen Verbrauchswert --> EEPROM
						bicbus(EEPADR, (char *)&mod37[0].zaehler[index].vwert, monv_spadr_r37_1, 4, BICWR);
					}
					monv_spadr_r37_1 += ZMV_OFFSET;								// Offset zur nächsten Zähleradresse im Verbrauchsbereich
				}
				monza_aktion++;
				break;
				
				// R37_2
			case 20:	index++;
			case 19:	index++;
			case 18:	index++;
			case 17:	index++;
			case 16:	index++;
			case 15:
				if(R37_MODULE > 1)
				{
					// Monatswechsel (evtl. inclusive Jahreswechsel) ?
					if(monats_flag == TRUE)
					{	if(retur_aktion != monza_aktion)	
							retur_aktion = Clear_Verbr(monza_aktion, index, monv_spadr_r37_2, 2 );	// Zählerindex 0-5
					}		
					else	// Jede Stunde
					{	// aktuellen Verbrauchswert --> EEPROM
						bicbus(EEPADR, (char *)&mod37[1].zaehler[index].vwert, monv_spadr_r37_2, 4, BICWR);
					}
					monv_spadr_r37_2 += ZMV_OFFSET;								// Offset zur nächsten Zähleradresse im Verbrauchsbereich
				}	
				monza_aktion++;
				break;
			
			// Abspeicherung von Low-High-Flanken veranlassen, wenn welche erkannt wurden
			//---------------------------------------------------------------------------
			case 28:	index++;
			case 27:	index++;
			case 26:	index++;
			case 25:	index++;
			case 24:	index++;
			case 23:	index++;
			case 22:	index++;
			case 21:
				// R36
				if((zaehler[index].ucZstatus & LO_HI_FLANKE) == LO_HI_FLANKE)
				{
					zaehler[index].ucZstatus &= (unsigned char)~LO_HI_FLANKE;										// löschen des Merkbits
					// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
					bicbus(EEPADR, (char	*)&buf[0], Z01LH_ADR + index * ZFEHLER_OFFS, 6, BICWR);
				}
				monza_aktion++;
				break;

			case 34:	index++;
			case 33:	index++;
			case 32:	index++;
			case 31:	index++;
			case 30:	index++;
			case 29:
				// R37_1
				if(R37_MODULE > 0)
				{
					if((mod37[0].zaehler[index].ucZstatus & LO_HI_FLANKE) == LO_HI_FLANKE)
					{
						mod37[0].zaehler[index].ucZstatus &= (unsigned char)~LO_HI_FLANKE;
						// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
						bicbus(EEPADR, (char	*)&buf[0], Z11LH_ADR + index * ZFEHLER_OFFS, 6, BICWR);
					}
				}	
				monza_aktion++;
				break;

			case 40:	index++;
			case 39:	index++;
			case 38:	index++;
			case 37:	index++;
			case 36:	index++;
			case 35:
				// R37_2
				if(R37_MODULE > 1)
				{
					if((mod37[1].zaehler[index].ucZstatus & LO_HI_FLANKE) == LO_HI_FLANKE)
					{
						mod37[1].zaehler[index].ucZstatus &= (unsigned char)~LO_HI_FLANKE;
						// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
						bicbus(EEPADR, (char	*)&buf[0], Z21LH_ADR + index * ZFEHLER_OFFS, 6, BICWR);
					}
				}	
				monza_aktion++;
				break;

			// Abspeicherung von High-Low-Flanken veranlassen, wenn welche erkannt wurden
			//---------------------------------------------------------------------------
			case 48:	index++;
			case 47:	index++;
			case 46:	index++;
			case 45:	index++;
			case 44:	index++;
			case 43:	index++;
			case 42:	index++;
			case 41:
				// R36
				if((zaehler[index].ucZstatus & HI_LO_FLANKE) == HI_LO_FLANKE)
				{
					zaehler[index].ucZstatus &= (unsigned char)~HI_LO_FLANKE;										// löschen des Merkbits
					// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
					bicbus(EEPADR, (char	*)&buf[0], Z01HL_ADR + index * ZFEHLER_OFFS, 6, BICWR);
				}
				monza_aktion++;
				break;

			case 54:	index++;
			case 53:	index++;
			case 52:	index++;
			case 51:	index++;
			case 50:	index++;
			case 49:
				// R37_1
				if(R37_MODULE > 0)
				{
					if((mod37[0].zaehler[index].ucZstatus & HI_LO_FLANKE) == HI_LO_FLANKE)
					{
						mod37[0].zaehler[index].ucZstatus &= (unsigned char)~HI_LO_FLANKE;
						// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
						bicbus(EEPADR, (char	*)&buf[0], Z11HL_ADR + index * ZFEHLER_OFFS, 6, BICWR);
					}
				}	
				monza_aktion++;
				break;

			case 60:	index++;
			case 59:	index++;
			case 58:	index++;
			case 57:	index++;
			case 56:	index++;
			case 55:
				// R37_2
				if(R37_MODULE > 1)
				{
					if((mod37[1].zaehler[index].ucZstatus & HI_LO_FLANKE) == HI_LO_FLANKE)
					{
						mod37[1].zaehler[index].ucZstatus &= (unsigned char)~HI_LO_FLANKE;
						// Abspeicherung von Tag, Monat, Jahr, Stunde, Minute, Sekunde
						bicbus(EEPADR, (char	*)&buf[0], Z21HL_ADR + index * ZFEHLER_OFFS, 6, BICWR);
					}
				}	
				monza_aktion++;
				break;

			// Stichtagsoperation für Hauptzähler
			//---------------------------------------------------------------------------
			case 68:	index++;
			case 67:	index++;
			case 66:	index++;
			case 65:	index++;
			case 64:	index++;
			case 63:	index++;
			case 62:	index++;
			case 61:
				// R36
				if(retur_aktion != monza_aktion)									// Überprüfen, ob Stichtag nach einer Unterbrechung(Reset) schon durchgeführt wurde
					retur_aktion = Zaehler_Stichtag(monza_aktion, index, 0);
				monza_aktion++;
				break;

			case 74:	index++;
			case 73:	index++;
			case 72:	index++;
			case 71:	index++;
			case 70:	index++;
			case 69:
				// R37_1
				if(R37_MODULE > 0)
				{
					if(retur_aktion != monza_aktion)									// Überprüfen, ob Stichtag nach einer Unterbrechung(Reset) schon durchgeführt wurde
						retur_aktion = Zaehler_Stichtag(monza_aktion, index, 1);
				}	
				monza_aktion++;
				break;
												
			case 80:	index++;
			case 79:	index++;
			case 78:	index++;
			case 77:	index++;
			case 76:	index++;
			case 75:
				// R37_2
				if(R37_MODULE > 1)
				{
					if(retur_aktion != monza_aktion)									// Überprüfen, ob Stichtag nach einer Unterbrechung(Reset) schon durchgeführt wurde
						retur_aktion = Zaehler_Stichtag(monza_aktion, index, 2);
				}	
				monza_aktion++;
				break;

#if ( MBUSANZ > 0 && MBUS_MASTER == 0 )
			case 86:	index++;
			case 85:	index++;
			case 84:	index++;
			case 83:	index++;
			case 82:	index++;
			case 81:
				// MBus
				if ( monza_aktion < MBUSANZ + 81 )
					if ( retur_aktion != monza_aktion )									// Überprüfen, ob Stichtag nach einer Unterbrechung(Reset) schon durchgeführt wurde
						retur_aktion = Zaehler_Stichtag ( monza_aktion, index, 3 );
				monza_aktion++;
				break;

			case 92:	index++;
			case 91:	index++;
			case 90:	index++;
			case 89:	index++;
			case 88:	index++;
			case 87:
				// MBus
				// Monatswechsel (evtl. inclusive Jahreswechsel) ?
				if ( monza_aktion < MBUSANZ + 87 )
				{
					if(monats_flag == TRUE)
					{	if(retur_aktion != monza_aktion)								// Überprüfen, ob Clear_Verbr nach einer Unterbrechung(Reset) schon durchgeführt wurde
							retur_aktion = Clear_Verbr(monza_aktion, index, monv_spadr_MBus, 3 );	// Zählerindex 0-5
					}		
					else	// Jede Stunde
					{	// aktuellen Verbrauchswert --> EEPROM
						bicbus(EEPADR, (char *)&mbZaehler[index].vwert, monv_spadr_MBus, 4, BICWR);
					}
				}
				monv_spadr_MBus += ZMV_OFFSET;													// Offset zur nächsten Zähleradresse im Verbrauchsbereich
				monza_aktion++;
				break;
#endif												

			//---------------------------------------------------------------------------
			default:
				// Speicherprozedur Ende
				monza_aktiv  = 0;
				monza_aktion = 0;
				retur_aktion = 0;
				nullg_aktion = 0;
				monats_flag  = 0;
				jahres_flag  = 0;
				break;
		}								
	}			
}				
/*---------------------------------- Task Ende ---------------------------------------*/


//-------------------------------------------------------------------------------------
// Monatverbräuche (und Vorjahreszählerstand) aus dem EEPROM in den Zähler-Ram kopieren
//-------------------------------------------------------------------------------------
void Copy_Verbr(char znum, char device)
{
	// znum: Zählernummer ist 0-basierend
	// Aktuelles Jahr und Vorjahr: je 12 Monate * 4 Byte = 48 Bytes
	switch (device)
	{
		case 0:		// R36
			bicbus(EEPADR, (char *)&zaehler[znum].verbr_akt[0],						ZMV01_ADR + znum * ZMV_OFFSET + 0,  				48,	BICRD);
			bicbus(EEPADR, (char *)&zaehler[znum].verbr_vor[0], 					ZMV01_ADR + znum * ZMV_OFFSET + ZMV_VJOFFS, 48,	BICRD);
			bicbus(EEPADR, (char *)&zaehler[znum].zwert_vj,						VJZAEHL01_ADR + znum * 8,												 8, BICRD);
			break;
		case 1:		// R37_1
			bicbus(EEPADR, (char *)&mod37[0].zaehler[znum].verbr_akt[0],	ZMV11_ADR + znum * ZMV_OFFSET + 0,					48,	BICRD);
			bicbus(EEPADR, (char *)&mod37[0].zaehler[znum].verbr_vor[0],	ZMV11_ADR + znum * ZMV_OFFSET + ZMV_VJOFFS,	48,	BICRD);
			bicbus(EEPADR, (char *)&mod37[0].zaehler[znum].zwert_vj,	VJZAEHL11_ADR + znum * 8,												 8, BICRD);
			break;
		case 2:		// R37_2
			bicbus(EEPADR, (char *)&mod37[1].zaehler[znum].verbr_akt[0],	ZMV21_ADR + znum * ZMV_OFFSET + 0,					48,	BICRD);
			bicbus(EEPADR, (char *)&mod37[1].zaehler[znum].verbr_vor[0],	ZMV21_ADR + znum * ZMV_OFFSET + ZMV_VJOFFS,	48,	BICRD);
			bicbus(EEPADR, (char *)&mod37[1].zaehler[znum].zwert_vj,	VJZAEHL21_ADR + znum * 8,												 8, BICRD);
			break;
#if ( MBUSANZ > 0 && MBUS_MASTER == 0 )
		case 3:		// MBus
			bicbus ( EEPADR, (char *)&mbZaehler[znum].verbr_akt[0],			 ZMVMB1_ADR + znum * ZMV_OFFSET + 0,					48,	BICRD );
			bicbus ( EEPADR, (char *)&mbZaehler[znum].verbr_vor[0],			 ZMVMB1_ADR + znum * ZMV_OFFSET + ZMV_VJOFFS,	48,	BICRD );
			bicbus ( EEPADR, (char *)&mbZaehler[znum].zwert_vj,			 VJZAEHLMB1_ADR + znum * 8,												 8, BICRD );
			break;
#endif
	}	
}


//----------------------------------------------------------------------------
// Prozedur zum Monatswechsel	(und evtl. Jahreswechsel)
//----------------------------------------------------------------------------
char Clear_Verbr(char monza_aktion, char znum, UINT monats_spadr, char device)
{
	char i;
	UINT vormon_adr, verbr_adr, monat_adr;
	char swap[48] = { 0 };
	zaehlsp *pZaehler;
	
	// Ermittlung des Vormonats zur Speicherung des Zählwertes der letzten Stunde vor dem Wechsel
	if(jahres_flag == TRUE)								// dann steht monats_spadr auf Januar (+0) und der
		vormon_adr = monats_spadr + 44;			// Vormonat ist der Dezember (4 Byte pro Monat * 11)
	else
		vormon_adr = monats_spadr - 4;			// Vormonat

	switch(device)
	{
		case 0:		// R36
			pZaehler	= &zaehler[znum];
			verbr_adr = VERBRAU01_ADR + znum * 4;
			monat_adr = ZMV01_ADR + znum * ZMV_OFFSET;
			break;
		case 1: 	// R37_1
			pZaehler	= &mod37[0].zaehler[znum];
			verbr_adr = VERBRAU11_ADR + znum * 4;
			monat_adr = ZMV11_ADR + znum * ZMV_OFFSET;
			break;
		case 2: 	// R37_2
			pZaehler	= &mod37[1].zaehler[znum];
			verbr_adr = VERBRAU21_ADR + znum * 4;
			monat_adr = ZMV21_ADR + znum * ZMV_OFFSET;
			break;
#if ( MBUSANZ > 0 && MBUS_MASTER == 0 )
		case 3: 	// MBus
			pZaehler	= &mbZaehler[znum];
			verbr_adr = VERBRAUMB1_ADR + znum * 4;
			monat_adr = ZMVMB1_ADR + znum * ZMV_OFFSET;
			break;
#endif
		default:
			pZaehler	= NULL;		// ungültig
			verbr_adr = 0;
			monat_adr = 0;
			break;
	}
	
	if(pZaehler != NULL)
	{	
		// Vormonat aktualisieren		
		bicbus(EEPADR, (char *)&pZaehler->vwert, vormon_adr, 4,	BICWR);

		// der RAM wurde bereits in BicMove aktualisiert
		// hier werden noch die nach Sekunde 0 erfassten Impulse (MBus !!!) berücksichtigt
		if ( Mon == 1 )	// Jahreswechsel
			pZaehler->verbr_akt[11] = pZaehler->vwert; 		// Monatswert in den Monatsbereich Dezember
		else						// Monatswechsel
			pZaehler->verbr_akt[Mon-2] = pZaehler->vwert; // Monatswert in den Monatsbereich Vormonat
		
		// Verbrauchszähler im RAM und EEPROM(Ferro) löschen
		// (die Verbrauchszähler sind monatsunabhängig, sie gelten immer nur für den aktuellen Monat)
		pZaehler->vwert = 0;
		bicbus(EEPADR, (char *)&pZaehler->vwert, verbr_adr, 4, BICWR);
				
		// Beim Jahreswechsel Speicherbereiche umkopieren
		if(jahres_flag == TRUE)
		{	
			memcpy(&pZaehler->verbr_vor[0], &pZaehler->verbr_akt[0], 48);												// im Ram: aktuell nach Vorjahr kopieren
			bicbus(EEPADR, (char *)&pZaehler->verbr_vor[0], monat_adr + ZMV_VJOFFS, 48, BICWR);	// Vorjahr speichern
			
			memset( &pZaehler->verbr_akt[0], 0x00, 48);																					// im Ram: aktuelles Jahr löschen
			bicbus(EEPADR, (char *)&pZaehler->verbr_akt[0], monat_adr, 48, BICWR);							// aktuelles Jahr speichern
		}
		else		// kein Jahreswechsel, nur Monatswechsel
		{
			// Verbrauchszähler im Monatsspeicher (neuer Monat) löschen, falls nicht leer
			bicbus(EEPADR, (char *)&pZaehler->vwert, monats_spadr, 4, BICWR);
		}
	}

	return(monza_aktion);	// Rückkehr mit der übergeben monza_aktion, damit mit Hilfe von retur_aktion überprüft werden kann, 
												// ob Clear_Verbr nach einer Unterbrechung(Reset) schon durchgeführt wurde			
}	


//----------------------------------------------------------------------------
// Stichtagsprozedur für Hauptzähler
//----------------------------------------------------------------------------
char Zaehler_Stichtag(char monza_aktion, char znum, char device)
{
	ULONG ulZeitaktu = 0;
	ULONG ulZeitsoll = 0;
	UINT  zadr, zadr_vj, zdim_adr;
	 
	zaehlsp *pZaehler;

	zadr = zadr_vj  = zdim_adr = 0;
	
	switch(device)
	{
		case 0:		// R36
			pZaehler	= &zaehler[znum];
			zadr	 		= ZAEHLER01_ADR 	+ znum * 8;
			zadr_vj		= VJZAEHL01_ADR 	+ znum * 8;
			zdim_adr	= ZAEHL01_DIM_ADR + znum * ZAEHL_DIM_OFFS;
			break;
		case 1:		// R37_1
			pZaehler = &mod37[0].zaehler[znum];
			zadr	 		= ZAEHLER11_ADR 	+ znum * 8;
			zadr_vj		= VJZAEHL11_ADR 	+ znum * 8;
			zdim_adr	= ZAEHL11_DIM_ADR + znum * ZAEHL_DIM_OFFS;
			break;
		case 2:		// R37_2
			pZaehler = &mod37[1].zaehler[znum];
			zadr	 		= ZAEHLER21_ADR 	+ znum * 8;
			zadr_vj		= VJZAEHL21_ADR 	+ znum * 8;
			zdim_adr	= ZAEHL21_DIM_ADR + znum * ZAEHL_DIM_OFFS;
			break;
#if ( MBUSANZ > 0 && MBUS_MASTER == 0 ) 
		case 3:		// MBus
			pZaehler	= &mbZaehler[znum];
			pZaehler->zdim.konfig &= ~Z_KONFIG_NULLUNG;				// kein Rücksetzen bei MBus-Zählern
			zadr	 		= ZAEHLERMB1_ADR 	+ znum * 8;
			zadr_vj		= VJZAEHLMB1_ADR 	+ znum * 8;
			zdim_adr	= ZAEHLMB1_DIM_ADR + znum * ZAEHL_DIM_OFFS;
			break;
#endif
		default:
			pZaehler = NULL;		// ungültig
			break;
	}
	
	if(pZaehler != NULL)
	{
		if( (pZaehler->zdim.konfig & Z_KONFIG_STICHTAG) == Z_KONFIG_STICHTAG )
		{
			ulZeitaktu = time2long(0, 0, 0, Tag, Mon, Jahr);
			ulZeitsoll = time2long(0, 0, 0, pZaehler->zdim.stich_tag[2], pZaehler->zdim.stich_tag[1], pZaehler->zdim.stich_tag[0]); // stich_tag [0]=Jahr [1]=Monat [2]=Tag		

			if(ulZeitsoll != 0)										// gültiges Stichtagsdatum
			{	
				if(ulZeitsoll <= ulZeitaktu)
				{	// der Stichtag ist heute oder lag vor einer Weile, RIEcon36 war nicht unter Betriebsspannung
					
					// 1. Kopieren des aktuellen Zählwertes auf den Vorjahreszählwert
					//    Überprüfung auf Nullung, falls Aktion durch Reset unterbrochen wurde 
					if(nullg_aktion != monza_aktion)
					{
						pZaehler->zwert_vj = pZaehler->zwert;
						bicbus(EEPADR, (char *)&pZaehler->zwert_vj, zadr_vj, 8,	BICWR);
					}
						
					// 2. Hauptzähler auf 0 setzen, wenn gewünscht
					if( (pZaehler->zdim.konfig & Z_KONFIG_NULLUNG) == Z_KONFIG_NULLUNG)
					{
						pZaehler->zwert = 0;
						bicbus(EEPADR, (char *)&pZaehler->zwert, zadr, 8,	BICWR);
						nullg_aktion = monza_aktion;				// Nullung merken, falls Aktion durch Reset unterbrochen wird
					}
					
					// 3. Stichtagsjahr erhöhen
					if ( pZaehler->zdim.stich_tag[1] > Mon || 
							( pZaehler->zdim.stich_tag[1] == Mon && pZaehler->zdim.stich_tag[2] > Tag ) )
						pZaehler->zdim.stich_tag[0]	= Jahr;		// nach Kaltstart: der programmierte Stichtag kommt erst noch 
					else
						pZaehler->zdim.stich_tag[0]	= Jahr + 1;
					bicbus(EEPADR, (char *)&pZaehler->zdim.stich_tag, zdim_adr + STICHTAG_OFFS, 3,	BICWR);
					
				}	
			}						
		}
	}				
	
	return(monza_aktion);	// Rückkehr mit der übergeben monza_aktion, damit mit Hilfe von retur_aktion überprüft werden kann, 
												// ob Stichtag nach einer Unterbrechung(Reset) schon durchgeführt wurde			
}	



//-------------------------------------------------------------------------------------------------
// Berechnung des maximalen Anzeigewert (max_display), bei dem ein Zählerüberlauf angezeigt wird
//-------------------------------------------------------------------------------------------------
unsigned long long  get_max_display(char dim_idx, char komma)
{
	unsigned long long ulAnzeigeMax;
	char dim;
	
	dim = ze_dim_namen[dim_idx].zdeinh;	// Einheit
	
	switch(dim & 0xC8) {
		case E_QMETER:
		case E_KILO:
//					ucExponent = 3;
				switch(komma) {
					case 0:
					default:
						ulAnzeigeMax = 9999999999;
						break;
					case 1:
						ulAnzeigeMax = 999999999;
						break;
					case 2:
						ulAnzeigeMax = 99999999;
						break;
					case 3:
						ulAnzeigeMax = 9999999;
						break;
				}
			break;

		case E_MEGA:
//					ucExponent = 6;
				switch(komma) {
					case 0:
					default:
						ulAnzeigeMax = 9999999999999;
						break;
					case 1:
						ulAnzeigeMax = 999999999999;
						break;
					case 2:
						ulAnzeigeMax = 99999999999;
						break;
					case 3:
						ulAnzeigeMax = 9999999999;
						break;
					case 4:
						ulAnzeigeMax = 999999999;
						break;
				}
			break;
			
		case E_GIGA:
//					ucExponent = 9;
				switch(komma) {
					case 0:
					default:
						ulAnzeigeMax = 9999999999999999;
						break;
					case 1:
						ulAnzeigeMax = 999999999999999;
						break;
					case 2:
						ulAnzeigeMax = 99999999999999;
						break;
					case 3:
						ulAnzeigeMax = 9999999999999;
						break;
					case 4:
						ulAnzeigeMax = 999999999999;
						break;
				}

			break;
	
		default:
//				ucExponent = 0;
			ulAnzeigeMax = 9999999;
			break;
	}
	return ulAnzeigeMax	;
}
		

