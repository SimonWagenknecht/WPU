/********************************************************************
 *																																	*
 *					genibus.c																								*
 *					Datenverarbeitung für Genibus														*
 *					Version: 06.11.2013		anbu															*
 *																																	*
 ********************************************************************/

// 131105: "Extended" bei Auswertung von Messdaten hinzugefügt
// 131106: Überarbeitung der Anzeige der Antwort bei manuellen Anfragen 
 
#include "sfr32C87.h"
#include <string.h>
#include "sio.h"
#include "timer.h"
#include "struct.h"
#include "defins.h"
#include "sramext.h"
#include "uramext.h"
#include "projdef.h"
#include "konvform.h"

//#include "Genibus/genibus_defines.h"
//#include "GeniBus/genibus_extconst.h"	
#include "genibus_defines.h"
#include "genibus_extconst.h"	

#if GENI == 1

// Unterfunktionen 
extern void Genibus_Sio_Init(char port);
extern void Genibus_Start_Transmit(char port);

void PumpeParametrieren(char pu);

char Genibus_Check_Work(char* tx_buffer);

char Genibus_Special_Function(char* tx_buffer);
char Genibus_Create_Telegram(char* tx_buffer);
char Genibus_Get_Class_Id(char dev_idx, char value_type, char* uc_class, char* uc_id_hi, char* uc_id_lo, void* dest_adr);
void Genibus_Add_Checksum(char* tx_buffer);

char Genibus_RX_Telegram(char* rx_buffer);
char Genibus_CheckCRC_RX_Telegram(char* rx_buffer);
char Genibus_Check_RX_Telegram(char* rx_buffer, char adr);
char Genibus_Analyse_Request_Answer(char* rx_buffer, char idx, char dev_idx);
char Genibus_Analyse_Measured_Data(char* rx_buffer, char idx, char dev_idx);
long Genibus_Convert_Measure_Value(char choice, char data_hi, char data_lo, char unit, signed char zero, char range, char comma, char* ret);
long Genibus_Convert_Measure_Value_Extended(char choice, char data_hi, char data_lo1, char data_lo2, char data_lo3, char unit, signed char zero, signed char zero_lo, char comma, char* ret);

void Genibus_Counter(void);
char Genibus_Set_Control_Value(char func, char dev_idx);
void ConvertHexBufferToAscii(char* source_buffer, char* dest_buffer, unsigned char count); 
unsigned char get_profil_ident(unsigned char family, unsigned char type, unsigned char version);  

// ##################################### ZUSATZFUNKTIONEN ZUORDNUNG ############################################
 
// ------------------------------------- Genibus Task ----------------------------------------------------------
// Zyklische Abarbeitung weiterer Genibus Aufgaben
	void Genibus(void)
		{
			static char neustart = 0;
			
			char func = 0;
			char i;

			if ( Funktion_S2 == GENI1_FU )
				Mode_S2 = 1;
			if ( Funktion_S3 == GENI1_FU )
				Mode_S3 = 1;
			for ( i=0; i < GENI_DEVICE_MAX; ++i )
				BusPuPara[i].Adresse = genibus_device_table[i].uc_adr;		

					
			if ( neustart == 0 )
				{
					neustart = 1;

				}
			
			if ( uc_genibus_func_test > 0 )
				{
					Genibus_Set_Control_Value(uc_genibus_func_test, 0);			// Test Command an Gerät 0 senden
					
					uc_genibus_func_test = 0;
				}
			
			// alte Funktionen aus U_Genibus.c zur Verknüpfung der Strukturen 
			for (i=0; i < GENI_DEVICE_MAX; ++i)
				{
					PumpeParametrieren(i);
					
					Geni_Values[i].uc_operation_reply 	= Geni_Values[i].uc_act_mode1 & 0x07;					// Aus Aktuellem Mode extrahieren
					if ( Geni_Values[i].uc_operation_reply < 2 )																				// Aktuelle Betriebsart START oder STOP ?
						Geni_Values[i].uc_operation_reply ^= 1;																						 // Dann: invertieren
						
					Geni_Values[i].uc_control_reply = (Geni_Values[i].uc_act_mode1 >> 3) & 0x07;		// Aus Aktuellem Mode extrahieren
				}	
			
		}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- PumpeParametrieren ---------------------------------------------------
	void PumpeParametrieren(char pu)
		{				
			if ( BusPuPara[pu].Funktion > FUNC_LOCAL ) 
				BusPuPara[pu].Funktion = FUNC_DEACTIVATED;
				
			genibus_device_table[pu].uc_function = BusPuPara[pu].Funktion;	
			
			if ( ( genibus_device_table[pu].uc_function > 0 ) && ( genibus_device_table[pu].uc_adr > 0 ) )
				{			
					if ( BusPuPara[pu].Hand == FALSE )
						{
							BusPuData[pu].uc_betriebsart 	= BusPuPara[pu].Betrieb;				// Aus / Ein / Min / Max 			
							BusPuData[pu].uc_regelart			= BusPuPara[pu].Regelart;				// Konstantdruck, Proportionaldruck, Konstantfrequenz, Automatik

							if ( BusPuPara[pu].Sollwert > 1000 )
								BusPuData[pu].ui_sollwert = 254;
							else
							 	BusPuData[pu].ui_sollwert = BusPuPara[pu].Sollwert; 
						}
					else
						{
							BusPuPara[pu].Hand = TRUE;
							
							BusPuData[pu].uc_betriebsart 	= BusPuPara[pu].BetriebHand;				// Aus / Ein / Min / Max 			
							BusPuData[pu].uc_regelart			= BusPuPara[pu].RegelartHand;				// Konstantdruck, Proportionaldruck, Konstantfrequenz, Automatik
							
							if ( BusPuPara[pu].SollHand > 1000 )
								BusPuData[pu].ui_sollwert = 254;
							else
							 	BusPuData[pu].ui_sollwert = BusPuPara[pu].SollHand; 
						}
					
					// BusPuData[pu].sollwert -> Über Adresse in Reply-Tabelle	
					
				} 
				
				// Ursprünglich aus PumpeLesen	
			BusPuData[pu].busSm  	= Geni_Values[pu].uc_communication_error ? 1 : 0;       // RX Fehlerspeicher übernehmen 0=OK / >0 siehe Defines
					
			BusPuData[pu].puAlarm = ( Geni_Values[pu].uc_led_control & 0x0C ) ? 1 : 0;		// Zustand der roten LED der Pumpe 
			
		}
// ------------------------------------------------------------------------------------------------------------


// ##################################### HAUPTFUNKTION DATENVERARBEITUNG #######################################

// ------------------------------------- U_Genibus -------------------------------------------------------------
// Erste Ebene der Verarbeitung der Daten

