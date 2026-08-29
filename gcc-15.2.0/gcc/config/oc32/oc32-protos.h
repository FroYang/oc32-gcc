/* Prototypes for oc32.cc functions used in the md file & elsewhere.
   Copyright (C) 2015-2023 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

extern void        oc32_expand_prologue (void);
extern void        oc32_expand_epilogue (void);
extern rtx         oc32_return_addr (int, rtx);
extern void        oc32_expand_eh_return (rtx);
extern HOST_WIDE_INT oc32_initial_elimination_offset (int, int);
extern void        oc32_print_operand (FILE *, rtx, int);
extern void        oc32_print_operand_address (FILE *, machine_mode, rtx);
extern void        oc32_expand_call (rtx, rtx, rtx, bool);
extern void        oc32_expand_cbranch (rtx *);
extern void        oc32_expand_cstore (rtx *);
extern void        oc32_expand_move (machine_mode, rtx *);
extern rtx         oc32_dynamic_chain_addr (rtx);
extern void        oc32_profile_hook (void);
extern rtx         oc32_initial_frame_addr (void);
extern bool        oc32_branch_in_range_p ();

#ifdef RTX_CODE
void oc32_expand_atomic_compare_and_swap (rtx operands[]);
void oc32_expand_atomic_compare_and_swap_qihi (rtx operands[]);
void oc32_expand_atomic_exchange (rtx operands[]);
void oc32_expand_atomic_exchange_qihi (rtx operands[]);
void oc32_expand_atomic_op (rtx_code, rtx, rtx, rtx, rtx);
void oc32_expand_atomic_op_qihi (rtx_code, rtx, rtx, rtx, rtx);
#endif