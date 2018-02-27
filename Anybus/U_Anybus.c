/***************** Any-Bus ***************/
// Diro - 20.09.2011

#include <string.h>
#include "konvform.h"
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "constext.h"
#include "uconstext.h"
#include "unoinitext.h"
#include "standdef.h"
#include "projdef.h"
#include "sfr32C87.h"
#include "sio.h"
#include "timer.h"

//#include "anybustabelle.h"
#include "U_anybus.h"
#include "anybus.h"
#include "abp.h"

#include <stdlib.h>


#if ( ANYBUS_MODUL == 1 )

extern UINT16 uiNumber_of_Data;
extern anybusDataTableRow anybusDataTable[];

void AnyTxInit ( char port );
void AnyRxInit ( char port );
void AnyRxEnde ( char port );
void InitAnyBusDaten(void);
UINT16 RiedelFormatToInstanceAnzahl(UINT8 ucRiedelFormat);
UINT8 HostEndian2NetEndain(void* pDestBuffer, void* pSourceBuffer, UINT8 uiByteAnzahl);

UINT8 GetAnyBusDataTyp(anybusDataTableRow *pDataRow);
UINT8 GetAnyBusDataTypNumbers(anybusDataTableRow *pDataRow);
UINT8 RiedelFormat2AnybusFormat(UINT8 ucRiedelFormat, UINT8 ucRiedelAttribute);
UINT16 RiedelFormat2AnybusAnzahl(UINT8 ucRiedelFormat, UINT8 ucRiedelAttribute);


const unsigned char pucConstStatus[8] = "STATUS ";
long lBaudrate_S3;
unsigned int uiAnybusTxCtr;
unsigned int uiAnybusTxLng;
unsigned int uiAnybusRxCtr;
unsigned int uiAnybusRxLng;
unsigned char *pucAnybusTx;
unsigned char *pucAnybusRx;
unsigned char ucAnyRxStatus;
char carAnybusstatus[16];
char carAnybustyp[16];
char carAnybusIPAdresse[16];
char carAnybusSubnetmask[16];
char carAnybusGateway[16];
unsigned char ucAnybusRxBuf[512];
UINT8 uiarIPAdresse[4];
UINT16 uiMaxInstanceNumber;
UINT16 uiInstanceCount;


//***************************************************
// Aufruf aus UserHandler.c: alle 50 ms 
//***************************************************
char U_Anybus ( char port, char funktion )
{
//***************************************************

	static unsigned char ucNeustart = TRUE;
	// Angabe ob die Initialiserung der Daten erfolgt ist. Init erfolgt nur, wenn das Netwerkmodul erkannt wurde (Bacnet oder Modbus...)
	static unsigned char ucNetWorkIdentifiy = FALSE;
	static unsigned char ucTimeoutCounter = 0;
	static unsigned int uiDatueTestTimer = 60000 /50;


	// vorerst wird nur S3 implementiert
	if ( port == S1 || port == S2 )
		return ( 1 );		

	if ( port == S3 )
	{
		Baudrate_S3 = 11520;
		lBaudrate_S3 = 115200;
		Mode_S3 = MASTER;
	}
	if((ucNetWorkIdentifiy==FALSE)&&(uiNetworkType!=0)) {
		ucNetWorkIdentifiy = TRUE;
		InitAnyBusDaten();
	}

	if ( ucNeustart == TRUE )
	{
//		InitAnyBusDaten();
		RTS_S3			 	= 1;				// Reset an Anybus

		ucTimeoutCounter = 0;
		memcpy( (void *)carAnybusstatus, " ANLAUF        ", 15 );
		memcpy( (void *)carAnybustyp,    " UNBEKANNT     ", 15 );
		uiAnybusTxLng = 19;
		uiAnybusRxLng = 19;
		
//	autoalarm = 1; // Alarmanzeige verhindern
//	serv_cnt = 12000; // Servicecount setzen
//	grupnum = 3;
//	paranum = 1;

		RTS_S3			 	= 0;				// Reset an Anybus beenden
		//...
		pucAnybusTx = AnybusMain(NULL,&uiAnybusTxLng,&uiAnybusRxLng);
		
		AnyTxInit ( port );
		ucNeustart = FALSE;
		ucNetWorkIdentifiy = FALSE;
//		ucAnyRxStatus = ANY_RX_BEGIN;
	}
	
	if(--uiDatueTestTimer<=1) {
		uiDatueTestTimer = 60000 / 50;
		if(++iFreigabe>10) {
			// 10 Minuten lange kein Antwort mehr gekommen auf das Telegramm
			// Neustart auslösen
			ucAnyRxStatus = ANY_RX_EXECPTION;
			iFreigabe = 0;
		}
		
	}
	
	if((ucAnyRxStatus == ANY_RX_TIMEOUT)||(ucAnyRxStatus == ANY_RX_ERR)||(ucAnyRxStatus == ANY_RX_EXECPTION)) {
		// Bei Timeout letzte Sendung wiederholen
		// Wiederholen der letzten Sendung
		if((ucTimeoutCounter++<15)&&(ucAnyRxStatus!=ANY_RX_EXECPTION)) {
			pucAnybusTx = AnybusMain(NULL,&uiAnybusTxLng,&uiAnybusRxLng);
			
			if(pucAnybusTx==NULL) {
				// Fehler beim Empfang, ich antworte nicht
				if(ucAnyRxStatus!=ANY_RX_EXECPTION) {
					ucAnyRxStatus = ANY_RX_ERR;
				}
			} else {
				// Ich habe etwas zu senden
				AnyTxInit ( port );
			}
		}	else {
			ucTimeoutCounter = 0;
			ucNeustart = TRUE;
			AnybusNeustart();

		}
	}
	if(ucAnyRxStatus == ANY_RX_ENDE) {
		// Sendung empfangen
		pucAnybusTx = AnybusMain(pucAnybusRx,&uiAnybusTxLng,&uiAnybusRxLng);
		
		if(pucAnybusTx==NULL) {
			// Fehler beim EMpfang, ich antworte nicht
			if(ucAnyRxStatus!=ANY_RX_EXECPTION) {
				ucAnyRxStatus = ANY_RX_ERR;
			}
		} else {
			// im Normalbetrieb geht es immer hier rein
			ucTimeoutCounter = 0;
			// Ich habe etwas zu senden
			AnyTxInit ( port );
		}
		
	}
	
	return(funktion);
}