char U_Genibus(char port, char funktion)
	{	
		char *pRxBuf;
		char *pTxBuf;
		char fu_ret = funktion;		// Ok Vorbelegung
		char i;

		unsigned char uc_genibus_mastermode = ( ( (Funktion_S2 == GENI1_FU && Mode_S2 == MASTER) && port == S2) ||
																				 	  ( (Funktion_S3 == GENI1_FU && Mode_S3 == MASTER) && port == S3) );
		pRxBuf = &ucf_genibus_rx_buffer[0];
		pTxBuf = NULL;
		
		if ( uc_genibus_mastermode )
			{	
				Genibus_Counter();			// Zähler verarbeiten
						
				switch ( port )
					{
						default:
						case S1:
							funktion = GBUS_FU;
							return funktion;		
							break;
							
						case S2:
							pTxBuf  = TxBuf_S2;
							break;
							
						case S3:
							pTxBuf  = TxBuf_S3;
							break;
					}
			
				switch ( uc_genibus_state )
					{
						case RESTART:																													// Neustart
							uc_genibus_rx_buff_size 			= GENIBUS_RX_BUFFER_SIZE;								// Speichergröße festlegen	
							uc_genibus_own_adr 						= 0x01;																	// Eigene Adresse laden			//ToDo muss noch raus
							uc_genibus_special_function 	= GET_UNIT_INFO;												// Unit Infos über Request laden	
							uc_genibus_request_table_idx	= 0xFF;																	// Damit ++ Null wird
								                                                              		
							for (i=0; i < GENI_DEVICE_MAX; ++i)                             		
								{                                                             		
									if (genibus_device_table[i].uc_function > FUNC_LOCAL)							// Ungültiger Wert?
										genibus_device_table[i].uc_function = FUNC_DEACTIVATED;						// Dann auf "deaktiviert" setzen
								} 				                                                    		
										                                                          		
							Genibus_Sio_Init(port);																								// Init
							                                                                		
							uc_genibus_state = CHECK_WORK;																				// Weiter mit: Aufgaben abarbeiten
							break;                                                          		
						                                                                  		
						case CHECK_WORK:																											// Zyklische Abarbeitung der Aufgaben		
							if ( Genibus_Check_Work(pTxBuf) )                               		
								uc_genibus_state = GENI_TX;																					// Weiter mit: Telegramm senden				
							break;	                                                        		
			                                                                        		
						case GENI_TX:																													// Telegramm senden
							Genibus_Add_Checksum(pTxBuf);																					// Checksumme an vorher zusammengestelltes Telegramm anhängen
							Genibus_Start_Transmit(port);																					// Übertragung auslösen (über U_Genibus_Sio)
							uc_genibus_state = GENI_RX;																						// Weiter mit: Auf Antwort warten
							break;				                                                  		
							                                                                		
						case GENI_RX:																													// Telegramm empfangen
							if ( uc_genibus_rx_telegram_ready == CHECK_NEW_TELEGRAM ) 						// Telegramm komplett? (einige ms Pause)
								{
									uc_genibus_rx_error 					= Genibus_RX_Telegram(pRxBuf);				// Telegramm auswerten
									                                                                  	
									uc_genibus_state 							= CHECK_WORK;													// Weiter mit: Aufgaben abarbeiten
									uc_genibus_rx_telegram_ready 	= TELEGRAM_READY;											// Status zurücksetzen
									uc_genibus_rx_timeout_ctr			= 0;																	// Zurücksetzen								
								}
							else if ( ++uc_genibus_rx_timeout_ctr >= GENIBUS_RX_TIMEOUT )					// RX Timeout?
								{
									uc_genibus_rx_error						= RX_TIMEOUT_ERROR;
								
									Geni_Values[Genibus_Request_Table[uc_genibus_request_table_idx].uc_devicetable_idx].uc_communication_error = uc_genibus_rx_error;  // Merlen für SM Bus
									 
									uc_genibus_rx_timeout_ctr			= 0;																	// Zurücksetzen
									uc_genibus_rx_count						= 0;
									uc_genibus_state 							= CHECK_WORK;													// Weiter mit: Aufgaben abarbeiten
									
									if ( uc_genibus_special_function )																	// Spezialfunktionen bei Timeout zurücksetzen
										{
											uc_genibus_get_adress 					= 0;
											uc_genibus_reset_pump						= 0;
											uc_genibus_start_manual_request	= 0;
										}
								}
							break;
					}
			}			
		return fu_ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ##################################### AUFGABEN VERWALTEN ###################################################

// ------------------------------------- Nächste Aufgabe ermitteln --------------------------------
// Ermittelt die nächste Aufgabe und liefert zurück, ob ein Telegramm verschickt werden soll
char Genibus_Check_Work(char* tx_buffer)
	{
		char ret 	= NO_TRANSMISSION;
		char i		= 0;
		
		if (	uc_genibus_special_function == GET_UNIT_INFO )											// Einmalig bei Neustart
			{
				if ( ++uc_genibus_request_table_idx < Genibus_Request_Table_Size )	
					{
						for ( i=0; i<uc_genibus_request_table_idx; ++i)
							{
								if ( Genibus_Request_Table[uc_genibus_request_table_idx].uc_devicetable_idx == Genibus_Request_Table[i].uc_devicetable_idx )		// Aktuelles Device wurde bereits abgefragt?
									return ret;																																																											// ABBRUCH mit RETURN!!!
							}		
						ret = Genibus_Special_Function(tx_buffer);
					}
				else
					uc_genibus_special_function 	= NO_SPECIAL_FKT;
				ui_genibus_relay_ctr = 0;		
				return ret;																															// mit vorzeitigem RETURN!!!
			}
			
		uc_genibus_special_function = NO_SPECIAL_FKT;
			
		if ( uc_genibus_get_adress > 0 )																					// Jederzeit zwischendurch
			{
				uc_genibus_special_function = GET_ADRESS;
				ui_genibus_relay_ctr 				= 0;
					
				ret = Genibus_Special_Function(tx_buffer);
			}
		else if ( uc_genibus_reset_pump > 0 )
			{ 
				uc_genibus_special_function = RESET_PUMP;
				ui_genibus_relay_ctr 				= 0;	
				
				ret = Genibus_Special_Function(tx_buffer);
			}
		else if ( uc_genibus_start_manual_request > 0 )
			{
				uc_genibus_special_function = SEND_VARIABLE_REQUEST;
				ui_genibus_relay_ctr 				= 0;	
				
				ret = Genibus_Special_Function(tx_buffer);
			}
		else 																																			// Zyklische Abarbeitung
			{
				if ( ++ui_genibus_relay_ctr > ((int)Genibus_Request_Table[uc_genibus_request_table_idx].uc_replay_time << 2) )		// Request Table: Relay Timer abgelaufen?
					{
						ui_genibus_relay_ctr = 0;	
						
						if ( ++uc_genibus_request_table_idx >= Genibus_Request_Table_Size )			// Ende der Request Tabelle erreicht?
							uc_genibus_request_table_idx = 0;																				// Von vorn beginnen		
						
						if ( Genibus_Create_Telegram(tx_buffer) )
							ret = GENI_TX;				
					}	
			}
			
		return ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ##################################### DATEN SENDEN #########################################################

// ------------------------------------- Genibus Spezialfunktionen verarbeiten --------------------------------
// Stellt ein Telegramm für einmalige oder manuell ausgelöste Ereignisse zusammen
char Genibus_Special_Function(char* tx_buffer)
	{
		char ret 					= TRANSMIT_TELEGR;
		char lenght				= 0;																														// Gesamte Telegrammlänge
		char apdu_lenght	= 0;																														// APDU Länge
		char idx 					= uc_genibus_request_table_idx;																	// Index in Request Table
		char dev_idx			= 0;																														// Index in Device Table
		char i 						= 0;																														// Allgemeine Zählvariable
				
		tx_buffer[START_BYTE] 			= STARTBYTE_DATA_REQUEST;															// Startbyte Request
		tx_buffer[SOURCE_ADR_BYTE] 	= uc_genibus_own_adr;																	// Eigene Adresse
		
		switch ( uc_genibus_special_function )
			{
				case GET_UNIT_INFO:																														
					dev_idx = Genibus_Request_Table[idx].uc_devicetable_idx;											// Aktueller Device Index
					i				= Genibus_Unit_Request_Size;
					tx_buffer[DEST_ADR_BYTE] 	= genibus_device_table[dev_idx].uc_adr+31;					// Device Adresse
					tx_buffer[PDU_HEADER1] 		= MEASURED_DATA;																		// APDU 1 für Unit Infos
					tx_buffer[PDU_HEADER2] 		= GENI_GET | i;																			// Genibus_Unit_Request_Size Byte (3 Byte)
					memcpy (&tx_buffer[PDU_HEADER2+1], &Genibus_Unit_Request[0], i); 
					++i;
					tx_buffer[PDU_HEADER2+i++] 	= ASCII_STR;																			// APDU 2 für Pumpenname
					tx_buffer[PDU_HEADER2+i++] 	= GENI_GET | 1;
					tx_buffer[PDU_HEADER2+i]		= GENI_product_name;
							
					lenght = 6 + i;
					break;
		//---------------------------------------			
				case GET_ADRESS:
					tx_buffer[DEST_ADR_BYTE] 	= GENI_BROADCAST_ADR;	
					tx_buffer[PDU_HEADER1] 		= CONFIG_PARAM;																	
					tx_buffer[PDU_HEADER2] 		= GENI_GET | 2;																			 
					tx_buffer[PDU_HEADER2+1] 	= GENI_unit_addr;
					tx_buffer[PDU_HEADER2+2] 	= GENI_group_addr;
					
					lenght = 8;			// 6+2															
					break;
		//---------------------------------------				
				case RESET_PUMP:
					tx_buffer[DEST_ADR_BYTE] 	= uc_genibus_reset_pump;											
					tx_buffer[PDU_HEADER1] 		= COMMANDS;																	
					tx_buffer[PDU_HEADER2] 		= GENI_SET | 1;															
					tx_buffer[PDU_HEADER2+1] 	= GENI_reset;
					
					lenght = 7;			// 6+1																	
					break;
		//---------------------------------------									
				case SEND_VARIABLE_REQUEST:
					if ( ( genibus_manual_request.uc_geni_class >= MEASURED_DATA ) && 
							 ( genibus_manual_request.uc_geni_class <= ASCII_STR ) )
						{
							tx_buffer[DEST_ADR_BYTE] 	= genibus_manual_request.uc_adrr;
							tx_buffer[PDU_HEADER1] 		= genibus_manual_request.uc_geni_class;	
							tx_buffer[PDU_HEADER2+1] 	= genibus_manual_request.uc_id_hi;
							if ( genibus_manual_request.uc_id_lo == 0xFF )
								{ 																
									tx_buffer[PDU_HEADER2] 		= genibus_manual_request.uc_geni_op | 1;	
									lenght = 7;		// 6+1;																		
								}
							else
								{
									tx_buffer[PDU_HEADER2] 		= genibus_manual_request.uc_geni_op | 2;
									tx_buffer[PDU_HEADER2+2] 	= genibus_manual_request.uc_id_lo;
									lenght = 8;		// 6+2;
								}
						}
					else
						ret = NO_TRANSMISSION;
						
					break;				
			}		
			
		tx_buffer[LENGHT_BYTE] 	= lenght-2;																								// Länge = Byteanzahl ohne Start und Länge
		uc_genibus_tx_count 		= lenght;																									// Telegrammgröße merken
																		
		return ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus Request Telegramm verarbeiten --------------------------------
// Stellt ein Telegramm aus der Request Table zusammen
char Genibus_Create_Telegram(char* tx_buffer)
	{
		static char last_idx;

    char ret 					= TRANSMIT_TELEGR;
    char lenght 			= 0;                            // Gesamte Telegrammlänge
    char apdu_lenght 	= 0;                          	// APDU Länge 
    char uc_class 		= 0;                            // Aktuelle Geni Class
    char uc_id_hi 		= 0;                            // Aktuelle Geni ID Hi
    char uc_id_lo 		= 0xFF;               					// Aktuelle Geni ID Lo (0xFF Vorbelegung falls nicht verwendet) 

    char idx = uc_genibus_request_table_idx;        	// Aktueller Request Index
    char dev_idx, value_type;
                                                      

    if ( uc_genibus_rx_error == RX_TIMEOUT_ERROR )          					// Bei letzter Abfrage kam es zum Timeout (Pumpe antwortet nicht)      
      {
      	while ( Genibus_Request_Table[last_idx].uc_devicetable_idx == Genibus_Request_Table[idx].uc_devicetable_idx )   // Solange Gerätindex der Timeoutpumpe gleich dem Gerätindex des aktuellen Requests
          {
            ++idx;  																											// Request Index hochzählen, damit Timeout-Pumpe erstmal nicht abgefragt wird!
            if ( idx >= Genibus_Request_Table_Size )     									// Überlauf abfangen
              {
                idx = 0;            																				// Zurücksetzen auf ersten Eintrag
                break;             																					// Schleife verlassen      
              }
         }
       uc_genibus_request_table_idx = idx;    													// Globalen Request Index aktualisieren
     } 

    last_idx   = idx;      																						// letzten Device Index merken               
    dev_idx    = Genibus_Request_Table[idx].uc_devicetable_idx;       // Aktueller Device Index
    value_type = Genibus_Request_Table[idx].uc_value_type;            // Welcher Type Wert soll abgefragt werden   
																																					
					
		if ( dev_idx < GENI_DEVICE_MAX ) 																																// Gültiger Deviceindex 
			{
				if ( genibus_device_table[dev_idx].uc_function > 0 ) 																					// Funktion REMOTE oder LOCAL
					{				                                                                      									
						tx_buffer[START_BYTE] 			= STARTBYTE_DATA_REQUEST;																						// Header zusammenstellen
						tx_buffer[DEST_ADR_BYTE] 		= genibus_device_table[dev_idx].uc_adr+31;           									
						tx_buffer[SOURCE_ADR_BYTE] 	= uc_genibus_own_adr;                           									
						lenght += 4;  
						                                                                									
						Genibus_Get_Class_Id(dev_idx, value_type, &uc_class, &uc_id_hi, &uc_id_lo, Genibus_Request_Table[idx].p_dest_value);											// Laden der Class und Id für aktuellen Request		
						
						if ( uc_class == NO_CLASS_ID_FOUND )																														// Abbruch, keine Aussendung!
							ret = NO_TRANSMISSION;
						else
							{                                                                          									
								tx_buffer[lenght++] = uc_class;																																	// APDU Header 1
								++lenght;																																												// Platzhalter für Header 2
								
								if ( uc_id_lo != 0xFF )																																					// APDU Data zusammenstellen
									{
										apdu_lenght = 2;
										tx_buffer[lenght++] = uc_id_hi;
										tx_buffer[lenght++] = uc_id_lo;
									}	
								else
									{
										apdu_lenght = 1;
										tx_buffer[lenght++] = uc_id_hi;
									}		
										
								switch ( uc_class )
									{
										case MEASURED_DATA:																																					// Measured Data Anfragen: immer mit INFO
											tx_buffer[lenght-apdu_lenght-1] = apdu_lenght | GENI_GET;																		// APDU Header 2 (GET)
											                                                                                      		
											tx_buffer[lenght++] = uc_class;																															// APDU Info Header 1 
											tx_buffer[lenght++] = 1 | GENI_INFO;																												// APDU Info Header 2 (immer Länge 1!)
											tx_buffer[lenght++] = uc_id_hi;																															// Info nur über Hi Byte Anfrage
											break;                                                                                		
										                                                                                        		
										case COMMANDS:
										case CONFIG_PARAM: 
										case REF_VALUES:                                                                         		
											tx_buffer[lenght-apdu_lenght-1] = apdu_lenght | GENI_SET;																		// APDU Header 2 (SET)
											break;
											
										case ASCII_STR:
											tx_buffer[lenght-apdu_lenght-1] = apdu_lenght | GENI_GET;																		// APDU Header 2 (GET)
											break;
									}	
									
								tx_buffer[LENGHT_BYTE] 	= lenght-2;																															// Länge = Byteanzahl ohne Start und Länge
								uc_genibus_tx_count 		= lenght;																																// Telegrammgröße merken
							}
					}
				else
					{
						Geni_Values[dev_idx].uc_communication_error = 0;			// Keine Abfrage, also Alarme löschen
						Geni_Values[dev_idx].uc_led_control 				= 0;
						ret = NO_TRANSMISSION;
					}
			}		
		else
			ret = NO_TRANSMISSION;

		return ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus Class ID für Request -----------------------------------------
// Ermitteln der Class und ID für aktuelles Request
char Genibus_Get_Class_Id(char dev_idx, char value_type, char* uc_class, char* uc_id_hi, char* uc_id_lo, void* dest_adr)
	{
		char ret = TRANSMIT_TELEGR;
		
		char profil_ident = genibus_device_table[dev_idx].uc_profile_ident; 
		char indiv_idx;
		
		*uc_class = MEASURED_DATA;			// Vorbelegung, da die meisten "Measured Data" sind
		
		switch ( value_type )
			{
				case GENI_OP_TIME:
					*uc_id_hi = GENI_t_2hour_hi;
					*uc_id_lo = GENI_t_2hour_lo;
					break;
				case GENI_VOLTAGE:  
					*uc_id_hi = GENI_v_dc;
					break;
				case GENI_MOTOR_TEMP:		
					*uc_id_hi = GENI_t_m;
					break;
				case GENI_WATER_TEMP:		
					*uc_id_hi = GENI_t_w;
					break;	
				case GENI_FLOW:
					*uc_id_hi = GENI_q;
					break;			
				case GENI_HEIGHT:
					*uc_id_hi = GENI_h;	
					break;	
				case GENI_POWER:
					*uc_id_hi = GENI_p;
					break;			
				case GENI_ENERGY:
					*uc_id_hi = GENI_energy_hi;
					*uc_id_lo = GENI_energy_lo;
					break;
				case GENI_RESISTANCE:
					// ToDo Bei UPE kein Wert
					break;	
				case GENI_CURRENT_DC:
					*uc_id_hi = GENI_i_dc; 
					break;	
				case GENI_CURRENT_MO: 
					*uc_id_hi = GENI_i_mo; 
					break;			
				case GENI_FREQUENCY:
					// ToDo Bei UPE kein Wert
					break; 	
				case GENI_PRESSURE:
					// ToDo Bei UPE kein Wert
					break;		
				case GENI_TIME:
					// ToDo Bei UPE kein Wert
					break;      	
				case GENI_VOLUME:
					// ToDo Bei UPE kein Wert
					break;	
				case GENI_LED_STATUS:
					*uc_id_hi = GENI_led_contr;
					break;	
				case GENI_ACT_MODE1:
					*uc_id_hi = GENI_act_mode1;	
					break;
				case GENI_ACT_MODE2:
					*uc_id_hi = GENI_act_mode2;
					break;	
				case GENI_ACT_MODE3:	
					*uc_id_hi = GENI_act_mode3;
					break;	
				case GENI_ALARM_CODE:	
					*uc_id_hi = GENI_alarm_code;
					break;	
				case GENI_INDIV1:	
				case GENI_INDIV2:
				case GENI_INDIV3:
				case GENI_INDIV4:
				case GENI_INDIV5:
				case GENI_INDIV6:
				case GENI_INDIV7:
				case GENI_INDIV8:
				case GENI_INDIV9:
				case GENI_INDIV10:
				case GENI_INDIV11:
				case GENI_INDIV12:
				case GENI_INDIV13:
				case GENI_INDIV14:
				case GENI_INDIV15:
				case GENI_INDIV16:
					indiv_idx = value_type & 0x7F;
					if ( indiv_idx < Genibus_Indiv_Req_Table_Size )		// Eintrag vorhanden?
						{
							*uc_class = Genibus_Indiv_Req_Table[indiv_idx].uc_class;
							*uc_id_hi = Genibus_Indiv_Req_Table[indiv_idx].uc_id_hi;
							*uc_id_lo = Genibus_Indiv_Req_Table[indiv_idx].uc_id_lo;
						}
					else
						{
							*uc_class = NO_CLASS_ID_FOUND;							
							ret 			= NO_TRANSMISSION;
						}
					break;
														
				case GENI_FUNCTION:
					*uc_class = COMMANDS;  
					switch ( *(char*)dest_adr )
						{
							case FUNC_DEACTIVATED:
								*uc_id_hi = 0;	// Eigentlich nicht möglich, da Func = 0 aussenden aller Telegramme für diese Pumpe verhindert!
								break;
							case FUNC_REMOTE:
								*uc_id_hi = GENI_remote;
								break;
							case FUNC_LOCAL:
								*uc_id_hi = GENI_local;
								break;
							default:
								*uc_class = NO_CLASS_ID_FOUND;							
								ret 			= NO_TRANSMISSION;
								break;
						}  
					break;
						
				case GENI_OPERATION:
					*uc_class = COMMANDS;
					switch ( *(char*)dest_adr )
						{
							case OPERATION_START:
								*uc_id_hi = GENI_start;
								break; 
							case OPERATION_STOP: 
								*uc_id_hi = GENI_stop; 	
						 		break;
							case OPERATION_MIN: 
								*uc_id_hi = GENI_min; 
								break;
							case OPERATION_MAX:
								*uc_id_hi = GENI_max;  	
								break;	
							default:
								*uc_id_hi = GENI_start;
								break;
						}
					break;
					 
				case GENI_CONTROL:
					*uc_class = COMMANDS; 
					switch ( *(char*)dest_adr )
						{
							case CONTROL_CONSTPRESS:
								*uc_id_hi = GENI_const_press;
								break; 
							case CONTROL_PROPRESS:  	
								*uc_id_hi = GENI_prop_press;
						 		break;
							case CONTROL_CONSTFREQ:
								*uc_id_hi = GENI_const_freq;  
								break;
							case CONTROL_AUTOMATIC: 
								*uc_id_hi = GENI_automatic; 	
								break;	
							default:
								*uc_id_hi = GENI_const_press;
								break;								
						}
					break;	
					  
				case GENI_SETPOINT:
					*uc_class = REF_VALUES;
					*uc_id_hi = GENI_ref_rem;
					*uc_id_lo = (char)( (unsigned long)(*(unsigned int*)dest_adr) * 254 / 1000 ); 
					break;	
							
				default:														// Unterscheidung nach Pumpe
					switch ( profil_ident )
						{
							case PROFIL1:
								switch ( value_type )
									{
										case GENI_SPEED:
											*uc_id_hi = GENI_speed;
											break;
			
										default:
											*uc_class = NO_CLASS_ID_FOUND;							
											ret 			= NO_TRANSMISSION;
										break;
									}
								break;
								
							case PROFIL2:
								switch ( value_type )
									{
										case GENI_SPEED:
											*uc_id_hi = GENI_speed;
											*uc_id_lo = GENI_speed_lo_xtra;
											break;
													
										default:
											*uc_class = NO_CLASS_ID_FOUND;			
											ret 			= NO_TRANSMISSION;
										break;
									}						
								break;

							case PROFIL3:
								switch ( value_type )
									{
										case GENI_SPEED:
											*uc_id_hi = GENI_speed;
											*uc_id_lo = GENI_speed_lo;
											break;
													
										default:
											*uc_class = NO_CLASS_ID_FOUND;			
											ret 			= NO_TRANSMISSION;
										break;
									}						
								break;
																
							default:
								*uc_class = NO_CLASS_ID_FOUND;	
								ret 			= NO_TRANSMISSION;
								break;
						} 
					break;
			}								
		
		return ret;			
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus Checksumme ---------------------------------------------------
// Fügt die Checksumme an die übergebene Stelle an
void Genibus_Add_Checksum(char* tx_buffer)
	{
		unsigned int fcs = Checksum ( &tx_buffer[1], uc_genibus_tx_count-1 );			// Frame Check Sequenz berechnen (Ohne Startbyte)
		fcs ^= 0xffff;   																															// Complement bilden
		tx_buffer[uc_genibus_tx_count++] = (fcs & 0x00ff);
		tx_buffer[uc_genibus_tx_count++] = ((fcs >> 8) & 0x00ff);
	}	
// ------------------------------------------------------------------------------------------------------------


// ##################################### DATEN EMPFANGEN ######################################################

// ------------------------------------- Genibus RX Telegramm Verarbeitung ------------------------------------
// Auswerten des empfangenen Telegramms
char Genibus_RX_Telegram(char* rx_buffer)
	{
		char ret;
		char idx 					= 0;
		char dev_idx 			= 0;
		char i						= 0;	
		
		ret = Genibus_CheckCRC_RX_Telegram(rx_buffer);								// Telegramm auf gültige Länge und CRC testen
		
		if ( ret == RX_NO_ERROR )																			// Telegramm bytemäßig OK
			{
				idx 		= uc_genibus_request_table_idx;											// Für diesen Eintrag wurde die Anfrage gestellt
				dev_idx = Genibus_Request_Table[idx].uc_devicetable_idx;		// An dieses Gerät wurde die Anfrage gestellt

				switch ( uc_genibus_special_function )											// Welche Anfrage wurde gestellt?
					{
						default:
						case NO_SPECIAL_FKT:																			// zyklische Anfrage aus Request Table
							ret = Genibus_Check_RX_Telegram(rx_buffer, genibus_device_table[dev_idx].uc_adr+31);				// Telegramm vom richtigen Slave und für mich?
							
							if ( ret == RX_NO_ERROR )																		// Telegramm OK
								ret = Genibus_Analyse_Request_Answer(rx_buffer, idx, dev_idx);
							break;
				// -------------------------------------	
						case GET_UNIT_INFO:																				// Unit Infos abfragen (bei Neustart)
							ret = Genibus_Check_RX_Telegram(rx_buffer, genibus_device_table[dev_idx].uc_adr+31);				// Telegramm vom richtigen Slave und für mich?
							
							if ( ret == RX_NO_ERROR )
								{								
									if ( ( rx_buffer[PDU_HEADER1] == MEASURED_DATA ) && 
											 ( (rx_buffer[PDU_HEADER2] & GENI_OP) == GENI_OK ) )
										{
											genibus_device_table[dev_idx].uc_unit_family	= rx_buffer[PDU_HEADER2+1];
											genibus_device_table[dev_idx].uc_unit_type		= rx_buffer[PDU_HEADER2+2];
											genibus_device_table[dev_idx].uc_unit_version	= rx_buffer[PDU_HEADER2+3];
											
											if ( ( genibus_device_table[dev_idx].uc_profile_ident == 0xFF ) || 																		// Bei allererster Abfrage (Eepromwert)
												 	 ( genibus_device_table[dev_idx].uc_profile_ident == 0x00 ) ) 																		// oder nach Einstellung 0 zur Ermittlung nach Neustart
												{
													genibus_device_table[dev_idx].uc_profile_ident = get_profil_ident(genibus_device_table[dev_idx].uc_unit_family, genibus_device_table[dev_idx].uc_unit_type, genibus_device_table[dev_idx].uc_unit_version);
												}
										}
									else
										ret = RX_APDU1_ERROR;	// ToDo ggf Fehlermeldung von Genibusgerät mitspeichern
										
									if ( ( rx_buffer[PDU_HEADER2+Genibus_Unit_Request_Size+1] == ASCII_STR ) && 
											 ( (rx_buffer[PDU_HEADER2+Genibus_Unit_Request_Size+2] & GENI_OP) == GENI_OK ) )
										{	
											i = rx_buffer[PDU_HEADER2+Genibus_Unit_Request_Size+2] & GENI_APDU_LENGHT;
											if ( i < 16 )
												{
													memcpy ( &genibus_device_table[dev_idx].ucf_name[0], &rx_buffer[PDU_HEADER2+Genibus_Unit_Request_Size+3], i);
													genibus_device_table[dev_idx].ucf_name[i] = '\0';
												}
											else
												memcpy ( &genibus_device_table[dev_idx].ucf_name[0], &rx_buffer[PDU_HEADER2+Genibus_Unit_Request_Size+3], 16);
										}
									else
										ret = RX_APDU2_ERROR;																			 
								}
							break;
				// -------------------------------------		
						case GET_ADRESS:
							if ( rx_buffer[DEST_ADR_BYTE] != uc_genibus_own_adr )	
								ret = RX_DEST_ERROR;
							
							if ( ret == RX_NO_ERROR )
								{	
									if ( ( rx_buffer[PDU_HEADER1] == CONFIG_PARAM ) && 
											 ( (rx_buffer[PDU_HEADER2] & GENI_OP) == GENI_OK ) )
										{
											if ( uc_genibus_get_adress <= GENI_DEVICE_MAX )
												{
													genibus_device_table[uc_genibus_get_adress-1].uc_adr				= rx_buffer[PDU_HEADER2+1]-31;
													genibus_device_table[uc_genibus_get_adress-1].uc_group_adr	= rx_buffer[PDU_HEADER2+2];
												}
											else
												ret = RX_DEVIDX_ERROR;
										}
									else
										ret = RX_APDU1_ERROR; 
								}
								
							uc_genibus_special_function = NO_SPECIAL_FKT;		
							uc_genibus_get_adress 			= 0; 
							break;
				// -------------------------------------		
						case RESET_PUMP:	
							ret = Genibus_Check_RX_Telegram(rx_buffer, uc_genibus_reset_pump);				// Telegramm vom richtigen Slave und für mich?
							
							if ( ret == RX_NO_ERROR )
								{	
									if ( ( rx_buffer[PDU_HEADER1] == COMMANDS ) && 
											 ( (rx_buffer[PDU_HEADER2] & GENI_OP) == GENI_OK ) )
										{	
											uc_genibus_reset_result = uc_genibus_reset_pump; 
										}	
									else
										{
											ret 										= RX_APDU1_ERROR;
											uc_genibus_reset_result = 0xFF;
										}	
								}
								
							uc_genibus_special_function = NO_SPECIAL_FKT;						
							uc_genibus_reset_pump 			= 0;
							break;
				// -------------------------------------								
						case SEND_VARIABLE_REQUEST:
							ret = Genibus_Check_RX_Telegram(rx_buffer, genibus_manual_request.uc_adrr);				// Telegramm vom richtigen Slave und für mich?
							
							if ( ret == RX_NO_ERROR )
								{	
									if ( ( rx_buffer[PDU_HEADER1] >= MEASURED_DATA) && 
									 		 ( rx_buffer[PDU_HEADER1] <= ASCII_STR) && 
											 ( (rx_buffer[PDU_HEADER2] & GENI_OP) == GENI_OK ) )
										{	
											ConvertHexBufferToAscii(&rx_buffer[PDU_HEADER1], &genibus_manual_request.ucf_answer[0], 8);  		// 8 Byte in 16 Ascii-Zeichen umwandeln								
										}
									else
										ret = RX_APDU1_ERROR; 	
								}
							uc_genibus_start_manual_request	= 0;
							uc_genibus_special_function 		= NO_SPECIAL_FKT;																	// Nächste Aufgabe: Keine Speizialfunktion -> Request Table abarbeiten	
							break;						
					}
			}
		Geni_Values[dev_idx].uc_communication_error = ret;               // Fehlercode für entsprechendes Gerät speichern
			
		return ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus RX Telegramm überprüfen --------------------------------------
// Überprüft das Startbyte, die Länge und Checksumme und liefert das Ergebnis der Prüfung zurück
char Genibus_CheckCRC_RX_Telegram(char* rx_buffer)
	{
		char ret 										= RX_NO_ERROR;
		unsigned int rx_checksum 		= 0;
		unsigned int calc_checksum 	= 0;
		unsigned char lenght 				= rx_buffer[LENGHT_BYTE];
 
 		if ( rx_buffer[START_BYTE] == STARTBYTE_DATA_REPLY )
			{
				rx_buffer[START_BYTE] = 0;	// Nachricht als behandelt kennzeichnen
				if ( lenght < GENIBUS_RX_BUFFER_SIZE )
					{
						calc_checksum  	= Checksum ( &rx_buffer[1], lenght+1 );			// Checksumme von Länge bis letzte Byte vor CRC
						calc_checksum  ^= 0xffff; 
						rx_checksum 		= rx_buffer[lenght+2] | ( (int)rx_buffer[lenght+3] << 8);
		
						if ( rx_checksum != calc_checksum )
							ret = RX_CHECKSUM_ERROR;
					}
				else
					ret = RX_LENGHT_ERROR;	
			}
		else
			ret = RX_START_ERROR;
				
		uc_genibus_rx_count	= 0;
			
		return ret;
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus RX Telegramm überprüfen --------------------------------------
// Überprüft, ob die Antwort vom erwarteten Gerät kommt
char Genibus_Check_RX_Telegram(char* rx_buffer, char adr)
	{
		char ret = RX_NO_ERROR;
		
		if ( rx_buffer[SOURCE_ADR_BYTE] == adr )		// Richtiges Gerät hat geantwortet?
			{
				if ( rx_buffer[DEST_ADR_BYTE] != uc_genibus_own_adr )	
					ret = RX_DEST_ERROR;
			}
		else
			ret = RX_SOURCE_ERROR;	
			
		return ret;		
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Genibus RX Request Table Antwort Auswertung --------------------------
// Auswertung der Anfrage aus der Request Table mit Rückgabe des Status
char Genibus_Analyse_Request_Answer(char* rx_buffer, char idx, char dev_idx)
	{
		char ret = RX_NO_ERROR;			
		
		switch ( rx_buffer[4] )
			{
				case MEASURED_DATA:
					ret = Genibus_Analyse_Measured_Data(rx_buffer, idx, dev_idx);
					break;
					
				case COMMANDS:
				case CONFIG_PARAM: 
				case REF_VALUES:  	
					ret = rx_buffer[5];		// Statusmeldung z.B. OK // ToDo Auswertung der Returncodes 
					break;
				default:
					ret = RX_CLASS_ERROR;
					break;
			}

		return ret;	
	}
// ------------------------------------------------------------------------------------------------------------


// Änderung 131105 - Extended Auswertung hinzugefügt
// ------------------------------------- Genibus Measured Data auswerten --------------------------------------
// Rechnet den Wert im Telegramm nach den Vergaben des INFO-Teils um und speichert ihn an der gewünschten Adresse
char Genibus_Analyse_Measured_Data(char* rx_buffer, char idx, char dev_idx)
	{
		char ret = RX_NO_ERROR;	
		
		char apdu_lenght1,	apdu_lenght2	= 0;																										// APDU Länge 1 (von Value) / APDU Länge 2 (von INFO)

		long value	= 0;		// Zwischenspeicher für gelieferten Wert

		unsigned char info_start 	= 0;
		unsigned char vi					= 0;
		unsigned char	bo					= 0;
		unsigned char	sif					= 0;
		unsigned char unit				= 0;
		signed   char zero				= 0;
		signed   char zero_lo			= 0;
		unsigned char range 			= 0;
		signed   char sz 					= 0;
		
		char comma 	= Genibus_Request_Table[idx].uc_dest_comma;
		char type 	= Genibus_Request_Table[idx].uc_dest_type;

	
	// Requestaufbau immer eine GET-Anfrage auf die ID und anschließend eine INFO-Anfrage auf die ID	
		apdu_lenght1 = rx_buffer[5] & 0x3F;
		apdu_lenght2 = rx_buffer[5+apdu_lenght1+2] & 0x3F;
			
		switch ( apdu_lenght2 ) 
			{
				case 1:																													// Keine Scale-Infos vorhanden -> direkte Übergabe der Daten
					info_start 	= rx_buffer[5+apdu_lenght1+3] & HEADER_START;
					vi					= rx_buffer[5+apdu_lenght1+3] & HEADER_VI;
					sif 				= rx_buffer[5+apdu_lenght1+3] & HEADER_SIF;
					bo 					= rx_buffer[5+apdu_lenght1+3] & HEADER_BO;
					break;
					
				case 4:																													// Scale-Infos vorhanden -> Extrahieren für Weiterverwendung
				case 5:
					info_start 	= rx_buffer[5+apdu_lenght1+3] & HEADER_START;
					vi					= rx_buffer[5+apdu_lenght1+3] & HEADER_VI;
					sif 				= rx_buffer[5+apdu_lenght1+3] & HEADER_SIF;
					bo 					= rx_buffer[5+apdu_lenght1+3] & HEADER_BO;
					sz 					= rx_buffer[5+apdu_lenght1+4] & HEADER_SZ;  
					unit 				= rx_buffer[5+apdu_lenght1+4] & UNIT;		                                       
					zero 				= rx_buffer[5+apdu_lenght1+5];                                                  
					range 			= rx_buffer[5+apdu_lenght1+6];
					zero_lo			= rx_buffer[5+apdu_lenght1+6];				// Bei Extended an der Stelle von Range  
					
					if ( ( info_start != 0x80 ) ||										// HeaderStart ist nicht korrekt  
						   ( sif == 0 ) ) 															// No Scaling Infos available	 
						ret = RX_NO_INFO_ERROR;	
					break;
					
				default:
					ret = RX_INFO_APDU_ERROR;		
					break;
			}
		
		if (sz == 1 )			// SZ gibt Vorzeichen von Zero an
			zero *= -1;

		if ( ret == RX_NO_ERROR )										 
    	{       
    		if (( apdu_lenght1 < 1 ) || ( apdu_lenght1 > 4 )) 
    			ret = RX_GET_APDU_ERROR;	
    		else
    			{                                                                 
						switch ( sif )
							{
								case SCALE_NOT_AVAILABLE:		
								case BIT_WISE_INTERPRETED:
									value = apdu_lenght1 == 2 ? (long)(((int)rx_buffer[6]<<8) | rx_buffer[7]) : (long)rx_buffer[6];
									break;
									
								case SCALED_816BIT_VALUE:
									switch ( apdu_lenght1 )
										{
											case 1:
												value = Genibus_Convert_Measure_Value(_8_BIT_VALUE, rx_buffer[6], 0, unit, zero, range, comma, &ret);
												break;
											case 2:
												value = Genibus_Convert_Measure_Value(_16_BIT_VALUE, rx_buffer[6], rx_buffer[7], unit, zero, range, comma, &ret);
												break;
											default:
												ret = RX_GET_APDU_ERROR;
											break;
										}
									break;
									
								case EXTENDED_PRECISION:
									switch ( apdu_lenght1 )
										{
											case 1:
												value = Genibus_Convert_Measure_Value_Extended(_8_BIT_VALUE, rx_buffer[6], 0, 						0, 						0, 						unit, zero, zero_lo, comma, &ret);
												break;                                                                                                     						
											case 2:                                                                                                      						
												value = Genibus_Convert_Measure_Value_Extended(_16_BIT_VALUE, rx_buffer[6], rx_buffer[7], 0,						0, 						unit, zero, zero_lo, comma, &ret);	
												break;                                                                                                     						
											case 3:                                                                                                      						
												value = Genibus_Convert_Measure_Value_Extended(_24_BIT_VALUE, rx_buffer[6], rx_buffer[7], rx_buffer[8],	0, 						unit, zero, zero_lo, comma, &ret);	
												break;
											case 4:
												value = Genibus_Convert_Measure_Value_Extended(_32_BIT_VALUE, rx_buffer[6], rx_buffer[7], rx_buffer[8], rx_buffer[9],	unit, zero, zero_lo, comma, &ret);	
												break;
											default:
												ret = RX_EXT_PRE_ERROR;
												break;	
										}																			
									break;
							}
					}												
				
				switch ( type )
					{
						case S_CHAR:
							if ( value > 0xFF )
								*(unsigned char*)Genibus_Request_Table[idx].p_dest_stat = STAT_BIGGER_CHAR;
								
							*(signed char*)Genibus_Request_Table[idx].p_dest_value = (signed char)value;	
								
							break;
							
						case US_CHAR:
							if ( value > 0xFF )
								*(unsigned char*)Genibus_Request_Table[idx].p_dest_stat = STAT_BIGGER_CHAR;
								
							*(unsigned char*)Genibus_Request_Table[idx].p_dest_value = (unsigned char)value;	
								
							break;
							
						case S_INT:
							if ( value > 0xFFFF )
								*(unsigned char*)Genibus_Request_Table[idx].p_dest_stat = STAT_BIGGER_INT;
								
							*(signed int*)Genibus_Request_Table[idx].p_dest_value = (signed int)value;	
							
							break;
							
						case US_INT:
							if ( value > 0xFFFF )
								*(unsigned char*)Genibus_Request_Table[idx].p_dest_stat = STAT_BIGGER_INT;

							*(unsigned int*)Genibus_Request_Table[idx].p_dest_value = (unsigned int)value;	
							
							break;	
							
/*						case S_LONG:
							if ( sz == 0 )                                                                 
								*(unsigned char*)Genibus_Request_Table[idx].p_dest_stat |= STAT_WRONG_SIGNESS;
								
							*(signed long*)Genibus_Request_Table[idx].p_dest_value = (signed long)value;	
							
							break;*/
							
						case US_LONG:								
							*(unsigned long*)Genibus_Request_Table[idx].p_dest_value = (unsigned long)value;	
							
							break;									
					}		
			}
		return ret;	
	}
// ------------------------------------------------------------------------------------------------------------


// ------------------------------------- Konvertierung Measure Value ------------------------------------------
// Übergabewerte:
// choice -> _16_BIT_VALUE oder _8_BIT_VALUE
// data_hi -> Highbyte der empfangenen Daten 
// data_lo -> Lowbyte (nur bei 16-Bit-Wert, sonst UNUSED = 255 belegt für Anzeige eines 8-Bit-Wertes)
// unit -> Index der Unittable, der empfangen wurde
// zero -> Für Verrechnung aus INFO-Antwort
// range -> Für Verrechnung aus INFO-Antwort
// comma -> Anzahl Kommastellen für Anzeige Riedeltyp

long Genibus_Convert_Measure_Value(char choice, char data_hi, char data_lo, char unit, signed char zero, char range, char comma, char* ret)
	{	
		char	i;
		int		factor = 1;
		long result;
		float fl_result = 0;
	
		for (i=0; i<Genibus_Unit_Table_Size; i++)
			{
				if ( Genibus_Unit_Table[i].uc_index == unit )
					break;
			}
		if ( i < Genibus_Unit_Table_Size )										// Unit gefunden:																
			{
				switch ( choice )
					{
						case _8_BIT_VALUE:
							fl_result = ( ( (float)zero + ( (float)data_hi * (float)range/254 ) ) * Genibus_Unit_Table[i].fl_prefix);
							break;
							
						case _16_BIT_VALUE:
							fl_result = ( ( (float)zero + ( (float)data_hi * (float)range/254 ) + ( (float)data_lo * (float)range/65024) ) * Genibus_Unit_Table[i].fl_prefix);
							break;
						
						default:
							*ret = RX_CHOICE_ERROR;
							break;
					}
			}					
		else			// Unit nicht gefunden
			{
				*ret = RX_UNIT_ERROR;
			}
			
		if ( ( comma >= 1 ) && ( comma <= 4 ) )	
			{
				for ( i=0; i<comma; i++)
					factor *=10;
				result = (long)( fl_result * factor );
			}
		else
			result = (long)fl_result;
			
		return result;
	}
// ------------------------------------------------------------------------------------------------------------


// Änderung 131105 - neue Funktion zur Auswertung Extended
// ------------------------------------- Konvertierung Measure Value Extended ---------------------------------
// Übergabewerte:
// choice -> _32_BIT_VALUE / _24_BIT_VALUE / _16_BIT_VALUE / _8_BIT_VALUE
// data_hi -> Highbyte der empfangenen Daten 
// data_lo1 -> Lowbyte (nur bei 16-Bit-Wert, sonst UNUSED = 255 belegt für Anzeige eines 8-Bit-Wertes)
// data_lo2 -> Lowbyte (nur bei 24-Bit-Wert, sonst UNUSED = 255 belegt für Anzeige eines 8-Bit-Wertes)
// data_lo3 -> Lowbyte (nur bei 32-Bit-Wert, sonst UNUSED = 255 belegt für Anzeige eines 8-Bit-Wertes)
// unit -> Index der Unittable, der empfangen wurde
// zero -> Für Verrechnung aus INFO-Antwort
// comma -> Anzahl Kommastellen für Anzeige Riedeltyp
// ret -> Rückgabe des Fehlercodes

long Genibus_Convert_Measure_Value_Extended(char choice, char data_hi, char data_lo1, char data_lo2, char data_lo3, char unit, signed char zero, signed char zero_lo, char comma, char* ret)
	{	
		char	i;
		int		factor = 1;
		long  result;
		float fl_result = 0;
		int 	zero16 = 0;
	
		for (i=0; i<Genibus_Unit_Table_Size; i++)
			{
				if ( Genibus_Unit_Table[i].uc_index == unit )
					break;
			}
		if ( i < Genibus_Unit_Table_Size )										// Unit gefunden:																
			{
				zero16 = (int)zero << 8 + (int)zero_lo;
				
				switch ( choice )
					{
						case _8_BIT_VALUE:
							fl_result = ( (float)zero16 + ( (float)data_hi ) * Genibus_Unit_Table[i].fl_prefix );
							break;
							
						case _16_BIT_VALUE:
							fl_result = ( ( (float)zero16 + ( ( (float)data_hi * 256 ) + ( (float)data_lo1 ) ) ) * Genibus_Unit_Table[i].fl_prefix );
							break;
							
						case _24_BIT_VALUE:
							fl_result = ( ( ( (float)zero16 * 256 ) + ( ( (float)data_hi * 256 * 256 ) + ( (float)data_lo1 * 256) + ( (float)data_lo2 ) ) )* Genibus_Unit_Table[i].fl_prefix );
							break;
				
						case _32_BIT_VALUE:
							fl_result = ( ( ( (float)zero16 * 256 * 256 ) + ( ( (float)data_hi * 256 * 256 * 256 ) + ( (float)data_lo1 * 256 * 256 ) + ( (float)data_lo2 * 256 ) + ( (float)data_lo3 ) ) )* Genibus_Unit_Table[i].fl_prefix );
							break;														
						
						default:
							*ret = RX_CHOICE_ERROR;
							break;
					}
			}					
		else			// Unit nicht gefunden
			{
				*ret = RX_UNIT_ERROR;
			}
			
		if ( ( comma >= 1 ) && ( comma <= 4 ) )	
			{
				for ( i=0; i<comma; i++)
					factor *=10;
				result = (long)( fl_result * factor );
			}
		else
			result = (long)fl_result;
			
		return result;
	}
// ------------------------------------------------------------------------------------------------------------
// ENDE Neu nach Magna 3 Problem in Schwerin


// ##################################### SONDERFUNKTIONEN #####################################################

// ------------------------------------- Genibus Counter ------------------------------------------------------
// Prüft und verarbeitet alle Countervariablen 
void Genibus_Counter(void)
	{		
		if ( uc_genibus_rx_telegram_ready >= RECEIVING_TELEGRAM )				// Wartet 5 Taskzeiten, in denen keine weiteren Bytes in den RX-Buffer kamen
			{
				if ( uc_genibus_rx_telegram_ready != CHECK_NEW_TELEGRAM )
					++uc_genibus_rx_telegram_ready;
			}		
	}
// ------------------------------------------------------------------------------------------------------------	


// ------------------------------------- Control-Values senden ------------------------------------------------
// Stellt ein Telegramm zum Einstellen der Pumpe zusammen entsprechend des Übergabewertes
char Genibus_Set_Control_Value(char func, char dev_idx)
	{
		unsigned char ret_error = 0;
		
		genibus_manual_request.uc_geni_class 	= COMMANDS;			// Vorbelegung: Andere auch möglich, dann in case eintragen
		genibus_manual_request.uc_geni_op			= GENI_SET;
		genibus_manual_request.uc_id_lo				= 0xFF;      
		
		if ( dev_idx < GENI_DEVICE_MAX )
			genibus_manual_request.uc_adrr = genibus_device_table[dev_idx].uc_adr+31;
		else
			ret_error = SET_DEV_IDX_ERROR;
					
		switch (func)
			{
				case GENI_REMOTE:
					genibus_manual_request.uc_id_hi = GENI_remote;
					break;
				case GENI_START_PUMP:
					genibus_manual_request.uc_id_hi = GENI_start;
					break;
				case GENI_STOP_PUMP:
					genibus_manual_request.uc_id_hi = GENI_stop;
				  break; 	
				case GENI_MIN_CMD:
					genibus_manual_request.uc_id_hi = GENI_min;
					break;
				case GENI_MAX_CMD:
					genibus_manual_request.uc_id_hi = GENI_max;
					break;
				case GENI_CONSTPRESS:
					genibus_manual_request.uc_id_hi = GENI_const_press;
					break;
				case GENI_PROPRESS:
					genibus_manual_request.uc_id_hi = GENI_prop_press;
					break;
				case GENI_CONSTFREQ:
					genibus_manual_request.uc_id_hi = GENI_const_freq;
					break;
				case GENI_AUTOMATIC:
					genibus_manual_request.uc_id_hi = GENI_automatic;
					break;
				default:
					ret_error = SET_WRONG_FUNC_ERROR;
					break;
			}
		
		if ( ret_error == 0 )															// Kein Fehler bei Werteauswahl?
			uc_genibus_start_manual_request = 0xFF;						// Senden eines Telegrammes mit den festgelegten Werten starten
	
		return ret_error;
	}

// ------------------------------------------------------------------------------------------------------------	


// ------------------------------------- Buffer Hexwerte in Ascii wandeln -------------------------------------
// Wandelt die in count übergebene Anzahl an Bytes des Source_Buffer in Ascii um und speichert in Dest_Buffer                                                                                                                                                                           
void ConvertHexBufferToAscii(char* source_buffer, char* dest_buffer, unsigned char count)                                                                                                                     
	{       
		if ( count > 8 )			// Begrenzung durch 16 Byte Ascii Anzeige
			count = 8;
		
		memset(dest_buffer, ' ',16);		// Zielbuffer leeren
		
		if ( count > 0 )
			{
				dest_buffer[0]	= (source_buffer[0]>>4) < 0x0A ? 0x30+(source_buffer[0]>>4) : 0x37+(source_buffer[0]>>4);       
				dest_buffer[1]	= (source_buffer[0]&0x0F) < 0x0A ?  0x30+(source_buffer[0]&0x0F) : 0x37+(source_buffer[0]&0x0F);
			}	
		if ( count > 1 )
			{			                                                                                                                           			                                                                 
				dest_buffer[2]	= (source_buffer[1]>>4) < 0x0A ?  0x30+(source_buffer[1]>>4) : 0x37+(source_buffer[1]>>4);                                                                       
				dest_buffer[3]	= (source_buffer[1]&0x0F) < 0x0A ?  0x30+(source_buffer[1]&0x0F) : 0x37+(source_buffer[1]&0x0F); 
			} 
		if ( count > 2 )
			{                                                               
				dest_buffer[4]	= (source_buffer[2]>>4) < 0x0A ?  0x30+(source_buffer[2]>>4) : 0x37+(source_buffer[2]>>4);                                                                       
				dest_buffer[5]	= (source_buffer[2]&0x0F) < 0x0A ?  0x30+(source_buffer[2]&0x0F) : 0x37+(source_buffer[2]&0x0F); 
			}
		if ( count > 3 )
			{				                                                                
				dest_buffer[6]	= (source_buffer[3]>>4) < 0x0A ?  0x30+(source_buffer[3]>>4) : 0x37+(source_buffer[3]>>4);                                                                       
				dest_buffer[7]	= (source_buffer[3]&0x0F) < 0x0A ?  0x30+(source_buffer[3]&0x0F) : 0x37+(source_buffer[3]&0x0F);
			}
		if ( count > 4 )
			{					                                                                 
				dest_buffer[8]	= (source_buffer[4]>>4) < 0x0A ?  0x30+(source_buffer[4]>>4) : 0x37+(source_buffer[4]>>4);                                                                     
				dest_buffer[9]	= (source_buffer[4]&0x0F) < 0x0A ?  0x30+(source_buffer[4]&0x0F) : 0x37+(source_buffer[4]&0x0F); 
			}
		if ( count > 5 )
			{	                                                                                                                           
				dest_buffer[10]	= (source_buffer[5]>>4) < 0x0A ?  0x30+(source_buffer[5]>>4) : 0x37+(source_buffer[5]>>4);                                                                                                                                    
				dest_buffer[11]	= (source_buffer[5]&0x0F) < 0x0A ?  0x30+(source_buffer[5]&0x0F) : 0x37+(source_buffer[5]&0x0F);   
			}
		if ( count > 6 )
			{					                                                                                                                           
				dest_buffer[12]	= (source_buffer[6]>>4) < 0x0A ?  0x30+(source_buffer[6]>>4) : 0x37+(source_buffer[6]>>4);                                                                                                                                    
				dest_buffer[13]	= (source_buffer[6]&0x0F) < 0x0A ?  0x30+(source_buffer[6]&0x0F) : 0x37+(source_buffer[6]&0x0F);
			}
		if ( count > 7 )
			{					                                                                                                                              
				dest_buffer[14]	= (source_buffer[7]>>4) < 0x0A ?  0x30+(source_buffer[7]>>4) : 0x37+(source_buffer[7]>>4);                                                                                                                                    
				dest_buffer[15]	= (source_buffer[7]&0x0F) < 0x0A ?  0x30+(source_buffer[7]&0x0F) : 0x37+(source_buffer[7]&0x0F);  
			}                                                                                                                            
	} 
// ------------------------------------------------------------------------------------------------------------		                                                                                                                                                                     


// ------------------------------------- Profil Identifier ermitteln ------------------------------------------
// Profil Identifier aus den Pumpeninfos Unit Family, Unit Type und Unit Version ermitteln                                                                                                                                                                         
unsigned char get_profil_ident(unsigned char family, unsigned char type, unsigned char version)    
	{
		unsigned char profil_identifier = PROFIL2;			// Vorbelegen mit Profil 2, da das die häufigste Pumpe ist (Änderung vom 15.10.2015 auf Wunsch von ThoSi)
		
		switch ( family )
			{
				case 1:
					switch ( type )
						{
							case 1:
							case 2:
							case 4:
							case 5:
							case 6:
							case 7:
							case 8:														
								profil_identifier = PROFIL1;
								break;
								
							case 10:
								// Pumpe aus Schwerin? Welches Profil?
								break;
						}
					break;			
					
				case 2:
					switch ( type )
						{
							case 1:
							case 2:
							case 4:
							case 5:
							case 7:
							case 10:
								profil_identifier = PROFIL3;
								break;
							
							case 3:
								profil_identifier = PROFIL2;
								break;
						}
					break;
			}
			
		return profil_identifier;	
	}
// ------------------------------------------------------------------------------------------------------------	

#endif // GENI == 1