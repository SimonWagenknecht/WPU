//*********************************************************************
// Programm: DManager
//*********************************************************************

// Vorbereitung zur Ausf�hrung des Kommandos RW_DMANAGER:
// Das Kommando dient zum Senden von Daten an adressierbare Slaves und zum Auswerten der Daten von den Slaves.
// Der Regler befindet sich im DM_Modus (Data Master)

// Das Programm kann als Funktion aus einem vorhandenen Task (z.B. Steuer.c) aufgerufen werden.

// Beteiligte Quellen:
//   in DTimer.c:      Organisation der Daten�bertragung
//   in UserRam.c:     Variablen f�r DM_Modus (Data Master)
//   in ustruct.h:     DM Struktur und Funktionsprototypen
//   in projdef.h:     Definitionen

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "standdef.h"
#include "projdef.h"
#include "konvform.h"

#if DM_MODE == 1

char DM_Fill_TxBuf (char slave, char offs, char *wert,  char konv);
char DM_Empty_RxBuf(char slave, char offs, mwsp *param, char konv);

void DManager(void)
{
	char offs, idx;
	static char neustart = 1;
	
	if(neustart == 1)
	{
		DM_CountDown();
		neustart = 0;
	}	
	
	//*******************************
	// 1. Slave 	Sendepuffer f�llen
	//*******************************
	idx  = 0;																					// Slave-Index
	// Erster Arbeitsschritt: muss sein !
	DM[idx].TxLeng = 0;																// Anzahl der Bytes, wird sp�ter korrigiert
	DM[idx].SlaveAdr = Slave[idx];										// Adresse
	if(Slave[idx] > 0)																// g�ltig ?
		DM[idx].aktiv = 1;															// Daten�bertragung anregen in DTimer()
	offs = 0;																					// Pufferzeiger

	// folgende User-Eintr�ge k�nnen auch entfallen, wenn der Master keine Daten sendet, sondern nur Daten holt 
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Beispiel: Zustand EIN/AUS						(Heizkreis1-Pumpe)
	//offs = DM_Fill_TxBuf(idx, offs, P&PU[HK1], JANEIN_FORMOP);	// Anmerkung: Wert und Format sind aus der parli kopiert
	
	// Beispiel: Ist-Temperatur mit Status  (Vorlauf Prim�r)
	//offs = DM_Fill_TxBuf(idx, offs, P&TVP[ANL], ANA_FORMP);
	
	// Beispiel: Soll-Temperatur						(Vorlauf Max)
	//offs = DM_Fill_TxBuf(idx, offs, P&hks[HK1].Tvma, US_INT);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Letzter Arbeitsschritt: muss sein !
	DM[idx].TxLeng = offs;	
	 		
	//*******************************
	// 2. Slave 	Sendepuffer f�llen
	//*******************************
	#if	DM_SLAVES > 1
	idx = 1;
	// Erster Arbeitsschritt: muss sein !
	DM[idx].TxLeng = 0;																// Anzahl der Bytes, wird sp�ter korrigiert
	DM[idx].SlaveAdr = Slave[idx];										// Adresse
	if(Slave[idx] > 0)																// g�ltig ?
		DM[idx].aktiv = 1;															// Daten�bertragung anregen in DTimer()
	offs = 0;																					// Pufferzeiger
	// folgende User-Eintr�ge k�nnen auch entfallen, wenn der Master keine Daten sendet, sondern nur Daten holt 
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Beispiel: Zustand EIN/AUS						(Heizkreis2-Pumpe)
	//offs = DM_Fill_TxBuf(idx, offs, P&PU[HK2], JANEIN_FORMOP);	// Anmerkung: Wert und Format sind aus der parli kopiert
	
	// Beispiel: Ist-Temperatur mit Status  (R�cklauf Prim�r)
	//offs = DM_Fill_TxBuf(idx, offs, P&TRP[ANL], ANA_FORMP);
	
	// Beispiel: Soll-Temperatur						(Vorlauf Min)
	//offs = DM_Fill_TxBuf(idx, offs, P&hks[HK1].Tvmi, US_INT);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Letzter Arbeitsschritt: muss sein !
	DM[idx].TxLeng = offs;	
	#endif
	
	//-------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------

	//************************************
	// 1. Slave 	Empfangsdaten behandeln
	//************************************
	idx  = 0;		// Slave-Index
	offs = 0;		// Pufferzeiger
	// Empfangene Daten werden grunds�tzliche auf Variablen vom Typ "mwsp" geladen und angezeigt.
	// Die Formatangabe entscheidet �ber die Anzahl der entnommenen Bytes aus dem Empfangspuffer,
	// nicht �ber die Darstellung in der parli  
	
	// folgende User-Eintr�ge k�nnen auch entfallen, wenn der Master keine Daten braucht, sondern nur welche sendet 
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Beispiel: Zustand EIN/AUS								(Sommerbetrieb)
	//offs = DM_Empty_RxBuf(idx, offs, &station1_ea, JANEIN_FORM);				// 1 Byte	
																														
	// Beispiel: Ist-Temperatur mit Status  		(Vorlauftemperatur)
	//offs = DM_Empty_RxBuf(idx, offs, &station1_istwert, ANA_FORM);			// 3 Byte

	// Beispiel: Soll-Temperatur								(berechn. Vorlauf)
	//offs = DM_Empty_RxBuf(idx, offs, &station1_sollwert, US_INT);			// 2 Byte

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//************************************
	// 2. Slave 	Empfangsdaten behandeln
	//************************************
	#if	DM_SLAVES > 1
	idx  = 1;		// Slave-Index
	offs = 0;		// Pufferzeiger

	// folgende User-Eintr�ge k�nnen auch entfallen, wenn der Master keine Daten braucht, sondern nur welche sendet 
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Beispiel: Zustand EIN/AUS								(Sommerbetrieb)
	//offs = DM_Empty_RxBuf(idx, offs, &station2_ea, JANEIN_FORM);				// 1 Byte	
																														
	// Beispiel: Ist-Temperatur mit Status  		(Vorlauftemperatur)
	//offs = DM_Empty_RxBuf(idx, offs, &station2_istwert, ANA_FORM);			// 3 Byte

	// Beispiel: Soll-Temperatur								(berechn. Vorlauf)
	//offs = DM_Empty_RxBuf(idx, offs, &station2_sollwert, US_INT);			// 2 Byte

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	#endif
	
}

