/********************************************************************
 *																																	*
 *					U_GenibusSio.c																					*
 *					Handler für Genibus																			*
 *					Version: 07.01.2013		anbu															*
 *																																	*
 ********************************************************************/
 
#include "sfr32C87.h"
#include <string.h>
#include "sio.h"
#include "timer.h"
#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "projdef.h"

//#include "Genibus/genibus_defines.h"
#include "genibus_defines.h"




void Genibus_Sio_Init(char port);
void Genibus_Start_Transmit(char port);
void Genibus_Rx_Ready(char port);
void Genibus_Rx_DeInit(char port);

	

// ##################################### Sio-Programme ########################################################
// Aufruf durch UserHandler.c

#if ( (( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) ||  (( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )

// ------------------------------------- Sende Interrupt ------------------------------------------------------
// Senden der Bytes im TX Buffer
void U_Genibus_ISR_Tx_Int(char port)
	{
		switch (port)
			{
				case S2:
					if (TIR_BIT_S2 == 1)													// Sendeinterrupt ?
						{
							TIR_BIT_S2	= 0;														// Interrupt Request Bit löschen
							
							if (uc_genibus_tx_count < uc_genibus_tx_lenght)
								{
									TB_S2	=	TxBuf_S2[uc_genibus_tx_count++];	// Bytes	senden
								}
							else		 
								{
									TE_BIT_S2		= 0;													// Sendeerlaubnis sperren
									TIE_BIT_S2	= 0;													// Interrupt Enable  Bit löschen
									TIC_S2 			= 0x00;												// Disable	Interrupt	Request
				
									P_TOUTR_S2	=	uc_genibus_rts_prescaler;		// Load  Prescaler
									C_TOUTR_S2	=	uc_genibus_rts_timer;				// Load  Counter
									P_TOUTS_S2	= 1;													// Start	 GENI-RTS Prescaler
									C_TOUTS_S2	= 1;													// Start	 GENI-RTS Timer Counter
								}
						}
					break;
		
				case S3:
					if (uc_genibus_tx_count < uc_genibus_tx_lenght)
						{
							TB_S3	=	TxBuf_S3[uc_genibus_tx_count++];			// Bytes	senden
						}
					else
						{
							TE_BIT_S3		= 0;															// Sendeerlaubnis sperren
							TIC_S3 			= 0x00;														// Disable	Interrupt	Request
							                                			
							P_TOUTR_S3	=	uc_genibus_rts_prescaler;				// Load  Prescaler
							C_TOUTR_S3	=	uc_genibus_rts_timer;						// Load  Counter
							P_TOUTS_S3	= 1;															// Start	 GENI-RTS Prescaler
							C_TOUTS_S3	= 1;															// Start	 GENI-RTS Timer Counter
						}
					break;
			}
	}

// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Empfangs Interrupt ---------------------------------------------------
// Speichert die empfangenen Bytes im RX Buffer

void U_Genibus_ISR_Rx_Int(char port)
	{																																																																												
		if ( port == S2 )
			{	
				ucf_genibus_rx_buffer[uc_genibus_rx_count] = RB_S2;					// Empfangenes Byte speichern	
							
				if (SUM_BIT_S2 == 1)																				// Summenfehlerbit gesetzt ?
					{	
						uc_genibus_rx_errorflag = RX_SIO_ERROR;										// Flag setzen
						Genibus_Rx_DeInit(port);																	// Neuinitialisieren
					}
						
				RIR_BIT_S2 = 0;																							// Interrupt Request Bit löschen
			}
		else if ( port == S3 )
			{
				ucf_genibus_rx_buffer[uc_genibus_rx_count] = RB_S3;					// Empfangenes Byte speichern
				
				if (SUM_BIT_S3 == 1)																				// Summenfehlerbit gesetzt ?
					uc_genibus_rx_errorflag = RX_SIO_ERROR;											// Flag setzen				
			}					
					
		if ( ++uc_genibus_rx_count >= uc_genibus_rx_buff_size)				// Ende vom Empfangsbuffer erreicht?
			uc_genibus_rx_count = 0;																			// ggf.zurücksetzen
			
		uc_genibus_rx_telegram_ready = RECEIVING_TELEGRAM;								// Status (zurück)setzen
	}
// ------------------------------------------------------------------------------------------------------------							 


// ------------------------------------- Timer für RTS --------------------------------------------------------
// Timer Interrupt für RTS-OFF

