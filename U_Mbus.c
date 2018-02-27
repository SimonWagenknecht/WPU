/***************** M-Bus ***************/
// 170214 Drehstromzähler Diehl Elicius sendet Elektroenergie in 0,01 Wh

#include <string.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "unoinitext.h"
#include "standdef.h"
#include "sfr32C87.h"
#include "sio.h"
#include "timer.h"


#include <stdlib.h>

// Abfrage-Status
#define INAKTIV				0
#define SND_NKE 			1
#define ACK_SND_NKE		2
#define REQ_UD2				3
#define RSP_UD2				4
#define NEXT					255

// Telegramm-Folgen
#define READ_DATA			1						// SND_NKE, REQ_UD2
#define FOLGE_MAX			1

// Timouts
#define ACK						1						// Warten auf E5H
#define RSP						2						// Warten auf Respond-Telegramm
#define ERR						3						// Warten auf Respond-Telegramm
#define NEXT_TEL			4						// Pause vor dem nächsten Senden

// M-Bus Empfang
#define RX_ENDE				0x80
#define OVERRUN				0x10
#define PARITY				0x08
#define FRAME					0x04
#define RX_ERR				0x01


// MBus Zähler-Typen
// Wasserzähler
// nur zur Telegramm-Auswertung, Typ ist Kalt- oder Warmwasser
#define WAS					7

// Funktionen
UINT SetTimeout ( char pause );
void DatenLoeschen ( void );
void ZaehlerDatenLoeschen ( char ctr );	
void FillTxBuffer ( char tel, char mbctr );
char CheckAnswer ( char tel, char mbctr );
void RspAuswertung (  char ctr  );
void BinaryValueFix ( char ctr, char unit, char* value, char length );					
void BCDValueFix ( char ctr, char unit, char* pValue, char length );
void EnergyVar ( char offs, char ctr );
void VolumeVar ( char offs, char ctr );
ULONG EltVar ( char offs, char ctr );
ULONG EltVarDIFE ( char offs, char ctr );
void FlowVar ( char offs, char ctr );
void PowerVar ( char offs, char ctr );
UINT TempVar ( char offs );								
int TempVarSigned ( char offs );								
char NextRecord ( char offs );
ULONG ValueBCD ( char *cPtr, char length );
int IntValueBCD ( char *cPtr );
long LongValueBCD ( char *cPtr );
ULONG LongValueSkalieren ( ULONG value, char exp );
void TxInit ( char port );
void RxInit ( char port );
void RxEnde ( char port );
void EnergyVarVIFE ( char offs, char ctr );
void EnergyVarVIFE_AMT_CalecLight ( char offs, char ctr );								
void HeatVarVIFE_Pollustat ( char offs, char ctr );							
void WriteForLT ( char mbctr );
void AktualisVerbrauch ( char mbctr );
void ClearVerbrauch ( void );			// Monatswechsel
void Stichtag ( void );
void Jahreswechsel ( void );
void SetzeZeit ( char ctr, char err );
void VolumeVarDIFE_AllmessISWZ ( char offs, char ctr );	
void KaelteVar_MollineInt8 ( char offs, char ctr );
long PowerBergDCMi ( char offs, char ctr, char vif );							
void EnergyVarVIFE_Pollustat ( char offs, char ctr );
void VolumeVarVIFE ( char offs, char ctr );
long ValueBCD_int ( char *cPtr, char length );
long Power64Bit ( char offs, char vif );
ULONG long ValueBCD1 ( char *cPtr, char length );
long PowerElt ( char offs, char ctr, char vif );							


/*---------------------- Task Mbus -----------------------------------*/
// Gemeinsamer Code für z.Zt. 3 Mbus-Zähler
// Aufruf aus UserHandler.c: alle 50 ms 

#if ( ((IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL) )

