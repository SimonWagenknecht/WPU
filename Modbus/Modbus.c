// Neue Modbus Implementierung
// JoJa - 15.08.2011
// joja 161101 READ_MULTIPLE_COILS
// josch 161108 modbusSioStopBit
// joja  170403 Zusammenfassung von Leseanfragen
// diro	 170601 Erweiterung der Kovnertierungen,
//							Fehlermeldung bei Konverterror im Slavemodus,
//							automatische Bestimmung des Modbusdatentypes im Slavemodus
// josch 170616 Parität und Stopbits für 3 Schnittstellen sind im System verankert. Zugriff erfolgt in der Gruppe ZLT
// joja  170830 Spezialkommando WRITE_SINGLE_COIL korrigiert, Indexfehler bei der Datenauswertung korrigiert
// joja  180406	Erweiterung der Funktion "modbusToFloat" um "case U_INT_64" und Funktion "floatToRiedel" bei "case ZE_FORMP:" um "case 0: // Zählerstand"
// joja  180626	Erweiterung der Funktion "U_Modbus" Registeranzahlkorrektur bei Coils und Funktion "floatToRiedel" bei "case NONE:"
// joja  180709 Überarbeitung von "S_INT_32" in "modbusToFloat" und Einführung von "SWAP_32" Konvertierung


#include "sfr32C87.h"
#include "defins.h"
#include "sio.h"
#include "sramext.h"
#include "snoinitext.h"

#include "string.h"
#include "struct.h"
#include <math.h>
#include "konvform.h"
#include "uskonvform.h"

#include "projdef.h"
#include "uramext.h"
#include "ustruct.h"
#include "userdef.h"
#include "modbusstanddef.h"
#include "uconstext.h"
#include "modbus.h"
#include <asmmacro.h>

#if MODBUS_UNI > 0

extern const Parli Pgruppe[];
extern char check_modb_rx_buffer(char *pRxBuf, char port);
extern void modb_sio_init(char port);
extern void modbus_request(char *pTxBuf, char port);
extern void Modb_Rx_Ready(char port);

void failure(void* address, DataType riedelType, void* oldAddress = NULL, unsigned char fallback = FALSE);

unsigned int exp10(unsigned int exp) {
	unsigned int returnVal = 1;
	while(exp > 0) {
		returnVal *= 10;
		exp -= 1;
	}
	return returnVal;
}
void generateShadowCopy(void* address, modbusTableRowSlaveData* rowData, DataType* type) {
		switch(type->type) {
			case ANA_FORMP:
				if((type->param & 0x80) == 0) {
					rowData->oldAddress = *((mwsp**)address);
					*((mwsp**)address) = (mwsp*)(&rowData->data[0]);
					(*((mwsp**)address))->stat = NICHTV;
				}
				break;
			case AOUT_FORMP:
					rowData->oldAddress = *((aasp**)address);
					// Wenn der Ausgang auf Dummy liegt muss seperater speicher vorgehalten werden
					if(&Aout_dummy[0] == (aasp*)rowData->oldAddress) {
						*((aasp**)address) = (aasp*)(&rowData->data[0]);
						(*((aasp**)address))->astat = NICHTV;
					}
				break;
			case JANEIN_FORMIP:
				rowData->oldAddress = *((dinpsp**)address);
				*((dinpsp**)address) = (dinpsp*)(&rowData->data[0]);
				(*((dinpsp**)address))->bstat = NICHTV;
				break;
		case JANEIN_FORMOP:
				rowData->oldAddress = *((doutsp**)address);
					// Wenn der Ausgang auf Dummy liegt muss seperater speicher vorgehalten werden
				if(&Dout_dummy[0] == (doutsp*)rowData->oldAddress) {
				*((doutsp**)address) = (doutsp*)(&rowData->data[0]);
				(*((doutsp**)address))->func = (*((doutsp**)rowData->oldAddress))->func;
//S				rowData->address = rowData->oldAddress;
				}
			break;
		}
}

void initModbusDataTable(void) {
	unsigned int i = 0;
	for(i = 0; i < modbusMasterTableSize; ++i) {
		modbusTableData[i].counter = 0;
		modbusTableData[i].retries = 0;
		modbusTableData[i].ready = FALSE;
		modbusTableData[i].autoSend = modbusTable[i].autoSend;
		generateShadowCopy(modbusTable[i].address, (modbusTableRowSlaveData*)&modbusTableData[i], &modbusTable[i].riedelType);
		// Wenn es sich um eine Leseoperation handelt werden die Status zu beginn Fehlerhaft initialisiert, damit nicht versehentlich mit den Daten gerechnet wird
		if(modbusTable[i].operation == READ_MULTIPLE_COILS || modbusTable[i].operation == READ_DISCRETE_INP || modbusTable[i].operation == READ_HOLDING_REGS ||modbusTable[i].operation == READ_INPUT_REGS) {
			failure(modbusTable[i].address, modbusTable[i].riedelType);
		}

	}

	for(i = 0; i < modbusSlaveTableSize; ++i) {
		modbusSlaveTableData[i].counter = 0;
		if(modbusSlaveTable[i].permission == READ_WRITE) {
			generateShadowCopy(modbusSlaveTable[i].address, &modbusSlaveTableData[i], &modbusSlaveTable[i].riedelType);
		}
	}
}


void Modbus(void) {

		static unsigned char firstRun = 1;
		char Time_Ptr[6];

		unsigned int i=0, j;
		unsigned int temp_time, outputkanal;

		if (firstRun == 1)
			{
				for(i = 0; i < 3; ++i) {
					activeRow[i] = -1;
				}
				initModbusDataTable();
				modbusDeviceNr = 1;
				firstRun = 2;
			}

	for(i = 0; i < modbusMasterTableSize; ++i) {
		++modbusTableData[i].counter;
		if(modbusTableData[i].autoSend != 0 && modbusTableData[i].counter / 20 >= modbusTable[i].interval && modbusTableData[i].ready == FALSE && modbusDeviceAddresses[modbusTable[i].deviceIndex] != 0) {
			modbusTableData[i].counter = 0;
			modbusTableData[i].ready = TRUE;
			modbusTableData[i].retries = 0;

		}
	}

	for(i = 0; i < modbusSlaveTableSize; ++i) {
		if(modbusSlaveTable[i].timeout > 0 && modbusSlaveTableData[i].counter / 600 >= modbusSlaveTable[i].timeout) {
			failure(modbusSlaveTable[i].address, modbusSlaveTable[i].riedelType, modbusSlaveTableData[i].oldAddress, modbusSlaveTable[i].useFallback);
			modbusSlaveTableData[i].status = DATERR;
		} else {
			++modbusSlaveTableData[i].counter;
		}
	}

}

void okStatus(void* address, DataType riedelType) {
	void* data;
	switch(riedelType.type) {
		case ANA_FORMP:
			data = *((mwsp**)address);
			((mwsp*)data)->stat = 0;
			break;
		case ANA_FORM:
			data = ((mwsp*)address);
			((mwsp*)data)->stat = 0;
			break;
		case AOUT_FORMP:
			data = *((aasp**)address);
			((aasp*)data)->astat = 0;
			break;
		case ANOUT_FORM:
			data = ((aasp*)address);
			((aasp*)data)->astat = 0;
			break;
		case JANEIN_FORMIP:
			data = *((dinpsp**)address);
			((dinpsp*)data)->bstat = 0;
			break;
		case JANEIN_FORMOP:
			data = *((doutsp**)address);
			((doutsp*)data)->func = 0;
			break;
	}
}

