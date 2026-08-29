/* tc-oc32.h -- Header file for tc-oc32.c.

   Copyright (C) 2023-2024 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with GAS; see the file COPYING.  If not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#define TC_OC32 1
#define TARGET_BYTES_BIG_ENDIAN 0
#define WORKING_DOT_WORD

/* This macro is the BFD architecture to pass to `bfd_set_arch_mach'.  */
#define TARGET_FORMAT  "elf32-oc32"

#define TARGET_ARCH bfd_arch_oc32

/* Call md_pcrel_from_section(), not md_pcrel_from().  */
#define MD_PCREL_FROM_SECTION(FIX, SEC) md_pcrel_from_section (FIX, SEC)

/* Permit temporary numeric labels.  */
#define LOCAL_LABELS_FB 1

#define DIFF_EXPR_OK    1       /* .-foo gets turned into PC relative relocs.  */

/* Values passed to md_apply_fix don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

/* Enable cfi directives.  */
#define TARGET_USE_CFIPOP 1

/* Stack grows to lower addresses and wants 4 byte boundary.  */
#define DWARF2_CIE_DATA_ALIGNMENT -4

/* Define the column that represents the PC.  */
#define DWARF2_DEFAULT_RETURN_COLUMN 7  /* OC32 LR is R7 */

/* oc32 instructions are 4 bytes long.  */
#define DWARF2_LINE_MIN_INSN_LENGTH 4

#define tc_cfi_frame_initial_instructions oc32_cfi_frame_initial_instructions
extern void oc32_cfi_frame_initial_instructions (void);

/* If this macro returns non-zero, it guarantees that a relocation will be emitted
   even when the value can be resolved locally. Do that if linkrelax is turned on */
#define TC_FORCE_RELOCATION(fix)	oc32_force_relocation (fix)
#define TC_FORCE_RELOCATION_SUB_SAME(fix, seg) \
  (! SEG_NORMAL (seg) || oc32_force_relocation (fix))
extern int oc32_force_relocation (struct fix *);

#define TC_LINKRELAX_FIXUP(seg) \
  ((seg->flags & SEC_CODE) || (seg->flags & SEC_DEBUGGING))


/* This macro is evaluated for any fixup with a fx_subsy that
   fixup_segment cannot reduce to a number.  If the macro returns
   false an error will be reported. */
#define TC_VALIDATE_FIX_SUB(fix, seg)   oc32_validate_fix_sub (fix)
extern int oc32_validate_fix_sub (struct fix *);

/* The difference between same-section symbols may be affected by linker
   relaxation, so do not resolve such expressions in the assembler.  */
#define md_allow_local_subtract(l,r,s) oc32_allow_local_subtract (l, r, s)
extern bool oc32_allow_local_subtract (expressionS *, expressionS *, segT);

#define md_single_noop_insn "nop"

/* These macros must be defined, but it will be a fatal assembler error if we ever hit them. */
#define md_estimate_size_before_relax(A, B) \
  (as_fatal (_("OC32 does not support relaxation\n")), 0)

#define md_convert_frag(B, S, F) \
  (as_fatal (_("OC32 does not support relaxation\n")))

/* Default section alignment - no special alignment needed */
#define md_section_align(SEGMENT, SIZE)     (SIZE)

/* Default undefined symbol handling - use GAS default */
#define md_undefined_symbol(NAME)           0