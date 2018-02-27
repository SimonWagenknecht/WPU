#ifndef COPYDATA_H_INCLUDED
#define COPYDATA_H_INCLUDED

	#if COPY_RESERV_EMPF > 0
		#if COPY_EMPF > 0
		{" ->;"," copyData1      ","       ", P&CopyData[0].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer1     ","       ", P&CopyData[0].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData1      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif 
	#endif
	
	#if COPY_RESERV_EMPF > 1
		#if COPY_EMPF > 1
		{" ->;"," copyData2      ","       ", P&CopyData[1].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer2     ","       ", P&CopyData[1].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData2      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif  
	#endif
	
	#if COPY_RESERV_EMPF > 2
		#if COPY_EMPF > 2
		{" ->;"," copyData3      ","       ", P&CopyData[2].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer3     ","       ", P&CopyData[2].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData3      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif 
	#endif
	
	#if COPY_RESERV_EMPF > 3
		#if COPY_EMPF > 3
		{" ->;"," copyData4      ","       ", P&CopyData[3].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer4     ","       ", P&CopyData[3].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData4      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 4
		#if COPY_EMPF > 4
		{" ->;"," copyData5      ","       ", P&CopyData[4].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer5     ","       ", P&CopyData[4].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData5      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
		
	#if COPY_RESERV_EMPF > 5
		#if COPY_EMPF > 5
		{" ->;"," copyData6      ","       ", P&CopyData[5].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer6     ","       ", P&CopyData[5].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData6      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 6
		#if COPY_EMPF > 6
		{" ->;"," copyData7      ","       ", P&CopyData[6].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer7     ","       ", P&CopyData[6].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData7      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 7
		#if COPY_EMPF > 7
		{" ->;"," copyData8      ","       ", P&CopyData[7].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer8     ","       ", P&CopyData[7].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData8      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif

	#if COPY_RESERV_EMPF > 8
		#if COPY_EMPF > 8
		{" ->;"," copyData9      ","       ", P&CopyData[8].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer9     ","       ", P&CopyData[8].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData9      ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif 
	#endif
	
	#if COPY_RESERV_EMPF > 9
		#if COPY_EMPF > 9
		{" ->;"," copyData10     ","       ", P&CopyData[9].copyData.messw,  ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer10    ","       ", P&CopyData[9].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData10     ","       ", P&LeerData[0], 									US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif  
	#endif
	
	#if COPY_RESERV_EMPF > 10
		#if COPY_EMPF > 10
		{" ->;"," copyData11     ","       ", P&CopyData[10].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer11    ","       ", P&CopyData[10].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData11     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif 
	#endif
	
	#if COPY_RESERV_EMPF > 11
		#if COPY_EMPF > 11
		{" ->;"," copyData12     ","       ", P&CopyData[11].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer12    ","       ", P&CopyData[11].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData12     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 12
		#if COPY_EMPF > 12
		{" ->;"," copyData13     ","       ", P&CopyData[12].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer13    ","       ", P&CopyData[12].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData13     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
		
	#if COPY_RESERV_EMPF > 13
		#if COPY_EMPF > 13
		{" ->;"," copyData14     ","       ", P&CopyData[13].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer14    ","       ", P&CopyData[13].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData14     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 14
		#if COPY_EMPF > 14
		{" ->;"," copyData15     ","       ", P&CopyData[14].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer15    ","       ", P&CopyData[14].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData15     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_EMPF > 15
		#if COPY_EMPF > 15
		{" ->;"," copyData16     ","       ", P&CopyData[15].copyData.messw, ANA_FORM, 0, P&hid2,	V1, 0, 0},
		{" ->;"," copyTimer16    ","       ", P&CopyData[15].copyRxTimer,			 US_INT, 0, P&hid2,	V0, 0, 0},  
		#else
		{" ->;"," leerData16     ","       ", P&LeerData[0],	 								US_CHAR, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	

//-----------------------------------------------------------------------------------------------------------	

	#if COPY_RESERV_SEND > 0
		#if COPY_SEND > 0
		{" ->;"," copySend1      ","       ", P&CopySend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend1      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 1
		#if COPY_SEND > 1
		{" ->;"," copySend2      ","       ", P&CopySend[1], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend2      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 2
		#if COPY_SEND > 2
		{" ->;"," copySend3      ","       ", P&CopySend[2], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend3      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 3
		#if COPY_SEND > 3
		{" ->;"," copySend4      ","       ", P&CopySend[3], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend4      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 4
		#if COPY_SEND > 4
		{" ->;"," copySend5      ","       ", P&CopySend[4], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend5      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 5
		#if COPY_SEND > 5
		{" ->;"," copySend6      ","       ", P&CopySend[5], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend6      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif	
	
	#if COPY_RESERV_SEND > 6
		#if COPY_SEND > 6
		{" ->;"," copySend7      ","       ", P&CopySend[6], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend7      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 7
		#if COPY_SEND > 7
		{" ->;"," copySend8      ","       ", P&CopySend[7], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend8      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif

	#if COPY_RESERV_SEND > 8
		#if COPY_SEND > 8
		{" ->;"," copySend9      ","       ", P&CopySend[8], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend9      ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 9
		#if COPY_SEND > 9
		{" ->;"," copySend10     ","       ", P&CopySend[9], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend10     ","       ", P&LeerSend[0], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 10
		#if COPY_SEND > 10
		{" ->;"," copySend11     ","       ", P&CopySend[10], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend11     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 11
		#if COPY_SEND > 11
		{" ->;"," copySend12     ","       ", P&CopySend[11], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend12     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 12
		#if COPY_SEND > 12
		{" ->;"," copySend13     ","       ", P&CopySend[12], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend13     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 13
		#if COPY_SEND > 13
		{" ->;"," copySend14     ","       ", P&CopySend[13], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend14     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif	
	
	#if COPY_RESERV_SEND > 14
		#if COPY_SEND > 14
		{" ->;"," copySend15     ","       ", P&CopySend[14], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend15     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif
	
	#if COPY_RESERV_SEND > 15
		#if COPY_SEND > 15
		{" ->;"," copySend16     ","       ", P&CopySend[15], 									S_INT, 0, P&hid2,	V1, 0, 0},
		#else
		{" ->;"," leerSend16     ","       ", P&LeerSend[0],	 									S_INT, 0, P&hid2,	V1, 0, 0},
		#endif
	#endif


#endif	// COPYDATA_H_INCLUDED