char U_Mbus ( char port, char funktion )
{
	static char mbctr;							// aktueller Zähler-Index	
	static UINT tickCtr = 300;			// zählt 50-ms-Ticks, 15 sec Start-Verzögerung
	static char neustart = TRUE;
	char i;	
	char mbCtrAkt = 0;
	static char aktualisiert = FALSE;
	char stdDiff;
	static char letzteAktTag = 0;

#if ( MBUSANZ > 0 )	

	mbus_vis = UNSICHTB;
	
	if ( MbCtrAkt > 1 )
		MbCtrAkt = 0 ;								// falsche Eingabe

	i = 0;
	if ( Funktion_S1 == MBUS1_FU )
		++i;
	if ( Funktion_S2 == MBUS1_FU )
		++i;
	if ( Funktion_S3 == MBUS1_FU )
		++i;
	if ( i > 1 )
		return ( funktion );

// vorerst wird nur S3 implementiert
	if ( port == S1 || port == S2 )
		return ( funktion );		

	mbus_vis = SICHTBAR;
	
	if ( zaehlerNummer == 0 )
		zaehlerNummer = 1;						// mit zaehlerNummer 0 funktioniert get_feldadresse nicht
	else if ( zaehlerNummer > MBUSANZ )
		zaehlerNummer = MBUSANZ;

	if ( MBUS_MASTER == 1 && MbusFrequ == 1 )
		MbusFrequ = 2;								// Abfrage von 60 Zählern dauert länger als 1 Minute
	
	
	if ( port == S3 )
	{
		Baudrate_S3 = 2400;
		Mode_S3 = MASTER;
	}
	
	if ( neustart == TRUE )
	{
		if ( --tickCtr > 0 )					// 15 sec Start-Verzögerung
			return ( funktion );
		MbusAktualisierung = TRUE;
		LetzteAkt.min = Min;
		LetzteAkt.std = Std;
		mbctr = 0;
		MbusStatus = INAKTIV;
#if MBUS_MASTER == 1
		StundeAlt = Std;
#endif
#if MBUS_MASTER == 0
		for ( i = 0; i < MBUSANZ; i++ )
		{
			bicbus ( EEPADR, MbusTxBuf, ZMB1EF_ADR + i*ZFEHLER_OFFS, 3, BICRD );	// MbusTxBuf kann hier unbesorgt zweckentfremdet werden
			// ucZstatus zeigt an, ob bereits ein Fehler aufgetreten ist und im Ferro gesichert wurde
			mbZaehler[i].ucZstatus = 	( MbusTxBuf[2] == 0 || MbusTxBuf[2] > Jahr ) ? 0 : FEHLER_GESICHERT;
		}
#endif		
		neustart = FALSE;
		for ( i = 0; i < MBUSANZ; i++ )
		{
			mbZaehler[i].sync_monat = 0;
			// wird fest vorgegeben, weil immer wieder Probleme durch Weglassen von Kaltstart (67), besonders nach Programmänderungen
			MbusPara[i].Folge = READ_DATA;		
		}			
	}

	AktStd.min = 0;

//-#if MBUS_MASTER == 0
//-// Sichtbarkeit im Display wird nach 30 sec erneuert
//-	if ( tickCtr == 0 )
//-	{
//-		for ( i = 0; i < MBUSANZ; i++ )
//-		{
//-			Sichtbar[i].was = 0;
//-			Sichtbar[i].elt = 0;
//-			Sichtbar[i].wrm = 0;
//-			Sichtbar[i].wrm_h2 = 0;
//-			if ( MbusPara[i].Freigabe == TRUE )
//-			{
//-				if ( MbusPara[i].Typ == WRM )
//-				{
//-					Sichtbar[i].wrm = vis;
//-					Sichtbar[i].wrm_h2 = hid2;
//-					Sichtbar[i].was = vis;
//-
//-				}										
//-				else if ( MbusPara[i].Typ == KWS || MbusPara[i].Typ == WWS	)
//-				{
//-					Sichtbar[i].was = vis;
//-					Sichtbar[i].wrm_h2 = hid2;
//-				}										
//-				else if ( MbusPara[i].Typ == ELT )
//-					Sichtbar[i].elt = vis;
//-			}				
//-		}
//-	}
//-
//-#else
//-// MBus-Master: Sichtbarkeit wird entsprechend der eingestellten Zähler-Nummer ständig neu bestimmt
	if ( MbusPara[zaehlerNummer-1].Freigabe == TRUE )
	{
		if ( MbusPara[zaehlerNummer-1].Typ == WRM )
		{
			Sichtbar.wrm = vis;
			Sichtbar.was = vis;
			Sichtbar.wrm_h2 = hid2;
			Sichtbar.elt = 0;
			Sichtbar.klm = 0;
			Sichtbar.vol = vis;			
		}										
		else if ( MbusPara[zaehlerNummer-1].Typ == KWS || MbusPara[zaehlerNummer-1].Typ == WWS	)
		{
			Sichtbar.was = vis;
			Sichtbar.wrm_h2 = hid2;
			Sichtbar.elt = 0;
			Sichtbar.wrm = 0;
			Sichtbar.klm = 0;
			Sichtbar.vol = vis;			
		}										
		else if ( MbusPara[zaehlerNummer-1].Typ == ELT )
		{
			Sichtbar.elt = vis;
			Sichtbar.was = 0;
			Sichtbar.wrm = 0;
			Sichtbar.wrm_h2 = 0;
			Sichtbar.klm = 0;
			Sichtbar.vol = 0;			
		}
		else if ( MbusPara[zaehlerNummer-1].Typ == KLM )
		{
			Sichtbar.wrm = vis;
			Sichtbar.was = vis;
			Sichtbar.wrm_h2 = hid2;
			Sichtbar.elt = 0;
			Sichtbar.klm = vis;
			Sichtbar.vol = vis;			
		}
		else if ( MbusPara[zaehlerNummer-1].Typ == GMB )
		{
			Sichtbar.vol = vis;
			Sichtbar.elt = 0;
			Sichtbar.was = 0;
			Sichtbar.wrm = 0;
			Sichtbar.wrm_h2 = 0;
			Sichtbar.klm = 0;
		}		
	}
	else
#if MBUS_MASTER == 0
	{
		Sichtbar.was = SICHTKOMTAB;
		Sichtbar.elt = SICHTKOMTAB;
		Sichtbar.wrm = SICHTKOMTAB;
		Sichtbar.wrm_h2 = SICHTKOMTAB;
		Sichtbar.klm = SICHTKOMTAB;
		Sichtbar.vol = SICHTKOMTAB;			
	}
#else	
	{
		Sichtbar.was = 0;
		Sichtbar.elt = 0;
		Sichtbar.wrm = 0;
		Sichtbar.wrm_h2 = 0;
		Sichtbar.klm = 0;
		Sichtbar.vol = 0;	
	}
#endif

#if MBUS_MASTER == 1	
// Stunden-, Monats-, Jahreswechsel ? 
	if ( Std != StundeAlt )
	{
		if ( Std == 0 && Tag != TagAlt )
		{
			TagNeu = TRUE;
			TagAlt = Tag;
			if ( Tag == 1 )
			{
				MonatNeu = TRUE;	
				if ( Mon == 1 )
					JahrNeu = TRUE;
			}
		}
		StundeAlt = Std;
	}

#endif



// Zähler-Aktualisierung: nur Minuten, die durch 60 bzw. 1440 (24 Stunden, Frequenz > 60 ) teilbar sind
// gültig: 1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60, 120, 240, 360, 720, 1440, bei 0 werden alle Daten gelöscht ( Parameter SOFORT AKTUAL.? )
// die Aktualisierungs-Frequenz wird alle 5 Sekunden auf Gültigkeit geprüft und gegebenenfalls korrigiert
	if ( ( tickCtr % 100 ) == 0 )
	{
		if ( MbusFrequ > 0 )
		{
			if ( ( MbusFrequ <= 60 && ( 60 % MbusFrequ ) != 0 ) || ( MbusFrequ > 60 && ( 1440 % MbusFrequ ) != 0 ) )
			{
				if ( MbusFrequ < 10 )
					MbusFrequ = 10;			
				else if ( MbusFrequ < 12 )
					MbusFrequ = 12;			
				else if ( MbusFrequ < 15 )
					MbusFrequ = 15;			
				else if ( MbusFrequ < 20 )
					MbusFrequ = 20;			
				else if ( MbusFrequ < 30 )
					MbusFrequ = 30;			
				else if ( MbusFrequ < 60 )
					MbusFrequ = 60;			
				else if ( MbusFrequ < 120 )
					MbusFrequ = 120;			
				else if ( MbusFrequ < 240 )
					MbusFrequ = 240;			
				else if ( MbusFrequ < 360 )
					MbusFrequ = 360;			
				else if ( MbusFrequ < 720 )
					MbusFrequ = 720;			
				else 
					MbusFrequ = 1440;	
			}
		}
	}		

	
	if ( ++tickCtr >= 600 )
		tickCtr = 0;

	if ( MbusFrequ == 0 )						// Aktualisierung alle MbusFrequ Minuten
	{
		DatenLoeschen ( );						// alle Zähler-Daten löschen
		return ( funktion );
	}

// Inaktivitäts-Zeit abgelaufen ? Sofortige ausserplanmässige Aktualisierung ?
// jede Minute einmal testen, ob eine Aktualisierung der Daten zu starten ist
	if ( MbusAktualisierung == FALSE  && tickCtr == 0 )
	{																
		if ( MbusFrequ < 60 )
		{
			if ( ( Min % MbusFrequ ) == 0 )
				mbCtrAkt = TRUE;
		}
		else													// im Prinzip immer Aktualisierung zur Minute 0, aber Vorsicht wegen Synchronisation der Uhrzeit durch R50 oder Leit-PC
		{
			if ( Min > 2 && Min < 58 )
				aktualisiert = FALSE;	
			else if ( Min <= 2 && aktualisiert == FALSE )
			{
				if ( Std >= AktStd.std )
					stdDiff = (char)( Std - AktStd.std );
				else
					stdDiff = (char)( 24 - AktStd.std + Std );	
				if ( ( stdDiff % ( MbusFrequ / 60 ) ) == 0 )
				{
					mbCtrAkt = TRUE;
					aktualisiert = TRUE;				
				} 							
			}						
		}
	}

// zweimalige Aktualisierung in der gleichen Minute wird verhindert
	if ( letzteAktTag == Tag && LetzteAkt.std == Std && LetzteAkt.min == Min )
		mbCtrAkt = FALSE;
				
	if ( MbusAktualisierung == FALSE && ( MbCtrAkt == TRUE || mbCtrAkt == TRUE ) )
	{
		mbCtrAkt = FALSE;
		MbCtrAkt = FALSE;
		MbusAktualisierung = TRUE;
		mbctr = 0;
		MbusStatus = INAKTIV;
		LetzteAkt.min = Min;
		LetzteAkt.std = Std;
		letzteAktTag = Tag;
	}						

	if ( MbusAktualisierung	== FALSE )			
	{
		MbusCtrAktiv = 0;												// zur Anzeige

#if MBUS_MASTER == 1		
		if ( JahrNeu == TRUE )
		{
			Jahreswechsel ();
			JahrNeu = FALSE;
		}	
		if ( MonatNeu == TRUE )
		{
			ClearVerbrauch ();
			MonatNeu = FALSE;
		}			
		if ( TagNeu == TRUE )
		{
			Stichtag ();
			TagNeu = FALSE;
		}
#endif
			
		return ( funktion );
	}																

	if ( MbusStatus == INAKTIV )
	{
		while ( mbctr < MBUSANZ )
		{
			if ( MbusPara[mbctr].Freigabe == 0 )
			{
				ZaehlerDatenLoeschen ( mbctr );	
				++mbctr;
			}
			else if ( MbusPara[mbctr].Typ != WRM && MbusPara[mbctr].Typ != WWS && MbusPara[mbctr].Typ != KWS && MbusPara[mbctr].Typ != GMB
									&& MbusPara[mbctr].Typ != ELT && MbusPara[mbctr].Typ != KLM && MbusPara[mbctr].Typ != OTHER ) 
			{
				ZaehlerDatenLoeschen ( mbctr );	
				++mbctr;
			}
			else if ( MbusPara[mbctr].Folge == 0 || MbusPara[mbctr].Folge > FOLGE_MAX )
			{
				MbusPara[mbctr].Folge = 0;
				ZaehlerDatenLoeschen ( mbctr );	
				++mbctr;
			}			
			else
				break;
		}

		if ( mbctr >= MBUSANZ )
		{
			mbctr = 0;
			MbusAktualisierung = FALSE;
			MbusStatus = INAKTIV;
		}
		else
		{
			if ( MbusPara[mbctr].Folge == READ_DATA )
				MbusStatus = SND_NKE;	
			else	
				MbusStatus = NEXT;	
		}											
	}

	if ( MbusAktualisierung	== FALSE )
	{
		MbusCtrAktiv = 0;												// zur Anzeige					
		return ( funktion );
	}
	else
		MbusCtrAktiv = (char)( mbctr + 1 );				// zur Anzeige
	
	switch ( MbusStatus )
	{
		case SND_NKE :
			FillTxBuffer ( SND_NKE, mbctr );				// Telegramm aufbauen
			Timeout = SetTimeout ( ACK );
			RxStatus = 0;														// wird von RxISR gesetzt			
			MbusStatus = ACK_SND_NKE;	
			TxInit ( port );												// UART Sende-Initialisierung, Startbyte ausgeben
			break;		

		case ACK_SND_NKE :												// Warten auf Quittung
			if ( RxStatus == RX_ENDE )
			{
				if ( CheckAnswer ( ACK_SND_NKE, mbctr ) == TRUE )
				{																			// ACK = E5h empfangen
					if ( MbusPara[mbctr].Folge == READ_DATA )
					{					
						Timeout = SetTimeout ( NEXT_TEL );									
						MbusStatus = REQ_UD2;
					}
				}
				else
				{
					MbusData[mbctr].errors = 0xFF;

					SetzeZeit ( mbctr, 0xFF );

					Timeout = SetTimeout ( ERR );
					MbusStatus = NEXT;
				}								
			}
			else if ( RxStatus != 0 )
			{
				MbusData[mbctr].errors = 0xFF;

				SetzeZeit ( mbctr, 0xFF );

				Timeout = SetTimeout ( ERR );
				MbusStatus = NEXT;
			}													
			else
			{
				if ( Timeout && ( --Timeout == 0 ) )
				{
					MbusData[mbctr].errors = 0xFF;

					SetzeZeit ( mbctr, 0xFF );

					Timeout = SetTimeout ( NEXT_TEL );									
					MbusStatus = NEXT;									// Keine Antwort, weiter mit dem nächsten Zähler
				}
			}
			break;

		case REQ_UD2 :
			if ( Timeout && ( --Timeout == 0 ) )
			{
// Telegramm aufbauen, Start Senden
				FillTxBuffer ( REQ_UD2, mbctr );
				Timeout = SetTimeout ( RSP );
				RxStatus = 0;													// wird von RxISR gesetzt			
				MbusStatus = RSP_UD2;		
				TxInit ( port );
			}		
			break;
			
		case RSP_UD2 :
			if ( RxStatus == RX_ENDE )
			{
				if ( CheckAnswer ( RSP_UD2, mbctr ) == TRUE )
				{
					RspAuswertung ( mbctr );						// Respond-Tel. ist formal ok, nun die Daten dechiffrieren	
					WriteForLT ( mbctr );								// Daten in Strukturen zaehldim und zaehlsp für Leittechnik
					AktualisVerbrauch ( mbctr );
				}				
				
				Timeout = SetTimeout ( NEXT_TEL );									
				MbusStatus = NEXT;
			}			
			else if ( RxStatus != 0 )
			{
				MbusData[mbctr].errors = 0xFF;

				SetzeZeit ( mbctr, 0xFF );

				Timeout = SetTimeout ( ERR );
				MbusStatus = NEXT;
			}													
			else
			{
				if ( Timeout && ( --Timeout == 0 ) )
				{
					MbusData[mbctr].errors = 0xFF;

					SetzeZeit ( mbctr, 0xFF );

					Timeout = SetTimeout ( NEXT_TEL );									
					MbusStatus = NEXT;							// Keine Antwort, weiter mit dem nächsten Zähler
				}
			}
		
			break;
				
		default :	
			if ( Timeout && ( --Timeout == 0 ) )
			{			
				++mbctr;
				MbusStatus = INAKTIV;	
			}
			break;				
	}		
	return ( funktion );
#else
	return ( 1 );		// GBUS	
#endif		
}		
		

#if ( ((IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL) || ((IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL) )

// Standard : Wartezeit auf Antwort 330 Bit-Perioden + 50 ms von Ende Senden bis 1.Byte der Antwort
// hier werden nochmal für die Telegramm-Laufzeiten addiert :
// - 6 * 11 Bit-Perioden bei SND_NKE (ACK = E5h erwartet)
// - 261 * 11 Bit-Perioden bei bei REQ_UD2 (RESPOND erwartet )
// vor Beginn des nächsten Sendens Pause 33 Bit-Perioden
UINT SetTimeout ( char pause )
{
	UINT ctr = 1;
	
	if ( pause == ACK )		
		ctr = 5;										// 215 ms	
	else if ( pause == RSP || pause == ERR )
		ctr = 28;										// 1384 ms
	else if ( pause == NEXT_TEL )
		ctr = 1;										// 14 ms
	return ( ctr ); 	
}



// Zähler-Daten löschen
void DatenLoeschen ( void )
{
	char ctr;
	  	
	for ( ctr = 0; ctr < MBUSANZ; ctr++ )
		ZaehlerDatenLoeschen ( ctr );
}


void ZaehlerDatenLoeschen ( char ctr )
{
	char i;
	
	MbusData[ctr].errors = 0;
	MbusData[ctr].wmeng = 0;
	MbusData[ctr].kaelte = 0;
	MbusData[ctr].volume = 0;
	MbusData[ctr].tVorlauf = 0;
	MbusData[ctr].tRuecklauf = 0;
	MbusData[ctr].tDifferenz = 0;
	MbusData[ctr].volStrom = 0;
	MbusData[ctr].leistung = 0;
	MbusData[ctr].komma.wmeng = 0;
	MbusData[ctr].komma.volume = 0;
	MbusData[ctr].elektro = 0;
	MbusData[ctr].zwertAkt = 0;
	mbZaehler[ctr].zwert = 0;

	#if MBUS_MASTER == 0
	mbZaehler[ctr].vwert = 0;

	for ( i = 0; i < 31; i++ )
 		MbusRxBuf[i] = 0;							// MbusRxBuf zweckentfremdet, risikolos
	MbusRxBuf[0] = 1;								// Tag, siehe standdef.h ZMB1LH_ADR ff.
	MbusRxBuf[1] = 1;								// Monat
	MbusRxBuf[6] = 1;
	MbusRxBuf[7] = 1;
	MbusRxBuf[12] = 1;
	MbusRxBuf[13] = 1;
	MbusRxBuf[19] = 1;
	MbusRxBuf[20] = 1;
	MbusRxBuf[28] = 1;
	MbusRxBuf[29] = 1;
	bicbus ( EEPADR,	MbusRxBuf, ZMB1LH_ADR + ctr*ZFEHLER_OFFS, 31,	BICWR );

	#else
	Z_Fehler[ctr].ok1Time.tag = 1;
	Z_Fehler[ctr].ok1Time.monat = 1;
	Z_Fehler[ctr].ok1Time.jahr = 0;
	Z_Fehler[ctr].ok1Time.std = 0;
	Z_Fehler[ctr].ok1Time.min = 0;
	Z_Fehler[ctr].ok1Time.sec = 0;

	Z_Fehler[ctr].ok2Time.tag = 1;
	Z_Fehler[ctr].ok2Time.monat = 1;
	Z_Fehler[ctr].ok2Time.jahr = 0;
	Z_Fehler[ctr].ok2Time.std = 0;
	Z_Fehler[ctr].ok2Time.min = 0;
	Z_Fehler[ctr].ok2Time.sec = 0;

	Z_Fehler[ctr].firstErrTime.tag = 1;
	Z_Fehler[ctr].firstErrTime.monat = 1;
	Z_Fehler[ctr].firstErrTime.jahr = 0;
	Z_Fehler[ctr].firstErrTime.std = 0;
	Z_Fehler[ctr].firstErrTime.min = 0;
	Z_Fehler[ctr].firstErrTime.sec = 0;
	Z_Fehler[ctr].firstErr = 0;

	Z_Fehler[ctr].lastErrTime.tag = 1;
	Z_Fehler[ctr].lastErrTime.monat = 1;
	Z_Fehler[ctr].lastErrTime.jahr = 0;
	Z_Fehler[ctr].lastErrTime.std = 0;
	Z_Fehler[ctr].lastErrTime.min = 0;
	Z_Fehler[ctr].lastErrTime.sec = 0;
	Z_Fehler[ctr].lastErr = 0;

	Z_Fehler[ctr].clearDongle = 0;
	Z_Fehler[ctr].clearTag = 1;
	Z_Fehler[ctr].clearMon = 1;
	Z_Fehler[ctr].clearJahr = 0;

	#endif
}