void failure(void* address, DataType riedelType, void* oldAddress, unsigned char fallback) {
	void* data;
	switch(riedelType.type) {
		case ANA_FORMP:
			if(fallback != FALSE) {
				*((mwsp**)address) = oldAddress;
			} else {
				data = *((mwsp**)address);
				((mwsp*)data)->stat = DATERR;
			}
			break;
		case ANA_FORM:
			data = ((mwsp*)address);
			((mwsp*)data)->stat = DATERR;
			break;
		case AOUT_FORMP:
			data = *((aasp**)address);
			((aasp*)data)->astat = DATERR;
			break;
		case ANOUT_FORM:
			data = ((aasp*)address);
			((aasp*)data)->astat = DATERR;
			break;
		case JANEIN_FORMIP:
			data = *((dinpsp**)address);
			((dinpsp*)data)->bstat = DATERR;
			break;
		case JANEIN_FORMOP:
			data = *((doutsp**)address);
			((doutsp*)data)->func = DATERR;
			break;
	}
}

void floatToModbus(float data, DataType* type, unsigned char* buffer, unsigned char offset, unsigned char command) {
	unsigned int i;
	unsigned char* dataBuf;
	if(command == WRITE_SINGLE_COIL) {
		if(data < 0.5) {
			buffer[offset] = 0;
		} else {
			buffer[offset] = 0xFF;
		}
		buffer[offset+1] = 0;
	} else {
		switch(type->type) {
			case U_INT_8:
				data *= exp10(type->param);
				if(command != WRITE_MULTIPLE_COILS) {
					buffer[offset] = 0;
					buffer[offset+1] = (unsigned char)(data);
				} else {
					buffer[offset] = (unsigned char)(data);
				}
				break;
	 		case S_INT_8:
				data *= exp10(type->param);
				if(command != WRITE_MULTIPLE_COILS) {
					buffer[offset] = 0;
					buffer[offset+1] = (char)(data);
				} else {
					buffer[offset] = (char)(data);
				}
				break;
			case U_INT_16:
				data *= exp10(type->param);
				buffer[offset] = (((unsigned int)data) >> 8) & 0x00FF;
				buffer[offset+1] = (((unsigned int)data)) & 0x00FF;
				break;
			case S_INT_16:
				data *= exp10(type->param);
				buffer[offset] = (((unsigned int)data) >> 8) & 0x00FF;
				buffer[offset+1] = (((unsigned int)data)) & 0x00FF;
				break;
			case U_INT_32:
				data *= exp10(type->param);
				buffer[offset] = (((unsigned long)data) >> 24) & 0x00FF;
				buffer[offset+1] = (((unsigned long)data) >> 16) & 0x00FF;
				buffer[offset+2] = (((unsigned long)data) >> 8) & 0x00FF;
				buffer[offset+3] = (((unsigned long)data)) & 0x00FF;
				break;
			case S_INT_32:
				data *= exp10(type->param);
				buffer[offset] = (((long)data) >> 24) & 0x00FF;
				buffer[offset+1] = (((long)data) >> 16) & 0x00FF;
				buffer[offset+2] = (((long)data) >> 8) & 0x00FF;
				buffer[offset+3] = (((long)data)) & 0x00FF;
				break;
			case U_INT_64:
				data *= exp10(type->param);
				for(i = 0; i < 4; ++i) {
					buffer[offset+i] = (((unsigned long long)data) >> (56-((i+offset)*8)));
				}
			break;
			case FLOAT:
				memcpy(&data, &buffer[0], 4);
				break;
			case BINARY:
				dataBuf = (unsigned char*)&data;
				for( i = 0; i < 4; ++i) {
					buffer[i+offset] = dataBuf[i];
				}
				break;
		}
	}
}
DataType riedelToModbus( DataType* type) {
	DataType resultType;

	resultType.type = KONVERT_ERROR;
	switch(type->type) {
				default:	// Error nicht behandelte Konverierungen
				break;
				case US_CHAR:
					resultType.type = U_INT_8;
					resultType.param = type->param;
					break;
				case GWOTAG2_FORM:	// auch für Wochentage z.B. Legionellen-Tage
				case GWOTAG_FORM:	// auch für Wochentage z.B. Legionellen-Tage
				case JANEIN_FORM:
				case JANEIN_FORMIP:
				case JANEIN_FORMOP:
				case DIGR_FORM:
				case DIGL_FORM:
				case MBUS_TYP_FORM:
					resultType.type = U_INT_8;
					resultType.param = 0;
					break;
				case S_CHAR:
					resultType.type = S_INT_8;
					resultType.param = type->param;
					break;
				case US_INT:
				case AOUT_FORMP:
				case ANOUT_FORM:
					resultType.type = U_INT_16;
					resultType.param = type->param;
					break;
				case S_INT:
				case ANA_FORM:
				case ANA_FORMP:
					if((type->param&0x80)==0) {
						// Messwert
						resultType.type = S_INT_16;
						resultType.param = type->param;
					} else {
						// Status
						resultType.type = U_INT_8;
						resultType.param = 0;
					}
					break;
				case US_LONG:
				case US_LONGP:
					resultType.type = U_INT_32;
					resultType.param = type->param;
					break;
				case MBUS_WMENG_FORM:
				case MBUS_VOL_FORM:
					resultType.type = U_INT_32;
					resultType.param = 0;
					break;
				case S_LONG:
					resultType.type = S_INT_32;
					resultType.param = type->param;
				break;
				case ZEIT_FORM:
				case UHR_FORM:
				case DATUM_FORM:
				case DATE_FORM:
				case TASK_FORM:
				case ASCII_FORM:
				case DYN_ASCII_FORM:
				case TELE_FORM:
				case EEP_ASCII_FORM:
					resultType.type = BINARY;
					resultType.param = 0;
				break;
				case ZE_FORMP:
					switch(type->param) {
						case 0:		// Darstellung Wert	und	Dimension
						case 42:	// Darstellung Hauptzähler Vorjahreswert
							resultType.type = U_INT_32;
							resultType.param = 0;
							break;
						case 1:		// Darstellung Name
						case 41:	// Darstellung Zählernummer
						case 37:	// Stichtagsdarstellung [0]Jahr,[1]Monat,[2]Tag
							resultType.type = BINARY;
							resultType.param = 0;
							break;
						case 38:	// MD Das Kennbyte des Zählermediums als UINT8 anzeigen
						case 2:		// Kommastellen
						case 3:		// Dimensionsnummer
//						case 9:		// Monatseinstellung für Sync. Verbrauch 1-12 aktuelles Jahr, 13-24 Vorjahr
//						case 36:	// Exponent
//		 				case 39:	// Impulswertdarstellung des Exponenten
							resultType.type = U_INT_8;
							resultType.param = 0;
		 					break;
						case 5:		// Aktueller Verbrauch(monatlich)
							resultType.type = U_INT_32;
							resultType.param = 0;
							break;
//						case 4:		// Teilerfaktor
//						case 6:		// Synchronisierwert Zähler
//						case 7:		// Synchronisierwert Verbrauch
//						case 8:		// Zwischenzähler
//						case 34:	// Konfiguration in Bitdarstellung
//						case 35:	// Fehlerbyte in Bitdarstellung
//						case 36:	// Exponent
//						case 40:	// Sync. Verbrauch 1-12 aktuelles Jahr, 13-24 Vorjahr
//						case 43:	// Synchronisierwert Zähler Vorjahr
							break;
						default:
							if(type->param > 9 && type->param < 34) {
								resultType.type = U_INT_32;
								resultType.param = 0;
							} else {
								// Error nicht behandelte Konvertierungen
							}
							break;
					}
				break;
		}
	 return resultType;

}
float modbusToFloat(unsigned char*buffer, DataType* type, unsigned char command, unsigned int offset = 0, unsigned int* numRows = NULL) {
	float tmpVal = 0;
	unsigned char* dataBuf = NULL;
	unsigned int i = 0;
	signed int si16 = 0;
	signed long si32 = 0;
	unsigned char dataType = type->type;
	
	if(command == READ_MULTIPLE_COILS) {
		//tmpVal = (unsigned char)buffer[3+offset];	joja 161101
		dataType = BINARY;
	}
	switch(dataType) {
	case U_INT_8:
		tmpVal = (unsigned char)buffer[4+offset];
		tmpVal /= exp10(type->param);
		break;
	case S_INT_8:
		tmpVal = (char)buffer[4+offset];
		tmpVal /= exp10(type->param);
		break;
	case U_INT_16:
		tmpVal =  ((unsigned int)buffer[4+offset]  & 0x00FF);
		tmpVal += ((unsigned char)buffer[3+offset] & 0x00FF) << 8 ;
		tmpVal /= exp10(type->param);
		break;
	case S_INT_16:
		dataBuf = (unsigned char*)&si16;
		dataBuf[0] =  buffer[4+offset];
		dataBuf[1] =  buffer[3+offset];
		tmpVal =  si16;
		tmpVal /= exp10(type->param);
		break;
	case U_INT_32:
		tmpVal =  ((unsigned int)buffer[6+offset]  & 0x00FF);
		tmpVal += ((unsigned char)buffer[5+offset] & 0x00FF) << 8 ;
		tmpVal += ((unsigned int)buffer[4+offset]  & 0x00FF) << 16;
		tmpVal += ((unsigned char)buffer[3+offset] & 0x00FF) << 24 ;
		tmpVal /= exp10(type->param);
	case S_INT_32:
		dataBuf = (unsigned char*)&si32;
		dataBuf[0] =  buffer[6+offset];
		dataBuf[1] =  buffer[5+offset];
		dataBuf[2] =  buffer[4+offset];
		dataBuf[3] =  buffer[3+offset];
		tmpVal = si32;
		tmpVal /= exp10(type->param);
		break;
	case U_INT_64:
		tmpVal = 0;
		for(i = 0; i < 8; ++i) {
			tmpVal += ((unsigned char)buffer[3+offset+i] & 0x00FF) << (56-(i*8));
		}
		tmpVal /= exp10(type->param);
		if(numRows) {
			*numRows += 1;
		}
	break;
	case FLOAT:
		tmpVal = *(float*)(&buffer[3+offset]);
		//memcpy(&buffer[3], &tmpVal, 4);
		break;
	case BINARY:
		dataBuf = (unsigned char*)&tmpVal;
		for( i = 0; i < 4; ++i) {
			dataBuf[i] = buffer[i+3+offset];
		}
		break;
	}
	return tmpVal;
}