//***************************************************************************************************************
// Unterprogramme
//***************************************************************************************************************

// Countdown Empfangsdaten
// wird bei Neustart und von Check_CountDown() min�tlich aufgerufen
void DM_CountDown(void)
{
	char i;
	
	for(i = 0; i < DM_SLAVES; i++)
	{
		if(DM[i].RxTout > 1)
		{	
			DM[i].RxStat = 0;						// OK
			if(--DM[i].RxTout == 1)
			{	DM[i].RxStat    = DATERR;
				DM[i].rdta_flag = 0;
			}	
		}
		if(DM[i].RxTout == 0)
			DM[i].RxStat = NICHTV;
	}		
}


// UP F�llen Sendepuffers mit Daten f�r den Slave
char 	DM_Fill_TxBuf(char slave, char offs, char *wert, char konv)
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
			if(offs <= DM_TXBUF_MAX - 1)
			{	
				DM[slave].TxBuf[offs] = charwert;
				offs++;
			}	
			ret = offs;
			break;
		
		case US_INT:
		case  S_INT:
			intwert = *(int *)wert;
		intload:
			if(offs <= DM_TXBUF_MAX - 2)
			{	
				*(int *)&DM[slave].TxBuf[offs] = intwert;
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
			if(offs <= DM_TXBUF_MAX - 3)
			{
				*(int *)&DM[slave].TxBuf[offs] = intwert;
				offs += 2;
				DM[slave].TxBuf[offs] =	charwert;
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
			charwert = pAASP->astat;							// Messwertstatus
			goto messwertload;
					
				
	}
	return(ret);		
}
	
// UP Entleeren des Empfangspuffers mit Daten vom Slave an den Master
char DM_Empty_RxBuf(char slave, char offs, mwsp *param, char konv)
{
	char charwert;
	int  intwert;
	char ret = 0;

	switch(konv)
	{
		case US_CHAR:
		case  S_CHAR:
		case JANEIN_FORM:
			charwert = DM[slave].RxBuf[offs];								// 1 Byte aus dem Empfangspuffer
			offs++;
			param->messw = (int)charwert;										// Parameterwert
			param->stat  = DM[slave].RxStat;								// Daten�bertragungsstatus
			ret = offs;
			break;
	
		case ANA_FORM:
			param->messw = *(int *)&DM[slave].RxBuf[offs];	// 2 Byte Temperatur aus dem Empfangspuffer
			offs += 2;
			param->stat  = DM[slave].RxBuf[offs];						// 1 Byte Temperaturstatus aus dem Empfangspuffer
			offs += 1;
			// wenn der Daten�bertragungsstatus nicht in Ordnung ist, wird der Temperaturstatus �berschrieben
			if(DM[slave].RxStat != 0)
				param->stat = DM[slave].RxStat;
			ret = offs;
			break;
			
		case US_INT:
		case  S_INT:
			param->messw = *(int *)&DM[slave].RxBuf[offs];	// 2 Byte aus dem Empfangspuffer
			offs += 2;
			param->stat  = DM[slave].RxStat;								// Daten�bertragungsstatus
			ret = offs;
			break;
		
	}
	return(ret);
}
			
#endif	// DM_MODE == 1
					
			
			