void U_Anybus_ISR_BaudCheck ( char port )
{
	
}


void U_Anybus_ISR_Tel_Tout ( char port )
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
			AnyRxEnde ( port );	
			
			ucAnyRxStatus = ANY_RX_TIMEOUT;

			break;
	}

}

// Sendeinterrupt
void U_Anybus_ISR_Tx_Int ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			if ( ++uiAnybusTxCtr < uiAnybusTxLng )	
				TB_S3	=	*( pucAnybusTx + uiAnybusTxCtr );		// nächstes Byte	senden
			else
			{
				TE_BIT_S3		= 0;							// Sendeerlaubnis sperren
				TIC_S3 			= 0x00;						// Disable	Interrupt	Request
				ucAnyRxStatus = ANY_TX_ENDE;
				AnyRxInit(port);
	
			}
			break;
	}	
}
		
void U_Anybus_ISR_Rx_Int ( char port )
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
	
	if ( /*cErr ==*/ !TRUE )
	{
		AnyRxEnde ( port );		
		ucAnyRxStatus = ANY_RX_ERR;
	}
	else
	{
		*( pucAnybusRx + uiAnybusRxCtr ) = cIn;						// Zeichen in Puffer
		uiAnybusRxCtr++;
		if ( uiAnybusRxCtr == uiAnybusRxLng )
		{
			AnyRxEnde ( port );
		}
	}
}

/* Empfang sperren */
void AnyRxEnde ( char port )
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
	ucAnyRxStatus = ANY_RX_ENDE;										// empfangen
}

void AnyTxInit ( char port )
{
	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			//RTS_S3 = 1;				// Sendekanal	Freigabe,	für RS232 nicht notwendig

			// Transmit Receive Mode Register
			MR_S3 = 0x05;				// 0 0 0 0 0 101	
													// | | | | | |||
													// | | | | | UART Mode 8 Bit
													// | | | | Internal Clock             
													// | | | 1 Stop Bit	
													// | | x	
													// | no parity
													// Rx Tx Polarity not inversed
		
			// Transmit Receive Control Register 0											
			C0_S3 = 0x10;				// 0 0 0 1 0 0 00
													// | | | | | | ||
													// | | | | | | f1 Takt
													// | | | | | x CTS oder RTS Funktion siehe Bit4
													// | | | | x Transmit Register Empty Flag TXEPT, Read Only, 
													// | | | Disable CTS/RTS Funktion											
													// | | TxD CMOS Output
													// | CLK Polarität
													// LSB first
	
			// Für	Telegramm-Timeout	Überwachung: TimerB0 (Vorteiler) + TimerB1 (Teiler) 
			// SourceClock: f2n = 16 (29,491200 MHz / 16 = 1,843200 MHz = 0,54253472 µs)
				// Timeout setzen	für	Telegrammüberwachung Senden + Empfang	ca.	1085	ms	
				P_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Prescaler
				C_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Counter
				P_TOUTR_S3	=	4000;						// Load  Prescaler
				C_TOUTR_S3	=	500;						// Load  Counter
				P_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Prescaler
				C_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Counter

			// Baudrate
			BRG_S3 = (unsigned char)( ( (f1_CLK_SPEED/16) / lBaudrate_S3) -1 );

			TE_BIT_S3 = 1;							// Senden erlauben
			DISABLE_IRQ			
			TIC_S3 = 0x04;							// Enable Transmit Interrupt Prio: 4			
			ENABLE_IRQ	
			
			ucAnyRxStatus = ANY_TX_BEGIN;
			uiAnybusTxCtr = 0;
			TB_S3 = *pucAnybusTx;						// Startbyte ausgeben
								
			break;
				
	}
}