unsigned long int bswap32(unsigned long int a)
{
  a = ((a & 0x000000FF) << 24) |
      ((a & 0x0000FF00) <<  8) |
      ((a & 0x00FF0000) >>  8) |
      ((a & 0xFF000000) >> 24);
  return a;
}

void convert(float* data, unsigned char conv) {
	switch(conv) {
		case NONE:
			// do nothing
			break;
		case ENOCEAN_TEMP_1090:
			*data = ((255-*data)*(80.0/256.0))+10;
			break;
		case SWAP_32:
			*(unsigned long int*)data = bswap32(*(unsigned long int*)data);
			break;
	}
}

float riedelToFloat(void* address, DataType* type, unsigned int offset = 0) {
		void* dataAddr = NULL;
		float result = 0.0;
		unsigned char* resultAddress = (unsigned char*)(&result);
		unsigned int i = 0;
		char anz = 0;
		switch(type->type) {
			case US_CHAR:
			case GWOTAG2_FORM:	// auch für Wochentage z.B. Legionellen-Tage
			case GWOTAG_FORM:	// auch für Wochentage z.B. Legionellen-Tage
			case JANEIN_FORM:
			case DIGR_FORM:
			case DIGL_FORM:
				dataAddr = ((unsigned char*)address);
				result = *(unsigned char*)dataAddr;
				result /= exp10(type->param);
				break;
			case S_CHAR:
				dataAddr = ((unsigned char*)address);
				result = *(signed char*)dataAddr;
				result /= exp10(type->param);
				break;
			case US_INT:
				dataAddr = ((unsigned char*)address);
				result = *(unsigned int*)dataAddr;
				result /= exp10(type->param);
				break;
			case S_INT:
				dataAddr = ((unsigned char*)address);
				result = *(signed int*)dataAddr;
				result /= exp10(type->param);
				break;
			case US_LONG:
				dataAddr = ((unsigned char*)address);
				result = *(unsigned long*)dataAddr;
				result /= exp10(type->param);
				break;
			case US_LONGP:
				dataAddr = *((unsigned long**)address);
				result = *(unsigned long*)dataAddr;
				result /= exp10(type->param);
				break;
			case S_LONG:
				dataAddr = ((unsigned char*)address);
				result = *(signed long*)dataAddr;
				result /= exp10(type->param);
				break;
			case MBUS_WMENG_FORM:
				dataAddr = ((MbusDynam*)address);
				result = (((MbusDynam*)dataAddr)->wmeng);
				break;
			case MBUS_VOL_FORM:
				dataAddr = ((MbusDynam*)address);
				result = (((MbusDynam*)dataAddr)->volume);
				break;
			case MBUS_TYP_FORM:
				dataAddr = ((MbusStandard*)address);
				result = (((MbusStandard*)dataAddr)->Typ);
				break;
			case ANA_FORM:
				dataAddr = ((mwsp*)address);
				if((type->param & 0x80) != 0) {
					// Status
					result = (((mwsp*)dataAddr)->stat);
				} else {
					// Messwert
					result = (((mwsp*)dataAddr)->messw);
					result /= exp10(type->param);
				}
			break;
			case ANA_FORMP:
				dataAddr = *((mwsp**)address);
				if((type->param & 0x80) != 0) {
					// Status
					result = (((mwsp*)dataAddr)->stat);
				} else {
					// Messwert
					result = (((mwsp*)dataAddr)->messw);
					result /= exp10(type->param);
				}
			break;
			case AOUT_FORMP:
				dataAddr = *((aasp**)address);
				result = (((aasp*)dataAddr)->awert);
				result /= exp10(type->param);
			break;
			case ANOUT_FORM:
				dataAddr = ((aasp*)address);
				result = (((aasp*)dataAddr)->awert);
				result /= exp10(type->param);
			break;
			case JANEIN_FORMIP:
				dataAddr = *((dinpsp**)address);
				result = (((dinpsp*)dataAddr)->bwert);
			break;
			case JANEIN_FORMOP:
				dataAddr = *((doutsp**)address);
				result = (((doutsp*)dataAddr)->wert);
			break;
			case ZEIT_FORM:
				dataAddr = ((unsigned char*)address);
				resultAddress[0] = ((unsigned char*)dataAddr)[1];
				resultAddress[1] = ((unsigned char*)dataAddr)[0];
			break;
			case UHR_FORM:
				resultAddress[0] = Std;
				resultAddress[1] = Min;
			break;
			case DATUM_FORM:
				i = Jahr+2000;
				resultAddress[0] = (i >> 8) & 0xFF;
				resultAddress[1] = i & 0xFF;
				resultAddress[2] = Mon;
				resultAddress[3] = Tag;
			break;
			case DATE_FORM:
				dataAddr = ((unsigned char*)address);
				resultAddress[0] = ((unsigned char*)dataAddr)[0];
				resultAddress[1] = ((unsigned char*)dataAddr)[1];
				resultAddress[2] = ((unsigned char*)dataAddr)[2];
			break;
			case TASK_FORM:
				resultAddress[0] = noi_reanimate;
				resultAddress[1] = noi_sys_tasknum_copy;
				resultAddress[2] = noi_user_tasknum_copy;
			break;
			case ASCII_FORM:
			case DYN_ASCII_FORM:
			case TELE_FORM:
	//		case BINARY:
				dataAddr = ((unsigned char*)address);
				for ( i = 0; i < 4; ++i) {
					resultAddress[i] = ((unsigned char*)dataAddr)[offset+i];
				}
				//memcpy(&((unsigned char*)dataAddr)[offset], &result, 4);
				break;
			case ZE_FORMP:
				// Attribute noch nicht implementiert
				dataAddr = *((zaehlsp**)address);
				//i = 0;
				//result = basiswert_to_displaywert ( (((zaehlsp*)dataAddr)->zdim.komma), (((zaehlsp*)dataAddr)->zdim.dim_idx), (((zaehlsp*)dataAddr)->zwert), (char*)&i);
				switch(type->param) {
					case 0:
						result = ((zaehlsp*)dataAddr)->zwert;
						break;
					case 42:
						result = ((zaehlsp*)dataAddr)->zwert_vj;
						break;
					case 1:	// Darstellung Name
						for ( i = 0; i < 4; ++i) {
							resultAddress[i] = ((zaehlsp*)dataAddr)->zdim.zname[offset+i];
						}
						break;
					case 38:	// MD Das Kennbyte des Zählermediums als Text anzeigen
						result = ((zaehlsp*)dataAddr)->zdim.medium;
						break;
					case 41:	// Darstellung Zählernummer
						for ( i = 0; i < 4; ++i) {
							resultAddress[i] = ((zaehlsp*)dataAddr)->zdim.znumm[offset+i];
						}
						break;
					case 37:	// Stichtagsdarstellung [0]Jahr,[1]Monat,[2]Tag
						resultAddress[0] = ((zaehlsp*)dataAddr)->zdim.stich_tag[0];
						resultAddress[1] = ((zaehlsp*)dataAddr)->zdim.stich_tag[1];
						resultAddress[2] = ((zaehlsp*)dataAddr)->zdim.stich_tag[2];
						break;
					case 2:	// Kommastellen
						result = ((zaehlsp*)dataAddr)->zdim.komma;
						break;
					case 3:
						result = ((zaehlsp*)dataAddr)->zdim.dim_idx;
						break;
					case 5:
						result = ((zaehlsp*)dataAddr)->vwert;
						break;
					default:
						if(type->param > 9 && type->param < 34) {
							if(type->param < 22) { // aktuell
								result = ((zaehlsp*)dataAddr)->verbr_akt[type->param-10];
							} else {					// Vorjahr
								result = ((zaehlsp*)dataAddr)->verbr_akt[type->param-22];
							}
						}
						break;
				}
				//result = (((zaehlsp*)dataAddr)->zwert);
				//result /= pow(10, i);
			break;
			case EEP_ASCII_FORM:
				anz = min_w(4, type->param-(offset));
				bicbus(EEPADR,	(char	*)&result,	(int)address+offset, anz, BICRD);
			break;
	}
	 return result;
}

