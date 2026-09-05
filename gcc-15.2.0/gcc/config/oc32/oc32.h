/* Target Definitions for oc32.
   Copyright (C) 2015-2023 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_OC32_H
#define GCC_OC32_H

#include "config/oc32/oc32-opts.h"

/* Run-time Target Specification */
#define TARGET_CPU_CPP_BUILTINS() \
  { \
    builtin_define ("__OC32__");          \
  }

/* for bare-metal without newlib */
#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crti-hw.o%s"


#define TARGET_CMODEL_SMALL \
  (oc32_code_model == CMODEL_SMALL)
#define TARGET_CMODEL_LARGE \
  (oc32_code_model == CMODEL_LARGE)

/* Storage Layout */
#define BITS_BIG_ENDIAN 0
#define BYTES_BIG_ENDIAN 0
#define WORDS_BIG_ENDIAN 0
#define DEFAULT_SIGNED_CHAR 1
#define BITS_PER_WORD 32
#define UNITS_PER_WORD 4
#define POINTER_SIZE 32
#define BIGGEST_ALIGNMENT 32
#define STRICT_ALIGNMENT 1
#define FUNCTION_BOUNDARY 32
#define PARM_BOUNDARY 32
#define STACK_BOUNDARY 32
#define PREFERRED_STACK_BOUNDARY 32
#define MAX_FIXED_MODE_SIZE 64
/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY  32
/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 32


/* Align definitions of arrays, unions and structures so that
   initializations and copies can be made more efficient.  This is not
   ABI-changing, so it only affects places where we can see the
   definition. Increasing the alignment tends to introduce padding,
   so don't do this when optimizing for size/conserving stack space. */
#define OC32_EXPAND_ALIGNMENT(COND, EXP, ALIGN)				\
  (((COND) && ((ALIGN) < BITS_PER_WORD)					\
    && (TREE_CODE (EXP) == ARRAY_TYPE					\
	|| TREE_CODE (EXP) == UNION_TYPE				\
	|| TREE_CODE (EXP) == RECORD_TYPE)) ? BITS_PER_WORD : (ALIGN))

/* Make arrays of chars word-aligned for the same reasons.  */
#define DATA_ALIGNMENT(TYPE, ALIGN)             \
  (TREE_CODE (TYPE) == ARRAY_TYPE               \
   && TYPE_MODE (TREE_TYPE (TYPE)) == QImode    \
   && (ALIGN) < FASTEST_ALIGNMENT ? FASTEST_ALIGNMENT : (ALIGN))

/* Similarly, make sure that objects on the stack are sensibly aligned.  */
#define LOCAL_ALIGNMENT(EXP, ALIGN)				\
  OC32_EXPAND_ALIGNMENT(/*!flag_conserve_stack*/ 1, EXP, ALIGN)

/* Layout of Source Language Data Types */
#define INT_TYPE_SIZE 32
#define SHORT_TYPE_SIZE 16
#define LONG_TYPE_SIZE 32
#define LONG_LONG_TYPE_SIZE 64
#define WCHAR_TYPE_SIZE 32

#undef SIZE_TYPE
#define SIZE_TYPE "unsigned int"

#undef PTRDIFF_TYPE
#define PTRDIFF_TYPE "int"

#undef WCHAR_TYPE
#define WCHAR_TYPE "unsigned int"

/* The maximum number of bytes that a single instruction can move
   quickly between memory and registers or between two memory
   locations.  */
#define MOVE_MAX 4
#define SLOW_BYTE_ACCESS 1


#define OC32_R0     0
#define OC32_R1     1
#define OC32_R2     2
#define OC32_R3     3
#define OC32_R4     4
#define OC32_R5     5
#define OC32_R6     6
#define OC32_R7     7
#define OC32_R8     8
#define OC32_R9     9
#define OC32_R10    10
#define OC32_R11    11
#define OC32_R12    12
#define OC32_R13    13
#define OC32_R14    14
#define OC32_R15    15
#define OC32_R16    16
#define OC32_R17    17
#define OC32_R18    18
#define OC32_R19    19
#define OC32_R20    20
#define OC32_R21    21
#define OC32_R22    22
#define OC32_R23    23
#define OC32_R24    24
#define OC32_R25    25
#define OC32_R26    26
#define OC32_R27    27
#define OC32_R28    28
#define OC32_R29    29
#define OC32_R30    30
#define OC32_R31    31
#define OC32_SAP    32
#define OC32_SFP    33
   