void U_Genibus_ISR_Tel_Tout(char port)
	{
#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
		if (port == S2)
			{
				P_TOUTS_S2	= 0;		// Stop	 GENI-RTS Prescaler
				C_TOUTS_S2	= 0;		// Stop	 GENI-RTS Timer Counter
				RTS_S2			=	0;		// Sendekanal sperren
			}
#endif
	
#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL		// ToDo Bei S3 eigntl nicht benötigt
		if (port == S3)
			{
				P_TOUTS_S3	= 0;		// Stop	 GENI-RTS Prescaler
				C_TOUTS_S3	= 0;		// Stop	 GENI-RTS Timer Counter
				RTS_S3			=	0;		// Sendekanal sperren
			}
#endif

		Genibus_Rx_Ready(port);
	}
// ------------------------------------------------------------------------------------------------------------

		
// ------------------------------------- Interrupt Baudratenmessung -------------------------------------------
     
void U_Genibus_ISR_BaudCheck(char port)
	{
		// ToDo: Könnten noch eingefügt werden
	}
// ------------------------------------------------------------------------------------------------------------	
	
// ##################################### Ende Sio-Programme ###################################################



// ##################################### Unterprogramme #######################################################

// ------------------------------------- Sio Initalisierung ---------------------------------------------------

void Genibus_Sio_Init(char port)
	{
#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
		if (port == S2)
			{
				RTS_S2				=	0;				// Empfangskanal freigeben RTS=Low
				MR_S2 				= 0x05;			// Transmit Receive Mode Register = UART Mode 8 Bit
				C0_S2 				= 0x10;			// Transmit Receive Control Register 0
			
				RIRLT_BIT_S2 	= 1;				// Interrupt Request Select Bit im Interrupt Enable  Register setzen  auf used for interrupt
				RIR_BIT_S2   	= 0;				// Interrupt Request Bit 				im Interrupt Request Register löschen
				RIE_BIT_S2   	= 1;				// Interrupt Enable  Bit 				im Interrupt Enable  Register setzen
			
//				TB_S2 				= RB_S2;		// SIO entleeren	(Kopieren auf Sendepuffer)
				TB_S2 				= 0;				// Sendepuffer löschen
			
				DISABLE_IRQ	
				RIC_S2 				= 0x00;			// Disable Receive Interrupt Prio: 4	(0x04)		
				ENABLE_IRQ
					
			// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
				C1_S2 				= 0x00;			// Transmit Receive Control Register 1		
				RE_BIT_S2 		= 0;				// Empfang noch nicht	erlauben 
				
				BRG_S2 = (unsigned char)( ( (f1_CLK_SPEED/16) / 9600) -1);
		
				uc_genibus_rts_prescaler 	= 64;		// Vorteiler: 64
				uc_genibus_rts_timer 			= 32;		// war 4800;
			
				P_TOUTS_S2		= 0;				// Stop	 GENI-RTS Prescaler
				C_TOUTS_S2		= 0;				// Stop	 GENI-RTS Timer Counter
		                          		
				TIRLT_BIT_S2 	= 1;				// Interrupt Request Select Bit im Interrupt Enable  Register (IIO1IE) setzen  auf used for interrupt
				TIR_BIT_S2	 	= 0;				// Interrupt Request Bit 				im Interrupt Request Register (IIO1IR) löschen
				TIE_BIT_S2	 	= 1;				// Interrupt Enable  Bit 				im Interrupt Enable  Register (IIO1IE) setzen
			}
#endif
	
#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
		if (port == S3)
			{
				RTS_S3				=	0;				// Empfangskanal freigeben RTS=Low
				MR_S3 				= 0x05;			// Transmit Receive Mode Register = UART Mode 8 Bit
				C0_S3 				= 0x10;			// Transmit Receive Control Register 0
//				TB_S3 				= RB_S3;		// SIO entleeren	(Kopieren auf Sendepuffer)
				TB_S3 				= 0;				// Sendepuffer löschen
			
				DISABLE_IRQ			
				RIC_S3 				= 0x04;			// Enable Receive Interrupt Prio: 4			
				ENABLE_IRQ	
				
			// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
				C1_S3 				= 0x00;			// Transmit Receive Control Register 1		
				RE_BIT_S3 		= 1;				// Empfang erlauben 		
																								
				BRG_S3 				= (unsigned char)( ( (f1_CLK_SPEED/16) / 9600) -1);
				
				uc_genibus_rts_prescaler 	= 64;		// Vorteiler: 64
				uc_genibus_rts_timer 			= 32;		// war 4800;
			
				P_TOUTS_S3		= 0;				// Stop	 GENI-RTS Prescaler
				C_TOUTS_S3		= 0;				// Stop	 GENI-RTS Timer Counter
			}
#endif
	}			
// ------------------------------------------------------------------------------------------------------------	


