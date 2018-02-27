/********************************************************************
 *																																	*
 *					genibus_defines.h																				*
 *					Definitionen für verschiedene Genibusgeräte							*
 *					Version: 05.11.2013		anbu															*
 *																																	*
 ********************************************************************/

// 131105: Neue Definitionen für "Extended"

// Identifikation der Pumpe
	// Unit_Family
		#define UPE											1
		#define MGE											2
		                          			
	// Unit Type                			
		#define UPE_CS3000							1					// Weitere Infos im Datenblatt UPE Seite 3
		#define UPE_CS2000							2
		#define UPE_CS1000							3
		#define UPE_CS100								4
		#define UPE_CS3000MRK3					5
		#define UPE_CS2000MRK3					6
		#define UPE_CS2600MAGNA					7
		#define UPE_CS300								8
	                            			
		#define MGE_HM3000MRK1					1 				// 3 phase MGE motors version HM3000Mrk I and MMGE motors				// Datenblatt MGE
		#define MGE_HM2000MRK2					2 				// single phase MGE motors (HM2000Mrk II)
		#define MGE_HM3000MRK2					3 				// 3 phase MGE motors version HM3000Mrk II
		#define MGE_HMLARGE							4 				// 3 phase MGE motors version HMLarge
		#define MGE_CUE									5					// CUE																													// Datenblatt Large MGE & CUE
		#define MGE_MODEL_HI						7					// MGE model H/I																								// Datenblatt RedWolf
		#define MGE_MAGNA3							10				// Magna3
		                          			
	// Unit Version             			
		#define MGE_NAKED								1					// Naked MGE motor (Large MGE only)															// Datenblatt MGE und Large MGE & CUE
		#define MGE_CRE									2					// CRE
		#define MGE_CRE_SENSOR					3					// CRE with sensor (Large MGE only)
		#define MGE_TPE1000							4					// TPE 1000 (and NB/NK)
		#define MGE_TPE2000							5					// TPE 2000 (Large MGE only)
		                          			
		#define MAGNA3_1000LME					1 				// Single stage Series 1000 (LME)																// Datenblatt RedWolf
		#define MAGNA3_2000LME					2 				// Single stage Series. 2000 (LME)
		#define MAGNA3_1000MGE					3					// Single stage MAGNA3 Series 1000 (MGE motor with MAGNA3 hydraulic)
		#define MAGNA3_S2000						4					// Single stage Redwolf S2000 (MGE motor with MAGNA3 hydraulic)
		#define MAGNA3_CRE							5					// Multi stage with sensor (CRE)
		#define MAGNA3_CRE_SENSOR				6					// Multi stage without sensor (CRE)
		#define MAGNA3_BOOSTER					7					// Home booster
		#define MAGNA3_NAKED						8					// naked MGE
                              			
	// Module Type              			
		#define MGE_75A									5					// 75 A Econo module (11 kW terminal box) (Large MGE only)
		#define MGE_100A								6					// 100 A Econo module (22 kW terminal box) (Large MGE only)	
		                          			
	// Profil Identifier        			
		#define PROFIL1									1					// Für alte Version (kleine Pumpen)
		#define PROFIL2									2					// Unit Family = 2 (MGE) und Unit Type = 3 (HM3000Mrk II)
		#define PROFIL3									3					// "große" Pumpen
		#define PROFIL4									4					// Bisher unbenutzt


// Geräte (Max 12 Geräte)
	#define GENI_DEVICE_0							0
	#define GENI_DEVICE_1							1
	#define GENI_DEVICE_2							2
	#define GENI_DEVICE_3							3
	#define GENI_DEVICE_4							4
	#define GENI_DEVICE_5							5
	#define GENI_DEVICE_6							6
	#define GENI_DEVICE_7							7
	#define GENI_DEVICE_8							8
	#define GENI_DEVICE_9							9
	#define GENI_DEVICE_10						10
	#define GENI_DEVICE_11						11
	

