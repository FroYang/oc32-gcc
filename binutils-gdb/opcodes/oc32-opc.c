/* oc32-opc.c -- Definitions for oc32 opcodes.
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
   along with this file; see the file COPYING.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include "opcode/oc32.h"

/* OC32 opcode information table.
   Mask has been normalized: leading zeros shifted out, highest 1-bit at bit31.
   Bits (opcode) has been shifted the same amount as mask.
*/
/* OC32 instruction field flags.  */
#define OC32_REGC 0x0001   /* register Rc */
#define OC32_REGB 0x0002   /* register Rb */
#define OC32_REGA 0x0004   /* register Ra */
#define OC32_IMM 0x0008    /* Immediate value */
#define OC32_SIMM 0x0010   /* Signed immediate value */
#define OC32_UIMM 0x0020   /* Unsigned immediate value */
#define OC32_REL 0x0040    /* Relative offset */
#define OC32_ADDR 0x0080   /* Address */
#define OC32_BIT 0x0100    /* Bit position */
#define OC32_SHIFT 0x0200  /* Shift count */
#define OC32_BITFLD 0x0400 /* Bit field specification (Ra.m.n) */

const oc32_opc_info_t oc32_opc_info[] =
    {
        /* Arithmetic instructions */
        {"ADD", 0xFFFF8000, OP_ADD_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"ADD", 0xFC000000, OP_ADD_I, OC32_REGC | OC32_REGB | OC32_SIMM},
        {"ADDC", 0xFFFF8000, OP_ADDC_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"ADDC", 0xFC000000, OP_ADDC_I, OC32_REGC | OC32_REGB | OC32_SIMM},
        {"SUB", 0xFFFF8000, OP_SUB_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"SUBB", 0xFFFF8000, OP_SUBB_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"SUBB", 0xFC000000, OP_SUBB_I, OC32_REGC | OC32_REGB | OC32_SIMM},
        {"MUL", 0xFFFF8000, OP_MUL_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"MUL", 0xFC000000, OP_MUL_I, OC32_REGC | OC32_REGB | OC32_SIMM},
        {"MULU", 0xFFFF8000, OP_MULU_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"MULU", 0xFC000000, OP_MULU_I, OC32_REGC | OC32_REGB | OC32_UIMM},
        {"DIV", 0xFFFF8000, OP_DIV_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"DIV", 0xFC000000, OP_DIV_I, OC32_REGC | OC32_REGB | OC32_SIMM},
        {"DIVU", 0xFFFF8000, OP_DIVU_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"DIVU", 0xFC000000, OP_DIVU_I, OC32_REGC | OC32_REGB | OC32_UIMM},

        /* Compare instructions */
        {"CMP", 0xFFFFFC00, OP_CMP_R, OC32_REGB | OC32_REGA},
        {"CMP", 0xFFE00000, OP_CMP_I, OC32_REGB | OC32_SIMM},
        {"CMPU", 0xFFFFFC00, OP_CMPU_R, OC32_REGB | OC32_REGA},
        {"CMPU", 0xFFE00000, OP_CMPU_I, OC32_REGB | OC32_UIMM},

        /* Logical instructions */
        {"AND", 0xFFFF8000, OP_AND_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"AND", 0xFC000000, OP_AND_I, OC32_REGC | OC32_REGB | OC32_UIMM},
        {"OR", 0xFFFF8000, OP_OR_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"OR", 0xFC000000, OP_OR_I, OC32_REGC | OC32_REGB | OC32_UIMM},
        {"XOR", 0xFFFF8000, OP_XOR_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"XOR", 0xFC000000, OP_XOR_I, OC32_REGC | OC32_REGB | OC32_UIMM},
        {"NAND", 0xFFFF8000, OP_NAND_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"NOR", 0xFFFF8000, OP_NOR_R, OC32_REGC | OC32_REGB | OC32_REGA},

        /* Move instructions */
        {"MOVZ", 0xFFFF8000, OP_MOVZ_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"MOVNZ", 0xFFFF8000, OP_MOVNZ_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"MOVH", 0xFFE00000, OP_MOVH_I, OC32_REGB | OC32_IMM},

        /* Load instructions */
        {"LW", 0xFFFF8000, OP_LW_R, OC32_REGC | OC32_REGA | OC32_REGB},
        {"LW", 0xFC000000, OP_LW_O, OC32_REGC | OC32_REGA | OC32_SIMM},
        {"LWI", 0xFFFF8000, OP_LWI_R, OC32_REGC | OC32_REGA | OC32_REGB},
        {"LWI", 0xFC000000, OP_LWI_O, OC32_REGC | OC32_REGA | OC32_SIMM},
        {"LWAS", 0xFC000000, OP_LWAS_O, OC32_REGC | OC32_REGA | OC32_SIMM},
        {"LB", 0xFFFF8000, OP_LB_R, OC32_REGC | OC32_REGA | OC32_REGB},
        {"LBU", 0xFFFF8000, OP_LBU_R, OC32_REGC | OC32_REGA | OC32_REGB},

        /* Store instructions */
        {"SW", 0xFFFF8000, OP_SW_R, OC32_REGA | OC32_REGB | OC32_REGC},
        {"SW", 0xFC000000, OP_SW_O, OC32_REGA | OC32_SIMM | OC32_REGC},
        {"SWI", 0xFFFF8000, OP_SWI_R, OC32_REGA | OC32_REGC | OC32_REGB},
        {"SWI", 0xFC000000, OP_SWI_O, OC32_REGA | OC32_REGC | OC32_SIMM},
        {"SWAT", 0xFC000000, OP_SWAT_O, OC32_REGA | OC32_SIMM | OC32_REGC},
        {"SB", 0xFFFF8000, OP_SB_R, OC32_REGA | OC32_REGB | OC32_REGC},

        /* Jump instructions */
        {"J", 0xF0000000, OP_J_P, OC32_REL},
        {"J", 0xFF800000, OP_J_O, OC32_ADDR},
        {"JZ", 0xFFFFFC00, OP_JZ_R, OC32_ADDR | OC32_REGB},
        {"JZ", 0xFF800000, OP_JZ_P, OC32_REGB | OC32_REL},
        {"JNZ", 0xFFFFFC00, OP_JNZ_R, OC32_ADDR | OC32_REGB},
        {"JNZ", 0xFF800000, OP_JNZ_P, OC32_REGB | OC32_REL},
        {"JL", 0xF0000000, OP_JL_P, OC32_REL},
        {"JL", 0xFF800000, OP_JL_O, OC32_ADDR},

        /* Shift instructions */
        {"RR", 0xFFFF8000, OP_RR_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"RR", 0xFFFF8000, OP_RR_N, OC32_REGC | OC32_REGB | OC32_SHIFT},
        {"SL", 0xFFFF8000, OP_SL_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"SL", 0xFFFF8000, OP_SL_N, OC32_REGC | OC32_REGB | OC32_SHIFT},
        {"SR", 0xFFFF8000, OP_SR_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"SR", 0xFFFF8000, OP_SR_N, OC32_REGC | OC32_REGB | OC32_SHIFT},
        {"SA", 0xFFFF8000, OP_SA_R, OC32_REGC | OC32_REGB | OC32_REGA},
        {"SA", 0xFFFF8000, OP_SA_N, OC32_REGC | OC32_REGB | OC32_SHIFT},

        /* Special instructions */
        {"MOV", 0xFFFFFFE0, OP_MOV_PSR, OC32_REGB},
        {"MOV", 0xFFFFFFE0, OP_MOV_EIA, OC32_REGB},
        {"RDSR", 0xFFE00000, OP_RDSR_A, OC32_REGC | OC32_ADDR},
        {"RDSR", 0xFFFFFC00, OP_RDSR_R, OC32_REGC | OC32_ADDR},
        {"WRSR", 0xFFE00000, OP_WRSR_A, OC32_ADDR | OC32_REGC},
        {"WRSR", 0xFFFFFC00, OP_WRSR_R, OC32_ADDR | OC32_REGC},
        {"SBSR", 0xFFE00000, OP_SBSR_A, OC32_ADDR | OC32_BIT},
        {"SBSR", 0xFFFFFC00, OP_SBSR_R, OC32_ADDR | OC32_BIT},
        {"CBSR", 0xFFE00000, OP_CBSR_A, OC32_ADDR | OC32_BIT},
        {"CBSR", 0xFFFFFC00, OP_CBSR_R, OC32_ADDR | OC32_BIT},
        {"TBSR", 0xFFE00000, OP_TBSR_A, OC32_ADDR | OC32_BIT},
        {"TBSR", 0xFFFFFC00, OP_TBSR_R, OC32_ADDR | OC32_BIT},
        {"EBF", 0xFFF00000, OP_EBF_R, OC32_REGC | OC32_REGB | OC32_BITFLD},
        {"MBF", 0xFFF00000, OP_MBF_R, OC32_REGC | OC32_REGB | OC32_BITFLD},
        {"CLO", 0xFFFFFC00, OP_CLO_R, OC32_REGC | OC32_REGB},
        {"CLZ", 0xFFFFFC00, OP_CLZ_R, OC32_REGC | OC32_REGB},
        {"SEB", 0xFFFFFC00, OP_SEB_R, OC32_REGC | OC32_REGB},
        {"SEH", 0xFFFFFC00, OP_SEH_R, OC32_REGC | OC32_REGB},
        {"RV", 0xFFFFFC00, OP_RV_R, OC32_REGC | OC32_REGB},
        {"RVB", 0xFFFFFC00, OP_RVB_R, OC32_REGC | OC32_REGB},
        {"RETE", 0xFFFFFFFF, OP_RETE, 0},
        {"SYSC", 0xFFFF0000, OP_SYSC, OC32_IMM},
        {"TRAPZ", 0xFFFFFFE0, OP_TRAPZ, OC32_REGB},
        {"TRAPNZ", 0xFFFFFFE0, OP_TRAPNZ, OC32_REGB},

        {NULL, 0, 0, 0}};
