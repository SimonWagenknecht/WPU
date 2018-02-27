// Task: AnalogEA
// Skalierung der 0-10V Eingänge, Grenzwertüberwachung

#include "struct.h"
#include "ustruct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "uconstext.h"
#include "unoinitext.h"


// --------------- Task Input -------------------------------
// Taskaufruf: 1 s

void AnalogEA ( void )
{

	static char anlauf = 1;
	static char i = 8;

	static char anlaufRm = 1;
	static char k = 8;

	static char anlaufDr = 1;
	static char l = 8;

	int mw;																			// aktueller Messwert
	char stat;																	// aktueller Messwert-Status
	int mwFilt;																	// gedämpfter Messwert
	float	fl_mwFilt = 0;												// gedämpfter Messwert
	int mwSkal;																	// skalierter Messwert

#if ANALOG_AE
	if ( ++i >= 8 )
		i = 0;
	if ( i < ANALOG_AE )
	{
	// kanalspez.Vorbereitung		
		mw = AE_UNI[i]->messw;
		stat = AE_UNI[i]->stat;		
		fl_mwFilt	= anaInp[i].fl_mwFilt;
		mwFilt = anaInp[i].mwFilt;
	
	// allg. Berechnung des gedämpften und des skalierten Messwerts													
		if ( anlauf )
		{
			mwFilt = mw;
			fl_mwFilt = (float)mwFilt / 10 ;		
		}	
		else
		{
			fl_mwFilt = g_filter ( fl_mwFilt, mw, 1, AnaInpPara[i].ZkFilt );	// Sprungantwort 66 % nach ZkFilt	
			mwFilt = (int)( fl_mwFilt * 10 );
		}	
	
		mwSkal = Gerade_YvonX ( mwFilt, 0, AnaInpPara[i].Skal0, 1000, AnaInpPara[i].Skal10 );
	
	// speichern der berechneten Daten		
		anaInp[i].mwFilt = mwFilt;
		anaInp[i].fl_mwFilt = fl_mwFilt;
		anaInp[i].mwSkal.messw = mwSkal;
		anaInp[i].mwSkal.stat = stat;
		
		if ( anlauf && i == ( ANALOG_AE - 1 ) )
			anlauf = 0;																// Anlauf ist füe alle Eingänge erledigt
	
	// Grenzwertüberwachung
		if ( anaInp[i].mwSkal.stat == 0 )
		{
			if ( AnaInpPara[i].UGW != 0 )
			{
				if ( anaInp[i].mwSkal.messw <= AnaInpPara[i].UGW )
					anaInp[i].ugwSM = TRUE;
				else if ( anaInp[i].mwSkal.messw >= AnaInpPara[i].UGW + AnaInpPara[i].Hyst )
					anaInp[i].ugwSM = FALSE;	
			}
			else
			{																					// kein Grenzwert
				anaInp[i].ugwSM = FALSE;					
	//		anaInp[i].ugwSM_sp = FALSE;	
			}				
		}
		else
			anaInp[i].ugwSM = FALSE;
	
		if ( anaInp[i].mwSkal.stat == 0 )
		{
			if ( AnaInpPara[i].OGW != 0 )
			{
				if ( anaInp[i].mwSkal.messw >= AnaInpPara[i].OGW )
					anaInp[i].ogwSM = TRUE;
				else if ( anaInp[i].mwSkal.messw <= AnaInpPara[i].OGW - AnaInpPara[i].Hyst )
					anaInp[i].ogwSM = FALSE;	
			}
			else
			{																					// kein Grenzwert
				anaInp[i].ogwSM = FALSE;					
	//		anaInp[i].ogwSM_sp = FALSE;	
			}					
		}
		else
			anaInp[i].ogwSM = FALSE;
	}	
#endif

#if RM_POWER_ANZ
	if ( ++k >= 8 )
		k = 0;
	if ( k < RM_POWER_ANZ )
	{
	// kanalspez.Vorbereitung		
		mw = RM_POWER[k]->messw;
		stat = RM_POWER[k]->stat;		
		fl_mwFilt	= rmPower[k].fl_mwFilt;
		mwFilt = rmPower[k].mwFilt;
	
	// allg. Berechnung des gedämpften und des skalierten Messwerts													
		if ( anlaufRm )
		{
			mwFilt = mw;
			fl_mwFilt = (float)mwFilt / 10 ;		
		}	
		else
		{
			fl_mwFilt = g_filter ( fl_mwFilt, mw, 1, RmPowerPara[k].ZkFilt );	// Sprungantwort 66 % nach ZkFilt	
			mwFilt = (int)( fl_mwFilt * 10 );
		}	
	
		if ( mwFilt < RmPowerPara[k].Skal0*10 )
			mwSkal = 0;
		else if ( mwFilt > RmPowerPara[k].Skal10*10 )
			mwSkal = 1000;
		else 				
			mwSkal = Gerade_YvonX ( mwFilt, RmPowerPara[k].Skal0*10, 0, 1000, RmPowerPara[k].Skal10 );
	
	// speichern der berechneten Daten		
		rmPower[k].mwFilt = mwFilt;
		rmPower[k].fl_mwFilt = fl_mwFilt;
		rmPower[k].mwSkal.messw = mwSkal;
		rmPower[k].mwSkal.stat = stat;
		
		if ( anlaufRm && k == ( RM_POWER_ANZ - 1 ) )
			anlaufRm = 0;																// Anlauf ist füe alle Eingänge erledigt
	}
#endif

#if AE_DRUCK_ANZ
	if ( ++l >= 8 )
		l = 0;
	if ( l < AE_DRUCK_ANZ )
	{
	// kanalspez.Vorbereitung		
		mw = AE_DRUCK[l]->messw;
		stat = AE_DRUCK[l]->stat;		
		fl_mwFilt	= druck[l].fl_mwFilt;
		mwFilt = druck[l].mwFilt;
	
	// allg. Berechnung des gedämpften und des skalierten Messwerts													
		if ( anlaufDr )
		{
			mwFilt = mw;
			fl_mwFilt = (float)mwFilt / 10 ;		
		}	
		else
		{
			fl_mwFilt = g_filter ( fl_mwFilt, mw, 1, DruckPara[l].ZkFilt );	// Sprungantwort 66 % nach ZkFilt	
			mwFilt = (int)( fl_mwFilt * 10 );
		}	
	
	
		if ( mwFilt < DruckPara[l].Skal0*10 )
			mwSkal = 0;
		else if ( mwFilt > 1000 )
			mwSkal = DruckPara[l].Skal10;
		else
			mwSkal = Gerade_YvonX ( mwFilt, DruckPara[l].Skal0*10, 0, 1000, DruckPara[l].Skal10 );
	
	// speichern der berechneten Daten		
		druck[l].mwFilt = mwFilt;
		druck[l].fl_mwFilt = fl_mwFilt;
		druck[l].mwSkal.messw = mwSkal;
		druck[l].mwSkal.stat = stat;
		
		if ( anlaufDr && l == ( AE_DRUCK_ANZ - 1 ) )
			anlaufDr = 0;																// Anlauf ist füe alle Eingänge erledigt
	
	// Grenzwertüberwachung
		if ( druck[l].mwSkal.stat == 0 )
		{
			if ( DruckPara[l].UGW != 0 )
			{
				if ( druck[l].mwSkal.messw <= DruckPara[l].UGW )
					druck[l].ugwSM = TRUE;
				else if ( druck[l].mwSkal.messw >= DruckPara[l].UGW + DruckPara[l].Hyst )
					druck[l].ugwSM = FALSE;	
			}
			else
				druck[l].ugwSM = FALSE;					
		}
		else
			druck[l].ugwSM = FALSE;
	
		if ( druck[l].mwSkal.stat == 0 )
		{
			if ( DruckPara[l].OGW != 0 )
			{
				if ( druck[l].mwSkal.messw >= DruckPara[l].OGW )
					druck[l].ogwSM = TRUE;
				else if ( druck[l].mwSkal.messw <= DruckPara[l].OGW - DruckPara[l].Hyst )
					druck[l].ogwSM = FALSE;	
			}
			else
				druck[l].ogwSM = FALSE;					
		}
		else
			druck[l].ogwSM = FALSE;
	}	
#endif

}				

