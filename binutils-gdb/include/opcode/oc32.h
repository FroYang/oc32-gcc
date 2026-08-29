/* OC32 opcode definitions */

#ifndef _OC32_H_
#define _OC32_H_

#include "bfd.h"



#ifdef __cplusplus
extern "C" {
#endif

/* Arithmetic instructions */
#define OP_ADD_R     (0x01FF0B << 15)     /* ADD 	Rc, 		   Ra, 	   Rb 		*/
#define OP_ADD_I     (0x024 << 26)        /* ADD 	Rb, 		   Ra, 	   simm16 	*/
#define OP_ADDC_R    (0x01FF0C << 15)     /* ADDC 	Rc, 		   Ra, 	   Rb 		*/
#define OP_ADDC_I    (0x025 << 26)        /* ADDC 	Rb, 		   Ra, 	   simm16 	*/
#define OP_SUB_R     (0x01FF0D << 15)     /* SUB 	Rc, 		   Ra, 	   Rb 		*/
#define OP_SUBB_R    (0x01FF0E << 15)     /* SUBB 	Rc, 		   Ra, 	   Rb 		*/
#define OP_SUBB_I    (0x026 << 26)        /* SUBB 	Rb, 		   Ra, 	   simm16 	*/
#define OP_MUL_R     (0x01FF14 << 15)     /* MUL 	Rc, 		   Ra, 	   Rb 		*/
#define OP_MUL_I     (0x02A << 26)        /* MUL 	Rb, 		   Ra, 	   simm16 	*/
#define OP_MULU_R    (0x01FF15 << 15)     /* MULU 	Rc, 		   Ra, 	   Rb 		*/
#define OP_MULU_I    (0x02B << 26)        /* MULU 	Rb, 		   Ra, 	   uimm16 	*/
#define OP_DIV_R     (0x01FF16 << 15)     /* DIV 	Rc, 		   Ra, 	   Rb 		*/
#define OP_DIV_I     (0x02C << 26)        /* DIV 	Rb, 		   Ra, 	   simm16 	*/
#define OP_DIVU_R    (0x01FF17 << 15)     /* DIVU 	Rc, 		   Ra, 	   Rb 		*/
#define OP_DIVU_I    (0x02D << 26)        /* DIVU 	Rb, 		   Ra, 	   uimm16 	*/

/* Compare instructions */
#define OP_CMP_R     (0x03FFFC6 << 10)    /* CMP 	Rb, 		   Ra 				   */
#define OP_CMP_I     (0x07E6 << 21)       /* CMP 	Ra, 		   simm16 			   */
#define OP_CMPU_R    (0x03FFFC7 << 10)    /* CMPU 	Rb, 		   Ra 				   */
#define OP_CMPU_I    (0x07E7 << 21)       /* CMPU 	Ra, 		   uimm16 			   */

/* Logical instructions */
#define OP_AND_R     (0x01FF0F << 15)     /* AND 	Rc, 		   Ra, 	   Rb 		*/
#define OP_AND_I     (0x027 << 26)        /* AND 	Rb, 		   Ra, 	   uimm16 	*/
#define OP_OR_R      (0x01FF10 << 15)     /* OR 	Rc, 		   Ra, 	   Rb 		*/
#define OP_OR_I      (0x028 << 26)        /* OR 	Rb, 		   Ra, 	   uimm16 	*/
#define OP_XOR_R     (0x01FF11 << 15)     /* XOR 	Rc, 		   Ra, 	   Rb 		*/
#define OP_XOR_I     (0x029 << 26)        /* XOR 	Rb, 		   Ra, 	   uimm16 	*/
#define OP_NAND_R    (0x01FF12 << 15)     /* NAND	Rc, 		   Ra, 	   Rb 		*/
#define OP_NOR_R     (0x01FF13 << 15)     /* NOR	Rc, 		   Ra, 	   Rb 		*/

/* Move instructions */
#define OP_MOVZ_R    (0x01FF00 << 15)     /* MOVZ	Rc,			Ra,		Rb 		*/
#define OP_MOVNZ_R   (0x01FF01 << 15)     /* MOVNZ	Rc,			Ra,		Rb 		*/
#define OP_MOVH_I    (0x07E5 << 21)       /* MOVH	Ra,			imm32 			   */

/* Load instructions */
#define OP_LW_R      (0x01FF02 << 15)     /* LW		Rc,			[Ra+Rb] 		      */
#define OP_LW_O      (0x08 << 26)         /* LW 	Rb, 		   [Ra+soff18]		   */
#define OP_LWI_R     (0x01FF03 << 15)     /* LWI	Rc,			[Ra],	   +Rb 	   */
#define OP_LWI_O     (0x0A << 26)         /* LWI 	Rb, 		   [Ra],	   +soff18  */
#define OP_LWAS_O    (0x0D << 26)         /* LWAS	Rb, 		   [Ra+soff18]		   */
#define OP_LB_R      (0x01FF08 << 15)     /* LB		Rc, 		   [Ra+Rb]			   */
#define OP_LBU_R     (0x01FF09 << 15)     /* LBU	Rc,			[Ra+Rb]			   */

/* Store instructions */
#define OP_SW_R      (0x01FF05 << 15)     /* SW		[Ra+Rb],	Rc 				      */
#define OP_SW_O      (0x09 << 26)         /* SW 	[Ra+soff18],Rb 				   */
#define OP_SWI_R     (0x01FF06 << 15)     /* SWI	[Ra],		Rc,		   +Rb 	   */
#define OP_SWI_O     (0x0B << 26)         /* SWI 	[Ra],		Rb,		   +soff18  */
#define OP_SWAT_O    (0x0E << 26)         /* SWAT	[Ra+soff18],Rb 				   */
#define OP_SB_R      (0x01FF0A << 15)     /* SB		[Ra+Rb],	Rc 				      */

/* Jump instructions */
#define OP_J_P       (0x00 << 28)         /* J 		rel30 						      */
#define OP_J_O       (0x01F0 << 23)       /* J 		[Ra+soff20]					      */
#define OP_JZ_R      (0x03FFFC8 << 10)    /* JZ 	[Ra], 		Rb 				   */
#define OP_JZ_P      (0x01F2 << 23)       /* JZ 	rel20, 		Ra 				   */
#define OP_JNZ_R     (0x03FFFC9 << 10)    /* JNZ 	[Ra], 		Rb 				   */
#define OP_JNZ_P     (0x01F3 << 23)       /* JNZ 	rel20, 		Ra 				   */
#define OP_JL_P      (0x01 << 28)         /* JL 	rel30 						      */
#define OP_JL_O      (0x01F1 << 23)       /* JL 	[Ra+soff20] 					   */

/* Shift instructions */
#define OP_RR_R      (0x01FF18 << 15)     /* RR		Rc,			Ra,		Rb 		*/
#define OP_RR_N      (0x01FF40 << 15)     /* RR		Rb,			Ra,		num5 	   */
#define OP_SL_R      (0x01FF19 << 15)     /* SL		Rc,			Ra,		Rb 		*/
#define OP_SL_N      (0x01FF41 << 15)     /* SL		Rb,			Ra,		num5 	   */
#define OP_SR_R      (0x01FF1A << 15)     /* SR		Rc,			Ra,		Rb 		*/
#define OP_SR_N      (0x01FF42 << 15)     /* SR		Rb,			Ra,		num5 	   */
#define OP_SA_R      (0x01FF1B << 15)     /* SA		Rc,			Ra,		Rb 		*/
#define OP_SA_N      (0x01FF43 << 15)     /* SA		Rb,			Ra,		num5 	   */

/* special instructions */
#define OP_MOV_PSR   (0x03FFFFCA << 5)   /* MOV     Ra,			PSR 			   */
#define OP_MOV_EIA   (0x03FFFFCD << 5)   /* MOV     Ra,			EIA 			   */
#define OP_RDSR_A    (0x07E0 << 21)       /* RDSR	   Ra,			addr18 			   */
#define OP_RDSR_R    (0x03FFFCA << 10)    /* RDSR	   Rb,			[Ra] 				   */
#define OP_WRSR_A    (0x07E1 << 21)       /* WRSR	   addr18,     Ra 			   */
#define OP_WRSR_R    (0x03FFFCB << 10)    /* WRSR	   [Ra],       Rb 				   */
#define OP_SBSR_A    (0x07E2 << 21)       /* SBSR	   addr18.n 			   */
#define OP_SBSR_R    (0x03FFFCC << 10)    /* SBSR	   [Ra].n 				   */
#define OP_CBSR_A    (0x07E3 << 21)       /* CBSR	   addr18.n 			   */
#define OP_CBSR_R    (0x03FFFCD << 10)    /* CBSR	   [Ra].n 				   */
#define OP_TBSR_A    (0x07E4 << 21)       /* TBSR	   addr18.n 			   */
#define OP_TBSR_R    (0x03FFFCE << 10)    /* TBSR	   [Ra].n 				   */
#define OP_EBF_R     (0x0FE0 << 20)       /* EBF	Rb,			Ra.m.n            */
#define OP_MBF_R     (0x0FE1 << 20)       /* MBF	Rb.m.n,	   Ra 				   */
#define OP_CLO_R     (0x03FFFC0 << 10)    /* CLO	Rb,			Ra 				   */
#define OP_CLZ_R     (0x03FFFC1 << 10)    /* CLZ	Rb,			Ra 				   */
#define OP_SEB_R     (0x03FFFC2 << 10)    /* SEB	Rb,			Ra 				   */
#define OP_SEH_R     (0x03FFFC3 << 10)    /* SEH	Rb,			Ra 				   */
#define OP_RV_R      (0x03FFFC4 << 10)    /* RV		Rb,			Ra 				   */
#define OP_RVB_R     (0x03FFFC5 << 10)    /* RVB	Rb,			Ra 				   */

/* System */
#define OP_SYSC      (0x0FF70 << 16)      /* SYSC	param16 				   */
#define OP_TRAPZ     (0x07FFFFCB << 5)   /* TRAPZ  Ra  */
#define OP_TRAPNZ    (0x07FFFFCC << 5)   /* TRAPNZ Ra  */
#define OP_RETE      0xFFFFFFFF          /* RETE  */
#define OP_NOP       0xFFFF2400

/* OC32 opcode information structure.  */
typedef struct
{
  const char *name;
  unsigned int mask;
  unsigned int bits;
  int fields;
} oc32_opc_info_t;



/* OC32 opcode information table.  */
extern const oc32_opc_info_t oc32_opc_info[];

#ifdef __cplusplus
}
#endif

#endif /* _OC32_H_ */
