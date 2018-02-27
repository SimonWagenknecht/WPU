/////////////////////////////////////////////////////////////////////////////////
//                       ---- MODBUS Interupt-Service ----                     //
//  										Basis für alle ModBus-Applikationen                    //
/////////////////////////////////////////////////////////////////////////////////

#include "sfr32C87.h"
#include <string.h>
#include "sio.h"
#include "timer.h"
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "projdef.h"
#include "modbus.h"

#if ( MODBUS_SIO )
void start_modb_transmit(char port);
void set_EoF_timer(char port);
void Modb_Rx_Ready(char port);
void Modb_Rx_DeInit(char port);
int modbus_crc(char *ptr, char Length);

///////////////////////////////////////////////////////////////////
// Sende Interrupt
///////////////////////////////////////////////////////////////////
void U_Modbus_ISR_Tx_Int(char port)
{
	switch(port)
	{
		case S1:
			if(modb_tx_count[port] < modb_leng_tx[port])
			{
					TB_S1	=	TxBuf_S1[modb_tx_count[port]++];		// Bytes	senden
			}
			else	// Ende	 
			{
				TE_BIT_S1		= 0;								// Sendeerlaubnis sperren
				TIC_S1 			= 0x00;							// Disable	Interrupt	Request
//				RTS_S1			=	0;							// Sendekanal sperren
//				Modb_Rx_Ready(port);	nach Timer-IT verlagert
				timer_function[port] = 1;							// Für Timer-ISR: RTS-Delay aktivieren
				P_TOUTR_S1	=	modb_rts_prescaler[port];	// Load  Prescaler
				C_TOUTR_S1	=	modb_rts_timer[port];			// Load  Counter
				P_TOUTS_S1	= 1;								// Start	 RTS Prescaler
				C_TOUTS_S1	= 1;								// Start	 RTS Timer Counter
			}
			break;

		case S2:
			if(TIR_BIT_S2 == 1)							// Sendeinterrupt ?
			{
				TIR_BIT_S2	= 0;							// Interrupt Request Bit löschen
				if(modb_tx_count[port] < modb_leng_tx[port])
				{
						TB_S2	=	TxBuf_S2[modb_tx_count[port]++];		// Bytes	senden
				}
				else	// Ende	 
				{
					TE_BIT_S2		= 0;							// Sendeerlaubnis sperren
					TIE_BIT_S2	= 0;							// Interrupt Enable  Bit löschen
					TIC_S2 			= 0x00;						// Disable	Interrupt	Request

					timer_function[port] = 1;						// Für Timer-ISR: RTS-Delay aktivieren
					P_TOUTR_S2	=	modb_rts_prescaler[port];		// Load  Prescaler
					C_TOUTR_S2	=	modb_rts_timer[port];				// Load  Counter
					P_TOUTS_S2	= 1;							// Start	 RTS Prescaler
					C_TOUTS_S2	= 1;							// Start	 RTS Timer Counter
				}
			}
			break;

		case S3:
			if(modb_tx_count[port] < modb_leng_tx[port])
			{
					TB_S3	=	TxBuf_S3[modb_tx_count[port]++];		// Bytes	senden
			}
			else	// Ende	 
			{
				TE_BIT_S3		= 0;								// Sendeerlaubnis sperren
				TIC_S3 			= 0x00;							// Disable	Interrupt	Request
//				RTS_S3			=	0;							// Sendekanal sperren
//				Modb_Rx_Ready(port);	nach Timer-IT verlagert
				timer_function[port] = 1;							// Für Timer-ISR: RTS-Delay aktivieren
				P_TOUTR_S3	=	modb_rts_prescaler[port];	// Load  Prescaler
				C_TOUTR_S3	=	modb_rts_timer[port];			// Load  Counter
				P_TOUTS_S3	= 1;								// Start	 RTS Prescaler
				C_TOUTS_S3	= 1;								// Start	 RTS Timer Counter
			}
			break;
	}
}

