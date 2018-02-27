//******************************************************************************
//******************************************************************************
//
// File Name
// ---------
//
// anybus.c
//
//******************************************************************************
//******************************************************************************
//
// Description
// -----------
//
// Source file containing a minimalistic Anybus-CC application example.
//
//******************************************************************************
//******************************************************************************
//                                                                            **
// User Information 2007 HMS Industrial Networks AB.                          **
//                                                                            **
// This program is the property of HMS Industrial Networks AB.                **
// It has been designed to be used and modified without permission from       **
// HMS Industrial Networks AB in any application using an Anybus CompactCom   **
// module.                                                                    **
// We, HMS Industrial Networks AB, reserve the right to modify our products   **
// in line with our policy of continuous product development. The information **
// in this program is subject to updates without notice and should not be     **
// considered as a commitment by HMS Industrial Networks AB.                  **
// HMS Industrial Networks AB assumes no responsibility for any errors that   **
// may appear in this program.                                                **
//                                                                            **
//******************************************************************************
//******************************************************************************
//
// Company: HMS Industrial Networks AB
//          Stationsgatan 37
//          SE-302 45  Halmstad
//          SWEDEN
//          Tel:     +46 (0)35 - 17 29 00
//          Fax:     +46 (0)35 - 17 29 09
//          e-mail:  info@hms.se
//
//******************************************************************************
//******************************************************************************


//******************************************************************************
//******************************************************************************
//
// Change Log
// ----------
//
// Latest modification:
// --------------------
// $Modtime: 2010-06-28 14:24:58+02:00 $
//
// Version history:
//
//    Ver 1.00     2007-12-14  TiB   First release
//
//******************************************************************************
//******************************************************************************


#include "projdef.h"
#include "konvform.h"
#include "struct.h"
#include "anybus.h"
#include "abp.h"
#include <string.h>
#include "U_anybus.h"

#if ( ANYBUS_MODUL == 1 )

#define	KONVRD			 0			// Konvertierung	"lesen"
#define	KONVWR			 1			// Konvertierung	"schreiben"

#define min( x , y )  ( ( x ) < ( y ) ? ( x ) : ( y ) )

//******************************************************************************
//
// Public Globals
//
//******************************************************************************
// siehe Userram unter Anybus - Anybus-Modul-Typ für die Anzeige
extern char carAnybustyp[];
extern char carAnybusIPAdresse[];
extern char carAnybusSubnetmask[];
extern char carAnybusGateway[];
extern char RiedelKonvert(char* Dat_Ptr, unsigned int dae_num, char rw, unsigned char ucExp_num);
extern void ctoa(char *buf, char ch);
extern void ctoa3(char	*buf,	unsigned char	ch);
extern const char Kategorie_Typ[];
extern const char Kategorie_Vers[];
extern char carAnybusstatus[];

extern char Sek;
extern char Min;
extern char Std;
extern char Tag;
extern char Mon;
extern char Jahr;
extern unsigned long LongZeit;
extern unsigned long ulWriteStamp;
extern unsigned int serv_cnt;
extern const Parli Pgruppe[];
extern const kotab KomTab[];
//
// bSw1 and bSw2 represent two DIP-switches (or similar) which are used to set
// the fieldbus node adress and baudrate of the Anybus-CC.
#ifdef USE_DIPSWITCHES
   UINT8                      bSw1 = 60;
   UINT8                      bSw2 = 3;
#endif

//
// Working copies of the status and control registers
UINT8                         bControlReg = ABP_CTRL_R_BIT | ABP_CTRL_T_BIT;
UINT8                         bStatusReg  = 0;

//
// State variable that keeps track of which initialization command to send next

#ifdef USE_DIPSWITCHES
   CmdStateType               eCmdState   = MSG_NODE_ADR;
#else
   CmdStateType               eCmdState   = MSG_DATA_FORMAT;//MSG_MAP_IO_1;
#endif

//
// This variable keeps track of if there are any commands which we have not yet
// received a response to.


UINT8 bOutstandingCmds = 0;

// Zähler zum Überwachen, ob Anybus kommunikation noch steht
SINT16 iFreigabe = 0;

//
// The application data instances (ADIs for short)
// This is the data which will be exchanged on the fieldbus
// In this simple example we only have eight UINT16 parameters


//DRUINT16                        aiApplicationData[ NUMBER_OF_ADIS ] = { 0, 1, 1, 3, 4, 5, 6, 7 };

//
// The names of each application data instance (ADI for short)
// These strings should be changed to reflect the proper names of the ADIs.




extern const anybusTableRow	anybusTable[];
extern anybusDataTableRow anybusDataTable[];
extern const UINT16 uiNumber_of_Data;
extern UINT16 uiMaxInstanceNumber;
extern UINT16 uiInstanceCount;
// Lookup table for retrieving the size of ABCC data types

const UINT8 appd_abTypesize[] =
{  ABP_BOOL_SIZEOF,
   ABP_SINT8_SIZEOF,
   ABP_SINT16_SIZEOF,
   ABP_SINT32_SIZEOF,
   ABP_UINT8_SIZEOF,
   ABP_UINT16_SIZEOF,
   ABP_UINT32_SIZEOF,
   ABP_CHAR_SIZEOF,
   ABP_ENUM_SIZEOF,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   ABP_SINT64_SIZEOF,
   ABP_UINT64_SIZEOF,
   ABP_FLOAT_SIZEOF
};


//
// This variable is used to indicate the network data format.
// Little endian or big endian.


NetFormatType                 eNetFormat = NET_UNKNOWN;
NetFormatType		eHostFormat = NET_LITTLEENDIAN;
UINT16		uiNetworkType = 0;

//******************************************************************************
//
// Private Globals
//
//******************************************************************************


//
// Serial transmission buffer.
// The standard telegram is 19 bytes, and the process data is be added.
// PORTING ALERT!
// The size of this buffer must be adapted to the chosen process data size.


UINT8                abTxBuffer[ 512 ];

//
// Message buffer.

ABP_MsgType          sMsgBuffer;

UINT16               iMsgBufferOffset = 0;
UINT16               iTxMsgBufferOffset = 0;

UINT16				uiWriteArraySize = 0;
UINT16				uiReadArraySize = 0;

// Index innerhalb der Datenpunkte zum Initialisieren der MSG-Map
UINT16				uiMSG_MAP_IO_INDEX = 0;
// Uhrzeit unsw. werden einzeln angelegt und hier als Subanzahl definiert
UINT16				uiMSG_MAP_SUB_INDEX = 0;
//
// Tables for generating CRC16 checksum


const UINT8 abCrc16Hi[] =
{
   0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
   0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
   0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
   0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
   0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
   0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
   0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
   0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
   0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
   0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
   0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
   0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
   0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
   0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
   0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
   0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
   0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
   0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
   0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};