/* Register aliases */
#define OC32_PC     OC32_R1  /* Program Counter */
#define OC32_LR     OC32_R7  /* Link Register */
#define OC32_SP     OC32_R8  /* Stack Pointer */
#define OC32_HFP    OC32_R31 /* HardFrame Pointer */
#define OC32_TMP    OC32_R6

/* First register used for passing arguments */
#define OC32_FIRST_ARG_REG     OC32_R9

/* Return value register */
#define OC32_RV     OC32_R15

#define OC32_TLS    OC32_R17

/* SR(special register) address range */
#define OC32_SRADR_START 0x3FF00000ULL
#define OC32_SRADR_END   0x3FF3FFFFULL
enum oc32_emit_sr_binop_result {
        OC32_SR_BINOP_FAIL = 0,
        OC32_SR_BINOP_OK
};

/* Virtual registers for frame and argument pointers */
#define FIRST_PSEUDO_REGISTER 34

#define REGISTER_NAMES {        \
  "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", \
  "R9", "R10", "R11", "R12", "R13", "R14", "R15", "R16", "R17", \
  "R18", "R19", "R20", "R21", "R22", "R23", "R24", "R25", "R26", \
  "R27", "R28", "R29", "R30", "R31", "SFP", "SAP" }

#define FIXED_REGISTERS  /*  r0  r1  r2  r3 */   { 1, 1, 1, 1, \
                         /*  r4  r5  r6  r7 */     1, 1, 1, 0, \
                         /*  r8  r9 r10 r11 */     1, 0, 0, 0, \
                         /* r12 r13 r14 r15 */     0, 0, 0, 0, \
                         /* r16 r17 r18 r19 */     0, 0, 0, 0, \
                         /* r20 r21 r22 r23 */     0, 0, 0, 0, \
                         /* r24 r25 r26 r27 */     0, 0, 0, 0, \
                         /* r28 r29 r30 r31 */     0, 0, 0, 0, \
                         /* SAP SFP         */     1, 1}

/* Caller saved/temporary registers + args(R9-R14) + fixed */
#define CALL_USED_REGISTERS \
                         /*  r0  r1  r2  r3 */   { 1, 1, 1, 1, \
                         /*  r4  r5  r6  r7 */     1, 1, 1, 1, \
                         /*  r8  r9 r10 r11 */     1, 1, 1, 1, \
                         /* r12 r13 r14 r15 */     1, 1, 1, 1, \
                         /* r16 r17 r18 r19 */     1, 1, 1, 1, \
                         /* r20 r21 r22 r23 */     1, 1, 1, 1, \
                         /* r24 r25 r26 r27 */     0, 0, 0, 0, \
                         /* r28 r29 r30 r31 */     0, 0, 0, 0, \
			                /* SAP SFP         */     1, 1 }


/* Register allocation order - prefer caller-saved registers first */
#define REG_ALLOC_ORDER { \
  15, 14, 13, 12, 11, 10, 9,		/* caller-saved, args, return value */ \
  16, 17, 18, 19, 20, 21, 22, 23,  	/* caller-saved */ \
  24, 25, 26, 27, 28, 29, 30, 31,  		/* callee-saved */ \
  7,		\
  33, 32, 8, 6, 5, 4, 3, 2, 1, 0}

enum reg_class
{
  NO_REGS,
  SIBCALL_REGS,
  GOT_REGS,
  GENERAL_REGS,
  FLAG_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define N_REG_CLASSES LIM_REG_CLASSES
#define REG_CLASS_NAMES {	\
  "NO_REGS",		\
  "SIBCALL_REGS",		\
  "GOT_REGS",			\
  "GENERAL_REGS",	\
  "FLAG_REGS",			\
  "ALL_REGS" }


/* The SIBCALL_REGS must be call-clobbered, and not used as a temporary
   in the epilogue.  This excludes R7 (LR), R8 (SP), R15 (STATIC_CHAIN), 
   and R6 (TMP).  */
#define SIBCALL_REGS_MASK  0x00FF7E00

#define REG_CLASS_CONTENTS \
{ { 0x00000000,  0x00000000 }, /* Empty */                      \
  { SIBCALL_REGS_MASK,   0  },	\
  { 0xFFFFFF00,  0x00000003 },  \
  { 0xFFFFFFFF,  0x00000003 }, /* General */ \
  { 0x0000003C,  0x00000000 }, /* flag */ \
  { 0xFFFFFFFF,  0x00000003 }  /* All registers */              \
}

