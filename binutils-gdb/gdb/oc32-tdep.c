/* Target-dependent code for the OC32, for the GDB.
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "extract-store-integer.h"
#include "frame.h"
#include "inferior.h"
#include "symtab.h"
#include "value.h"
#include "cli/cli-cmds.h"
#include "language.h"
#include "gdbcore.h"
#include "symfile.h"
#include "objfiles.h"
#include "gdbtypes.h"
#include "target.h"
#include "regcache.h"
#include "reggroups.h"
#include "arch-utils.h"
#include "frame-unwind.h"
#include "frame-base.h"
#include "dwarf2/frame.h"
#include "trad-frame.h"
#include "regset.h"
#include "remote.h"
#include "target-descriptions.h"
#include <inttypes.h>
#include "dis-asm.h"
#include "gdbarch.h"

/*  specific includes.  */
#include "oc32-tdep.h"
#include "features/oc32.c"

/* Global debug flag.  */

static bool oc32_debug = false;

static void
show_oc32_debug(struct ui_file *file, int from_tty,
                struct cmd_list_element *c, const char *value)
{
  gdb_printf(file, _(" debugging is %s.\n"), value);
}

/* The target-dependent structure for gdbarch.  */

struct oc32_gdbarch_tdep : gdbarch_tdep_base
{
  int bytes_per_word = 0;
  int bytes_per_address = 0;
};

/* Support functions for the architecture definition.  */

/* Get an instruction from memory.  */

static ULONGEST
oc32_fetch_instruction(struct gdbarch *gdbarch, CORE_ADDR addr)
{
  enum bfd_endian byte_order = gdbarch_byte_order(gdbarch);
  gdb_byte buf[OC32_INSTLEN];

  if (target_read_code(addr, buf, OC32_INSTLEN))
  {
    memory_error(TARGET_XFER_E_IO, addr);
  }

  return extract_unsigned_integer(buf, OC32_INSTLEN, byte_order);
}

/* Generic function to read bits from an instruction.  */

static bool
oc32_analyse_inst(uint32_t inst, const char *format, ...)
{
  /* Break out each field in turn, validating as we go.  */
  va_list ap;
  int i;
  int iptr = 0; /* Instruction pointer */

  va_start(ap, format);

  for (i = 0; 0 != format[i];)
  {
    const char *start_ptr;
    char *end_ptr;

    uint32_t bits;  /* Bit substring of interest */
    uint32_t width; /* Substring width */
    uint32_t *arg_ptr;

    switch (format[i])
    {
    case ' ':
      i++;
      break; /* Formatting: ignored */

    case '0':
    case '1': /* Constant bit field */
      bits = (inst >> (OC32_INSTBITLEN - iptr - 1)) & 0x1;

      if ((format[i] - '0') != bits)
        return false;

      iptr++;
      i++;
      break;

    case '%': /* Bit field */
      i++;
      start_ptr = &(format[i]);
      width = strtoul(start_ptr, &end_ptr, 10);

      /* Check we got something, and if so skip on.  */
      if (start_ptr == end_ptr)
        error(_("bitstring \"%s\" at offset %d has no length field."),
              format, i);

      i += end_ptr - start_ptr;

      /* Look for and skip the terminating 'b'.  If it's not there, we
         still give a fatal error, because these are fixed strings that
         just should not be wrong.  */
      if ('b' != format[i++])
        error(_("bitstring \"%s\" at offset %d has no terminating 'b'."),
              format, i);

      /* Break out the field.  There is a special case with a bit width
         of 32.  */
      if (32 == width)
        bits = inst;
      else
        bits =
            (inst >> (OC32_INSTBITLEN - iptr - width)) & ((1 << width) - 1);

      arg_ptr = va_arg(ap, uint32_t *);
      *arg_ptr = bits;
      iptr += width;
      break;

    default:
      error(_("invalid character in bitstring \"%s\" at offset %d."),
            format, i);
      break;
    }
  }

  /* Is the length OK?  */
  gdb_assert(OC32_INSTBITLEN == iptr);

  return true; /* Success */
}

/* This is used to parse l.addi instructions during various prologue
   analysis routines.  The l.addi instruction has semantics:

     assembly:        ADD  RB, RA, simm16
     implementation:  RB = RA + sign_extend(simm16)

   The rb_ptr, ra_ptr and simm_ptr must be non NULL pointers and are used
   to store the parse results.  Upon successful parsing true is returned,
   false on failure. */

static bool
oc32_analyse_add(uint32_t inst, unsigned int *rb_ptr,
                 unsigned int *ra_ptr, int *simm_ptr)
{
  /* Instruction fields */
  uint32_t rb, ra, i;

  if (oc32_analyse_inst(inst, "10 0100 %16b %5b %5b", &i, &rb, &ra))
  {
    /* Found it.  Construct the result fields.  */
    *rb_ptr = (unsigned int)rb;
    *ra_ptr = (unsigned int)ra;
    *simm_ptr = (int)(((i & 0x8000) == 0x8000) ? 0xffff0000 | i : i);

    return true; /* Success */
  }
  else
    return false; /* Failure */
}