void FillTxBuffer ( char tel, char mbctr )
{
	pMbusTx = MbusTxBuf;

	if ( tel == SND_NKE )
	{
		MbusTxLng = 5;																					// Laenge, Counter
		*(pMbusTx+0) = 0x10;																				// Start
		*(pMbusTx+1) = 0x40;																				// C-Feld
		*(pMbusTx+2) = (char)( MbusPara[mbctr].Adr );								// A-Feld	
		*(pMbusTx+3) = (char)( *(pMbusTx+1) + *(pMbusTx+2) );				// Checksum						
		*(pMbusTx+4) = 0x16;																				// Stop
//-		*(pMbusTx+5) = 0xFF;																				// Abschlußbyte, damit ETX sauber	gesendet wird
	}	

	else if ( tel == REQ_UD2 )						
	{
		MbusTxLng = 5;																					// Laenge, Counter
		*(pMbusTx+0) = 0x10;																				// Start
		*(pMbusTx+1) = 0x7B;																				// C-Feld, FCB ist immer 1 !
		*(pMbusTx+2) = (char)( MbusPara[mbctr].Adr );								// A-Feld	
		*(pMbusTx+3) = (char)( *(pMbusTx+1) + *(pMbusTx+2) );				// Checksum							
		*(pMbusTx+4) = 0x16;																				// Stop
//-		*(pMbusTx+5) = 0xFF;																				// Abschlußbyte, damit ETX sauber	gesendet wird
	}
}


char CheckAnswer ( char tel, char mbctr )
{
	char ret = FALSE;
	char i, l;
	char cs = 0;
	char *cPtr;
	
	pMbusRx = MbusRxBuf;							// Beginn des 256 Byte langen Empfangspuffers !!!	
	if ( tel == ACK_SND_NKE )
	{
		if ( *pMbusRx == 0xE5 )
			ret = TRUE;		
	}
	else if ( tel == RSP_UD2 )
	{
		if ( *pMbusRx == 0x68 && *(pMbusRx+3) == 0x68 && *(pMbusRx+1) == *(pMbusRx+2)	)
		{															// 1. und 2.Startzeichen ok, beide Längenbytes gleich
			if ( ( ( *(pMbusRx+4) & 0xCF ) == 0x08 ) && ( *(pMbusRx+5) == MbusPara[mbctr].Adr ) )
			{														// C-Feld ok ( ACD und DFC ignoriert ), Adresse ok
				if ( ( *(pMbusRx+6) & 0xF8 ) == 0x70 )	
				{													// CI-Feld ok (variable oder fixed data respond, Mode-Bit (Bit 2) ignoriert
					l = *(pMbusRx+1);				
					for ( pMbusRx += 4, cs = 0, i = 0; i < l; i++ ) 
						cs += *( pMbusRx + i );
					if ( cs == *( pMbusRx+l )	)
					{												// Checksum ok
						if ( *( pMbusRx+1+l ) == 0x16 )
							ret = TRUE;					// Ende-Kennung ok	
					}				
				}		
			} 		
		}
	}

	return ( ret );
}


