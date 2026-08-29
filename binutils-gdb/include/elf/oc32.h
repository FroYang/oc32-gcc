/* oc32 ELF support for BFD.
   Copyright (C) 2009-2023 Free Software Foundation, Inc.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef _ELF_OC32_H
#define _ELF_OC32_H

#include "elf/reloc-macros.h"


/* Relocation types.  */
START_RELOC_NUMBERS (elf_oc32_reloc_type)
  /* No relocation.  */
  RELOC_NUMBER (R_OC32_NONE, 0)

  /* Instruction related relocations (for code).  */
  RELOC_NUMBER (R_OC32_I_REL30, 1)  /* J/JL rel30 */
  RELOC_NUMBER (R_OC32_I_CON20, 2)  /* JZ/JNZ rel20 */

  /* Data related relocations (for data).  */
  RELOC_NUMBER (R_OC32_D_ABSLO, 3)  /* OR/ADD Ra, Ra, imm */
  RELOC_NUMBER (R_OC32_D_ABSHI, 4)  /* MOVH Ra, imm */
  RELOC_NUMBER (R_OC32_D_ABSHA, 5)  /* MOVH Ra, imm */
  RELOC_NUMBER (R_OC32_D_GOTLO, 6)  /* OR/ADD Ra, Ra, imm */
  RELOC_NUMBER (R_OC32_D_GOTHI, 7)  /* MOVH Ra, imm */
  RELOC_NUMBER (R_OC32_32, 8)  /* 32-bit address */


END_RELOC_NUMBERS (R_OC32_max)

#endif /* _ELF_OC32_H */