// ------------------------------------- Start Transmitter ----------------------------------------------------
// Transmitter Start-Routine für GENIbus über TX-S2 und TX-S3

void Genibus_Start_Transmit(char port)
	{	
#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
		if (port == S2)
			{
				RTS_S2		= 1;																								// Transmitter Enable		
		
				delay(1);																											// 100µs Zwangspause für eine stabile Startbit-Breite
				uc_genibus_tx_lenght 	= uc_genibus_tx_count;									// Telegrammlänge	für	Sendeinterrupt
				
			// SIO	-	Initialisieren und Sendeinterrupt	vorbereiten	
				DISABLE_IRQ			
				RIC_S2 								= 0x00;			// Disable Receive Interrupt Prio: 4	(0x04)	
				ENABLE_IRQ
			
				uc_genibus_tx_count		= 1;			 															// Sendepuffer Index	
				TE_BIT_S2 						= 1;																		// Senden erlauben
				TIRLT_BIT_S2 					= 1;																		// Interrupt Request Select Bit im Interrupt Enable  Register (IIO1IE) setzen  auf used for interrupt
				TIR_BIT_S2	 					= 0;																		// Interrupt Request Bit 				im Interrupt Request Register (IIO1IR) löschen
				TIE_BIT_S2	 					= 1;																		// Interrupt Enable  Bit 				im Interrupt Enable  Register (IIO1IE) setzen     
				                    	              													
				DISABLE_IRQ			    	              													
				TIC_S2 								= 0x04;																	// Enable Transmit Interrupt Prio: 4			
				ENABLE_IRQ	        	              													
				TB_S2	 								= TxBuf_S2[0];													// Beginnen mit erstem Byte
			}
#endif
	
#if (IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL
		if (port == S3)
			{
				RTS_S3								= 1;																		// Transmitter Enable			
			
				delay(1);																											// 100µs Zwangspause für eine stabile Startbit-Breite
				uc_genibus_tx_lenght 	= uc_genibus_tx_count;									// Telegrammlänge	für	Sendeinterrupt
				
			// SIO	-	Initialisieren und Sendeinterrupt	vorbereiten			
				uc_genibus_tx_count		= 1;			 															// Sendepuffer Index	
				TE_BIT_S3 						= 1;																		// Senden erlauben
				
				DISABLE_IRQ			
				TIC_S3 								= 0x04;																	// Enable Transmit Interrupt Prio: 4			
				ENABLE_IRQ	
				TB_S3	 								= TxBuf_S3[0];					  						  // Beginnen mit erstem Byte
			}
#endif
	}
// ------------------------------------------------------------------------------------------------------------	


// ------------------------------------- Deinitialisieren -----------------------------------------------------

void Genibus_Rx_DeInit(char port)
	{	
#if (IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL
		if (port == S2)
			{
				RE_BIT_S2 	= 0;					// Empfang sperren, löschen der Fehlerbits
				RIE_BIT_S2  = 0;					// Interrupt Enable  Bit 	löschen
				
				DISABLE_IRQ			
				RIC_S2 			= 0x00;				// Disable Receive Interrupt 
				ENABLE_IRQ	
			}
#endif
	}
// ------------------------------------------------------------------------------------------------------------	


// ------------------------------------- Empfang Vorbereitung -------------------------------------------------
void Genibus_Rx_Ready(char port)
	{
		uc_genibus_rx_count			=	0;		// Empfangspuffer Index zurücksetzen
		uc_genibus_rx_errorflag = 0;
//		gb_rx_empty = 0;
	
		if (port == S2)
			{
				RIRLT_BIT_S2 	= 1;					// Interrupt Request Select Bit im Interrupt Enable  Register setzen  auf used for interrupt
				RIR_BIT_S2   	= 0;					// Interrupt Request Bit 				im Interrupt Request Register löschen
				RIE_BIT_S2   	= 1;					// Interrupt Enable  Bit 				im Interrupt Enable  Register setzen
				
				DISABLE_IRQ			
				RIC_S2 				= 0x04;				// Enable Receive Interrupt Prio: 4			
				ENABLE_IRQ	
				
			// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
				C1_S2 = 0x00;								// Transmit Receive Control Register 1		
				RE_BIT_S2 = 1;							// Empfang erlauben 					
			}	
	}	
// ------------------------------------------------------------------------------------------------------------	

// ##################################### Ende Unterprogramme ##################################################
#endif // ( (( IMPLEMENT_S2 & GENI1_IMPL) == GENI1_IMPL ) ||  (( IMPLEMENT_S3 & GENI1_IMPL) == GENI1_IMPL ) )