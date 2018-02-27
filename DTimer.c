//*********************************************************************
// Task: DTimer
//*********************************************************************
// Timer zum Datenaustausch zwischen einem Master und maximal 4 Slaves.
// Dieser Task wird im R66-Master implementiert (DM_MODE 1). 

// Die Sendepuffer    werden vom User (in DManager.c) gefüllt.
// Die Empfangspuffer werden vom User (in DManager.c) ausgewertet.
// Eine serielle Schnittstelle ist als Master einzustellen (9600 Baud).

// Automatisch ablaufende Zusatzaufträge sind:
// - Lese  Außentemperatur von allen Slaves (Auswertung in Steuer.c)
// - Sende Broadcast-Außentemperatur
// - Sende Broadcast-Uhr    29.11.2016

#include <string.h>
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "constext.h"
#include "standdef.h"
#include "kommando.h"	

// states
#define DT_ANF								0
#define DT_DAT_START					1
#define DT_DAT_AUSW						2
#define DT_NEXT_AUFTR 				3
#define DT_NEXT_SLAVE					4
#define DT_LAST_AUFTRAG				5
#define DT_NACH_AUFTRAG				6
#define DT_LAST_AUFTRAG_AUSW	7
#define DT_END								8

#define DT_AUFTRAG_MAX	2

#if DM_MODE == 1

char Start_dt  (char slave, char dtauftrag);
char Auswert_dt(char slave, char dtauftrag);

// **** Task-Beginn **************
void DTimer(void)
{
	static char neustart = 1;
	char err = 0;
	char ret = 0;
	char ready = 0;
	char i;
	
	if(neustart)
	{	
		dtstate = DT_ANF;
		dtslave = 0;
		dtauftrag = 1;
		neustart = 0;
	}	

	switch(dtstate)
	{
		case DT_ANF:
			if(DM[dtslave].aktiv == 1)							// von DManager aktiviert ?
				dtstate = DT_DAT_START;
			else
				dtstate = DT_NEXT_SLAVE;
			break;
			
		case DT_DAT_START:
			ret = Start_dt(dtslave, dtauftrag);
			if(ret == 1)														// Auftrag angenommen ?
			{ 
				dtstate = DT_DAT_AUSW;								// ja
				DM[dtslave].ready = 1;								// ausgeführt
			}	
			else
				dtstate = DT_NEXT_SLAVE;							// nein	
			break;
			
		case DT_DAT_AUSW:
			if(vec[VEC_TI].request ==	0)						// Auftrag beendet ?
			{	if(proc_TI > 0)												// war Auftrag ausgelöst worden ?
				{	
					proc_TI = 0;
					err = Auswert_dt(dtslave, dtauftrag);			// ja, auswerten
				}
				dtstate = DT_NEXT_AUFTR;
			}
			break;
			
		case DT_NEXT_AUFTR:
			if(++dtauftrag > DT_AUFTRAG_MAX)				// Alle Aufträge ausgeführt ?
			{	
				dtstate = DT_NEXT_SLAVE;							// ja	
				DM[dtslave].aktiv = 0;
			}	
			else
				dtstate = DT_DAT_START;								// nächsten Auftrag ausführen
			break;
		
		case DT_NEXT_SLAVE:
			dtauftrag = 1;
			if(++dtslave >= DM_SLAVES)							// Alle Slaves abgearbeitet
			{
				dtslave = 0;
				ready = 0;
				for(i = 0; i < DM_SLAVES; i++)				// Untersuchen, ob wenigstens ein Auftrag ausgeführt wurde
					ready |= DM[i].ready;
				
				if(ready)	
					dtstate = DT_LAST_AUFTRAG;					// Broacast Ta senden
				else
					dtstate = DT_ANF;
			}	
			else	
				dtstate = DT_ANF;
			break;
			
		case DT_LAST_AUFTRAG:															// Broacast Ta senden
			if(Start_dt(dtslave, DT_AUFTRAG_MAX + 1) == 1)	// Auftrag angenommen ?
				dtstate = DT_NACH_AUFTRAG;
			else
				dtstate = DT_END;
			break;	
		
		case DT_NACH_AUFTRAG:															// Broacast Uhr senden
			if(Start_dt(dtslave, DT_AUFTRAG_MAX + 2) == 1)	// Auftrag angenommen ?
				dtstate = DT_LAST_AUFTRAG_AUSW;
			else
				dtstate = DT_END;
			break;	

		case DT_LAST_AUFTRAG_AUSW:
			if(vec[VEC_TI].request ==	0)						// Auftrag beendet ?
			{	if(proc_TI > 0)												// war Auftrag ausgelöst worden ?
				{	
					proc_TI = 0;
				}	
				dtstate = DT_END;
			}
			for(i = 0; i < DM_SLAVES; i++)
				DM[i].ready = 0;											// ready löschen
			
			break;								

		case DT_END:
		default:
			dtslave = 0;
			dtauftrag = 1;
			dtstate = DT_ANF;
			break;
	}
}		// Task-Ende

//***********************************************************************************
// Unterprogramme
//***********************************************************************************

