#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "ustruct.h"

#if WILO_MODBUS == 1

#include <string.h>
#include "Modbus/modbusconstext.h"

unsigned int Get_Op_Decoder(unsigned int);
unsigned int Put_Op_Decoder(unsigned int);
unsigned int Get_Con_Decoder(unsigned int);
unsigned int Put_Con_Decoder(unsigned int);
void Get_Messages (void);
void setHoldingOFF (ModBusWiloPumpen *modbusWilo);

#define INIT_INPUT_READ 		1	// Initialisierung des Lesen der Input-Register
#define INPUT_ON_READ				2	// lesen der Input-Register
#define SET_HOLDING_REG			3	// Setzen der Holding Register
#define INIT_HOLDING_WRITE	4	// Initialisierung des Schreibens der Holding Register
#define INIT_HOLDING_READ		5	// Initialisierung des Lesens der Holding Register
#define SET_PARA						6	// Anzeigeparameter setzen und durchlauf für nächste Pumpe starten

void WiloPumpenSteuerung(void) {
	
	static char puNummer = 0;															// Counter für Wilo Pumpen Anzahl
	char x=0; 																						// Schleifenvariable
	unsigned char ucBusKommunikation=FALSE; 							// Merker für BusKommunikation
	static unsigned char ucRegisterFolge=INIT_INPUT_READ;	// Merker für Kommunikationsabfolge
	static unsigned char ucNeustart = FALSE;
	
	
	if(BusPuPara[puNummer].Funktion == 1) // Pumpe soll über Bus abgefragt werden?
	{
		// Pumpen Zuweisung
		wiloPuBusPara = &BusPuPara[puNummer]; 
		wiloPuBusData = &BusPuData[puNummer];
		modbusDeviceAddresses[0]=wiloPuBusPara->Adresse;
	}
	else
	{
		puNummer = (++puNummer<BUS_PU_MAX) ? puNummer:0; // Druchlauf für nächste Pumpe	
		ucRegisterFolge=INIT_INPUT_READ;
		return; //Taskabbruch da Pumpe nicht über Bus abfragen soll
	}

				
	// Prüfung "Keine laufende Kommunikation"
	for(x=0;x<(WILO_INPUT_REG_ANZ+WILO_HOLDING_REG_ANZ+WILO_HOLDING_REG_ANZ)&&ucBusKommunikation==FALSE;++x)
	{
		ucBusKommunikation |= modbusTableData[x].ready;	
	} 
				
	if(ucBusKommunikation==FALSE) // Neue Wiloabfrage wenn keine Abfrage läuft
	{ 
		
		switch (ucRegisterFolge)
		{
			case INIT_INPUT_READ: // Input Register abfragen
			
				// Inputregister der Kommunikation leeren
				memset(&modWiloPuData.wiloInReg,0,WILO_INPUT_REG_ANZ); 
				// Alle Input-Register Abfragen
				for(x=0;x<WILO_INPUT_REG_ANZ;++x)
				{
					modbusTableData[x].ready = TRUE;
				} 			
				ucRegisterFolge=INPUT_ON_READ;				
				break;
			
			case INPUT_ON_READ: // Input-Register-Abfrage speichern
								
				if(modWiloPu[puNummer].ucNeustart == 0) // Pumpe wurde neugestartet?
				{
					// Holding Register für "AUS" vorbelegen
					setHoldingOFF(&modWiloPu[puNummer]);
					modWiloPuData.ucNeustart = 1; // Merker für "neugestartet" setzen (wird später in memcpy auf Pumpe übertragen)
				}
			
				// SetPoint
				modWiloPuData.SetPoint_Input = modWiloPu[puNummer].wiloHoldReg.mwspSetValue.messw/2;
				
				// Commands Opmode: Aus / Ein / Min / Max 	
				modWiloPuData.Operation_Input = Get_Op_Decoder(modWiloPu[puNummer].wiloHoldReg.mwspPumpCommand.messw);
				
				// Commands Control Mode: Konstantdruck, Proportionaldruck, Konstantfrequenz, Automatik
				modWiloPuData.Control_Input = Get_Con_Decoder(modWiloPu[puNummer].wiloInReg.mwspCurrentOperationMode.messw);			
				
				// Betriebsstunden
				modWiloPuData.op_hours = (modWiloPuData.wiloInReg.mwspOperationHours.messw>0) ?  modWiloPuData.wiloInReg.mwspOperationHours.messw * 10:0;
				
				Get_Messages(); // Jeweilige Bitdeutung extrahieren (für PumpenStatus ErrorMessage usw)
			
				ucRegisterFolge = SET_HOLDING_REG;			
				break;		
				
			case SET_HOLDING_REG: // Wilo Holding Register Daten zur Kommunikation aus den Pumpenparametern lesen
			
				// Holding Register für "AUS" vorbelegen
				setHoldingOFF(&modWiloPuData);
				
				// Holding Register entsprechend der Pumpe setzen
				if ( wiloPuBusPara->Sollwert > 1000 )
				{
						modWiloPuData.wiloHoldReg.mwspSetValue.messw = 200;
				}
				else if ( wiloPuBusPara->Sollwert > 0 )
				{
						modWiloPuData.wiloHoldReg.mwspSetValue.messw =  ( wiloPuBusPara->Hand == FALSE ) ? wiloPuBusPara->Sollwert / 5 : wiloPuBusPara->SollHand / 5;
				}
				else
				{
						modWiloPuData.wiloHoldReg.mwspSetValue.messw = 0;
				}
							
				//Commands Opmode: Aus / Ein / Min / Max 	
				modWiloPuData.output_value1_temp = ( wiloPuBusPara->Hand == FALSE ) ? wiloPuBusPara->Betrieb : wiloPuBusPara->BetriebHand; // parli Anzeige
				modWiloPuData.wiloHoldReg.mwspPumpCommand.messw = Put_Op_Decoder(modWiloPuData.output_value1_temp); // Wilo dekodiert
		
				//Commands Control Mode: Konstantdruck, Proportionaldruck, Konstantfrequenz, Automatik
				modWiloPuData.output_value2_temp = ( wiloPuBusPara->Hand == FALSE ) ? wiloPuBusPara->Regelart :wiloPuBusPara->RegelartHand;
				modWiloPuData.wiloHoldReg.mwspOperationMode.messw = Put_Con_Decoder(modWiloPuData.output_value2_temp);				
								
				if ( wiloPuBusData->busSm )
				{
					modWiloPuData.wiloHoldReg.mwspBusCommandTimer.messw = 1;				// HoldingReg300 = OFF
					modWiloPuData.busSmWar = TRUE;
				}
				else
				{
					if ( modWiloPuData.busSmWar == TRUE )
					{
						if ( modWiloPuData.wiloHoldReg.mwspBusCommandTimer.messw == 1 )
						{
							modWiloPuData.busSmWar = FALSE;		// OFF ist angekommen
							modWiloPuData.wiloHoldReg.mwspBusCommandTimer.messw = wiloPuBusPara->HoldingReg300;				
						}
						else
							modWiloPuData.wiloHoldReg.mwspBusCommandTimer.messw = 1;			 			
					}
					else	
					{
						modWiloPuData.wiloHoldReg.mwspBusCommandTimer.messw = wiloPuBusPara->HoldingReg300;		// alles ok
					}					
				}	
				
				ucRegisterFolge=INIT_HOLDING_WRITE;// nach setzen der Holding Register diese schreiben							
				break;		
			
			case INIT_HOLDING_WRITE: // Schreiben der Holding Register starten
			
		 		// schreiben der Holding-Register aktivieren
				for(x=WILO_INPUT_REG_ANZ;x<(WILO_INPUT_REG_ANZ+WILO_HOLDING_REG_ANZ);++x)
				{
					modbusTableData[x].ready = TRUE;
				}			
			
				ucRegisterFolge = INIT_HOLDING_READ;	// nach schreiben der Holding Register diese wieder zurück lesen				
				break;
			
			case INIT_HOLDING_READ: // Lesen der Holding Register starten 
			
				// Holding Register der Kommunikation leeren
				memset(&modWiloPuData.wiloHoldReg,0,WILO_HOLDING_REG_ANZ); 
								
				// lesen der Holding-Register aktivieren
				for(x=(WILO_INPUT_REG_ANZ+WILO_HOLDING_REG_ANZ);x<(WILO_INPUT_REG_ANZ+WILO_HOLDING_REG_ANZ+WILO_HOLDING_REG_ANZ);++x)
				{
					modbusTableData[x].ready = TRUE;
				}		
				
				ucRegisterFolge = SET_PARA;	// gelesene Werte im nächsten Schritt verarbeiten
				break;
			
			case SET_PARA: // Anzeigeparameter entsprechend der gelesenen Register setzen
			
				// Kommunikationsfehler prüfen stat !=0 ==> Kommunikationsfehler
				if(modWiloPuData.wiloInReg.mwspActualDifferentialPressure.stat !=0 || modWiloPuData.wiloInReg.mwspFlowRate.stat !=0 || 
					 modWiloPuData.wiloInReg.mwspPowerConsumption.stat !=0 || modWiloPuData.wiloInReg.mwspPowerRating.stat !=0 || 
					 modWiloPuData.wiloInReg.mwspOperationHours.stat !=0 || modWiloPuData.wiloInReg.mwspMainsCurrent.stat !=0 || 
					 modWiloPuData.wiloInReg.mwspSpeed.stat !=0 || modWiloPuData.wiloInReg.mwspMediumTemperature.stat !=0 || 
					 modWiloPuData.wiloInReg.mwspCurrentOperationMode.stat !=0 || modWiloPuData.wiloInReg.mwspPumpModule.stat != 0 || 
					 modWiloPuData.wiloInReg.mwspPumpType.stat !=0 || modWiloPuData.wiloInReg.mwspMaxSpeed.stat != 0 || 
					 modWiloPuData.wiloInReg.mwspMinSpeed.stat != 0 || modWiloPuData.wiloInReg.mwspSupportedErrors.stat !=0 || 
					 modWiloPuData.wiloInReg.mwspSupportedServiceMessages.stat != 0 || modWiloPuData.wiloInReg.mwspMaxPowerRating.stat !=0 ||
					 modWiloPuData.wiloInReg.mwspServiceMessage.stat != 0 || modWiloPuData.wiloInReg.mwspErrorType.stat != 0 ||
					 modWiloPuData.wiloInReg.mwspErrorMessage.stat != 0 || modWiloPuData.wiloInReg.mwspPumpStatus.stat != 0 ||            
					 modWiloPuData.wiloInReg.mwspStateDiagnostics.stat !=0 || modWiloPuData.wiloHoldReg.mwspSetValue.stat != 0 ||
					 modWiloPuData.wiloHoldReg.mwspPumpCommand.stat != 0 || modWiloPuData.wiloHoldReg.mwspOperationMode.stat!= 0 || 
					 modWiloPuData.wiloHoldReg.mwspBusCommandTimer.stat !=0)
				{ 
					wiloPuBusData->busSm = TRUE;
				}	
				else
				{
					wiloPuBusData->busSm = FALSE;
				}	
			
				modWiloPuData.siMediumTemperature = modWiloPuData.wiloInReg.mwspMediumTemperature.messw - 2732; // Kelvin in Celsius
			
				// Textzuweisung für Betriebszustände usw
				if ( modWiloPuData.output_value1_temp <= 4 )
					memcpy ( &wiloPuBusData->betriebsArtSoll, pu_operation_set[modWiloPuData.output_value1_temp], 15 );
				else
					memcpy ( &wiloPuBusData->betriebsArtSoll, ClearDisplay, 15 );
				if ( modWiloPuData.Operation_Input <= 4 )
					memcpy ( &wiloPuBusData->betriebsArtIst, pu_operation_set[modWiloPuData.Operation_Input], 15 );
				else
					memcpy ( &wiloPuBusData->betriebsArtIst, ClearDisplay, 15 );
				if ( modWiloPuData.output_value2_temp <= 4 )
					memcpy ( &wiloPuBusData->regelArtSoll, pu_control_set[modWiloPuData.output_value2_temp], 15 );
				else
					memcpy ( &wiloPuBusData->regelArtSoll, ClearDisplay, 15 );
				if ( modWiloPuData.Control_Input <= 4 )
					memcpy ( &wiloPuBusData->regelArtIst, pu_control_set[modWiloPuData.Control_Input], 15 );
				else
					memcpy ( &wiloPuBusData->regelArtIst, ClearDisplay, 15 );			
				
				memcpy(&modWiloPu[puNummer],&modWiloPuData,sizeof(ModBusWiloPumpen));  // Pumpenparameter für jeweilige Pumpe übernehmen
								
				puNummer = (++puNummer<BUS_PU_MAX) ? puNummer:0; // Druchlauf für nächste Pumpe
												
				ucRegisterFolge = INIT_INPUT_READ; // nach lesen der Holding Register wieder bei lesen der Input Register beginnen				
				break;
			
			default: // Im Fehlerfall immer von vorne bei der erste Pumpe beginnen
				ucRegisterFolge = INIT_INPUT_READ;
				puNummer=0;
				break;
		}	// switch (ucRegisterFolge)
	}	// Ende if(ucBusKommunikation==FALSE)	
}

