/* User RAM-Bereich, der bei Reset nicht gel�scht wird	*/

#include "struct.h"
#include "ustruct.h"

//###########################################################################################
#pragma SECTION	bss u_noinit
//###########################################################################################


UINT	ramex1;
float	fl_ta1mh;						// Aussentemperatur gemittelt �ber Stunden
float	fl_ta1m;						//		"							ged�mpft


// Absenk-Arbeitsbereiches f�r Heizkreise
absenkram			abs_ram[HKMAX];		// Zeitabsenkung 		(Absenk.c)
absenkram			abs_bed[HKMAX];		// Bedarfsabsenkung (Bedarf.c)

/*---------------- Z�hlerspeicherung pro Monat ------------------------*/
ULONG	monza_ramex;					// Sicherheitsabfrage
char	monza_aktiv;					// Prozedurablauf Aktiv (gestartet)
char	monza_aktion;					// Prozedurablauf aktuelle Aktion
char	retur_aktion;					// Prozedurablauf Merker
char	nullg_aktion;					// Stichtagsnullung Merker
char	monats_flag;
char	jahres_flag;
//UINT monz_spadr;					// Speicheradresse f�r Z�hlwerte im EEPROM
UINT	monv_spadr;						// Speicheradresse f�r Verbrauchswerte im EEPROM R36
UINT	monv_spadr_r37_1;			// Speicheradresse f�r Verbrauchswerte im EEPROM R37_1
UINT	monv_spadr_r37_2;			// Speicheradresse f�r Verbrauchswerte im EEPROM R37_2
UINT	monv_spadr_MBus;			// Speicheradresse f�r Verbrauchswerte im EEPROM MBus


char archivWriteError;
char archivParRep;

/***** ulsch *****/
UINT testPoint[10];				// ##### nur in Testphase #####
ULONG ltestPoint[6];

