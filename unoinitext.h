/* RAM-Bereich, der bei Reset nicht gel�scht wird	*/
#ifndef UNOINIT_H_INCLUDED
#define UNOINIT_H_INCLUDED


//#include "userdef.h"	
#include "ustruct.h"

extern UINT		ramex1;
extern float	fl_ta1mh;		// Aussentemperatur gemittelt �ber Stunden
extern float	fl_ta1m;		//		"							ged�mpft


// Absenk-Arbeitsbereiches f�r Heizkreise
extern absenkram			abs_ram[];		// Zeitabsenkung 		(Absenk.c)
extern absenkram			abs_bed[];		// Bedarfsabsenkung (Bedarf.c)

/*---------------- Z�hlerspeicherung pro Monat ------------------------*/
extern ULONG	monza_ramex;					// Sicherheitsabfrage
extern char		monza_aktiv;					// Prozedurablauf Aktiv (gestartet)
extern char		monza_aktion;					// Prozedurablauf aktuelle Aktion
extern char		retur_aktion;					// Prozedurablauf Merker
extern char		nullg_aktion;					// Stichtagsnullung Merker
extern char		monats_flag;
extern char		jahres_flag;
//extern UINT monz_spadr;						// Speicheradresse f�r Z�hlwerte im EEPROM
extern UINT		monv_spadr;						// Speicheradresse f�r Verbrauchswerte im EEPROM R36
extern UINT		monv_spadr_r37_1;			// Speicheradresse f�r Verbrauchswerte im EEPROM R37_1
extern UINT		monv_spadr_r37_2;			// Speicheradresse f�r Verbrauchswerte im EEPROM R37_2
extern UINT		monv_spadr_MBus;			// Speicheradresse f�r Verbrauchswerte im EEPROM MBus


extern char archivWriteError;
extern char archivParRep;

/***** ulsch *****/
extern UINT testPoint[10];				// ##### nur in Testphase #####
extern ULONG ltestPoint[6];

#endif // UNOINIT_H_INCLUDED