void floatToRiedel(float data, void* address, DataType* type, unsigned int offset = 0, unsigned char maxLength = 4) {
		void* dataAddr;
		unsigned int length = 4, i = 0;
		if(maxLength > 4) {
			maxLength = 4;
		}
		switch(type->type) {
			case US_CHAR:
			case GWOTAG2_FORM:	// auch für Wochentage z.B. Legionellen-Tage
			case JANEIN_FORM:
				dataAddr = ((unsigned char*)address);
				data *= exp10(type->param);
				*((unsigned char*)dataAddr) = (unsigned char)data;
				break;
			case US_INT:
				dataAddr = ((unsigned char*)address);
				data *= exp10(type->param);
				*((unsigned int*)dataAddr) = (unsigned int)data;
				break;
			case S_INT:
				dataAddr = ((unsigned char*)address);
				data *= exp10(type->param);
				*((signed int*)dataAddr) = (signed int)data;
				break;
			case US_LONG:
				dataAddr = ((unsigned char*)address);
				data *= exp10(type->param);
				*((unsigned long*)dataAddr) = (unsigned long)data;
				break;
			case S_LONG:
				dataAddr = ((unsigned char*)address);
				data *= exp10(type->param);
				*((signed long*)dataAddr) = (signed long)data;
				break;
			case MBUS_TYP_FORM:
				dataAddr = ((MbusStandard*)address);
				((MbusStandard*)dataAddr)->Typ = data;
				break;
			case ANA_FORMP:
				dataAddr = *((mwsp**)address);
				data *= exp10(type->param);
				((mwsp*)dataAddr)->messw = data;
				((mwsp*)dataAddr)->stat = 0;
				break;
			case ANA_FORM:
				dataAddr = ((mwsp*)address);
				data *= exp10(type->param);
				((mwsp*)dataAddr)->messw = data;
				((mwsp*)dataAddr)->stat = 0;
				break;
			case ANOUT_FORM:
				dataAddr = ((aasp*)address);
				data *= exp10(type->param);
				((aasp*)dataAddr)->awert = data;
				((aasp*)dataAddr)->astat = 0;
				break;
			case AOUT_FORMP:
				dataAddr = *((aasp**)address);
				data *= exp10(type->param);
				((aasp*)dataAddr)->awert = data;
				((aasp*)dataAddr)->astat = 0;
				break;
			case JANEIN_FORMIP:
				dataAddr = *((dinpsp**)address);
				((dinpsp*)dataAddr)->bwert = data;
				if(((dinpsp*)dataAddr)->bwert > 0) {
					((dinpsp*)dataAddr)->bwert = 1;
				}
				((dinpsp*)dataAddr)->bstat = 0;
				break;
			case JANEIN_FORMOP:
				dataAddr = *((doutsp**)address);
				((doutsp*)dataAddr)->wert = data;
				if(((doutsp*)dataAddr)->wert > 0) {
						((doutsp*)dataAddr)->wert = 1;
				}
				break;
			case ZEIT_FORM:
				dataAddr = ((unsigned char*)address);
				((unsigned char*)dataAddr)[0] = ((unsigned char*)&data)[1];
				((unsigned char*)dataAddr)[1] = ((unsigned char*)&data)[0];
				break;
			case UHR_FORM:
				Std = ((unsigned char*)&data)[0];
				Min = ((unsigned char*)&data)[1];
				break;
			case DATUM_FORM:
				i = (((unsigned char*)&data)[0] << 8) + ((unsigned char*)&data)[1];
				Jahr = i - 2000;
				Mon = ((unsigned char*)&data)[2];
				Tag = ((unsigned char*)&data)[3];
				break;
			case ASCII_FORM:
				length = strlen(&((unsigned char*)address)[offset]);
				for(i = 0; i < min_w(maxLength, length); ++i) {
					((unsigned char*)address)[offset+i] = ((unsigned char*)&data)[i];
				}
				break;
			case NONE:
				if(type->param != 0) {
					// maximale Länge nach Parameter begrenzen um bei kurzen Variablen nicht wild in den Speicher zu schreiben
					maxLength = type->param;
				}
				for(i = 0; i < maxLength; ++i) {
					((unsigned char*)address)[offset+i] = ((unsigned char*)&data)[i];
				}
			break;
		case EEP_ASCII_FORM:
			length = min_w(maxLength, type->param-(offset));
			bicbus(EEPADR,	(char	*)&data,	(int)address+offset, length, BICWR);
		break;
		case ZE_FORMP:
			// Attribute noch nicht implementiert
			dataAddr = *((zaehlsp**)address);
			switch(type->param) {
				case 0: // Zählerstand
					((zaehlsp*)dataAddr)->zwert = data;
					break;
				case 1:	// Darstellung Name
					length = strlen(&((zaehlsp*)dataAddr)->zdim.zname[offset]);
					for ( i = 0; i < min_w(maxLength, length); ++i) {
						((zaehlsp*)dataAddr)->zdim.zname[offset+i] = ((unsigned char*)&data)[i];
					}
					break;
				case 41:	// Darstellung Zählernummer
					length = strlen(&((zaehlsp*)dataAddr)->zdim.znumm[offset]);
					for ( i = 0; i < min_w(maxLength, length); ++i) {
						((zaehlsp*)dataAddr)->zdim.znumm[offset+i] = ((unsigned char*)&data)[i];
					}
					break;
			}
			break;
		}
}