void AnyRxInit ( char port )
{
	uiAnybusRxCtr		=	0;								// Sende-	/	Empfangspuffer Index
	pucAnybusRx			= ucAnybusRxBuf;
//	AnybusRxLng		= 1;								// mindestens ein Zeichen wird erwartet	

	switch ( port )
	{
		case S1:
			break;
			
		case S2:
			break;
			
		case S3:
			//RTS_S3 = 0;					// Empfangskanal	Freigabe, für RS232 nicht notwendig
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
		
			// Für	Telegramm-Timeout	Überwachung: TimerB0 (Vorteiler) + TimerB1 (Teiler) 
			// SourceClock: f2n = 16 (29,491200 MHz / 16 = 1,843200 MHz = 0,54253472 µs)
				// Timeout setzen	für	Telegrammüberwachung Senden + Empfang	ca.	1085	ms	
				P_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Prescaler
				C_TOUTS_S3	= 0;								// Stop	 Telegramm-Timeout	Timer Counter
				P_TOUTR_S3	=	4000;						// Load  Prescaler
				C_TOUTR_S3	=	500;						// Load  Counter
				P_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Prescaler
				C_TOUTS_S3	= 1;								// Start Telegramm-Timeout Timer Counter

			RE_BIT_S3 = 1;		// Empfang erlauben 																						
			
			break;
	}
}

//*******************************************************************************
// Status des AnyModModuls
//*******************************************************************************
SINT8 Abcc_Status(UINT8 iStatusReg) {
//*******************************************************************************
	SINT8 cReturn = 1;
	static UINT8 iOldState = 0xFF;

//	if(iOldState!=(iStatusReg&ABP_STAT_S_BITS)) {
		switch(iStatusReg&ABP_STAT_S_BITS) {
			case ABP_ANB_STATE_SETUP:
//					printf( "SETUP...\n" );
				memcpy( (void *)carAnybusstatus, " SETUP         ", 15 );
				if((iOldState>ABP_ANB_STATE_SETUP)&&(iOldState!=0xff)) {
					// Reset des Anybusmodul, alles auf Neuanfang
					ucAnyRxStatus = ANY_RX_EXECPTION;
					cReturn = -1;
				}
				break;
			case ABP_ANB_STATE_NW_INIT:
//					printf( "NW_INIT...\n" );
				memcpy( (void *)carAnybusstatus, " NW_INIT       ", 15 );
				break;
			case ABP_ANB_STATE_WAIT_PROCESS:
//					printf( "WAIT_PROCESS...\n" );
				memcpy( (void *)carAnybusstatus, " WAIT_PROCESS  ", 15 );
				break;
			case ABP_ANB_STATE_IDLE:
//					printf( "IDLE...\n" );
				memcpy( (void *)carAnybusstatus, " IDLE          ", 15 );
				break;
			case ABP_ANB_STATE_PROCESS_ACTIVE:
//					printf( "PROCESS_ACTIVE...\n" );
				memcpy( (void *)carAnybusstatus, " PROCESS_ACTIVE", 15 );
				break;
			case ABP_ANB_STATE_ERROR:
//					printf( "ERROR...\n" );
				memcpy( (void *)carAnybusstatus, " ERROR         ", 15 );
				break;
			case ABP_ANB_STATE_RESERVED:
//					printf( "RESERVED...\n" );
				memcpy( (void *)carAnybusstatus, " RESERVED         ", 15 );
				break;
			case ABP_ANB_STATE_EXCEPTION:
//					printf( "EXCEPTION...\n" );
				memcpy( (void *)carAnybusstatus, " EXCEPTION         ", 15 );
				ucAnyRxStatus = ANY_RX_EXECPTION;
				break;
			default:
//					printf( "UNBEKANNT...\n" );
				memcpy( (void *)carAnybusstatus, " UNBEKANNT     ", 15 );
				break;
		}
		iOldState = (iStatusReg&ABP_STAT_S_BITS);
//	}
	
	return cReturn;
}