///////////////////////////////////////////////////////////////////
// Empfangs Interrupt
///////////////////////////////////////////////////////////////////
void U_Modbus_ISR_Rx_Int(char port)
{
	char	rbyte;
	char	sio_err;
	char *pRxBuf;	

	pRxBuf = NULL;
	rbyte = 0;
	sio_err = 0;

	switch(port)
	{
		case S1:
			pRxBuf  = RxBuf_S1;
			modb_rx_buff_size = S1_RXSIZE;
			rbyte 	= RB_S1;			
			if(SUM_BIT_S1 == 1)					// Summenfehlerbit gesetzt ?
			{	modb_sio_errorflag[port] = SIO_ERROR;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;		// Rx-ISR in Leerlauf		
			}
			break;

		case S2:
			pRxBuf  = RxBuf_S2;
			modb_rx_buff_size = S2_RXSIZE;
			rbyte 	= RB_S2;			
			RIR_BIT_S2   = 0;						// Interrupt Request Bit löschen
	/*		if(SUM_BIT_S2 == 1)					// Summenfehlerbit gesetzt ?
			{	modb_sio_errorflag[port] = SIO_ERROR;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;		// Rx-ISR in Leerlauf		
			} */
			break;
	
		case S3:
			pRxBuf  = RxBuf_S3;
			modb_rx_buff_size = S3_RXSIZE;
			rbyte 	= RB_S3;			
			if(SUM_BIT_S3 == 1)					// Summenfehlerbit gesetzt ?
			{	modb_sio_errorflag[port] = SIO_ERROR;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;		// Rx-ISR in Leerlauf		
			}
	}

  switch(modb_rx_int_state[port])
	{
		case 0:				// do nothing
			break;

		case 1:
			if(modb_rx_count[port] < modb_rx_buff_size)			// Länge Testen
			{	
				pRxBuf[modb_rx_count[port]++] = rbyte;
			}
			else
			{	
				modb_sio_errorflag[port] = LENGT_ERROR;
				modb_rx_int_state[port] = 2;
			}
			set_EoF_timer(port);
			break;

		case 2:															// Mit Timer auf parasitäres EoF warten.
			set_EoF_timer(port);
			break;
	}
}							 

///////////////////////////////////////////////////////////////////
// Timer Interrupt für RTS-OFF / EoF-Dedect
///////////////////////////////////////////////////////////////////
void U_Modbus_ISR_Tel_Tout(char port)
{
	char *pRxBuf;

	#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S1)
	{
		pRxBuf  = RxBuf_S1;
		P_TOUTS_S1	= 0;		// Stop	 Prescaler
		C_TOUTS_S1	= 0;		// Stop	 Timer Counter
		if(timer_function[port])	// RTS-Delay-Funktion
		{
			RTS_S1			=	0;	// Sendekanal sperren
			Modb_Rx_Ready(port);
		}
		else								// EoF-Funktion
		{
			if((pRxBuf[0] == modb_curr_adr[port]) && (modb_rx_count[port] > 3))	// aus Zeitgründen nur gültige Frames dem Handler melden
			{	modb_rx_buffer_ready[port] = 1;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;	// Rx-ISR in Leerlauf		
			}
			else
			{ 			
				Modb_Rx_Ready(port);	// nach ungült. Adr oder Störung Rx neu aktivieren
				modb_rx_count[port]		=	0;		// Sende-	/	Empfangspuffer Index
				modb_sio_errorflag[port] = 0;
				modb_rx_buffer_ready[port] = 0;
				modb_rx_int_state[port] = 1;	// Rx-ISR aktiv
				timer_function[port] = 0;							// Für Timer-ISR: EoF für Rx-Funktion einschalten
			}
		}
	}
	#endif

	#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S2)
	{
		pRxBuf  = RxBuf_S2;
		P_TOUTS_S2	= 0;		// Stop	 Prescaler
		C_TOUTS_S2	= 0;		// Stop	 Timer Counter
		if(timer_function[port])	// RTS-Delay-Funktion
		{
			RTS_S2			=	0;	// Sendekanal sperren
			Modb_Rx_Ready(port);
		}
		else								// Rx-EoF-Funktion
		{
			if((pRxBuf[0] == modb_curr_adr[port]) && (modb_rx_count[port] > 3))	// aus Zeitgründen nur gültige Frames dem Handler melden
			{	modb_rx_buffer_ready[port] = 1;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;	// Rx-ISR in Leerlauf		
			}
			else
			{ 			
				Modb_Rx_Ready(port);	// nach ungült. Adr oder Störung Rx neu aktivieren
				modb_rx_count[port]		=	0;		// Sende-	/	Empfangspuffer Index
				modb_sio_errorflag[port] = 0;
				modb_rx_buffer_ready[port] = 0;
				modb_rx_int_state[port] = 1;	// Rx-ISR aktiv
				timer_function[port] = 0;							// Für Timer-ISR: EoF für Rx-Funktion einschalten
			}
		}
	}
	#endif

	#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S3)
	{
		pRxBuf  = RxBuf_S3;
		P_TOUTS_S3	= 0;		// Stop	 Prescaler
		C_TOUTS_S3	= 0;		// Stop	 Timer Counter
		if(timer_function[port])	// RTS-Delay-Funktion
		{
			RTS_S3			=	0;	// Sendekanal sperren
			Modb_Rx_Ready(port);
		}
		else								// EoF-Funktion
		{
			if((pRxBuf[0] == modb_curr_adr[port]) && (modb_rx_count[port] > 3))	// aus Zeitgründen nur gültige Frames dem Handler melden
			{	modb_rx_buffer_ready[port] = 1;
				Modb_Rx_DeInit(port);
				modb_rx_int_state[port] = 0;	// Rx-ISR in Leerlauf		
			}
			else
			{ 			
				Modb_Rx_Ready(port);	// nach ungült. Adr oder Störung Rx neu aktivieren
				modb_rx_count[port]		=	0;		// Sende-	/	Empfangspuffer Index
				modb_sio_errorflag[port] = 0;
				modb_rx_buffer_ready[port] = 0;
				modb_rx_int_state[port] = 1;	// Rx-ISR aktiv
				timer_function[port] = 0;							// Für Timer-ISR: EoF für Rx-Funktion einschalten
			}
		}
	}
	#endif
}
	
