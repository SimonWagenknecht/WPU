// User-Konvertierungsformate "uskonvform.h"

// WRE100
#define WOBIT_FORM              182   // 16 Bit Statusanzeige               attr = 0 Darstellung WRE01-16 , attr = 1 Darstellung WRE17-32 ...
#define LAST_FORM               183   // Nibble-Anzeige Lastverhältnis      High=Gesamtanzahl Low=offene Ventile
#define ENERG_FORM              184   // Energie lesen                      attr = Kommastelle
#define DATE_INV_FORM           185   // 3 Byte Datum inverse Reihenfolge   Index[0]Tag, [1]Monat, [2]Jahr
//#define XEEP_ASCII_FORM       186   // FerroRam: max. 15  ASCII-Zeichen   attr = Anzahl der darzustellenden Bytes (0=15)
#define XEEP_US_CHAR_FORM       187   // FerroRam: unsigned char            attr = Kommastelle
#define XEEP_N_BYTES_FORM       188   // FerroRam: n-Bytes                  attr = n
#define S_INT_P                 189   // signed int pointer                 attr = Kommastelle
#define EXP_N_WOSTAT            190   // n-Bytes aus wostat[] mit Adressexpansion durch WRE-Nr.     attr = n
#define EXP_1TO2_WOSTAT         191   // 1 Byte nibbles -> 2 Byte (für vlast) aus wostat[] mit Adressexpansion durch WRE-Nr
#define XEEP_EXP_ENERG          192   // FerroRam: Energie mit Adressexpansion durch WRE-Nr.
#define XEEP_EXP_ZAEHL          193   // FerroRam: WWZähler mit Adressexpansion durch WRE-Nr.
#define XEEP_EXP_DATUM          194   // FerroRam: Speicherdatum mit Adressexpansion durch WRE-Nr.
#define EXP_N_SAMSTAT           195   // 2-Byte Einzelbit aus samstat[] mit Adressexpansion durch WRE-Nr.
#define TA_BROADC               196   // Broadcast Außentemperatur, mit Eintrag der Gültigkeitsdauer beim Schreiben
#define RM_S_INT                197   // signed integer: KONVWR: Rauminfo merken für VTimer zum Auslösen eines Schreibauftrages an den WRE50
#define RM_DIGL_FORM            198   // signed integer: KONVWR: Rauminfo merken für VTimer zum Auslösen eines Schreibauftrages an den WRE50
#define LONG2TIME_FORM          199   // 

//reservierte Konvertierungsnummern: ab 200

// BUS_PUMPEN (GENI, WILO_MODBUS)
  #define PU_CTR_FORM             200
  #define PU_FUNC_FORM            201
//#define GB_STATE_FORM           202
//#define BUS_STAT_FORM           203
//#define CHARBUF_FORM            204
  #define PU_OPR_FORM             205
//#define PU_ZWECK_FORM           206


// MBus
#define MBUS_S_LONG             209 
#define MBUS_TYP_FORM           210
#define MBUS_WMENG_FORM         211
#define MBUS_VOL_FORM           212
#define MBUS_ZWERT_FORM         213
#define MBUSMASTER_SYNCVJ_FORM  214
#define MBUSMASTER_SYNCMON_FORM 215

// Modbus
#define SIO_NAME_FORM           218

// ZLT Funktionsnamen für Schnittstellen
#define FU_NAME_FORM            219


