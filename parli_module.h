#ifndef PARLI_MODULE_H_INCLUDED
#define PARLI_MODULE_H_INCLUDED

//----------------------------------------------------------------------------------------------------------
#if R37_MODULE > 0
 {" 41;"," BUS X0 G1 R37_1","       ", P&mod37[0].bus,						 US_CHAR, 0, P&r37vis[0],	V0, 0, 0},
 {" 42;"," ADR       R37_1","       ", P&mod37[0].adr,						 US_CHAR, 0, P&r37vis[0],	V0, 0, 0},
 {"*43;"," DAT-ERROR R37_1","       ", P&mod37[0].error,			 JANEIN_FORM, 0, P&r37vis[0],	E1, MODULE, 0},
 {"*44;"," DIGI-AUSG R37_1"," 1-3   ", P&mod37[0].Digout,				 DIGL_FORM, 3, P&r37vis[0],	V0, 0, 0},
 {"*45;"," DIGI-EING R37_1"," 1-6   ", P&mod37[0].Diginp,				 DIGL_FORM, 6, P&r37vis[0],	V0, 0, 0},
 {"*46;"," ANA-AUSG1 R37_1"," Volt  ", P&mod37[0].AoutSk[0],				US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_1"," Volt  ", P&mod37[0].AASkal[0].NullP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_1"," Volt  ", P&mod37[0].AASkal[0].HundP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {"*47;"," ANA-AUSG2 R37_1"," Volt  ", P&mod37[0].AoutSk[1],	 			US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_1"," Volt  ", P&mod37[0].AASkal[1].NullP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_1"," Volt  ", P&mod37[0].AASkal[1].HundP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {"*48;"," ANA-AUSG3 R37_1"," Volt  ", P&mod37[0].AoutSk[2],	 			US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_1"," Volt  ", P&mod37[0].AASkal[2].NullP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_1"," Volt  ", P&mod37[0].AASkal[2].HundP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {"*49;"," ANA-AUSG4 R37_1"," Volt  ", P&mod37[0].AoutSk[3],	 			US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_1"," Volt  ", P&mod37[0].AASkal[3].NullP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_1"," Volt  ", P&mod37[0].AASkal[3].HundP,	US_INT, 2, P&r37vis[0],	V0, 0, 0},
#endif

#if R37_MODULE > 1
 {" 41;"," BUS X0 G1 R37_2","       ", P&mod37[1].bus,						 US_CHAR, 0, P&r37vis[1],	V0, 0, 0},
 {" 42;"," ADR       R37_2","       ", P&mod37[1].adr,						 US_CHAR, 0, P&r37vis[1],	V0, 0, 0},
 {"*43;"," DAT-ERROR R37_2","       ", P&mod37[1].error,			 JANEIN_FORM, 0, P&r37vis[1],	E1, MODULE, 0},
 {"*44;"," DIGI-AUSG R37_2"," 1-3   ", P&mod37[1].Digout,				 DIGL_FORM, 3, P&r37vis[1],	V0, 0, 0},
 {"*45;"," DIGI-EING R37_2"," 1-6   ", P&mod37[1].Diginp,				 DIGL_FORM, 6, P&r37vis[1],	V0, 0, 0},
 {"*46;"," ANA-AUSG1 R37_2"," Volt  ", P&mod37[1].AoutSk[0],				US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_2"," Volt  ", P&mod37[1].AASkal[0].NullP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_2"," Volt  ", P&mod37[1].AASkal[0].HundP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {"*47;"," ANA-AUSG2 R37_2"," Volt  ", P&mod37[1].AoutSk[1],	 			US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_2"," Volt  ", P&mod37[1].AASkal[1].NullP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_2"," Volt  ", P&mod37[1].AASkal[1].HundP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {"*48;"," ANA-AUSG3 R37_2"," Volt  ", P&mod37[1].AoutSk[2],	 			US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_2"," Volt  ", P&mod37[1].AASkal[2].NullP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_2"," Volt  ", P&mod37[1].AASkal[2].HundP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {"*49;"," ANA-AUSG4 R37_2"," Volt  ", P&mod37[1].AoutSk[3],	 			US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_2"," Volt  ", P&mod37[1].AASkal[3].NullP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_2"," Volt  ", P&mod37[1].AASkal[3].HundP,	US_INT, 2, P&r37vis[1],	V0, 0, 0},
#endif

#if R37_MODULE > 2
 {" 41;"," BUS X0 G1 R37_3","       ", P&mod37[2].bus,						 US_CHAR, 0, P&r37vis[2],	V0, 0, 0},
 {" 42;"," ADR       R37_3","       ", P&mod37[2].adr,						 US_CHAR, 0, P&r37vis[2],	V0, 0, 0},
 {"*43;"," DAT-ERROR R37_3","       ", P&mod37[2].error,			 JANEIN_FORM, 0, P&r37vis[2],	E1, MODULE, 0},
 {"*44;"," DIGI-AUSG R37_3"," 1-3   ", P&mod37[2].Digout,				 DIGL_FORM, 3, P&r37vis[2],	V0, 0, 0},
 {"*45;"," DIGI-EING R37_3"," 1-6   ", P&mod37[2].Diginp,				 DIGL_FORM, 6, P&r37vis[2],	V0, 0, 0},
 {"*46;"," ANA-AUSG1 R37_3"," Volt  ", P&mod37[2].AoutSk[0],				US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_3"," Volt  ", P&mod37[2].AASkal[0].NullP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_3"," Volt  ", P&mod37[2].AASkal[0].HundP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {"*47;"," ANA-AUSG2 R37_3"," Volt  ", P&mod37[2].AoutSk[1],	 			US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_3"," Volt  ", P&mod37[2].AASkal[1].NullP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_3"," Volt  ", P&mod37[2].AASkal[1].HundP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {"*48;"," ANA-AUSG3 R37_3"," Volt  ", P&mod37[2].AoutSk[2],	 			US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_3"," Volt  ", P&mod37[2].AASkal[2].NullP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_3"," Volt  ", P&mod37[2].AASkal[2].HundP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {"*49;"," ANA-AUSG4 R37_3"," Volt  ", P&mod37[2].AoutSk[3],	 			US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_3"," Volt  ", P&mod37[2].AASkal[3].NullP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_3"," Volt  ", P&mod37[2].AASkal[3].HundP,	US_INT, 2, P&r37vis[2],	V0, 0, 0},
#endif

#if R37_MODULE > 3
 {" 41;"," BUS X0 G1 R37_4","       ", P&mod37[3].bus,						 US_CHAR, 0, P&r37vis[3],	V0, 0, 0},
 {" 42;"," ADR       R37_4","       ", P&mod37[3].adr,						 US_CHAR, 0, P&r37vis[3],	V0, 0, 0},
 {"*43;"," DAT-ERROR R37_4","       ", P&mod37[3].error,			 JANEIN_FORM, 0, P&r37vis[3],	E1, MODULE, 0},
 {"*44;"," DIGI-AUSG R37_4"," 1-3   ", P&mod37[3].Digout,				 DIGL_FORM, 3, P&r37vis[3],	V0, 0, 0},
 {"*45;"," DIGI-EING R37_4"," 1-6   ", P&mod37[3].Diginp,				 DIGL_FORM, 6, P&r37vis[3],	V0, 0, 0},
 {"*46;"," ANA-AUSG1 R37_4"," Volt  ", P&mod37[3].AoutSk[0],				US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_4"," Volt  ", P&mod37[3].AASkal[0].NullP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_4"," Volt  ", P&mod37[3].AASkal[0].HundP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {"*47;"," ANA-AUSG2 R37_4"," Volt  ", P&mod37[3].AoutSk[1],	 			US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_4"," Volt  ", P&mod37[3].AASkal[1].NullP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_4"," Volt  ", P&mod37[3].AASkal[1].HundP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {"*48;"," ANA-AUSG3 R37_4"," Volt  ", P&mod37[3].AoutSk[2],	 			US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_4"," Volt  ", P&mod37[3].AASkal[2].NullP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_4"," Volt  ", P&mod37[3].AASkal[2].HundP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {"*49;"," ANA-AUSG4 R37_4"," Volt  ", P&mod37[3].AoutSk[3],	 			US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 0%   R37_4"," Volt  ", P&mod37[3].AASkal[3].NullP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
 {" ->;"," SKAL 100% R37_4"," Volt  ", P&mod37[3].AASkal[3].HundP,	US_INT, 2, P&r37vis[3],	V0, 0, 0},
#endif
//----------------------------------------------------------------------------------------------------------

#if R38_MODULE > 0
 {" 51;"," BUS X0 G1 R38_1","       ", P&mod38[0].bus,				 				 US_CHAR, 0, P&r38vis[0],	V0, 0, 0},
 {" 52;"," ADR       R38_1","       ", P&mod38[0].adr,								 US_CHAR, 0, P&r38vis[0],	V0, 0, 0},
 {"*53;"," DAT-ERROR R38_1","       ", P&mod38[0].error,	 				 JANEIN_FORM, 0, P&r38vis[0],	E1, MODULE, 0},
 {"054;"," PROJ.EING R38_1","       ", P&mod38[0].inpUsed,					 DIGL_FORM,14, P&r38vis[0],	V0, 0, 0},
 {"055;"," LIFE.EING R38_1","       ", P&mod38[0].life,							 DIGL_FORM,14, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN1    R38_1"," C    ", P&mod38[0].pt_mewe[0].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN2    R38_1"," C    ", P&mod38[0].pt_mewe[1].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN3    R38_1"," C    ", P&mod38[0].pt_mewe[2].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN4    R38_1"," C    ", P&mod38[0].pt_mewe[3].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN5    R38_1"," C    ", P&mod38[0].pt_mewe[4].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN6    R38_1"," C    ", P&mod38[0].pt_mewe[5].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN7    R38_1"," C    ", P&mod38[0].pt_mewe[6].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN8    R38_1"," C    ", P&mod38[0].pt_mewe[7].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN9    R38_1"," C    ", P&mod38[0].pt_mewe[8].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN10   R38_1"," C    ", P&mod38[0].pt_mewe[9].messw,		ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN11   R38_1"," C    ", P&mod38[0].pt_mewe[10].messw,	ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN12   R38_1"," C    ", P&mod38[0].pt_mewe[11].messw,	ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN13   R38_1"," C    ", P&mod38[0].pt_mewe[12].messw,	ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
 {"*->;"," PT_IN14   R38_1"," C    ", P&mod38[0].pt_mewe[13].messw,	ANA_FORM, 1, P&r38vis[0],	V0, 0, 0},
#endif

#if R38_MODULE > 1
 {" 51;"," BUS X0 G1 R38_2","       ", P&mod38[1].bus,				 				 US_CHAR, 0, P&r38vis[1],	V0, 0, 0},
 {" 52;"," ADR       R38_2","       ", P&mod38[1].adr,								 US_CHAR, 0, P&r38vis[1],	V0, 0, 0},
 {"*53;"," DAT-ERROR R38_2","       ", P&mod38[1].error,	 				 JANEIN_FORM, 0, P&r38vis[1],	E1, MODULE, 0},
 {"054;"," PROJ.EING R38_2","       ", P&mod38[1].inpUsed,					 DIGL_FORM,14, P&r38vis[1],	V0, 0, 0},
 {"055;"," LIFE.EING R38_2","       ", P&mod38[1].life,							 DIGL_FORM,14, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN1    R38_2"," C    ", P&mod38[1].pt_mewe[0].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN2    R38_2"," C    ", P&mod38[1].pt_mewe[1].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN3    R38_2"," C    ", P&mod38[1].pt_mewe[2].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN4    R38_2"," C    ", P&mod38[1].pt_mewe[3].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN5    R38_2"," C    ", P&mod38[1].pt_mewe[4].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN6    R38_2"," C    ", P&mod38[1].pt_mewe[5].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN7    R38_2"," C    ", P&mod38[1].pt_mewe[6].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN8    R38_2"," C    ", P&mod38[1].pt_mewe[7].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN9    R38_2"," C    ", P&mod38[1].pt_mewe[8].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN10   R38_2"," C    ", P&mod38[1].pt_mewe[9].messw,		ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN11   R38_2"," C    ", P&mod38[1].pt_mewe[10].messw,	ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN12   R38_2"," C    ", P&mod38[1].pt_mewe[11].messw,	ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN13   R38_2"," C    ", P&mod38[1].pt_mewe[12].messw,	ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
 {"*->;"," PT_IN14   R38_2"," C    ", P&mod38[1].pt_mewe[13].messw,	ANA_FORM, 1, P&r38vis[1],	V0, 0, 0},
#endif

#if R38_MODULE > 2
 {" 51;"," BUS X0 G1 R38_3","       ", P&mod38[2].bus,				 				 US_CHAR, 0, P&r38vis[2],	V0, 0, 0},
 {" 52;"," ADR       R38_3","       ", P&mod38[2].adr,								 US_CHAR, 0, P&r38vis[2],	V0, 0, 0},
 {"*53;"," DAT-ERROR R38_3","       ", P&mod38[2].error,	 				 JANEIN_FORM, 0, P&r38vis[2],	E1, MODULE, 0},
 {"054;"," PROJ.EING R38_3","       ", P&mod38[2].inpUsed,					 DIGL_FORM,14, P&r38vis[2],	V0, 0, 0},
 {"055;"," LIFE.EING R38_3","       ", P&mod38[2].life,							 DIGL_FORM,14, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN1    R38_3"," C    ", P&mod38[2].pt_mewe[0].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN2    R38_3"," C    ", P&mod38[2].pt_mewe[1].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN3    R38_3"," C    ", P&mod38[2].pt_mewe[2].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN4    R38_3"," C    ", P&mod38[2].pt_mewe[3].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN5    R38_3"," C    ", P&mod38[2].pt_mewe[4].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN6    R38_3"," C    ", P&mod38[2].pt_mewe[5].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN7    R38_3"," C    ", P&mod38[2].pt_mewe[6].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN8    R38_3"," C    ", P&mod38[2].pt_mewe[7].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN9    R38_3"," C    ", P&mod38[2].pt_mewe[8].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN10   R38_3"," C    ", P&mod38[2].pt_mewe[9].messw,		ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN11   R38_3"," C    ", P&mod38[2].pt_mewe[10].messw,	ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN12   R38_3"," C    ", P&mod38[2].pt_mewe[11].messw,	ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN13   R38_3"," C    ", P&mod38[2].pt_mewe[12].messw,	ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
 {"*->;"," PT_IN14   R38_3"," C    ", P&mod38[2].pt_mewe[13].messw,	ANA_FORM, 1, P&r38vis[2],	V0, 0, 0},
#endif

#if R38_MODULE > 3
 {" 51;"," BUS X0 G1 R38_4","       ", P&mod38[3].bus,				 				 US_CHAR, 0, P&r38vis[3],	V0, 0, 0},
 {" 52;"," ADR       R38_4","       ", P&mod38[3].adr,								 US_CHAR, 0, P&r38vis[3],	V0, 0, 0},
 {"*53;"," DAT-ERROR R38_4","       ", P&mod38[3].error,	 				 JANEIN_FORM, 0, P&r38vis[3],	E1, MODULE, 0},
 {"054;"," PROJ.EING R38_4","       ", P&mod38[3].inpUsed,					 DIGL_FORM,14, P&r38vis[3],	V0, 0, 0},
 {"055;"," LIFE.EING R38_4","       ", P&mod38[3].life,							 DIGL_FORM,14, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN1    R38_4"," C    ", P&mod38[3].pt_mewe[0].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN2    R38_4"," C    ", P&mod38[3].pt_mewe[1].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN3    R38_4"," C    ", P&mod38[3].pt_mewe[2].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN4    R38_4"," C    ", P&mod38[3].pt_mewe[3].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN5    R38_4"," C    ", P&mod38[3].pt_mewe[4].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN6    R38_4"," C    ", P&mod38[3].pt_mewe[5].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN7    R38_4"," C    ", P&mod38[3].pt_mewe[6].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN8    R38_4"," C    ", P&mod38[3].pt_mewe[7].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN9    R38_4"," C    ", P&mod38[3].pt_mewe[8].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN10   R38_4"," C    ", P&mod38[3].pt_mewe[9].messw,		ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN11   R38_4"," C    ", P&mod38[3].pt_mewe[10].messw,	ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN12   R38_4"," C    ", P&mod38[3].pt_mewe[11].messw,	ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN13   R38_4"," C    ", P&mod38[3].pt_mewe[12].messw,	ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
 {"*->;"," PT_IN14   R38_4"," C    ", P&mod38[3].pt_mewe[13].messw,	ANA_FORM, 1, P&r38vis[3],	V0, 0, 0},
#endif
//----------------------------------------------------------------------------------------------------------

#if R39_MODULE > 0
 {" 61;"," BUS X0 G1 R39_1","       ", P&mod39[0].bus,				 US_CHAR,		0, P&r39vis[0],	V0, 0, 0},
 {" 62;"," ADR       R39_1","       ", P&mod39[0].adr,				 US_CHAR, 	0, P&r39vis[0],	V0, 0, 0},
 {"*63;"," DAT-ERROR R39_1","       ", P&mod39[0].error,	 JANEIN_FORM,		0, P&r39vis[0],	E1, MODULE, 0},
 {"*64;"," DIGI-ERR  R39_1","       ", P&mod39[0].Derror,	 JANEIN_FORM,   0, P&r39vis[0],	E1, MODULE, 0},
 {"*65;"," DIGI-EING R39_1","       ", P&mod39[0].Diginp2,	 DIGL_FORM,  12, P&r39vis[0],	V0, 0, 0},
 {"066;"," DIGI-ART  R39_1","       ", P&mod39[0].Digart,		 DIGL_FORM,  12, P&r39vis[0],	V0, 0, 0},
#endif

#if R39_MODULE > 1
 {" 61;"," BUS X0 G1 R39_2","       ", P&mod39[1].bus,				 US_CHAR,		0, P&r39vis[1],	V0, 0, 0},
 {" 62;"," ADR       R39_2","       ", P&mod39[1].adr,				 US_CHAR, 	0, P&r39vis[1],	V0, 0, 0},
 {"*63;"," DAT-ERROR R39_2","       ", P&mod39[1].error,	 JANEIN_FORM,		0, P&r39vis[1],	E1, MODULE, 0},
 {"*64;"," DIGI-ERR  R39_2","       ", P&mod39[1].Derror,	 JANEIN_FORM,   0, P&r39vis[1],	E1, MODULE, 0},
 {"*65;"," DIGI-EING R39_2","       ", P&mod39[1].Diginp2,	 DIGL_FORM,  12, P&r39vis[1],	V0, 0, 0},
 {"066;"," DIGI-ART  R39_2","       ", P&mod39[1].Digart,		 DIGL_FORM,  12, P&r39vis[1],	V0, 0, 0},
#endif

#if R39_MODULE > 2
 {" 61;"," BUS X0 G1 R39_3","       ", P&mod39[2].bus,				 US_CHAR,		0, P&r39vis[2],	V0, 0, 0},
 {" 62;"," ADR       R39_3","       ", P&mod39[2].adr,				 US_CHAR, 	0, P&r39vis[2],	V0, 0, 0},
 {"*63;"," DAT-ERROR R39_3","       ", P&mod39[2].error,	 JANEIN_FORM,		0, P&r39vis[2],	E1, MODULE, 0},
 {"*64;"," DIGI-ERR  R39_3","       ", P&mod39[2].Derror,	 JANEIN_FORM,   0, P&r39vis[2],	E1, MODULE, 0},
 {"*65;"," DIGI-EING R39_3","       ", P&mod39[2].Diginp2,	 DIGL_FORM,  12, P&r39vis[2],	V0, 0, 0},
 {"066;"," DIGI-ART  R39_3","       ", P&mod39[2].Digart,		 DIGL_FORM,  12, P&r39vis[2],	V0, 0, 0},
#endif

#if R39_MODULE > 3
 {" 61;"," BUS X0 G1 R39_4","       ", P&mod39[3].bus,				 US_CHAR,		0, P&r39vis[3],	V0, 0, 0},
 {" 62;"," ADR       R39_4","       ", P&mod39[3].adr,				 US_CHAR, 	0, P&r39vis[3],	V0, 0, 0},
 {"*63;"," DAT-ERROR R39_4","       ", P&mod39[3].error,	 JANEIN_FORM,		0, P&r39vis[3],	E1, MODULE, 0},
 {"*64;"," DIGI-ERR  R39_4","       ", P&mod39[3].Derror,	 JANEIN_FORM,   0, P&r39vis[3],	E1, MODULE, 0},
 {"*65;"," DIGI-EING R39_4","       ", P&mod39[3].Diginp2,	 DIGL_FORM,  12, P&r39vis[3],	V0, 0, 0},
 {"066;"," DIGI-ART  R39_4","       ", P&mod39[3].Digart,		 DIGL_FORM,  12, P&r39vis[3],	V0, 0, 0},
#endif
//----------------------------------------------------------------------------------------------------------

#if R33_MODULE > 0
 {" 71;"," BUS X0 G1 R33_1","       ", P&mod33[0].bus,				 US_CHAR,		0, P&r33vis[0],	V0, 0, 0},
 {" 72;"," ADR       R33_1","       ", P&mod33[0].adr,				 US_CHAR, 	0, P&r33vis[0],	V0, 0, 0},
 {"*73;"," DAT-ERROR R33_1","       ", P&mod33[0].error,	 JANEIN_FORM,		0, P&r33vis[0],	E1, MODULE, 0},
 {"*74;"," DIGI-AUSG R33_1"," 1-6   ", P&mod33[0].Digout,		 DIGL_FORM, 	6, P&r33vis[0],	V0, 0, 0},
#endif

#if R33_MODULE > 1
 {" 71;"," BUS X0 G1 R33_2","       ", P&mod33[1].bus,				 US_CHAR,		0, P&r33vis[1],	V0, 0, 0},
 {" 72;"," ADR       R33_2","       ", P&mod33[1].adr,				 US_CHAR, 	0, P&r33vis[1],	V0, 0, 0},
 {"*73;"," DAT-ERROR R33_2","       ", P&mod33[1].error,	 JANEIN_FORM,		0, P&r33vis[1],	E1, MODULE, 0},
 {"*74;"," DIGI-AUSG R33_2"," 1-6   ", P&mod33[1].Digout,		 DIGL_FORM, 	6, P&r33vis[1],	V0, 0, 0},
#endif

#if R33_MODULE > 2
 {" 71;"," BUS X0 G1 R33_3","       ", P&mod33[2].bus,				 US_CHAR,		0, P&r33vis[2],	V0, 0, 0},
 {" 72;"," ADR       R33_3","       ", P&mod33[2].adr,				 US_CHAR, 	0, P&r33vis[2],	V0, 0, 0},
 {"*73;"," DAT-ERROR R33_3","       ", P&mod33[2].error,	 JANEIN_FORM,		0, P&r33vis[2],	E1, MODULE, 0},
 {"*74;"," DIGI-AUSG R33_3"," 1-6   ", P&mod33[2].Digout,		 DIGL_FORM, 	6, P&r33vis[2],	V0, 0, 0},
#endif

#if R33_MODULE > 3
 {" 71;"," BUS X0 G1 R33_4","       ", P&mod33[3].bus,				 US_CHAR,		0, P&r33vis[3],	V0, 0, 0},
 {" 72;"," ADR       R33_4","       ", P&mod33[3].adr,				 US_CHAR, 	0, P&r33vis[3],	V0, 0, 0},
 {"*73;"," DAT-ERROR R33_4","       ", P&mod33[3].error,	 JANEIN_FORM,		0, P&r33vis[3],	E1, MODULE, 0},
 {"*74;"," DIGI-AUSG R33_4"," 1-6   ", P&mod33[3].Digout,		 DIGL_FORM, 	6, P&r33vis[3],	V0, 0, 0},
#endif
//----------------------------------------------------------------------------------------------------------

#endif // PARLI_MODULE_H_INCLUDED