int findNextRow(char port, signed int oldRow) {
	int rowIndex = 0;
	int i = 0;

	if(oldRow >= modbusMasterTableSize) {
		oldRow = modbusMasterTableSize-1;
	}
	for(i = 0; i < modbusMasterTableSize; ++i) {
		rowIndex = (i+1+oldRow) % modbusMasterTableSize;

		// Prüfen ob das Gerät an der entsprechenden Schnitstelle hängt
		if(port == modbusDevices[modbusTable[rowIndex].deviceIndex].sio || modbusDevices[modbusTable[rowIndex].deviceIndex].sio == SX) {
			// Prüfen ob die aktuelle Zeile schon wieder an der Reihe ist
			if(modbusTableData[rowIndex].ready != FALSE) {
				return rowIndex;
			}
		}
	}

	return -1;
}


unsigned char checkRegisters(unsigned char port, unsigned char command) {
	unsigned int numRegs = cnt_regs[port];
	unsigned int firstReg = first_reg[port];
	unsigned int length = 0;
	unsigned int i;
	while(numRegs > 0) {
		for( i = 0; i < modbusSlaveTableSize; ++i) {
			if(firstReg >= modbusSlaveTable[i].registerAddress && firstReg < ( modbusSlaveTable[i].registerAddress + modbusSlaveTable[i].numRegisters)) {
				if( ( (command == WRITE_MULTIPLE_HOLDINGS) || (command == WRITE_SINGLE_HOLDING) ) && modbusSlaveTable[i].permission == READ_ONLY ) {
					return FALSE;
				}
				length = ( modbusSlaveTable[i].numRegisters - (firstReg - modbusSlaveTable[i].registerAddress));
				if(length > numRegs) {
					length = numRegs;
				}
				numRegs -= length;
				firstReg += length;
				break;
			}
		}
		// Wenn die for-Schleife bis zur Abbruchbedingung durchlaufen wurde, konnte keine Übereinstimmung in der
		// Modbus-Slave-Tabelle gefunden werden. Somit sind die angefragten Register nicht gültig
		if( i == modbusSlaveTableSize) {
			return FALSE;
		}
	}
	return TRUE;
}

signed char readValues(unsigned char port) {
	unsigned int numRegs = cnt_regs[port];
	unsigned int firstReg = first_reg[port];
	unsigned int length = 0;
	unsigned int i;
	signed char result = 0;
	unsigned int byteCount = 0;
	float tmpVal = 0;
	unsigned int offset = 0;
	DataType modBusType;

	while(numRegs > 0) {
		offset = 0;
		for( i = 0; i < modbusSlaveTableSize; ++i) {
			if(firstReg >= modbusSlaveTable[i].registerAddress && firstReg < ( modbusSlaveTable[i].registerAddress + modbusSlaveTable[i].numRegisters)) {
				do {
					length = ( modbusSlaveTable[i].numRegisters - (firstReg - modbusSlaveTable[i].registerAddress));
					if(length == 0) {
						break;
					}
					// Maximal zwei Register gleichzeitig verarbeiten. Sonst schlägt die Konvertierung über den Float fehl!
					if(length > 2) {
						length = 2;
					}
					if(length > numRegs) {
						length = numRegs;
					}
					numRegs -= length;
					firstReg += length;
					tmpVal = riedelToFloat(modbusSlaveTable[i].address, &modbusSlaveTable[i].riedelType, offset);
					convert(&tmpVal, modbusSlaveTable[i].convert);
					modBusType = riedelToModbus(&modbusSlaveTable[i].riedelType);
					if(modBusType.type==KONVERT_ERROR) {
						ModBusKonvertError = modbusSlaveTable[i].registerAddress;		// Registeradresse des Tabelleneintrages merken
						result = -1;
						break;
					}
					//TODO: Hier nochmal nach dem command für floatToModbus gucken
					floatToModbus(tmpVal, &modBusType, modbusConvertBuffer[port], byteCount, 0);
					byteCount += length*2;
					offset += length*2;
				} while (length > 0);
				break;
			}
		}
	}
	return result;
}