///////////////////////////////////////////////////////////////////
// Interrupt Baudratenmessung
///////////////////////////////////////////////////////////////////
void U_Modbus_ISR_BaudCheck(char port)
{
}

/************************* Ende Sio-Programme ********************/



/*********************** Beginn Unter-Programme ******************/

void modb_sio_init(char port)
{
	#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S1)
	{
		RTS_S1	=	0;			// Empfangskanal freigeben RTS=Low
		MR_S1 = 0x05;			// Transmit Receive Mode Register = UART Mode 8 Bit
		C0_S1 = 0x10;			// Transmit Receive Control Register 0
		TB_S1 = RB_S1;			// SIO entleeren	(Kopieren auf Sendepuffer)
		TB_S1 = 0;				// Sendepuffer löschen
	
		DISABLE_IRQ			
		RIC_S1 = 0x04;		// Enable Receive Interrupt Prio: 4			
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S1 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S1 = 1;		// Empfang erlauben 																						
		BRG_S1 = (unsigned char)( ( (f1_CLK_SPEED/16) / Baudrate_S1) -1);
		modb_rts_prescaler[port] = 64;		// Vorteiler: 64
		modb_rts_timer[port] = 32;		// war 4800;
		EoF_timer[port]		=	120;	
		P_TOUTS_S1	= 0;		// Stop	 RTS Prescaler
		C_TOUTS_S1	= 0;		// Stop	 RTS Timer Counter
	}
	#endif

	#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S2)
	{
	
		RTS_S2	=	0;			// Empfangskanal freigeben RTS=Low
		MR_S2 = 0x05;			// Transmit Receive Mode Register = UART Mode 8 Bit
		C0_S2 = 0x10;			// Transmit Receive Control Register 0
	
		RIRLT_BIT_S2 = 1;		// Interrupt Request Select Bit im Interrupt Enable  Register setzen  auf used for interrupt
		RIR_BIT_S2   = 0;		// Interrupt Request Bit 				im Interrupt Request Register löschen
		RIE_BIT_S2   = 1;		// Interrupt Enable  Bit 				im Interrupt Enable  Register setzen
	
		TB_S2 = RB_S2;		// SIO entleeren	(Kopieren auf Sendepuffer)
		TB_S2 = 0;				// Sendepuffer löschen
	
		DISABLE_IRQ			
		RIC_S2 = 0x00;		// Disable Receive Interrupt Prio: 4	(0x04)		wegen Rückkopplung
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S2 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S2 = 0;		// Empfang noch nicht	erlauben 
		BRG_S2 = (unsigned char)( ( (f1_CLK_SPEED/16) / Baudrate_S2) -1);

		modb_rts_prescaler[port] = 64;		// Vorteiler: 64
		modb_rts_timer[port] = 32;		// war 4800;
		EoF_timer[port]		=	120;	
		
		P_TOUTS_S2	= 0;		// Stop	 RTS Prescaler
		C_TOUTS_S2	= 0;		// Stop	 RTS Timer Counter

		TIRLT_BIT_S2 = 1;		// Interrupt Request Select Bit im Interrupt Enable  Register (IIO1IE) setzen  auf used for interrupt
		TIR_BIT_S2	 = 0;		// Interrupt Request Bit 				im Interrupt Request Register (IIO1IR) löschen
		TIE_BIT_S2	 = 1;		// Interrupt Enable  Bit 				im Interrupt Enable  Register (IIO1IE) setzen

	}
	#endif

	#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S3)
	{
		RTS_S3	=	0;			// Empfangskanal freigeben RTS=Low
		MR_S3 = 0x05;				// 0 1 1 0 0 101	
		C0_S3 = 0x10;			// Transmit Receive Control Register 0
		TB_S3 = RB_S3;			// SIO entleeren	(Kopieren auf Sendepuffer)
		TB_S3 = 0;				// Sendepuffer löschen
	
		DISABLE_IRQ			
		RIC_S3 = 0x04;		// Enable Receive Interrupt Prio: 4			
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S3 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S3 = 1;		// Empfang erlauben 																						
		BRG_S3 = (unsigned char)( ( (f1_CLK_SPEED/16) / Baudrate_S3) -1);
		modb_rts_prescaler[port] = 64;		// Vorteiler: 64
		modb_rts_timer[port] = 32;		// war 4800;
		EoF_timer[port]		=	120;	
		P_TOUTS_S3	= 0;		// Stop	 RTS Prescaler
		C_TOUTS_S3	= 0;		// Stop	 RTS Timer Counter
	}
	#endif
}			