//***************************************************
// InitAnyBusDaten()
// Baut die Datentabelle für Anybus anhand der anybusTable auf
//***************************************************
void InitAnyBusDaten(void) {
//***************************************************
	unsigned int uiIndex;
	unsigned int uiDae;
	unsigned int uiInstanceNummer, uiOrderNumber;
	anybusDataTableRow *pDataRow;
	UINT8 ucAnyBusFormat;
	
//	RTS_S3			 	= 1;				// Reset an Anybus
	uiInstanceNummer = 1;			// Instancen fangen immer mit 1 an
	uiOrderNumber = 1;
	uiMaxInstanceNumber = 0;
	uiInstanceCount = 0;
	if((uiNumber_of_Data>0)) {
#if KOMTAB_ALL == 0
		// Es sind Daten in der Tabelle angegeben worden
		for(uiIndex=0; uiIndex<uiNumber_of_Data; uiIndex++) {
			// Ermitteln der DAE-Nummer anhand der Speicheradresse
			pDataRow = &anybusDataTable[uiIndex];
			pDataRow->ucZugriff = ANYBUS_NONE_ACCESS;
			pDataRow->siProcessIndex = NO_PROCESS_DATA;
			for(uiDae=0; uiDae<KOMTAB_LENG; uiDae++) {
				if(anybusTable[uiIndex].pValue==Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].label) {
					// Paraemter gefunden, Daten merken
					pDataRow->uiDAE = uiDae;
					if((Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].num[0]=='*')||(Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].num[0]=='+')) {
						pDataRow->ucZugriff = ANYBUS_READ_ACCESS;
					} else {
						pDataRow->ucZugriff = anybusTable[uiIndex].ucZugriff;
					}
//120623					anybusDataTable[uiIndex].uiInstanceNumber = uiIndex+1;
//120628					pDataRow->uiInstanceNumber = uiInstanceNummer;
//120628					pDataRow->uiInstanceAnzahl = RiedelFormatToInstanceAnzahl(Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].konv);
//120628					uiInstanceNummer += pDataRow->uiInstanceAnzahl;

					ucAnyBusFormat = GetAnyBusDataTyp(pDataRow);
					if(ucAnyBusFormat!=255) {
						if(((uiNetworkType==BACNET_IP)||(uiNetworkType==BACNET_MSTP))&&(ucAnyBusFormat==ABP_CHAR)) {
							pDataRow->ucZugriff = ANYBUS_NONE_ACCESS;
							pDataRow->uiInstanceNumber = uiInstanceNummer++;
						} else {
							pDataRow->uiInstanceNumber = uiInstanceNummer;
							pDataRow->uiInstanceAnzahl = RiedelFormatToInstanceAnzahl(Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].konv);
							pDataRow->uiOrderNumber = uiOrderNumber;
							uiInstanceNummer += pDataRow->uiInstanceAnzahl;
							uiInstanceCount += pDataRow->uiInstanceAnzahl;
							uiOrderNumber += pDataRow->uiInstanceAnzahl;
						}
					} else {
						pDataRow->ucZugriff = ANYBUS_NONE_ACCESS;
						pDataRow->uiInstanceNumber = uiInstanceNummer++;
					}


					break;// Dae-Schleife
				} // if(anybusTable[iIndex].pValue==Pgruppe[KomTab[iDae].gnum].Param[KomTab[iDae].pnum].label) 
			} // for(iDae=0; iDae<KOMTAB_LENG; iDae++) 
		}
#else
		uiNumber_of_Data = KOMTAB_LENG;
		for(uiDae=0; uiDae<KOMTAB_LENG; uiDae++) {
			// Paraemter gefunden, Daten merken
			pDataRow = &anybusDataTable[uiDae];

			pDataRow->uiDAE = uiDae;
			pDataRow->uiInstanceNumber = 0;
			pDataRow->siProcessIndex = NO_PROCESS_DATA;
			if((Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].num[0]=='*')||(Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].num[0]=='+')) {
				// ANYBUS_READ_ACCESS alleine -> Dieser Parameter wird mit jeder Datü zum Anybus übertragen
				pDataRow->ucZugriff = ANYBUS_READ_ACCESS;
			} else {
				pDataRow->ucZugriff = ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS; 
			}

