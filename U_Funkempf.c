//	Funkempf Handler
//	Aufruf erfolgt vom HandlerS1 bis HandlerS3 organisiert vom UserHandler
 
#include "sfr32C87.h"
#include "sio.h"
#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "projdef.h"

void Rx_Init_RF(char port);

#define FL_K	((float)(40.0	/	256.0))
#define RF_NO_TEMP_SHIFT 0x01		// Soll die Temperaturverschiebung übernommen werden ? (0=JA;1=NEIN)

#define SRC_RS485_EVC 1		//090310

#if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )

void U_Funkempf_ok(char port);

//----------------------------------------------------------
// Funktion U_Funkempf
//----------------------------------------------------------
char U_Funkempf(char port, char funktion)
{
	static char neustart = 1;
	static unsigned char ucTeachInSekunden = 0;		// Die Task erzeugt einen eigenen Sekundentakt für TimeOut beim TeachIn

	char fu_ret = funktion;		// Ok Vorbelegung
	char rf_clear = 0;				// Merker für Port-Änderung
	char i;
	
	unsigned char ucZeiger;
//	unsigned char ucFeld;
//	unsigned char s, d;
//	signed char scRf_temp_shift;
	
//	ucFeld = 0;
	
	if(++ucTeachInSekunden >= 10) {
		ucTeachInSekunden = 0;
		if((ucTeach_idx & 0xF0) > 0) {
			// Ein TeachIn läuft gerade. Die Variable ucTeach_idx wird durch die Datenübertragung mit dem RIEcon50 nachgetriggert.
			if((ucTeach_idx & 0xF0) == 0x10) {
				// TeachIn abbrechen!
				ucTeach_idx = 0;
				for(ucZeiger=0;ucZeiger<12;ucZeiger++) {
					rf_feld[ucZeiger].ucRf_teach_enable = 0;
				} // for(ucZeiger=0;ucZeiger<12;ucZeiger++)
			} // if((ucTeach_idx & 0xF0) == 0x10)
			else {
				ucTeach_idx -= 0x10;	// TimeOut zurückzählen
			} // ELSE if((ucTeach_idx & 0xF0) == 0x10)
		} // if((ucTeach_idx & 0xF0) > 0)
	} // if(++ucTeachInSekunden >= 10)

	if((bedien_rf_nummer < 1) || (bedien_rf_nummer > 12)) {
		bedien_rf_nummer = 1;
	}

	if(neustart == 1)
	{
		rf_clear = 1;
		neustart = 0;
	}	
		
	// Sicherstellen, dass nur ein Port für Funkempfang ausgewählt ist.
	// Die zuletzt parametrierte Funktion setzt sich durch, die anderen werden auf Standard zurückgestellt.
	switch(port)
	{
				
		case S1:
			if(Funktion_S2 == FUNK1_FU)
			{	Funktion_S2 = GBUS_FU;
				rf_clear = 1;
			}	
			if(Funktion_S3 == FUNK1_FU)
			{	Funktion_S3 = GBUS_FU;
				rf_clear = 1;
			}	
			break;

		case S2:
			if(Funktion_S1 == FUNK1_FU)
			{	Funktion_S1 = GBUS_FU;
				rf_clear = 1;
			}	
			if(Funktion_S3 == FUNK1_FU)
			{	Funktion_S3 = GBUS_FU;
				rf_clear = 1;
			}	
			break;
	
		case S3:
			if(Funktion_S1 == FUNK1_FU)
			{	Funktion_S1 = GBUS_FU;
				rf_clear = 1;
			}	
			if(Funktion_S2 == FUNK1_FU)
			{	Funktion_S2 = GBUS_FU;
				rf_clear = 1;
			}	
			break;
	}
	
	if(rf_clear == 1)					// RF-Empfangsport wurde geändert, bzw. Neustart
	{
		Rx_Init_RF(port);
		for(i=0;i<12;i++)
			rf_feld[i].ucRf_status	=	0;

	}			

	funk_vis = SICHTBAR;							// Parli Gruppe sichtbar
	// fu_ret = GBUS_FU;					// zur Zeit nicht vorhanden	
	

	switch(ucRxFlag_RF) {
//		case RX_END: //siehe U_Funkempf_ok()
//			break;
			
		case RX_ERROR:
			Rx_Init_RF(port);
			break;
	
	}// switch(ucRxFlag_RF)

	return(fu_ret);
}

//-------------------------------------------------
// Initialisierung der Schnittstelle auf RF-Empfang
//-------------------------------------------------
void Rx_Init_RF(char port)
{
	switch(port)
	{
		case S1:								// Die S1-Sio soll RF machen
			Mode_S1 = SLAVE;
			Baudrate_S1	=	9600;
			baudset_S1();
			Rx_Init_S1();
			break;
		case S2:								// Die S2-Sio soll RF machen
			Mode_S2 = SLAVE;
			Baudrate_S2	=	9600;
			baudset_S2();
			Rx_Init_S2();
			break;
		case S3:								// Die S3-Sio soll RF machen
			Mode_S3 = SLAVE;
			Baudrate_S3	=	9600;
			baudset_S3();
			Rx_Init_S3();
			break;
	}
	ucRxFlag_RF = 0;
	ucCond_RF = 0;

}
			