/* ----- Transmitter Start-Routine für MODBUS über TX-S2 und TX-S3----*/
void start_modb_transmit(char port)
{
	unsigned int  fcs;

	#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S1)
	{
		RTS_S1		= 1;						// Transmitter Enable
	// Frame Check Sequenz berechnen
		fcs = modbus_crc ( (char *)(TxBuf_S1), modb_tx_count[port] );
		TxBuf_S1[modb_tx_count[port]++] = ((fcs >> 8) & 0x00ff);
		TxBuf_S1[modb_tx_count[port]++] = (fcs & 0x00ff);
		Modb_Rx_DeInit(port);	
		RxBuf_S1[0] = 0;
		delay(1);									// 100µs Zwangspause für eine stabile Startbit-Breite
		modb_leng_tx[port] = modb_tx_count[port];		// Telegrammlänge	für	Sendeinterrupt
		/*	SIO	-	Initialisieren und Sendeinterrupt	vorbereiten			*/
		modb_tx_count[port]	= 0;			 		// Sendepuffer	Index	
		TE_BIT_S1 = 1;						// Senden erlauben
		DISABLE_IRQ			
		TIC_S1 = 0x04;						// Enable Transmit Interrupt Prio: 4			
		ENABLE_IRQ	

		TB_S1	=	TxBuf_S1[modb_tx_count[port]++];	// Start Tx-Int
	}
	#endif

	#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S2)
	{
		RTS_S2		= 1;						// Transmitter Enable
	// Frame Check Sequenz berechnen
		fcs = modbus_crc ( (char *)(TxBuf_S2), modb_tx_count[port] );
		TxBuf_S2[modb_tx_count[port]++] = ((fcs >> 8) & 0x00ff);
		TxBuf_S2[modb_tx_count[port]++] = (fcs & 0x00ff);
		Modb_Rx_DeInit(port);
		RxBuf_S2[0] = 0;
		delay(1);									// 100µs Zwangspause für eine stabile Startbit-Breite
		modb_leng_tx[port] = modb_tx_count[port];		// Telegrammlänge	für	Sendeinterrupt
		/*	SIO	-	Initialisieren und Sendeinterrupt	vorbereiten			*/
		modb_tx_count[port]	= 0;			 		// Sendepuffer	Index	
		TE_BIT_S2 = 1;						// Senden erlauben
		TIRLT_BIT_S2 = 1;		// Interrupt Request Select Bit im Interrupt Enable  Register (IIO1IE) setzen  auf used for interrupt
		TIR_BIT_S2	 = 0;		// Interrupt Request Bit 				im Interrupt Request Register (IIO1IR) löschen
		TIE_BIT_S2	 = 1;		// Interrupt Enable  Bit 				im Interrupt Enable  Register (IIO1IE) setzen
		DISABLE_IRQ			
		TIC_S2 = 0x04;						// Enable Transmit Interrupt Prio: 4			
		ENABLE_IRQ	
		TB_S2	 = TxBuf_S2[modb_tx_count[port]++];	// Start Tx-Int
	}
	#endif

	#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S3)
	{
		RTS_S3		= 1;						// Transmitter Enable
	// Frame Check Sequenz berechnen
		fcs = modbus_crc ( (char *)(TxBuf_S3), modb_tx_count[port] );
		TxBuf_S3[modb_tx_count[port]++] = ((fcs >> 8) & 0x00ff);
		TxBuf_S3[modb_tx_count[port]++] = (fcs & 0x00ff);
		Modb_Rx_DeInit(port);	
		RxBuf_S3[0] = 0;
		delay(1);									// 100µs Zwangspause für eine stabile Startbit-Breite
		modb_leng_tx[port] = modb_tx_count[port];		// Telegrammlänge	für	Sendeinterrupt
		/*	SIO	-	Initialisieren und Sendeinterrupt	vorbereiten			*/
		modb_tx_count[port]	= 0;			 		// Sendepuffer	Index	
		TE_BIT_S3 = 1;						// Senden erlauben
		DISABLE_IRQ			
		TIC_S3 = 0x04;						// Enable Transmit Interrupt Prio: 4			
		ENABLE_IRQ	

		TB_S3	=	TxBuf_S3[modb_tx_count[port]++];	// Start Tx-Int
	}
	#endif
}