//120623			if(GetAnyBusDataTyp(&anybusDataTable[uiDae])!=255) {
			ucAnyBusFormat = GetAnyBusDataTyp(pDataRow);
			if(ucAnyBusFormat!=255) {
				if(((uiNetworkType==BACNET_IP)||(uiNetworkType==BACNET_MSTP))&&(ucAnyBusFormat==ABP_CHAR)) {
					pDataRow->ucZugriff = ANYBUS_NONE_ACCESS;
					pDataRow->uiInstanceNumber = uiInstanceNummer++;
				} else {
					pDataRow->uiInstanceNumber = uiInstanceNummer;
					pDataRow->uiInstanceAnzahl = RiedelFormatToInstanceAnzahl(Pgruppe[KomTab[uiDae].gnum].Param[KomTab[uiDae].pnum].konv);
					pDataRow->uiOrderNumber = uiOrderNumber;
					uiInstanceNummer += pDataRow->uiInstanceAnzahl;
					uiInstanceCount += pDataRow->uiInstanceAnzahl;
					uiOrderNumber += pDataRow->uiInstanceAnzahl;
				}
			} else {
				pDataRow->ucZugriff = ANYBUS_NONE_ACCESS;
				pDataRow->uiInstanceNumber = uiInstanceNummer++;
			}
		} // for(iDae=0; iDae<KOMTAB_LENG; iDae++) 
#endif
		uiMaxInstanceNumber = uiInstanceNummer - 1;
	}
//	RTS_S3			 	= 0;				// Reset an Anybus beenden

}

//***************************************************
// GetName()
// Gibt Namen aus der Parlizurück
//***************************************************
UINT8* GetName(anybusDataTableRow *pDataRow) {
//***************************************************
	unsigned char ucRiedelFormat, ucRiedelAttribute;

	UINT8 ucRetFormat;

	ucRetFormat  = 0;
	if(pDataRow!=NULL) {
		// Ermitteln der Formatierung des Elementes
		return Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].bez;
	}
	return NULL;
}

//***************************************************
// GetGruppe()
// Gibt die Bezeichnung der Gruppe aus der Parlizurück
//***************************************************
UINT8* GetGruppe(anybusDataTableRow *pDataRow) {
//***************************************************
	unsigned char ucRiedelFormat, ucRiedelAttribute;

	UINT8 ucRetFormat;

	ucRetFormat  = 0;
	if(pDataRow!=NULL) {
		// Ermitteln der Formatierung des Elementes
		return Pgruppe[KomTab[pDataRow->uiDAE].gnum].grkz;
	}
	return NULL;
}
//***************************************************
// GetUnit()
// Gibt die Einheit aus der Parlizurück
//***************************************************
UINT8* GetUnit(anybusDataTableRow *pDataRow) {
//***************************************************
	unsigned char ucRiedelFormat, ucRiedelAttribute;

	UINT8 ucRetFormat;

	ucRetFormat  = 0;
	if(pDataRow!=NULL) {
		// Ermitteln der Formatierung des Elementes
		if((Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].attr&0x80)!=0) {
			return pucConstStatus;
		} else {
			return Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].dim;
		}
	}
	return NULL;
}

//***************************************************
// GetAnyBusDataTyp()
// Gibt den Anybusdatentyp zurück
//***************************************************
UINT8 GetAnyBusDataTyp(anybusDataTableRow *pDataRow) {
//***************************************************
	unsigned char ucRiedelFormat, ucRiedelAttribute;

	UINT8 ucRetFormat;

	ucRetFormat  = 0;
	if(pDataRow!=NULL) {
		// Ermitteln der Formatierung des Elementes
		ucRiedelFormat = Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].konv;
		ucRiedelAttribute = Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].attr;
		ucRetFormat = RiedelFormat2AnybusFormat(ucRiedelFormat, ucRiedelAttribute);
	}
	
	return ucRetFormat;
}

