/********************************************************************
 *																																	*
 *					genibus_const.c																					*
 *					Konstanten für verschiedene Genibusgeräte								*
 *					Version: 03.01.2013		anbu															*
 *																																	*
 ********************************************************************/

// 130514 ulsch: Wenn die Pumpen nicht mindestens alle 6 Sekunden adressiert werden, fallen sie in local mode (gilt z.B. für UPE unit_type 1 bis 4,
// bei anderen Typen beträgt diese Zeit 35 Sekunden). Da nicht klar ist, ob und wie diese Zeit geändert werden kann, wird in einem Zyklus jede
// parametrierte Pumpe nur einmal abgefragt/gestellt. Ein derartiger Zyklus dauert etwa 0,5 Sekunden pro Pumpe.
 
#include "struct.h"
#include "ustruct.h"
#include "uramext.h"
#include "konvform.h"
	
#if GENI == 1

	const char Genibus_Unit_Request[] = {GENI_unit_family, GENI_unit_type, GENI_unit_version};
	const char Genibus_Unit_Request_Size = sizeof(Genibus_Unit_Request);


	const GeniRequestTable Genibus_Request_Table[] =  
		{
		//	p_dest_value													p_dest_stat																uc_dest_type		uc_dest_comma		uc_value_type				uc_devicetable_idx		uc_replay_time[sek]
		//	Zieladresse														Zielstatus																ZielRieldtyp 		Kommastellen		Abfragewert-Type		Index des Geräts			Abfrageintervall
			{ &genibus_device_table[0].uc_function,	&Geni_Values[0].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &genibus_device_table[1].uc_function,	&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &genibus_device_table[2].uc_function,	&Geni_Values[2].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &genibus_device_table[3].uc_function,	&Geni_Values[3].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &genibus_device_table[4].uc_function,	&Geni_Values[4].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &genibus_device_table[5].uc_function,	&Geni_Values[5].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &genibus_device_table[6].uc_function,	&Geni_Values[6].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &genibus_device_table[7].uc_function,	&Geni_Values[7].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_7,				1,	},
#endif

			{ &BusPuData[0].uc_betriebsart,					&Geni_Values[0].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &BusPuData[1].uc_betriebsart,					&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &BusPuData[2].uc_betriebsart,					&Geni_Values[2].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &BusPuData[3].uc_betriebsart,					&Geni_Values[3].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &BusPuData[4].uc_betriebsart,					&Geni_Values[4].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &BusPuData[5].uc_betriebsart,					&Geni_Values[5].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &BusPuData[6].uc_betriebsart,					&Geni_Values[6].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &BusPuData[7].uc_betriebsart,					&Geni_Values[7].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_7,				1,	},
#endif

			{ &BusPuData[0].uc_regelart,						&Geni_Values[0].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &BusPuData[1].uc_regelart,						&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &BusPuData[2].uc_regelart,						&Geni_Values[2].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &BusPuData[3].uc_regelart,						&Geni_Values[3].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &BusPuData[4].uc_regelart,						&Geni_Values[4].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &BusPuData[5].uc_regelart,						&Geni_Values[5].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &BusPuData[6].uc_regelart,						&Geni_Values[6].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &BusPuData[7].uc_regelart,						&Geni_Values[7].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_7,				1,	},
#endif

			{ &BusPuData[0].ui_sollwert,						&Geni_Values[0].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &BusPuData[1].ui_sollwert,						&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &BusPuData[2].ui_sollwert,						&Geni_Values[2].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &BusPuData[3].ui_sollwert,						&Geni_Values[3].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &BusPuData[4].ui_sollwert,						&Geni_Values[4].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &BusPuData[5].ui_sollwert,						&Geni_Values[5].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &BusPuData[6].ui_sollwert,						&Geni_Values[6].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &BusPuData[7].ui_sollwert,						&Geni_Values[7].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_7,				1,	},
#endif
			                                      	
			{ &Geni_Values[0].si_medium_temp,				&Geni_Values[0].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].si_medium_temp,				&Geni_Values[1].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].si_medium_temp,				&Geni_Values[2].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].si_medium_temp,				&Geni_Values[3].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].si_medium_temp,				&Geni_Values[4].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].si_medium_temp,				&Geni_Values[5].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].si_medium_temp,				&Geni_Values[6].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].si_medium_temp,				&Geni_Values[7].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].ui_height,						&Geni_Values[0].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].ui_height,						&Geni_Values[1].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].ui_height,						&Geni_Values[2].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].ui_height,						&Geni_Values[3].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].ui_height,						&Geni_Values[4].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].ui_height,						&Geni_Values[5].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].ui_height,						&Geni_Values[6].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].ui_height,						&Geni_Values[7].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].ui_flow,							&Geni_Values[0].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].ui_flow,							&Geni_Values[1].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].ui_flow,							&Geni_Values[2].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].ui_flow,							&Geni_Values[3].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].ui_flow,							&Geni_Values[4].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].ui_flow,							&Geni_Values[5].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].ui_flow,							&Geni_Values[6].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].ui_flow,							&Geni_Values[7].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].ui_power,							&Geni_Values[0].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].ui_power,							&Geni_Values[1].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].ui_power,							&Geni_Values[2].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].ui_power,							&Geni_Values[3].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].ui_power,							&Geni_Values[4].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].ui_power,							&Geni_Values[5].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].ui_power,							&Geni_Values[6].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].ui_power,							&Geni_Values[7].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].ui_speed,							&Geni_Values[0].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].ui_speed,							&Geni_Values[1].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].ui_speed,							&Geni_Values[2].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].ui_speed,							&Geni_Values[3].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].ui_speed,							&Geni_Values[4].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].ui_speed,							&Geni_Values[5].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].ui_speed,							&Geni_Values[6].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].ui_speed,							&Geni_Values[7].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].ui_energy,						&Geni_Values[0].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].ui_energy,						&Geni_Values[1].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].ui_energy,						&Geni_Values[2].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].ui_energy,						&Geni_Values[3].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].ui_energy,						&Geni_Values[4].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].ui_energy,						&Geni_Values[5].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].ui_energy,						&Geni_Values[6].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].ui_energy,						&Geni_Values[7].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_7,				1,	},