///////////////////////////////////////////////////////////////////
// Timer Setup 
///////////////////////////////////////////////////////////////////
void set_EoF_timer(char port)
{
	timer_function[port] = 0;						// Für Timer-ISR: EoF-Funktion
	switch(port)
	{
		case S1:
			P_TOUTS_S1	= 0;					// Stop	 Prescaler
			C_TOUTS_S1	= 0;					// Stop	 Timer Counter
			P_TOUTR_S1	=	modb_rts_prescaler[port];	// Load  Prescaler
			C_TOUTR_S1	=	EoF_timer[port];			// Load  Counter
			P_TOUTS_S1	= 1;					// Start	 Prescaler
			C_TOUTS_S1	= 1;					// Start	 Timer Counter
			break;

		case S2:
			P_TOUTS_S2	= 0;					// Stop	 Prescaler
			C_TOUTS_S2	= 0;					// Stop	 Timer Counter
			P_TOUTR_S2	=	modb_rts_prescaler[port];	// Load  Prescaler
			C_TOUTR_S2	=	EoF_timer[port];			// Load  Counter
			P_TOUTS_S2	= 1;					// Start	 Prescaler
			C_TOUTS_S2	= 1;					// Start	 Timer Counter
			break;

		case S3:
			P_TOUTS_S3	= 0;					// Stop	 Prescaler
			C_TOUTS_S3	= 0;					// Stop	 Timer Counter
			P_TOUTR_S3	=	modb_rts_prescaler[port];	// Load  Prescaler
			C_TOUTR_S3	=	EoF_timer[port];			// Load  Counter
			P_TOUTS_S3	= 1;					// Start	 Prescaler
			C_TOUTS_S3	= 1;					// Start	 Timer Counter
			break;
	}
}

void Modb_Rx_DeInit(char port)
{
	#if (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S1)
	{
		RE_BIT_S1 = 0;						// Empfang sperren, löschen der Fehlerbits
		DISABLE_IRQ			
		RIC_S1 = 0x00;						// Disable Receive Interrupt 
		ENABLE_IRQ	
	}
	#endif

	#if (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL

	if(port == S2)
	{
		RE_BIT_S2 	= 0;					// Empfang sperren, löschen der Fehlerbits
		RIE_BIT_S2  = 0;					// Interrupt Enable  Bit 	löschen
		
		DISABLE_IRQ			
		RIC_S2 = 0x00;						// Disable Receive Interrupt 
		ENABLE_IRQ	
	}

	#endif

	#if (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL
	if(port == S3)
	{
		RE_BIT_S3 = 0;						// Empfang sperren, löschen der Fehlerbits
		DISABLE_IRQ			
		RIC_S3 = 0x00;						// Disable Receive Interrupt 
		ENABLE_IRQ	
	}
	#endif	
}