void RspAuswertung ( char ctr )
{
	char *cPtr;
	char c;
	char offs, dif, vif;
	char offs1;
	char treffer = 0;																					// bei Abbruchkriterium bei variable data respond, wenn alle relevanten Daten ermittelt
	char i;
	
	ULONG wmeng = MbusData[ctr].wmeng;
	ULONG volume = MbusData[ctr].volume;
	ULONG elektro = MbusData[ctr].elektro;
	
		
	cPtr = MbusRxBuf;

	// Test CI-Feld, Mode-Bit ignoriert 
	// Mode-Bit Bit 2 steuert Codierung von Multibyte-Daten-Sequenzen 
	// Mode 1 ( Bit 2 = 0 ) : LSByte first, Mode 2 ( Bit 2 = 1 ) : MSByte first 
	// Mode 2 wird nicht empfohlen und deshalb vorerst nicht implementiert
	if ( ( *(cPtr+6) & 0x04 ) == 4 )
		return;	

//Seriennummer
	for ( i = 8; i <= 15; i++ )
		mbZaehler[ctr].zdim.znumm[i] = ' ';
	mbZaehler[ctr].zdim.znumm[0] = ( ( *(cPtr+10) & 0xF0 ) >> 4 ) + '0';
	mbZaehler[ctr].zdim.znumm[1] = ( *(cPtr+10) & 0x0F ) + '0';
	mbZaehler[ctr].zdim.znumm[2] = ( ( *(cPtr+9) & 0xF0 ) >> 4 ) + '0';
	mbZaehler[ctr].zdim.znumm[3] = ( *(cPtr+9) & 0x0F ) + '0';
	mbZaehler[ctr].zdim.znumm[4] = ( ( *(cPtr+8) & 0xF0 ) >> 4 ) + '0';
	mbZaehler[ctr].zdim.znumm[5] = ( *(cPtr+8) & 0x0F ) + '0';
	mbZaehler[ctr].zdim.znumm[6] = ( ( *(cPtr+7) & 0xF0 ) >> 4 ) + '0';
	mbZaehler[ctr].zdim.znumm[7] = ( *(cPtr+7) & 0x0F ) + '0';

	
	// fixed data respond
	if ( *(cPtr+6) == 0x73 )
	{
		// Medium : Wärme
		if ( ( ( *(cPtr+13) & 0xC0 ) == 0 )	&& ( ( *(cPtr+14) & 0xC0 ) == 0x40 ) )
		{
			c = *(cPtr+12);																						// Status
			MbusData[ctr].errors = c & 0x1C;													// Fehler
			if ( c & 0x02 )
				return;																									// "stored at fixed date" nicht implementiert
																																// "Counter 1 and 2 are actual values"
			if ( c & 0x01 )	
			{																													// "Counter 1 and 2 coded signed binary"
				BinaryValueFix ( ctr, *(cPtr+13) & 0x3F, cPtr+15, 4 );					
				BinaryValueFix ( ctr, *(cPtr+14) & 0x3F, cPtr+19, 4 );					
			}
			else
			{																													// "Counter 1 and 2 coded BCD"
				BCDValueFix ( ctr, *(cPtr+13) & 0x3F, cPtr+15, 4 );					
				BCDValueFix ( ctr, *(cPtr+14) & 0x3F, cPtr+19, 4 );					
			}																																																								
		}
		
		// Medium : Wasser
		else if ( ( ( *(cPtr+13) & 0xC0 ) == 0xC0 )	&& ( ( *(cPtr+14) & 0xC0 ) == 0x40 ) )
		{
			c = *(cPtr+12);																						// Status
			MbusData[ctr].errors = c & 0x1C;													// Fehler
			if ( c & 0x02 )
				return;																									// "stored at fixed date" nicht implementiert
																																// "Counter 1 and 2 are actual values"
			if ( c & 0x01 )	
			{																													// "Counter 1 and 2 coded signed binary"
				BinaryValueFix ( ctr, *(cPtr+13) & 0x3F, cPtr+15, 4 );					
				BinaryValueFix ( ctr, *(cPtr+14) & 0x3F, cPtr+19, 4 );					
			}
			else
			{																													// "Counter 1 and 2 coded BCD"
				BCDValueFix ( ctr, *(cPtr+13) & 0x3F, cPtr+15, 4 );					
				BCDValueFix ( ctr, *(cPtr+14) & 0x3F, cPtr+19, 4 );					
			}																																																								
			
		} 
	} 
	
	// variable data respond
	// Records, die DIFE oder VIFE enthalten, werden nicht ausgewertet
	// Ausnahmen:
	// VIF = FBh Wärmemenge in 0,1 MWh oder 1 MWh
	// DIF = 83h DIFE = 40h VIF = 15h	VolumeVarDIFE_AllmessISWZ ( offs, ctr );	
	// DIF = 0x8C DIFE = 0x10 VIF = 0x06 WDV-Molline (Hydrometer) Energy INT 8 (Klimazähler)  gibt als Medium "Wärme" an ! 	
	// DIF = 04h VIFE = 86h VIFE = 3Bh HeatVarVIFE_Pollustat ( offs, ctr ) (Klimazähler)  gibt als Medium "Klima" an !
	else if ( *(cPtr+6) == 0x72 )
	{
//-		if ( ( *(cPtr+14) == WRM ) 																																	// Medium: 4 = Wärme
//-					|| ( ( *(cPtr+14) == WWS ) && ( *(cPtr+11) == 0x18 ) && ( *(cPtr+12) == 0x4E ) && ( ( *(cPtr+20) & 0xF8 ) == 0 ) ) 
//-					// WMZ Sensus	Polluflow meldet sich als WWS !																
//-					|| ( *(cPtr+14) == KLM )
//-					)		
					// WMZ Sensus	Pollustat meldet sich als KLM !	
		if ( MbusPara[ctr].Typ == WRM || MbusPara[ctr].Typ == KLM )																		
		{
			c = *(cPtr+16);																						// Status
			MbusData[ctr].errors = c & 0x1F;													// Fehler
			
			offs = 19;																								// DIF des ersten Data Record
			while ( offs <= ( *(cPtr+1) ) )		
			// basiert auf der Festlegung, dass ein akzeptierter Record aus mindestens 4 Byte besteht
			{
				dif = *( cPtr + offs );
				if ( ( dif & 0x80 ) == 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0 ) )
				{																												// kein DIFE, kein VIFE
					vif = *( cPtr + offs + 1 );
					if ( ( dif & 0xF0 ) == 0 )														// Auswertung nur für Storage Number = 0 und aktuelle Werte 
					{																											// (instantaneous value, function field = 00b )
						switch ( vif & 0x78 )
						{
							case 0 :																					// Energy
								EnergyVar ( offs, ctr );								
								treffer |= 0x01;								
								break;
							
							case 0x10 :																				// Volume
								VolumeVar ( offs, ctr );								
								treffer |= 0x02;								
								break;
							
							case 0x28 :																				// Power
								PowerVar ( offs, ctr );								
								treffer |= 0x04;								
								break;
							
							case 0x38 :																				// Volume Flow
								FlowVar ( offs, ctr );								
								treffer |= 0x08;								
								break;
							
							case 0x58 :																				// Temperatur VL/RL
								if ( ( vif & 0x04 ) == 0 )
								{
									MbusData[ctr].tVorlauf = TempVar ( offs );					// Flow Temperatur
									treffer |= 0x10;								
								}
								else 
								{
									MbusData[ctr].tRuecklauf = TempVar ( offs );				// Return Temperatur
									treffer |= 0x20;								
								}									
								break;
							
							case 0x60 :
								if ( ( vif & 0x0C ) == 0 )
								{
									MbusData[ctr].tDifferenz = TempVarSigned ( offs );	// Differenz VL/RL
									treffer |= 0x40;								
								}
								break;
								
							
							default :
								break;								
																							
						}						
					}					
				}
				
				else if ( ( ( dif & 0xF0 ) == 0 ) && ( *( cPtr + offs + 1 ) == 0xFB ) && ( ( *( cPtr + offs + 2 ) & 0xFE ) == 0 ) )
// Wärmemenge in 0,1 MWh oder 1 MWh
// mit VIFE, kein DIFE, Storage Number = 0, aktuelle Werte
				{
					EnergyVarVIFE ( offs, ctr );								
					treffer |= 0x01;								
				}

				else if ( dif == 0x83 && ( *( cPtr + offs + 1 ) == 0x40 ) && (  *( cPtr + offs + 2 ) == 0x15 ) )
// Allmess IS-WZ Impulssammler-Wasserzähler gibt als Medium "Wärme" an
// liefert zwei Zählwerte, von denen hier nur der erste ausgewertet wird
// DIF = 83h DIFE = 40h VIF = 15h
				{
					VolumeVarDIFE_AllmessISWZ ( offs, ctr );								
					treffer |= 0x01;								
				}

				else if ( *(cPtr+11) == 0xB4 && *(cPtr+12) == 0x05 
								&& dif == 0x04 && ( *( cPtr + offs + 1 ) == 0x85 ) && (  *( cPtr + offs + 2 ) == 0x7D ) )
// Aquametro Calec light, Wärmemenge in 0,1 MWh: DIF = 0x04 VIF = 0x85 VIFE = 0x7D
				{
					EnergyVarVIFE_AMT_CalecLight ( offs, ctr );								
					treffer |= 0x01;								
				}
		
				else if ( dif == 0x8C && ( *( cPtr + offs + 1 ) == 0x10 ) && (  *( cPtr + offs + 2 ) == 0x06 ) )
				// WDV-Molline (Hydrometer) Energy INT 8 (Klimazähler)  gibt als Medium "Wärme" an ! 	
				// alles also wie bei Wärmezählern, nur die Kälte (Tarif 1) wird hier ausgewertet			
				{
					KaelteVar_MollineInt8 ( offs, ctr );
					treffer |= 0x80;								
				}
				
				else if ( ( ( dif & 0xF0 ) == 0 ) && ( ( *( cPtr + offs + 1 ) & 0xF8 ) == 0x80 ) && ( *( cPtr + offs + 2 ) == 0x3B ) )
// Wärmemenge Sensus Pollustat (Klimazähler)
// VIFE kennzeichnet Heat, kein DIFE
				{
					HeatVarVIFE_Pollustat ( offs, ctr );								
					treffer |= 0x01;								
				}
				else if ( ( ( dif & 0xF0 ) == 0 ) && ( ( *( cPtr + offs + 1 ) & 0xF8 ) == 0x80 ) && ( *( cPtr + offs + 2 ) == 0x7D ) )
// Wärmemenge Sensus Pollustat
// VIF = 85 oder 86, VIFE = 7D = value * 1000, also Wert in 0,1 MWh oder in MWh. Kein DIFE
				{
					EnergyVarVIFE_Pollustat ( offs, ctr );								
					treffer |= 0x01;								
				}
				
				offs = NextRecord ( offs );
				if ( offs == 0 )
					break;
				
				if ( treffer == 0xFF )			// alle zu erfassenden Messwerte wurden im Telegramm gefunden, Abbruch der Auswertung
					break;
			}
			
		}

//##### spez.für Prenzlauer Gärten, ISTA-Warmwasser-Zähler					
//-		else if ( ( *(cPtr+14) == KWS ) || ( *(cPtr+14) == WWS ) )// Medium = 16h : Kaltwasser, = 06h : Warmwasser, gilt für PadPuls 2	
//-		else if ( ( *(cPtr+14) == KWS ) || ( *(cPtr+14) == WWS ) || ( *(cPtr+14) == OTHER || ( *(cPtr+14) == WAS ) ) )
//-			// Medium = 16h : Kaltwasser, = 06h : Warmwasser, gilt für PadPuls 2;; Medium = 0 : Warmwasser (ISTA)
		else if ( MbusPara[ctr].Typ == KWS || MbusPara[ctr].Typ == WWS || MbusPara[ctr].Typ == WAS )																		
		{
			c = *(cPtr+16);																						// Status
			MbusData[ctr].errors = c & 0x08;													// Fehler ( PadPuls 2, Bit 3: EEPROM-Fehler )

			// speziell für Wasserzähler Itron Cyble M-Bus, Lübbenau, KiTa Findus
//			if ( ( *(cPtr+14) == WAS ) && ( *(cPtr+11) == 0x77 ) && ( *(cPtr+12) == 0x04 ) 
			if ( ( *(cPtr+1) > 70 ) && ( *(cPtr+14) == WAS ) && ( *(cPtr+11) == 0x77 ) && ( *(cPtr+12) == 0x04 ) 				// 170207
						&& ( *(cPtr+67) == 4 ) && ( *(cPtr+68) >= 0x13 ) && ( *(cPtr+68) <= 0x16 ) )
			{
				VolumeVar ( 67, ctr );
			}
			
			else
			{			
				offs = 19;																								// DIF des ersten Data Record

				while ( offs <= ( *(cPtr+1) ) )		
				// basiert auf der Festlegung, dass ein akzeptierter Record aus mindestens 4 Byte besteht
				{
					dif = *( cPtr + offs );
					if ( ( dif & 0x80 ) == 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0 ) )
					{																												// kein DIFE, kein VIFE
						vif = *( cPtr + offs + 1 );
						if ( ( dif & 0xF0 ) == 0 )														// Auswertung nur für Storage Number = 0 und aktuelle Werte 
						{																											// (instantaneous value, function field = 00b )
							switch ( vif & 0x78 )
							{
								case 0x10 :																				// Volume
									VolumeVar ( offs, ctr );								
									treffer |= 0x01;								
									break;
							
								case 0x38 :																				// Volume Flow
									FlowVar ( offs, ctr );								
									treffer |= 0x08;								
									break;
	
								default :
									break;								
																								
							}						
						}					
					}
					offs = NextRecord ( offs );
					if ( offs == 0 )
						break;

					if ( treffer == 0x09 )			// alle zu erfassenden Messwerte wurden im Telegramm gefunden, Abbruch der Auswertung
						break;
						
				}
			}
			
		}	

		else if ( MbusPara[ctr].Typ == GMB )	// Gas																		
		{
			c = *(cPtr+16);																						// Status
			MbusData[ctr].errors = c & 0x08;													// Fehler ( PadPuls 2, Bit 3: EEPROM-Fehler )

			// speziell für Gaszähler Actaris / Itron Cyble M-Bus Version 1.3/1.4 (Berlin, Joachimstr.8)
			if ( ( *(cPtr+14) == GMB ) && ( *(cPtr+11) == 0x77 ) && ( *(cPtr+12) == 0x04 ) 
						&& ( *(cPtr+67) == 4 ) && ( *(cPtr+68) >= 0x13 ) && ( *(cPtr+68) <= 0x16 ) )
			{
				VolumeVar ( 67, ctr );
			}
			
			else
			{			
				offs = 19;																								// DIF des ersten Data Record
	
				while ( offs <= ( *(cPtr+1) ) )		
				// basiert auf der Festlegung, dass ein akzeptierter Record aus mindestens 4 Byte besteht
				{
					dif = *( cPtr + offs );
					if ( ( ( dif & 0x80 ) == 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0x80  ) && ( *( cPtr + offs + 2 ) == 0x3A  ) ) // ohne
							|| ( ( dif & 0x80 ) == 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0 ) ) )																			// mit Temperaturkompensation
					{																												
						vif = *( cPtr + offs + 1 );
						if ( ( dif & 0xF0 ) == 0 )														// Auswertung nur für Storage Number = 0 und aktuelle Werte 
						{																											// (instantaneous value, function field = 00b )
							switch ( vif & 0x78 )
							{
								case 0x10 :																				// Volumen ohne Kompensation
									VolumeVarVIFE ( offs, ctr );								
									treffer |= 0x01;								
									break;
							
	//-							case 0x38 :																				// Volume Flow
	//-								FlowVar ( offs, ctr );								
	//-								treffer |= 0x08;								
	//-								break;
	
								default :
									break;								
																								
							}						
						}					
					}
	
					offs = NextRecord ( offs );
					if ( offs == 0 )
						break;
	
					if ( treffer == 0x01 )			// alle zu erfassenden Messwerte wurden im Telegramm gefunden, Abbruch der Auswertung
						break;
						
				}
			}
		}

//-		else if ( *(cPtr+14) == ELT )															// Medium = 02h : Elektrizität, gilt für PadPuls 2	
		else if ( MbusPara[ctr].Typ == ELT )
		{
			c = *(cPtr+16);																						// Status
			MbusData[ctr].errors = c & 0x08;													// Fehler ( PadPuls 2, Bit 3: EEPROM-Fehler )
			
			offs = 19;																								// DIF des ersten Data Record

// CYNOX-Umsetzer in Poing speziell codiert
			if ( MbusRxBuf[11] == 0x2E && MbusRxBuf[12] == 0x0F )			// Manufacturer = CYNOX; nur der erste Datensatz wird ausgewertet			
			{
				if ( *( cPtr + offs ) == 0x84 )													// M-Count 2C
					MbusData[ctr].elektro = EltVarDIFE ( offs, ctr );								
				else
					MbusData[ctr].elektro = EltVar ( offs, ctr );					// SchellCount			
			}
			
			else
			{
				while ( offs <= ( *(cPtr+1) ) )		
				// basiert auf der Festlegung, dass ein akzeptierter Record aus mindestens 4 Byte besteht
				{
					dif = *( cPtr + offs );
					if ( ( dif & 0x80 ) == 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0 ) )
					{																												// kein DIFE, kein VIFE
						vif = *( cPtr + offs + 1 );
						if ( ( dif & 0xF0 ) == 0 )														// Auswertung nur für Storage Number = 0 und aktuelle Werte 
						{																											// (instantaneous value, function field = 00b )
							switch ( vif & 0x78 )
							{
								case 0 :																					// Electricity
									if ( ( treffer & 0x01 ) == 0 )
									{ 
										MbusData[ctr].elektro = EltVar ( offs, ctr );								
										treffer |= 0x01;
									}								
									break;

								case 0x28 :	
									if ( ( treffer & 0x04 ) == 0 )
									{
										if ( dif == 7 )																	// Leistung Berg DIF = 7, VIF  = 0x28 -> 64 Bit int, Wert in 0,001 W
											MbusData[ctr].leistung = Power64Bit ( offs, vif );
										else
											MbusData[ctr].leistung = PowerElt ( offs, ctr, vif );	// Anzeige in 0,1 kW				
										treffer |= 0x04;								
									}
									break;
									
								default :
									break;								
																								
							}						
						}					
					}
	
	// Berg-Elektrozähler Biberach: 
//					else if ( ( dif & 0x80 ) != 0 && ( ( *( cPtr + offs + 1 ) & 0x80 ) == 0 ) && ( ( *( cPtr + offs + 2 ) & 0x80 ) == 0 ) )
// TIP Sinus schickt auch Tarif 2 ! Nur Tarif 1 wird ausgewertet, sonst wird elektro überschrieben
					else if ( ( dif & 0x80 ) != 0 && ( ( *( cPtr + offs + 1 ) & 0xF0 ) == 0x10 ) && ( ( *( cPtr + offs + 2 ) & 0x80 ) == 0 ) )
					{																												// ein DIFE, kein VIFE
						vif = *( cPtr + offs + 2 );
						if ( ( dif & 0x70 ) == 0 )														// Auswertung nur für Storage Number = 0 und aktuelle Werte 
						{																											// (instantaneous value, function field = 00b )
							switch ( vif & 0x78 )
							{
	// Energie: DIF = 8Ch, DIFE = 10h (Tarif 1), VIF = 03/04/05 (1Wh/10 Wh/100Wh)
								case 0 :																					// Electricity
									if ( ( treffer & 0x01 ) == 0 )
									{
										MbusData[ctr].elektro = EltVarDIFE ( offs, ctr );								
										treffer |= 0x01;
									}								
									break;
	
								default :
									break;								
																								
							}						
						}
						else if ( dif == 0xC4 && ( *( cPtr + offs + 1 ) == 0 ) && ( vif >= 0x2A || vif <= 0x2E ) )
	// Leistung (PSumme) DIF = C4h, DIFE = 0h (PSumme), VIF = 2Ah (100mW) bis 0x2E (1kW)
							MbusData[ctr].leistung = PowerBergDCMi ( offs, ctr, vif );	// Anzeige in 0,1 kW				
											
					}
	
					offs = NextRecord ( offs );
					if ( offs == 0 )
						break;
	
					if ( treffer == 0x05 )			// alle zu erfassenden Messwerte wurden im Telegramm gefunden, Abbruch der Auswertung
						break;

				}
				
			}
						
		}
		
	} 
	
	if ( wmeng != MbusData[ctr].wmeng || volume != MbusData[ctr].volume || elektro != MbusData[ctr].elektro || MbusData[ctr].errors )
		SetzeZeit ( ctr, MbusData[ctr].errors );	

}