/* This is used to to parse store instructions during various prologue
   analysis routines.  The l.sw instruction has semantics:

     assembly:        SW  [RA+soff18], RB
     implementation:  store RB contents to memory at effective address of
          RA + sign_extend(soff18)

   The simm_ptr, ra_ptr and rb_ptr must be non NULL pointers and are used
   to store the parse results. Upon successful parsing true is returned,
   false on failure. */

static bool
oc32_analyse_sw(uint32_t inst, int *simm_ptr, unsigned int *ra_ptr,
                unsigned int *rb_ptr)
{
  /* Instruction fields */
  uint32_t i, ra, rb;

  if (oc32_analyse_inst(inst, "00 1001 %16b %5b %5b", &i, &rb, &ra))

  {
    /* Found it.  Construct the result fields.  */
    *simm_ptr = (int)((((i & 0x8000) == 0x8000) ? 0xffff0000 | i : i) << 2);

    *ra_ptr = (unsigned int)ra;
    *rb_ptr = (unsigned int)rb;

    return true; /* Success */
  }
  else
    return false; /* Failure */
}

/* Functions defining the architecture.  */

/* Implement the return_value gdbarch method.  */

static enum return_value_convention
oc32_return_value(struct gdbarch *gdbarch, struct value *functype,
                  struct type *valtype, struct regcache *regcache,
                  gdb_byte *readbuf, const gdb_byte *writebuf)
{
  enum bfd_endian byte_order = gdbarch_byte_order(gdbarch);
  enum type_code rv_type = valtype->code();
  unsigned int rv_size = valtype->length();
  oc32_gdbarch_tdep *tdep = gdbarch_tdep<oc32_gdbarch_tdep>(gdbarch);
  int bpw = tdep->bytes_per_word;

  /* Deal with struct/union as addresses.  If an array won't fit in a
     single register it is returned as address.  Anything larger than 2
     registers needs to also be passed as address (matches gcc
     default_return_in_memory).  */
  if ((TYPE_CODE_STRUCT == rv_type) || (TYPE_CODE_UNION == rv_type) || ((TYPE_CODE_ARRAY == rv_type) && (rv_size > bpw)) || (rv_size > 2 * bpw))
  {
    if (readbuf != NULL)
    {
      ULONGEST tmp;

      regcache_cooked_read_unsigned(regcache, OC32_RV_REGNUM, &tmp);
      read_memory(tmp, readbuf, rv_size);
    }
    if (writebuf != NULL)
    {
      ULONGEST tmp;

      regcache_cooked_read_unsigned(regcache, OC32_RV_REGNUM, &tmp);
      write_memory(tmp, writebuf, rv_size);
    }

    return RETURN_VALUE_ABI_RETURNS_ADDRESS;
  }

  if (rv_size <= bpw)
  {
    /* Up to one word scalars are returned in R15.  */
    if (readbuf != NULL)
    {
      ULONGEST tmp;

      regcache_cooked_read_unsigned(regcache, OC32_RV_REGNUM, &tmp);
      store_unsigned_integer(readbuf, rv_size, byte_order, tmp);
    }
    if (writebuf != NULL)
    {
      gdb_byte *buf = XCNEWVEC(gdb_byte, bpw);

      if (BFD_ENDIAN_BIG == byte_order)
        memcpy(buf + (sizeof(gdb_byte) * bpw) - rv_size, writebuf,
               rv_size);
      else
        memcpy(buf, writebuf, rv_size);

      regcache->cooked_write(OC32_RV_REGNUM, buf);

      free(buf);
    }
  }
  else
  {
    /* 2 word scalars are returned in r15/r16 (with the MS word in r15).  */
    if (readbuf != NULL)
    {
      ULONGEST tmp_lo;
      ULONGEST tmp_hi;
      ULONGEST tmp;

      regcache_cooked_read_unsigned(regcache, OC32_RV_REGNUM,
                                    &tmp_hi);
      regcache_cooked_read_unsigned(regcache, OC32_RV_REGNUM + 1,
                                    &tmp_lo);
      tmp = (tmp_hi << (bpw * 8)) | tmp_lo;

      store_unsigned_integer(readbuf, rv_size, byte_order, tmp);
    }
    if (writebuf != NULL)
    {
      gdb_byte *buf_lo = XCNEWVEC(gdb_byte, bpw);
      gdb_byte *buf_hi = XCNEWVEC(gdb_byte, bpw);

      /* This is cheating.  We assume that we fit in 2 words exactly,
         which wouldn't work if we had (say) a 6-byte scalar type on a
         big endian architecture (with the  1000 usually is).  */
      memcpy(buf_hi, writebuf, rv_size - bpw);
      memcpy(buf_lo, writebuf + bpw, bpw);

      regcache->cooked_write(OC32_RV_REGNUM, buf_hi);
      regcache->cooked_write(OC32_RV_REGNUM + 1, buf_lo);

      free(buf_lo);
      free(buf_hi);
    }
  }

  return RETURN_VALUE_REGISTER_CONVENTION;
}

/* OR1K always uses a SYSC 0xFFFF instruction for breakpoints.  */

constexpr gdb_byte oc32_break_insn[] = {0xFF, 0x70, 0xFF, 0xFF};

typedef BP_MANIPULATION (oc32_break_insn) oc32_breakpoint;


/* oc32_software_single_step() is called just before we want to resume
   the inferior, if we want to single-step it but there is no hardware
   or kernel single-step support */