void Modb_Rx_Ready(char port)
{
	modb_rx_count[port]		=	0;		// Sende-	/	Empfangspuffer Index
	modb_sio_errorflag[port] = 0;
	modb_rx_buffer_ready[port] = 0;
	modb_rx_int_state[port] = 1;	// Rx-ISR aktiv
	timer_function[port] = 0;							// Für Timer-ISR: EoF für Rx-Funktion einschalten

	if(port == S1)
	{
		DISABLE_IRQ			
		RIC_S1 = 0x04;		// Enable Receive Interrupt Prio: 4			
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S1 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S1 = 1;		// Empfang erlauben 					
	}

	if(port == S2)
	{
		RIRLT_BIT_S2 = 1;		// Interrupt Request Select Bit im Interrupt Enable  Register setzen  auf used for interrupt
		RIR_BIT_S2   = 0;		// Interrupt Request Bit 				im Interrupt Request Register löschen
		RIE_BIT_S2   = 1;		// Interrupt Enable  Bit 				im Interrupt Enable  Register setzen
		DISABLE_IRQ			
		RIC_S2 = 0x04;		// Enable Receive Interrupt Prio: 4			
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S2 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S2 = 1;		// Empfang erlauben 					
	}

	if(port == S3)
	{
		DISABLE_IRQ			
		RIC_S3 = 0x04;		// Enable Receive Interrupt Prio: 4			
		ENABLE_IRQ	
	// Fehlerstatus im Empfangspuffer löschen durch Rücksetzen des Receive Enable Bit
		C1_S3 = 0x00;			// Transmit Receive Control Register 1		
		RE_BIT_S3 = 1;		// Empfang erlauben 					
	}
}	

// Modbus-Kommando-Generierung
void 	modbus_request(char *pTxBuf, char port)
{
	char i, cnt;
	unsigned char slaveMode = ((Funktion_S1 == MODBUS1_FU && Mode_S1 == SLAVE) && port == S1) || ((Funktion_S2 == MODBUS1_FU && Mode_S2 == SLAVE) && port == S2) || ((Funktion_S3 == MODBUS1_FU && Mode_S3 == SLAVE) && port == S3);
	modb_tx_count[port] = 0;
	pTxBuf[modb_tx_count[port]++] = modb_curr_adr[port];
	pTxBuf[modb_tx_count[port]++] = func_code[port];	
	
	if((func_code[port] & 0x80) == 0x80) {
		// Im Fehlerfall wird nur der Fehlercode übertragen
		pTxBuf[modb_tx_count[port]++] = byte_count_or_exception[port];
	} else {
	
	if(!slaveMode) {
		pTxBuf[modb_tx_count[port]++] = ((first_reg[port] >> 8) & 0x00ff);
		pTxBuf[modb_tx_count[port]++] = (first_reg[port] & 0x00ff);
	}

	switch(func_code[port])
	{
		case READ_MULTIPLE_COILS:
					pTxBuf[modb_tx_count[port]++] = ((cnt_regs[port] >> 8) & 0x00ff);
					pTxBuf[modb_tx_count[port]++] = (cnt_regs[port] & 0x00ff);
			break;

		case READ_HOLDING_REGS:
		case READ_INPUT_REGS:
			if(slaveMode) {
				pTxBuf[modb_tx_count[port]++] = byte_count_or_exception[port];
				cnt = cnt_regs[port]*2;
				for(i=0; i<cnt; i++) {
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[i];
				}
				break;				
			}
			else {
				pTxBuf[modb_tx_count[port]++] = ((cnt_regs[port] >> 8) & 0x00ff);
				pTxBuf[modb_tx_count[port]++] = (cnt_regs[port] & 0x00ff);
			}
			break;

		case WRITE_SINGLE_HOLDING:
			if(slaveMode) {
				pTxBuf[modb_tx_count[port]++] = ((first_reg[port] >> 8) & 0x00ff);
				pTxBuf[modb_tx_count[port]++] = (first_reg[port] & 0x00ff);
			}
				for(i=0; i<2; i++) {
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[i];
				}
//			save_tx_count = modb_tx_count[port];					// für Response-Check
			break;
			
		case WRITE_MULTIPLE_HOLDINGS:
			if(slaveMode) {
					pTxBuf[modb_tx_count[port]++] = ((first_reg[port] >> 8) & 0x00ff);
					pTxBuf[modb_tx_count[port]++] = (first_reg[port] & 0x00ff);
			}
			
			//ToDo: überprüfung testen ob mehrere gesendet werden können
			
			pTxBuf[modb_tx_count[port]++] = ((cnt_regs[port] >> 8) & 0x00ff);
			pTxBuf[modb_tx_count[port]++] = (cnt_regs[port] & 0x00ff);
			
			if(!slaveMode) {
				cnt = pTxBuf[modb_tx_count[port]++] = (char)(cnt_regs[port] * 2);			
				for(i=0; i<cnt; i++) {
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[i];
				}
			}
			break;
		
		case WRITE_MULTIPLE_COILS:		//anbu Änderung erweiterung
			if(slaveMode) {
					pTxBuf[modb_tx_count[port]++] = ((first_reg[port] >> 8) & 0x00ff);
					pTxBuf[modb_tx_count[port]++] = (first_reg[port] & 0x00ff);
			}
			
			//ToDo: überprüfung testen ob mehrere gesendet werden können
			
			pTxBuf[modb_tx_count[port]++] = ((cnt_regs[port] >> 8) & 0x00ff);
			pTxBuf[modb_tx_count[port]++] = (cnt_regs[port] & 0x00ff);
			
			if(!slaveMode) {
				cnt = pTxBuf[modb_tx_count[port]++] = (char)(cnt_regs[port] * 1);			
				for(i=0; i<cnt; i++) {
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[i];
				}
			}
			break;
			
		case WRITE_SINGLE_COIL:		//anbu Änderung erweiterung
			if(slaveMode) {
					pTxBuf[modb_tx_count[port]++] = ((first_reg[port] >> 8) & 0x00ff);
					pTxBuf[modb_tx_count[port]++] = (first_reg[port] & 0x00ff);
			}
			
//			//ToDo: überprüfung testen ob mehrere gesendet werden können
//			
//			pTxBuf[modb_tx_count[port]++] = ((cnt_regs[port] >> 8) & 0x00ff);
//			pTxBuf[modb_tx_count[port]++] = (cnt_regs[port] & 0x00ff);
//			
			if(!slaveMode) {
//				cnt = pTxBuf[modb_tx_count[port]++] = (char)(cnt_regs[port] * 1);			
//				for(i=0; i<cnt; i++) {
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[0];
					pTxBuf[modb_tx_count[port]++] = ((unsigned char*)reg_address[port])[1];
//				}
			}
			break;
	}
	}
	start_modb_transmit(port);
}