/* A C expression whose value is a register class containing hard
   register REGNO.  In general there is more that one such class;
   choose a class which is "minimal", meaning that no smaller class
   also contains the register.  */
#define REGNO_REG_CLASS(REGNO) \
  ((REGNO) < 6 && (REGNO) > 1 ? FLAG_REGS \
   : (REGNO) < 32 && ((SIBCALL_REGS_MASK >> (REGNO)) & 1) ? SIBCALL_REGS \
   : GENERAL_REGS)

#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE)               \
do {                                                    \
  if (GET_MODE_CLASS (MODE) == MODE_INT                 \
      && GET_MODE_SIZE (MODE) < UNITS_PER_WORD)         \
    (MODE) = word_mode;                                 \
} while (0)

/* A macro whose definition is the name of the class to which a valid
   base register must belong.  A base register is one used in an
   address which is the register value plus a displacement.  */
#define BASE_REG_CLASS GENERAL_REGS

#define INDEX_REG_CLASS NO_REGS

/* The Overall Framework of an Assembler File */

#define ASM_APP_ON ""
#define ASM_APP_OFF ""

#define ASM_COMMENT_START "#"

/* Output and Generation of Labels */

#define GLOBAL_ASM_OP "\t.global\t"
#define TEXT_SECTION_ASM_OP "\t.section\t.text"
#define DATA_SECTION_ASM_OP "\t.section\t.data"
#define BSS_SECTION_ASM_OP "\t.section\t.bss"
#define SBSS_SECTION_ASM_OP "\t.section\t.sbss"

/* This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  */
#define ASM_OUTPUT_ALIGN(FILE,LOG)			\
  do							\
    {							\
      if ((LOG) != 0)					\
	fprintf (FILE, "\t.align %d\n", (LOG));	\
    }							\
  while (0)

  /* This is used in crtstuff to create call stubs in the
   _init() and _fini() functions.  Defining this here saves
   a few bytes created by the dummy call_xxx() functions.  */
#define CRT_CALL_STATIC_FUNCTION(SECTION_OP, FUNC)	\
  asm (SECTION_OP "\n"					\
"	JL " #FUNC "\n"				\
"	.previous");

#define PRINT_OPERAND_PUNCT_VALID_P(CODE) (code == '#')

/* Calling convention definitions.  */
#define CUMULATIVE_ARGS int
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, FNDECL, N_NAMED_ARGS) \
  do { (CUM) = 0; } while (0)


/* Trampolines for Nested Functions. 5 instructions */
#define TRAMPOLINE_SIZE 20

/* Alignment required for trampolines, in bits.  */
#define TRAMPOLINE_ALIGNMENT 32

/* An alias for the machine mode for pointers.  */
#define Pmode         SImode
#define FUNCTION_MODE SImode
#define STACK_POINTER_REGNUM OC32_SP
#define FRAME_POINTER_REGNUM OC32_SFP
#define HARD_FRAME_POINTER_REGNUM OC32_HFP
#define STATIC_CHAIN_REGNUM    OC32_R15

/* The register number of the arg pointer register, which is used to
   access the function's argument list.  */
#define ARG_POINTER_REGNUM OC32_SAP

/* Position Independent Code.  See oc32_init_pic_reg.  */
#define REAL_PIC_OFFSET_TABLE_REGNUM  OC32_R24

/* ??? Follow i386 in working around gimple costing estimation, which
   happens without properly initializing the pic_offset_table pseudo.  */
#define PIC_OFFSET_TABLE_REGNUM \
  (pic_offset_table_rtx ? INVALID_REGNUM : REAL_PIC_OFFSET_TABLE_REGNUM)

/* A C expression that is nonzero if REGNO is the number of a hard
   register in which function arguments are sometimes passed.  */
#define FUNCTION_ARG_REGNO_P(r) (r >= OC32_FIRST_ARG_REG && r <= OC32_R14)

/* A number, the maximum number of registers that can appear in a
   valid memory address.  */
#define MAX_REGS_PER_ADDRESS 2

/* The ELIMINABLE_REGS macro specifies a table of register pairs used to
   eliminate unneeded registers that point into the stack frame. Note,
   the only elimination attempted by the compiler is to replace references
   to the frame pointer with references to the stack pointer.  */
