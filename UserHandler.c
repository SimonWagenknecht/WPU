/*-----------------------------------------------------------------------------------------------------	
		Task:		 UserHandler	für Schnittstellen S1, S2 und S3

		Einsprünge erfolgen aus den System-Task's HandlerS1 bis HandlerS3
		und aus den entsprechenden Interrupt-Service-Routinen SioS1 bis SioS3
		
		Die Funktionsausführung erfolgt abhängig vom eingestellten Parameter Funktion_S1 bis Funktion_S3

--------------------------------------------------------------------------------------------------------*/		

#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "projdef.h"

//-------------------------------------------------
// externe Routinen
extern char U_Test_RS485(char port, char funktion);
extern void U_Test_ISR_BaudCheck(char port);
extern void U_Test_ISR_Tel_Tout(char port);
extern void U_Test_ISR_Tx_Int(char port);
extern void U_Test_ISR_Rx_Int(char port);

extern char U_Funkempf(char port, char funktion);
extern void U_Funkempf_ISR_BaudCheck(char port);
extern void U_Funkempf_ISR_Tel_Tout(char port);
extern void U_Funkempf_ISR_Tx_Int(char port);
extern void U_Funkempf_ISR_Rx_Int(char port);

extern char U_Genibus(char port, char funktion);
extern void U_Genibus_ISR_BaudCheck(char port);
extern void U_Genibus_ISR_Tel_Tout(char port);
extern void U_Genibus_ISR_Tx_Int(char port);
extern void U_Genibus_ISR_Rx_Int(char port);

extern char U_Mbus ( char port, char funktion );
extern void U_Mbus_ISR_BaudCheck ( char port );
extern void U_Mbus_ISR_Tel_Tout ( char port );
extern void U_Mbus_ISR_Tx_Int ( char port );
extern void U_Mbus_ISR_Rx_Int ( char port );

extern char U_Modbus(char port, char funktion);
extern void U_Modbus_ISR_BaudCheck(char port);
extern void U_Modbus_ISR_Tel_Tout(char port);
extern void U_Modbus_ISR_Tx_Int(char port);
extern void U_Modbus_ISR_Rx_Int(char port);

extern char U_Anybus ( char port, char funktion );
extern void U_Anybus_ISR_BaudCheck ( char port );
extern void U_Anybus_ISR_Tel_Tout ( char port );
extern void U_Anybus_ISR_Tx_Int ( char port );
extern void U_Anybus_ISR_Rx_Int ( char port );

//--------------------------------------------------

//------------------------------------------------------------
// Einprung vom HandlerS1, HandlerS2 oder HandlerS3
//------------------------------------------------------------
char UserHandler(char port, char funktion)
{
	
	if(port == S1)
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				funktion = U_Funkempf(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				funktion = U_Genibus(port, funktion);
				break;
			#endif

			#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				funktion = U_Modbus(port, funktion);
				break;
			#endif
			
			#if (IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				funktion = U_Test_RS485(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				funktion = U_Mbus ( port, funktion );
				break;
			#endif

			default:
				funktion = GBUS_FU;		// RS485 RIEcon Gebäudebus ist die Standardfunktion der S1, S2, S3 Schnittstelle
				break;
		}
	}
	else if(port == S2)
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				funktion = U_Funkempf(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				funktion = U_Genibus(port, funktion);
				break;
			#endif

			#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				funktion = U_Modbus(port, funktion);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				funktion = U_Test_RS485(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				funktion = U_Mbus ( port, funktion );
				break;
			#endif

			default:
				funktion = GBUS_FU;		// RS485 RIEcon Gebäudebus ist die Standardfunktion der S1, S2, S3 Schnittstelle
				break;
		}
	}
	else if(port == S3)
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				funktion = U_Funkempf(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				funktion = U_Genibus(port, funktion);
				break;
			#endif

			#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				funktion = U_Modbus(port, funktion);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				funktion = U_Test_RS485(port, funktion);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS232 MBus
				funktion = U_Mbus ( port, funktion );
				break;
			#endif

			#if (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL
			case ANYBUS1_FU:																	// RS232 AnyBus
				funktion = U_Anybus ( port, funktion );
				break;
			#endif

			default:
				funktion = GBUS_FU;		// RS485 RIEcon Gebäudebus ist die Standardfunktion der S1, S2, S3 Schnittstelle
				break;
		}
	}

	return(funktion);
}

//--------------------------------------------------

/* ------------- Interrupt Service Routine: SIO Sendeinterrupt ---------------------------*/
/* Einsprung vom System-Interrupt: ISR_Tx_Int_S1	_S2  oder _S3																				*/
/*----------------------------------------------------------------------------------------*/
/* Eintrag in Vektortabelle (sect308_R36.inc) unter: uart2 trance/NACK (software int 33)*/
/*----------------------------------------------------------------------------------------*/
void U_ISR_Tx_Int(char port, char funktion)
{	
	if(port == S1)
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tx_Int(port);
				break;
			#endif

			#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																		// RS485 MBus
				U_Mbus_ISR_Tx_Int ( port );
				break;
			#endif

			default:
				break;
		}
	}	
	else if(port == S2)	
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																		// RS485 MBus
				U_Mbus_ISR_Tx_Int ( port );
				break;
			#endif

			default:
				break;
		}			
	}
	else if(port == S3)	
	{		
		switch(funktion)
		{
			#if (IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tx_Int(port);
				break;
			#endif
	
			#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																		// RS232 MBus
				U_Mbus_ISR_Tx_Int ( port );
				break;
			#endif

			#if (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL
			case ANYBUS1_FU:																	// RS232 AnyBus
				U_Anybus_ISR_Tx_Int ( port );
				break;
			#endif

			default:
				break;
		}			
	}
}