char check_modb_rx_buffer(char *pRxBuf, char port)
{	
	char ret = 0;
	unsigned char slaveMode = ((Funktion_S1 == MODBUS1_FU && Mode_S1 == SLAVE) && port == S1) || ((Funktion_S2 == MODBUS1_FU && Mode_S2 == SLAVE) && port == S2) || ((Funktion_S3 == MODBUS1_FU && Mode_S3 == SLAVE) && port == S3);
	if( modb_rx_buffer_ready[port] ) {
		if( modb_sio_errorflag[port] == 0 ) {
			if ( modbus_crc ( pRxBuf, modb_rx_count[port] ) == 0 ) {
				if((pRxBuf[0] == modb_curr_adr[port]) || slaveMode != FALSE)	// Vergleich, ist Slave-Adr identisch mit angeforderter, SoF ok
				{
					if (pRxBuf[1] == func_code[port] || slaveMode != FALSE) {
						ret = RX_BUF_READY;
					}
					else
						ret = EXEPTION;
				}
				else
					ret = SoF_ERROR; 	// Adresszuordnung nicht ok
			}
			else
				ret = CRC_ERROR;
		}
		else
			ret = modb_sio_errorflag[port];
	}
	return ret;
}


///////////////////////////////
// Modbus-CRC16 Implementation 
///////////////////////////////

char crc_table[] =
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5,
0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9,
0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F,
0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13,
0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6,
0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA,
0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6,
0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3,
0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF,
0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79,
0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75,
0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90,
0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C,
0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C,
0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81,
0x80, 0x40 } ;

int modbus_crc(char *ptr, char Length)
{
int i, index;
int crc_Low = 0xFF;
int crc_High = 0xFF;

for (i = 0; i<Length; i++)
{
index = crc_High ^ ptr[i];
crc_High = crc_Low ^ crc_table[index];
crc_Low = (char)crc_table[index + 256];
}

return crc_High*256+crc_Low;
}

#endif