const UINT8 abCrc16Lo[] =
{
   0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05,
   0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA,
   0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA,
   0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15,
   0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0,
   0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35,
   0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
   0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
   0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
   0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
   0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64,
   0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
   0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE,
   0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
   0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
   0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
   0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99,
   0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E,
   0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
   0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


//******************************************************************************
//
// Public Functions
//
//******************************************************************************

//------------------------------------------------------------------------------
// Crc16()
//------------------------------------------------------------------------------


UINT16 Crc16( UINT8* pbBufferStart, UINT16 iLength )
{
   UINT8   bIndex;
   UINT8   bCrcLo;
   UINT8   bCrcHi;
	 


   //
   // Init crc to 0xFFFF
   

   bCrcLo = 0xFF;
   bCrcHi = 0xFF;

   //
   // Do the crc calculation
   

   while( iLength > 0 )
   {
      bIndex = bCrcLo ^ *pbBufferStart++;
      bCrcLo = bCrcHi ^ abCrc16Hi[ bIndex ];
      bCrcHi = abCrc16Lo[ bIndex ];
      iLength--;
   }

   return( ((UINT16)bCrcHi) << 8 | bCrcLo );

} // End of Crc16() 


//------------------------------------------------------------------------------
// AnybusMain()
//------------------------------------------------------------------------------


UINT8* AnybusMain( UINT8* pbRxData, UINT16* piTxSize, UINT16* piRxSize )
{
   UINT16 iCrc;
	 unsigned char ucIndex;
	anybusDataTableRow *pDataRow;
#ifdef USE_DIPSWITCHES
   static UINT8 bLastSw1, bLastSw2;
#endif

   if( pbRxData == NULL ) {
      //
      // This is either the initial call to this function, or the
      // retransmission timer has timed out.
      // Perform a (re)transmission.
      

      if( ( bStatusReg & ABP_STAT_S_BITS ) > ABP_ANB_STATE_SETUP )
      {
         //
         // Update the "write process data" in the telegram before (re)transmission.
         // The data format of the process data is network specific.
         // Convert our data accordingly.
         

				AppProcessData2AnyBus(&abTxBuffer[ 17 ]);
			} else {
				abTxBuffer[0] = bControlReg;
			}

      iCrc = Crc16( (UINT8*)abTxBuffer, (UINT16)(*piTxSize - 2) );

      abTxBuffer[ *piTxSize - 2 ] = (UINT8)( ( iCrc >> 8 ) & 0x00FF );
      abTxBuffer[ *piTxSize - 1 ] = (UINT8)( iCrc & 0x00FF );

      return( abTxBuffer );

   }

	 memset(abTxBuffer,0,*piTxSize);

   iCrc  = (UINT16)pbRxData[ *piRxSize - 2 ] << 8;
   iCrc |= pbRxData[ *piRxSize - 1 ];

   if( iCrc != Crc16( pbRxData, (UINT16)( *piRxSize - 2 ) ) )
   {
      //
      // CRC error.
      // This data must be ignored.
      memcpy( (void *)carAnybusstatus, " CRC-Error     ", 15 );
      

      return( NULL );
   }

   //
   // Check if the toggle bit in the status register has toggled.
   

   if( ( pbRxData[ 0 ] & ABP_STAT_T_BIT ) !=
       ( bControlReg & ABP_CTRL_T_BIT ) )
   {
      //
      // Toggle bit has not toggled.
      // This data must be ignored.
      memcpy( (void *)carAnybusstatus, " ToggleBitError", 15 );

      return( NULL );
   }

   bStatusReg = pbRxData[ 0 ];

   bControlReg ^= ABP_CTRL_T_BIT;

//DR
	 if(Abcc_Status(bStatusReg)<0) {
		 // Status hat wieder auf Setup gewechselt, alles auf anfang
		 return(NULL);
	 }

   if( bStatusReg & ABP_STAT_M_BIT )
   {
      //
      // There is a message fragment available.
      

      //
      // Clear the Ready bit so that we don't have to handle more than one command at a time.
      

      bControlReg &= ~ABP_CTRL_R_BIT;

      if( iMsgBufferOffset >= sizeof(ABP_MsgType) )
      {
         //
         // Defragmentation error!
         // This should never occur. There is probably a software bug somewhere.
         // PORTING ALERT!
         // Add error handling here.
         
      }
      else
      {
         memcpy( &(((UINT8*)&sMsgBuffer)[ iMsgBufferOffset ] ), &pbRxData[ 1 ], min( 16, sizeof(ABP_MsgType) - iMsgBufferOffset ) );

         iMsgBufferOffset += 16;
      }
   }
   else if( iMsgBufferOffset != 0 ) {
    //
    // A new defragmented message is available.
    

		iMsgBufferOffset = 0;

		if( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_C_BIT ) {
			//
			// The message is a command
			// Prepare the response.

			sMsgBuffer.sHeader.bCmd &= ~ABP_MSG_HEADER_C_BIT;

			switch( sMsgBuffer.sHeader.bDestObj ) {
				case ABP_OBJ_NUM_BAC:
					//*******************************************************************************
					// Command to the Bacnet Host object
					// Alle Daten (Process- und Nichtprocessdaten) werden hier zu Anybus gesendet
					//*******************************************************************************
					BacnetHostObject();
					bControlReg |= ABP_CTRL_M_BIT;
					break;
				case ABP_OBJ_NUM_APPD:

					//*******************************************************************************
					// Command to the application data object
					// Alle Daten (Process- und Nichtprocessdaten) werden hier zu Anybus gesendet
					//*******************************************************************************
					ApplicationDataObject();
					bControlReg |= ABP_CTRL_M_BIT;

					break;
				case ABP_OBJ_NUM_APP:
					//*******************************************************************************
					// Command to the application object
					// Alle Daten (Process- und Nichtprocessdaten) werden hier zu Anybus gesendet
					//*******************************************************************************
					ApplicationObject();
					bControlReg |= ABP_CTRL_M_BIT;

					break;
				default:
					// Command to a unsupported object.
					SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_OBJ );
					bControlReg |= ABP_CTRL_M_BIT;
					break;
			}
		} else {
			//
			// The message is a reponse to a command we have sent.


			bOutstandingCmds--;

			if( bOutstandingCmds == 0 ) {
				//
				// We are not waiting for any repsonses any more.
				// Set the Ready to allow the ABCC to send a command to us.

				bControlReg |= ABP_CTRL_R_BIT;
			}

			switch( sMsgBuffer.sHeader.bSourceId ) {
#ifdef USE_DIPSWITCHES
				case MSG_NODE_ADR:
				case MSG_BAUDRATE:

					//
					// It is a response to a command to set one of the DIP-switch values.
					// This response shall normally be ignored, even if the error bit is set.
					// (Not all Anybus-CC modules support the use of DIP-switches)


					break;
#endif

				case MSG_SETUP_COMPLETE:

					// It is a response to the setup complete command.
					// Check if everything went well.
            
					if( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_E_BIT ){
						// The Anybus-CC has responded with the error bit set. This means
						// something is wrong.
						// PORTING ALERT!
						// Application specific error handling can optionally be inserted
						// here. For example: Indicate to user that something is wrong
						// with the Anybus-CC.
//						printf("Fehler bei der Initialisierung");
						iMsgBufferOffset = 0;
					} else {
						// The process data will be transmitted in every serial telegram from now on.
						// Increase the Rx and Tx sizes accordingly.
						// The standard telegram is 19 bytes, and the process data sizes are 4 bytes
						// in this example.
						// PORTING ALERT!
						// This must be adapted to match the chosen process data sizes.
               
//DR
						*piRxSize = 19 + uiReadArraySize; //+4;
						*piTxSize = 19 + uiWriteArraySize;//4;
					}
					break;

				case MSG_DATA_FORMAT:
					// It is a response to the 'get data format' command

					if( sMsgBuffer.abData[ 0 ] > 1 ) {
						// The Anybus-CC responded with a unknown data format type.
						// This is a unexpected error. Application specific error
						// handling can optionally be inserted here.
						// PORTING ALERT!
						// For example: Indicate to user that something is wrong.

						eNetFormat = NET_UNKNOWN;
					} else {
						// The Anybus-CC responded with a correct data format type.
						// Store it for later use.


						eNetFormat = sMsgBuffer.abData[ 0 ];
					}
					break;
				case MSG_NW_TYPE:
					// Antwort auf den Netzwerktyp als Integer
					if(sMsgBuffer.sHeader.bDataSize < 2 ) {
						// The Anybus-CC responded with a unknown data format type.
						// This is a unexpected error. Application specific error
						// handling can optionally be inserted here.
						// PORTING ALERT!
						// For example: Indicate to user that something is wrong.

						uiNetworkType = 0;
					} else {
						// The Anybus-CC responded with a correct data format type.
						// Store it for later use.
						HostEndian2NetEndain(&uiNetworkType,&sMsgBuffer.abData[ 0 ],ABP_UINT16_SIZEOF);

					}
					break;
				case MSG_NW_TYPE_STR:
					// Antwort auf den Netzwerktyp als Zeichenkette unbekannter Länge
					for(ucIndex=0; ucIndex<16; ucIndex++) {
						if(ucIndex<sMsgBuffer.sHeader.bDataSize) {
							carAnybustyp[ucIndex] = sMsgBuffer.abData[ucIndex];
						} else {
							carAnybustyp[ucIndex] = ' ';
						}
					}
					break;
				case MSG_IPADRESS:
					if(sMsgBuffer.sHeader.bDataSize>=4) {
						// Ich erwarte 4 Byte
						for(ucIndex=0; ucIndex<4; ucIndex++) {
							ctoa3(&carAnybusIPAdresse[ucIndex*4],sMsgBuffer.abData[ucIndex]);
							carAnybusIPAdresse[ucIndex*4+3] = '.';
						}
						if(iFreigabe>0) {
							iFreigabe--;
						}
					} else {
						memcpy(&carAnybusIPAdresse[0], "000.000.000.000",15);
					}
					break;
				case MSG_SUBNETMASK:
					if(sMsgBuffer.sHeader.bDataSize>=4) {
						// Ich erwarte 4 Byte
						for(ucIndex=0; ucIndex<4; ucIndex++) {
							ctoa3(&carAnybusSubnetmask[ucIndex*4],sMsgBuffer.abData[ucIndex]);
							carAnybusSubnetmask[ucIndex*4+3] = '.';
						}
						
					} else {
						memcpy(&carAnybusSubnetmask[0], "000.000.000.000",15);
					}
					break;
				case MSG_GATEWAY:
					if(sMsgBuffer.sHeader.bDataSize>=4) {
						// Ich erwarte 4 Byte
						for(ucIndex=0; ucIndex<4; ucIndex++) {
							ctoa3(&carAnybusGateway[ucIndex*4],sMsgBuffer.abData[ucIndex]);
							carAnybusGateway[ucIndex*4+3] = '.';
						}
					} else {
						memcpy(&carAnybusGateway[0], "000.000.000.000",15);
					}
					break;
//				default:
				case MSG_MAP_IO:
					// It is a response to a mapping command.
					// Check if everything went well.

						if( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_E_BIT ) {
							// The Anybus-CC has responded with the error bit set. This means
							// something is wrong.
							// PORTING ALERT!
							// Application specific error handling can optionally be inserted
							// here. For example: Indicate to user that something is wrong
							// with the Anybus-CC.
							// sMsgBuffer contains information about exactly which command
							// caused the error and what the error is.
							// Beim Mapping von Write oder Read ist ein Fehler aufgetreten.
							// Größe der Prozessdaten wird wieder auf den vorherigen Wert geändert
							if(anybusDataTable[uiMSG_MAP_IO_INDEX].ucZugriff==ABP_NC_DESCR_GET_ACCESS) {
								// WRITE_MAP_ONLY
							} else  if(anybusDataTable[uiMSG_MAP_IO_INDEX].ucZugriff==ABP_NC_DESCR_SET_ACCESS) {
								// READ_MAP_ONLY
							}
						} else {
							// Last ADI-Objekt ist ok, merken des Index in den Prozessdaten
							if(sMsgBuffer.sHeader.bDataSize>=1) {
								//merken des Index in den Prozessdaten
								if(uiMSG_MAP_SUB_INDEX==0) {
									// die Processdaten des ersten Elementes dieses Objektes  merken.
									anybusDataTable[uiMSG_MAP_IO_INDEX].siProcessIndex = sMsgBuffer.abData[ 0 ];
								}
							}
							// Erhöhen der Anzahl der Daten im Buffer 
							if(anybusDataTable[uiMSG_MAP_IO_INDEX].ucZugriff==ABP_NC_DESCR_GET_ACCESS) {
								// WRITE_MAP_ONLY
								uiWriteArraySize += appd_abTypesize[GetAnyBusDataTyp(&anybusDataTable[uiMSG_MAP_IO_INDEX])] * GetAnyBusDataTypNumbers(&anybusDataTable[uiMSG_MAP_IO_INDEX]);
							} else  if(anybusDataTable[uiMSG_MAP_IO_INDEX].ucZugriff==ABP_NC_DESCR_SET_ACCESS) {
								// READ_MAP_ONLY
								uiReadArraySize += appd_abTypesize[GetAnyBusDataTyp(&anybusDataTable[uiMSG_MAP_IO_INDEX])] * GetAnyBusDataTypNumbers(&anybusDataTable[uiMSG_MAP_IO_INDEX]);
							}
						}
						if((++uiMSG_MAP_SUB_INDEX) >= anybusDataTable[uiMSG_MAP_IO_INDEX].uiInstanceAnzahl) {
							// Alle Instancen des Datenpunktes erledigt, weiter mit dem nächsten
							++uiMSG_MAP_IO_INDEX;
							uiMSG_MAP_SUB_INDEX = 0;
						}
					//} else {
					//	//
					//	// The Anybus-CC has responded with a source ID which is unknown
					//	// to us. This means something is wrong.
					//	// PORTING ALERT!
					//	// Application specific error handling can optionally be inserted
					//	// here.
					//	// For example: Indicate to user that something is wrong.
					//}
					break;
				default:
						//
						// The Anybus-CC has responded with a source ID which is unknown
						// to us. This means something is wrong.
						// PORTING ALERT!
						// Application specific error handling can optionally be inserted
						// here.
						// For example: Indicate to user that something is wrong.
					break;
			} // end switch( Source ID ) 

		} // end if( message is a command ) 

	} // end if( message available ) 

	if( ( bStatusReg & ABP_STAT_R_BIT ) &&
      !( bControlReg & ABP_CTRL_M_BIT ) &&
      ( bOutstandingCmds == 0 ) ) {
      //
      // The Anybus-CC is ready to accept commands, and the WriteMsg buffer is free
      

      switch( eCmdState ) {
#ifdef USE_DIPSWITCHES
				case MSG_NODE_ADR:

					sMsgBuffer.sHeader.bSourceId          = MSG_NODE_ADR;
					sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
					sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_SW1 );
					sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;
					sMsgBuffer.sHeader.bDataSize          = 1;
					sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
					sMsgBuffer.sHeader.bCmdExt1           = 0;
					sMsgBuffer.abData[ 0 ]                = bLastSw1 = bSw1;
					bControlReg                           |= ABP_CTRL_M_BIT;
					eCmdState                              = MSG_BAUDRATE;

					break;
				case MSG_BAUDRATE:

					sMsgBuffer.sHeader.bSourceId          = MSG_BAUDRATE;
					sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
					sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_SW2 );
					sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;
					sMsgBuffer.sHeader.bDataSize          = 1;
					sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
					sMsgBuffer.sHeader.bCmdExt1           = 0;
					sMsgBuffer.abData[ 0 ]                = bLastSw2 = bSw2;
					bControlReg                           |= ABP_CTRL_M_BIT;
					eCmdState                              = MSG_DATA_FORMAT;//MSG_MAP_IO_1;

					break;
#endif
			case MSG_MAP_IO:
				if(uiMSG_MAP_IO_INDEX<=uiMaxInstanceNumber) {
					pDataRow = &anybusDataTable[uiMSG_MAP_IO_INDEX];
					if((GetAnyBusDataTyp(pDataRow)!=ABP_CHAR)							// 1. ABP_CHAR - Zeichenketten können nicht als Processdata gesendet werden
						&&(((pDataRow->ucZugriff==ABP_NC_DESCR_GET_ACCESS)		// UND (2. Es ist ein Lesewert für Anybus && die Processdaten sind noch nicht Größer als 255
							&&((uiWriteArraySize+GetAnyBusDataTypNumbers(pDataRow))<=255))
							||((pDataRow->ucZugriff==ABP_NC_DESCR_SET_ACCESS) 		// ODER 3. Es ist ein Schreibwert für Anybus && die Processdaten sind noch nicht Größer als 255)
							&&((uiReadArraySize+GetAnyBusDataTypNumbers(pDataRow))<=255)))) {
						
						sMsgBuffer.sHeader.bSourceId          = MSG_MAP_IO;// + uiMSG_MAP_IO_INDEX;
						sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NW;
						sMsgBuffer.sHeader.iInstance          = iTOiLe( 1 );
						if(pDataRow->ucZugriff==ABP_NC_DESCR_GET_ACCESS) {
							// WRITE_MAP_ONLY
							sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_WRITE_AREA;
						} else if(pDataRow->ucZugriff==ABP_NC_DESCR_SET_ACCESS) {
							// READ_MAP_ONLY
							sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_NW_CMD_MAP_ADI_READ_AREA;
						}
						sMsgBuffer.sHeader.bDataSize          = 4;

						*(UINT16*)&sMsgBuffer.sHeader.bCmdExt0 = (iTOiLe( pDataRow->uiInstanceNumber + uiMSG_MAP_SUB_INDEX )&0x00FF);// Instancanumber Low of the ADI-Object (	{"1st ADI", ABP_UINT16, 1, ABP_NC_DESCR_GET_ACCESS, &aiApplicationData[0], &uiMaxValue, &uiMinValue, &aiApplicationData[0],0},)
						*(UINT16*)&sMsgBuffer.sHeader.bCmdExt1 = ((iTOiLe( pDataRow->uiInstanceNumber + uiMSG_MAP_SUB_INDEX)&0xFF00)>>8);// Instancanumber High of the ADI-Object (	{"1st ADI", ABP_UINT16, 1, ABP_NC_DESCR_GET_ACCESS, &aiApplicationData[0], &uiMaxValue, &uiMinValue, &aiApplicationData[0],0},)
						sMsgBuffer.abData[ 0 ]                = GetAnyBusDataTyp(pDataRow); //DATA_TYP
						sMsgBuffer.abData[ 1 ]                = GetAnyBusDataTypNumbers(pDataRow);					 //Number of Elements in the ADI
						*(UINT16*)&sMsgBuffer.abData[ 2 ]     = iTOiLe( pDataRow->uiOrderNumber + uiMSG_MAP_SUB_INDEX );//Ordernumber of the ADI-Objekt -> entspricht ABP_APPD_CMD_GET_INST_BY_ORDER

						bControlReg                           |= ABP_CTRL_M_BIT;
					} else {
						++uiMSG_MAP_IO_INDEX;
						uiMSG_MAP_SUB_INDEX = 0;
					}
/*					if((++uiMSG_MAP_SUB_INDEX) >= anybusDataTable[uiMSG_MAP_IO_INDEX].uiInstanceAnzahl) {
						// Alle INstence des Datenpunktes erledigt
						++uiMSG_MAP_IO_INDEX;
						uiMSG_MAP_SUB_INDEX = 0;
					} else {
						++uiMSG_MAP_SUB_INDEX;
					}
*/
					eCmdState                              = MSG_MAP_IO;
				} else {
	        eCmdState                              = MSG_IPADRESS;//MSG_SETUP_COMPLETE;
				}
			 break;


      case MSG_DATA_FORMAT:

         sMsgBuffer.sHeader.bSourceId          = MSG_DATA_FORMAT;
         sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NW;
         sMsgBuffer.sHeader.iInstance          = iTOiLe( 1 );
         sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
         sMsgBuffer.sHeader.bDataSize          = 0;
         sMsgBuffer.sHeader.bCmdExt0           = ABP_NW_IA_DATA_FORMAT;
         sMsgBuffer.sHeader.bCmdExt1           = 0;
         bControlReg                           |= ABP_CTRL_M_BIT;
         eCmdState                              = MSG_NW_TYPE_STR;//MSG_NW_TYPE; //MSG_MAP_IO;//_1;////MSG_SETUP_COMPLETE;
         break;
			case MSG_NW_TYPE:
         sMsgBuffer.sHeader.bSourceId          = MSG_NW_TYPE;
         sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NW;
         sMsgBuffer.sHeader.iInstance          = iTOiLe( 1 );
         sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
         sMsgBuffer.sHeader.bDataSize          = 0;
         sMsgBuffer.sHeader.bCmdExt0           = ABP_NW_IA_NW_TYPE;
         sMsgBuffer.sHeader.bCmdExt1           = 0;
         bControlReg                           |= ABP_CTRL_M_BIT;
         eCmdState                              = MSG_MAP_IO; //MSG_NW_TYPE_STR;
				break;
			case MSG_NW_TYPE_STR:
         sMsgBuffer.sHeader.bSourceId          = MSG_NW_TYPE_STR;
         sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NW;
         sMsgBuffer.sHeader.iInstance          = iTOiLe( 1 );
         sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
         sMsgBuffer.sHeader.bDataSize          = 0;
         sMsgBuffer.sHeader.bCmdExt0           = ABP_NW_IA_NW_TYPE_STR;
         sMsgBuffer.sHeader.bCmdExt1           = 0;
         bControlReg                           |= ABP_CTRL_M_BIT;
         eCmdState                              = MSG_NW_TYPE; //MSG_MAP_IO;
				break;
      case MSG_SETUP_COMPLETE:

         //
         // End the setup phase by sending the SETUP_COMPLETE command after
         // we have received the responses to all commands which we have sent.
         

         if( bOutstandingCmds == 0 )
         {
            //
            // All the configuration commands have been processed by the Anybus-CC.
            // Go ahead and end the setup phase by sending SETUP_COMPLETE.
            

            sMsgBuffer.sHeader.bSourceId          = MSG_SETUP_COMPLETE;
            sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_ANB;
            sMsgBuffer.sHeader.iInstance          = iTOiLe( 1 );
            sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;
            sMsgBuffer.sHeader.bDataSize          = ABP_BOOL_SIZEOF;
            sMsgBuffer.sHeader.bCmdExt0           = ABP_ANB_IA_SETUP_COMPLETE;
            sMsgBuffer.sHeader.bCmdExt1           = 0;
            sMsgBuffer.abData[ 0 ]                = TRUE;
            bControlReg                           |= ABP_CTRL_M_BIT;
            eCmdState                              = MSG_DONE;//MSG_DONE;
         }

         break;

				case MSG_IPADRESS:

					sMsgBuffer.sHeader.bSourceId          = MSG_IPADRESS;
					sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
					if(uiNetworkType==MODBUS_IP) {
						// ModusIP stehen die Network auf anderen Instancen
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_MODBUS_IPADRESSE );
					} else {
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_IPADRESSE );
					}
					sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
					sMsgBuffer.sHeader.bDataSize          = 0;
					sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
					sMsgBuffer.sHeader.bCmdExt1           = 0;
					bControlReg                           |= ABP_CTRL_M_BIT;
					eCmdState                              = MSG_SUBNETMASK;
					iFreigabe++;
				 break;
				 
				case MSG_SUBNETMASK:

					sMsgBuffer.sHeader.bSourceId          = MSG_SUBNETMASK;
					sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
					if(uiNetworkType==MODBUS_IP) {
						// ModusIP stehen die Network auf anderen Instancen
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_MODBUS_SUBNETMASK );
					} else {
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_SUBNETMASK );
					}
					sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
					sMsgBuffer.sHeader.bDataSize          = 0;
					sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
					sMsgBuffer.sHeader.bCmdExt1           = 0;
					bControlReg                           |= ABP_CTRL_M_BIT;
					eCmdState                              = MSG_GATEWAY;
				 break;
				 
				case MSG_GATEWAY:

					sMsgBuffer.sHeader.bSourceId          = MSG_GATEWAY;
					sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
					if(uiNetworkType==MODBUS_IP) {
						// ModusIP stehen die Network auf anderen Instancen
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_MODBUS_GATEWAY );
					} else {
						sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_GATEWAY );
					}
					sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
					sMsgBuffer.sHeader.bDataSize          = 0;
					sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
					sMsgBuffer.sHeader.bCmdExt1           = 0;
					bControlReg                           |= ABP_CTRL_M_BIT;
					eCmdState                              = MSG_SETUP_COMPLETE;
				 break;

      case MSG_DONE:

         //
         // All the initialization commands have already been sent.
         // Check the node adress or baudrate DIP-switches if available, send
         // a update to the Anybus-CC if they have changed.
         
