#if WILO_MODBUS 
// Expansionsgruppe WLP
const Pgrup wlp[] = {

	{"***:"," WILO-MODBUS    ","       ", P&Leer15,																								ASCII_FORM,  0, P&vis,	V0, 0, 0},

	{" 00:"," PUMPE          "," # 1-8 ", P&cWiloPuCount,															 						EXPAND_CHAR, 0, P&vis,	V0, 0, EXP_WLP},

	{"_01:"," BEZEICHNUNG    ","       ", P&BusPuPara[0].Name,																		ASCII_FORM,  0, P&vis,	V1, 0, EXP_WLP},

	{" 02;"," BUS EIN ?      "," 1=JA  ", P&BusPuPara[0].Funktion,										 	 		 				 US_CHAR,  0, P&hid1,	V1, 0, EXP_WLP},

	{"*03:"," BUS            ","       ", P&BusPuPara[0].Funktion,															PU_FUNC_FORM,  0, P&vis,	V0, 0, EXP_WLP},
	{" 04;"," ADRESSE        ","       ", P&BusPuPara[0].Adresse,					 						 		 	 				 US_CHAR,  0, P&hid1,	V0, 0, EXP_WLP},
                                                                  			  					
	{"*05:"," BETR.-ART AUTO ","       ", P&BusPuPara[0].Betrieb,	  									 	 				 PU_OPR_FORM,  0, P&vis,	V0, 0, EXP_WLP},
	{" 06;"," BETR.-ART AUTO_"," 0..1  ", P&BusPuPara[0].Betrieb,	 				 						 		 	 				 US_CHAR,  0, P&hid1,	V1, 0, EXP_WLP},
                                                                  			  					
	{"*07:"," REGELART AUTO  ","       ", P&BusPuPara[0].Regelart, 										 	 				 PU_CTR_FORM,  0, P&vis,	V0, 0, EXP_WLP},
	{" 08;"," REGELART AUTO_ "," 0..3  ", P&BusPuPara[0].Regelart,										 	 		 				 US_CHAR,  0, P&hid1,	V1, 0, EXP_WLP},
                                                                  			  					
	{"*09:"," SOLLWERT AUTO  "," %     ", P&BusPuPara[0].Sollwert,										  							US_INT,  1, P&vis,	V1, 0, EXP_WLP},
                                                                  			  					
	{" 10;"," RMELDG.-VERZOEG"," min   ", P&BusPuPara[0].RMverz,											 	 		 				 US_CHAR,  0, P&hid1,	V1, 0, EXP_WLP},
	{" ->;"," rm-verzoeg.ctr "," min/2 ", P&BusPuData[0].betriebRmCtr,	 															US_INT,  0, P&hid2,	V0, 0, EXP_WLP},

	{"*20:"," BETR.-ART SOLL ","       ", P&BusPuData[0].betriebsArtSoll,	 										DYN_ASCII_FORM, 15, P&vis,	V1, 0, EXP_WLP},
	{"*->;"," betr.-art soll "," 0-1   ", P&modWiloPu[0].output_value1_temp,		 	 										US_INT,	 0, P&hid2,	V0, 0, EXP_WLP},
	{"*->;"," betr.soll wilo ","       ", P&modWiloPu[0].wiloHoldReg.mwspPumpCommand,								ANA_FORM,	 0, P&hid2,	V0, 0, EXP_WLP},
 
	{"*22:"," BETR.-ART IST  ","       ", P&BusPuData[0].betriebsArtIst,	 										DYN_ASCII_FORM, 15, P&vis,	V1, 0, EXP_WLP},
	{"*23;"," betr.-art ist  "," 0-1   ", P&modWiloPu[0].Operation_Input, 			 											US_INT,  0, P&hid2,	V0, 0, EXP_WLP},

	{"*24:"," REGELART SOLL  ","       ", P&BusPuData[0].regelArtSoll,	 											DYN_ASCII_FORM, 15, P&vis,	V1, 0, EXP_WLP},
	{"*->;"," regelart soll  "," 0-3   ", P&modWiloPu[0].output_value2_temp, 		 	 										US_INT,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*->;"," regel.soll wilo","       ", P&modWiloPu[0].wiloHoldReg.mwspOperationMode,							ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*26:"," REGELART IST   ","       ", P&BusPuData[0].regelArtIst,	 												DYN_ASCII_FORM, 15, P&vis,	V1, 0, EXP_WLP},
	{"*->;"," regelart ist   "," 0-3   ", P&modWiloPu[0].Control_Input,	 		 													US_INT,  0, P&hid2,	V0, 0, EXP_WLP},

	{"*28:"," SOLLWERT       "," %     ", P&modWiloPu[0].SetPoint_Input, 		 													US_INT,  0, P&vis,	V1, 0, EXP_WLP},
	{" 29;"," busCommandTimer","       ", P&BusPuPara[0].HoldingReg300,		 	 		 						 				 US_CHAR,  0, P&hid2,	V0, 0, EXP_WLP},

	{"*30:"," DREHZAHL       "," 1/min ", P&modWiloPu[0].wiloInReg.mwspSpeed, 											ANA_FORM,  0, P&vis,	V1, 0, EXP_WLP},
	{"*31:"," DIFFERENZDRUCK "," mWS   ", P&modWiloPu[0].wiloInReg.mwspActualDifferentialPressure, 	ANA_FORM,	 1, P&vis,	V1, 0, EXP_WLP},
	{"*32:"," FOERDERSTROM   "," m/h  ", P&modWiloPu[0].wiloInReg.mwspFlowRate, 										ANA_FORM,	 1, P&vis,	V1, 0, EXP_WLP},
	{"*33:"," MEDIEN-TEMP.   "," C    ", P&modWiloPu[0].siMediumTemperature, 											   S_INT,  1, P&vis,	V1, 0, EXP_WLP},
	{"*->;"," medien-temp.   "," K     ", P&modWiloPu[0].wiloInReg.mwspMediumTemperature, 					ANA_FORM,  1, P&hid2,	V0, 0, EXP_WLP},
	{"*34:"," LEISTUNG       "," W     ", P&modWiloPu[0].wiloInReg.mwspPowerRating, 								ANA_FORM,  0, P&vis,	V1, 0, EXP_WLP},
	{"*35:"," ENERGIEVERBR.  "," kWh   ", P&modWiloPu[0].wiloInReg.mwspPowerConsumption,						ANA_FORM,  0, P&vis,  V1, 0, EXP_WLP},
	{"*36:"," BETRIEBSZEIT   "," h     ", P&modWiloPu[0].op_hours, 							 										 US_LONG,  0, P&vis,  V1, 0, EXP_WLP},

	{"*40;"," SERVICE-MELDUNG","       ", P&modWiloPu[0].strServiceMessage,										DYN_ASCII_FORM, 15, P&hid1,	V1, 0, EXP_WLP},
	{"*->;"," Service Message","       ", P&modWiloPu[0].wiloInReg.mwspServiceMessage.messw,  		 DIGR_FORM, 16, P&hid2,	V1, 0, EXP_WLP},
	{"*41;"," ALARM-MELDUNG  ","       ", P&modWiloPu[0].strErrorType,		   	 								DYN_ASCII_FORM, 15, P&hid1,	V1, 0, EXP_WLP},
	{"*->;"," alarm-meldung  ","       ", P&modWiloPu[0].wiloInReg.mwspErrorType.messw,   				 DIGR_FORM, 16, P&hid2,	V1, 0, EXP_WLP},
	{"*42;"," ALARM-STATUS   ","       ", P&modWiloPu[0].strErrorMessage,   	 						 		DYN_ASCII_FORM, 15, P&hid1,	V1, 0, EXP_WLP},
	{"*->;"," alarm-status   ","       ", P&modWiloPu[0].wiloInReg.mwspErrorMessage.messw,   			 DIGR_FORM, 16, P&hid2,	V1, 0, EXP_WLP},
	{"*43;"," PUMPEN-STATUS  ","       ", P&modWiloPu[0].strPumpStatus,	    									DYN_ASCII_FORM, 15, P&hid1,	V1, 0, EXP_WLP},
	{"*->;"," pumpen-status  ","       ", P&modWiloPu[0].wiloInReg.mwspPumpStatus.messw,   				 DIGR_FORM, 16, P&hid2,	V1, 0, EXP_WLP},
	{"*44;"," STATUS DIAGNOSE","       ", P&modWiloPu[0].strStateDiagnostics,  								DYN_ASCII_FORM, 15, P&hid1,	V1, 0, EXP_WLP},
	{"*->;"," status diagnose","       ", P&modWiloPu[0].wiloInReg.mwspStateDiagnostics.messw,   	 DIGR_FORM, 16, P&hid2,	V1, 0, EXP_WLP},
	{"*45;"," mehrfach_msg   ","       ", P&modWiloPu[0].ucGlobalFlag,													 JANEIN_FORM,  0, P&hid2,	V1, 0, EXP_WLP},

	{"*50;"," Mains Current  "," A     ", P&modWiloPu[0].wiloInReg.mwspMainsCurrent,   							ANA_FORM,  1, P&hid2,	V0, 0, EXP_WLP},
	{"*51;"," Current Op-Mode","       ", P&modWiloPu[0].wiloInReg.mwspCurrentOperationMode,  			ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*52;"," Pump Module    ","       ", P&modWiloPu[0].wiloInReg.mwspPumpModule,  								ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*53;"," Pump Type      ","       ", P&modWiloPu[0].wiloInReg.mwspPumpType,  									ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*54;"," Max Speed      "," 1/min ", P&modWiloPu[0].wiloInReg.mwspMaxSpeed,  									ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*55;"," Min Speed      "," 1/min ", P&modWiloPu[0].wiloInReg.mwspMinSpeed,  									ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},
	{"*56;"," Max PWR Rating "," W     ", P&modWiloPu[0].wiloInReg.mwspMaxPowerRating,  						ANA_FORM,  0, P&hid2,	V0, 0, EXP_WLP},

	{"*64;"," Support.Errors ","       ", P&modWiloPu[0].wiloInReg.mwspSupportedErrors.messw,   	 DIGR_FORM, 16, P&hid2,	V0, 0, EXP_WLP},
	{"*65;"," Supported SM   ","       ", P&modWiloPu[0].wiloInReg.mwspSupportedServiceMessages,   DIGR_FORM, 16, P&hid2,	V0, 0, EXP_WLP},

	{"*80:"," SM WILO-PUMPE ?","       ", P&BusPuData[0].puAlarm,							  JANEIN_FORM, 0, P&vis, 	V1, 0, EXP_WLP},
	{"*81:"," SM MODBUS ?    ","       ", P&BusPuData[0].busSm,				 					JANEIN_FORM, 0, P&vis, 	V1, 0, EXP_WLP},
	{"*->;"," sm modbus war  ","       ", P&modWiloPu[0].busSmWar,				 					US_CHAR, 0, P&hid2,	V0, 0, EXP_WLP},
	{"*82:"," SM RM PUMPE ?  ","       ", P&BusPuData[0].betriebSm,		 					JANEIN_FORM, 0, P&vis, 	V1, 0, EXP_WLP},

	{"*89:"," HANDBETRIEB ?  ","       ", P&BusPuPara[0].Hand,		 			 				JANEIN_FORM, 0, P&vis,	V1, 0, EXP_WLP},
	{" 99;"," HAND/AUTOMATIK "," HAND=1", P&BusPuPara[0].Hand,				 		 	 				US_CHAR, 0, P&hid1,	V1, 0, EXP_WLP},
	{" ->;"," BETR.-ART HAND "," 0..3  ", P&BusPuPara[0].BetriebHand,		 	 					US_CHAR, 0, P&hid1,	V1, 0, EXP_WLP},
	{" ->;"," REGELART  HAND "," 0..3  ", P&BusPuPara[0].RegelartHand,		 	 				US_CHAR, 0, P&hid1,	V1, 0, EXP_WLP},
	{" ->;"," SOLLWERT HAND  "," %     ", P&BusPuPara[0].SollHand,		 							 US_INT, 1, P&hid1,	V1, 0, EXP_WLP},

};
#endif