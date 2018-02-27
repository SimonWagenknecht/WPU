/********************************************************************
 *																																	*
 *					genibus_struct.h																				*
 *					Structuren für verschiedene Genibusgeräte								*
 *					Version: 25.04.2013		anbu															*
 *																																	*
 ********************************************************************/

#ifndef GENIBUS_STRUCT_H_INCLUDED
#define GENIBUS_STRUCT_H_INCLUDED

#include "genibus_defines.h"

	typedef struct gunt
		{
			unsigned char uc_index;
			float					fl_prefix;				
		} GeniUnitTable;
	#define GENI_UNIT_TABLE_SIZE sizeof(struct gunt)	

	
	
// Werte von Genibuspumpen
	typedef struct 
		{
			signed   int 	si_medium_temp;							// Medientemperatur
			unsigned char uc_medium_temp_stat;			
			unsigned int 	ui_height;									// Förderhöhe
			unsigned char uc_height_stat;
			unsigned int 	ui_flow;										// Flussmenge
			unsigned char uc_flow_stat;
			unsigned int 	ui_power;										// Leistung
			unsigned char uc_power_stat;
			unsigned int 	ui_speed;										// Umdrehung
			unsigned char uc_speed_stat;
			unsigned int 	ui_energy;									// Energy
			unsigned char uc_energy_stat;
			unsigned long	ul_operating_hours;					// Betriebsstunden
			unsigned char uc_operating_hours_stat;
			
			unsigned char uc_led_control;
			unsigned char uc_act_mode1;
			unsigned char uc_act_mode2;
			unsigned char uc_act_mode3;
			unsigned char uc_alarm_code;
			
			unsigned char uc_operation_reply;					// Zurückgelesene aktuelle Betriebsart
			unsigned char uc_control_reply;						// Zurückgelesene aktuelle Regelart
			
			unsigned char uc_communication_error;    	// Fehlerspeicher für jedes Gerät
			
			unsigned char uc_uni_stat;								// Universeller Status
		} GeniValues;

			

// Request Table für Anfragen an GeniBus Geräte
	typedef struct grt
		{
			void*					p_dest_value;					// Adresse des Ziel-Wert-Speichers
			void*					p_dest_stat;					// Adresse des Ziel-Status-Speichers
			unsigned char	uc_dest_type;					// Type des Zielwertes (RiedelType)
			unsigned char uc_dest_comma;				// Kommastellen des Zielwertes
			unsigned char uc_value_type;				// Auszulesener Wert
			unsigned char uc_devicetable_idx;		// Zeiger auf Pumpen "Device Table"
			unsigned char uc_replay_time;				// Wiederhol-Zeit
			

		} GeniRequestTable;
	#define GENI_REQUEST_TABLE_SIZE sizeof(struct grt)	

// Request Table für individuelle Anfragen zur Einbettung in die Request Table	
	typedef struct girt
		{
			unsigned char uc_class;
			unsigned char uc_id_hi;
			unsigned char uc_id_lo;
		} GeniIndivReqTable;
	#define GENI_INDIV_REQ_TABLE_SIZE sizeof(struct girt)		
		
// Device Table für Informationen über die angeschlossenen Pumpen
	typedef struct gdt
		{
			unsigned char uc_adr;								// Adresse der Pumpe
			unsigned char uc_group_adr;					// Gruppenadresse der Pumpe
			unsigned char uc_unit_family;				// ausgelesene "Unit Family" 
			unsigned char uc_unit_type;					// ausgelesener "Unit Type"
			unsigned char uc_unit_version;			// ausgelesene "Unit Version"
			unsigned char uc_profile_ident;			// Identifier für zu verwendendes Profil (ermittelt aus vorh 3)
			unsigned char uc_function;					// Funktion der Pumpe: 0=Deaktiviert / 1=Remote / 2=Local 
			unsigned char ucf_name[16];					// Feld für Name der Pumpe
		} GeniDeviceTable;
	#define GENI_DEVICE_TABLE_SIZE sizeof(struct gdt)		
	
	typedef struct gmr
		{	
			unsigned char uc_adrr;					
			unsigned char uc_geni_class;
			unsigned char uc_geni_op;
			unsigned char uc_id_hi;
			unsigned char uc_id_lo;
			unsigned char ucf_answer[16];
		}	GeniManualRequest;
		
#endif