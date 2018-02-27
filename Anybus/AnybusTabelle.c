#include "defins.h"
#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "constext.h"
#include "uconstext.h"
#include "anybus.h"
#include "U_anybus.h"

//----------------------------------------------------------------
//	AnybusTabelle
//----------------------------------------------------------------
// Aufbau:
// 1.	Parameter aus der parli.h
// 2. Exp-Nummer  (noch nicht validiert)
// 3.	Zugriff auf diesen Parameter:
//		ANYBUS_READ_ACCESS 											= der Parameter ist nur lesbar (für alle * und + Parameter zwingend vorgeschrieben)
//		ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS	= der Parameter ist lesbar und kann überschrieben werden 
//		ANYBUS_WRITE_ACCESS											= der Parameter wird nur über Anybus beschrieben (ähnlich Komtab-Copy) 

#if KOMTAB_ALL == 0									// nicht die ganze Komtab, sondern nur diese Tabelle. Einstellung in projdef.h
const anybusTableRow	anybusTable[] =
{	// Value aus Parli, 				, Exp			ZUGRIFF (* und + Parameter bekommen immer ANYBUS_READ_ACCESS alle anderen sind wählbar)
	{P&bst											,	0,			ANYBUS_READ_ACCESS},
	{P&abs_tab[HK1][0].begin		, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
/*{P&TRP[ANL]									, 0,			ANYBUS_READ_ACCESS},
	{P&ta1m											, 0,			ANYBUS_READ_ACCESS},
	{P&Ta1mhzk									, 0,			ANYBUS_WRITE_ACCESS},
	{P&Tae											, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&Taa											, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&sowi											, 0,			ANYBUS_READ_ACCESS},
	{P&HardHand									, 0,			ANYBUS_READ_ACCESS},
	{P&TVS[HK1]									, 0,			ANYBUS_READ_ACCESS},
	{P&TRS[HK1]									, 0,			ANYBUS_READ_ACCESS},
	{P&TI[HK1]									, 0,			ANYBUS_READ_ACCESS},
	{P&PU[HK1]									,	0,			ANYBUS_READ_ACCESS},
	{P&hks[HK1].Tvma						, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&hks[HK1].Tvmi						, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&RVENT[HK1]								, 0,			ANYBUS_READ_ACCESS},
	{P&TVS[HK2]									, 0,			ANYBUS_READ_ACCESS},
	{P&TRS[HK2]									, 0,			ANYBUS_READ_ACCESS},
	{P&PU[HK2]									,	0,			ANYBUS_READ_ACCESS},
	{P&hks[HK2].Tvma						, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&hks[HK2].Tvmi						, 0,			ANYBUS_READ_ACCESS|ANYBUS_WRITE_ACCESS},
	{P&RVENT[HK2]								, 0,			ANYBUS_READ_ACCESS},
	{P&Resetcnt									, 0,			ANYBUS_WRITE_ACCESS},
	{P&TVP[ANL]									, 0,			ANYBUS_READ_ACCESS},
	{P&DA_UNI[U3]								, 0,  		ANYBUS_WRITE_ACCESS},*/
};
UINT16 uiNumber_of_Data = (sizeof(anybusTable)/sizeof(anybusTableRow));
anybusDataTableRow anybusDataTable[(sizeof(anybusTable)/sizeof(anybusTableRow))];

#else
// die ganze Komtab
UINT16 uiNumber_of_Data = 1;
anybusDataTableRow anybusDataTable[512];
#endif
