/* Definitions to target GDB to OC32 targets.
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   With this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef GDB_OC32_TDEP_H
#define GDB_OC32_TDEP_H

#ifndef TARGET_OC32
#define TARGET_OC32
#endif

/* General Purpose Registers */
#define OC32_ZERO_REGNUM          0
#define OC32_PC_REGNUM            1
#define OC32_LR_REGNUM            7
#define OC32_SP_REGNUM            8
#define OC32_FIRST_ARG_REGNUM     9
#define OC32_LAST_ARG_REGNUM      14
#define OC32_FIRST_SAVED_REGNUM   15
#define OC32_RV_REGNUM            15
#define OC32_FP_REGNUM            31
#define OC32_PPC_REGNUM          (OC32_MAX_GPR_REGS + 0)
#define OC32_NPC_REGNUM          (OC32_MAX_GPR_REGS + 1)


/* Properties of the architecture. GDB mapping of registers is all the GPRs
   and SPRs followed by the PPC, NPC and SR at the end. Red zone is the area
   past the end of the stack reserved for exception handlers etc.  */

#define OC32_MAX_GPR_REGS            32
#define OC32_NUM_PSEUDO_REGS         0
#define OC32_NUM_REGS               (OC32_MAX_GPR_REGS + 2)
#define OC32_STACK_ALIGN             4
#define OC32_INSTLEN                 4
#define OC32_INSTBITLEN             (OC32_INSTLEN * 8)
#define OC32_NUM_TAP_RECORDS         8
#define OC32_FRAME_RED_ZONE_SIZE     2536

#define OC32_MAX_HW_BREAKPOINTS      2

/* Single step based on where the current instruction will take us.  */
extern std::vector<CORE_ADDR> oc32_software_single_step
  (struct regcache *regcache);

#endif /* GDB_OC32_TDEP_H */