std::vector<CORE_ADDR>
oc32_software_single_step (struct regcache *regcache)
{
  //struct gdbarch *gdbarch = regcache->arch ();
  CORE_ADDR pc, next_pc;

  pc = regcache_read_pc (regcache);
  next_pc = pc + 4;

  return {next_pc};
}

/* Name for oc32 general registers.  */

static const char *const oc32_reg_names[OC32_NUM_REGS] = {
    /* general purpose registers */
    "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15",
    "R16", "R17", "R18", "R19", "R20", "R21", "R22", "R23",
    "R24", "R25", "R26", "R27", "R28", "R29", "R30", "R31",

    /* previous program counter, next program counter */
    "PPC", "NPC"};

static int
oc32_is_arg_reg(unsigned int regnum)
{
  return (OC32_FIRST_ARG_REGNUM <= regnum) && (regnum <= OC32_LAST_ARG_REGNUM);
}

static int
oc32_is_callee_saved_reg(unsigned int regnum)
{
  return (OC32_FIRST_SAVED_REGNUM <= regnum) && (0 == regnum % 2);
}

/* Implement the skip_prologue gdbarch method.  */

static CORE_ADDR
oc32_skip_prologue(struct gdbarch *gdbarch, CORE_ADDR pc)
{
  CORE_ADDR start_pc;
  CORE_ADDR addr;
  uint32_t inst;

  unsigned int ra, rb, rd; /* for instruction analysis */
  int simm;

  int frame_size = 0;

  /* Try using SAL first if we have symbolic information available.  This
     only works for DWARF 2, not STABS.  */

  if (find_pc_partial_function(pc, NULL, &start_pc, NULL))
  {
    CORE_ADDR prologue_end = skip_prologue_using_sal(gdbarch, pc);

    if (0 != prologue_end)
    {
      struct symtab_and_line prologue_sal = find_sal_for_pc(start_pc, 0);
      struct compunit_symtab *compunit = prologue_sal.symtab->compunit();
      const char *debug_format = compunit->debugformat();

      if ((NULL != debug_format) && (strlen("dwarf") <= strlen(debug_format)) && (0 == strncasecmp("dwarf", debug_format, strlen("dwarf"))))
        return (prologue_end > pc) ? prologue_end : pc;
    }
  }

  /* Look to see if we can find any of the standard prologue sequence.  All
     quite difficult, since any or all of it may be missing.  So this is
     just a best guess!  */

  addr = pc; /* Where we have got to */
  inst = oc32_fetch_instruction(gdbarch, addr);

  /* Look for the new stack pointer being set up.  */
  if (oc32_analyse_add(inst, &rd, &ra, &simm) && (OC32_SP_REGNUM == rd) && (OC32_SP_REGNUM == ra) && (simm < 0) && (0 == (simm % 4)))
  {
    frame_size = -simm;
    addr += OC32_INSTLEN;
    inst = oc32_fetch_instruction(gdbarch, addr);
  }

  /* Look for the frame pointer being manipulated.  */
  if (oc32_analyse_sw(inst, &simm, &ra, &rb) && (OC32_SP_REGNUM == ra) && (OC32_FP_REGNUM == rb) && (simm >= 0) && (0 == (simm % 4)))
  {
    addr += OC32_INSTLEN;
    inst = oc32_fetch_instruction(gdbarch, addr);

    gdb_assert(oc32_analyse_add(inst, &rd, &ra, &simm) && (OC32_FP_REGNUM == rd) && (OC32_SP_REGNUM == ra) && (simm == frame_size));

    addr += OC32_INSTLEN;
    inst = oc32_fetch_instruction(gdbarch, addr);
  }

  /* Look for the link register being saved.  */
  if (oc32_analyse_sw(inst, &simm, &ra, &rb) && (OC32_SP_REGNUM == ra) && (OC32_LR_REGNUM == rb) && (simm >= 0) && (0 == (simm % 4)))
  {
    addr += OC32_INSTLEN;
    inst = oc32_fetch_instruction(gdbarch, addr);
  }

  /* Look for arguments or callee-saved register being saved.  The register
     must be one of the arguments (r9-r15) or the callee saved registers.  The base
     register must be the FP (for the args) or the SP (for the callee_saved
     registers).  */
  while (1)
  {
    if (oc32_analyse_sw(inst, &simm, &ra, &rb) && (((OC32_FP_REGNUM == ra) && oc32_is_arg_reg(rb)) || ((OC32_SP_REGNUM == ra) && oc32_is_callee_saved_reg(rb))) && (0 == (simm % 4)))
    {
      addr += OC32_INSTLEN;
      inst = oc32_fetch_instruction(gdbarch, addr);
    }
    else
    {
      /* Nothing else to look for.  We have found the end of the
         prologue.  */
      break;
    }
  }
  return addr;
}

/* Implement the frame_align gdbarch method.  */

static CORE_ADDR
oc32_frame_align(struct gdbarch *gdbarch, CORE_ADDR sp)
{
  return align_down(sp, OC32_STACK_ALIGN);
}

/* Implement the unwind_pc gdbarch method.  */