// Universelle Abfragenamen
	// Measured Data
		#define GENI_OP_TIME						1					// Betriebsstunden		
		#define GENI_MOTOR_TEMP					2					// Motortemperatur					
		#define GENI_HEIGHT 						3					// Förderhöhe	
		#define GENI_FLOW								4					// Durchflussmenge
		#define GENI_POWER							5					// Leistung	
		#define GENI_SPEED							6					// Motorspeed	
		#define GENI_ENERGY 						7					// Energyaufnahme	
		#define GENI_RESISTANCE					8					// Widerstand
		#define GENI_CURRENT_DC					9					// Strom DC Link
		#define GENI_VOLTAGE   					10				// Akt. Spannung		
		#define GENI_FREQUENCY 					11				// Frequenz
		#define GENI_PRESSURE						12				// Druck
		#define GENI_TIME      					13				// Zeit
		#define GENI_VOLUME							14				// Volumen
		#define GENI_CURRENT_MO					15				// Strom Motor
		#define GENI_LED_STATUS					16				// Status der grünen und roten LED
		#define GENI_ACT_MODE1					17				// Aktueller Mode 1
		#define GENI_ACT_MODE2					18				// Aktueller Mode 1
		#define GENI_ACT_MODE3					19				// Aktueller Mode 1
		#define GENI_ALARM_CODE					20				// Alarm Code
		#define GENI_WATER_TEMP					21				// Wasser (Medium-) Temperatur
	// Einstellungen              		
		#define GENI_FUNCTION						50				// Funktion: 		0=Deaktiviert / 1=Remote / 2=Local                                      
		#define GENI_OPERATION					51				// Betriebsart: 0=Aus / 1=Ein / 2=Min / 3=Max                                           
		#define GENI_CONTROL						52				// Regelart:		0=Konstantdruck / 1=Proportionaldruck / 2=Konstantfrequenz / 3=Automatik
		#define GENI_SETPOINT						53				// Sollwertvorgabe     
			// -> Defines zu Übergabewerten weiter unten!                                                                 
	// Individuelle Werte lesen	
		#define GENI_INDIV1							0x80
		#define GENI_INDIV2							0x81
		#define GENI_INDIV3							0x82
		#define GENI_INDIV4							0x83
		#define GENI_INDIV5							0x84
		#define GENI_INDIV6							0x85
		#define GENI_INDIV7							0x86
		#define GENI_INDIV8							0x87
		#define GENI_INDIV9							0x88
		#define GENI_INDIV10						0x89
		#define GENI_INDIV11						0x8A
		#define GENI_INDIV12						0x8B
		#define GENI_INDIV13						0x8C
		#define GENI_INDIV14						0x8D
		#define GENI_INDIV15						0x8E
		#define GENI_INDIV16						0x8F
	
	// Für func "Genibus_Set_Control_Value"
		#define GENI_START_PUMP    			0x41          
		#define GENI_STOP_PUMP     			0x42         
		#define GENI_MIN_CMD       			0x43            
		#define GENI_MAX_CMD       			0x44            
		#define GENI_CONSTPRESS    			0x45            
		#define GENI_PROPRESS      			0x46            
		#define GENI_CONSTFREQ     			0x47            
		#define GENI_AUTOMATIC     			0x48
		#define GENI_REMOTE							0x49
	
  