signed char writeValues(unsigned char* buffer, unsigned char port) {
	unsigned int numRegs = cnt_regs[port];
	unsigned int firstReg = first_reg[port];
	unsigned int length = 0, realLength = 0;
	unsigned int i;
	signed char result = 0;
	unsigned int byteCount = 0;
	float tmpVal = 0;
	unsigned int offset = 0;
	unsigned int bufferOffset = 0;
	DataType modBusType;

	while(numRegs > 0) {

		bufferOffset = (func_code[port] == WRITE_SINGLE_HOLDING)?1:4;
		offset = 0;
		for( i = 0; i < modbusSlaveTableSize; ++i) {

			if(firstReg >= modbusSlaveTable[i].registerAddress && firstReg < ( modbusSlaveTable[i].registerAddress + modbusSlaveTable[i].numRegisters)) {
				if(modbusSlaveTableData[i].status != 0) {
					generateShadowCopy(modbusSlaveTable[i].address, &modbusSlaveTableData[i], &modbusSlaveTable[i].riedelType);
					modbusSlaveTableData[i].status = 0;
				}
				modbusSlaveTableData[i].counter = 0;
				offset = (firstReg - modbusSlaveTable[i].registerAddress)*2;
				do {
					realLength = ( modbusSlaveTable[i].numRegisters - (firstReg - modbusSlaveTable[i].registerAddress));
					if(realLength > numRegs) {
						realLength = numRegs;
					}
					length = realLength;
					if(length == 0) {
						break;
					}
					// Maximal zwei Register gleichzeitig verarbeiten. Sonst schlägt die Konvertierung über den Float fehl!
					if(length > 2) {
						length = 2;
					}
					if(length > numRegs) {
						length = numRegs;
					}
					numRegs -= length;
					firstReg += length;
					modBusType = riedelToModbus(&modbusSlaveTable[i].riedelType);
					if(modBusType.type==KONVERT_ERROR) {
						ModBusKonvertError = modbusSlaveTable[i].registerAddress;		// Registeradresse des Tabelleneintrages merken
						result = -1;
						break;
					}

					tmpVal = modbusToFloat(&buffer[bufferOffset+byteCount], &modBusType, func_code[port]);
					convert(&tmpVal, modbusSlaveTable[i].convert);
					floatToRiedel(tmpVal, modbusSlaveTable[i].address, &modbusSlaveTable[i].riedelType, offset, length*2);
					byteCount += length*2;
					offset += length*2;
					realLength -= length;
				} while (realLength > 0);

				break;
			}
		}
	}

	return result;
}