//***************************************************
// Gibt das Anybusformat für einen Riedel Formatyp zurück
// das Attribute wird dabei berücksichtigt
//***************************************************
UINT8 RiedelFormat2AnybusFormat(UINT8 ucRiedelFormat, UINT8 ucRiedelAttribute) {
//***************************************************
	UINT8 ucRetformat = 255;
	switch(ucRiedelFormat) {
		case ANOUT_FORM:
			ucRetformat = ABP_UINT8;
			break;
		case US_CHAR:
		case KALTST_FORM:
			if(ucRiedelAttribute==0) {
				ucRetformat = ABP_UINT8;
			} else {
				ucRetformat = ABP_FLOAT;
			}
			break;
		case S_CHAR:
			if(ucRiedelAttribute==0) {
				ucRetformat = ABP_SINT8;
			} else {
				ucRetformat = ABP_FLOAT;
			}
			break;
		case BAUD_FORM:
		case US_INT:
		case AOUT_FORMP:
			if(ucRiedelAttribute==0) {
				ucRetformat = ABP_UINT16;
			} else {
				ucRetformat = ABP_FLOAT;
			}
			break;
		case UHR_FORM:
		case DATUM_FORM:
		case DATE_FORM:
		case ZEIT_FORM:
			ucRetformat = ABP_UINT8;
			break;
		case ANA_FORM:
		case ANA_FORMP:
		case S_INT:
			if(ucRiedelAttribute==0) {
				ucRetformat = ABP_SINT16;
			} else if((ucRiedelAttribute&0x80)==0) {
				//Messwert
				ucRetformat = ABP_FLOAT;
			} else {
				// Status
				ucRetformat = ABP_UINT8;
			}
			break;
		case US_LONG:
		case US_LONGP:
			if(ucRiedelAttribute==0) {
				ucRetformat = ABP_UINT32;
			} else {
				ucRetformat = ABP_FLOAT;
			}
			break;
		case ASCII_FORM:
		case TELE_FORM:
//		case UHR_FORM:
		case EA_NAME_FORM:
		case TASK_FORM:
		default:
			ucRetformat = ABP_CHAR;
			break;
		case JANEIN_FORM:
		case JANEIN_FORMOP:
		case JANEIN_FORMIP:
		case JAFRAGE_FORM:
			ucRetformat = ABP_BOOL;
			break;
		case ZE_FORMP:
		// Spezialumsetzung der Zähler
			switch(ucRiedelAttribute&0x78) {
				case 0x40:
					ucRetformat = RiedelFormat2AnybusFormat(ASCII_FORM,15);
					break;
				case 0x30:
					ucRetformat = RiedelFormat2AnybusFormat(DATE_FORM,3);
					break;
				case 0x28:
					ucRetformat = RiedelFormat2AnybusFormat(DIGL_FORM,8);
					break;
				case 0x20:
				default:
					ucRetformat = RiedelFormat2AnybusFormat(US_LONG,(ucRiedelAttribute&0x07));
					break;
				case 0x10:
					ucRetformat = RiedelFormat2AnybusFormat(US_INT,(ucRiedelAttribute&0x07));
					break;
				case 0x08:
					ucRetformat = RiedelFormat2AnybusFormat(US_CHAR,(ucRiedelAttribute&0x07));
					break;
				case 0x18:
					ucRetformat = RiedelFormat2AnybusFormat(DIGR_FORM,8);
					break;
				
			}
			break;
	}
// UHR_FORM, DATUM_FORM, WOTAG_FORM, BICMOVE_FORM,DIGR_FORM,
	return ucRetformat;
}
//***************************************************
// GetAnyBusDataTypNumbers()
// Gibt die Anzahl der Datenelemente für diesen Eintrag zurück
//***************************************************
UINT8 GetAnyBusDataTypNumbers(anybusDataTableRow *pDataRow) {
//***************************************************
	unsigned char ucRiedelFormat, ucRiedelAttribute;
	UINT8 ucRetAnzahl;
	UINT16 uiAnzahl;

	ucRetAnzahl  = 0;
	uiAnzahl = 0;
	if(pDataRow!=NULL) {
		// Ermitteln der Formatierung des Elementes
		ucRiedelFormat = Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].konv;
		ucRiedelAttribute = Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].attr;
		uiAnzahl = RiedelFormat2AnybusAnzahl(ucRiedelFormat, ucRiedelAttribute);
		if(uiAnzahl>255) {
			// Fehler mehr als 255 darf die Anzahl nicht sein
			ucRetAnzahl = 255;
		} else {
			ucRetAnzahl = (UINT8) uiAnzahl;
		}
	}
	return ucRetAnzahl;
}

//***************************************************
// Gibt das AnybusAnzahl für einen Riedel Formatyp zurück
// das Attribute wird dabei berücksichtigt
//***************************************************
UINT16 RiedelFormatToInstanceAnzahl(UINT8 ucRiedelFormat) {
//***************************************************
	UINT16 uiRetanzahl;

	if((uiNetworkType==BACNET_IP)||(uiNetworkType==BACNET_MSTP)) {
		switch(ucRiedelFormat) {
			default:
				uiRetanzahl = 1;
				break;
			case ZEIT_FORM:
				uiRetanzahl = 2;
				break;
			case UHR_FORM:
			case DATUM_FORM:
			case DATE_FORM:
				uiRetanzahl = 3;
				break;
		}
	} else {
		uiRetanzahl = 1;
	}
	return uiRetanzahl;
}
//***************************************************
// Gibt das AnybusAnzahl für einen Riedel Formatyp zurück
// das Attribute wird dabei berücksichtigt
//***************************************************
UINT16 RiedelFormat2AnybusAnzahl(UINT8 ucRiedelFormat, UINT8 ucRiedelAttribute) {
//***************************************************
	UINT16 uiRetanzahl;

	switch(ucRiedelFormat) {
		default:
			uiRetanzahl = 1;
			break;
		case TASK_FORM:
			uiRetanzahl = 3;
			break;
		case ZEIT_FORM:
			if((uiNetworkType==BACNET_MSTP)||(uiNetworkType==BACNET_IP)) {
				uiRetanzahl = 1;
			} else {
				uiRetanzahl = 2;
			}
			break;
		case UHR_FORM:
		case DATUM_FORM:
		case DATE_FORM:
			if((uiNetworkType==BACNET_MSTP)||(uiNetworkType==BACNET_IP)) {
				uiRetanzahl = 1;
			} else {
				uiRetanzahl = 3;
			}
			break;
		case ASCII_FORM:
		case TELE_FORM:
//		case UHR_FORM:
		case EA_NAME_FORM:
			if((ucRiedelAttribute<1)||(ucRiedelAttribute>15)) {
				uiRetanzahl =  15;
			} else {
				uiRetanzahl = ucRiedelAttribute;
			}
			break;
	}

	return uiRetanzahl;
}

