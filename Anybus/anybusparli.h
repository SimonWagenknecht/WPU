#if ANYBUS_MODUL == 1
extern unsigned int	uiWriteArraySize;
extern unsigned int	uiReadArraySize;
extern char carAnybusstatus[];
extern char carAnybustyp[];
extern char carAnybusIPAdresse[];
extern char carAnybusSubnetmask[];
extern char carAnybusGateway[];


const Pgrup any[] = {
	{"***;", " ANYBUS         ", "       ", P&Leer15,											ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 01;", " Anybustyp      ", "       ", P&carAnybustyp,								ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 10;", " Anybus-STATUS  ", "       ", P&carAnybusstatus,						ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 11;", " IP-Adresse     ", "       ", P&carAnybusIPAdresse,					ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 12;", " Subnetmask     ", "       ", P&carAnybusSubnetmask,				ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 13;", " Gateway        ", "       ", P&carAnybusGateway,						ASCII_FORM, 0, P&hid1, V0, 0, 0},
	{" 14;", " WriteProcessAnz", "       ", P&uiWriteArraySize,								US_INT, 0, P&hid1, V0, 0, 0},
	{" 15;", " ReadProcessAnz ", "       ", P&uiReadArraySize,								US_INT, 0, P&hid1, V0, 0, 0},
};

#endif // ANYBUS_MODUL