// Steuerkommandos
	// Classes
		#define MEASURED_DATA						2
		#define COMMANDS								3
		#define CONFIG_PARAM						4
		#define REF_VALUES							5
		#define ASCII_STR								7
		
	// IDs zu Class "Measured Data"
		#define GENI_t_2hour_hi    			24			// Operating hours counter high/low order byte.
		#define GENI_t_2hour_lo    			25
		#define GENI_i_dc          			26			// Frequency converter DC link current
		#define GENI_v_dc          			27			// Frequency converter DC link voltage
		#define GENI_t_m           			29			// Temperature in freq. converter power electronics
		#define GENI_i_mo          			30			// Motor current
		#define GENI_f_act         			32			// Actual control signal (frequency) applied to motor, Scaling: Hz 
		#define GENI_p             			34                                                
		#define GENI_speed         			35
		#define GENI_speed_lo      			36
		#define GENI_speed_lo_xtra 			55
		#define GENI_h             			37
		#define GENI_q             			39
		#define GENI_ref_loc       			40
		#define GENI_p_max         			41
		#define GENI_q_kn1         			42
		#define GENI_q_max         			43
		#define GENI_h_max         			44
		#define GENI_torque        			45
		#define GENI_ind_alarm_bak 			46
		#define GENI_led_contr     			47
		#define GENI_ref_act       			48
		#define GENI_v_mo          			51
		#define GENI_phi           			57
		#define GENI_t_w								58
		#define GENI_sys_fb        			60
		#define GENI_ref_att       			61
		#define GENI_sys_ref       			62
		#define GENI_start_alarm1  			64
		#define GENI_start_alarm2  			65
		#define GENI_qsd_alarm1    			66
		#define GENI_qsd_alarm2    			67
		#define GENI_stop_alarm1   			68
		#define GENI_stop_alarm2   			69
		#define GENI_surv_alarm1   			70                            		
		#define GENI_unit_family				148
		#define GENI_unit_type					149
		#define GENI_unit_version				150
		#define GENI_surv_alarm2				71 
		#define GENI_ind_alarm 					72 
		#define GENI_start_alarm1_bak  	73 
		#define GENI_start_alarm2_bak  	74 
		#define GENI_qsd_alarm1_bak  		75 
		#define GENI_qsd_alarm2_bak  		76 
		#define GENI_stop_alarm1_bak  	77 
		#define GENI_stop_alarm2_bak 		78 
		#define GENI_surv_alarm1_bak 		79 
		#define GENI_surv_alarm2_bak  	80
		#define GENI_act_mode1  				81 
		#define GENI_act_mode2  				82 
		#define GENI_act_mode3  				83 
		#define GENI_loc_setup1  				85 
		#define GENI_rem_setup1  				87 
		#define GENI_extern_inputs 			89 
		#define GENI_contr_source  			90 
		#define GENI_stop_alarm3 				93 
		#define GENI_stop_alarm3_bak  	96
		#define GENI_curve_no_ref  			97
		#define GENI_contr_ref 					147		
		#define GENI_energy_hi					152
		#define GENI_energy_lo					153
		#define GENI_alarm_code					158
		
	// IDs zu "Commands"	
		#define GENI_reset							1
		#define GENI_stop								5
		#define GENI_start							6
		#define GENI_remote							7
		#define GENI_local							8
		#define GENI_const_freq					22
		#define GENI_prop_press					23
		#define GENI_const_press				24
		#define GENI_min								25
		#define GENI_max								26
		#define GENI_automatic					52
		                            		
	// IDs zu "Config Parameters" 		
		#define GENI_unit_addr					46
		#define GENI_group_addr					47
	                              		
	// IDs zu "Reference Values"  		
		#define GENI_ref_rem						1	
		                            		
	// IDs zu "Ascii String"      		
		#define GENI_product_name				1


	// InfoHeader Bitzuordnung (nach GeniSpec Figure4 Seite 5)
		#define HEADER_START 						0b11000000
		#define HEADER_VI								0b00100000
		#define HEADER_BO								0b00010000
		#define HEADER_SIF							0b00000011
		#define HEADER_SZ								0b10000000 		// in UnitByte Bit 7
		#define UNIT										0x7F
		
		#define SCALE_NOT_AVAILABLE			0x00					// SIF - Scale Information Format
		#define BIT_WISE_INTERPRETED		0x01					
		#define SCALED_816BIT_VALUE			0x02
		#define EXTENDED_PRECISION			0x03
		
				
		
// Für die Anzeige-Konvertierung der Werte in UserKonv.c
	// Einheiten
		#define EINH_MA									0
		#define EINH_A									1
		#define EINH_W									2
		#define EINH_KW									3
		#define EINH_RPM								4
		#define EINH_M									5
	
	// Anzeigenamen
		#define TYPE_DREHZ							0
		#define TYPE_STROM							1
		#define TYPE_SPANNUNNG					2
		#define TYPE_LEISTUNG						3
		#define TYPE_HOEHE							4            
		
		
	// Sonstige Definitionen
		#define MAX_TELEGRAM_LENGHT			70		// In Bytes	
		                              	
		#define GENI_DEFAULT_ADR				231
		#define GENI_BROADCAST_ADR			255
  
  