#ifdef USE_DIPSWITCHES
         if( bSw1 != bLastSw1 ) {
            //
            // DIP switch 1 (node adress) has changed.
            // Send a update to the Anybus-CC.
            

            sMsgBuffer.sHeader.bSourceId          = MSG_NODE_ADR;
            sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
            sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_SW1 );
            sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;
            sMsgBuffer.sHeader.bDataSize          = 1;
            sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
            sMsgBuffer.sHeader.bCmdExt1           = 0;
            sMsgBuffer.abData[ 0 ]                = bLastSw1 = bSw1;
            bControlReg                           |= ABP_CTRL_M_BIT;
         } else if( bSw2 != bLastSw2 ) {
            //
            // DIP switch 2 (baudrate) has changed.
            // Send a update to the Anybus-CC.
            

            sMsgBuffer.sHeader.bSourceId          = MSG_BAUDRATE;
            sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
            sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_SW2 );
            sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_SET_ATTR;
            sMsgBuffer.sHeader.bDataSize          = 1;
            sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
            sMsgBuffer.sHeader.bCmdExt1           = 0;
            sMsgBuffer.abData[ 0 ]                = bLastSw2 = bSw2;
            bControlReg                           |= ABP_CTRL_M_BIT;
         } else
#endif
         {
            //
            // New commands can be added here.
						if(iFreigabe>0) {
/*							sMsgBuffer.sHeader.bSourceId          = MSG_IPADRESS;
							sMsgBuffer.sHeader.bDestObj           = ABP_OBJ_NUM_NC;
							if(uiNetworkType==MODBUS_IP) {
								// ModusIP stehen die Network auf anderen Instancen
								sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_MODBUS_IPADRESSE );
							} else {
								sMsgBuffer.sHeader.iInstance          = iTOiLe( ABP_NC_INST_NUM_IPADRESSE );
							}
							sMsgBuffer.sHeader.bCmd               = ABP_MSG_HEADER_C_BIT | ABP_CMD_GET_ATTR;
							sMsgBuffer.sHeader.bDataSize          = 0;
							sMsgBuffer.sHeader.bCmdExt0           = ABP_NC_VAR_IA_VALUE;
							sMsgBuffer.sHeader.bCmdExt1           = 0;
							bControlReg                           |= ABP_CTRL_M_BIT;*/
							iFreigabe = 0;
						}          

         }

         break;

      } // end switch( command state ) 

      if( bControlReg & ABP_CTRL_M_BIT )
      {
         //
         // A new command has been posted to the Anybus-CC.
         // Increment counter for outstanding commands.
         // Clear the Ready bit to prevent the ABCC from sending any commands to us.
         

         bOutstandingCmds++;

         bControlReg &= ~ABP_CTRL_R_BIT;
      }

   } // end if( it's OK to send a command ) 


   if( bControlReg & ABP_CTRL_M_BIT )
   {
      if( iTxMsgBufferOffset < sMsgBuffer.sHeader.bDataSize + 8 )
      {
         //
         // There is message data waiting for transmission to the ABCC
         

         memcpy( &abTxBuffer[ 1 ], &(((UINT8*)&sMsgBuffer)[ iTxMsgBufferOffset ]), min( 16, sizeof(ABP_MsgType) - iTxMsgBufferOffset ) );

         iTxMsgBufferOffset += 16;

      }
      else
      {
         //
         // All fragments have been sent.
         // Terminate fragmentation.
         

         bControlReg &= ~ABP_CTRL_M_BIT;

         iTxMsgBufferOffset = 0;

         if( bOutstandingCmds == 0 )
         {
            //
            // We are not waiting for any repsonses, and the message buffer is free.
            // Set the Ready bit to allow the ABCC to send a command to us.
            

            bControlReg |= ABP_CTRL_R_BIT;
         }
      }
   }


   if( ( bStatusReg & ABP_STAT_S_BITS ) > ABP_ANB_STATE_SETUP )
   {
      //
      // Send new "write process data" to the Anybus-CC.
      // The data format of the process data is network specific.
      // Convert our data accordingly.
      

		 AppProcessData2AnyBus(&abTxBuffer[17]);
   }

   if( ( bStatusReg & ABP_STAT_S_BITS ) == ABP_ANB_STATE_PROCESS_ACTIVE )
   {
      //
      // The "read process data" is only valid in the PROCESS_ACTIVE state.
      // Retrieve the new "read process data" from the Anybus-CC.
      // The data format of the process data is network specific.
      // Convert it to our native format.
      

		 if((*piRxSize>=(19+uiReadArraySize))&&(uiReadArraySize>0)) {
			 // Wegen einem Fehler im Bacnet-Modul geht das Lesen von Processdaten nicht.
			 // Diese stellen im Puffer werden mit der CRC-Summe beschrieben
			 AnyBusData2AppProcess(&pbRxData[17]);
		 }
   }

   //
   // Finally, complete the telegram with the control register and CRC values
   // and send it to the Anybus-CC.
   

   abTxBuffer[ 0 ] = bControlReg;

   iCrc = Crc16( abTxBuffer, (UINT16)(*piTxSize - 2) );

   abTxBuffer[ *piTxSize - 2 ] = (UINT8)( ( iCrc >> 8 ) & 0x00FF );
   abTxBuffer[ *piTxSize - 1 ] = (UINT8)( iCrc & 0x00FF );

   return( abTxBuffer );

} // end of AnybusMain() 