/* ------------- Interrupt Service Routine: SIO Empfangsinterrupt ------------------------*/
/*	Einsprung vom System-Interrupt: ISR_Rx_Int_S1 _S2 oder _S3														*/
/* ---------------------------------------------------------------------------------------*/
/*	Eintrag in Vektortabelle (sect308_R36.inc) unter: uart2 receive/ACK (software int 34)	*/
/*----------------------------------------------------------------------------------------*/
void  U_ISR_Rx_Int(char port, char funktion)
{	
	if(port == S1)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Rx_Int(port);
				break;
			#endif

			#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Rx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_Rx_Int ( port );
				break;
			#endif

			default:
				break;
		}			
	}
	else if(port == S2)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Rx_Int(port);
				break;
			#endif

			#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Rx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_Rx_Int ( port );
				break;
			#endif

			default:
				break;
		}			
	}
	else if(port == S3)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Rx_Int(port);
				break;
			#endif

			#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Rx_Int(port);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Rx_Int(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS232 MBus
				U_Mbus_ISR_Rx_Int ( port );
				break;
			#endif

			#if (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL
			case ANYBUS1_FU:																	// RS232 MBus
				U_Anybus_ISR_Rx_Int ( port );
				break;
			#endif

			default:
				break;
		}			
	}
}
		
/* ------------- Interrupt Service Routine: Timeout ----------------------------------*/
/*	Einsprung vom System-Interrupt: ISR_Tel_Tout_S1	_S2 oder _S3											*/
/*------------------------------------------------------------------------------------*/
void U_ISR_Tel_Tout(char port, char funktion)
{
	if(port == S1)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tel_Tout(port);
				break;
			#endif
	
			#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tel_Tout(port);
				break;
			#endif	
			
			#if (IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_Tel_Tout ( port );
				break;
			#endif
	
			default:
				break;
		}			
	}	
	else if(port == S2)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tel_Tout(port);
				break;
			#endif

			#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tel_Tout(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_Tel_Tout ( port );
				break;
			#endif

			default:
				break;
		}			
	}	
	else if(port == S3)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:																	// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:																	// RS485 Genibus Pumpen
				U_Genibus_ISR_Tel_Tout(port);
				break;
			#endif

			#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_Tel_Tout(port);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_Tel_Tout(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS232 MBus
				U_Mbus_ISR_Tel_Tout ( port );
				break;
			#endif

			#if (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL
			case ANYBUS1_FU:																	// RS232 AnyBus
				U_Anybus_ISR_Tel_Tout ( port );
				break;
			#endif

			default:
				break;
		}			
	}
}

/*-------------- Interrupt Service Routine: Baudcheck --------------------------------------*/
/* Einsprung vom System-Interrupt:  ISR_BaudCheck_S1 _S2 oder _S3														*/
/*------------------------------------------------------------------------------------------*/
void U_ISR_BaudCheck(char port, char funktion)
{
	if(port == S1)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S1 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:							// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:							// RS485 Genibus Pumpen
				U_Genibus_ISR_BaudCheck(port);
				break;
			#endif

			#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_BaudCheck(port);
				break;
			#endif
			
			#if (IMPLEMENT_S1 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S1 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_BaudCheck ( port );
				break;
			#endif

			default:
				break;
		}			
	}
	else if(port == S2)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S2 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:							// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:							// RS485 Genibus Pumpen
				U_Genibus_ISR_BaudCheck(port);
				break;
			#endif

			#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_BaudCheck(port);
				break;
			#endif
			
			#if (IMPLEMENT_S2 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S2 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS485 MBus
				U_Mbus_ISR_BaudCheck ( port );
				break;
			#endif

			default:
				break;
		}			
	}
	else if(port == S3)
	{
		switch(funktion)
		{
			#if (IMPLEMENT_S3 & FUNK1_IMPL) == FUNK1_IMPL
			case FUNK1_FU:							// RS485 Funkmodul EnOcean 
				U_Funkempf_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
			case GENI1_FU:							// RS485 Genibus Pumpen
				U_Genibus_ISR_BaudCheck(port);
				break;
			#endif

			#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
			case MODBUS1_FU:																	// RS485 oder RS232 MODBUS Master
				U_Modbus_ISR_BaudCheck(port);
				break;
			#endif
			
			#if (IMPLEMENT_S3 & GBUS1_IMPL) == GBUS1_IMPL
			case GBUS1_FU:																	// Test-RS485
				U_Test_ISR_BaudCheck(port);
				break;
			#endif
				
			#if (IMPLEMENT_S3 & MBUS1_IMPL) == MBUS1_IMPL
			case MBUS1_FU:																	// RS232 MBus
				U_Mbus_ISR_BaudCheck ( port );
				break;
			#endif

			#if (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL
			case ANYBUS1_FU:																	// RS232 AnyBus
				U_Anybus_ISR_BaudCheck ( port );
				break;
			#endif

			default:
				break;
		}			
	}
}

				