static CORE_ADDR
oc32_unwind_pc(struct gdbarch *gdbarch, const frame_info_ptr &next_frame)
{
  CORE_ADDR pc;

  if (oc32_debug)
    gdb_printf(gdb_stdlog, "oc32_unwind_pc, next_frame=%d\n",
               frame_relative_level(next_frame));

  pc = frame_unwind_register_unsigned(next_frame, OC32_NPC_REGNUM);

  if (oc32_debug)
    gdb_printf(gdb_stdlog, "oc32_unwind_pc, pc=%s\n",
               paddress(gdbarch, pc));

  return pc;
}

/* Implement the unwind_sp gdbarch method.  */

static CORE_ADDR
oc32_unwind_sp(struct gdbarch *gdbarch, const frame_info_ptr &next_frame)
{
  CORE_ADDR sp;

  if (oc32_debug)
    gdb_printf(gdb_stdlog, "oc32_unwind_sp, next_frame=%d\n",
               frame_relative_level(next_frame));

  sp = frame_unwind_register_unsigned(next_frame, OC32_SP_REGNUM);

  if (oc32_debug)
    gdb_printf(gdb_stdlog, "oc32_unwind_sp, sp=%s\n",
               paddress(gdbarch, sp));

  return sp;
}

/* Implement the push_dummy_code gdbarch method.  */

static CORE_ADDR
oc32_push_dummy_code(struct gdbarch *gdbarch, CORE_ADDR sp,
                     CORE_ADDR function, struct value **args, int nargs,
                     struct type *value_type, CORE_ADDR *real_pc,
                     CORE_ADDR *bp_addr, struct regcache *regcache)
{
  CORE_ADDR bp_slot;

  /* Reserve enough room on the stack for our breakpoint instruction.  */
  bp_slot = sp - 4;
  /* Store the address of that breakpoint.  */
  *bp_addr = bp_slot;
  /* keeping the stack aligned.  */
  sp = oc32_frame_align(gdbarch, bp_slot);
  /* The call starts at the callee's entry point.  */
  *real_pc = function;

  return sp;
}

/* Implement the push_dummy_call gdbarch method.  */