//------------------------------------------------------------------------------
// AnybusNeustart()
// alles wieder auf Anfang setzen
//------------------------------------------------------------------------------
void AnybusNeustart(void){
	
	bControlReg = ABP_CTRL_R_BIT | ABP_CTRL_T_BIT;
	bStatusReg = 0;
#ifdef USE_DIPSWITCHES
   eCmdState   = MSG_NODE_ADR;
#else
   eCmdState   = MSG_DATA_FORMAT;//MSG_MAP_IO_1;
#endif

	bOutstandingCmds = 0;
	uiWriteArraySize = 0;
	uiReadArraySize = 0;
	uiMSG_MAP_IO_INDEX = 0;
	uiMSG_MAP_SUB_INDEX = 0;
	eNetFormat = NET_UNKNOWN;
	uiNetworkType = 0;
	uiMaxInstanceNumber = 0;
	iMsgBufferOffset = 0;
	bOutstandingCmds = 0;
	
}
//------------------------------------------------------------------------------
// appd_Compare()
//------------------------------------------------------------------------------
SINT32 appd_Compare( void* pVal1, void* pVal2, UINT8 bDataType ) {
	switch(bDataType){
		case ABP_BOOL:
			return ( (SINT32)*(BOOL8*)pVal1 - (SINT32)*(BOOL8*)pVal2 );
		case ABP_SINT8:
			return ( (SINT32)*(SINT8*)pVal1 - (SINT32)*(SINT8*)pVal2 );
		case ABP_SINT16:
			return ( (SINT32)*(SINT16*)pVal1 - (SINT32)*(SINT16*)pVal2 );
		case ABP_SINT32:
			if( *(SINT32*)pVal1 > *(SINT32*)pVal2 ) {
				return ( 1 );
			}
			if( *(SINT32*)pVal1 < *(SINT32*)pVal2 ) {
				return ( -1 );
			}
			return ( 0 );
		case ABP_UINT8:
			return ( (SINT32)*(UINT8*)pVal1 - (SINT32)*(UINT8*)pVal2 );
		case ABP_UINT16:
			return ( (SINT32)*(UINT16*)pVal1 - (SINT32)*(UINT16*)pVal2 );
		case ABP_UINT32:
			if( *(UINT32*)pVal1 > *(UINT32*)pVal2 ) {
				return ( 1 );
			}
			if( *(UINT32*)pVal1 < *(UINT32*)pVal2 ) {
				return ( -1 );
			}
			return 0;
		case ABP_CHAR:
			return ( (SINT32)*(UINT8*)pVal1 - (SINT32)*(UINT8*)pVal2 );
		case ABP_ENUM:
			return ( (SINT32)*(UINT8*)pVal1 - (SINT32)*(UINT8*)pVal2 );
		case ABP_SINT64:
			if((*(SINT64*)pVal1)>(*(SINT64*)pVal2)) {
				return 1;
			}
			if((*(SINT64*)pVal1)<(*(SINT64*)pVal2)) {
				return -1;
			}
			return 0;
		case ABP_UINT64:
			if((*(UINT64*)pVal1)>(*(UINT64*)pVal2)) {
				return 1;
			}
			if((*(UINT64*)pVal1)<(*(UINT64*)pVal2)) {
				return -1;
			}
			return 0;
		case ABP_FLOAT:
			if( *(FLOAT32*)pVal1 > *(FLOAT32*)pVal2 ) {
				return ( 1 );
			}
			if( *(FLOAT32*)pVal1 < *(FLOAT32*)pVal2 ) {
				return ( -1 );
			}
			return ( 0 );
		default:
			return ( 0 );
	}
} // End of appd_Compare()