void BinaryValueFix ( char ctr, char unit, char* pValue, char length )	
{
	ULONG value;
	char exp;
	
	value = *(ULONG *)pValue;
	if ( length == 3 )
		value &= 0xFFFFFF;
	else if ( length == 2 )	
		value &= 0xFFFF;
	else if ( length == 1 )	
		value &= 0xFF;
	
	// Wärme	
	if ( unit >= 2 && unit <= 0x13 )
	{
		if ( unit < 0x5 )
			return;																										// Wh nicht implementiert
		else if ( unit > 0x8 )
			return;																										// MWh * 10 und MWh * 100 nicht implementiert
		exp = (char)( unit - 5 );																		// Exponent 0..3
		MbusData[ctr].komma.wmeng = (char)( 3 - exp );							// Kommastelle variabel (Anzeige MWh mit max. 3 Kommastellen)
		MbusData[ctr].wmeng = value;
	}
	
	// Volumen
	// Anzeige in cbm mit 3 Kommastellen, der value ist also in Liter umzurechnen
	else 	if ( unit >= 0x26 && unit <= 0x2E )
	{
		if ( unit < 0x29 )
			return;																										// Milliliter sind nicht implementiert
		else if ( unit > 0x2C )
			return;																										// cbm * 10 und cbm * 100 nicht implementiert
		exp = (char)( unit - 0x29 );																// Exponent 0..3
		MbusData[ctr].komma.volume = (char)( 3 - exp );							// Kommastelle variabel (Anzeige cbm mit max. 3 Kommastellen)	
		MbusData[ctr].volume = value;
	}
}				


void BCDValueFix ( char ctr, char unit, char* pValue, char length )	
{
	char exp;

	// Wärme	
	if ( unit >= 2 && unit <= 0x13 )
	{
		if ( unit < 5	)	
			return;																										// Wh nicht implementiert
		else if ( unit > 8	)	
			return;																										// MWh * 10 und MWh * 100 nicht implementiert
																																// Einheit Joule generell nicht implementiert
		exp = (char)( unit - 5 );																		// Exponent 0..3
		MbusData[ctr].komma.wmeng = (char)( 3 - exp );							// Kommastelle variabel (Anzeige MWh mit max. 3 Kommastellen)
		MbusData[ctr].wmeng = ValueBCD ( pValue, length );;
	}																															
	
	// Volumen
	// Anzeige in cbm mit 3 Kommastellen, der value ist also in Liter umzurechnen
	else 	if ( unit >= 0x26 && unit <= 0x2E )
	{
		if ( unit < 0x29 )
			return;																										// ml sind nicht implementiert
		else if ( unit > 0x2C )
			return;																										// cbm * 10 und cbm * 100 nicht implementiert

		exp = (char)( unit - 0x29 );																// Exponent 0..3
		MbusData[ctr].komma.volume = (char)( 3 - exp );							// Kommastelle variabel (Anzeige cbm mit max. 3 Kommastellen)	
		MbusData[ctr].volume = ValueBCD ( pValue, length );;
	}

}				


void EnergyVar ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 6 && exp <= 7 )
	{
		value = *(ULONG *)(cPtr+2);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+2, length );			
		}
		MbusData[ctr].wmeng = value;
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );		// Kommastelle variabel (Anzeige MWh mit max. 3 Kommastellen)
	}
	
// für ISTA sensonic 2
	else if ( dif == 4 && exp == 5 )
	{
		value = *(ULONG *)(cPtr+2);								// 4 Byte int, in 100 Wh
		MbusData[ctr].wmeng = value;
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );						// Anzeige kWh mit 1 Kommastellen
	}

// für Hydrometer	(Techem) compact IV S
	else if ( dif == 0x0C && exp == 5 )					// 4 Byte BCD, in 100 Wh
	{
		length = 4;
		MbusData[ctr].wmeng	= ValueBCD ( cPtr+2, length );
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );						// Anzeige kWh mit 1 Kommastellen			
	}			
				
}


void EnergyVarVIFE ( char offs, char ctr )
{
	char dif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	exp = ( *( cPtr + 2 ) == 0 ) ? 8 : 9;
	// 2-4 Byte int, 4-8 digit BCD
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 )
	{
		value = *(ULONG *)(cPtr+3);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+3, length );			
		}
		MbusData[ctr].wmeng = value;
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );		// Kommastelle variabel (Anzeige MWh mit max. 1 Kommastellen)
	}
	
}


void HeatVarVIFE_Pollustat ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 6 && exp <= 7 )
	{
		value = *(ULONG *)(cPtr+3);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+3, length );			
		}
		MbusData[ctr].wmeng = value;
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );		// Kommastelle variabel (Anzeige MWh mit max. 3 Kommastellen)
	}	
}


void EnergyVarVIFE_Pollustat ( char offs, char ctr )
{
	char dif = 0;
	char vif = 0;
	char exp = 0;
	char *cPtr = 0;
	char length = 0;
		
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = ( vif & 0x07 ) + 3;
	if ( exp == 8 || exp == 9 )													// immer BCD-codiert
	{
		length = dif & 0x07;
		MbusData[ctr].wmeng = ValueBCD ( cPtr+3, length );			
		MbusData[ctr].komma.wmeng = (char)( 9 - exp );		// Kommastelle variabel (Anzeige MWh mit max. 3 Kommastellen)
	}	
}


void EnergyVarVIFE_AMT_CalecLight ( char offs, char ctr )								
{
	char exp;
	char *cPtr;
	
	cPtr = MbusRxBuf + offs;
	exp = 8;

	MbusData[ctr].wmeng = *(ULONG *)(cPtr+3);					// 4 Byte int
	MbusData[ctr].komma.wmeng = (char)( 9 - exp );		// Anzeige MWh mit 1 Kommastellen
}



// Allmess IS-WZ Impulssammler-Wasserzähler gibt als Medium "Wärme" an
// liefert zwei Zählwerte, von denen hier nur der erste ausgewertet wird
// DIF = 83h DIFE = 40h VIF = 15h
void VolumeVarDIFE_AllmessISWZ ( char offs, char ctr )							
{
// dif = 0x83 3 Byte Integer
// vif = 0x15	volume, Kubikmeter, 3 Byte Integer
	ULONG value = 0;
	char *cPtr;
	
	cPtr = MbusRxBuf + offs;
	value = *(ULONG *)(cPtr+3);								// 4 Byte lesen
	value &= 0xFFFFFF;												// 3 Byte int 	
	MbusData[ctr].volume = value;
	MbusData[ctr].komma.volume = (char)( 1 );	// 1 Kommastelle fest
}


void VolumeVar ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	char os;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	os = (vif & 0x80 ) ? 3 : 2;	// mit/ohne vife
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 0,1l..1cbm
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 2 && exp <= 6 )
	{
		value = *(ULONG *)(cPtr+os);		// 4 Byte int, mit/ohne vife
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+os, length );			
		}
		MbusData[ctr].volume = value;
		MbusData[ctr].komma.volume = (char)( 6 - exp );		// Kommastelle variabel (Anzeige cbm mit max. 3 Kommastellen)	
	}
}


void VolumeVarVIFE ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 0,1l..1cbm
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 2 && exp <= 6 )
	{
		value = *(ULONG *)(cPtr+3);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+3, length );			
		}
		MbusData[ctr].volume = value;
		MbusData[ctr].komma.volume = (char)( 6 - exp );		// Kommastelle variabel (Anzeige cbm mit max. 3 Kommastellen)	
	}
}


void FlowVar ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	float fValue;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 0,1l..1cbm/h
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 2 && exp <= 6 )
	{
		value = *(ULONG *)(cPtr+2);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+2, length );			
		}
		
		// Dimension 0,1 l/h
		if ( exp == 2 )
		{
			value = ( value + 5 ) / 10;							// letzte Nachkomma-Stelle streichen, Wert gerunde 
			exp = 3;
		}
		
		exp -= 3;																	// value bleibt unverändert, wenn in l/h, sonst *10...*1000
		value = LongValueSkalieren ( value, exp );// Anzeige in cbm mit 3 Kommastellen	
		MbusData[ctr].volStrom = value;
	}
		
	else if ( ( dif & 7 ) == 5 && exp == 6 )
	// float, für Sensus PolluStat, in m3/h
	{
		fValue = *(float *)(cPtr+2);	// 4 Byte float
		fValue *= 1000;								
		value = fValue;								// Anzeige in m3/h mit drei Kommastellen
		MbusData[ctr].volStrom = value;		
	}
}


//-void PowerVar ( char offs, char ctr )
//-{
//-	char dif, vif;
//-	char exp;
//-	ULONG value = 0;
//-	char *cPtr;
//-	char length;
//-	float fValue;
//-	
//-	cPtr = MbusRxBuf + offs;
//-	dif = *cPtr & 0x0F;
//-	vif = *(cPtr+1);
//-	exp = vif & 0x07;
//-	// 2-4 Byte int, 4-8 digit BCD, exp 0,1W..1kW
//-	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 2 && exp <= 6 )
//-	{
//-		value = *(ULONG *)(cPtr+2);								// 4 Byte int
//-		if ( dif == 2 )
//-			value &= 0xFFFF;												// 2 Byte int
//-		else if ( dif == 3 )
//-			value &= 0xFFFFFF;											// 3 Byte int 	
//-		else if ( dif & 0x08 )										// BCD
//-		{
//-			length = dif & 0x07;
//-			value = ValueBCD ( cPtr+2, length );			
//-		}
//-		
//-		// Dimension 0,1 W
//-		if ( exp == 2 )
//-		{
//-			value = ( value + 5 ) / 10;							// letzte Nachkomma-Stelle streichen, Wert gerunde 
//-			exp = 3;
//-		}
//-
//-		exp -= 3;																	// value bleibt unverändert, wenn in W, sonst *10 / 100 / 1000
//-		value = LongValueSkalieren ( value, exp );// Anzeige in kW mit 3 Kommastellen	
//-		MbusData[ctr].leistung = value;
//-	}
//-	
//-	else if ( ( dif & 7 ) == 5 && exp == 6 )
//-	// float, für Sensus PolluStat, in kW
//-	{
//-		fValue = *(float *)(cPtr+2);	// 4 Byte float
//-		fValue *= 1000;								
//-		value = fValue;								// Anzeige in kW mit drei Kommastellen
//-		MbusData[ctr].leistung = value;		
//-	}
//-}