//----------------------------------------------------------
// Sende Interrupt
//----------------------------------------------------------
void U_Funkempf_ISR_Tx_Int(char port)
{

}

//--------------------------------------------------------------
// Empfangs Interrupt
//--------------------------------------------------------------
void U_Funkempf_ISR_Rx_Int(char port)
{
	char rbyte = 0;
	
	switch(port)
	{
		case S1:
			rbyte = RB_S1;										// Empfangsbyte	abholen
			break;
		case S2:
			RIR_BIT_S2   = 0;									// Interrupt Request Bit löschen
			rbyte = RB_S2;										// Empfangsbyte	abholen
			break;
		case S3:
			rbyte = RB_S3;										// Empfangsbyte	abholen
			break;
	}		

  switch(ucCond_RF) {
		case 0:
			if(rbyte != 0xA5)			// Test SYNC_BYTE1
				break;
			ucCond_RF = 1;
			break;

		case 1:
			if(rbyte != 0x5A) {		// Test SYNC_BYTE2
				ucRxFlag_RF = RX_ERROR;
				ucCond_RF = 0;
			}
			else {
				ucCond_RF = 2;
				ucBcounter_RF = 0;
			}
			break;

		case 2:
			ucRxFlag_RF = RX_RUN;
//090310			ucfRxBuf_RF[ucBcounter_RF++] = rbyte;			// Bytecounter
			ucfRxBuf_RF[ucBcounter_RF++] = rbyte;			// Adresse
			ucChecksum_RF = rbyte;
			ucCond_RF = 3;
			break;

		case 3:
			ucfRxBuf_RF[ucBcounter_RF++] = rbyte;			// Daten
			ucChecksum_RF	+= rbyte;
			if(ucBcounter_RF <= 10)
				break;
			ucCond_RF = 4;
			break;

		case 4:
			if(ucReceiverTyp_RF == SRC_RS485_EVC) {
				ucChecksum_RF += 0xFF;
			}
			if(ucChecksum_RF == rbyte) {
				ucRxFlag_RF = RX_END;
				ucCond_RF = 5;
				U_Funkempf_ok(port);
			}
			else {
				ucRxFlag_RF = RX_ERROR;
				ucCond_RF = 5;
			}
			break;
		case 5:
			rbyte = 0;
			break;
	} // switch(ucCond_RF)
	
}

//----------------------------------------------------------
// Timer Interrupt für Telegramm-Timeout
//----------------------------------------------------------
void U_Funkempf_ISR_Tel_Tout(char port)
{

}
	
//---------------------------------------------------------------
// Interrupt Baudratenmessung
//---------------------------------------------------------------
void U_Funkempf_ISR_BaudCheck(char port)
{

}