//------------------------------------------------------------------------------
// InstanceMaxValue()
// gibt evtl. vorhandenen Grenzwerte zurück
//------------------------------------------------------------------------------
unsigned char CopyInstanceMinValue(unsigned int uiInstanceNumber, void* pMsgBuffer)
{
	unsigned char ucAnzahlBytes = 0;
	anybusDataTableRow *pDataRow;
	unsigned int uiSubIndex;
	signed int iIndex;
	float flDaten;
	UINT8 ucDataTyp;
	UINT8 ucMinValue;
		
	ucAnzahlBytes = 0;
	pDataRow = NULL;
	uiSubIndex = 0;
	for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(pDataRow==NULL); iIndex--) {
		if(anybusDataTable[iIndex].uiInstanceNumber<=uiInstanceNumber) {
			for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
				if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==uiInstanceNumber) {
					// gefunden
					pDataRow = &anybusDataTable[iIndex];
					break;
				}
			}
		}
	}

	if(pDataRow!=NULL) {
		
		switch(Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].konv) {
			case ZEIT_FORM:
				ucMinValue = 0;
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMinValue,ucAnzahlBytes);
				break;
			case DATUM_FORM:
				if(uiSubIndex==0) {
					ucMinValue = 1;
				} else if(uiSubIndex==1) {
					ucMinValue = 1;
				} else {
					ucMinValue = 0;
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMinValue,ucAnzahlBytes);
				break;
			case DATE_FORM:
				if(uiSubIndex==0) {
					ucMinValue = 0;
				} else if(uiSubIndex==1) {
					ucMinValue = 1;
				} else {
					ucMinValue = 1;
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMinValue,ucAnzahlBytes);
				break;
			case UHR_FORM:
				ucMinValue = 0;
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMinValue,ucAnzahlBytes);
				break;
			default:
				ucAnzahlBytes = 0;
			break;
		}
	}
	return ucAnzahlBytes;
} // end of CopyInstanceMinValue
//------------------------------------------------------------------------------
// InstanceMaxValue()
// gibt evtl. vorhandenen Grenzwerte zurück
//------------------------------------------------------------------------------
unsigned char CopyInstanceMaxValue(unsigned int uiInstanceNumber, void* pMsgBuffer)
{
	unsigned char ucAnzahlBytes = 0;
	anybusDataTableRow *pDataRow;
	unsigned int uiSubIndex;
	signed int iIndex;
	float flDaten;
	UINT8 ucDataTyp;
	UINT8 ucMaxValue;
		
	ucAnzahlBytes = 0;
	pDataRow = NULL;
	uiSubIndex = 0;
	for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(pDataRow==NULL); iIndex--) {
		if(anybusDataTable[iIndex].uiInstanceNumber<=uiInstanceNumber) {
			for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
				if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==uiInstanceNumber) {
					// gefunden
					pDataRow = &anybusDataTable[iIndex];
					break;
				}
			}
		}
	}

	if(pDataRow!=NULL) {
		
		switch(Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].konv) {
			case ZEIT_FORM:
				if(uiSubIndex==0) {
					if(Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].attr==0) {
						ucMaxValue = 23;
					} else {
						ucMaxValue = 31;
					}
				} else {
					if(Pgruppe[KomTab[pDataRow->uiDAE].gnum].Param[KomTab[pDataRow->uiDAE].pnum].attr==0) {
						ucMaxValue = 59;
					} else {
						ucMaxValue = 12;
					}
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMaxValue,ucAnzahlBytes);
				break;
			case DATUM_FORM:
				if(uiSubIndex==0) {
					ucMaxValue = 31;
				} else if(uiSubIndex==1) {
					ucMaxValue = 12;
				} else {
					ucMaxValue = 99;
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMaxValue,ucAnzahlBytes);
				break;
			case DATE_FORM:
				if(uiSubIndex==0) {
					ucMaxValue = 99;
				} else if(uiSubIndex==1) {
					ucMaxValue = 12;
				} else {
					ucMaxValue = 31;
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMaxValue,ucAnzahlBytes);
				break;
			case UHR_FORM:
				if(uiSubIndex==0) {
					ucMaxValue = 23;
				} else if(uiSubIndex==1) {
					ucMaxValue = 59;
				} else {
					ucMaxValue = 59;
				}
				ucDataTyp = GetAnyBusDataTyp(pDataRow);
				ucAnzahlBytes = appd_abTypesize[ucDataTyp];
				memcpy(pMsgBuffer,&ucMaxValue,ucAnzahlBytes);
				break;
			default:
				ucAnzahlBytes = 0;
			break;
		}
	}
	return ucAnzahlBytes;
} // end of CopyInstanceMaxValue

//------------------------------------------------------------------------------
// CopyData2AnyBus()
// kopiert die Daten in den Messagebuffer in abhängigkeit des Datentypes
//------------------------------------------------------------------------------
unsigned char CopyData2AnyBus(unsigned int uiInstanceNumber /*anybusDataTableRow* pappData*/, /*void *pValue,*/void* pMsgBuffer)
{
	unsigned char ucAnzahlBytes = 0;
	anybusDataTableRow *pDataRow;
	unsigned int uiSubIndex;
	signed int iIndex;
	float flDaten;
		
	pDataRow = NULL;
	uiSubIndex = 0;
	for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(pDataRow==NULL); iIndex--) {
		if(anybusDataTable[iIndex].uiInstanceNumber<=uiInstanceNumber) {
			for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
				if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==uiInstanceNumber) {
					// gefunden
					pDataRow = &anybusDataTable[iIndex];
					break;
				}
			}
		}
	}

	if(pDataRow!=NULL) {
		ucAnzahlBytes = RiedelKonvert((char*)&flDaten,pDataRow->uiDAE,KONVRD,0);
		if(ucAnzahlBytes>0) {
			ucAnzahlBytes = AnybusKonvert(&flDaten,(UINT8*)pMsgBuffer,pDataRow,KONVRD, uiSubIndex);
		}
	}
	return ucAnzahlBytes;
} // end of CopyData2AnyBus

//------------------------------------------------------------------------------
// CopyAnyBus2Data()
// kopiert die Daten aus dem Messagebuffer in abhängigkeit des Datentypes
//------------------------------------------------------------------------------

signed char CopyAnyBus2Data(unsigned int uiInstance/*anybusDataTableRow* pappData*/, void* pMsgBuffer)
{
	signed char cReturn = 0;
	unsigned char ucAnzahlBytes = 0;
//	unsigned char ucIndex = 0;
	float		flData;
	anybusDataTableRow *pDataRow;
	unsigned int uiSubIndex;
	signed int iIndex;
		
	pDataRow = NULL;
	uiSubIndex = 0;
	for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(pDataRow==NULL); iIndex--) {
		if(anybusDataTable[iIndex].uiInstanceNumber<=uiInstance) {
			for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
				if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==uiInstance) {
					// gefunden
					pDataRow = &anybusDataTable[iIndex];
					if(((pDataRow->ucZugriff&ABP_NC_DESCR_SET_ACCESS)==ABP_NC_DESCR_SET_ACCESS)&&(pDataRow->siProcessIndex==NO_PROCESS_DATA)) {
						// Zeitstempel für Schreibvorgang ablegen
						ulWriteStamp = LongZeit;
						serv_cnt  = 3000;													// nach 5 Minuten Parameter sichern (in Bedien.c)
					}
					break;
				}
			}
		}
	}

	if(pDataRow!=NULL) {
		if(pDataRow->uiInstanceAnzahl>1) {
			// flData mit den alten Werten füllen, um Sie anschließend in AnybusKonvert zu überschreiben, dann wieder auf die Daten schaffen
			ucAnzahlBytes = RiedelKonvert((char*)&flData,pDataRow->uiDAE,KONVRD,0);
		}
		ucAnzahlBytes = AnybusKonvert(&flData,pMsgBuffer,pDataRow,KONVWR, uiSubIndex);
		if(ucAnzahlBytes>0) {
			ucAnzahlBytes = RiedelKonvert((char*)&flData,pDataRow->uiDAE,KONVWR,0);
		}
	}
	return ucAnzahlBytes;
} // end of CopyAnyBus2Data

//------------------------------------------------------------------------------
// ApplicationObject()
//------------------------------------------------------------------------------

void ApplicationObject( void )
{
	UINT8 ucDataSize;
	UINT16 uiAnzahl, uiSubIndex;
	UINT16 uiOrdernumber;
	SINT16 iIndex;
	anybusDataTableRow *pDataRow;
	
	pDataRow = NULL;
	switch( iLeTOi( sMsgBuffer.sHeader.iInstance ) ) {
		case ABP_INST_OBJ: // The object instance 

			switch( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) {
				case ABP_CMD_RESET:

					switch( sMsgBuffer.sHeader.bCmdExt1 ) {
						case ABP_RESET_POWER_ON:
							AnybusNeustart();
							break;
						case ABP_RESET_FACTORY_DEFAULT:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
						case ABP_RESET_POWER_ON_FACTORY_DEFAULT:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
						default:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
					} // end switch( attribute ) 

					break;
				case ABP_CMD_RESET_REQUEST:
					switch( sMsgBuffer.sHeader.bCmdExt1 ) {
						case ABP_RESET_POWER_ON:
							break;
						case ABP_RESET_FACTORY_DEFAULT:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
						case ABP_RESET_POWER_ON_FACTORY_DEFAULT:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
						default:
							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_1 );
							break;
					} // end switch( attribute ) 

					break;
				default:
					SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
					break;
			} // end switch( command ) 

			break;
		default: 
			SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_INST );
//			SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_OBJ );
			break;

	} // end switch( instance ) 


} // end of ApplicationObject() 

//------------------------------------------------------------------------------
// ApplicationDataObject()
//------------------------------------------------------------------------------

