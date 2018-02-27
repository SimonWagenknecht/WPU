/*******************************************************************************
********************************************************************************
**
** File Name
** ---------
**
** anybus.h
**
********************************************************************************
********************************************************************************
**
** Description
** -----------
**
** A minimalistic Anybus-CC application example.
**
********************************************************************************
********************************************************************************
**
** Services List
** -------------
**
** Public Services:
**
**    AnybusMain()               - Anybus-CC interface main function
**
** Private Services:
**
**    ApplicationDataObject()    - Implements the Application Data Object
**    iBeTOi()                   - Transform a big endian integer to native.
**    iTOiBe()                   - Transform a native integer to big endian.
**    iLeTOi()                   - Transform a little endian integer to native.
**    iTOiLe()                   - Transform a native integer to little endian.
**
********************************************************************************
********************************************************************************
**                                                                            **
** User Information 2007 HMS Industrial Networks AB.                          **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It has been designed to be used and modified without permission from       **
** HMS Industrial Networks AB in any application using an Anybus CompactCom   **
** module.                                                                    **
** We, HMS Industrial Networks AB, reserve the right to modify our products   **
** in line with our policy of continuous product development. The information **
** in this program is subject to updates without notice and should not be     **
** considered as a commitment by HMS Industrial Networks AB.                  **
** HMS Industrial Networks AB assumes no responsibility for any errors that   **
** may appear in this program.                                                **
**                                                                            **
********************************************************************************
********************************************************************************
**
** Company: HMS Industrial Networks AB
**          Stationsgatan 37
**          SE-302 45  Halmstad
**          SWEDEN
**          Tel:     +46 (0)35 - 17 29 00
**          Fax:     +46 (0)35 - 17 29 09
**          e-mail:  info@hms.se
**
********************************************************************************
********************************************************************************
*/

/*******************************************************************************
********************************************************************************
**
** Change Log
** ----------
**
** Latest modification:
** --------------------
** $Modtime: 2011-09-23 $
**
** Version history:
**
**    Ver 1.00     2007-12-14  TiB   First release
**    Ver 1.10     2011-09-20  diro  First R66 release
**
********************************************************************************
********************************************************************************
*/

#ifndef ANYBUS_H
#define ANYBUS_H

#include "U_anybus.h"

//#define USE_DIPSWITCHES

#define LITTLE_ENDIAN
//#define BIG_ENDIAN

/*******************************************************************************
**
** Compiler dependent general purpose definitions
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
**
** FALSE
** TRUE
**
** These are the symbolic constants for true and false used in boolean
** data type comparisons.
**
**------------------------------------------------------------------------------
*/

#ifndef FALSE
   #define FALSE     0
#endif

#ifndef TRUE
   #define TRUE      ( !FALSE )
#endif


/*------------------------------------------------------------------------------
**
** BOOL8
**
** SINT8
** SINT16
** SINT32
**
** UINT8
** UINT16
** UINT32
**
** FLOAT32
**
** The standard boolean data type, 8-bit.
** The standard signed 8, 16, and 32 bit data types (respectively).
** The standard unsigned 8, 16, and 32 bit data types (respectively).
**
** PORTING ALERT!
** Make sure that the defined data types match your host system.
**
**------------------------------------------------------------------------------
*/

typedef unsigned char   BOOL8;
typedef signed char     SINT8;
//typedef signed short    SINT16;
typedef signed int	    SINT16;
typedef signed long     SINT32;
//typedef signed __int64  SINT64;
typedef signed long long  SINT64;

typedef unsigned char   UINT8;
//typedef unsigned short  UINT16;
typedef unsigned int  UINT16;
typedef unsigned long   UINT32;
//typedef unsigned __int64 UINT64;
typedef unsigned long long UINT64;
typedef float           FLOAT32;


/*------------------------------------------------------------------------------
**
** NULL
**
** Default value for invalid pointers.
**
**------------------------------------------------------------------------------
*/

#ifndef NULL
   #define NULL 0
#endif


/*------------------------------------------------------------------------------
**
** Define for struct
** Compiler independent symbols to pack structures for compilers that
** need an in-line directive.
**
** PORTING ALERT!
**
**------------------------------------------------------------------------------
*/

#define  start_packet_struct     struct
#define  end_packet_struct


/*------------------------------------------------------------------------------
**
** CPLUSPLUS
** EXTFUNC
**
** C/C++ Compatibility Macros (for C++ compatibility)
**
**------------------------------------------------------------------------------
*/

#ifdef __cplusplus
   #define CPLUSPLUS
#endif


#ifdef CPLUSPLUS
   #define EXTFUNC extern "C"
#else
   #define EXTFUNC extern
#endif