#endif

      { &Geni_Values[0].ul_operating_hours,		&Geni_Values[0].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
      { &Geni_Values[1].ul_operating_hours,		&Geni_Values[1].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
      { &Geni_Values[2].ul_operating_hours,		&Geni_Values[2].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
      { &Geni_Values[3].ul_operating_hours,		&Geni_Values[3].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
      { &Geni_Values[4].ul_operating_hours,		&Geni_Values[4].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
      { &Geni_Values[5].ul_operating_hours,		&Geni_Values[5].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
      { &Geni_Values[6].ul_operating_hours,		&Geni_Values[6].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
      { &Geni_Values[7].ul_operating_hours,		&Geni_Values[7].uc_operating_hours_stat,	US_LONG, 				0,							GENI_OP_TIME,				GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
 			{ &Geni_Values[7].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_7,				1,	},
#endif

			{ &Geni_Values[0].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
			{ &Geni_Values[1].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_7,				1,	},
#endif

      { &Geni_Values[0].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_0,				1,	},
#if BUS_PU_MAX > 1
      { &Geni_Values[1].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_1,				1,	},
#endif
#if BUS_PU_MAX > 2
			{ &Geni_Values[2].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_2,				1,	},
#endif
#if BUS_PU_MAX > 3
			{ &Geni_Values[3].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_3,				1,	},
#endif
#if BUS_PU_MAX > 4
			{ &Geni_Values[4].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_4,				1,	},
#endif
#if BUS_PU_MAX > 5
			{ &Geni_Values[5].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_5,				1,	},
#endif
#if BUS_PU_MAX > 6
			{ &Geni_Values[6].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_6,				1,	},
#endif
#if BUS_PU_MAX > 7
			{ &Geni_Values[7].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_7,				1,	},
#endif

//-#if BUS_PU_MAX > 1
//-			{ &genibus_device_table[1].uc_function,	&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_FUNCTION,			GENI_DEVICE_1,				1,	},
//-			{ &BusPuData[1].uc_betriebsart,					&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_OPERATION,			GENI_DEVICE_1,				1,	},
//-			{ &BusPuData[1].uc_regelart,						&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_CONTROL,				GENI_DEVICE_1,				1,	},
//-			{ &BusPuData[1].ui_sollwert,						&Geni_Values[1].uc_uni_stat,							US_CHAR, 				0,							GENI_SETPOINT,			GENI_DEVICE_1,				1,	},
//-			                                      	
//-			{ &Geni_Values[1].si_medium_temp,				&Geni_Values[1].uc_medium_temp_stat,			S_INT, 					1,							GENI_WATER_TEMP,		GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].ui_height,						&Geni_Values[1].uc_height_stat,						US_INT, 				2,							GENI_HEIGHT,				GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].ui_flow,							&Geni_Values[1].uc_flow_stat,							US_INT, 				1,							GENI_FLOW,					GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].ui_power,							&Geni_Values[1].uc_power_stat,						US_INT, 				0,							GENI_POWER,					GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].ui_speed,							&Geni_Values[1].uc_speed_stat,						US_INT, 				0,							GENI_SPEED,					GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].ui_energy,						&Geni_Values[1].uc_energy_stat,						US_INT, 				0,							GENI_ENERGY,				GENI_DEVICE_1,				1,	},
//-      { &Geni_Values[1].ui_operating_hours,		&Geni_Values[1].uc_operating_hours_stat,	US_INT, 				0,							GENI_OP_TIME,				GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].uc_led_control,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_LED_STATUS,		GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].uc_act_mode1,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE1,			GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].uc_act_mode2,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE2,			GENI_DEVICE_1,				1,	},
//-			{ &Geni_Values[1].uc_act_mode3,					&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ACT_MODE3,			GENI_DEVICE_1,				1,	},
//-      { &Geni_Values[1].uc_alarm_code,				&uc_geni_dummy_stat,											US_CHAR, 				0,							GENI_ALARM_CODE,		GENI_DEVICE_1,				1,	},
//-#endif

		};	
	const char Genibus_Request_Table_Size = sizeof(Genibus_Request_Table) / GENI_REQUEST_TABLE_SIZE;	

// Aktuell verfügbare Abfragetypen: 
// GENI_OP_TIME				Grundeinheit: h
// GENI_MOTOR_TEMP		Grundeinheit: °C (K)
// GENI_WATER_TEMP		Grundeinheit: °C (K)
// GENI_HEIGHT				Grundeinheit: m
// GENI_FLOW					Grundeinheit: m³/h	
// GENI_POWER					Grundeinheit: W	
// GENI_SPEED					Grundeinheit: rpm	
// GENI_ENERGY				Grundeinheit: kWh
// GENI_RESISTANCE		Grundeinheit: Ohm		
// GENI_CURRENT_DC 		Grundeinheit: mA		
// GENI_CURRENT_MO 		Grundeinheit: mA		
// GENI_VOLTAGE   		Grundeinheit: V		
// GENI_FREQUENCY 		Grundeinheit: Hz
// GENI_PRESSURE			Grundeinheit: bar		
// GENI_TIME      		Grundeinheit: min		
// GENI_VOLUME				Grundeinheit: m³
// GENI_LED_STATUS		Bits
// GENI_ACT_MODE1			Bits
// GENI_ACT_MODE2			Bits
// GENI_ACT_MODE3			Bits
// GENI_ALARM_CODE		Bits

// GENI_FUNCTION			0=Deaktiviert / 1=Remote / 2=Local                                      
// GENI_OPERATION     0=Aus / 1=Ein / 2=Min / 3=Max                                           
// GENI_CONTROL	      0=Konstantdruck / 1=Proportionaldruck / 2=Konstantfrequenz / 3=Automatik
// GENI_SETPOINT	    Sollwertvorgabe                                                                      

// Status speichert mögliche Fehler bei der Konvertierung - OK = 0
// Kommastellen maximal 4!

// GENI_INDIV1 bis GENI_INDIV16 für individuelle Anfragen	

	const GeniIndivReqTable Genibus_Indiv_Req_Table[] = 
		{
		//	uc_class				uc_id_hi			uc_id_lo
			{	MEASURED_DATA,	GENI_qsd_alarm1,		0xFF,			},		// GENI_INDIV1
/*			{	MEASURED_DATA,	GENI_qsd_alarm2,			0xFF,			},		// GENI_INDIV2
			{	MEASURED_DATA,	GENI_stop_alarm1,		0xFF,			},		// GENI_INDIV3
			{	MEASURED_DATA,	GENI_stop_alarm2,			0xFF,			},		// GENI_INDIV4
			{	MEASURED_DATA,	GENI_surv_alarm1,		0xFF,			},		// GENI_INDIV5
			{	MEASURED_DATA,	GENI_ref_loc,		0xFF,			},		// GENI_INDIV1
			{	MEASURED_DATA,	GENI_p_max,			0xFF,			},		// GENI_INDIV2
			{	MEASURED_DATA,	GENI_q_kn1,		0xFF,			},		// GENI_INDIV3
			{	MEASURED_DATA,	GENI_q_max,			0xFF,			},		// GENI_INDIV4
			{	MEASURED_DATA,	GENI_h_max,		0xFF,			},		// GENI_INDIV5
			{	MEASURED_DATA,	GENI_torque,			0xFF,			},		// GENI_INDIV6
			{	MEASURED_DATA,	GENI_ind_alarm_bak,		0xFF,			},		// GENI_INDIV7
			{	MEASURED_DATA,	GENI_led_contr,		0xFF,			},		// GENI_INDIV8
			{	MEASURED_DATA,	GENI_ref_act,		0xFF,			},		// GENI_INDIV9
			{	MEASURED_DATA,	GENI_v_mo,		0xFF,			},		// GENI_INDIV10
			{	MEASURED_DATA,	GENI_phi,		0xFF,			},		// GENI_INDIV11
			{	MEASURED_DATA,	GENI_sys_fb,		0xFF,			},		// GENI_INDIV12
			{	MEASURED_DATA,	GENI_ref_att,		0xFF,			},		// GENI_INDIV13
			{	MEASURED_DATA,	GENI_sys_ref,		0xFF,			},		// GENI_INDIV14
			{	MEASURED_DATA,	GENI_start_alarm1,		0xFF,			},		// GENI_INDIV15
			{	MEASURED_DATA,	GENI_start_alarm2,		0xFF,			},		// GENI_INDIV16 */
		};
	const char Genibus_Indiv_Req_Table_Size = sizeof(Genibus_Indiv_Req_Table) / GENI_INDIV_REQ_TABLE_SIZE;	// Maximal 16 Einträge!


// Für die Umwandlung der empfangenen Infodaten
	const GeniUnitTable Genibus_Unit_Table[] = 
		{
			{20, 	 0.1},	  				// Temperature  °C						Grundeinheit: °C
			{21, 	 1},		  				// Temperature  °C        		      		
			{84, 	 0.01},	  				// Temperature  K         		
	 //	{57, 	 1},		  				// Temperature  °F    
			   	                                            		
			{91, 	 0.0001},			 	 	// Head/Distance  m 					Grundeinheit: m
			{83, 	 0.01},				 		// Head/Distance  m       		
			{24, 	 0.1},				 		// Head/Distance  m       		
			{25, 	 1},					 		// Head/Distance  m       		
			{26, 	 10},					 		// Head/Distance  m       		
			{56, 	 0.3046},			 		// Head/Distance  ft      		
			{59, 	 3.046},			 		// Head/Distance  ft						
		       	                                            		
			{95, 	 0.01},	  				// Flow  m3/h									Grundeinheit: m³/h
			{22, 	 0.1},	  				// Flow  m3/h             		
			{23, 	 1},		  				// Flow  m3/h             		
			{41, 	 5},		  				// Flow  m3/h             		
			{92, 	 10},		  				// Flow  m3/h             		
			{93, 	 100},	  				// Flow  m3/h             		
	//	{73, 	 0.5},	  				// Flow  l/h 		// ToDo Umrechnung Faktor in Grundeinheit			  		
	//	{69, 	 0.1},	  				// Flow  ml/h			      		
	//	{52, 	 1}, 		  				// Flow  l/s  
	//	{63, 	 0.1},	  				// Flow  l/s  
	//	{53, 	 1},		  				// Flow  m3/s 
	//	{54, 	 1},		  				// Flow  gpm  
	//	{58, 	 10},		  				// Flow  gpm  
	//	{82, 	 0.1},	  				// Flow  l/min
           	                                            		
			{7,  	 1},		  				// Power (active)  W 					Grundeinheit: W
			{8,  	 10},		  				// Power (active)  W 	    		
			{9,  	 100},	  				// Power (active)  W 	    		
			{44, 	 1000},		  			// Power (active)  kW 	  		
			{45, 	 10000},		  		// Power (active)  kW     		
		       	                                            		
			{98, 	 1},		  				// Rot. velocity  rpm  				Grundeinheit: rpm
			{18, 	 12},		  				// Rot. velocity  rpm     		
			{19, 	 100},	  				// Rot. velocity  rpm					
	 	       	                                            		
			{94, 	 0.001},		 			// Energy  Wh 								Grundeinheit: kWh
			{103,	 0.1},	  				// Energy  kWh            		
			{31, 	 1},		  				// Energy  kWh            		
			{85, 	 2},		  				// Energy  kWh            		
			{32, 	 10},		  				// Energy  kWh            		
			{33, 	 100},	  				// Energy  kWh            		
			{40, 	 512},	  				// Energy  kWh            		
			{46, 	 1000},		  			// Energy  MWh            		
			{47, 	 10000},		  		// Energy  MWh            		
			{48, 	 100000},	  			// Energy  MWh            		
	//	{87, 	 1},							// Energy  Ws	          		
		       	                                            		
			{6,  	 1,},							// Elec. resistance  Ohm			Grundeinheit: Ohm	
			{43, 	 10000,}, 				// Elec. resistance  kOhm 		
			{89, 	 100000,},				// Elec. resistance  kOhm 		
           	                	                          		
						 				        	                          		
			{15, 	 1,},							// Electrical current  mA 		Grundeinheit: mA
			{1,  	 100,},						// Electrical current  A  		
			{42, 	 200,}, 					// Electrical current  A  		
			{62, 	 500,},						// Electrical current  A  		
			{2,  	 5000,},					// Electrical current  A  		
	         	  			        	                          		
			{3,  	 0.1,},						// Voltage  V 								Grundeinheit: V
			{4,  	 1,},							// Voltage  V 
			{104,	 2,},		 					// Voltage  V 
			{5,  	 5,},							// Voltage  V 
			     	  			            	
			{105,	 0.01},	  				// Frequency  Hz 							Grundeinheit: Hz
			{11, 	 0.5},	  				// Frequency  Hz 
			{16, 	 1},		  				// Frequency  Hz 
			{38, 	 2},		  				// Frequency  Hz 
			{17, 	 2.5},	  				// Frequency  Hz 

			{51, 	 0.001},  				// Pressure  bar 							Grundeinheit: bar
			{27, 	 0.01},	  				// Pressure  bar 
			{28, 	 0.1},	  				// Pressure  bar 
			{29, 	 1},		  				// Pressure  bar 
			{61, 	 1},		  				// Pressure  kPa 
			{55, 	 1},		  				// Pressure  psi 
			{60, 	 10},		  				// Pressure  psi 

			{39, 	 1024},	  				// Time  h  									Grundeinheit: h     
	  	{81, 	 100},	  				// Time  h       
	  	{13, 	 2},	  					// Time  h            
	 		{35, 	 1},		  				// Time  h       
	 		{72, 	 1024},		  			// Time  min  								Grundeinheit: min 
	 		{36, 	 2},		  				// Time  min     
			{80, 	 1},		  				// Time  min     
	 		{14, 	 0.5},		  			// Time  s       
	 		{78, 	 0.1666},		  		// Time  s       
	 		{37, 	 0.01666},		  	// Time  s       
	 		{79, 	 0.001666},	  		// Time  s       
	 		
	  	{13, 	 2},		  				// Time  h										Grundeinheit: h (nur für Betriebsstunden)	 		
	 //	{108,  1}		  					// Time Unix time
	 

			{109,  0.0001},	  			// Volume  ltr								Grundeinheit: m³
			{99, 	 0.001},		  		// Volume  ltr
			{64, 	 0.1},	  				// Volume  m3
			{86, 	 1},		  				// Volume  m3
			{67, 	 256},	  				// Volume  m3
	 		{65, 	 1000},	  				// Volume  m3	 
	 //	{71, 	 1},		  		   	// Volume  nl
	 //	{106,  1},		  		   	// Volume  ml
	 //	{70, 	 0.1},	  		   	// Volume  ml
	 //	{88, 	 1},		  		   	// Volume  ml	

		 	{107,  0.01},	  				// Percentage  % 							Grundeinheit: %
		 	{12,	 0.1}, 	  				// Percentage  % 		
		 	{30,	 1},		  				// Percentage  % 			
		 	{76,	 10}, 	  				// Percentage  % 
	 	
	 //	{10, 	 1},		  				// Elec. capacitance  mF		// ToDo Erstmal nicht verwendet
                    		
	 //	{90, 	 1},		  				// Velocity  mm/s      
	 //	{34, 	 2},		  				// Ang. velocity  rad/s
	 //	{100,  1},		  				// Time  day          
                    		
	 //	{49, 	 1},		  				// Ang. degrees  o
                	  		  
	 //	{77, 	 0.01},	  				// Gain 
	 //	{96, 	 0.1},	  				// Gain 
	 //	{50, 	 1},		  				// Gain
	                	  		  
	 //	{66, 	 10},		  				// Energy pr vol.  kWh/m3
	 //	{74, 	 1},		  				// Energy pr vol.  Wh/m3
               	  		
	 //	{68, 	 1},		  				// Area  m2
	 //	{75, 	 1},		  				// Torque  Nm
	 //	{97, 	 0.1},	  				// Torque  Nm
	 //	{101,  0.01},	  				// Acceleration  m/s2
	 //	{102,  0.1},	  				// Mass density  kg/m3 	 
		};
	const char Genibus_Unit_Table_Size = sizeof( Genibus_Unit_Table ) / GENI_UNIT_TABLE_SIZE; 
	
	

// Für die Anzeige-Konvertierung der Werte in UserKonv.c
//	const char geni_units[][8] = 
//		{
//			{" mA    "},			// 0 = EINH_MA
//			{" A     "},			// 1 = EINH_A
//			{" W     "},			// 2 = EINH_W
//			{" kW    "},   		// 3 = EINH_KW
//			{" rpm   "},   		// 4 = EINH_RPM
//			{" m     "},   		// 4 = EINH_M
//			{" ERROR "},			// wenn ungültiger Eintrag
//		};
//	const char UNIT_MAX = sizeof(geni_units) / 8;	
//	
//	const char geni_types[][17] = 
//		{
//			{" DREHZAHL       "},			// 0 = TYPE_DREHZ
//			{" AKT STROMAUFN  "},			// 1 = TYPE_STROM
//			{" SPANNUNG       "},			// 2 = TYPE_SPANNUNNG
//			{" LEISTUNG       "},   	// 3 = TYPE_LEISTUNG
//			{" FOERDERHOEHE   "},   	// 4 = TYPE_HOEHE
//			{" UNKNOWN VALUE  "},			// wenn ungültiger Eintrag
//		};
//	const char TYPE_MAX = sizeof(geni_types) / 17;	

#endif // GENI == 1