// Datenübertragung starten
char Start_dt(char slave, char dtauftrag)
{
	char dtleng = 0;
	char ret  = 1;
	char flag = 0;
	char master = 0;
	
	switch(dtauftrag)
	{
		case 1:								// DManager Sendeauftrag
			dtleng = DM[slave].TxLeng;											// Datenanzahl (reine Daten ohne das Anzahlbyte dtleng)
			vec[VEC_TI].buffer[0] =	DM[slave].SlaveAdr;			// R66-Adresse
			vec[VEC_TI].buffer[1] = dtleng + 3;							// Leng: Kommando + dtleng + reine Daten
			vec[VEC_TI].buffer[2] = R35_KOM;								// Primärkommando:	0xAC
			vec[VEC_TI].buffer[3] = RW_DMANAGER;						// Kommando:				0x24
			vec[VEC_TI].buffer[4] = dtleng;									// Datenanzahl
			if(dtleng > 0)
				memcpy(&vec[VEC_TI].buffer[5], &DM[slave].TxBuf[0], dtleng + 1);	// +1 dtleng
			break;
			
		case 2:								// Lese Außentemperatur
			// AC 0B                    		
			vec[VEC_TI].buffer[0] =	DM[slave].SlaveAdr;			// R66-Adresse
			vec[VEC_TI].buffer[1] = 2;											// Leng: Kommando + Daten
			vec[VEC_TI].buffer[2] = R35_KOM;								// Primärkommando:	0xAC
			vec[VEC_TI].buffer[3] = RD_TA1;									// Kommando:				0x0B
			break;
			
		case 3:								// LAST_AUFTRAG  sende Broadcast TA
			vec[VEC_TI].buffer[0] =	BROADC;									// Broadcast-Adresse 0xFC
			vec[VEC_TI].buffer[1] = 8;											// Leng: Kommando + Daten
			vec[VEC_TI].buffer[2] = BCTA1;									// Kommando					 0x21
			if ( ta.stat == UEBERL || ta.stat == UNTERL || ta.stat == NICHTV )	// Über- oder Unterlauf ? 170102: oder NICHTV !
				*(int*)&vec[VEC_TI].buffer[3] = -501;					// -50,1 °C als Fehlermeldung
    	else
    		*(int*)&vec[VEC_TI].buffer[3] = ta.messw;
    	vec[VEC_TI].buffer[5] = 30;											// 30 Minuten Countdown
			// ab 19.07.2011
	  	vec[VEC_TI].buffer[6] = CoolBc; 
	  	vec[VEC_TI].buffer[7] = CoolBcStat; 
	  	vec[VEC_TI].buffer[8] = HeatBc; 
	  	vec[VEC_TI].buffer[9] = HeatBcStat; 
    	break;	
			
		case 4:								// NACH_AUFTRAG  Sende Broadcast - Uhr
			vec[VEC_TI].buffer[0] =	BROADC;									// Broadcast-Adresse 0xFC
			vec[VEC_TI].buffer[1] = 8;											// Leng: Kommando + Daten
			vec[VEC_TI].buffer[2] = BCUHR;									// Kommando:				 0x20
			vec[VEC_TI].buffer[3] = Sek;
			vec[VEC_TI].buffer[4] = Min;
			vec[VEC_TI].buffer[5] = Std;
			vec[VEC_TI].buffer[6] = Tag;
			vec[VEC_TI].buffer[7] = Mon;
			vec[VEC_TI].buffer[8] = Jahr;
			vec[VEC_TI].buffer[9] = Wotag;
			break;

		default:
			ret = 0;	// Error	
	}
	
	if(ret == 1)
	{
		// Vektor laden
		// gibt es	einen	Master mit Gebäudebusfunktion ?
		if(      (Mode_S1	== MASTER	&& Funktion_S1 == GBUS_FU) )
			master = MASTER_S1;
		else if( (Mode_S2	== MASTER	&& Funktion_S2 == GBUS_FU) )	
			master = MASTER_S2;
		else if( (Mode_S3	== MASTER	&& Funktion_S3 == GBUS_FU  && spl_kennung == SPL_RS485) )	
			master = MASTER_S3;

		if(master > 0)
		{
			if(dtauftrag == 3)
				vec[VEC_TI].request = 0x81;					// Broadcastflag + 1 Versuch
			else
				vec[VEC_TI].request = 0x01;					// 1 Versuch 
			
			vec[VEC_TI].master  = master;
			proc_TI				 			= 1;							// Auftrag merken
		}
		else
			ret = 0;															// Error	
	}
	
	return(ret);	
}

// Datenübertragung auswerten
char Auswert_dt(char slave, char dtauftrag)
{
	char i, rc, rc1;	
	char err = 0;
	char leng = 0;
	
	// Telegrammauswertung
	rc	= vec[VEC_TI].rcode;										// Returncode	des	Handlers
	rc1 = vec[VEC_TI].buffer[2];								// Returncode des Teilnehmers
	if((rc == RC_OK) && (rc1 == RC_OK))
	{
		switch(dtauftrag)
		{
			case 1:																											// Empfangsdaten vom DManager Sendeauftrag
				leng = vec[VEC_TI].buffer[3];															// 1. Datenbyte gibt die Anzahl der Daten an
				if(leng > 0 && leng <= DM_RXBUF_MAX)
				{
					for(i = 0; i < leng; i++)
						DM[slave].RxBuf[i] = vec[VEC_TI].buffer[4 + i];				// Nur Nutzdaten werden kopiert, Auswertung in DManager
					
					DM[slave].RxTout = 10; 	// Minuten
				}
				else
					DM[slave].RxTout = 0;		// Fehler NICHTV
				break;
				
			case 2:																											// Lese Außentemperatur
				DM[slave].rdta.messw = *(int*)&vec[VEC_TI].buffer [3];		// Temperatur
				DM[slave].rdta.stat  = vec[VEC_TI].buffer [5];						// Temperaturstatus
				DM[slave].rdta_bc		 = vec[VEC_TI].buffer [7];						// bc_flag
				DM[slave].rdta_flag  = 1;																	// Flag für erfolgte Datenübertragung
				break;
				
			case 3:																											// Sende Broadcast-Ta
				break;			
		}
	}
	else
		err = 1;
		
		
	return(err);
}
	
#endif // DM_MODE == 1

	