/*******************************************************************************
**
** Type Definitions
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
** State variable that keeps track of which initialization command to send next
** It is also used as a "source ID" to simplify the identification of responses
** from the Anybus-CC.
**------------------------------------------------------------------------------
*/

typedef enum CmdStateType
{
#ifdef USE_DIPSWITCHES
   MSG_NODE_ADR,
   MSG_BAUDRATE,
#endif
	MSG_DATA_FORMAT,
	MSG_NW_TYPE_STR,
	MSG_NW_TYPE,
	MSG_IPADRESS,
	MSG_SUBNETMASK,
	MSG_GATEWAY,
	MSG_SETUP_COMPLETE,
	MSG_DONE,
	MSG_MAP_IO
}
CmdStateType;


/*------------------------------------------------------------------------------
** Used for storing the data format of the fieldbus
** NET_UNKNOWN means that the Anybus-CC has not yet responded to our command to
** read the fieldbus data format.
**------------------------------------------------------------------------------
*/

typedef enum NetFormatType
{
   NET_LITTLEENDIAN,
   NET_BIGENDIAN,
   NET_UNKNOWN
}
NetFormatType;

// Netzwerktypen
#define MODBUS_IP		0x0093
#define BACNET_MSTP	0x9900
#define BACNET_IP		0x9A00

/*******************************************************************************
**
** Public Globals
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
**
** bSw1, bSw2
**
** bSw1 and bSW2 represent two DIP-switches (or similar) which are used to set
** the fieldbus node adress and baudrate of the Anybus-CC.
**
**------------------------------------------------------------------------------
*/

extern UINT8   bSw1;
extern UINT8   bSw2;


/*------------------------------------------------------------------------------
**
** bControlReg, bStatusReg
**
** Working copies of the status and control registers
**
**------------------------------------------------------------------------------
*/

extern UINT8   bControlReg;
extern UINT8   bStatusReg;


/*------------------------------------------------------------------------------
**
** eCmdState
**
** State variable that keeps track of which initialization command to send next
**
**------------------------------------------------------------------------------
*/

extern CmdStateType   eCmdState;


/*------------------------------------------------------------------------------
**
** bOutstandingCmds
**
** This variable keeps track of if there are any commands which we have not yet 
** received a response to.
**
**------------------------------------------------------------------------------
*/

extern UINT8 bOutstandingCmds;

// Zähler zum Überwachen, ob Anybus kommunikation noch steht
extern SINT16 iFreigabe;
/*------------------------------------------------------------------------------
**
** aiApplicationData
**
** The application data instances (ADIs for short)
** This is the data which will be exchanged on the fieldbus
** In this simple example we only have eight UINT16 parameters
**
**------------------------------------------------------------------------------
*/

//extern UINT16  aiApplicationData[];


/*------------------------------------------------------------------------------
**
** eNetFormat
**
** This variable is used to indicate the network data format.
** Little endian or big endian.
**
**------------------------------------------------------------------------------
*/

extern NetFormatType eNetFormat;
extern NetFormatType eHostFormat;

extern UINT16		uiNetworkType;

/*******************************************************************************
**
** Public Services Definitions
**
********************************************************************************
*/

//------------------------------------------------------------------------------
//
// AnybusMain()
//
// Handles communicaton with the Anybus-CC
// This function should be called cyclically
//
//------------------------------------------------------------------------------
//
// Inputs:
//          pbRxData    - Pointer to received data, NULL if a timeout has occured.
//
// Outputs:
//          Returns     - Pointer to data to send.
//          piTxSize    - Number of bytes to send.
//          piRxSize    - Number of bytes to receive
//
// Usage:
//          pTxData = AnybusMain( abRxData, &iTxSize, &iRxSize );
//
//------------------------------------------------------------------------------


EXTFUNC UINT8* AnybusMain( UINT8* pbRxData, UINT16* piTxSize, UINT16* piRxSize );

//------------------------------------------------------------------------------
//
// Abcc_Status()
//
// wertet den Status von Anybus-CC aus
//
//------------------------------------------------------------------------------
//
// Inputs:
//          iStatusReg    - Statusregister von Anybus-CC.
//
// Outputs:
//
// Usage:
//          Abcc_Status( bytStatusReg);
//
//------------------------------------------------------------------------------
EXTFUNC SINT8 Abcc_Status(UINT8 iStatusReg);
//------------------------------------------------------------------------------
//
// AnybusNeustart()
//
// setzt alle Kommunikationsvariablen auf Anfang zurück
//
//------------------------------------------------------------------------------
//
// Inputs:
//
// Outputs:
//
// Usage:
//          AnybusNeustart();
//
//------------------------------------------------------------------------------
EXTFUNC void AnybusNeustart(void);

/*******************************************************************************
**
** Private Services Definitions
**
********************************************************************************
*/

