/********************************************************************
 *																																	*
 *					genibus_defines.h																				*
 *					Definitionen für verschiedene Genibusgeräte							*
 *					Version: 03.01.2013		anbu															*
 *																																	*
 ********************************************************************/

#if GENI == 1
	extern const GeniDeviceTable 	Genibus_Device_Table_Const[];
	extern const char 						Genibus_Device_Table_Size;
		
		
	extern const GeniRequestTable Genibus_Request_Table[];
	extern const char 						Genibus_Request_Table_Size;
	
	extern const GeniIndivReqTable 	Genibus_Indiv_Req_Table[];
	extern const char 							Genibus_Indiv_Req_Table_Size;
	
	extern const char Genibus_Unit_Request[];
	extern const char Genibus_Unit_Request_Size;

//	extern const char geni_units[];
//	extern const char UNIT_MAX;	
//	
//	extern const char geni_types[];
//	extern const char TYPE_MAX;
 
	extern const GeniUnitTable 	Genibus_Unit_Table[];
	extern const char 					Genibus_Unit_Table_Size;
	
#endif // GENI == 1