static CORE_ADDR
oc32_push_dummy_call(struct gdbarch *gdbarch, struct value *function,
                     struct regcache *regcache, CORE_ADDR bp_addr,
                     int nargs, struct value **args, CORE_ADDR sp,
                     function_call_return_method return_method,
                     CORE_ADDR struct_addr)
{

  int argreg;
  int argnum;
  int first_stack_arg;
  int stack_offset = 0;
  int heap_offset = 0;
  CORE_ADDR heap_sp = sp - 128;
  enum bfd_endian byte_order = gdbarch_byte_order(gdbarch);
  oc32_gdbarch_tdep *tdep = gdbarch_tdep<oc32_gdbarch_tdep>(gdbarch);
  int bpa = tdep->bytes_per_address;
  int bpw = tdep->bytes_per_word;
  struct type *func_type = function->type();

  /* Return address */
  regcache_cooked_write_unsigned(regcache, OC32_LR_REGNUM, bp_addr);

  /* Register for the next argument.  */
  argreg = OC32_FIRST_ARG_REGNUM;

  /* Location for a returned structure.  This is passed as a silent first
     argument.  */
  if (return_method == return_method_struct)
  {
    regcache_cooked_write_unsigned(regcache, OC32_FIRST_ARG_REGNUM,
                                   struct_addr);
    argreg++;
  }

  /* Put as many args as possible in registers.  */
  for (argnum = 0; argnum < nargs; argnum++)
  {
    const gdb_byte *val;
    gdb_byte valbuf[sizeof(ULONGEST)];

    struct value *arg = args[argnum];
    struct type *arg_type = check_typedef(arg->type());
    int len = arg_type->length();
    enum type_code typecode = arg_type->code();

    if (func_type->has_varargs() && argnum >= func_type->num_fields())
      break; /* end or regular args, varargs go to stack.  */

    /* Extract the value, either a reference or the data.  */
    if ((TYPE_CODE_STRUCT == typecode) || (TYPE_CODE_UNION == typecode) || (len > bpw * 2))
    {
      CORE_ADDR valaddr = arg->address();

      /* If the arg is fabricated (i.e. 3*i, instead of i) valaddr is
         undefined.  */
      if (valaddr == 0)
      {
        /* The argument needs to be copied into the target space.
     Since the bottom of the stack is reserved for function
     arguments we store this at the these at the top growing
     down.  */
        heap_offset += align_up(len, bpw);
        valaddr = heap_sp + heap_offset;

        write_memory(valaddr, arg->contents().data(), len);
      }

      /* The ABI passes all structures by reference, so get its
         address.  */
      store_unsigned_integer(valbuf, bpa, byte_order, valaddr);
      len = bpa;
      val = valbuf;
    }
    else
    {
      /* Everything else, we just get the value.  */
      val = arg->contents().data();
    }

    /* Stick the value in a register.  */
    if (len > bpw)
    {
      /* Big scalars use two registers, but need NOT be pair aligned.  */

      if (argreg <= (OC32_LAST_ARG_REGNUM - 1))
      {
        ULONGEST regval = extract_unsigned_integer(val, len,
                                                   byte_order);

        unsigned int bits_per_word = bpw * 8;
        ULONGEST mask = (((ULONGEST)1) << bits_per_word) - 1;
        ULONGEST lo = regval & mask;
        ULONGEST hi = regval >> bits_per_word;

        regcache_cooked_write_unsigned(regcache, argreg, hi);
        regcache_cooked_write_unsigned(regcache, argreg + 1, lo);
        argreg += 2;
      }
      else
      {
        /* Run out of regs */
        break;
      }
    }
    else if (argreg <= OC32_LAST_ARG_REGNUM)
    {
      /* Smaller scalars fit in a single register.  */
      regcache_cooked_write_unsigned(regcache, argreg, extract_unsigned_integer(val, len, byte_order));
      argreg++;
    }
    else
    {
      /* Ran out of regs.  */
      break;
    }
  }

  first_stack_arg = argnum;

  /* If we get here with argnum < nargs, then arguments remain to be
     placed on the stack.  This is tricky, since they must be pushed in
     reverse order and the stack in the end must be aligned.  The only
     solution is to do it in two stages, the first to compute the stack
     size, the second to save the args.  */

  for (argnum = first_stack_arg; argnum < nargs; argnum++)
  {
    struct value *arg = args[argnum];
    struct type *arg_type = check_typedef(arg->type());
    int len = arg_type->length();
    enum type_code typecode = arg_type->code();

    if ((TYPE_CODE_STRUCT == typecode) || (TYPE_CODE_UNION == typecode) || (len > bpw * 2))
    {
      /* Structures are passed as addresses.  */
      sp -= bpa;
    }
    else
    {
      /* Big scalars use more than one word.  Code here allows for
         future quad-word entities (e.g. long double.)  */
      sp -= align_up(len, bpw);
    }

    /* Ensure our dummy heap doesn't touch the stack, this could only
 happen if we have many arguments including fabricated arguments.  */
    gdb_assert(heap_offset == 0 || ((heap_sp + heap_offset) < sp));
  }

  sp = gdbarch_frame_align(gdbarch, sp);
  stack_offset = 0;

  /* Push the remaining args on the stack.  */
  for (argnum = first_stack_arg; argnum < nargs; argnum++)
  {
    const gdb_byte *val;
    gdb_byte valbuf[sizeof(ULONGEST)];

    struct value *arg = args[argnum];
    struct type *arg_type = check_typedef(arg->type());
    int len = arg_type->length();
    enum type_code typecode = arg_type->code();
    /* The EABI passes structures that do not fit in a register by
 reference.  In all other cases, pass the structure by value.  */
    if ((TYPE_CODE_STRUCT == typecode) || (TYPE_CODE_UNION == typecode) || (len > bpw * 2))
    {
      store_unsigned_integer(valbuf, bpa, byte_order,
                             arg->address());
      len = bpa;
      val = valbuf;
    }
    else
      val = arg->contents().data();

    while (len > 0)
    {
      int partial_len = (len < bpw ? len : bpw);

      write_memory(sp + stack_offset, val, partial_len);
      stack_offset += align_up(partial_len, bpw);
      len -= partial_len;
      val += partial_len;
    }
  }

  /* Save the updated stack pointer.  */
  regcache_cooked_write_unsigned(regcache, OC32_SP_REGNUM, sp);

  if (heap_offset > 0)
    sp = heap_sp;

  return sp;
}

/* Support functions for frame handling.  */

/* Initialize a prologue cache

   We build a cache, saying where registers of the prev frame can be found
   from the data so far set up in this this.

   We also compute a unique ID for this frame, based on the function start
   address and the stack pointer (as it will be, even if it has yet to be
   computed.

   STACK FORMAT
   ============

   The oc32 has a falling stack frame and a simple prolog.  The Stack
   pointer is R1 and the frame pointer R2.  The frame base is therefore the
   address held in R2 and the stack pointer (R1) is the frame base of the
   next frame.

   ADD  R8,R8,-frame_size	# SP now points to end of new stack frame

   The stack pointer may not be set up in a frameless function (e.g. a
   simple leaf function).

   SW    fp_loc(R8),R31        # old FP saved in new stack frame
   ADD  R31,R8,frame_size     # FP now points to base of new stack frame

   The frame pointer is not necessarily saved right at the end of the stack
   frame - oc32 saves enough space for any args to called functions right
   at the end (this is a difference from the Architecture Manual).

   SW    lr_loc(R8),R7        # Link (return) address

   The link register is usually saved at fp_loc - 4.  It may not be saved at
   all in a leaf function.

   SW    reg_loc(R8),ry       # Save any callee saved regs

   The offsets x for the callee saved registers generally (always?) rise in
   increments of 4, starting at fp_loc + 4.  If the frame pointer is
   omitted (an option to GCC), then it may not be saved at all.  There may
   be no callee saved registers.

   So in summary none of this may be present.  However what is present
   seems always to follow this fixed order, and occur before any
   substantive code (it is possible for GCC to have more flexible
   scheduling of the prologue, but this does not seem to occur for oc32).

   ANALYSIS
   ========

   This prolog is used, even for -O3 with GCC.

   All this analysis must allow for the possibility that the PC is in the
   middle of the prologue.  Data in the cache should only be set up insofar
   as it has been computed.

   HOWEVER.  The frame_id must be created with the SP *as it will be* at
   the end of the Prologue.  Otherwise a recursive call, checking the frame
   with the PC at the start address will end up with the same frame_id as
   the caller.

   A suite of "helper" routines are used, allowing reuse for
   oc32_skip_prologue().

   Reportedly, this is only valid for frames less than 0x7fff in size.  */

