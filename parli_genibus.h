// Expansionsgruppe GBP			GENIBUS	
const Pgrup gbp[] = {
	{"***:"," GENIBUS        ","       ", P&Leer15,														ASCII_FORM, 0, P&vis,		V0, 0, 0},
	{" 00:"," PUMPE          "," # 1-8 ", P&uc_genibus_device_nr,						 EXPAND_CHAR, 0, P&vis,		V0, 0, EXP_GBP},

	{" 01;"," BUS EIN ?      "," 1=JA  ", P&BusPuPara[0].Funktion,							 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{"*01:"," BUS            ","       ", P&BusPuPara[0].Funktion,				 	PU_FUNC_FORM, 0, P&vis,		V0, 0, EXP_GBP},

	{"_02:"," BEZEICHNUNG    ","       ", P&BusPuPara[0].Name,								ASCII_FORM, 0, P&vis,		V1, 0, EXP_GBP},
	{"*03:"," PUMPENTYP      ","       ", P&genibus_device_table[0].ucf_name, ASCII_FORM, 0, P&vis,		V1, 0, EXP_GBP},

	{" 04;"," ADRESSE        ","       ", P&genibus_device_table[0].uc_adr, 	 	 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{"*->;"," adresse        ","       ", P&BusPuPara[0].Adresse,							 	 US_CHAR, 0, P&hid2,	V0, 0, EXP_GBP},

  {" 05;"," PROFIL         ","       ", P&genibus_device_table[0].uc_profile_ident,US_CHAR,0,P&hid1,V1, 0, EXP_GBP},

// Werte im Automatikbetrieb
	{"*06:"," BETR.-ART AUTO_","       ", P&BusPuPara[0].Betrieb,	  			 	 PU_OPR_FORM, 0, P&vis,		V0, 0, EXP_GBP},
	{" 07;"," BETR.-ART AUTO "," 0..3  ", P&BusPuPara[0].Betrieb,	 						 	 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},

	{"*08:"," REGELART AUTO_ ","       ", P&BusPuPara[0].Regelart, 		  	 	 PU_CTR_FORM, 0, P&vis,		V0, 0, EXP_GBP},
	{" 09;"," REGELART AUTO  "," 0..3  ", P&BusPuPara[0].Regelart,							 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},

	{" 10:"," SOLLWERT AUTO  "," %     ", P&BusPuPara[0].Sollwert,		 		 	 		  US_INT, 1, P&vis,		V1, 0, EXP_GBP},

	{" 12;"," RMELDG.-VERZOEG"," min   ", P&BusPuPara[0].RMverz,								 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{" ->;"," rm-verzoeg.ctr "," min/2 ", P&BusPuData[0].betriebRmCtr,					 	US_INT, 0, P&hid2,	V0, 0, EXP_GBP},


// Aktuelle Sollwerte
  {"*20:"," BETR.-ART SOLL_","       ", P&BusPuData[0].uc_betriebsart,		 	 				PU_OPR_FORM, 	0, P&vis,		V0, 0, EXP_GBP},
  {"*20;"," BETR.-ART SOLL "," 0-3   ", P&BusPuData[0].uc_betriebsart,		 	 				US_CHAR, 			0, P&hid2,	V1, 0, EXP_GBP},
  {"*21:"," BETR.-ART IST_ ","       ", P&Geni_Values[0].uc_operation_reply,	 	 		PU_OPR_FORM, 	0, P&vis,		V0, 0, EXP_GBP},  
  {"*21;"," BETR.-ART IST  "," 0-3   ", P&Geni_Values[0].uc_operation_reply,		 	 	US_CHAR, 			0, P&hid2,	V1, 0, EXP_GBP}, 
  
	{"*22:"," REGELART SOLL_ ","       ", P&BusPuData[0].uc_regelart,			 	 					PU_CTR_FORM, 	0, P&vis,		V0, 0, EXP_GBP},  
	{"*22;"," REGELART SOLL  "," 0-3   ", P&BusPuData[0].uc_regelart,			 	 					US_CHAR, 			0, P&hid2,	V1, 0, EXP_GBP},
	{"*23:"," REGELART IST_  ","       ", P&Geni_Values[0].uc_control_reply,		 	 		PU_CTR_FORM, 	0, P&vis,		V0, 0, EXP_GBP},	
	{"*23;"," REGELART IST   "," 0-3   ", P&Geni_Values[0].uc_control_reply,		 	 		US_CHAR, 			0, P&hid2,	V1, 0, EXP_GBP},

	{"*24:"," SOLLWERT AKT   "," %     ", P&BusPuData[0].ui_sollwert,		 		 	 		  	US_INT, 			1, P&vis,		V1, 0, EXP_GBP},
				    

// Von der Pumpe gelesene Werte (Komma und Konvertierung beachten in genibus_const-Tabelle)
	{"*30:"," DREHZAHL       "," 1/min ", P&Geni_Values[0].ui_speed, 									US_INT, 			0, P&vis,		V1, 0, EXP_GBP},
	{"*31:"," FOERDERHOEHE   "," m     ", P&Geni_Values[0].ui_height, 								US_INT,				2, P&vis,		V1, 0, EXP_GBP},
	{"*32:"," FOERDERSTROM   "," m/h  ", P&Geni_Values[0].ui_flow, 									US_INT,				1, P&vis,		V1, 0, EXP_GBP},
	{"*33;"," MEDIEN-TEMP.   "," C    ", P&Geni_Values[0].si_medium_temp, 						US_INT,				1, P&vis,		V1, 0, EXP_GBP},
	{"*34;"," LEISTUNG       "," W     ", P&Geni_Values[0].ui_power, 									US_INT,				0, P&vis,		V1, 0, EXP_GBP},
	{"*35:"," ENERGIEVERBR.  "," kWh   ", P&Geni_Values[0].ui_energy, 								US_INT,				0, P&vis,		V1, 0, EXP_GBP},
	{"*36;"," BETRIEBSZEIT   "," h     ", P&Geni_Values[0].ul_operating_hours, 				US_LONG, 			0, P&vis,		V1, 0, EXP_GBP},
                                                                                                	
	{"*39;"," ALARM-CODE     ","       ", P&Geni_Values[0].uc_alarm_code, 						US_CHAR, 			0, P&vis,		V1, 0, EXP_GBP},
	{"*40:"," LED-ANZEIGE    ","       ", P&Geni_Values[0].uc_led_control, 				 		DIGR_FORM, 		8, P&vis,		V1, 0, EXP_GBP},


// Störmeldungen
	{"*80:"," SM GB-PUMPE ?  ","       ", P&BusPuData[0].puAlarm,						 					JANEIN_FORM, 	0, P&vis,		V1, 0, EXP_GBP},
	{"*81:"," SM GENIBUS ?   ","       ", P&BusPuData[0].busSm,							 					JANEIN_FORM, 	0, P&vis,		V1, 0, EXP_GBP},
	{"*82:"," SM RM PUMPE ?  ","       ", P&BusPuData[0].betriebSm,					 					JANEIN_FORM, 	0, P&vis,		V1, 0, EXP_GBP},


// Werte für den Handbetrieb
	{"*89:"," HANDBETRIEB ?  ","       ", P&BusPuPara[0].Hand,		 					 JANEIN_FORM, 0, P&vis,		V1, 0, EXP_GBP},
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&BusPuPara[0].Hand,				 					 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{" ->;"," BETR.-ART HAND "," 0..3  ", P&BusPuPara[0].BetriebHand,					 	 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{" ->;"," REGELART  HAND "," 0..3  ", P&BusPuPara[0].RegelartHand,					 US_CHAR, 0, P&hid1,	V1, 0, EXP_GBP},
	{" ->;"," SOLLWERT HAND  "," %     ", P&BusPuPara[0].SollHand,		 						US_INT, 1, P&hid1,	V1, 0, EXP_GBP},

};

// Genibus Device Information Gruppe
// Expansionsgruppe GEN		Gruppe Genibus Pumpen	
const Pgrup gen[] = {
	{"***;"," Genibus Pumpen ","       ", P&Leer15,																		ASCII_FORM, 			0, 	P&hid4,		V0, 0, 0},	
                                                                                                  		
	{" 00;"," Device Number  "," # 1-8 ", P&uc_genibus_device_nr,						 					EXPAND_CHAR, 			0,  P&hid4,		V0, 0, EXP_GEN},
                                                                                                  		
	{" 01;"," Product Name   ","       ", P&genibus_device_table[0].ucf_name, 	 	 	 	ASCII_FORM, 			0, 	P&hid4,		V0, 0, EXP_GEN},
	{" 02;"," Device Adress  ","       ", P&genibus_device_table[0].uc_adr, 	 	 	 		US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{" 03;"," Group Adress   ","       ", P&genibus_device_table[0].uc_group_adr,	 		US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},	
	{" 04;"," Unit Family    ","       ", P&genibus_device_table[0].uc_unit_family,	 	US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{" 05;"," Unit Type      ","       ", P&genibus_device_table[0].uc_unit_type,	 		US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},	
 	{" 06;"," Unit Version   ","       ", P&genibus_device_table[0].uc_unit_version,	US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
  {" ->;"," Profile Ident  ","       ", P&genibus_device_table[0].uc_profile_ident,	US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
  {" 07;"," Function       ","       ", P&genibus_device_table[0].uc_function,			PU_FUNC_FORM, 		0, 	P&hid4,		V0, 0, EXP_GEN}, 
  {" ->;"," function       ","       ", P&genibus_device_table[0].uc_function,			US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN}, 
                                                                                                  		
	{" 23;"," Own Adress     ","       ", P&uc_genibus_own_adr, 	 	 	 								US_CHAR, 					0, 	P&hid4,		V0, 0, 0},
	
// Nochmal alle Werte in der Zusammenfassung mit Status	  	 	
	{"*40;"," Medien Temp    "," C    ", P&Geni_Values[0].si_medium_temp,						S_INT, 						1, 	P&hid4,		V0, 0, EXP_GEN},	  
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_medium_temp_stat,				US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*41;"," Foerderhoehe   "," m     ", P&Geni_Values[0].ui_height,				  				US_INT, 					2, 	P&hid4,		V0, 0, EXP_GEN},
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_height_stat,						US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*42;"," Flussmenge     "," m/h  ", P&Geni_Values[0].ui_flow,					  				US_INT, 					1, 	P&hid4,		V0, 0, EXP_GEN},
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_flow_stat,							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*43;"," Leistung       "," W     ", P&Geni_Values[0].ui_power,									US_INT, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_power_stat,							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*44;"," Drehzahl       "," rpm   ", P&Geni_Values[0].ui_speed,									US_INT, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_speed_stat,							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*45;"," Energieverbr   "," kWh   ", P&Geni_Values[0].ui_energy,				  				US_INT, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_energy_stat,						US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*46;"," Betriebsstunden"," h     ", P&Geni_Values[0].ul_operating_hours, 				US_LONG, 					0, 	P&hid4,		V0, 0, EXP_GEN},  
	{"*->;"," status         ","       ", P&Geni_Values[0].uc_operating_hours_stat,		US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},

	{"*50;"," Betriebsart Rep","       ", P&Geni_Values[0].uc_operation_reply, 				US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*51;"," Regelart Reply ","       ", P&Geni_Values[0].uc_control_reply, 					US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	
	{"*60;"," LED Control    ","       ", P&Geni_Values[0].uc_led_control, 						US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*60;"," Act Mode 1     ","       ", P&Geni_Values[0].uc_act_mode1, 							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*61;"," Act Mode 2     ","       ", P&Geni_Values[0].uc_act_mode2, 							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
	{"*62;"," Act Mode 3     ","       ", P&Geni_Values[0].uc_act_mode3, 							US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},		
	{"*63;"," Alarm Code     ","       ", P&Geni_Values[0].uc_alarm_code, 						US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},
		
	{"*->;"," uni status     ","       ", P&Geni_Values[0].uc_uni_stat,								US_CHAR, 					0, 	P&hid4,		V0, 0, EXP_GEN},	

}; 	

// Genibus Control Gruppe
const Pgrup gec[] = {	
	{"***;"," Genibus Control","       ", P&Leer15,																		ASCII_FORM, 	0, 	P&hid4,		V0, 0, 0},
	
	{" 01;"," Get Adress     "," Idx   ", P&uc_genibus_get_adress, 	 	 	 							US_CHAR, 			0, 	P&hid4,		V0, 0, 0},		
	                                                                        					         			   	        	
	{" 02;"," Reset Pump     "," Adr   ", P&uc_genibus_reset_pump, 	 	 	 							US_CHAR, 			0, 	P&hid4,		V0, 0, 0},		
	{" ->;"," reset result   ","       ", P&uc_genibus_reset_result, 	 	 	 						US_CHAR, 			0, 	P&hid4,		V0, 0, 0},

	{" 03;"," Start Man Req  "," >1    ", P&uc_genibus_start_manual_request, 	 	 	 		US_CHAR, 			0, 	P&hid4,		V0, 0, 0},					
	{" ->;"," Manual Adress  ","       ", P&genibus_manual_request.uc_adrr, 	 	 	 		US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	{" ->;"," Manual Class   ","       ", P&genibus_manual_request.uc_geni_class, 	 	US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," Manual Operat  "," 1=GET ", P&genibus_manual_request.uc_geni_op, 	 			US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," Manual ID hi   ","       ", P&genibus_manual_request.uc_id_hi, 	 	 	 		US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," Manual ID lo   ","       ", P&genibus_manual_request.uc_id_lo, 	 	 	 		US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{"[->;"," Manual Answer  ","       ", P&genibus_manual_request.ucf_answer, 	 	 	 	HEXBUF_FORM, 	0, 	P&hid4,		V0, 0, 0},
		
	{" 10;"," RX Buffer      ","       ", P&ucf_genibus_rx_buffer, 	 	 	 							HEXBUF_FORM, 	0, 	P&hid4,		V0, 0, 0},
	{" ->;"," rx error       ","       ", P&uc_genibus_rx_error, 	 	 	 								US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	{" ->;"," rx count       ","       ", P&uc_genibus_rx_count, 	 	 	 								US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	{" ->;"," rx timeout ctr ","       ", P&uc_genibus_rx_timeout_ctr, 	 	 	 					US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," rx telegr ready","       ", P&uc_genibus_rx_telegram_ready, 	 					US_CHAR, 			0, 	P&hid4,		V0, 0, 0},		
	{" ->;"," rx buf size    ","       ", P&uc_genibus_rx_buff_size, 	 	 	 						US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," rx errorflagSio","       ", P&uc_genibus_rx_errorflag, 	 	 	 						US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	
	{" 11;"," TX Buffer S2   ","       ", P&TxBuf_S2, 	 	 	 													HEXBUF_FORM, 	0, 	P&hid4,		V0, 0, 0},
	{" 12;"," TX Buffer S3   ","       ", P&TxBuf_S3, 	 	 	 													HEXBUF_FORM, 	0, 	P&hid4,		V0, 0, 0},	
	{" ->;"," tx count       ","       ", P&uc_genibus_tx_count, 	 	 	 								US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" ->;"," tx lenght      ","       ", P&uc_genibus_tx_lenght, 	 	 	 							US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	

	{" 20;"," State          ","       ", P&uc_genibus_state, 	 	 	 									US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	{" 21;"," Special Funct  ","       ", P&uc_genibus_special_function, 	 	 	 				US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" 22;"," Request Idx    ","       ", P&uc_genibus_request_table_idx, 	 	 	 			US_CHAR, 			0, 	P&hid4,		V0, 0, 0},		
	{" ->;"," relay ctr      ","       ", P&ui_genibus_relay_ctr, 	 	 	 							US_CHAR, 			0, 	P&hid4,		V0, 0, 0},	
	 	                                                                      					         			   	        	
	{" 30;"," RTS Prescaler  ","       ", P&uc_genibus_rts_prescaler, 	 	 						US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
	{" 31;"," RTS Timer      ","       ", P&uc_genibus_rts_timer, 	 	 	 							US_CHAR, 			0, 	P&hid4,		V0, 0, 0},

	{" 40;"," Func Test      ","       ", P&uc_genibus_func_test, 	 	 								US_CHAR, 			0, 	P&hid4,		V0, 0, 0},
};