char U_Modbus(char port, char funktion) {

	// Statische Variablen
	static unsigned char state[3] = {0,0,0};
	static unsigned int counter[3] = {0,0,0};
	static unsigned int startRow[3] = {0,0,0};

	char fu_ret = funktion;		// Ok Vorbelegung
	char *pRxBuf = NULL;
	char *pTxBuf = NULL;
	float tmpVal;
	unsigned int tmpRow = 0, offset = 0, tmpRegCount = 0, i = 0, j=0, numRegs = 0;

	unsigned char slaveMode = ((Funktion_S1 == MODBUS1_FU && Mode_S1 == SLAVE) && port == S1) || ((Funktion_S2 == MODBUS1_FU && Mode_S2 == SLAVE) && port == S2) || ((Funktion_S3 == MODBUS1_FU && Mode_S3 == SLAVE) && port == S3);

	Modbus();

	switch(port)
	{
		case S1:
			pRxBuf  = RxBuf_S1;
			pTxBuf  = TxBuf_S1;
			break;
		case S2:
			pRxBuf  = RxBuf_S2;
			pTxBuf  = TxBuf_S2;
			break;
		case S3:
			pRxBuf  = RxBuf_S3;
			pTxBuf  = TxBuf_S3;
			break;
	}


	switch(state[port]) {
		/*
		 * Initialisierung der Schnittstelle
		 */
		case 0:
			modb_sio_init(port);
			switch(port)
			{
				case S1:
					if(Parity_S1 == 1) {
						MR_S1 |= 0x40;
					} else if(Parity_S1 == 2) {
						MR_S1 |= 0x60;
					}
					if(Stopbits_S1 == 2)			
						MR_S1 |= 0x10;
					break;

				case S2:
					if(Parity_S2 == 1) {
						MR_S2 |= 0x40;
					} else if(Parity_S2 == 2) {
						MR_S2 |= 0x60;
					}
					if(Stopbits_S2 == 2)
						MR_S2 |= 0x10;
					break;

				case S3:
					if(Parity_S3 == 1) {
						MR_S3 |= 0x40;
					} else if(Parity_S3 == 2) {
						MR_S3 |= 0x60;
					}
					if(Stopbits_S3 == 2)
						MR_S3 |= 0x10;
					break;
			}
			Modb_Rx_Ready(port);
			state[port] = 1;
			activeRow[port] = findNextRow(port, -1);
			break;

		/*
		 * Erzeugen einer Anfrage für den nächsten Listeneintrag
		 */
		case 1:
			if(slaveMode) {
				modb_curr_adr[port] = modbusSlaveAddresses[port];
				// Status des Empfangs bestimmen
				modb_rx_status = check_modb_rx_buffer(pRxBuf, port);
				switch(modb_rx_status) {
					/*
				 	*  Datenübertragung war erfolgreich.
				 	*  Telegramm auswerten, ggf. Daten übernehmen und Antwort erzeugen
				 	*/
					case RX_BUF_READY:
						// Verarbeitung nur wenn die Adresse stimmt
						if(pRxBuf[0] == modbusSlaveAddresses[port]) {
							// Daten nur übernehmen wenn es ein Schreibbefehl war.
							func_code[port] = pRxBuf[1];
							switch(func_code[port]) {
								case WRITE_SINGLE_HOLDING:
									first_reg[port] = pRxBuf[3] + ((unsigned int)pRxBuf[2] << 8);
									cnt_regs[port] = 1;
									break;
								case WRITE_MULTIPLE_HOLDINGS:
									first_reg[port] = pRxBuf[3] + ((unsigned int)pRxBuf[2] << 8);
									cnt_regs[port] = pRxBuf[5] + ((unsigned int)pRxBuf[4] << 8);
									break;
								case READ_HOLDING_REGS:
									first_reg[port] = pRxBuf[3] + ((unsigned int)pRxBuf[2] << 8);
									cnt_regs[port] = pRxBuf[5] + ((unsigned int)pRxBuf[4] << 8);
									break;
								case READ_INPUT_REGS:
									first_reg[port] = pRxBuf[3] + ((unsigned int)pRxBuf[2] << 8);
									cnt_regs[port] = pRxBuf[5] + ((unsigned int)pRxBuf[4] << 8);
									break;
								default:
									func_code[port] |= 0x80;
									byte_count_or_exception[port] = ILLEGAL_FUNCTION;
									break;
							}
							if(cnt_regs[port] > 61) {
									func_code[port] |= 0x80;
									byte_count_or_exception[port] = ILLEGAL_DATA_ADDRESS;
							}
							// Falls noch kein Fehler vorliegt werden die Registeradressen überprüft
							if((func_code[port] & 0x80) == 0) {
								if(checkRegisters(port, func_code[port]) != TRUE) {
									func_code[port] |= 0x80;
									byte_count_or_exception[port] = ILLEGAL_DATA_ADDRESS;
								}
							}

							// Falls noch kein Fehler vorliegt können die Daten übernommen oder das Senderegister gefüllt werden
							if((func_code[port] & 0x80) == 0) {
								byte_count_or_exception[port] = cnt_regs[port]*2;
								switch(func_code[port]) {
									case WRITE_SINGLE_HOLDING:
									case WRITE_MULTIPLE_HOLDINGS:
										if(writeValues(pRxBuf, port)>=0) {
											byte_count_or_exception[port] = 0;
											reg_address[port] = &pRxBuf[4];
										} else {
											func_code[port] |= 0x80;
											byte_count_or_exception[port] = SLAVE_DEVICE_FAILURE;
										}

										break;
									case READ_HOLDING_REGS:
									case READ_INPUT_REGS:
										if(readValues(port)>=0) {
											reg_address[port] = &modbusConvertBuffer[port][0];
										} else {
											func_code[port] |= 0x80;
											byte_count_or_exception[port] = SLAVE_DEVICE_FAILURE;
										}
										break;
									default:
										break;
								}
							}
							modbus_request(pTxBuf, port);
						}
					break;

					/*
				 	 *	Keine Daten vorhanden oder es ist ein Fehler aufgetreten. Empfang wieder aktivieren
				 	 */
					case 0:
					case SoF_ERROR:
					case CRC_ERROR:
					default:
						Modb_Rx_Ready(port);
					break;
				}
			} else {
				/*
				 * Master Mode
				 */
				if(activeRow[port] != -1) {
					// Adresse des ersten Registers
					first_reg[port] = modbusTable[activeRow[port]].registerAddress;

					// Funktion
					func_code[port] = modbusTable[activeRow[port]].operation;
					// Adresse des Modbus Gerätes
					modb_curr_adr[port] = modbusDeviceAddresses[modbusTable[activeRow[port]].deviceIndex];
					// Speicheradresse der Daten im R66
					reg_address[port] = &modbusConvertBuffer[port][0];

					// Bei Schreibeanfragen den modbusConvertBuffer mit Daten füllen
					tmpRow = activeRow[port];
					startRow[port] = tmpRow;
					if(modbusTable[tmpRow].operation == WRITE_SINGLE_COIL) {
						tmpRegCount	= 0;
					}
					if(modbusTable[tmpRow].operation == WRITE_SINGLE_COIL || modbusTable[tmpRow].operation == WRITE_SINGLE_HOLDING || modbusTable[tmpRow].operation == WRITE_MULTIPLE_COILS ||modbusTable[tmpRow].operation == WRITE_MULTIPLE_HOLDINGS) {
						tmpRegCount	= 0;
						offset = 0;
						do {
							for(i = 0; i < (modbusTable[tmpRow].numRegisters/2)+modbusTable[tmpRow].numRegisters%2; ++i) {
								tmpVal = riedelToFloat(modbusTable[tmpRow].address, &modbusTable[tmpRow].riedelType, i*4);
								convert(&tmpVal, modbusTable[tmpRow].convert);
								floatToModbus(tmpVal, &modbusTable[tmpRow].modbusType, modbusConvertBuffer[port], i*4+offset, modbusTable[tmpRow].operation);
							}
							tmpRegCount += modbusTable[tmpRow].numRegisters;
							offset += 2*modbusTable[tmpRow].numRegisters;
							activeRow[port] = tmpRow;
							tmpRow = findNextRow(port, tmpRow);
						}	while( tmpRow < (modbusMasterTableSize) && modbusTable[tmpRow-1].operation == WRITE_MULTIPLE_HOLDINGS && modbusTable[tmpRow-1].registerAddress + modbusTable[tmpRow-1].numRegisters == modbusTable[tmpRow].registerAddress );

					} else {
						// Auch für Lesekommandos können Daten zusammengefasst werden
						tmpRegCount = modbusTable[tmpRow].numRegisters; // Anzahl der Register die gelesen werden sollen
						// Anzahl der gleichzeitig zu lesenden Elemente bestimmen. Es werden maximal 40 Register auf einen Schlag gelesen
						// Die Einträge können nur zusammen gefasst werden, wenn sie in der Tabelle direkt hintereinander liegen und kein anderes Gerät dazwischen liegt.
						for(i = 0; i < modbusMasterTableSize && tmpRegCount < 40; ++i) {
							tmpRow = (tmpRow+1) % modbusMasterTableSize;
							// Prüfen, ob die Zeilen zum gleichen Gerät gehören
							if(modbusTable[tmpRow].deviceIndex == modbusTable[activeRow[port]].deviceIndex) {
								// Prüfen ob die aktuelle Zeile schon wieder an der Reihe ist
								if(modbusTableData[tmpRow].ready != FALSE) {
									// Prüfen ob die aktuelle Zeile sich direkt an die vorherige anschließt und das gleiche Kommando zum lesen der Daten verwendet wird
									if(modbusTable[activeRow[port]].registerAddress +	modbusTable[activeRow[port]].numRegisters == modbusTable[tmpRow].registerAddress && modbusTable[activeRow[port]].operation == modbusTable[tmpRow].operation) {
										tmpRegCount += modbusTable[tmpRow].numRegisters; // Anzahl der zu lesenden Register anpassen
										activeRow[port] = tmpRow;
									} else {
										break;
									}
								}
							} else {
								// Wenn ein anderes Gerät in der Tabelle dazwischen liegt können die Daten nicht mehr zusammen gefasst werden
								break;
							}
						}
						//tmpRegCount = modbusTable[activeRow[port]].numRegisters;
					}

					// Anzahl der Register
					cnt_regs[port]  = tmpRegCount;

					// Anfrage senden
					modbus_request(pTxBuf, port);
					// Weiter mit Antwort auswerten
					state[port] = 2;
				} else {
					// Wenn gerade nichts zu tun ist kann nach dem nächten gültigen Eintrag gesucht werden
					activeRow[port] = findNextRow(port, -1);
				}
			}
		break;

		/*
		 * Auswerten der Antwort
		 */
		case 2:
			if(!slaveMode) {
				// Status des Empfangs bestimmen
				modb_rx_status = check_modb_rx_buffer(pRxBuf, port);
				switch(modb_rx_status) {
					/*
				 	*	Es ist noch keine Antwort eingetroffen.
				 	*	Die maximale Wartezeit beträgt 1 Sekunde
				 	*/
					case 0:
						++counter[port];
						if(counter[port] > 20) {
							counter[port] = 0;
							state[port] = 1;
							for(tmpRow = startRow[port]; tmpRow <= activeRow[port]; ++tmpRow) {
								++modbusTableData[tmpRow].retries;

								if(modbusTableData[tmpRow].retries > 2) {
									failure(modbusTable[tmpRow].address, modbusTable[tmpRow].riedelType);
									modbusTableData[tmpRow].retries = 0;
									modbusTableData[tmpRow].ready = FALSE;
								}
							}
							if(modbusTableData[tmpRow-1].retries == 0) {
								activeRow[port] = findNextRow(port, tmpRow-1);
							} else {
								activeRow[port] = startRow[port];
							}
						}
					break;

					/*
				 	*  Datenübertragung war erfolgreich.
				 	*  Daten können konvertiert und gespeichert werden
				 	*	Anschließend wird mit der nächten Zeile der ModbusTable fortgefahren
				 	*/
					case RX_BUF_READY:
						offset = 0;
						for(tmpRow = startRow[port]; tmpRow <= activeRow[port]; ++tmpRow) {
							// Daten nur übernehmen wenn es ein Leseauftrag war.
							// Status ggf auf ok setzten
							okStatus(modbusTable[tmpRow].address, modbusTable[tmpRow].riedelType);
							if(modbusTable[tmpRow].operation == READ_MULTIPLE_COILS || modbusTable[tmpRow].operation == READ_DISCRETE_INP || modbusTable[tmpRow].operation == READ_HOLDING_REGS ||modbusTable[tmpRow].operation == READ_INPUT_REGS) {
								// Über einen Float werden die Daten zwischengespeichert.
								// Darauf lassen sich die meisten Datentypen leicht abbilden und umformen
								// Eventuell Spezialkonvertierungen an den Daten vornehmen
								numRegs = modbusTable[tmpRow].numRegisters / ((modbusTable[tmpRow].operation == READ_MULTIPLE_COILS)?16:1);
								if(modbusTable[tmpRow].operation == READ_MULTIPLE_COILS && modbusTable[tmpRow].numRegisters % 16 != 0) {
									// Wenn nicht ein vielfaches von 16 Coils geholt wird muss die Registeranzahl um 1 erhöhrt werden, da sonst die letzten Coils nicht betrachtet werden
									numRegs += 1;
								}
								for(i = 0; i < (numRegs/2)+numRegs%2; ++i) {
									tmpVal = modbusToFloat(pRxBuf, &modbusTable[tmpRow].modbusType, modbusTable[tmpRow].operation, i*4+offset, &i);
									convert(&tmpVal, modbusTable[tmpRow].convert);
									floatToRiedel(tmpVal, modbusTable[tmpRow].address, &modbusTable[tmpRow].riedelType, i*4, min_w(4, (numRegs*2)-i*4));
								}
								offset += numRegs*2;
							}
						}
						// Ready für Schreibaufträge zurück setzten
						for(tmpRow = startRow[port]; tmpRow <= activeRow[port]; ++tmpRow) {
							modbusTableData[tmpRow].ready = FALSE;
						}
						state[port] = 1;
						activeRow[port] = findNextRow(port, tmpRow-1);
						counter[port] = 0;
					break;

					/*
				 	*  Es ist ein Fehler in der Datenübertragung aufgetreten.
				 	*  Die gleiche Zeile der ModbusTable wird erneut angefragt.
				 	*  Nach maximal drei Versuchen wird mit der nächsten Zeile fortgefahren.
				 	*/
					case SoF_ERROR:
					case CRC_ERROR:
					default:
						counter[port] = 0;
							state[port] = 1;
							for(tmpRow = startRow[port]; tmpRow <= activeRow[port]; ++tmpRow) {
								++modbusTableData[tmpRow].retries;

								failure(modbusTable[tmpRow].address, modbusTable[tmpRow].riedelType);
								if(modbusTableData[tmpRow].retries > 2) {
									modbusTableData[tmpRow].retries = 0;
										modbusTableData[tmpRow].ready = FALSE;
								}
							}
							if(modbusTableData[tmpRow-1].retries == 0) {
								activeRow[port] = findNextRow(port, tmpRow-1);
							} else {
								activeRow[port] = startRow[port];
							}

					break;
				}
			}
		break;
	}
	return fu_ret;
}