// Commands Opmode: Aus / Ein / Min / Max 	
unsigned int Get_Op_Decoder(unsigned int GetOp)
{
	switch(GetOp)
	{
		case 8:		return 0;
		case 9:		return 1;
		case 10:	return 2;
		case 12:	return 3;
		default:	return 4;
	}
}

unsigned int Put_Op_Decoder(unsigned int PutOp)
{
	switch(PutOp)
	{
		case 0:		return 8;
		case 1:		return 9;
		case 2:		return 10;
		case 3:		return 12;
		default:	return 9;
	}
}

// Commands Control Mode: Konstantdruck, Proportionaldruck, Konstantfrequenz, Automatik
unsigned int Get_Con_Decoder(unsigned int GetCon)
{	
	switch(GetCon)
	{	
		case 1:		return 2;
		case 3:		return 0;
		case 4:		return 1;
		case 6:		return 3;
		default:	return 4;	// ungültiger Wert
	}
}

unsigned int Put_Con_Decoder(unsigned int PutCon)
{
	switch(PutCon)
	{
		case 0:		return 3;
		case 1:		return 4;
		case 2:		return 1;
		case 3:		return 6;
		default:	return 3;
	}
}

void Get_Messages (void)
{
	char i;
	unsigned int	msg, mask;

	mask = 0x0001;

	modWiloPuData.ucGlobalFlag = 0;
	wiloPuBusData->puAlarm = 0;

// Service Message
	msg	= ( modWiloPuData.wiloInReg.mwspServiceMessage.messw & 0x000F ); // inrelevante Bit maskieren
	if(msg)
	{
		for(i=1; i<4; i++)
		{
			if(msg	&	mask)		
				break;
			mask <<= 1;
		}
		modWiloPuData.ucServiceMessage = i;		
		mask ^= 0xFFFF;	// Für Test auf Doppelbelegung, Vorläufig
		if(msg	&	mask)		
			modWiloPuData.ucGlobalFlag = 1;	// Vorläufig global! event. Doppelbelegungen markieren
	}		
	else
		modWiloPuData.ucServiceMessage = 0;		

// Error Type
	msg	= ( modWiloPuData.wiloInReg.mwspErrorType.messw & 0x001B ); // inrelevante Bit maskieren
	if(msg)
	{
		wiloPuBusData->puAlarm = 1;		// Störungsmeld. der der Pumpe 
		mask = 0x0001;
		for(i=1; i<5; i++)
		{
			if(msg	&	mask)		
				break;
			mask <<= 1;
		}
		modWiloPuData.ucErrorType = i;		
		mask ^= 0xFFFF;
		if(msg	&	mask)		
			modWiloPuData.ucGlobalFlag = 1;
	}		
	else
		modWiloPuData.ucErrorType = 0;		

// Error Message
	msg	= ( modWiloPuData.wiloInReg.mwspErrorMessage.messw & 0x7F3F ); // inrelevante Bit maskieren
	if(msg)
	{
		mask = 0x0001;
		for(i=1; i<15; i++)
		{
			if(msg	&	mask)		
				break;
			mask <<= 1;
		}
		modWiloPuData.ucErrorMessage = i;		
		mask ^= 0xFFFF;
		if(msg	&	mask)		
			modWiloPuData.ucGlobalFlag = 1;
	}		
	else
		modWiloPuData.ucErrorMessage = 0;

// 	Pump Status	
// Betriebsmeldung separieren d.h. Bit 0 einzeln testen dann maskieren. Nur wenn sinnvoll, noch prüfen
	msg	= ( modWiloPuData.wiloInReg.mwspPumpStatus.messw & 0x20FF ); // inrelevante Bit maskieren
	if(msg)
	{
		mask = 0x0001;
		for(i=1; i<14; i++)
		{
			if(msg	&	mask)		
				break;
			mask <<= 1;
		}
		modWiloPuData.ucPumpStatus = i;		
		mask ^= 0xFFFF;
		if(msg	&	mask)		
			modWiloPuData.ucGlobalFlag = 1;
	}		
	else
		modWiloPuData.ucPumpStatus = 0;		

// 	State Diagnose	
	msg	= ( modWiloPuData.wiloInReg.mwspStateDiagnostics.messw & 0x055B ); // inrelevante Bit maskieren
	if(msg)
	{
		if (modWiloPuData.wiloInReg.mwspStateDiagnostics.messw & 0x03)
		{
			wiloPuBusData->puAlarm = 1;		// Störungsmeld. der der Pumpe 
		}
		mask = 0x0001;
		for(i=1; i<11; i++)
		{
			if(msg	&	mask)		
				break;
			mask <<= 1;
		}
		modWiloPuData.ucStateDiagnostics = i;		
		mask ^= 0xFFFF;
		if(msg	&	mask)		
			modWiloPuData.ucGlobalFlag = 1;	// Vorläufig! event. Doppelbelegungen global markieren
	}		
	else
		modWiloPuData.ucStateDiagnostics = 0;		
		
	// Textzuweisung zu den verschiedenen "Messages"
	if ( modWiloPuData.ucServiceMessage > 4 )
		modWiloPuData.ucServiceMessage = 5;
	memcpy ( modWiloPuData.strServiceMessage, wilo_servstat_set[modWiloPuData.ucServiceMessage], 15 );
	
	if ( modWiloPuData.ucErrorType > 5 )
		modWiloPuData.ucErrorType = 6;
	memcpy ( modWiloPuData.strErrorType, wilo_error_set[modWiloPuData.ucErrorType],		 15 );
	
	if ( modWiloPuData.ucErrorMessage > 15 )
		modWiloPuData.ucErrorMessage = 16;
	memcpy ( modWiloPuData.strErrorMessage, wilo_errorstat_set[modWiloPuData.ucErrorMessage],15 );
	
	if ( modWiloPuData.ucPumpStatus > 14 )
		modWiloPuData.ucPumpStatus = 15;
	memcpy ( modWiloPuData.strPumpStatus, wilo_pustat_set[modWiloPuData.ucPumpStatus],	 15 );
	
	if ( modWiloPuData.ucStateDiagnostics > 11 )
		modWiloPuData.ucStateDiagnostics = 12;
	memcpy ( modWiloPuData.strStateDiagnostics, wilo_statdiag_set[modWiloPuData.ucStateDiagnostics], 15 );
}

// vorbelegen der Holding Register für "AUS"
void setHoldingOFF (ModBusWiloPumpen *modbusWilo)
{
	modbusWilo->wiloHoldReg.mwspSetValue.messw = 0;
	modbusWilo->wiloHoldReg.mwspPumpCommand.messw = 4;
	modbusWilo->wiloHoldReg.mwspOperationMode.messw = 4;
	modbusWilo->wiloHoldReg.mwspBusCommandTimer.messw = 0;		
}

#endif  // WILO_MODBUS == 1