void ApplicationDataObject( void )
{
	UINT8 ucDataSize;
	UINT16 uiAnzahl, uiSubIndex;
	UINT16 uiOrdernumber;
	SINT16 iIndex;
	anybusDataTableRow *pDataRow;
	
	pDataRow = NULL;
	switch( iLeTOi( sMsgBuffer.sHeader.iInstance ) ) {
		case ABP_INST_OBJ: // The object instance 

			switch( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) {
				case ABP_CMD_GET_ATTR:

					switch( sMsgBuffer.sHeader.bCmdExt0 ) {
						case ABP_OA_NAME: // Name 

							memcpy( sMsgBuffer.abData, "Application data", 16 );
							sMsgBuffer.sHeader.bDataSize = 16;

							break;

						case ABP_OA_REV: // Revision 

							sMsgBuffer.abData[ 0 ] = 1;
							sMsgBuffer.sHeader.bDataSize = ABP_OA_REV_DS;

							break;

						case ABP_OA_NUM_INST: // Number of instances 
						
							// Anzahl der Instanzen hängt von der Initialisierung aus der Komtab ab
							uiAnzahl = 0;
/*							for(iIndex=0; iIndex<uiNumber_of_Data; iIndex++) {
								if(anybusDataTable[iIndex].ucZugriff!=ANYBUS_NONE_ACCESS) {
									uiAnzahl += anybusDataTable[iIndex].uiInstanceAnzahl;
								}
							}*/
							uiAnzahl = uiInstanceCount; //120628 uiMaxInstanceNumber;
							*(UINT16*)&sMsgBuffer.abData[ 0 ] = iTOiLe( uiAnzahl);
							sMsgBuffer.sHeader.bDataSize = ABP_OA_NUM_INST_DS;

							break;

						case ABP_OA_HIGHEST_INST: // Highest instance no. 

							// die höhste Instanzen hängt von der Initialisierung aus der Komtab ab
							uiAnzahl = 0;
/*							for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
								//Rückwärts gehen, die Höchste Nummer steht am Ende
								if(anybusDataTable[iIndex].ucZugriff!=ANYBUS_NONE_ACCESS) {
									uiAnzahl = anybusDataTable[iIndex].uiInstanceNumber + anybusDataTable[iIndex].uiInstanceAnzahl - 1;
									break;
								}
							}*/
							uiAnzahl = uiMaxInstanceNumber;
							*(UINT16*)&sMsgBuffer.abData[ 0 ] = iTOiLe( uiAnzahl );
							sMsgBuffer.sHeader.bDataSize = ABP_OA_HIGHEST_INST_DS;

							break;

						default: // Unsupported attribute 

							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );

							break;

					} // end switch( attribute ) 

					break;

			case ABP_APPD_CMD_GET_INST_BY_ORDER:

					// Wenn nicht alle Datenpunkte in der Komtab standen bzw. nicht für Anybus sind,
					// entstehen Lücken zwischen den Instancenummern und der Ordernummer
					// Hier wird die Zuordnung zwischen den beiden ermittelt.
					uiOrdernumber = iLeTOi( *(UINT16*)&sMsgBuffer.sHeader.bCmdExt0 );
//120622 Odernumber und Instancenumber sind gleich					
					uiAnzahl = uiOrdernumber;
					uiAnzahl = 0;
					for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(uiAnzahl==0); iIndex--) {
						if((anybusDataTable[iIndex].ucZugriff!=ANYBUS_NONE_ACCESS)&&(anybusDataTable[iIndex].uiOrderNumber<=uiOrdernumber)) {
							// Diese Instance ist initialisiert worden, 
							// Hier muss sich die Ordernumber befinden
							for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
								if((anybusDataTable[iIndex].uiOrderNumber+uiSubIndex)==uiOrdernumber) {
									// gewünschte Ordernumber wurde gefunden
									// Instancenummer zur Ordernummer merken
									uiAnzahl = anybusDataTable[iIndex].uiInstanceNumber + uiSubIndex;
									break;
								}
							}
						}
					}
				
					if(uiAnzahl==0) {
						// Ordernummer wurde nicht gefunden
						SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
					} else {
						*(UINT16*)&sMsgBuffer.abData[ 0 ] = iTOiLe(uiAnzahl);
						sMsgBuffer.sHeader.bDataSize = ABP_UINT16_SIZEOF;
					}

					break;

			default:

					SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_CMD );

					break;

			} // end switch( command ) 

			break;
		default: //Hostapplication Instances
			if(sMsgBuffer.sHeader.iInstance<=uiMaxInstanceNumber) {
				switch( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) {
					case ABP_CMD_GET_ATTR:

						switch( sMsgBuffer.sHeader.bCmdExt0 )	{
								case ABP_APPD_IA_NAME: // Instance name 

									//
									// Get the name of the requested ADI
									for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
										if(anybusDataTable[iIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
											// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
											for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
												if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
													// gefunden
													pDataRow = &anybusDataTable[iIndex];
													break;
												}
											}
											break;
										}
									}
									//120623 Austausch von &anybusDataTable[iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ] mit pDataRow
									// Gruppenkennzeichen
									if(pDataRow!=0) {
										strcpy( (char*)sMsgBuffer.abData, (char*)GetGruppe(pDataRow) );
										sMsgBuffer.sHeader.bDataSize = strlen( (char*)GetGruppe(pDataRow) );

										//Bezeichnung
										strcpy( (char*)&sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize], (char*)GetName(pDataRow) );
										sMsgBuffer.sHeader.bDataSize += strlen( (char*)GetName(pDataRow) );
									
										sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize] = '[';
										sMsgBuffer.sHeader.bDataSize +=1;
										strcpy( (char*)&sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize], (char*)GetUnit(pDataRow) );
										for(iIndex=0;iIndex<8;iIndex++) {
											if(sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize+iIndex]=='') {
												// Grad austauschen in "richtiges" Grad
												sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize+iIndex] = '°';
												break;
											}
										}
										sMsgBuffer.sHeader.bDataSize += strlen( (char*)GetUnit(pDataRow) );
										sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize] = ']';
										sMsgBuffer.sHeader.bDataSize +=1;
										sMsgBuffer.abData[sMsgBuffer.sHeader.bDataSize] = '\0';
									}		
									break;

								case ABP_APPD_IA_DATA_TYPE: // Data type 

									for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
										if(anybusDataTable[iIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
											// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
											pDataRow = &anybusDataTable[iIndex];
/*											for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
												if(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
													// gefunden
													break;
												}
											}*/
											break;
										}
									}
//120623									sMsgBuffer.abData[ 0 ] = GetAnyBusDataTyp(&anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ]);
									sMsgBuffer.abData[ 0 ] = GetAnyBusDataTyp(pDataRow);
									sMsgBuffer.sHeader.bDataSize = ABP_APPD_IA_DATA_TYPE_DS;

									break;

								case ABP_APPD_IA_NUM_ELEM: // Number of elements 

									for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
										if(anybusDataTable[iIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
											// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
											pDataRow = &anybusDataTable[iIndex];
/*											for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
												if(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
													// gefunden
													break;
												}
											}*/
											break;
										}
									}
//120623									sMsgBuffer.abData[ 0 ] = GetAnyBusDataTypNumbers(&anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ]);
									sMsgBuffer.abData[ 0 ] = GetAnyBusDataTypNumbers(pDataRow);
									sMsgBuffer.sHeader.bDataSize = ABP_APPD_IA_NUM_ELEM_DS;

									break;

								case ABP_APPD_IA_DESCRIPTOR: // Descriptor 

									for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
										if(anybusDataTable[iIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
											// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
											pDataRow = &anybusDataTable[iIndex];
/*											for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
												if(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
													// gefunden
													break;
												}
											}*/
											break;
										}
									}
//120623									sMsgBuffer.abData[ 0 ] = anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ].ucZugriff;
									sMsgBuffer.abData[ 0 ] = pDataRow->ucZugriff;
									sMsgBuffer.sHeader.bDataSize = ABP_APPD_IA_DESCRIPTOR_DS;

									break;

								case ABP_APPD_IA_VALUE: // Value 

									//
									// A request to read the application data instance value
									// The data format of ADI values is network specific.
									// Convert our native data format accordingly.
								/*	for(uiIndex=uiNumber_of_Data-1; uiIndex>=0; uiIndex--) {
										if(anybusDataTable[uiIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
											// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
											for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
												if(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
													// gefunden
													pDataRow = &anybusDataTable[uiIndex];
													break;
												}
											}
											break;
										}
									}*/
//120623									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(anybusDataTable[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],&sMsgBuffer.abData[0]);
//120623									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(pDataRow,&sMsgBuffer.abData[0]);
									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(iLeTOi( sMsgBuffer.sHeader.iInstance ),&sMsgBuffer.abData[0]);
									break;
								case ABP_APPD_IA_MAX_VALUE:
									if((sMsgBuffer.sHeader.bDataSize=CopyInstanceMaxValue(iLeTOi( sMsgBuffer.sHeader.iInstance ), &sMsgBuffer.abData[0]))!=0) {
									} else {
										// Unsupported attribute
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
									}
//									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1].pxMaxValue,&sMsgBuffer.abData[0]);
									break;
								case ABP_APPD_IA_MIN_VALUE:
									if((sMsgBuffer.sHeader.bDataSize=CopyInstanceMinValue(iLeTOi( sMsgBuffer.sHeader.iInstance ), &sMsgBuffer.abData[0]))!=0) {
									} else {
										// Unsupported attribute
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
									}
//									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1].pxMinValue,&sMsgBuffer.abData[0]);
									break;
								case ABP_APPD_IA_DFLT_VALUE:
//									sMsgBuffer.sHeader.bDataSize = CopyData2AnyBus(appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],appd_asInstance[iLeTOi(sMsgBuffer.sHeader.iInstance)-1].pxDefaultValue,&sMsgBuffer.abData[0]);
									break;

								default: // Unsupported attribute 

									SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );

									break;

							} // end switch( attribute ) 

						break;

					case ABP_CMD_SET_ATTR:

						switch( sMsgBuffer.sHeader.bCmdExt0 ){
							case ABP_APPD_IA_NAME: // Instance name 
							case ABP_APPD_IA_DATA_TYPE: // Data type 
							case ABP_APPD_IA_NUM_ELEM: // Number of elements 
							case ABP_APPD_IA_DESCRIPTOR: // Descriptor 
							case ABP_APPD_IA_MAX_VALUE:
							case ABP_APPD_IA_MIN_VALUE:
							case ABP_APPD_IA_DFLT_VALUE:

								SetMsgError( &sMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );

								break;

							case ABP_APPD_IA_VALUE: // Value 
								pDataRow = NULL;
								for(iIndex=uiNumber_of_Data-1; iIndex>=0; iIndex--) {
									if(anybusDataTable[iIndex].uiInstanceNumber<=iLeTOi( sMsgBuffer.sHeader.iInstance )) {
										// Hier muss jetzt das Objekt vorliegen, evtl ist es eine SubInstance vom Datenpunkt
										pDataRow = &anybusDataTable[iIndex];
/*											for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
											if(anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex==iLeTOi( sMsgBuffer.sHeader.iInstance )) {
												// gefunden
												pDataRow = &anybusDataTable[iIndex];
												break;
											}
										}*/
										break;
									}
								}
								if(pDataRow!=NULL) {
//120623								ucDataSize = appd_abTypesize[GetAnyBusDataTyp(&anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ])] * GetAnyBusDataTypNumbers(&anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ]);
									ucDataSize = appd_abTypesize[GetAnyBusDataTyp(pDataRow)] * GetAnyBusDataTypNumbers(pDataRow);
									if((pDataRow->ucZugriff&ABP_NC_DESCR_SET_ACCESS)==0) {
										// Keine Schreibrechte für diesen Parameter
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );
									} 
									else if( sMsgBuffer.sHeader.bDataSize > ucDataSize)
									{
											SetMsgError( &sMsgBuffer, 1, ABP_ERR_TOO_MUCH_DATA );
									}
									else if( sMsgBuffer.sHeader.bDataSize < ucDataSize)
									{
											SetMsgError( &sMsgBuffer, 1, ABP_ERR_NOT_ENOUGH_DATA );
									}
									else
									{
										//
										// Write the parameter value.
										// The data format of ADI values is network specific.
										// Convert it to our native data format.
										
//120623									if(CopyAnyBus2Data(anybusDataTable[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],&sMsgBuffer.abData[0])<0) 
									if(CopyAnyBus2Data(iLeTOi(sMsgBuffer.sHeader.iInstance),&sMsgBuffer.abData[0])<0) {
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
									}

								}
							} else {
								SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
							}