static struct trad_frame_cache *
oc32_frame_cache(const frame_info_ptr &this_frame, void **prologue_cache)
{
  struct gdbarch *gdbarch;
  struct trad_frame_cache *info;

  CORE_ADDR this_pc;
  CORE_ADDR this_sp;
  CORE_ADDR this_sp_for_id;
  int frame_size = 0;

  CORE_ADDR start_addr;
  CORE_ADDR end_addr;

  if (oc32_debug)
    gdb_printf(gdb_stdlog,
               "oc32_frame_cache, prologue_cache = %s\n",
               host_address_to_string(*prologue_cache));

  /* Nothing to do if we already have this info.  */
  if (NULL != *prologue_cache)
    return (struct trad_frame_cache *)*prologue_cache;

  /* Get a new prologue cache and populate it with default values.  */
  info = trad_frame_cache_zalloc(this_frame);
  *prologue_cache = info;

  /* Find the start address of this function (which is a normal frame, even
     if the next frame is the sentinel frame) and the end of its prologue.  */
  this_pc = get_frame_pc(this_frame);
  find_pc_partial_function(this_pc, NULL, &start_addr, NULL);

  /* Get the stack pointer if we have one (if there's no process executing
     yet we won't have a frame.  */
  this_sp = (NULL == this_frame) ? 0 : get_frame_register_unsigned(this_frame, OC32_SP_REGNUM);

  /* Return early if GDB couldn't find the function.  */
  if (start_addr == 0)
  {
    if (oc32_debug)
      gdb_printf(gdb_stdlog, "  couldn't find function\n");

    /* JPB: 28-Apr-11.  This is a temporary patch, to get round GDB
 crashing right at the beginning.  Build the frame ID as best we
 can.  */
    trad_frame_set_id(info, frame_id_build(this_sp, this_pc));

    return info;
  }

  /* The default frame base of this frame (for ID purposes only - frame
     base is an overloaded term) is its stack pointer.  For now we use the
     value of the SP register in this frame.  However if the PC is in the
     prologue of this frame, before the SP has been set up, then the value
     will actually be that of the prev frame, and we'll need to adjust it
     later.  */
  trad_frame_set_this_base(info, this_sp);
  this_sp_for_id = this_sp;

  /* The default is to find the PC of the previous frame in the link
     register of this frame.  This may be changed if we find the link
     register was saved on the stack.  */
  trad_frame_set_reg_realreg(info, OC32_NPC_REGNUM, OC32_LR_REGNUM);

  /* We should only examine code that is in the prologue.  This is all code
     up to (but not including) end_addr.  We should only populate the cache
     while the address is up to (but not including) the PC or end_addr,
     whichever is first.  */
  gdbarch = get_frame_arch(this_frame);
  end_addr = oc32_skip_prologue(gdbarch, start_addr);

  /* All the following analysis only occurs if we are in the prologue and
     have executed the code.  Check we have a sane prologue size, and if
     zero we are frameless and can give up here.  */
  if (end_addr < start_addr)
    error(_("end addr %s is less than start addr %s"),
          paddress(gdbarch, end_addr), paddress(gdbarch, start_addr));

  if (end_addr == start_addr)
    frame_size = 0;
  else
  {
    /* We have a frame.  Look for the various components.  */
    CORE_ADDR addr = start_addr; /* Where we have got to */
    uint32_t inst = oc32_fetch_instruction(gdbarch, addr);

    unsigned int ra, rb, rd; /* for instruction analysis */
    int simm;

    /* Look for the new stack pointer being set up.  */
    if (oc32_analyse_add(inst, &rd, &ra, &simm) && (OC32_SP_REGNUM == rd) && (OC32_SP_REGNUM == ra) && (simm < 0) && (0 == (simm % 4)))
    {
      frame_size = -simm;
      addr += OC32_INSTLEN;
      inst = oc32_fetch_instruction(gdbarch, addr);

      /* If the PC has not actually got to this point, then the frame
         base will be wrong, and we adjust it.

         If we are past this point, then we need to populate the stack
         accordingly.  */
      if (this_pc <= addr)
      {
        /* Only do if executing.  */
        if (0 != this_sp)
        {
          this_sp_for_id = this_sp + frame_size;
          trad_frame_set_this_base(info, this_sp_for_id);
        }
      }
      else
      {
        /* We are past this point, so the stack pointer of the prev
     frame is frame_size greater than the stack pointer of this
     frame.  */
        trad_frame_set_reg_value(info, OC32_SP_REGNUM,
                                 this_sp + frame_size);
      }
    }

    /* From now on we are only populating the cache, so we stop once we
 get to either the end OR the current PC.  */
    end_addr = (this_pc < end_addr) ? this_pc : end_addr;

    /* Look for the frame pointer being manipulated.  */
    if ((addr < end_addr) && oc32_analyse_sw(inst, &simm, &ra, &rb) && (OC32_SP_REGNUM == ra) && (OC32_FP_REGNUM == rb) && (simm >= 0) && (0 == (simm % 4)))
    {
      addr += OC32_INSTLEN;
      inst = oc32_fetch_instruction(gdbarch, addr);

      /* At this stage, we can find the frame pointer of the previous
         frame on the stack of the current frame.  */
      trad_frame_set_reg_addr(info, OC32_FP_REGNUM, this_sp + simm);

      /* Look for the new frame pointer being set up.  */
      if ((addr < end_addr) && oc32_analyse_add(inst, &rd, &ra, &simm) && (OC32_FP_REGNUM == rd) && (OC32_SP_REGNUM == ra) && (simm == frame_size))
      {
        addr += OC32_INSTLEN;
        inst = oc32_fetch_instruction(gdbarch, addr);

        /* If we have got this far, the stack pointer of the previous
     frame is the frame pointer of this frame.  */
        trad_frame_set_reg_realreg(info, OC32_SP_REGNUM,
                                   OC32_FP_REGNUM);
      }
    }

    /* Look for the link register being saved.  */
    if ((addr < end_addr) && oc32_analyse_sw(inst, &simm, &ra, &rb) && (OC32_SP_REGNUM == ra) && (OC32_LR_REGNUM == rb) && (simm >= 0) && (0 == (simm % 4)))
    {
      addr += OC32_INSTLEN;
      inst = oc32_fetch_instruction(gdbarch, addr);

      /* If the link register is saved in the this frame, it holds the
         value of the PC in the previous frame.  This overwrites the
         previous information about finding the PC in the link
         register.  */
      trad_frame_set_reg_addr(info, OC32_NPC_REGNUM, this_sp + simm);
    }

    /* Look for arguments or callee-saved register being saved.  The
 register must be one of the arguments (r3-r8) or the 10 callee
 saved registers (r10, r12, r14, r16, r18, r20, r22, r24, r26, r28,
 r30).  The base register must be the FP (for the args) or the SP
 (for the callee_saved registers).  */
    while (addr < end_addr)
    {
      if (oc32_analyse_sw(inst, &simm, &ra, &rb) && (((OC32_FP_REGNUM == ra) && oc32_is_arg_reg(rb)) || ((OC32_SP_REGNUM == ra) && oc32_is_callee_saved_reg(rb))) && (0 == (simm % 4)))
      {
        addr += OC32_INSTLEN;
        inst = oc32_fetch_instruction(gdbarch, addr);

        /* The register in the previous frame can be found at this
     location in this frame.  */
        trad_frame_set_reg_addr(info, rb, this_sp + simm);
      }
      else
        break; /* Not a register save instruction.  */
    }
  }

  /* Build the frame ID */
  trad_frame_set_id(info, frame_id_build(this_sp_for_id, start_addr));

  if (oc32_debug)
  {
    gdb_printf(gdb_stdlog, "  this_sp_for_id = %s\n",
               paddress(gdbarch, this_sp_for_id));
    gdb_printf(gdb_stdlog, "  start_addr     = %s\n",
               paddress(gdbarch, start_addr));
  }

  return info;
}