//***************************************************
// AnybusKonvert() - konvertiert aus einen Float in den Sendepuffer und umgekehrt
// 					IN:		pflData - Pointer auf das Ergebnis oder auf den Wert, bei ABP_CHAR steht hier die Adresse des erstens Zeichens
//								*ucBuffer - Sende bzw. Empfangspuffer
//								pDataRow - aktueller DatenSpeicher
//								ucRW	-	gewünschte Richtung Lesen(KONVRD) (in den Sendepuffer) oder Schreiben(KONVWR) (kommen aus dem ucBuffer)
//					OUT:	pflData - Pointer auf den Wert, der aus der Datenübertragung kam
//								*ucBuffer - bei Leserichtung die Daten zum Übertragen
//					return Anzahl der gelesenen/geschriebenen Bytes
//***************************************************
UINT8 AnybusKonvert(float *pflData, UINT8 *ucBuffer, anybusDataTableRow *pDataRow, UINT8 ucRW, UINT8 ucSubIndex) {
//***************************************************
	unsigned char ucAnzahl = 0;
	UINT8 ucData;
	SINT8 cData;
	UINT16 uiData16;
	SINT16 siData16;
	UINT32 uiData32;
	SINT32 siData32;
	FLOAT32 flData32;
	UINT8 *pucBuffer;
 
	if(pDataRow!=NULL) {
		switch(GetAnyBusDataTyp(pDataRow)) {
			case ABP_CHAR:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = GetAnyBusDataTypNumbers(pDataRow);
					pucBuffer = (UINT8*)*(long*)pflData;
					memcpy(ucBuffer,pucBuffer,ucAnzahl);
				} else {
					ucAnzahl = GetAnyBusDataTypNumbers(pDataRow);
//					memcpy(pflData,&((long)ucBuffer),sizeof(float));
					uiData32 = (long)ucBuffer;
					memcpy(pflData,&uiData32,sizeof(uiData32));
				}
				break;
			case ABP_BOOL:
			case ABP_UINT8:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_UINT8_SIZEOF*GetAnyBusDataTypNumbers(pDataRow);
//					ucData = (UINT8)*pflData;
//					memcpy(ucBuffer,&ucData,ucAnzahl);
					if(ucAnzahl<=4) {
						// es werden die Werte direkt übergeben
						// Hier werden außer reinen Bytewerten auch, UHR-FORM und Datum konvertiert
						// Uhr_Form, Datum, Date und Zeit_Form werden die Einzelbytes auf den Float geschafft und übertragen
						if(pDataRow->uiInstanceAnzahl==1) {
							// komtt direkt als Floatwert
							ucData = (UINT8)*pflData;
							memcpy(ucBuffer,&ucData,ucAnzahl);
						} else {
							flData32 = (FLOAT32)*pflData;
							memcpy(ucBuffer,&(((UINT8*)&flData32)[ucSubIndex]),1);
						}
/*						if(ucSubIndex>0){
							memcpy(ucBuffer,&(((UINT8*)&flData32)[ucSubIndex]),1);
						} else {
							memcpy(ucBuffer,&flData32,ucAnzahl);
						}*/
					} else {
						// es wird eine Pointer übergeben
						// bei Mehr als 4 BYte reicht der Float nur noch für die Adresse der Quelle (siehe ABP_CHAR)
						pucBuffer = &(((UINT8*)*(long*)pflData)[ucSubIndex]);
						memcpy(ucBuffer,pucBuffer,ucAnzahl);
					}
				} else {
					ucAnzahl = ABP_UINT8_SIZEOF*GetAnyBusDataTypNumbers(pDataRow);
//					memcpy(&ucData,ucBuffer,ucAnzahl);
//					*pflData = ucData;
					if(ucAnzahl<=4) {
						// es werden die Werte direkt übergeben
						if(pDataRow->uiInstanceAnzahl==1) {
							// kommt als Floatwert
							memcpy(&ucData,ucBuffer,ucAnzahl);
							*pflData = ucData;
						} else {
							flData32 = (FLOAT32)*pflData;
//						if(ucSubIndex>0){
							memcpy(&(((UINT8*)&flData32)[ucSubIndex]),ucBuffer,1);
//							memcpy(&flData32,ucBuffer,ucAnzahl);
/*						} else {
//							memcpy(ucBuffer,&flData32,ucAnzahl);
							memcpy(&flData32,ucBuffer,ucAnzahl);
						}*/
//120623						memcpy(&flData32,ucBuffer,ucAnzahl);
							*pflData = flData32;
						}
					} else {
						// es wird eine Pointer übergeben
						uiData32 = (long)ucBuffer;
//120623						memcpy(pflData,&uiData32,sizeof(uiData32));
						memcpy(&(((UINT8*)&flData32)[ucSubIndex]),&uiData32,sizeof(uiData32));
					}
				}
				break;
			case ABP_SINT8:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_SINT8_SIZEOF;
					cData = (SINT8)*pflData;
					memcpy(ucBuffer,&cData,ucAnzahl);
				} else {
					ucAnzahl = ABP_SINT8_SIZEOF;
					memcpy(&cData,ucBuffer,ucAnzahl);
					*pflData = cData;
				}
				break;
			case ABP_UINT16:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_UINT16_SIZEOF;
					uiData16 = (UINT16)*pflData;
					HostEndian2NetEndain(ucBuffer,(void*)&uiData16,ucAnzahl);
				} else {
					ucAnzahl = ABP_UINT16_SIZEOF;
					HostEndian2NetEndain((void*)&uiData16,ucBuffer,ucAnzahl);
					*pflData = uiData16;
				}
				break;
			case ABP_ENUM:
			case ABP_SINT16:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_SINT16_SIZEOF;
					siData16 = (SINT16)*pflData;
					HostEndian2NetEndain(ucBuffer,(void*)&siData16,ucAnzahl);
				} else {
					ucAnzahl = ABP_SINT16_SIZEOF;
					HostEndian2NetEndain((void*)&siData16,ucBuffer,ucAnzahl);
					*pflData = siData16;
				}
				break;
			case ABP_UINT32:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_UINT32_SIZEOF;
					uiData32 = (UINT32)*pflData;
					HostEndian2NetEndain(ucBuffer,(void*)&uiData32,ucAnzahl);
				} else {
					ucAnzahl = ABP_UINT32_SIZEOF;
					HostEndian2NetEndain((void*)&uiData32,ucBuffer,ucAnzahl);
					*pflData = uiData32;
				}
				break;
			case ABP_SINT32:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_SINT32_SIZEOF;
					siData32 = (SINT32)*pflData;
					HostEndian2NetEndain(ucBuffer,(void*)&siData32,ucAnzahl);
				} else {
					ucAnzahl = ABP_SINT32_SIZEOF;
					HostEndian2NetEndain((void*)&siData32,ucBuffer,ucAnzahl);
					*pflData = siData32;
				}
				break;
			case ABP_FLOAT:
				if(ucRW ==	KONVRD)	{
					ucAnzahl = ABP_FLOAT_SIZEOF;
					flData32 = (FLOAT32)*pflData;
					HostEndian2NetEndain(ucBuffer,(void*)&flData32,ucAnzahl);
//					memcpy(ucBuffer,&flData32,ucAnzahl);
				} else {
					ucAnzahl = ABP_FLOAT_SIZEOF;
//					memcpy(&flData32,ucBuffer,ucAnzahl);
					HostEndian2NetEndain((void*)&flData32,ucBuffer,ucAnzahl);
					*pflData = flData32;
				}
				break;
		}
 	}
 return ucAnzahl;
}

//***************************************************
// HostEndian2NetEndain() - konvertiert ein Host abhängignen Endian
// in ein Netzabhängiges Endian (R66==LittleEndian -> Bacnet/IP==Bigendian)
// return die Anzahl
//***************************************************
UINT8 HostEndian2NetEndain(void* pDestBuffer, void* pSourceBuffer, UINT8 uiByteAnzahl) {
//***************************************************
	UINT8 ucIndex;
	for(ucIndex=0; ucIndex<uiByteAnzahl; ucIndex++) {
//		if( eNetFormat == NET_BIGENDIAN) {
		if( eNetFormat != eHostFormat) {
			((UINT8*)pDestBuffer)[uiByteAnzahl-(ucIndex+1)] = ((UINT8*)pSourceBuffer)[ucIndex];
		} else {
			((UINT8*)pDestBuffer)[ucIndex] = ((UINT8*)pSourceBuffer)[ucIndex];
		}
	}
	return uiByteAnzahl;
}
#endif //( (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL) 
		