/*									if((anybusDataTable[ iLeTOi( sMsgBuffer.sHeader.iInstance ) - 1 ].ucZugriff&ABP_NC_DESCR_SET_ACCESS)==0) {
										// Keine Schreibrechte für diesen Parameter
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_ATTR_NOT_SETABLE );
									} 
									else if( sMsgBuffer.sHeader.bDataSize > ucDataSize)
									{
											SetMsgError( &sMsgBuffer, 1, ABP_ERR_TOO_MUCH_DATA );
									}
									else if( sMsgBuffer.sHeader.bDataSize < ucDataSize)
									{
											SetMsgError( &sMsgBuffer, 1, ABP_ERR_NOT_ENOUGH_DATA );
									}
									else
									{
										//
										// Write the parameter value.
										// The data format of ADI values is network specific.
										// Convert it to our native data format.
										
//120623									if(CopyAnyBus2Data(anybusDataTable[iLeTOi(sMsgBuffer.sHeader.iInstance)-1],&sMsgBuffer.abData[0])<0) {
									if(CopyAnyBus2Data(iLeTOi(sMsgBuffer.sHeader.iInstance),&sMsgBuffer.abData[0])<0) {
										SetMsgError( &sMsgBuffer, 1, ABP_ERR_OUT_OF_RANGE );
									}

								}
*/
								break;

							default: // Unsupported attribute 

								SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );

								break;

						} // end switch( attribute ) 

						break;

				default: // Unsupported command 

						SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_CMD );

						break;

				} // end switch( command ) 


			} else {
				//
				// Request to a unsupported Application Data Instance.
				// Respond with the appropriate error code.
				

				SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_INST );
			}
			break;

	} // end switch( instance ) 


} // end of ApplicationDataObject() 


//------------------------------------------------------------------------------
// BacnetHostObject()
//------------------------------------------------------------------------------

void BacnetHostObject( void )
{
	UINT8 ucDataSize;
	UINT16 uiIndex,uiAnzahl, uiSubIndex;
	UINT16 uiBacnNetObjectType;
	UINT16 uiAdiNumber;
	UINT16 uiIndexBacnetObjekt;
	UINT32 ulBacnetInstanceNumber;
	UINT32 ulObjectInstanceNumber;
	UINT8  carName[20];
	SINT16 iIndex;
	anybusDataTableRow *pDataRow;
	
	pDataRow = NULL;
	switch( iLeTOi( sMsgBuffer.sHeader.iInstance ) ) {
		case ABP_INST_OBJ: // The object instance 

			switch( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) {
				case ABP_CMD_GET_ATTR:

					switch( sMsgBuffer.sHeader.bCmdExt0 ) {
						case ABP_OA_NAME: // Name 

							memcpy( sMsgBuffer.abData, "Riedel Bacnet", 13 );
							sMsgBuffer.sHeader.bDataSize = 13;

							break;


						default: // Unsupported attribute 

							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );

							break;

					} // end switch( attribute ) 

					break;

				case ABP_BACNET_CMD_GET_ADI_BY_BACNET_OBJECT:

					// Wenn nicht alle Datenpunkte in der Komtab standen bzw. nicht für Anybus sind,
					// entstehen Lücken zwischen den Instancenummern und der Ordernummer
					// Die INstancenumber Bacnet ist zusätzlich vom Datentyp abhängig. Für jeden Datentyp (Analog, Binär, Multistate) gibt es Ordernumber von 0..2039
					// Hier wird die Zuordnung zwischen den beiden ermittelt.
					uiBacnNetObjectType = iLeTOi( *(UINT16*)&sMsgBuffer.abData[0] );
					ulBacnetInstanceNumber = *(UINT32*)&sMsgBuffer.abData[2];
					
					uiAnzahl = 0;
//					ulBacnetInstanceNumber += 1;
					for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(uiAnzahl==0); iIndex--) {
						if(anybusDataTable[iIndex].ucZugriff!=ANYBUS_NONE_ACCESS) {
							// Diese Instance ist initialisiert worden, 
							if(anybusDataTable[iIndex].uiObjectTyp==uiBacnNetObjectType) {
								// Gleichen Objekttyp gefunden
								if(anybusDataTable[iIndex].ulObjectInstance<=ulBacnetInstanceNumber) {
									// Habe jetzt den Index für die Bacnetnummer gefunden, nur noch den Subindex ermitteln
									for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
										if(anybusDataTable[iIndex].ulObjectInstance+uiSubIndex==ulBacnetInstanceNumber) {
										//if(--ulBacnetInstanceNumber==0)
											// gewünschte Ordernumber wurde gefunden
											uiAnzahl = anybusDataTable[iIndex].uiInstanceNumber + uiSubIndex; 
											break;
										}
									}
//								ulBacnetInstanceNumber--;
								}
							}
						}
					}
					
					if(uiAnzahl==0) {
						// Ordernummer wurde nicht gefunden
						SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
					} else {
						*(UINT16*)&sMsgBuffer.abData[ 0 ] = iTOiLe(uiAnzahl);
						sMsgBuffer.sHeader.bDataSize = ABP_UINT16_SIZEOF;
					}
					break;
				case ABP_BACNET_CMD_GET_ADI_BY_BACNET_NAME:
				// Namen speichern
				memcpy(carName,sMsgBuffer.abData,20);
				break;
			case ABP_BACNET_CMD_GET_ALL_BACNET_OBJECT_INSTANCES: // Anfrage eines Bacnetobjectes -> Antwort ein Bitsets von 2040 Bits, 1= Instance vorhanden
				uiBacnNetObjectType = sMsgBuffer.abData[0];
				uiAnzahl = 0x01;
