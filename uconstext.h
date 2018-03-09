#ifndef UCONSTEXT_H_INCLUDED
#define UCONSTEXT_H_INCLUDED

/*    uconstext.h																									*/

/*    Header für externe Konstanten im Flash-Bereich des Users		*/

/* User-Compilierungszeit und Datum	*/
extern const char user_compile_time[];
extern const char user_compile_date[];


/* Typ und Version des gesamten Projektes									*/
extern const char Kategorie_Typ[];
extern const char Kategorie_Vers[];
extern const char Proj_Typ;

// Unterschriften in der 2.Zeile
extern const char Zsync[];
extern const char Leitstr[];
extern const char Ein_Ausg[];

extern const char KesBegr[];
extern const char KesAllg[];
extern const char KesPu[];
extern const char KesTyp[][16];
                            
// SiWa
extern const char Wpmod[];						                           
                            
extern const char Ntmod[];
extern const char Hkmod_fuss[];

extern const char Hkmod[][16];

extern const char Monatswerte[];

extern const char Wwmod[][16];

extern const char Stoermeld[];
extern const char ModBusMasterSlave[];
extern const char ModBusMaster[];	
extern const char ModBusSlave[];	

/***** ulsch : Heizgradtage *****/
extern const char HzGrdTg[];

extern const char Leistung[];

// Objekt- und Standortbezeichnungen
extern const char c_Objektbez [];		
extern const char c_Objektnum [];		
extern const char c_Strasse   [];		
extern const char c_Nummer    [];		
extern const char c_Ort       [];		
extern const char c_Plz       [];		

// Voreinstellung BUS-Betrieb
extern const char c_Service_Idf []; 
extern const char c_ModulAdr;
extern const char c_StationsAdr;

// Voreinstellung Schnittstelle und Funktionen
extern const char c_Mode_S1    ;
extern const char c_Mode_S2    ;
extern const char c_Mode_S3    ;
extern const UINT c_Baudrate_S1;
extern const UINT c_Baudrate_S2;
extern const UINT c_Baudrate_S3;
extern const char c_Funktion_S1;
extern const char c_Funktion_S2;
extern const char c_Funktion_S3;
extern const char c_Parity_S1; 
extern const char c_Parity_S2; 
extern const char c_Parity_S3; 
extern const char c_Stopbits_S1;
extern const char c_Stopbits_S2;
extern const char c_Stopbits_S3;
 
//---------- Bus-Pumpen ---------
extern const char * pu_operation_set[];
extern const char * pu_control_set[];
extern const char ClearDisplay[];
extern const char * pu_func_set[];

extern const char * wilo_servstat_set[];
extern const char * wilo_error_set[];
extern const char * wilo_errorstat_set[];
extern const char * wilo_pustat_set[];
extern const char * wilo_statdiag_set[];
//-------------------------------

/* Liste von Anlagen																			*/
// ulsch : Test
extern const char PROJTYPEN;
extern const Anlage Projekte[];

/* Liste von Ein-/Ausgangs-Belegungssätzen für R37-Module	*/
extern const char R37SETS;																		
extern const Anl37 R37_Beleg[];

/* Liste von Ein-/Ausgangs-Belegungssätzen für R38-Module	*/
extern const char R38SETS;																		
extern const Anl38 R38_Beleg[];

/* Liste von Ein-/Ausgangs-Belegungssätzen für R39-Module	*/
extern const char R39SETS;																		
extern const Anl39 R39_Beleg[];

/* Liste von Ein-/Ausgangs-Belegungssätzen für R33-Module	*/
extern const char R33SETS;																		
extern const Anl33 R33_Beleg[];

// MBUS	
/*  Liste von Standardwerten für verschiedene Zähler	*/
extern const char MB_PROFILE;
extern const MbusStandard  Mbus_Standparam[];

// Modbus
#if MODBUS_UNI > 0
	#include "Modbus/modbusconstext.h"
#endif

// Pumpenbus	
/*  Liste von Standardwerten für verschiedene Pumpen	*/
extern const PuBusStandard  PuBus_Standparam[];

/*  Liste von Standardwerten für verschiedene Heizkreise	*/
extern const char HK_PROFILE;
extern const HkStandard  Hk_Standparam[];

/* Liste von Absenktabellen																*/
extern const char ABS_PROFILE;
extern const absenktab abs_standard[][8];

/*  Liste von Standardwerten für verschiedene Kesselkreise	*/
extern const char KE_PROFILE;
extern const KeStandard  Ke_Standparam[];

/*  Liste von Standardwerten für verschiedene Warmwasserkreise	*/
extern const char WW_PROFILE ;
extern const WwStandard  Ww_Standparam[];

/*  Liste von Standardwerten für verschiedene Vorregelkreise	*/
extern const char NT_PROFILE ;
extern const NtStandard  Nt_Standparam[];

/*  Liste von Standardwerten für verschiedene Solarkreise	*/
extern const char SO_PROFILE ;
extern const SoStandard  So_Standparam[];

/*  Liste von Standardwerten für Benutzersteuerung mit UNI-Elementen	*/
extern const char UNI_PROFILE ;
extern const UniStandard  Uni_Standparam[];

//	Liste von Standardwerten für Skalierung von Analogausgängen
extern const aask R36_37_Standparam[][5];

//#####ulsch: skalierbare Analogeingänge
extern const sAnaInpPara AnaInpStandardPara[];

#if RM_POWER_ANZ
extern const sPowInpPara RmPowerStandardPara[];
#endif
#if AE_DRUCK_ANZ
extern const sAnaInpPara DruckStandardPara[];
#endif

// Dimensionsnamen für Zähler
extern const zdname ze_dim_namen[];

// Funktionsnamen für Schnittstellen
extern const char fu_namen[9][7];

/* Zählerdimensionierungen für Systemstart 	*/
extern const zaehldim ze_dimsys[];

/* Tabelle der Aufheizzeiten											*/
extern const char tabse[6][5];


/* Sonderzeichen LCD */
extern const char CarTab[64];


// diese allgemeinen Namen entfallen, wenn jedem Zähler individuell ein Name zugeordnet wird
//#####MBUS
extern const char ZlrWaerme[];														 
extern const char ZlrElektr[];														 
extern const char ZlrWarmwas[];														 
extern const char ZlrKaltwas[];														 
extern const char ZlrFrei[];														 


#endif	// UCONSTEXT_H_INCLUDED