/*------------------------------------------------------------------------------
**
** ApplicationDataObject()
**
** Application Data Object implementation.
** This function handles commands to the Application Data Object.
**
**------------------------------------------------------------------------------
**
** Inputs:
**          None
**
** Outputs:
**          None
**
** Usage:
**          ApplicationDataObject();
**
**------------------------------------------------------------------------------
*/

EXTFUNC void ApplicationDataObject( void );
EXTFUNC void ApplicationObject(void);
EXTFUNC void BacnetHostObject(void);
EXTFUNC void ApplicationSendDataObject(int iInstance);
EXTFUNC void AppProcessData2AnyBus(void* pSendBuffer);
EXTFUNC void AnyBusData2AppProcess(void* pReceiveBuffer);
EXTFUNC UINT8 * GetGruppe(anybusDataTableRow *pDataRow);
EXTFUNC UINT8 * GetName(anybusDataTableRow *pDataRow);
EXTFUNC UINT8 * GetUnit(anybusDataTableRow *pDataRow);
EXTFUNC UINT8 GetAnyBusDataTyp(anybusDataTableRow *pDataRow);
EXTFUNC UINT8 GetAnyBusDataTypNumbers(anybusDataTableRow *pDataRow);
EXTFUNC UINT8 AnybusKonvert(float *pflData, UINT8 *ucBuffer, anybusDataTableRow *pDataRow, UINT8 ucRW, UINT8 ucSubIndex);
EXTFUNC UINT8 HostEndian2NetEndain(void* pDestBuffer, void* pSourceBuffer, UINT8 uiByteAnzahl);
/*------------------------------------------------------------------------------
**
** iBeTOi()
** iTOiBe()
** iLeTOi()
** iTOiLe()
**
** Transforms a big/little endian integer into a native format integer and
** a native format integer to big/little endian integer.
**
**------------------------------------------------------------------------------
**
** Inputs:
**    iBeFoo                  - a big endian integer to be transformed
**    iLeFoo                  - a little endian integer to be transformed
**    iFoo                    - a native format integer to be transformed
**
** Outputs:
**    iFoo                    - a native format integer
**    iBeFoo                  - a big endian integer
**    iLeFoo                  - a little endian integer
**
**
** Usage:
**    iFoo   = iBeTOi( iBeFoo );
**    iBeFoo = iTOiBe( iFoo );
**    iFoo   = iLeTOi( iLeFoo );
**    iLeFoo = iTOiLe( iFoo );
**
**------------------------------------------------------------------------------
*/

#ifdef BIG_ENDIAN
   #define iBeTOi( iBeFoo )    (UINT16)(iBeFoo)
   #define iTOiBe( iFoo )      (UINT16)(iFoo)
   #define iLeTOi( iLeFoo )    (UINT16)( ( (UINT16)(iLeFoo) >> 8 ) | ( (iLeFoo) << 8 ) )
   #define iTOiLe( iFoo )      (UINT16)( ( (UINT16)(iFoo) >> 8 ) | ( (iFoo) << 8 ) )
#else
   #define iBeTOi( iBeFoo )    (UINT16)( ( (UINT16)(iBeFoo) >> 8 ) | ( (iBeFoo) << 8 ) )
   #define iTOiBe( iFoo )      (UINT16)( ( (UINT16)(iFoo) >> 8 ) | ( (iFoo) << 8 ) )
   #define iLeTOi( iLeFoo )    (UINT16)(iLeFoo)
   #define iTOiLe( iFoo )      (UINT16)(iFoo)
#endif


/*------------------------------------------------------------------------------
**
** SetMsgError()
**
** Sets the E-bit and enters the submitted error code.
**
**------------------------------------------------------------------------------
**
** Inputs:
**    psMsg                - Pointer to the message command to convert.
**    bMsgDataSize         - The number of valid message data field bytes.
**    eErr                 - The requested error code (ABP_MsgErrorCodeType).
**
** Outputs:
**    None
**
** Usage:
**    SetMsgError( psMsg, bMsgDataSize, bErr );
**
**------------------------------------------------------------------------------
*/

#define SetMsgError( psMsg, bMsgDataSize, bErr )                           \
{                                                                              \
   (psMsg)->sHeader.bCmd      |=  ABP_MSG_HEADER_E_BIT;                        \
   (psMsg)->sHeader.bDataSize  =  (bMsgDataSize);                              \
   (psMsg)->abData[ 0 ]        =  (UINT8) (bErr);                              \
                                                                               \
} /* end of SetMsgError() */

#endif  /* inclusion lock */

// Definition für Element ist kein Processdatenelement
#define	NO_PROCESS_DATA	((signed int)-1)

/*******************************************************************************
**
** End of anybus.h
**
********************************************************************************
*/