/*				for(uiIndex=0; uiIndex<255; uiIndex++) {
					sMsgBuffer.abData[uiIndex] = 0;
				}*/
				memset(sMsgBuffer.abData,0,255);

				ulBacnetInstanceNumber = 0;
				for(uiIndex=0; uiIndex<uiNumber_of_Data; uiIndex++) {
					if(anybusDataTable[uiIndex].ucZugriff!=ANYBUS_NONE_ACCESS) {
						// Diese Instance ist initialisiert worden, 
						pDataRow = &anybusDataTable[uiIndex];

						if(pDataRow->uiObjectTyp==0) {
							// Objekttyp nochnicht gestzt 
							switch(GetAnyBusDataTyp(pDataRow)) {
								case ABP_BOOL:
									pDataRow->uiObjectTyp = BACNET_BINARY;
									break;
								case ABP_ENUM:
									pDataRow->uiObjectTyp = BACNET_MULTI;
									break;
								case ABP_FLOAT:
								case ABP_UINT8:
								case ABP_SINT8:
								case ABP_UINT16:
								case ABP_SINT16:
								case ABP_UINT32:
								case ABP_SINT32:
									pDataRow->uiObjectTyp = BACNET_ANALOG;
									break;
								case ABP_CHAR:
									pDataRow->uiObjectTyp = BACNET_STRING;
									break;
								default:
									pDataRow->uiObjectTyp = BACNET_PROPRIETARY;
									break;
							}
						}
						if(pDataRow->uiObjectTyp==uiBacnNetObjectType) {
							// Gleichen Objekttyp gefunden
							for(uiSubIndex=0; uiSubIndex<pDataRow->uiInstanceAnzahl; uiSubIndex++) {
								// Dieser gilt für alle SubInstancen dieses Objektes
								if(uiSubIndex==0) {
									pDataRow->ulObjectInstance = ulBacnetInstanceNumber;
								}
								sMsgBuffer.abData[(ulBacnetInstanceNumber/8)] |= uiAnzahl<<((ulBacnetInstanceNumber)%8);
								ulBacnetInstanceNumber++;
							}
						}
					} else {
						// Diese Instance exisitert nicht kein Bit Setzen
					}
				}
				sMsgBuffer.sHeader.bDataSize = 255;
				break;
			case ABP_BACNET_CMD_GET_BACNET_OBJECT_BY_ADI:
					// Anhand der Adinumber wird hier das entsprechende Element gefunden und dem Anybus die Bacnetkonfiguration mitgeteilt
					uiAdiNumber = iLeTOi( *(UINT16*)&sMsgBuffer.abData[0] );
					uiIndexBacnetObjekt = 0;
					uiSubIndex = 0;
					for(iIndex=uiNumber_of_Data-1; (iIndex>=0)&&(uiIndexBacnetObjekt==0); iIndex--) {
						if((anybusDataTable[iIndex].uiInstanceNumber)<=uiAdiNumber) {
							// habe den Index für das Adiobjekt gefunden
							for(uiSubIndex=0; uiSubIndex<anybusDataTable[iIndex].uiInstanceAnzahl; uiSubIndex++) {
								if((anybusDataTable[iIndex].uiInstanceNumber+uiSubIndex)==uiAdiNumber) {
									// gewünschte Ordernumber wurde gefunden
									// Index zur Ordernummer merken
									uiIndexBacnetObjekt = iIndex+1;
									pDataRow = &anybusDataTable[iIndex];
									break;
								}
							}
						}
					}
					if((uiIndexBacnetObjekt-->0)&&(pDataRow!=NULL)) {
						// Objekttyp merken
						if(pDataRow->uiObjectTyp==0) {

							switch(GetAnyBusDataTyp(pDataRow)) {
								case ABP_BOOL:
									pDataRow->uiObjectTyp = BACNET_BINARY;
									break;
									case ABP_ENUM:
										pDataRow->uiObjectTyp = BACNET_MULTI;
										break;
									case ABP_FLOAT:
									case ABP_UINT8:
									case ABP_SINT8:
									case ABP_UINT16:
									case ABP_SINT16:
									case ABP_UINT32:
									case ABP_SINT32:
										pDataRow->uiObjectTyp = BACNET_ANALOG;
										break;
									case ABP_CHAR:
										pDataRow->uiObjectTyp = BACNET_STRING;
										break;
									default:
										pDataRow->uiObjectTyp = BACNET_PROPRIETARY;
										break;
							}
						}
						memcpy(&sMsgBuffer.abData[0],&pDataRow->uiObjectTyp,2);
						ulObjectInstanceNumber = pDataRow->ulObjectInstance +uiSubIndex;
						memcpy(&sMsgBuffer.abData[2],&ulObjectInstanceNumber , 4);
						sMsgBuffer.sHeader.bDataSize = 6;
					} else {
						// Fehler Objeckt nicht gefunden
						SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
					}

					break;
			default:
					SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );

					break;

			} // end switch( command ) 

			break;
		case ABP_INST_ATTRIBUTE:
			switch( sMsgBuffer.sHeader.bCmd & ABP_MSG_HEADER_CMD_BITS ) {
				case ABP_CMD_GET_ATTR:
					switch( sMsgBuffer.sHeader.bCmdExt0 )	{
						case ABP_APPD_IA_BACNET_ONAME:	// Objektname
							// Evtl. Reglerbezeichnung
							
//							memcpy( sMsgBuffer.abData, "Riedel Bacnet", 13 );
							memcpy( sMsgBuffer.abData, "Riedel ", 7 );
							memcpy(&sMsgBuffer.abData[7],Kategorie_Typ, 15);
							sMsgBuffer.sHeader.bDataSize = 7+15;
							break;
						case ABP_APPD_IA_BACNET_VNAME:
							// Bacnet-Vendorname
							strcpy(sMsgBuffer.abData, "Dr. Riedel Automatisierungstechnik GmbH");
							sMsgBuffer.sHeader.bDataSize = strlen("Dr. Riedel Automatisierungstechnik GmbH");
							break;
						case ABP_APPD_IA_BACNET_VID:
							// Bacnet-VendorID
							*(UINT16*)&sMsgBuffer.abData[ 0 ] = iTOiLe(577); // VendorID Riedel

							sMsgBuffer.sHeader.bDataSize = ABP_UINT16_SIZEOF;
							break;
						case ABP_APPD_IA_BACNET_MNAME:
							// Bacnet-Vendorname
							strcpy(sMsgBuffer.abData, "RIEcon66E");
							sMsgBuffer.sHeader.bDataSize = strlen("RIEcon66E");
							break;
//						case ABP_APPD_IA_BACNET_FW:
						case ABP_APPD_IA_BACNET_APP_VERSION:

							for(uiIndex=0; uiIndex<3; uiIndex++) { 
								ctoa(&sMsgBuffer.abData[3*uiIndex], Kategorie_Vers[2-uiIndex]);
								sMsgBuffer.abData[3*uiIndex+2] = '.';
							}
							sMsgBuffer.sHeader.bDataSize = 8;
							break;
						case ABP_APPD_IA_BACNET_ADVANCED: // Advanced Modus betreiben
							sMsgBuffer.abData[0] = TRUE;
							sMsgBuffer.sHeader.bDataSize = 1;
							break;
						case ABP_APPD_IA_BACNET_DATE_TIME:  //Uhrzeit an das Bacnetmodul senden
							uiIndex = 2000+Jahr;
							*(UINT16*)&sMsgBuffer.abData[ 0 ] = uiIndex;
							sMsgBuffer.abData[2] = Mon;
							sMsgBuffer.abData[3] = Tag;
							sMsgBuffer.abData[4] = Std;
							sMsgBuffer.abData[5] = Min;
							sMsgBuffer.abData[6] = Sek;
							sMsgBuffer.sHeader.bDataSize = 7;
							break;
						default: // Unsupported attribute 

//							SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
						SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_CMD );

							break;
					} // switch( sMsgBuffer.sHeader.bCmdExt0 )	
					break; // case ABP_CMD_GET_ATTR

				default:
					SetMsgError( &sMsgBuffer, 1, ABP_ERR_INV_CMD_EXT_0 );
//					SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_CMD );
					break;
			}
			break;
		default: 
				//
				// Request to a unsupported Application Data Instance.
				// Respond with the appropriate error code.
				

			SetMsgError( &sMsgBuffer, 1, ABP_ERR_UNSUP_INST );
			break;

	} // end switch( instance ) 


} // end of BacnetHostObject() 

//------------------------------------------------------------------------------
// AppProcessData2AnyBus()
//------------------------------------------------------------------------------
void AppProcessData2AnyBus(void* pSendBuffer) {

	UINT16 uiAnzahlBytes = 0;
	UINT16 uiAnzahlSubBytes;
	UINT16 uiIndex;
	UINT16 uiSubIndex;
	float flDaten;
	anybusDataTableRow *pDataRow;

	if((pSendBuffer!=NULL)&&(uiWriteArraySize>0)) {
		for(uiIndex=0; uiIndex<uiNumber_of_Data;uiIndex++) {
			uiAnzahlBytes = NO_PROCESS_DATA;
			if((anybusDataTable[uiIndex].ucZugriff==ABP_NC_DESCR_GET_ACCESS)&&(anybusDataTable[uiIndex].siProcessIndex!=NO_PROCESS_DATA)) {
				// alle aus der WRITE_MAP_ONLY
				pDataRow = &anybusDataTable[uiIndex]; 
//				if(pDataRow->uiInstanceAnzahl>1) {
					uiAnzahlSubBytes = 0;
					for(uiSubIndex=0; uiSubIndex<pDataRow->uiInstanceAnzahl; uiSubIndex++) {
						uiAnzahlBytes = RiedelKonvert((char*)&flDaten,pDataRow->uiDAE,KONVRD,0);
						if(uiAnzahlBytes>0) {
							uiAnzahlSubBytes += AnybusKonvert(&flDaten,(((UINT8*)pSendBuffer)+(pDataRow->siProcessIndex+uiAnzahlSubBytes)),pDataRow,KONVRD, uiSubIndex);
						}
//						uiAnzahlSubBytes += CopyData2AnyBus(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex,/*appd_asInstance[uiIndex].pxValue,*/(((UINT8*)pSendBuffer)+(anybusDataTable[uiIndex].siProcessIndex+uiAnzahlSubBytes)));
					}
//					uiAnzahlBytes += uiAnzahlSubBytes;
/*				} else {
//					uiAnzahlBytes += CopyData2AnyBus(anybusDataTable[uiIndex].uiInstanceNumber,(((UINT8*)pSendBuffer)+anybusDataTable[uiIndex].siProcessIndex));
					uiAnzahlBytes = RiedelKonvert((char*)&flDaten,pDataRow->uiDAE,KONVRD,0);
					if(uiAnzahlBytes>0) {
						uiAnzahlSubBytes += AnybusKonvert(&flDaten,(((UINT8*)pSendBuffer)+(pDataRow->siProcessIndex)),pDataRow,KONVRD, uiSubIndex);
					}
				}*/
			}
		}
	}
}// end of AppProcessData2AnyBus() 

//*****************************************************************************
// AnyBusData2AppProcess
// wandelt die Processdaten wieder in den Hostdaten zurück
//*****************************************************************************
void AnyBusData2AppProcess(void* pReceiveBuffer) {
//*****************************************************************************
	UINT16 uiAnzahlBytes = 0;
	UINT16 uiAnzahlSubBytes;
	UINT16 uiIndex;
	UINT16 uiSubIndex;
	UINT8 ucAnzahlBytes;
	anybusDataTableRow *pDataRow;
	float		flData;

	if((pReceiveBuffer!=NULL)&&(uiReadArraySize>0)) {
		for(uiIndex=0; uiIndex<uiNumber_of_Data;uiIndex++) {
			if((anybusDataTable[uiIndex].ucZugriff==ABP_NC_DESCR_SET_ACCESS)&&(anybusDataTable[uiIndex].siProcessIndex!=NO_PROCESS_DATA)) {
				// alle aus der Read_MAP_ONLY
				pDataRow = &anybusDataTable[uiIndex];
				if(pDataRow->uiInstanceAnzahl>1) {
					// flData mit den alten Werten füllen, um Sie anschließend in AnybusKonvert zu überschreiben, dann wieder auf die Daten schaffen
					ucAnzahlBytes = RiedelKonvert((char*)&flData,pDataRow->uiDAE,KONVRD,0);
				}
				uiAnzahlSubBytes = 0;
				for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
					ucAnzahlBytes = AnybusKonvert(&flData,(((UINT8*)pReceiveBuffer)+(pDataRow->siProcessIndex+uiAnzahlSubBytes)),pDataRow,KONVWR, uiSubIndex);
					if(ucAnzahlBytes>0) {
						ucAnzahlBytes = RiedelKonvert((char*)&flData,pDataRow->uiDAE,KONVWR,0);
					}
					uiAnzahlSubBytes += ucAnzahlBytes;
				}
/*				if(anybusDataTable[uiIndex].uiInstanceAnzahl>1) {
					uiAnzahlSubBytes = 0;
					for(uiSubIndex=0; uiSubIndex<anybusDataTable[uiIndex].uiInstanceAnzahl; uiSubIndex++) {
						uiAnzahlSubBytes += CopyAnyBus2Data(anybusDataTable[uiIndex].uiInstanceNumber+uiSubIndex,(((UINT8*)pReceiveBuffer)+(anybusDataTable[uiIndex].siProcessIndex+uiAnzahlSubBytes)));
					}
					uiAnzahlBytes += uiAnzahlSubBytes;
				} else {
					uiAnzahlBytes += CopyAnyBus2Data(anybusDataTable[uiIndex].uiInstanceNumber,(((UINT8*)pReceiveBuffer)+anybusDataTable[uiIndex].siProcessIndex));
				}*/
//120623				uiAnzahlBytes += CopyAnyBus2Data(anybusDataTable[uiIndex]/*,appd_asInstance[uiIndex].pxValue*/,(((UINT8*)pReceiveBuffer)+anybusDataTable[uiIndex].siProcessIndex));
			}
		}
	}
}

#endif //( (IMPLEMENT_S3 & ANYBUS1_IMPL) == ANYBUS1_IMPL) 

///*****************************************************************************
//
// End of anybus.c
//
///*****************************************************************************

