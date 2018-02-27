//*********************************************************************
// Programm: DReply
//*********************************************************************

// Auswertung des Kommandos RW_DMANAGER:
// Dieses Kommando dient zum Empfang von Daten vom Master und zum Senden von Daten an den Master zurück.
// Der Regler befindet sich im Modus DS_MODE  (Data Slave)

// Das Programm kann als Funktion aus einem vorhandenen Task (z.B. Alarme.c) aufgerufen werden.

// Beteiligte Quellen:
//   in UserReply.c:   Kommando RW_DMANAGER in User36Reply(...)
//   in UserReply.c:   Timeoutbehandlung in Check_CountDown()
//   in UserRam.c:     Variablen für DS_MODE (Data Slave)
//   in ustruct.h:     Funktionsprototypen
//   in projdef.h:     Definitionen


#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "projdef.h"
#include "konvform.h"

#if DS_MODE == 1

char DS_Empty_RxBuf(char offs, mwsp *param, char konv);
char DS_Fill_TxBuf (char offs, char *wert, char konv);

void DReply(void)
{
	char offs;
	static char neustart = 1;
	
	if(neustart == 1)
	{
		DS_CountDown();
		neustart = 0;
	}	

	//************************************
	// Empfangsdaten behandeln
	//************************************
	offs = 0;		// Pufferzeiger
	// Empfangene Daten werden grundsätzliche auf Variablen vom Typ "mwsp" geladen und angezeigt.
	// Die Formatangabe entscheidet über die Anzahl der entnommenen Bytes aus dem Empfangspuffer,
	// nicht über die Darstellung in der parli  
	
	// folgende User-Einträge können auch entfallen, wenn der Master keine Daten sendet, sondern nur Daten holt 
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Beispiel: Zustand EIN/AUS							(Heizkreis-Pumpe)
	//offs = DS_Empty_RxBuf(offs, &zentrale_ea, JANEIN_FORM);				// 1 Byte	

	// Beispiel: Ist-Temperatur mit Status  	(Vorlauf Primär)
	//offs = DS_Empty_RxBuf(offs, &zentrale_istwert, ANA_FORM);			// 3 Byte
		
	// Beispiel: Soll-Temperatur							(Vorlauf Max)
	//offs = DS_Empty_RxBuf(offs, &zentrale_sollwert, US_INT);			// 2 Byte

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	//-------------------------------------------------
	// Sendepuffer füllen
	//-------------------------------------------------
	// Erster Arbeitsschritt: muss sein !
	DS_TxLeng = 0;																// Anzahl der Bytes, wird später korrigiert
	offs = 0;
	
	// folgende User-Einträge können auch entfallen, wenn der Master keine Daten braucht, sondern nur welche sendet 
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Beispiel: Zustand EIN/AUS		(Sommerbetrieb)
	//offs = DS_Fill_TxBuf(offs, P&hkd[HK1].somme, JANEIN_FORM);	// Anmerkung: Wert und Format sind aus der parli kopiert
 
 	// Beispiel: Ist-Temperatur mit Status  (Vorlauftemperatur)
	//offs = DS_Fill_TxBuf(offs, P&TVS[HK1], ANA_FORMP);
	
	// Beispiel: Soll-Temperatur						(berechn. Vorlauf)
	//offs = DS_Fill_TxBuf(offs, P&hkd[HK1].tvsb,	US_INT);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Letzter Arbeitsschritt: muss sein !
	DS_TxLeng = offs;	
	
	
}

//***************************************************************************************************************
// Unterprogramme
//***************************************************************************************************************

// Countdown Empfangsdaten
// wird bei Neustart und von Check_CountDown() minütlich aufgerufen
void DS_CountDown(void)
{
	if(DS_RxTout > 1)
	{	
		DS_RxStat = 0;						// OK
		if(--DS_RxTout == 1)
			DS_RxStat = DATERR;
	}
	if(DS_RxTout == 0)
		DS_RxStat = NICHTV;
			
}