//----------------------------------------------------------------------------------------------
// Aufruf von "UserReply.c" in der Funktion "void SysEEP_InitUser_66(void)"
//----------------------------------------------------------------------------------------------
void Init_Modbus(void)
{
	char i;

	for(i = 0; i < modbusDeviceCount; i++)
		modbusDeviceAddresses[i] = modbusDevices[i].address;		// Adressen aus ModbusTabelle.c laden

	// Erste Page
	bicbus(EEPADR,	(char	*)&modbusDeviceAddresses[0],	MODBUS_ADDRESSES, 64, BICWR);
	hwdog(1);
	// Zweite Page
	bicbus(EEPADR,	(char	*)&modbusDeviceAddresses[64],	MODBUS_ADDRESSES+64, 64, BICWR);
	hwdog(1);


}

//----------------------------------------------------------------------------------------------
// Aufruf von "UserReply.c" in der Funktion "get_feldadresse(...)"
//----------------------------------------------------------------------------------------------
char* get_feldadresse_modbus(char *wert, unsigned char ucExp_num, unsigned char ucAttr)
{
	#if (((IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL) || ((IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL) || ((IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL))
	if(wert == (char *)&modbusDeviceNr) {
		if ( ( modbusDeviceNr == 0 ) || ( modbusDeviceNr > modbusDeviceCount) ) {
			modbusDeviceNr = 1;
		}
	}
	if( (wert>=(char *)&modbusDevices[0].name) && (wert<(char *)&modbusDevices[1].name) )
	{
		if ( ( modbusDeviceNr == 0 ) || ( modbusDeviceNr > modbusDeviceCount) ) {
			modbusDeviceNr = 1;
		}
		if ( ucExp_num == 0 ) {
			ucExp_num = modbusDeviceNr;
		}
		wert += ( (char *)&modbusDevices[1] - (char *)&modbusDevices[0] ) * (ucExp_num-1);
	}

	if( (wert ==(char *)&modbusDeviceAddresses[0]))
	{
		if( modbusDeviceNr == 0 ) {
			modbusDeviceNr = 1;
		}
		if ( ucExp_num == 0 ) {
			ucExp_num = modbusDeviceNr;
		}
		wert += ( (char *)&modbusDeviceAddresses[1] - (char *)&modbusDeviceAddresses[0] ) * (ucExp_num-1);
	}
	#endif

	return(wert);

}

//----------------------------------------------------------------------------------------------
// Aufruf von "UserKonv.c" in der Funktion "user_erste_zeile(...)"
//----------------------------------------------------------------------------------------------
char user_erste_zeile_modbus(char*	Visual_Ptr,	char gnum, char	pnum, unsigned char ucExp_num)
{
	unsigned char ucReturn = 0;
#if MODBUS_MASTER == 1
	#if ( ( (IMPLEMENT_S1 & MODBUS1_IMPL) == MODBUS1_IMPL) || ( (IMPLEMENT_S2 & MODBUS1_IMPL) == MODBUS1_IMPL) || ( (IMPLEMENT_S3 & MODBUS1_IMPL) == MODBUS1_IMPL) )
		if(strcmp(Pgruppe[gnum].grkz,"MOD;") == 0 && pnum < 5)
		{
			if(pnum > 1)
			{
				ctoa(&Visual_Ptr[1],modbusDeviceNr);
			}
			ucMax_expander = modbusDeviceCount;
			ucReturn = ucMax_expander;
		}
	#endif
#endif
	return(ucReturn);

}


#endif