void U_Funkempf_ok(char port) {
	long temp_id;
	unsigned char ucZeiger;
	unsigned char ucFeld;
	unsigned char ucSchalter;
	unsigned char ucZwischen;
	signed char scRf_temp_shift;

	temp_id	= *(long*) &ucfRxBuf_RF[6];	// Transmitter-ID temporär ablegen
	ucFeld = 0;
//100308	switch(ucRxFlag_RF) {
		if(ucfRxBuf_RF[1] != 0x07) { //090722 NEU 0x07 entspricht einen Temperatursensor (SR04.) der Firma Thermokon
			Rx_Init_RF(port);
			return; //100308 break;
		} // if(ucfRxBuf_RF[1] != 0x07)
		// Test Teach-In 2 ( Im R36-ID-Feld anlegen )
		if((ucfRxBuf_RF[5]	==	0x07) || (ucfRxBuf_RF[5]	==	0x87)) {	//090722 Es ist ein Teach-In-Telegramm
			if(((ucTeach_idx & 0x0F) > 0) && ((ucTeach_idx & 0x0F) < 13)) {
				if((rf_feld[(ucTeach_idx & 0x0F)-1].ucRf_teach_enable) == TRUE) {
					rf_feld[(ucTeach_idx & 0x0F)-1].ulRf_modul_id = temp_id;
					rf_feld[(ucTeach_idx & 0x0F)-1].ucRf_teach_enable += RCOK;
					ucTeach_idx = RCOK;
					Rx_Init_RF(port);
					return;//100308 break;
				} // if((rf_feld[(ucTeach_idx & 0x0F)-1].ucRf_teach_enable) == TRUE
			} // if(((ucTeach_idx & 0x0F) > 0) && ((ucTeach_idx & 0x0F) < 13))
			Rx_Init_RF(port);	//090722 hinzugefügt, da jeder Empfänger neu initialisiert werden muß (z.B.: Telegramm nicht für mich)
			return;//100308	break;	//090722 hinzugefügt, da bei einem Teach-In-Telegramm in den Daten die Herstellerkennung ist!
		} // if(((ucfRxBuf_RF[5]	==	0x07) || (ucfRxBuf_RF[5]	==	0x87))
		// Raum-Index generieren
		ucFeld = 0;
		for (ucZeiger = 0; ucZeiger < 12; ucZeiger++)				// momentan 12 ID`s vorgesehen
		{ if(rf_feld[ucZeiger].ulRf_modul_id ==	temp_id)
			{	ucFeld	= (char)(ucZeiger + 1);									// +1 wird zur Anzeige gebraucht
				break;
			}
		}
		if(ucFeld > 0)																			// ID wurde in der Liste gefunden
		{	
			rf_feld[ucZeiger].ucLive_check_timer	= 60;				// wird im Minutentakt runtergezählt
			// --------------- Statusbyte generieren ----------------
			// Auswertung 5-Stufenschalter
			ucSchalter	=	ucfRxBuf_RF[2];
			ucZwischen = 0xC0;						// Status: Frame neu und noch nicht zum R50 gesendet
			if(ucSchalter	>=	210) {
				ucZwischen	|=	0x01;
			}
			else {
				if((ucSchalter	>=	190) && (ucSchalter < 210)) {
					ucZwischen	|=	0x02;
				}
				else {
					if((ucSchalter	>=	165) && (ucSchalter < 190)) {
						ucZwischen	|=	0x04;
					}
					else {
						if((ucSchalter	>=	145) && (ucSchalter < 165)) {
							ucZwischen	|=	0x08;
						}
						else {
							ucZwischen	|=	0x10;
						}
					}
				}
			}
			rf_feld[ucZeiger].ucRf_status	=	ucZwischen;
	
			if(ucfRxBuf_RF[5]	==	0x0E) {														// Auswertung Praesenztaste
				if(rf_feld[ucZeiger].ucRf_praesens <= 90) {						// <= 3 Stunden ?
					rf_feld[ucZeiger].ucRf_praesens += 30;							// Neue Nachforderung 60 Minuten
				}
				// nur eine Änderung der Taste merken
				rf_feld[ucZeiger].ucfBedienZeit[0] = hextobcd(Tag);
				rf_feld[ucZeiger].ucfBedienZeit[1] = hextobcd(Mon);
				rf_feld[ucZeiger].ucfBedienZeit[2] = hextobcd(Jahr);
				rf_feld[ucZeiger].ucfBedienZeit[3] = hextobcd(Std);
				rf_feld[ucZeiger].ucfBedienZeit[4] = hextobcd(Min);
				rf_feld[ucZeiger].ucfBedienZeit[5] = hextobcd(Sek);
			}
			rf_feld[ucZeiger].siRf_temp	=	(int)((40	-	((float)ucfRxBuf_RF[4]	*	FL_K))	*	10);
			if((rf_feld[ucZeiger].ucSetup & RF_NO_TEMP_SHIFT) == FALSE) {
				scRf_temp_shift = (signed char) ((ucfRxBuf_RF[3] - 127) / 4);	// 0x00 - 0xFF werden zu -3,x bis +3,x Grad konvertiert
				if(rf_feld[ucZeiger].scRf_temp_shift != scRf_temp_shift) {
					// die Änderung eintragen
					rf_feld[ucZeiger].scRf_temp_shift = scRf_temp_shift;
					rf_feld[ucZeiger].ucfBedienZeit[0] = hextobcd(Tag);
					rf_feld[ucZeiger].ucfBedienZeit[1] = hextobcd(Mon);
					rf_feld[ucZeiger].ucfBedienZeit[2] = hextobcd(Jahr);
					rf_feld[ucZeiger].ucfBedienZeit[3] = hextobcd(Std);
					rf_feld[ucZeiger].ucfBedienZeit[4] = hextobcd(Min);
					rf_feld[ucZeiger].ucfBedienZeit[5] = hextobcd(Sek);
				}
			}
			else {	//091023 ELSE NEU 
				rf_feld[ucZeiger].scRf_temp_shift = 0;
			}
			rf_feld[ucZeiger].ucfEmpfangsZeit[0] = hextobcd(Tag);
			rf_feld[ucZeiger].ucfEmpfangsZeit[1] = hextobcd(Mon);
			rf_feld[ucZeiger].ucfEmpfangsZeit[2] = hextobcd(Jahr);
			rf_feld[ucZeiger].ucfEmpfangsZeit[3] = hextobcd(Std);
			rf_feld[ucZeiger].ucfEmpfangsZeit[4] = hextobcd(Min);
			rf_feld[ucZeiger].ucfEmpfangsZeit[5] = hextobcd(Sek);
		}
		
		Rx_Init_RF(port);
//100308		break;
//100308	} // switch(ucRxFlag_RF)
}

#endif // #if ( ((IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL) || ((IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL) )