// Leistung kann negativ sein, Anzeige in kW mit drei Kommastellen (max +9999,999 bzw. -999,999)
void PowerVar ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	long value = 0;
	char *cPtr;
	char length;
	float fValue;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 0,1W..1kW
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 2 && exp <= 6 )
	{
		value = *(long *)(cPtr+2);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD_int ( cPtr+2, length );			
		}
		
		// Dimension 0,1 W
		if ( exp == 2 )
		{
			value = ( value + 5 ) / 10;							// letzte Nachkomma-Stelle streichen, Wert gerunde 
			exp = 3;
		}

		exp -= 3;																	// value bleibt unverändert, wenn in W, sonst *10 / 100 / 1000
		value = LongValueSkalieren ( value, exp );// Anzeige in kW mit 3 Kommastellen	
//		value /= 100;															// Anzeige in kW mit 1 Kommastellen	
		MbusData[ctr].leistung = value;
	}
	
	else if ( ( dif & 7 ) == 5 && exp == 6 )
	// float, für Sensus PolluStat, in kW
	{
		fValue = *(float *)(cPtr+2);	// 4 Byte float
		fValue *= 1000;								
		value = fValue;								// Anzeige in kW mit drei Kommastellen
//		value /= 100;															// Anzeige in kW mit 1 Kommastellen	
		MbusData[ctr].leistung = value;		
	}
}


long PowerBergDCMi ( char offs, char ctr, char vif )
{
	long value = 0;
	char *cPtr;
	
	cPtr = MbusRxBuf + offs;
	value = *(ULONG *)(cPtr+3);									// 4 Byte int
	if ( vif == 0x2A )													// Wert übertragen in 100 mW
		value /= 1000;	
	else if ( vif == 0x2B )											// Wert übertragen in 1 W
		value /= 100;	
	else if ( vif == 0x2C )											// Wert übertragen in 10 W
		value /= 10;	
	else if ( vif == 0x2E )											// Wert übertragen in 1 kW
		value *= 10;	
	
	return value;																// in 0,1 kW
}


// Elektrozähler codiert Leistung 4 Byte int, kein DIFE, kein VIFE
long PowerElt ( char offs, char ctr, char vif )
{
	long value = 0;
	char *cPtr;
	
	cPtr = MbusRxBuf + offs;
	value = *(ULONG *)(cPtr+2);									// 4 Byte int
	if ( vif == 0x2A )													// Wert übertragen in 100 mW
		value /= 1000;	
	else if ( vif == 0x2B )											// Wert übertragen in 1 W
		value /= 100;	
	else if ( vif == 0x2C )											// Wert übertragen in 10 W
		value /= 10;	
	else if ( vif == 0x2E )											// Wert übertragen in 1 kW
		value *= 10;	
	
	return value;																// in 0,1 kW
}


long Power64Bit ( char offs, char vif )
{
	long long v;
	long value = 0;
	char *cPtr;
	
	cPtr = MbusRxBuf + offs;
	v = *(ULONG long *)(cPtr+2);								// 8 Byte int
	if ( vif == 0x28 )													// Wert übertragen in 1 mW
		v /= 100000;	
	else if ( vif == 0x29 )											// Wert übertragen in 10 mW
		v /= 10000;	
	else if ( vif == 0x2A )											// Wert übertragen in 100 mW
		v /= 1000;	
	else if ( vif == 0x2B )											// Wert übertragen in 1 W
		v /= 100;	
	else if ( vif == 0x2C )											// Wert übertragen in 10 W
		v /= 10;	
	else if ( vif == 0x2E )											// Wert übertragen in 1 kW
		v *= 10;	

	value = (long)v;	
	return value;																// in 0,1 kW
}


void KaelteVar_MollineInt8 ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+2);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 0,1 kWh..10 kWh
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 5 && exp <= 7 )
	{
		value = *(ULONG *)(cPtr+3);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+3, length );			
		}
		
		if ( exp == 5 )
			value = ( value + 5 ) / 10;							// Dimension 0,1 kWh: letzte Nachkomma-Stelle streichen, Wert gerunde 
		else if ( exp == 7 )
			value = value * 10;	

		MbusData[ctr].kaelte = value;
	}
}


UINT TempVar ( char offs )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	float fValue;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x03;

	// 2 Byte int, 4 digit BCD, Temp. in 0,1°C oder 1°C
//	if ( ( dif & 0x07 ) == 2 && exp >= 2 && exp <= 3 )
// Temperaturen kommen in 4 Byte (QUNDIS G20/R20
// die zwei High-Bytes werden in den Skat gedrückt (VL, RL > 1000 °C)
	if ( ( dif & 0x07 ) >= 2 && ( dif & 0x07 ) <= 4 && exp >= 2 && exp <= 3 )
	{
		value = ( *(ULONG *)(cPtr+2) ) & 0xFFFF;	// 2 Byte 
		if ( dif & 0x08 )													// BCD
		{
			length = dif & 0x07;
			if ( length > 2 )
			  length = 2;
			value = ValueBCD ( cPtr+2, length );
		}
		exp -= 2;
		value = LongValueSkalieren ( value, exp );	
	}

	// float, für Sensus PolluStat
	else if ( ( dif & 0x0F ) == 5 && exp == 3 )
	{
		fValue = *(float *)(cPtr+2);	// 4 Byte float
		fValue *= 10;								
		value = fValue;								// Anzeige mit einer Kommastelle
	}	

	// MULTCAL 602 (Kamstrup) Temperaturen in 0,01°C
	else if ( ( dif & 0x07 ) >= 2 && ( dif & 0x07 ) <= 4 && exp == 1 )
	{
		value = ( *(ULONG *)(cPtr+2) );						// 2 - 4 Byte 
		if ( ( dif & 0x07 ) == 2 )
			value &= 0xFFFF;
		else 		if ( ( dif & 0x07 ) == 3 )
			value &= 0xFF;
		if ( dif & 0x08 )													// BCD
		{
			length = dif & 0x07;
			value = ValueBCD1 ( cPtr+2, length );
		}
		value /= 10;
	}

	return ( (UINT)value );
}


int TempVarSigned ( char offs )
{
	char dif, vif;
	char exp;
	int iValue = 0;
	char *cPtr;
	float fValue;
	long lgValue;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x03;

	// 2 Byte int, 4 digit BCD, Temp. in 0,1°C oder 1°C
	if ( ( dif & 0x07 ) == 2 && exp >= 2 && exp <= 3 )
	{
		if ( dif & 0x08 )													// BCD
			iValue = IntValueBCD ( cPtr+2 );
		else
			iValue = ( *(int *)(cPtr+2) );	// 2 Byte 

		exp -= 2;
		
		if ( exp == 1 )
			iValue *= 10;
	}

	// float, , für Sensus PolluStat
	else if ( ( dif & 0x0F ) == 5 && exp == 3 )
	{
		fValue = *(float *)(cPtr+2);	// 4 Byte float
		fValue *= 10;								
		iValue = fValue;								// Anzeige mit einer Kommastelle
	}	

	// MULTCAL 602 (Kamstrup) Temperaturen in 0,01°C / K, 4 digit BCD
	else if ( ( dif & 0x07 ) == 4 && exp == 1 )
	{
		if ( dif & 0x08 )													// BCD
			lgValue = LongValueBCD ( cPtr+2 );
		else
			lgValue = *(cPtr+2);										// 4 Byte 

		iValue = lgValue / 10;
	}

	return ( iValue );
}


ULONG EltVar ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	UINT div = 1;
	ULONG long llvalue;
	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+1);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 1Wh..1kWh
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 3 && exp <= 6 )
	{
		value = *(ULONG *)(cPtr+2);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+2, length );			
		}

		if ( exp == 5 )
			div = 10;																// Dimension 100 Wh
		else if ( exp == 4 )
			div = 100;															// Dimension 10 Wh
		else if ( exp == 3 )
			div = 1000;															// Dimension 1 Wh
		value = value / div;
	}

	// Berg-Zähler schickt 12 digit BCD in 0,1 Wh
	else if ( dif == 0x0E && exp >= 2 && exp <= 3 )
	{
		length = 6;
		llvalue = ValueBCD1 ( cPtr+2, length );	
		if ( exp == 3 )
			div = 1000;															// Dimension 1 Wh
		else if ( exp == 2 )
			div = 10000;														// Dimension 0,1 Wh
		value = llvalue / div;
	}

	// 6/8 Byte integer
	else if ( ( dif & 7 ) >= 6 && ( dif & 7 ) <= 7 && exp >= 1 && exp <= 6 )
	{
		llvalue = *(ULONG long *)(cPtr+2);				// 8 Byte int
		if ( ( dif & 7 ) == 6 )
			llvalue &= 0x00FFFFFF;									// nur 6 Byte
		if ( exp == 5 )
			value = llvalue / 10;										// Dimension 100 Wh
		else if ( exp == 4 )
			value = llvalue / 100;									// Dimension 10 Wh
		else if ( exp == 3 )
			value = llvalue / 1000;									// Dimension 1 Wh
		else if ( exp == 2 )
			value = llvalue / 10000;								// Dimension 0,1 Wh
		else if ( exp == 1 )
			value = llvalue / 100000;								// Dimension 0,01 Wh
	}
	
	return ( value );
}


ULONG EltVarDIFE ( char offs, char ctr )
{
	char dif, vif;
	char exp;
	ULONG value = 0;
	char *cPtr;
	char length;
	UINT div = 1;
	ULONG long llvalue;

	
	cPtr = MbusRxBuf + offs;
	dif = *cPtr & 0x0F;
	vif = *(cPtr+2);
	exp = vif & 0x07;
	// 2-4 Byte int, 4-8 digit BCD, exp 1Wh..1kWh
	if ( ( dif & 7 ) >= 2 && ( dif & 7 ) <= 4 && exp >= 3 && exp <= 6 )
	{
		value = *(ULONG *)(cPtr+3);								// 4 Byte int
		if ( dif == 2 )
			value &= 0xFFFF;												// 2 Byte int
		else if ( dif == 3 )
			value &= 0xFFFFFF;											// 3 Byte int 	
		else if ( dif & 0x08 )										// BCD
		{
			length = dif & 0x07;
			value = ValueBCD ( cPtr+3, length );			
		}

		if ( exp == 5 )
			div = 10;																// Dimension 100 Wh
		else if ( exp == 4 )
			div = 100;															// Dimension 10 Wh
		else if ( exp == 3 )
			div = 1000;															// Dimension 1 Wh
		value = value / div;
	}
	
	else if ( ( dif & 7 ) >= 6 && ( dif & 7 ) <= 7 && exp >= 3 && exp <= 6 )
	{
		llvalue = *(ULONG long*)(cPtr+3);					// 6/8 Byte int
		if ( ( dif & 7 ) == 6 )
			llvalue &= 0x0000FFFFFFFFFFFF;

		if ( exp == 5 )
			div = 10;																// Dimension 100 Wh
		else if ( exp == 4 )
			div = 100;															// Dimension 10 Wh
		else if ( exp == 3 )
			div = 1000;															// Dimension 1 Wh
		value = llvalue / div;
	}
	
	return ( value );
}


