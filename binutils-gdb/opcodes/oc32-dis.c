/* Disassemble oc32 instructions.
   Copyright (C) 2023-2024 Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include <stdio.h>

#include "opcode/oc32.h"
#include "disassemble.h"

extern const oc32_opc_info_t oc32_opc_info[87];

static fprintf_ftype fpr;
static void *stream;

static int
sign_extend(int bit, int value)
{
	int shift = 32 - bit;
	int tmp = value << shift;
	return (tmp >> shift);
}

static void
oc32_opcode(bfd_vma addr, unsigned int iword,
			struct disassemble_info *info)
{
	const oc32_opc_info_t *oo;

	for (oo = oc32_opc_info; oo->name; oo++)
		if ((iword & oo->mask) == oo->bits)
			break;

	if (oo->name)
	{
		if (iword == OP_NOP)
		{
			fpr(stream, "  //\tNOP");
		}
		else
		{
			fpr(stream, "  //\t%s\t", oo->name);
			/* Parse operands based on opcode */
			switch (oo->bits)
			{
			/* Arithmetic instructions */
			case OP_ADD_R:
			case OP_ADDC_R:
			case OP_SUB_R:
			case OP_SUBB_R:
			case OP_MUL_R:
			case OP_MULU_R:
			case OP_DIV_R:
			case OP_DIVU_R:
			case OP_AND_R:
			case OP_OR_R:
			case OP_XOR_R:
			case OP_NAND_R:
			case OP_NOR_R:
			/* Move instructions */
			case OP_MOVZ_R:
			case OP_MOVNZ_R:
			/* Shift instructions */
			case OP_RR_R:
			case OP_SL_R:
			case OP_SR_R:
			case OP_SA_R:
				fpr(stream, "R%d,  R%d,  R%d",
					(iword >> 10) & 0x1F,
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_ADD_I:
			case OP_ADDC_I:
			case OP_SUBB_I:
			case OP_MUL_I:
			case OP_DIV_I:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  R%d,  %+d",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					sign_extend(16, (iword >> 10) & 0xFFFF));
				break;
                        
            case OP_MULU_I:
            case OP_DIVU_I:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  R%d,  %d",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					(iword >> 10) & 0xFFFF);
				break;

			case OP_AND_I:
			case OP_OR_I:
			case OP_XOR_I:
				fpr(stream, "R%d,  R%d,  0x%08x",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					(iword >> 10) & 0xFFFF);
				break;

			/* Compare instructions */
			case OP_CMP_R:
			case OP_CMPU_R:
				fpr(stream, "R%d,  R%d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_CLO_R:
			case OP_CLZ_R:
			case OP_SEB_R:
			case OP_SEH_R:
			case OP_RV_R:
			case OP_RVB_R:
				fpr(stream, "R%d,  R%d",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F);
				break;

			case OP_CMP_I:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  %+d",
					(iword >> 0) & 0x1F,
					sign_extend(16, (iword >> 5) & 0xFFFF));
				break;

			case OP_CMPU_I:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  %d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0xFFFF);
				break;

			case OP_MOVH_I:
				fpr(stream, "R%d,  0x%04x",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0xFFFF);
				break;

			/* Load instructions */
			case OP_LW_R:
			case OP_LWI_R:
			case OP_LB_R:
			case OP_LBU_R:
				fpr(stream, "R%d,  [R%d+R%d]",
					(iword >> 10) & 0x1F,
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_LW_O:
			case OP_LWAS_O:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  [R%d%+d]",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					sign_extend(18, ((iword >> 10) & 0xFFFF) << 2));
				break;

			case OP_LWI_O:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "R%d,  [R%d],  %+d",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					sign_extend(18, ((iword >> 10) & 0xFFFF) << 2));
				break;

			/* Store instructions */
			case OP_SW_R:
			case OP_SB_R:
				fpr(stream, "[R%d+R%d],  R%d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F,
					(iword >> 10) & 0x1F);
				break;

			case OP_SWI_R:
				fpr(stream, "[R%d],  R%d,  +R%d",
					(iword >> 0) & 0x1F,
					(iword >> 10) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_SW_O:
			case OP_SWAT_O:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "[R%d%+d],  R%d",
					(iword >> 0) & 0x1F,
					sign_extend(18, ((iword >> 10) & 0xFFFF) << 2),
					(iword >> 5) & 0x1F);
				break;

			case OP_SWI_O:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "[R%d],  R%d,  %+d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F,
					sign_extend(18, ((iword >> 10) & 0xFFFF) << 2));
				break;

			/* Jump instructions */
			case OP_J_P:
			case OP_JL_P:
			{
				bfd_vma target = addr + (bfd_vma)sign_extend(30, (iword & 0xFFFFFF) << 2);
				fpr(stream, "0x");
				info->print_address_func(target, info);
				break;
			}

			case OP_J_O:
			case OP_JL_O:
                /* use decimalism form because it is arithmetical data */
				fpr(stream, "[R%d%+d]",
					(iword >> 0) & 0x1F,
					sign_extend(20, ((iword >> 5) & 0x3FFFF) << 2));
				break;

			case OP_JZ_P:
			case OP_JNZ_P:
			{
				bfd_vma target = addr + (bfd_vma)sign_extend(20, ((iword >> 5) & 0x3FFFF) << 2);
				fpr(stream, "0x");
				info->print_address_func(target, info);
				fpr(stream, ",  R%d", (iword >> 0) & 0x1F);
				break;
			}


			/* Jump Zero/not instructions */
			case OP_JZ_R:
			case OP_JNZ_R:
				fpr(stream, "[R%d],  R%d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_RR_N:
			case OP_SL_N:
			case OP_SR_N:
			case OP_SA_N:
				fpr(stream, "R%d,  R%d,  %d",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F,
					(iword >> 10) & 0x1F);
				break;

			/* Special instructions */
			case OP_MOV_PSR:
				fpr(stream, "R%d,  PSR",
					(iword >> 0) & 0x1F);
				break;

			case OP_MOV_EIA:
				fpr(stream, "R%d,  EIA",
					(iword >> 0) & 0x1F);
				break;

			case OP_RDSR_R:
				fpr(stream, "R%d,  [R%d]",
					(iword >> 5) & 0x1F,
					(iword >> 0) & 0x1F);
				break;

			case OP_RDSR_A:
				fpr(stream, "R%d,  %d",
					(iword >> 0) & 0x1F,
					((iword >> 5) & 0xFFFF) << 2);
				break;

			case OP_WRSR_R:
				fpr(stream, "[R%d],  R%d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_WRSR_A:
				fpr(stream, "0x%08x,  R%d",
					((iword >> 5) & 0xFFFF) << 2,
					(iword >> 0) & 0x1F);
				break;

			case OP_SBSR_R:
			case OP_CBSR_R:
			case OP_TBSR_R:
				fpr(stream, "[R%d],%d",
					(iword >> 0) & 0x1F,
					(iword >> 5) & 0x1F);
				break;

			case OP_SBSR_A:
			case OP_CBSR_A:
			case OP_TBSR_A:
				fpr(stream, "0x%08x,%d",
					((iword >> 5) & 0xFFFF) << 2,
					(iword >> 0) & 0x1F);
				break;

			case OP_EBF_R:
				fpr(stream, "R%d,  R%d,%d,%d",
					(iword >> 5) & 0x1F,   // Rb
					(iword >> 0) & 0x1F,   // Ra
					(iword >> 15) & 0x1F,  // m (size)
					(iword >> 10) & 0x1F); // n (start)
				break;

			case OP_MBF_R:
				fpr(stream, "R%d,%d,%d,  R%d",
					(iword >> 5) & 0x1F,  // Rb
					(iword >> 15) & 0x1F, // m (size)
					(iword >> 10) & 0x1F, // n (start)
					(iword >> 0) & 0x1F); // Ra
				break;

			case OP_RETE:
				break;

			case OP_SYSC:
				fpr(stream, "%d",
					(iword >> 0) & 0xFFFF);
				break;

			case OP_TRAPZ:
			case OP_TRAPNZ:
				fpr(stream, "R%d",
					(iword >> 0) & 0x1F);
				break;

			default:
				break;
			}
		}
	}
	else
		fpr(stream, "  //\tData?");
}

int print_insn_oc32(bfd_vma addr, struct disassemble_info *info)
{
	int status;
	stream = info->stream;
	bfd_byte buffer[4];
	unsigned int iword;

	fpr = info->fprintf_func;

	if ((status = info->read_memory_func(addr, buffer, 4, info)))
		goto fail;

	iword = bfd_getl32(buffer);

	fpr(stream, "  |:  0x%08x ", iword);

	oc32_opcode(addr, iword, info);

	return 4;

fail:
	info->memory_error_func(status, addr, info);
	return -1;
}