/* Implement the this_id function for the stub unwinder.  */

static void
oc32_frame_this_id(const frame_info_ptr &this_frame,
                   void **prologue_cache, struct frame_id *this_id)
{
  struct trad_frame_cache *info = oc32_frame_cache(this_frame,
                                                   prologue_cache);

  trad_frame_get_id(info, this_id);
}

/* Implement the prev_register function for the stub unwinder.  */

static struct value *
oc32_frame_prev_register(const frame_info_ptr &this_frame,
                         void **prologue_cache, int regnum)
{
  struct trad_frame_cache *info = oc32_frame_cache(this_frame,
                                                   prologue_cache);

  return trad_frame_get_register(info, this_frame, regnum);
}

/* Data structures for the normal prologue-analysis-based unwinder.  */

static const struct frame_unwind_legacy oc32_frame_unwind(
    "oc32 prologue",
    NORMAL_FRAME,
    FRAME_UNWIND_ARCH,
    default_frame_unwind_stop_reason,
    oc32_frame_this_id,
    oc32_frame_prev_register,
    NULL,
    default_frame_sniffer,
    NULL);

/* Architecture initialization */

static struct gdbarch *
oc32_gdbarch_init(struct gdbarch_info info, struct gdbarch_list *arches)
{
  const struct bfd_arch_info *binfo;
  tdesc_arch_data_up tdesc_data;
  const struct target_desc *tdesc = info.target_desc;

  /* Find a candidate among the list of pre-declared architectures.  */
  arches = gdbarch_list_lookup_by_info(arches, &info);
  if (NULL != arches)
    return arches->gdbarch;

  /* None found, create a new architecture from the information
     provided.  Can't initialize all the target dependencies until we
     actually know which target we are talking to, but put in some defaults
     for now.  */
  binfo = info.bfd_arch_info;
  gdbarch *gdbarch = gdbarch_alloc(&info, gdbarch_tdep_up(new oc32_gdbarch_tdep));
  oc32_gdbarch_tdep *tdep = gdbarch_tdep<oc32_gdbarch_tdep>(gdbarch);

  tdep->bytes_per_word = binfo->bits_per_word / binfo->bits_per_byte;
  tdep->bytes_per_address = binfo->bits_per_address / binfo->bits_per_byte;

  /* Target data types */
  set_gdbarch_short_bit(gdbarch, 16);
  set_gdbarch_int_bit(gdbarch, 32);
  set_gdbarch_long_bit(gdbarch, 32);
  set_gdbarch_long_long_bit(gdbarch, 64);
  set_gdbarch_float_bit(gdbarch, 32);
  set_gdbarch_float_format(gdbarch, floatformats_ieee_single);
  set_gdbarch_double_bit(gdbarch, 64);
  set_gdbarch_double_format(gdbarch, floatformats_ieee_double);
  set_gdbarch_long_double_bit(gdbarch, 64);
  set_gdbarch_long_double_format(gdbarch, floatformats_ieee_double);
  set_gdbarch_ptr_bit(gdbarch, binfo->bits_per_address);
  set_gdbarch_addr_bit(gdbarch, binfo->bits_per_address);
  set_gdbarch_char_signed(gdbarch, 1);

  /* Information about the target architecture */
  set_gdbarch_return_value(gdbarch, oc32_return_value);
  set_gdbarch_breakpoint_kind_from_pc(gdbarch, oc32_breakpoint::kind_from_pc);
  set_gdbarch_sw_breakpoint_from_kind(gdbarch, oc32_breakpoint::bp_from_kind);
  set_gdbarch_have_nonsteppable_watchpoint(gdbarch, 0);

  /* Register architecture */
  set_gdbarch_num_regs(gdbarch, OC32_NUM_REGS);
  set_gdbarch_num_pseudo_regs(gdbarch, OC32_NUM_PSEUDO_REGS);
  set_gdbarch_sp_regnum(gdbarch, OC32_SP_REGNUM);
  set_gdbarch_pc_regnum(gdbarch, OC32_PC_REGNUM);
  set_gdbarch_deprecated_fp_regnum(gdbarch, OC32_FP_REGNUM);

  /* Functions to analyse frames */
  set_gdbarch_skip_prologue(gdbarch, oc32_skip_prologue);
  set_gdbarch_inner_than(gdbarch, core_addr_lessthan);
  set_gdbarch_frame_align(gdbarch, oc32_frame_align);
  set_gdbarch_frame_red_zone_size(gdbarch, OC32_FRAME_RED_ZONE_SIZE);

  /* Functions to access frame data */
  set_gdbarch_unwind_pc(gdbarch, oc32_unwind_pc);
  set_gdbarch_unwind_sp(gdbarch, oc32_unwind_sp);

  /* Functions handling dummy frames */
  set_gdbarch_call_dummy_location(gdbarch, ON_STACK);
  set_gdbarch_push_dummy_code(gdbarch, oc32_push_dummy_code);
  set_gdbarch_push_dummy_call(gdbarch, oc32_push_dummy_call);

  /* Frame unwinders.  Use DWARF debug info if available, otherwise use our
     own unwinder.  */
  dwarf2_append_unwinders(gdbarch);
  frame_unwind_append_unwinder(gdbarch, &oc32_frame_unwind);

  if (!tdesc_has_registers(info.target_desc))
    /* Pick a default target description.  */
    tdesc = tdesc_oc32.get();

  /* Check any target description for validity.  */
  if (tdesc_has_registers(tdesc))
  {
    const struct tdesc_feature *feature;
    int valid_p;
    int i;

    feature = tdesc_find_feature(tdesc, "org.gnu.gdb.oc32.group0");
    if (feature == NULL)
      return NULL;

    tdesc_data = tdesc_data_alloc();

    valid_p = 1;

    for (i = 0; i < OC32_NUM_REGS; i++)
      valid_p &= tdesc_numbered_register(feature, tdesc_data.get(), i,
                                         oc32_reg_names[i]);

    if (!valid_p)
      return NULL;
  }

  if (tdesc_data != NULL)
    tdesc_use_registers(gdbarch, tdesc, std::move(tdesc_data));

  /* Hook in ABI-specific overrides, if they have been registered.  */
  gdbarch_init_osabi(info, gdbarch);

  return gdbarch;
}

/* Dump the target specific data for this architecture.  */

static void
oc32_dump_tdep(struct gdbarch *gdbarch, struct ui_file *file)
{
  oc32_gdbarch_tdep *tdep = gdbarch_tdep<oc32_gdbarch_tdep>(gdbarch);

  if (NULL == tdep)
    return; /* Nothing to report */

  gdb_printf(file, "oc32_dump_tdep: %d bytes per word\n",
             tdep->bytes_per_word);
  gdb_printf(file, "oc32_dump_tdep: %d bytes per address\n",
             tdep->bytes_per_address);
}

INIT_GDB_FILE(oc32_tdep)
{
  /* Register this architecture.  */
  gdbarch_register(bfd_arch_oc32, oc32_gdbarch_init, oc32_dump_tdep);

  initialize_tdesc_oc32();

  /* Debugging flag.  */
  add_setshow_boolean_cmd("oc32", class_maintenance, &oc32_debug,
                          _("Set  debugging."),
                          _("Show  debugging."),
                          _("When on,  specific debugging is enabled."),
                          NULL,
                          show_oc32_debug,
                          &setdebuglist, &showdebuglist);
}