// offs verweist auf DIF des aktuellen Records. Rückgabe : nächster offs oder 0 ( Beenden/Abbruch der Auswertung )
char NextRecord ( char offs )
{
	char *cPtr;
	char lng;
	char newOffs;
	char dif;
	char lvar;
	char vif;
	
	cPtr = MbusRxBuf;
	lng = *(cPtr+1);								// Länge der Nutzer-Daten + 3
	dif = *( cPtr + offs );
	
	if ( dif == 0x2F )							// idle filler, nächstes Byte ist wieder ein DIF
		newOffs = (char)( offs + 1 );
	else 
	{
		while ( *( cPtr + offs )	& 0x80 )										
			++offs;																	// es folgt mindestens noch ein DIFE
		++offs;																		// danach verweist offs auf 1.VIFE																						
		while ( *( cPtr + offs )	& 0x80 )										
			++offs;																	// es folgt mindestens noch ein VIFE

		vif = *( cPtr + offs );

		++offs;																		// danach verweist offs auf das erste Datenbyte
		
		dif &= 0x0F;
		
		if ( vif == 0x7C )												// ASCII-String, Länge im ersten Byte
		{
			newOffs = (char)( offs + ( dif & 0x07 ) );
			newOffs += *( cPtr + offs ) + 1;
		}

		else if ( dif == 0x0F )												// herstellerspez.Daten bis zum Ende des Telegramms 
			newOffs = 0;											
		else if  ( dif == 8 )											// Selection for Readout	
			newOffs = 0;
		else if ( dif == 0x0D )										// variable Länge
		{
			lvar = *( cPtr + offs );
			if ( lvar >= 0xFB )
				newOffs = 0;													// reserviert
			else if ( lvar >= 0xC0 )
				newOffs = (char)( offs + ( lvar & 0x0F ) );	// Low-Teil von LVAR : Anzahl folgender Bytes (BCD, binär, float)
			else 
//				newOffs = (char)( offs + lvar );			// ASCII
// 141103 Test Gaszähler Elster: hoffentlich ist die Änderung immer richtig 																
				newOffs = (char)( offs + 1 + lvar );			// ASCII																
		}	
		else if ( dif == 5 )
			newOffs = (char)( offs + 4 );						// 32 Bit float								
		else if ( dif == 7 )
			newOffs = (char)( offs + 8 );						// 64 Bit int
		else
			newOffs = (char)( offs + ( dif & 0x07 ) );		// in allen anderen Fällen liefern die 3 LSBit des DIF
																							// die Anzahl der Bytes des (Mess)-Wertes								
	}	
	
	if ( newOffs != 0 )
		if ( offs > *(cPtr+1 ) )									// weniger als 4 Byte bis zum Ende der User-Daten
			newOffs = 0;
	return ( newOffs );

}


ULONG ValueBCD ( char *cPtr, char length )
{
	char i;
	ULONG value;
	ULONG factor;	

	for ( i = 0, value = 0, factor = 1; i < length; i++, cPtr++ )
	{
		value += ( *cPtr & 0x0F ) * factor;	
		factor *= 10;
		value += ( ( *cPtr >> 4 ) & 0x0F ) * factor;
		factor *= 10;
	}	
	return ( value );	
}


ULONG long ValueBCD1 ( char *cPtr, char length )
{
	char i;
	ULONG long value;
	ULONG factor;	

	for ( i = 0, value = 0, factor = 1; i < length; i++, cPtr++ )
	{
		value += ( *cPtr & 0x0F ) * factor;	
		factor *= 10;
		value += ( ( *cPtr >> 4 ) & 0x0F ) * factor;
		factor *= 10;
	}	
	return ( value );	
}


long ValueBCD_int ( char *cPtr, char length )
{
	char i;
	long value;
	ULONG factor;	
	char c;

	c = *( cPtr + length - 1 );
	if ( ( c & 0xF0 ) == 0xF0 )
	{																// Wert ist negativ
		for ( i = 0, value = 0, factor = 1; i < length - 1; i++, cPtr++ )
		{
			value += ( *cPtr & 0x0F ) * factor;	
			factor *= 10;
			value += ( ( *cPtr >> 4 ) & 0x0F ) * factor;
			factor *= 10;
		}
		value += ( *cPtr & 0x0F ) * factor;	
		value *= -1;		
	}
	else
	{
		for ( i = 0, value = 0, factor = 1; i < length; i++, cPtr++ )
		{
			value += ( *cPtr & 0x0F ) * factor;	
			factor *= 10;
			value += ( ( *cPtr >> 4 ) & 0x0F ) * factor;
			factor *= 10;
		}	
	}
	return ( value );	
}

int IntValueBCD ( char *cPtr )
{
	int value;
	char cLow, cHigh;
	
	cLow = *cPtr;
	cHigh = *(cPtr+1);

	value = (cLow & 0x0F) + ( ( (cLow >> 4) & 0x0F) * 10 ) + ( (cHigh & 0x0F) * 100 );
	if ( cHigh & 0x80 ) 						// BCD ist negativ, das ganze Nibble kann gesetzt sein, nicht weiter auswerten (Biberach, Molline)
		value *= (-1);
	else
		value += ( (cHigh >> 4) & 0x0F) * 1000;	
	return ( value );	
}


long LongValueBCD ( char *cPtr )
{
	long value;
	char cLow, cHigh;
	
	cLow = *cPtr;
	cHigh = *(cPtr+1);

	value = (cLow & 0x0F) + ( ( (cLow >> 4) & 0x0F) * 10 ) + ( (cHigh & 0x0F) * 100 ) + ( ( (cHigh >> 4) & 0x0F) * 1000 );
	
	cLow = *(cPtr+2);
	cHigh = *(cPtr+3);

	value += ( (cLow & 0x0F) + ( ( (cLow >> 4) & 0x0F) * 10 ) + ( (cHigh & 0x0F) * 100 ) );

	if ( cHigh & 0x80 ) 						// BCD ist negativ, das ganze Nibble kann gesetzt sein, nicht weiter auswerten (Biberach, Molline)
		value *= (-1);
	else
		value += ( (cHigh >> 4) & 0x0F) * 1000;	
	
	return ( (int)value );	
}


ULONG LongValueSkalieren ( ULONG value, char exp )
{
	if ( exp == 1 )
		value *= 10;
	else if ( exp == 2 )
		value *= 100;
	else if ( exp == 3 )
		value *= 1000;
	return ( value );
}


void TxInit ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			RTS_S3 = 1;				// Sendekanal	Freigabe

			// Transmit Receive Mode Register
			MR_S3 = 0x65;				// 0 1 1 0 0 101	
													// | | | | | |||
													// | | | | | UART Mode 8 Bit
													// | | | | Internal Clock             
													// | | | 1 Stop Bit	
													// | | parity even	
													// | parity
													// Rx Tx Polarity not inversed
		
			// Transmit Receive Control Register 0											
			C0_S3 = 0x11;				// 0 0 0 1 0 0 01
													// | | | | | | ||
													// | | | | | | f8 Takt
													// | | | | | x CTS oder RTS Funktion siehe Bit4
													// | | | | x Transmit Register Empty Flag TXEPT, Read Only, 
													// | | | Disable CTS/RTS Funktion											
													// | | TxD CMOS Output
													// | CLK Polarität
													// LSB first
	
//-			// Für	Telegramm-Timeout	Überwachung: TimerB0 (Vorteiler) + TimerB1 (Teiler) 
//-			// SourceClock: f2n = 16 (29,491200 MHz / 16 = 1,843200 MHz = 0,54253472 µs)
//-			toutl_S3 		= 64;		// Vorteiler: 64
//-			P_TOUTS_S3	= 0;		// Stop	 Telegramm-Timeout	Timer Prescaler
//-			C_TOUTS_S3	= 0;		// Stop	 Telegramm-Timeout	Timer Counter

			// Baudrate
			BRG_S3 = (unsigned char)( ( (f8_CLK_SPEED/16) / 2400) -1 );
//-			touth_S3 = 19200;

			TE_BIT_S3 = 1;							// Senden erlauben
			DISABLE_IRQ			
			TIC_S3 = 0x04;							// Enable Transmit Interrupt Prio: 4			
			ENABLE_IRQ	
			
			MbusTxCtr = 0;
			TB_S3 = *pMbusTx;						// Startbyte ausgeben
								
			break;
				
	}
}


void RxInit ( char port )
{
	MbusRxCtr		=	0;								// Sende-	/	Empfangspuffer Index
	pMbusRx			= MbusRxBuf;
	MbusRxLng		= 1;								// mindestens ein Zeichen wird erwartet	

	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			RTS_S3 = 0;					// Empfangskanal	Freigabe
			TB_S3 = RB_S3;			// SIO entleeren	(Kopieren auf Sendepuffer)
			TB_S3 = 0;					// Sendepuffer löschen
		
			DISABLE_IRQ			
			RIC_S3 = 0x04;			// Enable Receive Interrupt Prio: 4			
			ENABLE_IRQ	
		
			// Transmit Receive Control Register 1											
			// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
			C1_S3 = 0x00;				// 0 0 0 0 0 0 0 0
													// | | | | | | | |
													// | | | | | | | Transmit Enable Bit TE
													// | | | | | | Transmit Buffer Empty Flag TI (nach Reset = 1), Read Only
													// | | | | | Receive Enable Bit RE
													// | | | | x Receive Complete Flag, Read Only
													// | | | Auswahl Transmit Interrupt: 0= no Data in TB (TI = 1), 1= Transmission complete (TXEPT = 1)
													// | | Continious Receive Mode Enable Bit: 0= Disable to be to be entered
													// | Data Logic: 0= not inversed
													// nur für Spezialmode 3 und 5
		
			RE_BIT_S3 = 1;		// Empfang erlauben 																						
			
			break;
	}
}

/* Empfang sperren */
void RxEnde ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			RE_BIT_S3 = 0;						// Empfang sperren, löschen der Fehlerbits
			DISABLE_IRQ			
			RIC_S3 = 0x00;						// Disable Receive Interrupt 
			ENABLE_IRQ	
		
			P_TOUTS_S3	= 0;					// Stop	 Telegramm-Timeout	Timer Prescaler
			C_TOUTS_S3	= 0;					// Stop	 Telegramm-Timeout	Timer Counter
		
			break;
	}	
}


void U_Mbus_ISR_BaudCheck ( char port )
{
	
}


void U_Mbus_ISR_Tel_Tout ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			P_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Prescaler
			C_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Counter
			RxInit ( port );	

			break;
	}
}


void U_Mbus_ISR_Tx_Int ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			if ( ++MbusTxCtr < MbusTxLng )	
				TB_S3	=	*( pMbusTx + MbusTxCtr );		// nächstes Byte	senden
			else
			{
				TE_BIT_S3		= 0;							// Sendeerlaubnis sperren
				TIC_S3 			= 0x00;						// Disable	Interrupt	Request
	
				// Timeout setzen	für	Verzögerung	ca.	2	ms	
				P_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Prescaler
				C_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Counter
				P_TOUTR_S3	=	64;								// Load  Prescaler
				C_TOUTR_S3	=	58;								// Load  Counter
				P_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Prescaler
				C_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Counter
			}
			break;
	}	
}


void U_Mbus_ISR_Rx_Int ( char port )
{
	char cErr = 0;
	char cIn = 0;
	
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			cErr = SUM_BIT_S3;									// Summenfehlerbit gesetzt ?
			cIn = RB_S3;										// Empfangsbyte	abholen
			break;
	}
	
	if ( cErr == TRUE )
	{
		RxStatus = RX_ERR;
		RxEnde ( port );		
	}
	else
	{
		*( pMbusRx + MbusRxCtr ) = cIn;						// Zeichen in Puffer
		MbusRxCtr++;
		if ( MbusRxCtr == MbusRxLng )
		{
			if ( MbusRxCtr == 1 )
			{
				if ( cIn == 0xE5 )										// Single Character
				{				
					RxStatus = RX_ENDE;
					RxEnde ( port );
				}
				else if ( cIn == 0x10 || cIn == 0x68 )
					MbusRxLng = 5;											// Länge des Short Frame
				else
				{				
					RxStatus = RX_ERR;
					RxEnde ( port );
				}				
			}
			else if ( MbusRxCtr == 5 )
			{
				if ( *(pMbusRx) == 0x10 )
				{				
					RxStatus = RX_ENDE;									// Short Frame empfangen
					RxEnde ( port );
				}
				else 
				{																			// Länge des erwarteten Long Frame ermitteln
					MbusRxLng = (char)( *(pMbusRx+1) + 6 );	
					if ( *(pMbusRx+2) > *(pMbusRx+1) )			
						MbusRxLng = (char)( *(pMbusRx+1) + 6 );					
				}	
			}
			else				
			{				
				RxStatus = RX_ENDE;										// Long Frame empfangen
				RxEnde ( port );
			}
		}				
	}
}