#define ELIMINABLE_REGS					\
{{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM },	\
 { FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM },	\
 { ARG_POINTER_REGNUM,   STACK_POINTER_REGNUM },	\
 { ARG_POINTER_REGNUM,   HARD_FRAME_POINTER_REGNUM }}


/* This macro returns the initial difference between the specified pair
   of registers.  */
#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)                    \
  do {                                                                  \
    (OFFSET) = oc32_initial_elimination_offset ((FROM), (TO));            \
  } while (0)

/* A C expression which is nonzero if register number NUM is suitable
   for use as an index register in operand addresses.  */
#define REGNO_OK_FOR_INDEX_P(REGNO) ((REGNO) > 5)
#define REGNO_OK_FOR_BASE_P(REGNO)  ((REGNO) > 5)

/* If defined, the maximum amount of space required for outgoing arguments
   will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* This plus ARG_POINTER_REGNUM points to the first word of incoming args.  */
#define FIRST_PARM_OFFSET(FNDECL) (0)

/* This plus STACK_POINTER_REGNUM points to the first work of outgoing args.  */
#define STACK_POINTER_OFFSET (0)

/* Define this macro if pushing a word onto the stack moves the stack
   pointer to a smaller address.  */
#define STACK_GROWS_DOWNWARD 1

/* Define this macro to nonzero value if the addresses of local variable slots
   are at negative offsets from the frame pointer.  */
#define FRAME_GROWS_DOWNWARD 1

/* An alias for a machine mode name.  This is the machine mode that
   elements of a jump-table should have.  */
#define CASE_VECTOR_MODE SImode

#define STORE_FLAG_VALUE 1

/* load operations zero extend.  */
#define LOAD_EXTEND_OP(MODE) (ZERO_EXTEND)

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK 1

/* Macros related to the access of the stack frame chain.  */
#define INITIAL_FRAME_ADDRESS_RTX  oc32_initial_frame_addr ()
#define DYNAMIC_CHAIN_ADDRESS      oc32_dynamic_chain_addr
#define RETURN_ADDR_RTX            oc32_return_addr

/* Always pass the SYMBOL_REF for direct calls to the expanders.  */
#define NO_FUNCTION_CSE 1

#define NO_PROFILE_COUNTERS 1

/* Emit rtl for profiling.  Output assembler code to call "_mcount" for
   profiling a function entry.  */
#define PROFILE_HOOK(LABEL)  oc32_profile_hook()

/* All the work is done in PROFILE_HOOK, but this is still required.  */
#define FUNCTION_PROFILER(STREAM, LABELNO) do { } while (0)

/* A C expression that returns the debugger register number for the compiler
   register number REGNO.  In simple cases, the value of this expression may be
   REGNO itself.  But sometimes there are some registers that the compiler
   knows about and debugger does not, or vice versa.  In such cases, some register
   may need to have one number in the compiler and another for debugger.

   If two registers have consecutive numbers inside GCC, and they can be
   used as a pair to hold a multiword value, then they *must* have consecutive
   numbers after renumbering with `DEBUGGER_REGNO'.  Otherwise, debuggers
   will be unable to access such a pair, because they expect register pairs to
   be consecutive in their own numbering scheme.

   If you find yourself defining `DEBUGGER_REGNO' in way that does not
   preserve register pairs, then what you must do instead is redefine the
   actual register numbering scheme.

   This declaration is required.  */
#define DEBUGGER_REGNO(REGNO) (REGNO)

#undef  PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

#define DWARF2_DEBUGGING_INFO 1
/* Pick up the return address upon entry to a procedure. Used for
   dwarf2 unwind information.  This also enables the table driven
   mechanism.  */
#define INCOMING_RETURN_ADDR_RTX	gen_rtx_REG (Pmode, OC32_LR)
#define DWARF_FRAME_RETURN_COLUMN OC32_LR

/* Describe how we implement __builtin_eh_return.  */
#define EH_RETURN_REGNUM OC32_R23
#define EH_RETURN_DATA_REGNO(N) \
  ((N) < 4 ? (N) + OC32_R19 : INVALID_REGNUM)
#define EH_RETURN_STACKADJ_RTX  gen_rtx_REG (Pmode, EH_RETURN_REGNUM)

#define ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL) \
  ((GLOBAL) ? DW_EH_PE_indirect : 0) | DW_EH_PE_pcrel | DW_EH_PE_sdata4


#endif /* GCC_OC32_H */