// UP Entleeren des Empfangspuffers mit Daten vom Master an den Slave
char DS_Empty_RxBuf(char offs, mwsp *param, char konv)
{
	char charwert;
	int  intwert;
	char ret = 0;

	switch(konv)
	{
		case US_CHAR:
		case  S_CHAR:
		case JANEIN_FORM:
			charwert = DS_RxBuf[offs];										// 1 Byte aus dem Empfangspuffer
			offs++;
			param->messw = (int)charwert;									// Parameterwert
			param->stat  = DS_RxStat;											// Datenübertragungsstatus
			ret = offs;
			break;
	
		case ANA_FORM:
			param->messw = *(int *)&DS_RxBuf[offs];				// 2 Byte Temperatur aus dem Empfangspuffer
			offs += 2;
			param->stat  = DS_RxBuf[offs];								// 1 Byte Temperaturstatus aus dem Empfangspuffer
			offs += 1;
			// wenn der Datenübertragungsstatus nicht in Ordnung ist, wird der Temperaturstatus überschrieben
			if(DS_RxStat != 0)
				param->stat = DS_RxStat;
			ret = offs;
			break;
			
		case US_INT:
		case  S_INT:
			param->messw = *(int *)&DS_RxBuf[offs];				// 2 Byte aus dem Empfangspuffer
			offs += 2;
			param->stat  = DS_RxStat;											// Datenübertragungsstatus
			ret = offs;
			break;
		
	}
	return(ret);
}
			
// UP Füllen Sendepuffers mit Daten für den Master
char 	DS_Fill_TxBuf(char offs, char *wert, char konv)
{
	char charwert;
	int  intwert;
	char ret = 0;
	// Pointer
	mwsp						*pMWSP;
	doutsp					*pDOUTSP;
	dinpsp					*pDINPSP;
	aasp						*pAASP;
	
	switch(konv)
	{
		case US_CHAR:
		case  S_CHAR:
		case JANEIN_FORM:
			charwert = wert[0];
		charload:
			if(offs <= DS_TXBUF_MAX - 1)
			{	
				DS_TxBuf[offs] = charwert;
				offs++;
			}	
			ret = offs;
			break;
		
		case US_INT:
		case  S_INT:
			intwert = *(int *)wert;
		intload:
			if(offs <= DS_TXBUF_MAX - 2)
			{	
				*(int *)&DS_TxBuf[offs] = intwert;
				offs += 2;
			}	
			ret = offs;
			break;

		case JANEIN_FORMOP:
			pDOUTSP	=	(doutsp*)*(long*)wert;			// Extrahieren der Adresse,	auf	die	"wert" zeigt
			charwert = pDOUTSP->wert;
			goto charload;
			
		case JANEIN_FORMIP:
			pDINPSP	=	(dinpsp*)*(long*)wert;			// Extrahieren der Adresse,	auf	die	"wert" zeigt
			charwert = pDINPSP->bwert;
			goto charload;
			
		case ANA_FORM:
			intwert = *(int *)wert;
			charwert = wert[2];										// Messwertstatus
		messwertload:
			if(offs <= DS_TXBUF_MAX - 3)
			{
				*(int *)&DS_TxBuf[offs] = intwert;
				offs += 2;
				DS_TxBuf[offs] =	charwert;
				offs++;
			}	
			ret = offs;
			break;

		case ANA_FORMP:
			pMWSP	=	(mwsp*)*(long*)wert;					// Extrahieren der Adresse,	auf	die	"wert" zeigt
			intwert = pMWSP->messw;
			charwert = pMWSP->stat;								// Messwertstatus
			goto messwertload;
		
		case AOUT_FORMP:
			pAASP	=	(aasp*)*(long*)wert;					// Extrahieren der Adresse,	auf	die	"awert" zeigt
			intwert  = pAASP->awert;							// Analogausgabewert
			goto intload;
					
				
	}
	return(ret);		
}

//----------------------------------------------------------------------------------------------
// Kommando zum Austausch von Daten zwischen Master und Slave
//----------------------------------------------------------------------------------------------
void RW_dmanager_reply(char *RsTxBuf, char *RsRxBuf, char prot)
{
	char i, k, offs;
	
	// Empfangene Daten werden kopiert nach DS_RxBuf, zur Auswertung durch die Funktion DReply()
	// Zu sendende Daten werden von DS_TxBuf geholt, gefüllt von der Funktion DReply()
	
	// Daten empfangen
	//------------------------
	k = RsRxBuf[2];													// 1. Byte: Anzahl der Datenbytes
	if(k > 0 && k <= DS_RXBUF_MAX)
	{	
		for(i=0; i<k; i++)
			DS_RxBuf[i] = RsRxBuf[3 + i];
		
		DS_RxTout = 10;												// 10 Minuten TimeOut setzen, wird durch Check_CountDown() überwacht
	}
	else	// keine Daten 
		DS_RxTout = 0;
			
	// Daten senden
	//------------------------
	offs = RsFrame(RsTxBuf,	DS_TxLeng + 1, RCOK, prot);
	RsTxBuf[offs++] = DS_TxLeng;						// Erstes Byte ist die Datenbyteanzahl
	if(DS_TxLeng > 0)
	{
		for(i = 0; i < DS_TxLeng; i++)
			RsTxBuf[offs + i] = DS_TxBuf[i];
	}
}

#endif // DS_MODE == 1