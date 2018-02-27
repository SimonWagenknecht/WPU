// CopyCheck

// Behandlung von Datenelementen, welche über "Komtab-Schreiben" empfangen wurden.

// Das Quellprogramm enthält die Teile:
// 1. CopyCheck:	Task-Funktion zur Gültigkeitsüberwachung
// 2. CopyInit: 	Funktion zum Kopieren ausgesuchter Datenelemente und deren Initialisierung zur Gültigkeitsüberwachung
//								Aufruf erfolgt aus LzKonv.c 


#include "struct.h"
#include "ustruct.h"
#include "uramext.h"
#include "uconstext.h"
#include "userdef.h"

//----------------------------------------------------
// Überwachungs-Task
//----------------------------------------------------
void CopyCheck(void)
{
	static char neustart = 1;
	char i;
	
#if COPY_EMPF > 0
	if ( neustart )
	{
		for ( i = 0; i < COPY_EMPF; i++ )											// Anzahl der über Bus zu empfangenen Datenelemente 
			CopyData[i].copyData.stat = NICHTV; 								// Copy-Status mit NICHTV vorbelegen
		neustart = 0;
	}
	
	else																										// Timer überprüfen
	{
		for ( i = 0; i < COPY_EMPF; i++ )											// Anzahl der über Bus zu empfangenen Datenelemente 
		{
			if ( ++CopyData[i].copyRxTimer == 0xFFFF )
				CopyData[i].copyRxTimer = 0xFFFE;									// Überlauf verhindert			
			if ( CopyData[i].copyFirst && CopyData[i].copyRxTimer >= COPY_TIMEOUT )
				CopyData[i].copyData.stat = DATERR;								// Copy-Status auf DATERR setzen
				
			// projektspezifischer Teil	
			switch ( i )																				// Anzahl case entspr.COPY_EMPF
			{
				case 0:
					anfExt[0].stat = CopyData[i].copyData.stat;
					break;		
				case 1:
//				anfExt[1].stat = CopyData[i].copyData.stat;
					break;								
				case 2:
//				anfExt[2].stat = CopyData[i].copyData.stat;
					break;
				case 3:
//				anfExt[3].stat = CopyData[i].copyData.stat;
					break;
				case 4:
//				anfExt[4].stat = CopyData[i].copyData.stat;
					break;												
				case 5:
//				anfExt[5].stat = CopyData[i].copyData.stat;
					break;												

				// Beispiel für Temperatur mit Statusauswertung
				case 6:																						// Temperatur-Wert	
				case 7:																						// Temperatur-Status
					if(CopyData[i].copyData.stat != 0)							// wenn Copy-Status nicht i.O. (egal, welcher)
						raumtemp[0].stat = CopyData[i].copyData.stat;	// Überschreiben des Ziel-Status
					break;		
			}
		}
	}
#endif


#if COPY_SEND > 0
	for(i = 0; i < COPY_SEND; i++)
	{
		switch(i)
		{
			// Beispiel Temperatur
			case 0:
				CopySend[i] = TVS[HK1]->messw;
				break;
			case 1:	
				CopySend[i] = TVS[HK1]->stat;
				break;
				
			// Beispiel Sollwert
			case 2:
				CopySend[i] = hkd[HK1].tvsb;
				break;
				
			// Beispiel Relais
			case 3:
				CopySend[i] = PU[HK1]->wert;
				break;
				
			// Beispiel Kontakt
			case 4:
				CopySend[i] = STWHK[HK1]->bwert;
				break;

		}		
	}				
#endif

//TEST ############################################################################# TEST
#if COPY_TEST == 1

		if ( anfExt[0].messw != CopyData[0].copyData.messw )
			CopyInit(0, (char *)&CopyData[0].copyData.messw);
		if ( anfExt[1].messw != CopyData[1].copyData.messw )
			CopyInit(1, (char *)&CopyData[1].copyData.messw);
		if ( anfExt[2].messw != CopyData[2].copyData.messw )
			CopyInit(2, (char *)&CopyData[2].copyData.messw);
		if ( anfExt[3].messw != CopyData[3].copyData.messw )
			CopyInit(3, (char *)&CopyData[3].copyData.messw);
		if ( anfExt[4].messw != CopyData[4].copyData.messw )
			CopyInit(4, (char *)&CopyData[4].copyData.messw);
		if ( anfExt[5].messw != CopyData[5].copyData.messw )
			CopyInit(5, (char *)&CopyData[5].copyData.messw);
				
#endif

}


//----------------------------------------------------
// Initialisierung
//----------------------------------------------------
// Die Funktion wird mit jeder Komtab-Schreiboperation aufgerufen
// Filterung erfolgt durch Adressenvergleich
// 
char CopyInit(unsigned int dae_num, char *pDae)
{
	char cReturn = FALSE;

#if COPY_EMPF > 0

	char i;
	
	for ( i = 0; i < COPY_EMPF; i++ )
	{
		if ( pDae == (char *)&CopyData[i].copyData.messw )
		{
			CopyData[i].copyRxTimer = 0;
			CopyData[i].copyFirst = 1;
			CopyData[i].copyData.stat = 0;			
			cReturn = TRUE;
		
		
			// projektspezifischer Teil
			switch ( i )											// Anzahl case entspr.COPY_EMPF
			{
				case 0:
					anfExt[0].messw = CopyData[0].copyData.messw;	
					anfExt[0].stat = CopyData[0].copyData.stat;		
					break;		
//-				case 1:
//-					anfExt[1].messw = CopyData[1].copyData.messw;	
//-					anfExt[1].stat = CopyData[1].copyData.stat;		
//-					break;
//-				case 2:
//-					anfExt[2].messw = CopyData[2].copyData.messw;	
//-					anfExt[2].stat = CopyData[2].copyData.stat;		
//-					break;
//-				case 3:
//-					anfExt[3].messw = CopyData[3].copyData.messw;	
//-					anfExt[3].stat = CopyData[3].copyData.stat;		
//-					break;
//-				case 4:
//-					anfExt[4].messw = CopyData[4].copyData.messw;	
//-					anfExt[4].stat = CopyData[4].copyData.stat;		
//-					break;															
//-				case 5:
//-					anfExt[5].messw = CopyData[5].copyData.messw;	
//-					anfExt[5].stat = CopyData[5].copyData.stat;		
//-					break;

				// Beispiel für Temperatur mit Statusauswertung
				case 6:																										// Temperatur-Wert empfangen
					raumtemp[0].messw	= CopyData[i].copyData.messw;					// Ziel-Messwert überschreiben	
					break;		
				case 7:																										// Temperatur-Status empfangen
					raumtemp[0].stat	= (char)(CopyData[i].copyData.messw);	// Ziel-Status  überschreiben	
					break;


				default:
					break;															
			}	
		}

	}

#endif
	return(cReturn);	
}

		