// Datenübertragung 



		
	// RX Fehlerbehandlung (Ausstiegspunkte)
		#define RX_NO_ERROR							0x00
		#define RX_TIMEOUT_ERROR				0x01
		#define RX_LENGHT_ERROR					0x02
		#define RX_CHECKSUM_ERROR				0x03
		#define RX_SOURCE_ERROR					0x04
		#define RX_DEST_ERROR						0x05
		#define RX_APDU1_ERROR					0x06
		#define RX_DEVIDX_ERROR					0x07
		#define RX_START_ERROR					0x08
		#define RX_APDU2_ERROR					0x09
		#define RX_APDU3_ERROR					0x0A
  	#define RX_INFO_APDU_ERROR			0x0B
  	#define RX_NO_INFO_ERROR				0x0C		
  	#define RX_GET_APDU_ERROR				0x0D
  	#define RX_EXT_PRE_ERROR				0x0E
  	#define RX_CLASS_ERROR					0x0F
  	#define RX_UNIT_ERROR						0x10
  	#define RX_CHOICE_ERROR					0x11
  	                              	
  	#define RX_SIO_ERROR						0x80
                                  	
		#define SET_DEV_IDX_ERROR				0x41
		#define SET_WRONG_FUNC_ERROR		0x42  	
  	                              	
  // Status-Fehlermeldungen       	
 		#define STAT_BIGGER_CHAR				0x01
  	#define STAT_BIGGER_INT					0x02	
		  
	// Ablaufstatus
		#define RESTART									0x00
		#define CHECK_WORK							0x01
		#define GENI_TX									0x02
		#define GENI_RX									0x03
		
	// Timeout Values	
		#define GENIBUS_RX_TIMEOUT			8						
		#define GENIBUS_RX_BUFFER_SIZE	128
		#define GENI_DEVICE_MAX					BUS_PU_MAX
	
	// Ablaufstatus beim Empfang neuer Telegramme	
		#define TELEGRAM_READY					0			
		#define RECEIVING_TELEGRAM 			1
		#define CHECK_NEW_TELEGRAM 			5							// nach 5 - 1 Taktzeiten der U_Genibus() wird Telegramm frühestens ausgewertet
  
 	// Sonderfunktion Bedeutungen
 		#define NO_SPECIAL_FKT					0
 		#define GET_UNIT_INFO						1 
 		#define GET_ADRESS							2
 		#define SEND_VARIABLE_REQUEST		3
 		#define RESET_PUMP							4
  
  // Return TX
  	#define NO_TRANSMISSION					0
  	#define TRANSMIT_TELEGR					1
  	
// Byte Names
		#define STARTBYTE_DATA_REQUEST	0x27  	
  	#define STARTBYTE_DATA_MESSAGE	0x26		// Bisher nicht unterstützt  
  	#define STARTBYTE_DATA_REPLY		0x24  
  	
// Byte Positions
		#define START_BYTE							0
		#define LENGHT_BYTE							1 
		#define DEST_ADR_BYTE						2
		#define SOURCE_ADR_BYTE					3
		#define PDU_HEADER1							4
		#define PDU_HEADER2							5
		
// APDU Header 2
		#define GENI_OP									0xC0
		#define GENI_APDU_LENGHT				0x3F
	// Operation Spezifier        		
		#define GENI_GET								0b00000000	
		#define GENI_SET								0b10000000
		#define GENI_INFO								0b11000000
	// ACK	                      		
		#define GENI_OK									0b00000000
		#define GENI_CLASS_UNKNOWN			0b01000000
		#define GENI_ID_UNKNOWN					0b10000000
		#define GENI_OP_ILLEGAL					0b11000000	
		                            		
		#define _8_BIT_VALUE 						1
		#define _16_BIT_VALUE						2
// Änderung 131105		      				
		#define _24_BIT_VALUE						3
		#define _32_BIT_VALUE 					4
						                    		
		#define NO_CLASS_ID_FOUND				0xFF
  
// Defines für Funktion, Betriebsart, Regelart  
		#define FUNC_DEACTIVATED				0
		#define FUNC_REMOTE							1
		#define FUNC_LOCAL							2
		                            		 
		#define OPERATION_START					1
		#define OPERATION_STOP					0
		#define OPERATION_MIN						2
		#define OPERATION_MAX						3
		                            		 
		#define CONTROL_CONSTPRESS			0
		#define CONTROL_PROPRESS  			1
		#define CONTROL_CONSTFREQ 			2
		#define CONTROL_AUTOMATIC 			3