void WriteForLT ( char mbctr )
{
	char i;
	ULONG f;
		 
	if ( MbusPara[mbctr].Typ == WRM || MbusPara[mbctr].Typ == KLM )
	{
		mbZaehler[mbctr].zdim.exponent = 6 - MbusData[mbctr].komma.wmeng;		// ergibt den Faktor zur Berechnung in Basiseinheit Wh
		mbZaehler[mbctr].zdim.medium = WAERMEMENGE;
		
		for ( i = 0, f = 1; i < mbZaehler[mbctr].zdim.exponent; i++ )
			f *= 10;
		mbZaehler[mbctr].zwert = (unsigned long long)MbusData[mbctr].wmeng * f;
		
		if ( MbusData[mbctr].komma.wmeng > 3 )
		{
			mbZaehler[mbctr].zdim.komma = MbusData[mbctr].komma.wmeng - 3; 
			mbZaehler[mbctr].zdim.dim_idx	= 5;			// kWh mit Komma (komma = 4 -> 1 Kommastelle)
		}	
		else
		{
			mbZaehler[mbctr].zdim.komma = MbusData[mbctr].komma.wmeng; 
			mbZaehler[mbctr].zdim.dim_idx	= 4;			// MWh mit Komma (komma = 2/3 -> 2/3 Kommastellen)
		}									
	}
	
	else if ( MbusPara[mbctr].Typ == KWS || MbusPara[mbctr].Typ == WWS )
	{
		mbZaehler[mbctr].zdim.exponent = 3 - MbusData[mbctr].komma.volume;		// ergibt den Faktor zur Berechnung in Basiseinheit l
		mbZaehler[mbctr].zdim.medium = ( MbusPara[mbctr].Typ == KWS ) ? KALTWASSER : WARMWASSER;		
		for ( i = 0, f = 1; i < mbZaehler[mbctr].zdim.exponent; i++ )
			f *= 10;
		mbZaehler[mbctr].zwert = (unsigned long long)MbusData[mbctr].volume * f;
		mbZaehler[mbctr].zdim.komma = MbusData[mbctr].komma.volume;
		mbZaehler[mbctr].zdim.dim_idx	= 6;				// cbm mit Komma 
	}

	else if ( MbusPara[mbctr].Typ == GMB )
	{
		mbZaehler[mbctr].zdim.exponent = 3 - MbusData[mbctr].komma.volume;		// ergibt den Faktor zur Berechnung in Basiseinheit l
		mbZaehler[mbctr].zdim.medium = GMB;			
		for ( i = 0, f = 1; i < mbZaehler[mbctr].zdim.exponent; i++ )
			f *= 10;
		mbZaehler[mbctr].zwert = (unsigned long long)MbusData[mbctr].volume * f;
		mbZaehler[mbctr].zdim.komma = MbusData[mbctr].komma.volume;
		mbZaehler[mbctr].zdim.dim_idx	= 6;				// cbm mit Komma 
	}

	else if ( MbusPara[mbctr].Typ == ELT )
	{
		mbZaehler[mbctr].zdim.exponent = 3;
		mbZaehler[mbctr].zdim.medium = ELEKTRO;	
		mbZaehler[mbctr].zwert = (unsigned long long)MbusData[mbctr].elektro * 1000;
		mbZaehler[mbctr].zdim.komma = 0;
		mbZaehler[mbctr].zdim.dim_idx	= 5;				// kWh 							
	}		
}


void AktualisVerbrauch ( char mbctr )
{
	if ( MbusPara[mbctr].Typ == WRM 
			|| MbusPara[mbctr].Typ == KWS || MbusPara[mbctr].Typ == WWS 
			|| MbusPara[mbctr].Typ == ELT || MbusPara[mbctr].Typ == KLM || MbusPara[mbctr].Typ == GMB )
	{
		MbusData[mbctr].kommaLT = mbZaehler[mbctr].zdim.komma;
		MbusData[mbctr].dim_idx = mbZaehler[mbctr].zdim.dim_idx;
		if ( MbusData[mbctr].zwertAkt > 0 )
		{
			if ( mbZaehler[mbctr].zwert > MbusData[mbctr].zwertAkt )
				mbZaehler[mbctr].vwert += (ULONG)( mbZaehler[mbctr].zwert - MbusData[mbctr].zwertAkt );
			else if ( mbZaehler[mbctr].zwert < MbusData[mbctr].zwertAkt )
				#if MBUS_MASTER == 0
				mbZaehler[mbctr].vwert = 0;
				#else
				mbZaehler[mbctr].vwert = mbZaehler[mbctr].verbr_akt[Mon-1];
				#endif	
			
			#if MBUS_MASTER == 1
			mbZaehler[mbctr].verbr_akt[Mon-1] = mbZaehler[mbctr].vwert;		
			mbZaehler[mbctr].syncv = mbZaehler[mbctr].vwert;		
			#endif			
		}
		MbusData[mbctr].zwertAkt = mbZaehler[mbctr].zwert;		
	}		
}

#if MBUS_MASTER == 1

void ClearVerbrauch ( void )
{
	char i;
	
	for ( i = 0; i < MBUSANZ; i++ )
		mbZaehler[i].vwert = 0;	
}

void Stichtag ( void )
{
	char i;
	ULONG ulZeitsoll = 0;
	zaehlsp *pMbusZaehler = &mbZaehler[0];
			
	for ( i = 0; i < MBUSANZ; i++, pMbusZaehler++ )
	{
		if ( ( pMbusZaehler->zdim.konfig & Z_KONFIG_STICHTAG ) == Z_KONFIG_STICHTAG )
		{
			ulZeitsoll = time2long ( 0, 0, 0, pMbusZaehler->zdim.stich_tag[2], pMbusZaehler->zdim.stich_tag[1], pMbusZaehler->zdim.stich_tag[0] );
		 // stich_tag [0]=Jahr [1]=Monat [2]=Tag		
			if ( ulZeitsoll != 0 && ulZeitsoll <= time2long ( 0, 0, 0, Tag, Mon, Jahr ) )
			{	// der Stichtag ist heute oder in der Vergangenheit (R36 war ausgeschaltet oder nach Kaltstart) 
				// 1. Kopieren des aktuellen Zählwertes auf den Vorjahreszählwert
				pMbusZaehler->zwert_vj = pMbusZaehler->zwert;
				pMbusZaehler->syncw_vj = pMbusZaehler->zwert;
				// 2. Stichtagsjahr erhöhen
				if ( pMbusZaehler->zdim.stich_tag[1] > Mon || ( pMbusZaehler->zdim.stich_tag[1] == Mon && pMbusZaehler->zdim.stich_tag[2] > Tag ) )
					pMbusZaehler->zdim.stich_tag[0]	= Jahr;		// nach Kaltstart: der programmierte Stichtag kommt erst noch 
				else
					pMbusZaehler->zdim.stich_tag[0]	= Jahr + 1;
				
				LetzterStichtag [0] = Jahr; LetzterStichtag [1] = Mon; LetzterStichtag [2] = Tag;
			}
		}		
	}
}


void Jahreswechsel ( void )
{
	char i;
	zaehlsp *pMbusZaehler = &mbZaehler[0];
			
	for ( i = 0; i < MBUSANZ; i++, pMbusZaehler++ )
	{
		memcpy ( (char *)&pMbusZaehler->verbr_vor, (char *)&pMbusZaehler->verbr_akt, 48 );												// im Ram: aktuell nach Vorjahr kopieren
		memset ( (char *)&pMbusZaehler->verbr_akt, 0, 48 );																					// im Ram: aktuelles Jahr löschen
	}
}

#endif		// #if MBUS_MASTER == 1


// Zeit der Erfassung der letzten Zählwert-Änderung oder des Fehlers bei der Abfrage setzen
void SetzeZeit ( char ctr, char err )
{
#if MBUS_MASTER == 1

	if ( err != 0 )
	{			
		if ( Z_Fehler[ctr].firstErrTime.jahr == 0 )
		{																			// erstes Auftreten
			Z_Fehler[ctr].firstErrTime.tag = Tag;					
			Z_Fehler[ctr].firstErrTime.monat = Mon;					
			Z_Fehler[ctr].firstErrTime.jahr = Jahr;
			Z_Fehler[ctr].firstErrTime.std = Std;					
			Z_Fehler[ctr].firstErrTime.min = Min;					
			Z_Fehler[ctr].firstErrTime.sec = Sek;	
			Z_Fehler[ctr].firstErr = err;				
		}

		Z_Fehler[ctr].lastErrTime.tag = Tag;					
		Z_Fehler[ctr].lastErrTime.monat = Mon;					
		Z_Fehler[ctr].lastErrTime.jahr = Jahr;
		Z_Fehler[ctr].lastErrTime.std = Std;					
		Z_Fehler[ctr].lastErrTime.min = Min;					
		Z_Fehler[ctr].lastErrTime.sec = Sek;	
		Z_Fehler[ctr].lastErr = err;				
	}
																				// Abfrage ok, Wert geändert	
	else
	{
		Z_Fehler[ctr].ok1Time.tag = Tag;					
		Z_Fehler[ctr].ok1Time.monat = Mon;					
		Z_Fehler[ctr].ok1Time.jahr = Jahr;
		Z_Fehler[ctr].ok1Time.std = Std;					
		Z_Fehler[ctr].ok1Time.min = Min;					
		Z_Fehler[ctr].ok1Time.sec = Sek;	

		Z_Fehler[ctr].ok2Time.tag = Tag;					
		Z_Fehler[ctr].ok2Time.monat = Mon;					
		Z_Fehler[ctr].ok2Time.jahr = Jahr;
		Z_Fehler[ctr].ok2Time.std = Std;					
		Z_Fehler[ctr].ok2Time.min = Min;					
		Z_Fehler[ctr].ok2Time.sec = Sek;	
	}

#else
	if ( err != 0 )
	{
		if ( ( mbZaehler[ctr].ucZstatus & FEHLER_GESICHERT ) == 0 )		
		{																			// erstes Auftreten eines Fehlers
			MbusTxBuf[0] = Tag;					
			MbusTxBuf[1] = Mon;					
			MbusTxBuf[2] = Jahr;
			MbusTxBuf[3] = Std;					
			MbusTxBuf[4] = Min;					
			MbusTxBuf[5] = Sek;	
			MbusTxBuf[6] = err;	
			MbusTxBuf[7] = Tag;					
			MbusTxBuf[8] = Mon;					
			MbusTxBuf[9] = Jahr;
			MbusTxBuf[10] = Std;					
			MbusTxBuf[11] = Min;					
			MbusTxBuf[12] = Sek;	
			MbusTxBuf[13] = err;
			bicbus ( EEPADR,	MbusTxBuf, ZMB1EF_ADR + ctr*ZFEHLER_OFFS, 14,	BICWR );	
			mbZaehler[ctr].ucZstatus |= FEHLER_GESICHERT; 
		}
		else
		{
			MbusTxBuf[0] = Tag;					
			MbusTxBuf[1] = Mon;					
			MbusTxBuf[2] = Jahr;
			MbusTxBuf[3] = Std;					
			MbusTxBuf[4] = Min;					
			MbusTxBuf[5] = Sek;	
			MbusTxBuf[6] = err;	
			bicbus ( EEPADR,	MbusTxBuf, ZMB1LF_ADR + ctr*ZFEHLER_OFFS, 7,	BICWR );	
		}
	}
	else
	{
		MbusTxBuf[0] = Tag;					
		MbusTxBuf[1] = Mon;					
		MbusTxBuf[2] = Jahr;
		MbusTxBuf[3] = Std;					
		MbusTxBuf[4] = Min;					
		MbusTxBuf[5] = Sek;	
		MbusTxBuf[6] = Tag;					
		MbusTxBuf[7] = Mon;					
		MbusTxBuf[8] = Jahr;
		MbusTxBuf[9] = Std;					
		MbusTxBuf[10] = Min;					
		MbusTxBuf[11] = Sek;	
		bicbus ( EEPADR,	MbusTxBuf, ZMB1LH_ADR + ctr*ZFEHLER_OFFS, 12,	BICWR );	
	}			

#endif
}				


#endif		// MBUSANZ

#endif		// Implementierung der Schnittstelle
