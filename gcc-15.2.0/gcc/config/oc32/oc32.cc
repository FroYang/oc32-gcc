/* Target Code for oc32
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

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "df.h"
#include "regs.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "diagnostic-core.h"
#include "output.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
#include "expr.h"
#include "builtins.h"
#include "optabs.h"
#include "explow.h"
#include "cfgrtl.h"
#include "alias.h"
#include "targhooks.h"
#include "case-cfn-macros.h"

/* These 4 are needed to allow using satisfies_constraint_J.  */
#include "insn-config.h"
#include "recog.h"
#include "tm_p.h"
#include "tm-constrs.h"

/* This file should be included last.  */
#include "target-def.h"

#include "tree.h"      // 需要 DECL_SECTION_NAME, SYMBOL_REF_DECL
#include "varasm.h"    // 需要 current_function_section, section

/* Per-function machine data.  */
struct GTY(()) machine_function
{
        /* Number of bytes saved on the stack for callee saved registers.  */
        HOST_WIDE_INT callee_saved_reg_size;

        /* Number of bytes saved on the stack for local variables.  */
        HOST_WIDE_INT local_vars_size;

        /* Number of bytes saved on the stack for outgoing/sub-function args.  */
        HOST_WIDE_INT args_size;

        /* The sum of sizes: locals vars, called saved regs, stack pointer
           and an optional frame pointer.
           Used in expand_prologue () and expand_epilogue ().  */
        HOST_WIDE_INT total_size;

        /* Remember where the set_got_placeholder is located.  */
        rtx_insn *set_got_insn;

        /* Remember where mcount args are stored so we can insert set_got_insn
           after.  */
        rtx_insn *set_mcount_arg_insn;
};

/* Zero initialization is OK for all current fields.  */

static struct machine_function *
oc32_init_machine_status(void)
{
        return ggc_cleared_alloc<machine_function>();
}

/* The TARGET_OPTION_OVERRIDE worker.
   All this curently does is set init_machine_status.  */
static void
oc32_option_override(void)
{
        /* Set the per-function-data initializer.  */
        init_machine_status = oc32_init_machine_status;
}

/* Returns true if REGNO must be saved for the current function.  */

static bool
callee_saved_regno_p(int regno)
{
        /* Check call-saved registers.  */
        if (!call_used_or_fixed_reg_p(regno) && df_regs_ever_live_p(regno))
                return true;

        switch (regno)
        {
        case HARD_FRAME_POINTER_REGNUM:
                return frame_pointer_needed;

        case OC32_LR:
                /* Always save LR if we are saving HFP, producing a walkable
                   stack chain with -fno-omit-frame-pointer.  */
                return (frame_pointer_needed || !crtl->is_leaf || crtl->uses_pic_offset_table || df_regs_ever_live_p(regno));

        case OC32_R20:
        case OC32_R21:
        case OC32_R22:
        case OC32_R23:
                /* See EH_RETURN_DATA_REGNO.  */
                return crtl->calls_eh_return;

        default:
                return false;
        }
}

/* Worker for TARGET_COMPUTE_FRAME_LAYOUT.
   Compute and populate machine specific function attributes which are globally
   accessible via cfun->machine.  These include the sizes needed for
   stack stored local variables, callee saved registers and space for stack
   arguments which may be passed to a next function.  The values are used for
   the epilogue, prologue and eliminations.

   stack grows downwards and contains:

    ---- previous frame --------
    current func arg[n]
    current func arg[0]   <-- r31 [HFP,AP]
    ---- current stack frame ---  ^  ---\
    return address      r7        |     |
    old frame pointer   r31       (+)    |-- machine->total_size
    callee saved regs             |     | > machine->callee_saved_reg_size
    local variables               |     | > machine->local_vars_size       <-FP
    next function args    <-- r8 [SP]---/ > machine->args_size
    ----------------------------  |
                                 (-)
           (future)               |
                                  V

   All of these contents are optional.  */

static void
oc32_compute_frame_layout(void)
{
        HOST_WIDE_INT local_vars_size, args_size, save_reg_size;

        local_vars_size = get_frame_size();
        local_vars_size = ROUND_UP(local_vars_size, UNITS_PER_WORD);

        args_size = crtl->outgoing_args_size;
        args_size = ROUND_UP(args_size, UNITS_PER_WORD);

        save_reg_size = 0;
        for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
                if (callee_saved_regno_p(regno))
                        save_reg_size += UNITS_PER_WORD;

        cfun->machine->local_vars_size = local_vars_size;
        cfun->machine->args_size = args_size;
        cfun->machine->callee_saved_reg_size = save_reg_size;
        cfun->machine->total_size = save_reg_size + local_vars_size + args_size;
}

/* Emit rtl to save register REGNO contents to stack memory at the given OFFSET
   from the current stack pointer.  */

static void
oc32_save_reg(int regno, HOST_WIDE_INT offset)
{
        rtx reg = gen_rtx_REG(Pmode, regno);
        rtx mem = gen_frame_mem(SImode, plus_constant(Pmode, stack_pointer_rtx, offset));
        rtx insn = emit_move_insn(mem, reg);
        RTX_FRAME_RELATED_P(insn) = 1;
}

/* Emit rtl to restore register REGNO contents from stack memory at the given
   OFFSET from the current stack pointer.  */

static rtx
oc32_restore_reg(int regno, HOST_WIDE_INT offset, rtx cfa_restores)
{
        rtx reg = gen_rtx_REG(Pmode, regno);
        rtx mem = gen_frame_mem(SImode, plus_constant(Pmode, stack_pointer_rtx, offset));
        emit_move_insn(reg, mem);
        return alloc_reg_note(REG_CFA_RESTORE, reg, cfa_restores);
}

/* Expand the "prologue" pattern.  */
void oc32_expand_prologue(void)
{
        HOST_WIDE_INT sp_offset = -cfun->machine->total_size;
        HOST_WIDE_INT reg_offset, this_offset;
        rtx insn;

        if (flag_stack_usage_info)
                current_function_static_stack_size = -sp_offset;

        /* Early exit for frameless functions.  */
        if (sp_offset == 0)
                goto fini;

        /* Adjust the stack pointer.  For large stack offsets we will
           do this in multiple parts, before and after saving registers.  */
        reg_offset = (sp_offset + cfun->machine->local_vars_size + cfun->machine->args_size);
        this_offset = MAX(sp_offset, -32764);
        reg_offset -= this_offset;
        sp_offset -= this_offset;

        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx, stack_pointer_rtx,
                                          GEN_INT(this_offset)));
        RTX_FRAME_RELATED_P(insn) = 1;

        /* Save callee-saved registers.  */
        for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
                if (regno != HARD_FRAME_POINTER_REGNUM && regno != OC32_LR && callee_saved_regno_p(regno))
                {
                        oc32_save_reg(regno, reg_offset);
                        reg_offset += UNITS_PER_WORD;
                }

        /* Save and update frame pointer.  */
        if (callee_saved_regno_p(HARD_FRAME_POINTER_REGNUM))
        {
                oc32_save_reg(HARD_FRAME_POINTER_REGNUM, reg_offset);
                if (frame_pointer_needed)
                {
                        insn = emit_insn(gen_addsi3(hard_frame_pointer_rtx,
                                                    stack_pointer_rtx,
                                                    GEN_INT(-this_offset)));
                        RTX_FRAME_RELATED_P(insn) = 1;
                }
                reg_offset += UNITS_PER_WORD;
        }

        /* Save the link register.  */
        if (callee_saved_regno_p(OC32_LR))
        {
                oc32_save_reg(OC32_LR, reg_offset);
                reg_offset += UNITS_PER_WORD;
        }
        gcc_assert(reg_offset + this_offset == 0);

        /* Allocate the rest of the stack frame, if any.  */
        if (sp_offset != 0)
        {
                if (sp_offset < -32768)
                {
                        /* For very large offsets, we need a temporary register.  */
                        rtx tmp = gen_rtx_REG(Pmode, OC32_R6);
                        emit_insn(gen_movsi_simm(tmp, GEN_INT(sp_offset)));
                        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx,
                                                          stack_pointer_rtx, tmp));
                        if (!frame_pointer_needed)
                        {
                                RTX_FRAME_RELATED_P(insn) = 1;
                                add_reg_note(insn, REG_CFA_ADJUST_CFA,
                                             gen_rtx_SET(stack_pointer_rtx,
                                                         plus_constant(Pmode,
                                                                       stack_pointer_rtx,
                                                                       sp_offset)));
                        }
                }
                else
                {
                        this_offset = MAX(sp_offset, -32768);
                        sp_offset -= this_offset;

                        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx,
                                                          stack_pointer_rtx,
                                                          GEN_INT(this_offset)));
                        if (!frame_pointer_needed)
                                RTX_FRAME_RELATED_P(insn) = 1;
                }
        }

fini:
        /* Fix up, or remove, the insn that initialized the pic register.  */
        rtx_insn *set_got_insn = cfun->machine->set_got_insn;
        if (crtl->uses_pic_offset_table)
        {
                rtx reg = SET_DEST(PATTERN(set_got_insn));
                rtx_insn *insn = emit_insn_before(gen_set_got(reg), set_got_insn);
                RTX_FRAME_RELATED_P(insn) = 1;
                add_reg_note(insn, REG_CFA_FLUSH_QUEUE, NULL_RTX);
        }
        delete_insn(set_got_insn);
}

/* Expand the "epilogue" pattern.  */
void oc32_expand_epilogue(void)
{
        HOST_WIDE_INT reg_offset, sp_offset;
        rtx insn, cfa_restores = NULL;

        sp_offset = cfun->machine->total_size;
        if (sp_offset == 0)
                return;

        reg_offset = cfun->machine->local_vars_size + cfun->machine->args_size;

        if (sp_offset >= 32768 || cfun->calls_alloca)
        {
                /* The saved registers are out of range of the stack pointer.
             We need to partially deallocate the stack frame now.  */
                /* Reset the stack pointer to the bottom of the saved regs.  */
                if (frame_pointer_needed)
                {
                        sp_offset -= reg_offset;
                        reg_offset = 0;
                        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx,
                                                          hard_frame_pointer_rtx,
                                                          GEN_INT(-sp_offset)));
                        RTX_FRAME_RELATED_P(insn) = 1;
                        add_reg_note(insn, REG_CFA_DEF_CFA,
                                     plus_constant(Pmode, stack_pointer_rtx, sp_offset));
                }
                else if (sp_offset >= 32768)
                {
                        rtx tmp = gen_rtx_REG(Pmode, OC32_R6);
                        emit_insn(gen_movsi_uimm(tmp, GEN_INT(reg_offset)));
                        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx,
                                                          stack_pointer_rtx, tmp));
                        sp_offset -= reg_offset;
                        reg_offset = 0;
                        RTX_FRAME_RELATED_P(insn) = 1;
                        add_reg_note(insn, REG_CFA_DEF_CFA,
                                     plus_constant(Pmode, stack_pointer_rtx, sp_offset));
                }
                else
                {
                        HOST_WIDE_INT this_offset = MIN(reg_offset, 32764);
                        reg_offset -= this_offset;
                        sp_offset -= this_offset;

                        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx,
                                                          stack_pointer_rtx,
                                                          GEN_INT(this_offset)));
                        RTX_FRAME_RELATED_P(insn) = 1;
                        add_reg_note(insn, REG_CFA_DEF_CFA,
                                     plus_constant(Pmode, stack_pointer_rtx,
                                                   sp_offset));
                }
        }

        /* Restore callee-saved registers.  */
        for (int regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
                if (regno != HARD_FRAME_POINTER_REGNUM && regno != OC32_LR && callee_saved_regno_p(regno))
                {
                        cfa_restores = oc32_restore_reg(regno, reg_offset, cfa_restores);
                        reg_offset += UNITS_PER_WORD;
                }

        /* Restore frame pointer.  */
        if (callee_saved_regno_p(HARD_FRAME_POINTER_REGNUM))
        {
                cfa_restores = oc32_restore_reg(HARD_FRAME_POINTER_REGNUM,
                                                reg_offset, cfa_restores);
                reg_offset += UNITS_PER_WORD;
        }

        /* Restore link register.  */
        if (callee_saved_regno_p(OC32_LR))
        {
                cfa_restores = oc32_restore_reg(OC32_LR, reg_offset, cfa_restores);
                reg_offset += UNITS_PER_WORD;
        }
        gcc_assert(reg_offset == sp_offset);

        /* Restore stack pointer.  */
        insn = emit_insn(gen_frame_addsi3(stack_pointer_rtx, stack_pointer_rtx,
                                          GEN_INT(sp_offset)));
        RTX_FRAME_RELATED_P(insn) = 1;
        REG_NOTES(insn) = cfa_restores;
        add_reg_note(insn, REG_CFA_DEF_CFA, stack_pointer_rtx);

        /* Move up to the stack frame of an exception handler.  */
        if (crtl->calls_eh_return)
                emit_insn(gen_addsi3(stack_pointer_rtx, stack_pointer_rtx,
                                     EH_RETURN_STACKADJ_RTX));
}

/* Worker for PROFILE_HOOK.
   The profile hook uses the link register which will get clobbered by
   the GOT setup RTX.  This sets up a placeholder to allow injecting of the GOT
   setup RTX to avoid clobbering.  */
void oc32_profile_hook(void)
{
        rtx a1 = gen_rtx_REG(Pmode, OC32_R9);
        rtx ra = get_hard_reg_initial_val(Pmode, OC32_LR);
        rtx fun = gen_rtx_SYMBOL_REF(Pmode, "_mcount");

        cfun->machine->set_mcount_arg_insn = emit_move_insn(a1, ra);

        emit_library_call(fun, LCT_NORMAL, VOIDmode, a1, Pmode);
}

/* Worker for TARGET_INIT_PIC_REG.
   Initialize the cfun->machine->set_got_insn rtx and insert it at the entry
   of the current function.  The rtx is just a temporary placeholder for
   the GOT and will be replaced or removed during oc32_expand_prologue.  */
static void
oc32_init_pic_reg(void)
{

        if (crtl->profile)
                cfun->machine->set_got_insn =
                    emit_insn_after(gen_set_got_tmp(pic_offset_table_rtx),
                                    cfun->machine->set_mcount_arg_insn);
        else
        {
                start_sequence();

                cfun->machine->set_got_insn =
                    emit_insn(gen_set_got_tmp(pic_offset_table_rtx));

                rtx_insn *seq = get_insns();
                end_sequence();

                edge entry_edge = single_succ_edge(ENTRY_BLOCK_PTR_FOR_FN(cfun));
                insert_insn_on_edge(seq, entry_edge);
                commit_one_edge_insertion(entry_edge);
        }
}

#undef TARGET_INIT_PIC_REG
#define TARGET_INIT_PIC_REG oc32_init_pic_reg
#undef TARGET_USE_PSEUDO_PIC_REG
#define TARGET_USE_PSEUDO_PIC_REG hook_bool_void_true

/* Worker for INITIAL_FRAME_ADDRESS_RTX.
   Returns the RTX representing the address of the initial stack frame.  */

rtx oc32_initial_frame_addr()
{
        /* Use this to force a stack frame for the current function.  */
        crtl->accesses_prior_frames = 1;
        return arg_pointer_rtx;
}

/* Worker for DYNAMIC_CHAIN_ADDRESS.
   Returns the RTX representing the address of where the caller's frame pointer
   may be stored on the stack.  */
rtx oc32_dynamic_chain_addr(rtx frame)
{
        return plus_constant(Pmode, frame, -2 * UNITS_PER_WORD);
}

/* Worker for RETURN_ADDR_RTX.
   Returns the RTX representing the address of where the link register may be
   stored on the stack.  */
rtx oc32_return_addr(int, rtx frame)
{
        return gen_frame_mem(Pmode, plus_constant(Pmode, frame, -UNITS_PER_WORD));
}

static bool
oc32_frame_pointer_required()
{
        /* ??? While IRA checks accesses_prior_frames, reload does not.
           We do want the frame pointer for this case.  */
        return (crtl->accesses_prior_frames);
}

/* Expand the "eh_return" pattern.
   Used for defining __builtin_eh_return, this will emit RTX to override the
   current function's return address stored on the stack.  The emitted RTX is
   inserted before the epilogue so we can't just update the link register.
   This is used when handling exceptions to jump into the exception handler
   catch block upon return from _Unwind_RaiseException.  */
void oc32_expand_eh_return(rtx eh_addr)
{
        rtx lraddr;

        lraddr = gen_frame_mem(Pmode, plus_constant(Pmode,
                                                    arg_pointer_rtx,
                                                    -UNITS_PER_WORD));
        /* Set address to volatile to ensure the store doesn't get optimized out.  */
        MEM_VOLATILE_P(lraddr) = true;
        emit_move_insn(lraddr, eh_addr);
}

/* Helper for defining INITIAL_ELIMINATION_OFFSET.
   We allow the following eliminiations:
     FP -> HARD_FP or SP
     AP -> HARD_FP or SP

   HARD_FP and AP are the same which is handled below.  */

HOST_WIDE_INT
oc32_initial_elimination_offset(int from, int to)
{
        HOST_WIDE_INT offset;

        /* Set OFFSET to the offset from the stack pointer.  */
        switch (from)
        {
        /* Incoming args are all the way up at the previous frame.  */
        case ARG_POINTER_REGNUM:
                offset = cfun->machine->total_size;
                break;

        /* Local args grow downward from the saved registers.  */
        case FRAME_POINTER_REGNUM:
                offset = cfun->machine->args_size + cfun->machine->local_vars_size;
                break;

        default:
                gcc_unreachable();
        }

        if (to == HARD_FRAME_POINTER_REGNUM)
                offset -= cfun->machine->total_size;

        return offset;
}

/* Worker for TARGET_LEGITIMATE_ADDRESS_P.
   Returns true if X is a legitimate address RTX.  */
static bool
oc32_legitimate_address_p(machine_mode mode, rtx x, bool strict_p,
                          code_helper = ERROR_MARK)
{
        rtx base, addend;

        switch (GET_CODE(x))
        {
        case REG:
                base = x;
                break;

        case PLUS:
                base = XEXP(x, 0);
                addend = XEXP(x, 1);
                if (!REG_P(base))
                        return false;
                if (!REG_P(addend) && (mode == QImode))
                        return false;
                if (!satisfies_constraint_L(addend))
                        return false;
                break;

        case LO_SUM:
                base = XEXP(x, 0);
                if (!REG_P(base))
                        return false;
                x = XEXP(x, 1);
                switch (GET_CODE(x))
                {
                case CONST:
                case SYMBOL_REF:
                case LABEL_REF:
                        /* Assume legitimize_address properly categorized
                           the symbol.  Continue to check the base.  */
                        break;

                case UNSPEC:
                        switch (XINT(x, 1))
                        {
                        case UNSPEC_GOT:
                        case UNSPEC_GOTOFF:
                        case UNSPEC_TPOFF:
                        case UNSPEC_GOTTPOFF:
                                /* Assume legitimize_address properly categorized
                                   the symbol.  Continue to check the base.  */
                                break;
                        default:
                                return false;
                        }
                        break;

                default:
                        return false;
                }
                break;

        default:
                return false;
        }

        unsigned regno = REGNO(base);
        if (regno >= FIRST_PSEUDO_REGISTER)
        {
                if (strict_p)
                        regno = reg_renumber[regno];
                else
                        return true;
        }
        if (strict_p)
                return regno <= 31;
        else
                return REGNO_OK_FOR_BASE_P(regno);
}

/* Return the TLS type for TLS symbols, 0 otherwise.  */
static tls_model
oc32_tls_symbolic_operand(rtx op)
{
        rtx sym, addend;
        split_const(op, &sym, &addend);
        if (SYMBOL_REF_P(sym))
                return SYMBOL_REF_TLS_MODEL(sym);
        return TLS_MODEL_NONE;
}

/* Get a reference to the '__tls_get_addr' symbol.  */
static GTY(()) rtx gen_tls_tga;

static rtx
gen_tls_get_addr(void)
{
        if (!gen_tls_tga)
                gen_tls_tga = init_one_libfunc("__tls_get_addr");
        return gen_tls_tga;
}

/* Emit a call to '__tls_get_addr'.  */
static void
oc32_tls_call(rtx dest, rtx arg)
{
        emit_library_call_value(gen_tls_get_addr(), dest, LCT_CONST,
                                Pmode, arg, Pmode);
}

/* Helper for oc32_legitimize_address_1.  Wrap X in an unspec.  */
static rtx
gen_sym_unspec(rtx x, int kind)
{
        return gen_rtx_UNSPEC(Pmode, gen_rtvec(1, x), kind);
}

/* Helper function to implement both TARGET_LEGITIMIZE_ADDRESS and expand the
   patterns "movqi", "movqi" and "movsi".  Returns an valid RTX that
   represents the argument X which is an invalid address RTX.  The argument
   SCRATCH may be used as a temporary when building addresses.  */

static rtx
oc32_legitimize_address_1(rtx x, rtx scratch, machine_mode mode)
{
        rtx base, addend, t1, t2;
        tls_model tls_kind = TLS_MODEL_NONE;
        bool is_local = true;

        /* If x is [reg+reg], it's already a valid address, return as-is */
        if (GET_CODE(x) == PLUS)
        {
                rtx op0 = XEXP(x, 0);
                rtx op1 = XEXP(x, 1);
                if (REG_P(op0) && REG_P(op1))
                        return x;
        }

        split_const(x, &base, &addend);
        switch (GET_CODE(base))
        {
        default:
                gcc_assert(can_create_pseudo_p());
                base = force_reg(Pmode, base);
                break;

        case REG:
        case SUBREG:
                break;

        case SYMBOL_REF:
                tls_kind = SYMBOL_REF_TLS_MODEL(base);
                is_local = SYMBOL_REF_LOCAL_P(base);
                /* FALLTHRU */

        case LABEL_REF:
                switch (tls_kind)
                {
                case TLS_MODEL_NONE:
                        t1 = can_create_pseudo_p() ? gen_reg_rtx(Pmode) : scratch;
                        if (!flag_pic)
                        {
                                emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, x)));
                                return gen_rtx_LO_SUM(Pmode, t1, x);
                        }
                        else if (is_local)
                        {
                                crtl->uses_pic_offset_table = 1;
                                t2 = gen_sym_unspec(x, UNSPEC_GOTOFF);
                                emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, t2)));
                                emit_insn(gen_add3_insn(t1, t1, pic_offset_table_rtx));
                                return gen_rtx_LO_SUM(Pmode, t1, copy_rtx(t2));
                        }
                        else
                        {
                                base = gen_sym_unspec(base, UNSPEC_GOT);
                                crtl->uses_pic_offset_table = 1;
                                if (TARGET_CMODEL_LARGE)
                                {
                                        emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, base)));
                                        emit_insn(gen_add3_insn(t1, t1, pic_offset_table_rtx));
                                        t2 = gen_rtx_LO_SUM(Pmode, t1, base);
                                }
                                else
                                        t2 = gen_rtx_LO_SUM(Pmode, pic_offset_table_rtx, base);
                                t2 = gen_const_mem(Pmode, t2);
                                emit_insn(gen_rtx_SET(t1, t2));
                                base = t1;
                        }
                        break;

                case TLS_MODEL_GLOBAL_DYNAMIC:
                case TLS_MODEL_LOCAL_DYNAMIC:
                        /* TODO: For now, treat LD as GD.  */
                        t1 = gen_reg_rtx(Pmode);
                        base = gen_sym_unspec(base, UNSPEC_TLSGD);
                        emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, base)));
                        emit_insn(gen_rtx_SET(t1, gen_rtx_LO_SUM(Pmode, t1, base)));
                        crtl->uses_pic_offset_table = 1;
                        emit_insn(gen_add3_insn(t1, t1, pic_offset_table_rtx));
                        base = gen_reg_rtx(Pmode);
                        oc32_tls_call(base, t1);
                        break;

                case TLS_MODEL_INITIAL_EXEC:
                        t1 = gen_reg_rtx(Pmode);
                        t2 = gen_reg_rtx(Pmode);
                        base = gen_sym_unspec(base, UNSPEC_GOTTPOFF);
                        emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, base)));
                        crtl->uses_pic_offset_table = 1;
                        emit_insn(gen_add3_insn(t1, t1, pic_offset_table_rtx));
                        t1 = gen_rtx_LO_SUM(Pmode, t1, base);
                        emit_move_insn(t2, gen_const_mem(Pmode, t1));
                        t1 = gen_rtx_REG(Pmode, OC32_TLS);
                        emit_insn(gen_add3_insn(t2, t2, t1));
                        base = t2;
                        break;

                case TLS_MODEL_LOCAL_EXEC:
                        x = gen_sym_unspec(x, UNSPEC_TPOFF);
                        t1 = gen_reg_rtx(Pmode);
                        emit_insn(gen_rtx_SET(t1, gen_rtx_HIGH(Pmode, x)));
                        t2 = gen_rtx_REG(Pmode, OC32_TLS);
                        emit_insn(gen_add3_insn(t1, t1, t2));
                        return t1;

                default:
                        gcc_unreachable();
                }
                break;

                /* Accept what we may have already emitted.  */

        case LO_SUM:
        case UNSPEC:
                if (GET_CODE(addend) == CONST)
                        break;
                else
                        return x;
        }

        /* If we get here, we still have addend outstanding.  */
        gcc_checking_assert(register_operand(base, Pmode));
        if (addend == const0_rtx)
                return base;
        if (satisfies_constraint_L(addend) && (mode == SImode))
                return gen_rtx_PLUS(Pmode, base, addend);
        else
        {
                addend = force_reg(Pmode, addend);
                return gen_rtx_PLUS(Pmode, base, addend);
        }
}

/* Worker for TARGET_LEGITIMIZE_ADDRESS.
   This delegates implementation to oc32_legitimize_address_1.  */

static rtx
oc32_legitimize_address(rtx x, rtx /* oldx */, machine_mode mode)
{
        return oc32_legitimize_address_1(x, NULL_RTX, mode);
}

#undef TARGET_LEGITIMIZE_ADDRESS
#define TARGET_LEGITIMIZE_ADDRESS oc32_legitimize_address

/* Worker for TARGET_DELEGITIMIZE_ADDRESS.
   In the name of slightly smaller debug output, and to cater to
   general assembler lossage, recognize PIC+GOTOFF and turn it back
   into a direct symbol reference.  */

static rtx
oc32_delegitimize_address(rtx x)
{
        if (GET_CODE(x) == UNSPEC)
        {
                /* The LO_SUM to which X was attached has been stripped.
                   Since the only legitimate address we could have been computing
                   is that of the symbol, assume that's what we've done.  */
                if (XINT(x, 1) == UNSPEC_GOTOFF)
                        return XVECEXP(x, 0, 0);
        }
        else if (MEM_P(x))
        {
                rtx addr = XEXP(x, 0);
                if (GET_CODE(addr) == LO_SUM && XEXP(addr, 0) == pic_offset_table_rtx)
                {
                        rtx inner = XEXP(addr, 1);
                        if (GET_CODE(inner) == UNSPEC && XINT(inner, 1) == UNSPEC_GOT)
                                return XVECEXP(inner, 0, 0);
                }
        }
        return delegitimize_mem_from_attrs(x);
}

#undef TARGET_DELEGITIMIZE_ADDRESS
#define TARGET_DELEGITIMIZE_ADDRESS oc32_delegitimize_address

/* Worker for TARGET_CANNOT_FORCE_CONST_MEM.
   Primarily this is required for TLS symbols, but given that our move
   patterns *ought* to be able to handle any symbol at any time, we
   should never be spilling symbolic operands to the constant pool, ever.  */

static bool
oc32_cannot_force_const_mem(machine_mode, rtx x)
{
        rtx_code code = GET_CODE(x);
        return (code == SYMBOL_REF || code == LABEL_REF || code == CONST || code == HIGH);
}

#undef TARGET_CANNOT_FORCE_CONST_MEM
#define TARGET_CANNOT_FORCE_CONST_MEM oc32_cannot_force_const_mem

/* Worker for TARGET_LEGITIMATE_CONSTANT_P.
   Returns true is the RTX X represents a constant that can be used as an
   immediate operand  */

static bool
oc32_legitimate_constant_p(machine_mode, rtx x)
{
        switch (GET_CODE(x))
        {
        case CONST_INT:
        case CONST_WIDE_INT:
        case HIGH:
                /* We construct these, rather than spilling to memory.  */
                return true;

        case CONST:
        case SYMBOL_REF:
        case LABEL_REF:
                /* These may need to be split and not reconstructed.  */
                return oc32_tls_symbolic_operand(x) == TLS_MODEL_NONE;

        default:
                return false;
        }
}

#undef TARGET_LEGITIMATE_CONSTANT_P
#define TARGET_LEGITIMATE_CONSTANT_P oc32_legitimate_constant_p

/* Return non-zero if the function argument described by ARG is to be
   passed by reference.  */

static bool
oc32_pass_by_reference(cumulative_args_t, const function_arg_info &arg)
{
        if (arg.aggregate_type_p())
                return true;
        HOST_WIDE_INT size = arg.type_size_in_bytes();
        return size < 0 || size > 8;
}

/* Worker for TARGET_FUNCTION_VALUE.
   Returns an RTX representing the location where function return values will
   be stored.  On this is the register r11.  64-bit return value's
   upper 32-bits are returned in r12, this is automatically done by GCC.  */

static rtx
oc32_function_value(const_tree valtype,
                    const_tree /* fn_decl_or_type */,
                    bool /* outgoing */)
{
        return gen_rtx_REG(TYPE_MODE(valtype), OC32_RV);
}

/* Worker for TARGET_LIBCALL_VALUE.
   Returns an RTX representing the location where function return values to
   external libraries will be stored.  On this the same as local
   function calls.  */

static rtx
oc32_libcall_value(machine_mode mode,
                   const_rtx /* fun */)
{
        return gen_rtx_REG(mode, OC32_RV);
}

/* Worker for TARGET_FUNCTION_VALUE_REGNO_P.
   Returns true if REGNO is a valid register for storing a function return
   value.  */

static bool
oc32_function_value_regno_p(const unsigned int regno)
{
        return (regno == OC32_RV);
}

/* Worker for TARGET_STRICT_ARGUMENT_NAMING.
   Return true always as on the last argument in a variatic function
   is named.  */

static bool
oc32_strict_argument_naming(cumulative_args_t /* ca */)
{
        return true;
}

#undef TARGET_STRICT_ARGUMENT_NAMING
#define TARGET_STRICT_ARGUMENT_NAMING oc32_strict_argument_naming

/* Worker for TARGET_FUNCTION_ARG.
   Return the next register to be used to hold a function argument or NULL_RTX
   if there's no more space.  Arugment CUM_V represents the current argument
   offset, zero for the first function argument. function arguments
   maybe be passed in registers r9 to r14.  */

static rtx
oc32_function_arg(cumulative_args_t cum_v, const function_arg_info &arg)
{
        /* Handle the special marker for the end of the arguments.  */
        if (arg.end_marker_p())
                return NULL_RTX;

        CUMULATIVE_ARGS *cum = get_cumulative_args(cum_v);
        int nreg = CEIL(GET_MODE_SIZE(arg.mode), UNITS_PER_WORD);

        /* Note that all large arguments are passed by reference.  */
        gcc_assert(nreg <= 2);
        if (arg.named && *cum + nreg <= 6)
                return gen_rtx_REG(arg.mode, *cum + OC32_FIRST_ARG_REG);
        else
                return NULL_RTX;
}

/* Worker for TARGET_FUNCTION_ARG_ADVANCE.
   Update the cumulative args descriptor CUM_V to advance past the next function
   argument.  Note, this is not called for arguments passed on the stack.  */

static void
oc32_function_arg_advance(cumulative_args_t cum_v,
                          const function_arg_info &arg)
{
        CUMULATIVE_ARGS *cum = get_cumulative_args(cum_v);
        int nreg = CEIL(GET_MODE_SIZE(arg.mode), UNITS_PER_WORD);

        /* Note that all large arguments are passed by reference.  */
        gcc_assert(nreg <= 2);
        if (arg.named)
                *cum += nreg;
}

/* worker function for TARGET_RETURN_IN_MEMORY.
   Returns true if the argument of TYPE should be returned in memory.
   this is any value larger than 64-bits.  */

static bool
oc32_return_in_memory(const_tree type, const_tree /* fntype */)
{
        const HOST_WIDE_INT size = int_size_in_bytes(type);
        return (size == -1 || size > (2 * UNITS_PER_WORD));
}

static void
output_addr_reloc(FILE *stream, rtx x, HOST_WIDE_INT add, const char *reloc)
{
        if (*reloc)
        {
                fputs(reloc, stream);
                fputc('(', stream);
        }
        output_addr_const(stream, x);
        if (add)
        {
                if (add > 0)
                        fputc('+', stream);
                fprintf(stream, HOST_WIDE_INT_PRINT_DEC, add);
        }
        if (*reloc)
                fputc(')', stream);
}

/* The PRINT_OPERAND_ADDRESS worker.  */
void oc32_print_operand_address(FILE *file, machine_mode mode, rtx x)
{
        switch (GET_CODE(x))
        {
        case REG:
        {
                unsigned int regno = REGNO(x);
                if (regno < FIRST_PSEUDO_REGISTER)
                        fprintf(file, "%s", reg_names[regno]);
                else
                        fprintf(file, "r%d", regno);
        }
        break;

        case PLUS:
                if (GET_CODE(XEXP(x, 0)) != REG)
                {
                        fprintf(file, "INVALID");
                        return;
                }
                {
                        unsigned int regno = REGNO(XEXP(x, 0));
                        if (regno >= FIRST_PSEUDO_REGISTER)
                        {
                                fprintf(file, "r%d", regno);
                        }
                        else
                        {
                                fprintf(file, "%s", reg_names[regno]);
                        }
                }
                switch (GET_CODE(XEXP(x, 1)))
                {
                case CONST_INT:
                {
                        HOST_WIDE_INT offset = INTVAL(XEXP(x, 1));
                        if (offset >= 0)
                                fprintf(file, "+%ld", offset);
                        else
                                fprintf(file, "%ld", offset);
                }
                break;
                case REG:
                {
                        /* Base + register offset addressing */
                        unsigned int regno2 = REGNO(XEXP(x, 1));
                        if (regno2 >= FIRST_PSEUDO_REGISTER)
                                fprintf(file, "+r%d", regno2);
                        else
                                fprintf(file, "+%s", reg_names[regno2]);
                }
                break;
                case CONST:
                {
                        rtx plus = XEXP(XEXP(x, 1), 0);
                        if (GET_CODE(XEXP(plus, 0)) == SYMBOL_REF && CONST_INT_P(XEXP(plus, 1)))
                        {
                                fprintf(file, "+");
                                output_addr_const(file, XEXP(plus, 0));
                                fprintf(file, "+%ld]", INTVAL(XEXP(plus, 1)));
                        }
                        else
                                fprintf(file, "+INVALID");
                }
                break;
                default:
                        fprintf(file, "+INVALID");
                }
                break;

        default:
                output_addr_const(file, x);
                break;
        }
}

/* The PRINT_OPERAND worker.  */
void oc32_print_operand(FILE *file, rtx x, int code)
{
        rtx operand = x;

        /* New code entries should just be added to the switch below.  If
           handling is finished, just return.  If handling was just a
           modification of the operand, the modified operand should be put in
           "operand", and then do a break to let default handling
           (zero-modifier) output the operand.  */

        switch (code)
        {
        case 'P':
                if (!flag_pic || SYMBOL_REF_LOCAL_P(x))
                        output_addr_const(file, x);
                else
                        output_addr_reloc(file, x, 0, "plt");
                break;
        case 0:
                /* Print an operand as without a modifier letter.  */
                switch (GET_CODE(operand))
                {
                case REG:
                        if (REGNO(operand) > 31)
                                internal_error("internal error: unknown register: R%d", REGNO(operand));
                        else
                                fprintf(file, "R%d", REGNO(operand));

                        break;

                case MEM:
                        output_address(GET_MODE(XEXP(operand, 0)), XEXP(operand, 0));
                        return;

                case CODE_LABEL:
                case LABEL_REF:
                        output_asm_label(operand);
                        break;

                default:
                        /* No need to handle all strange variants, let output_addr_const
                           do it for us.  */
                        if (CONSTANT_P(operand))
                                output_addr_const(file, operand);

                        else
                                internal_error("unknown operand: %d", GET_CODE(operand));
                        break;
                }
                break;

        default:
                output_operand_lossage("unknown prefix code: %c", code);
                break;
        }
}

/* Worker for TARGET_TRAMPOLINE_INIT.
   This is called to initialize a trampoline.  The argument M_TRAMP is an RTX
   for the memory block to be initialized with trampoline code.  The argument
   FNDECL contains the definition of the nested function to be called, we use
   this to get the function's address.  The argument CHAIN is an RTX for the
   static chain value to be passed to the nested function.  */

static void
oc32_trampoline_init(rtx m_tramp, tree fndecl, rtx chain)
{
        const unsigned or_r6_lo = 0xA00000C0;
        const unsigned movh_r6_hi = 0xFCA00006;
        const unsigned or_r15_lo = 0xA00001E0;
        const unsigned movh_r15_hi = 0xFCA0000F;
        const unsigned j_r6 = 0xF8000006;
        rtx tramp[5], fnaddr, f_hi, f_lo, c_hi, c_lo;

        fnaddr = force_operand(XEXP(DECL_RTL(fndecl), 0), NULL);
        f_hi = expand_binop(SImode, lshr_optab, fnaddr, GEN_INT(16),
                            NULL, true, OPTAB_DIRECT);
        f_lo = expand_binop(SImode, and_optab, fnaddr, GEN_INT(0xffff),
                            NULL, true, OPTAB_DIRECT);

        chain = force_operand(chain, NULL);
        c_hi = expand_binop(SImode, lshr_optab, chain, GEN_INT(16),
                            NULL, true, OPTAB_DIRECT);
        c_lo = expand_binop(SImode, and_optab, chain, GEN_INT(0xffff),
                            NULL, true, OPTAB_DIRECT);

        /* We want to generate
        mov R6, (nested_func) -- 2 inst
        mov R15, (static_chain) -- 2 inst
        J R6
        */
        /* Shift lo values to correct position in instruction encoding */
        rtx f_lo_shifted = expand_binop(SImode, ashl_optab, f_lo, GEN_INT(10),
                                        NULL, true, OPTAB_DIRECT);
        rtx c_lo_shifted = expand_binop(SImode, ashl_optab, c_lo, GEN_INT(10),
                                        NULL, true, OPTAB_DIRECT);
        rtx f_hi_shifted = expand_binop(SImode, ashl_optab, f_hi, GEN_INT(5),
                                        NULL, true, OPTAB_DIRECT);
        rtx c_hi_shifted = expand_binop(SImode, ashl_optab, c_hi, GEN_INT(5),
                                        NULL, true, OPTAB_DIRECT);
        tramp[0] = expand_binop(SImode, ior_optab, f_hi_shifted,
                                gen_int_mode(movh_r6_hi, SImode),
                                f_lo, true, OPTAB_DIRECT);
        tramp[1] = expand_binop(SImode, ior_optab, c_hi_shifted,
                                gen_int_mode(movh_r15_hi, SImode),
                                c_lo, true, OPTAB_DIRECT);
        tramp[2] = expand_binop(SImode, ior_optab, f_lo_shifted,
                                gen_int_mode(or_r6_lo, SImode),
                                f_hi, true, OPTAB_DIRECT);
        tramp[3] = expand_binop(SImode, ior_optab, c_lo_shifted,
                                gen_int_mode(or_r15_lo, SImode),
                                c_hi, true, OPTAB_DIRECT);

        tramp[4] = gen_int_mode(j_r6, SImode);

        for (int i = 0; i < 5; ++i)
        {
                rtx mem = adjust_address(m_tramp, SImode, i * 4);
                emit_move_insn(mem, tramp[i]);
        }

        /* Flushing the trampoline from the instruction cache needs
           to be done here. */
}

/* Worker for TARGET_HARD_REGNO_MODE_OK.
   Returns true if the hard register REGNO is ok for storing values of mode
   MODE.  */
static bool
oc32_hard_regno_mode_ok(unsigned int regno, machine_mode mode)
{
        /* GENERAL_REGS can hold anything, while
           FLAG_REGS are really single bits within SP[SR].  
        if (REGNO_REG_CLASS(regno) == FLAG_REGS)
                return mode == BImode;
        */
        return true;
}

#undef TARGET_HARD_REGNO_MODE_OK
#define TARGET_HARD_REGNO_MODE_OK oc32_hard_regno_mode_ok

/* Worker for TARGET_CAN_CHANGE_MODE_CLASS.
   Returns true if its ok to change a register in class RCLASS from mode FROM to
   mode TO. */
static bool
oc32_can_change_mode_class(machine_mode from, machine_mode to,
                           reg_class_t rclass)
{
        if (rclass == FLAG_REGS)
                return from == to;
        return true;
}

#undef TARGET_CAN_CHANGE_MODE_CLASS
#define TARGET_CAN_CHANGE_MODE_CLASS oc32_can_change_mode_class

/* Expand the patterns "movqi", "movqi" and "movsi".  The argument OP0 is the
   destination and OP1 is the source.  This expands to set OP0 to OP1.  */
void oc32_expand_move(machine_mode mode, rtx *operands)
{
        rtx op0 = operands[0];
        rtx op1 = operands[1];

        if (MEM_P(op0))
        {
                /* Check if target memory address is legitimate */
                rtx addr = XEXP(op0, 0);
                if (!oc32_legitimate_address_p(mode, addr, false))
                {
                        addr = oc32_legitimize_address_1(addr, NULL_RTX, mode);
                        op0 = replace_equiv_address(op0, addr);
                }
                if (!const0_operand(op1, mode))
                        op1 = force_reg(mode, op1);
        }
        else if (MEM_P(op1))
        {
                /* Check if source memory address is legitimate */
                rtx addr = XEXP(op1, 0);
                if (!oc32_legitimate_address_p(mode, addr, false))
                {
                        addr = oc32_legitimize_address_1(addr, op0, mode);
                        op1 = replace_equiv_address(op1, addr);
                }
        }
        /* NEW: Handle complex constant expressions */
        else
        {
                switch (GET_CODE(op1))
                {
                case CONST_INT:
                        break;
                case CONST:
                case SYMBOL_REF:
                case LABEL_REF:
                        op1 = oc32_legitimize_address_1(op1, op0, mode);
                        break;
                default:
                        break;
                }
        }


        if (mode == HImode)
        {
                /* HImode: force memory addresses to [Reg] form (base only).
                   then it will match movhi_internal insn  */
                if (MEM_P(op0))
                {
                        rtx addr = XEXP(op0, 0);
                        if (!REG_P(addr))
                        {
                                rtx tmp = gen_reg_rtx(SImode);
                                emit_insn(gen_rtx_SET(tmp, addr));
                                op0 = replace_equiv_address(op0, tmp);
                        }
                }

                if (MEM_P(op1))
                {
                        rtx addr = XEXP(op1, 0);
                        if (!REG_P(addr))
                        {
                                rtx tmp = gen_reg_rtx(SImode);
                                emit_insn(gen_rtx_SET(tmp, addr));
                                op1 = replace_equiv_address(op1, tmp);
                        }
                }

                rtx op2 = gen_reg_rtx(SImode);
                emit_insn(gen_movhi_internal(op0, op1, op2));
                return;
        }

        emit_insn(gen_rtx_SET(op0, op1));
}

/* Expand a conditional store.
   operands[0] is the set register
   operands[1] is the operator
   operands[2] is the first operand of the comparison
   operands[3] is the second operand of the comparison
*/
void oc32_expand_cstore(rtx *operands)
{
        rtx_code code = GET_CODE(operands[1]);

        /* Ensure operands are in registers.
           For immediate operands, check if they are within the valid range
           for OC32's 16-bit immediate fields (-32768 to 32767 for signed,
           0 to 65535 for unsigned).  */
        if (!register_operand(operands[2], SImode))
                operands[2] = force_reg(SImode, operands[2]);

        /* Check if it is a valid immediate for OC32.
           For unsigned comparisons (LTU, LEU, GTU, GEU), use 0-65535 range.
           For signed comparisons, use -32768 to 32767 range.  */
        bool valid_imm = false;
        if (CONST_INT_P(operands[3]))
        {
                HOST_WIDE_INT val = INTVAL(operands[3]);
                if (code == LTU || code == LEU || code == GTU || code == GEU)
                        valid_imm = (val >= 0 && val <= 65535);
                else
                        valid_imm = (val >= -32768 && val <= 32767);
        }

        if (!register_operand(operands[3], SImode) && !valid_imm)
                operands[3] = force_reg(SImode, operands[3]);


        switch (code)
        {
        case GT:
                if (TARGET_ZFSF)
                {
                        emit_insn(gen_sgt_zf(operands[0], operands[2], operands[3]));
                }
                else
                {
                        emit_insn(gen_sgt(operands[0], operands[2], operands[3]));
                }
                break;
        
        case GTU:
                if (TARGET_ZFSF)
                {
                        emit_insn(gen_sgtu_zf(operands[0], operands[2], operands[3]));
                }
                else
                {
                        emit_insn(gen_sgtu(operands[0], operands[2], operands[3]));
                }
                break;

        case GE:
                if (TARGET_ZFSF)
                {
                        emit_insn(gen_sge_zf(operands[0], operands[2], operands[3]));
                }
                else
                {
                        emit_insn(gen_sge(operands[0], operands[2], operands[3]));
                }
                break;
        
        case GEU:
                if (TARGET_ZFSF)
                {
                        emit_insn(gen_sgeu_zf(operands[0], operands[2], operands[3]));
                }
                else
                {
                        emit_insn(gen_sgeu(operands[0], operands[2], operands[3]));
                }
                break;

        case LT:
                emit_insn(gen_slt(operands[0], operands[2], operands[3]));
                break;

        case LTU:
                emit_insn(gen_sltu(operands[0], operands[2], operands[3]));
                break;

        case LE:
                emit_insn(gen_sle(operands[0], operands[2], operands[3]));
                break;

        case LEU:
                emit_insn(gen_sleu(operands[0], operands[2], operands[3]));
                break;

        case EQ:
                emit_insn(gen_seq(operands[0], operands[2], operands[3]));
                break;

        case NE:
                emit_insn(gen_sne(operands[0], operands[2], operands[3]));
                break;

        case LTGT:
                emit_insn(gen_sne(operands[0], operands[2], operands[3]));
                break;

        default:        
                break;
        }
}

/* Expand a conditional branch.
   operands[0] is the operator
   operands[1] is the first operand of the comparison
   operands[2] is the second operand of the comparison
   operands[3] is the label to jump to
*/
void oc32_expand_cbranch(rtx *operands)
{
        rtx_code code = GET_CODE(operands[0]);
        rtx ccflag_eq = gen_rtx_REG(SImode, OC32_R3);
        rtx ccflag_lt = gen_rtx_REG(SImode, OC32_R4);
        rtx ccflag_gt = gen_rtx_REG(SImode, OC32_R5);

        /* Ensure operand 1 are in registers */
        if (!register_operand(operands[1], SImode))
                operands[1] = force_reg(SImode, operands[1]);

        /* Check if it is a valid immediate for OC32.
           For unsigned comparisons (LTU, LEU, GTU, GEU), use 0-65535 range.
           For signed comparisons, use -32768 to 32767 range.  */
        bool valid_imm = false;
        HOST_WIDE_INT val = INTVAL(operands[2]);
        if (CONST_INT_P(operands[2]))
        {
                if (code == LTU || code == LEU || code == GTU || code == GEU)
                        valid_imm = (val >= 0 && val <= 65535);
                else
                        valid_imm = (val >= -32768 && val <= 32767);
        }

        if (!register_operand(operands[2], SImode) && !valid_imm)
                operands[2] = force_reg(SImode, operands[2]);


        switch (code)
        {
        case GT:     
                if (TARGET_ZFSF)
                {
                        emit_jump_insn(gen_jumpgt_zf(operands[1], operands[2], operands[3]));
                }
                else
                {
                        emit_jump_insn(gen_jumpgt(operands[1], operands[2], operands[3]));
                }
                return;


        case GTU:
                if (CONST_INT_P(operands[2]) && (val == 0))
                {
                        emit_jump_insn(gen_jump_ne_z(operands[1], operands[3]));
                }
                else 
                {    
                        if (TARGET_ZFSF)
                        {
                                emit_jump_insn(gen_jumpgtu_zf(operands[1], operands[2], operands[3]));
                        }
                        else
                        {
                                emit_jump_insn(gen_jumpgtu(operands[1], operands[2], operands[3]));
                        }
                }
                return;
                

        case GE:
                emit_jump_insn(gen_jumpge(operands[1], operands[2], operands[3]));
                return;
        
        case GEU:
                emit_jump_insn(gen_jumpgeu(operands[1], operands[2], operands[3]));
                return;

        case LT:
                emit_jump_insn(gen_jumplt(operands[1], operands[2], operands[3]));
                return;

        case LTU:
                emit_jump_insn(gen_jumpltu(operands[1], operands[2], operands[3]));
                return;

        case LE:
                if (TARGET_ZFSF)
                {
                        emit_jump_insn(gen_jumple_zf(operands[1], operands[2], operands[3]));
                }
                else
                {
                        emit_jump_insn(gen_jumple(operands[1], operands[2], operands[3]));                      
                }
                return;

        case LEU:
                if (TARGET_ZFSF)
                {
                        emit_jump_insn(gen_jumpleu_zf(operands[1], operands[2], operands[3]));
                }
                else
                {
                        emit_jump_insn(gen_jumpleu(operands[1], operands[2], operands[3]));                        
                }
                return;

        case EQ:
                if (CONST_INT_P(operands[2]) && (val == 0))
                {
                        emit_jump_insn(gen_jump_eq_z(operands[1], operands[3]));
                }
                else 
                {
                        emit_jump_insn(gen_jumpeq(operands[1], operands[2], operands[3]));
                }
                return;

        case NE:
                if (CONST_INT_P(operands[2]) && (val == 0))
                {
                        emit_jump_insn(gen_jump_ne_z(operands[1], operands[3]));
                }
                else
                {
                        emit_jump_insn(gen_jumpne(operands[1], operands[2], operands[3]));
                }
                return;

        default:
                return;
        }
}


/* Expand the patterns "call", "sibcall", "call_value" and "sibcall_value".
   Expands a function call where argument RETVAL is an optional RTX providing
   return value storage, the argument FNADDR is and RTX describing the function
   to call, the argument CALLARG1 is the number or registers used as operands
   and the argument SIBCALL should be true if this is a nested function call.
   If FNADDR is a non local symbol and FLAG_PIC is enabled this will generate
   a PLT call.  */

void oc32_expand_call(rtx retval, rtx fnaddr, rtx callarg1, bool sibcall)
{
        rtx call, use = NULL;

        /* Calls via the PLT require the PIC register.  */
        if (flag_pic && GET_CODE(XEXP(fnaddr, 0)) == SYMBOL_REF && !SYMBOL_REF_LOCAL_P(XEXP(fnaddr, 0)))
        {
                crtl->uses_pic_offset_table = 1;
                rtx hard_pic = gen_rtx_REG(Pmode, REAL_PIC_OFFSET_TABLE_REGNUM);
                emit_move_insn(hard_pic, pic_offset_table_rtx);
                use_reg(&use, hard_pic);
        }

        rtx fnaddr_inner = XEXP(fnaddr, 0);
        if (GET_CODE(fnaddr_inner) == SYMBOL_REF)
        {
                /* 检查是否跨section调用 */
                tree target_decl = SYMBOL_REF_DECL(fnaddr_inner);
                const char *target_section = (target_decl != NULL) ? DECL_SECTION_NAME(target_decl) : nullptr;
                
                section *current_sec = current_function_section();
                const char *current_section = (current_sec && (current_sec->common.flags & SECTION_NAMED)) 
                                        ? current_sec->named.name 
                                        : nullptr;
                
                if (target_section != nullptr && current_section != nullptr)
                {
                        if (strcmp(target_section, current_section) != 0)
                        {
                                /* 跨section调用，强制使用间接调用 */
                                fnaddr = gen_rtx_MEM(SImode, copy_to_mode_reg(Pmode, fnaddr_inner));
                        }
                }
        }

        if (!call_insn_operand(XEXP(fnaddr, 0), Pmode))
        {
                fnaddr = copy_to_mode_reg(Pmode, XEXP(fnaddr, 0));
                fnaddr = gen_rtx_MEM(SImode, fnaddr);
        }

        call = gen_rtx_CALL(VOIDmode, fnaddr, callarg1);
        if (retval)
                call = gen_rtx_SET(retval, call);

        /* Normal calls clobber LR.  This is required in order to
           prevent e.g. a prologue store of LR being placed into
           the delay slot of the call, after it has been updated.  */
        if (!sibcall)
        {
                rtx clob = gen_rtx_CLOBBER(VOIDmode, gen_rtx_REG(Pmode, OC32_LR));
                call = gen_rtx_PARALLEL(VOIDmode, gen_rtvec(2, call, clob));
        }
        call = emit_call_insn(call);

        CALL_INSN_FUNCTION_USAGE(call) = use;
}

/* Worker for TARGET_FUNCTION_OK_FOR_SIBCALL.
   Returns true if the function declared by DECL is ok for calling as a nested
   function.  */

static bool
oc32_function_ok_for_sibcall(tree decl, tree /* exp */)
{
        /* We can sibcall to any function if not PIC.  */
        if (!flag_pic)
                return true;

        /* We can sibcall any indirect function.  */
        if (decl == NULL)
                return true;

        /* If the call may go through the PLT, we need r16 live.  */
        return targetm.binds_local_p(decl);
}

#undef TARGET_FUNCTION_OK_FOR_SIBCALL
#define TARGET_FUNCTION_OK_FOR_SIBCALL oc32_function_ok_for_sibcall

/* Worker for TARGET_RTX_COSTS.  */
static bool
oc32_rtx_costs(rtx x, machine_mode mode, int outer_code, int opno,
               int *total, bool speed)
{
        switch (GET_CODE(x))
        {
        case CONST_INT:
                if (x == const0_rtx)
                        *total = 0;
                else if ((outer_code == PLUS || outer_code == XOR || outer_code == MULT) && satisfies_constraint_I(x))
                        *total = 0;
                else if (satisfies_constraint_I(x))
                        *total = 1;  // 改为1：单个ADD指令
                else if (satisfies_constraint_J(x))
                        *total = 2;  // MOVH+OR，两条指令
                else
                        *total = COSTS_N_INSNS(3);  // 内存加载，三条指令
                return true;

        case CONST_DOUBLE:
                *total = (x == CONST0_RTX(mode) ? 0 : COSTS_N_INSNS(2));
                return true;

        case HIGH:
                *total = 2;
                return true;

        case LO_SUM:
                *total = (outer_code == MEM ? 0 : 2);
                return true;

        case CONST:
        case SYMBOL_REF:
        case LABEL_REF:
                if (outer_code == LO_SUM || outer_code == HIGH)
                        *total = 0;
                else
                        *total = COSTS_N_INSNS(1 + (outer_code != MEM));
                return true;

        case PLUS:
                if (outer_code == MEM)
                        *total = 0;
                break;

        case IF_THEN_ELSE:
        {
                rtx cond = XEXP(x, 0);
                rtx then_part = XEXP(x, 1);
                rtx else_part = XEXP(x, 2);

                /* Case 1: cmov_ne — condition is a plain register (no comparison operator) */
                if (!COMPARISON_P(cond) && REG_P(cond))
                {
                        *total = COSTS_N_INSNS(1);
                        return true;
                }

                /* Case 2: cmov_z / cmov_nz — EQ/NE against const 0, result used as integer */
                if ((GET_CODE(cond) == EQ || GET_CODE(cond) == NE) &&
                        REG_P(XEXP(cond, 0)) &&
                        CONST_INT_P(XEXP(cond, 1)) &&
                        INTVAL(XEXP(cond, 1)) == 0)
                {
                        *total = COSTS_N_INSNS(2);   // OR + MOVZ/MOVNZ = 2 insns
                        return true;
                }

                /* Case 3: cbranchsi4 — comparison with label_ref / pc */
                if ((LABEL_REF_P(then_part) && else_part == pc_rtx) ||
                        (LABEL_REF_P(else_part) && then_part == pc_rtx))
                {
                        bool is_zero_eqne = (GET_CODE(cond) == EQ || GET_CODE(cond) == NE || GET_CODE(cond) == GTU) &&
                                        CONST_INT_P(XEXP(cond, 1)) &&
                                        INTVAL(XEXP(cond, 1)) == 0;
                        if (is_zero_eqne)
                        *total = COSTS_N_INSNS(1);               // jump_eq_z/jump_ne_z: 1 insn
                        else
                        *total = COSTS_N_INSNS(TARGET_ZFSF ? 3 : 2);  // cmp + jump
                        return true;
                }
        }
        break;

        default:
                break;
        }
        return false;
}

#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS oc32_rtx_costs

/* Emit an atomic load operation using LWAS.  */
static void
oc32_emit_load_atomic(machine_mode mode, rtx dest, rtx mem)
{
        rtx addr = XEXP(mem, 0);
        rtx base;
        rtx offset;

        /* Check if address is legitimate, if not, legitimize it */
        if (!oc32_legitimate_address_p(mode, addr, false))
        {
                addr = oc32_legitimize_address_1(addr, dest, mode);
                mem = replace_equiv_address(mem, addr);
                addr = XEXP(mem, 0);
        }

        if (GET_CODE(addr) == PLUS)
        {
                base = XEXP(addr, 0);
                offset = XEXP(addr, 1);
                if (!CONST_INT_P(offset))
                {
                        /* convert to [reg] form, load atomic does not support [reg+reg]*/
                        rtx tmp = gen_reg_rtx(SImode);
                        emit_insn(gen_addsi3(tmp, base, offset));
                        addr = tmp;
                }
        }

        emit_insn(gen_movsi_load_atomic(dest, gen_rtx_MEM(SImode, addr)));
}

/* Emit an atomic store operation using SWAT.  */
static void
oc32_emit_store_atomic(machine_mode mode, rtx mem, rtx value)
{
        rtx addr = XEXP(mem, 0);
        rtx base;
        rtx offset;

        /* Check if address is legitimate, if not, legitimize it */
        if (!oc32_legitimate_address_p(mode, addr, false))
        {
                addr = oc32_legitimize_address_1(addr, NULL_RTX, mode);
                mem = replace_equiv_address(mem, addr);
                addr = XEXP(mem, 0);
        }

        if (GET_CODE(addr) == PLUS)
        {
                base = XEXP(addr, 0);
                offset = XEXP(addr, 1);
                if (!CONST_INT_P(offset))
                {
                        /* convert to [reg] form, load atomic does not support [reg+reg]*/
                        rtx tmp = gen_reg_rtx(SImode);
                        emit_insn(gen_addsi3(tmp, base, offset));
                        addr = tmp;
                }
        }

        emit_insn(gen_movsi_store_atomic(gen_rtx_MEM(SImode, addr), value));
}

/* A subroutine of the various atomic expanders.  For sub-word operations,
   we must adjust things to operate on SImode.  Given the original MEM,
   return a new aligned memory.  Also build and return the quantities by
   which to shift and mask.  */

static rtx
oc32_adjust_atomic_subword(rtx orig_mem, rtx *pshift, rtx *pmask)
{
        rtx addr, align, shift, mask, mem;
        machine_mode mode = GET_MODE(orig_mem);

        addr = XEXP(orig_mem, 0);
        addr = force_reg(Pmode, addr);

        /* Aligned memory containing subword.  Generate a new memory.  We
           do not want any of the existing MEM_ATTR data, as we're now
           accessing memory outside the original object.  */
        align = expand_binop(Pmode, and_optab, addr, GEN_INT(-4),
                             NULL_RTX, 1, OPTAB_LIB_WIDEN);
        mem = gen_rtx_MEM(SImode, align);
        MEM_VOLATILE_P(mem) = MEM_VOLATILE_P(orig_mem);
        if (MEM_ALIAS_SET(orig_mem) == ALIAS_SET_MEMORY_BARRIER)
                set_mem_alias_set(mem, ALIAS_SET_MEMORY_BARRIER);

        /* Shift amount for subword relative to aligned word.  */
        rtx mode_mask = GEN_INT(mode == QImode ? 3 : 2);
        shift = expand_binop(SImode, and_optab, gen_lowpart(SImode, addr),
                             mode_mask, NULL_RTX, 1, OPTAB_LIB_WIDEN);
        if (BYTES_BIG_ENDIAN)
                shift = expand_binop(SImode, xor_optab, shift, mode_mask,
                                     shift, 1, OPTAB_LIB_WIDEN);
        shift = expand_binop(SImode, ashl_optab, shift, GEN_INT(3),
                             shift, 1, OPTAB_LIB_WIDEN);
        *pshift = shift;

        /* Mask for insertion.  */
        mask = expand_binop(SImode, ashl_optab, GEN_INT(GET_MODE_MASK(mode)),
                            shift, NULL_RTX, 1, OPTAB_LIB_WIDEN);
        *pmask = mask;

        return mem;
}

/* A subroutine of the various atomic expanders.  For sub-word operations,
   complete the operation by shifting result to the lsb of the SImode
   temporary and then extracting the result in MODE with a SUBREG.  */

static void
oc32_finish_atomic_subword(machine_mode mode, rtx o, rtx n, rtx shift)
{
        n = expand_binop(SImode, lshr_optab, n, shift,
                         NULL_RTX, 1, OPTAB_LIB_WIDEN);
        emit_move_insn(o, gen_lowpart(mode, n));
}

void oc32_expand_atomic_compare_and_swap(rtx operands[])
{
        rtx boolval, retval, mem, oldval, newval;
        rtx label1, label2;
        machine_mode mode;
        bool is_weak;

        boolval = operands[0];
        retval = operands[1];
        mem = operands[2];
        oldval = operands[3];
        newval = operands[4];
        is_weak = (INTVAL(operands[5]) != 0);
        mode = GET_MODE(mem);

        if (reg_overlap_mentioned_p(retval, oldval))
                oldval = copy_to_reg(oldval);

        label1 = NULL_RTX;
        label2 = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());

        /* 1. set loopback label1 - for strong only */
        if (!is_weak)
        {
                label1 = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
                emit_label(XEXP(label1, 0));
        }

        /* 2. load target memory */
        oc32_emit_load_atomic(mode, retval, mem);

        /* 3. compare memory value and expected value */
        /* 4. jump return if not equal - failed */
        //emit_jump_insn(gen_jumpne(retval, oldval, XEXP(label2, 0)));

        emit_insn(gen_cmpeq(retval, oldval));
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R4), XEXP(label2, 0)));        


        /* 5. store new value to memory */
        oc32_emit_store_atomic(mode, mem, newval);

        /* 6. jump back if store failed(ZF==0) - for stong only */
        if (!is_weak)
        {
                emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label1, 0)));
        }

        /* 7. set return label */
        emit_label(XEXP(label2, 0));

        /* 8. set return value */
        emit_insn(gen_rtx_SET(boolval, gen_rtx_REG(SImode, OC32_R3)));
}

void oc32_expand_atomic_compare_and_swap_qihi(rtx operands[])
{
        rtx boolval, orig_retval, retval, scratch, mem, oldval, newval;
        rtx label1, label2, mask, shift;
        machine_mode mode;
        bool is_weak;

        boolval = operands[0];
        orig_retval = operands[1];
        mem = operands[2];
        oldval = operands[3];
        newval = operands[4];
        is_weak = (INTVAL(operands[5]) != 0);
        mode = GET_MODE(mem);

        // 1. 调整内存地址和创建掩码/移位
        mem = oc32_adjust_atomic_subword(mem, &shift, &mask);

        // 2. 准备 OLDVAL 和 NEWVAL（移位和扩展）
        if (oldval != const0_rtx)
        {
                oldval = convert_modes(SImode, mode, oldval, 1);
                oldval = expand_binop(SImode, ashl_optab, oldval, shift,
                                      NULL_RTX, 1, OPTAB_LIB_WIDEN);
        }
        if (newval != const0_rtx)
        {
                newval = convert_modes(SImode, mode, newval, 1);
                newval = expand_binop(SImode, ashl_optab, newval, shift,
                                      NULL_RTX, 1, OPTAB_LIB_WIDEN);
        }

        // 3. 创建标签
        label1 = NULL_RTX;
        if (!is_weak)
        {
                label1 = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
                emit_label(XEXP(label1, 0));
        }
        label2 = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());

        // 4. 原子加载整个字
        scratch = gen_reg_rtx(SImode);
        oc32_emit_load_atomic(SImode, scratch, mem);

        // 5. 提取目标字段
        retval = expand_binop(SImode, and_optab, scratch, mask,
                              NULL_RTX, 1, OPTAB_LIB_WIDEN);

        // 6. 清除原字段位置
        scratch = expand_binop(SImode, xor_optab, scratch, retval,
                               scratch, 1, OPTAB_LIB_WIDEN);

        // 7. 比较提取的字段值
        // 8. 不相等则跳转到失败
        //emit_jump_insn(gen_jumpne(retval, oldval, XEXP(label2, 0)));        

        emit_insn(gen_cmpeq(retval, oldval));
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R4), XEXP(label2, 0)));

        // 9. 插入新值
        if (newval != const0_rtx)
                scratch = expand_binop(SImode, ior_optab, scratch, newval,
                                       scratch, 1, OPTAB_LIB_WIDEN);

        // 10. 原子存储整个字
        oc32_emit_store_atomic(SImode, mem, scratch);
        // SWAT 后：R3 = 1(成功) 或 R3 = 0(失败)

        // 11. Strong CAS: 存储失败则重试
        if (!is_weak)
        {
                emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label1, 0)));
        }

        // 12. 完成标签
        emit_label(XEXP(label2, 0));

        // 13. 设置布尔结果
        emit_insn(gen_rtx_SET(boolval, gen_rtx_REG(SImode, OC32_R3)));

        // 14. 提取原始值返回
        oc32_finish_atomic_subword(mode, orig_retval, retval, shift);
}

/* Expand an atomic exchange operation.
   Emits the RTX to perform an exchange operation.  This function takes 4 RTX
   arguments in the OPERANDS array.  The exchange operation atomically loads a
   value from memory (OPERANDS[1]) to a return value (OPERANDS[0]) and stores a
   new value (OPERANDS[2]) back to the memory location. */

void oc32_expand_atomic_exchange(rtx operands[])
{
        rtx retval, mem, val, label;
        machine_mode mode;

        retval = operands[0];
        mem = operands[1];
        val = operands[2];
        mode = GET_MODE(mem);

        if (reg_overlap_mentioned_p(retval, val))
                val = copy_to_reg(val);

        label = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
        emit_label(XEXP(label, 0));
        oc32_emit_load_atomic(mode, retval, mem);
        oc32_emit_store_atomic(mode, mem, val);
        /* jump back if store failed(R3==0) */
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label, 0)));
}

/* Expand an atomic exchange operation for QImode and HImode.
   For sub-word exchanges, we need to load the full word, extract the
   target field, replace it with the new value, and store the full word
   atomically using LL/SC.  */

void oc32_expand_atomic_exchange_qihi(rtx operands[])
{
        rtx orig_retval, retval, mem, val, scratch;
        rtx label, mask, shift;
        machine_mode mode;

        orig_retval = operands[0];
        mem = operands[1];
        val = operands[2];
        mode = GET_MODE(mem);

        /* Adjust memory address and get shift/mask values */
        mem = oc32_adjust_atomic_subword(mem, &shift, &mask);

        /* Shift and mask VAL into position with the word */
        if (val != const0_rtx)
        {
                val = convert_modes(SImode, mode, val, 1);
                val = expand_binop(SImode, ashl_optab, val, shift,
                                   NULL_RTX, 1, OPTAB_LIB_WIDEN);
        }

        /* Create retry label for LL/SC loop */
        label = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
        emit_label(XEXP(label, 0));

        /* Atomic load of the full word */
        scratch = gen_reg_rtx(SImode);
        oc32_emit_load_atomic(SImode, scratch, mem);

        /* Extract the target field */
        retval = expand_binop(SImode, and_optab, scratch, mask,
                              NULL_RTX, 1, OPTAB_LIB_WIDEN);

        /* Clear the original field position */
        scratch = expand_binop(SImode, xor_optab, scratch, retval,
                               scratch, 1, OPTAB_LIB_WIDEN);

        /* Insert the new value */
        if (val != const0_rtx)
                scratch = expand_binop(SImode, ior_optab, scratch, val,
                                       scratch, 1, OPTAB_LIB_WIDEN);

        /* Atomic store of the full word */
        oc32_emit_store_atomic(SImode, mem, scratch);

        /* Jump back if store failed (R3==0) */
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label, 0)));

        /* Extract the original value and return it */
        oc32_finish_atomic_subword(mode, orig_retval, retval, shift);
}

/* Expand an atomic fetch-and-operate pattern.  CODE is the binary operation
   to perform (with MULT as a stand-in for NAND).  MEM is the memory on which
   to operate.  VAL is the second operand of the binary operator.  BEFORE and
   AFTER are optional locations to return the value of MEM either before of
   after the operation.  */

void oc32_expand_atomic_op(rtx_code code, rtx mem, rtx val,
                           rtx orig_before, rtx orig_after)
{
        machine_mode mode = GET_MODE(mem);
        rtx before = orig_before, after = orig_after;
        rtx label;

        label = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
        emit_label(XEXP(label, 0));

        if (before == NULL_RTX)
                before = gen_reg_rtx(mode);

        oc32_emit_load_atomic(mode, before, mem);

        if (code == MULT)
        {
                /* Implement NAND using OC32's NAND instruction */
                if (after == NULL_RTX)
                        after = gen_reg_rtx(mode);
                emit_insn(gen_nandsi3(after, before, val));
        }
        else
                after = expand_simple_binop(mode, code, before, val,
                                            after, 1, OPTAB_LIB_WIDEN);

        oc32_emit_store_atomic(mode, mem, after);
        /* Jump back if store failed (R3==0) */
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label, 0)));

        if (orig_before)
                emit_move_insn(orig_before, before);
        if (orig_after)
                emit_move_insn(orig_after, after);
}

/* Expand an atomic fetch-and-operate pattern for QImode and HImode.
   CODE is the binary operation to perform (with MULT as a stand-in for NAND).
   MEM is the memory on which to operate.  VAL is the second operand of the
   binary operator.  ORIG_BEFORE and ORIG_AFTER are optional locations to
   return the value of MEM either before or after the operation.  */

void oc32_expand_atomic_op_qihi(rtx_code code, rtx mem, rtx val,
                                rtx orig_before, rtx orig_after)
{
        machine_mode mode = GET_MODE(mem);
        rtx label, mask, shift, x;
        rtx before, after, scratch;

        /* Adjust memory address and get shift/mask values */
        mem = oc32_adjust_atomic_subword(mem, &shift, &mask);

        /* Shift and mask VAL into position with the word */
        val = convert_modes(SImode, mode, val, 1);
        val = expand_binop(SImode, ashl_optab, val, shift,
                           NULL_RTX, 1, OPTAB_LIB_WIDEN);

        /* Prepare VAL based on operation type */
        switch (code)
        {
        case IOR:
        case XOR:
                /* We've already zero-extended VAL.  That is sufficient to
                   make certain that it does not affect other bits.  */
                break;

        case AND:
        case MULT: /* NAND */
                /* If we make certain that all of the other bits in VAL are
                   set, that will be sufficient to not affect other bits.  */
                x = expand_unop(SImode, one_cmpl_optab, mask, NULL_RTX, 1);
                val = expand_binop(SImode, ior_optab, val, x,
                                   val, 1, OPTAB_LIB_WIDEN);
                break;

        case PLUS:
        case MINUS:
                /* These will all affect bits outside the field and need
                   adjustment via MASK within the loop.  */
                break;

        default:
                gcc_unreachable();
        }

        /* Create retry label for LL/SC loop */
        label = gen_rtx_LABEL_REF(VOIDmode, gen_label_rtx());
        emit_label(XEXP(label, 0));

        /* Atomic load of the full word */
        before = scratch = gen_reg_rtx(SImode);
        oc32_emit_load_atomic(SImode, before, mem);

        /* Perform the operation based on code */
        switch (code)
        {
        case IOR:
        case XOR:
        case AND:
                after = expand_simple_binop(SImode, code, before, val,
                                            NULL_RTX, 1, OPTAB_LIB_WIDEN);
                scratch = after;
                break;

        case PLUS:
        case MINUS:
                /* Extract field, operate, mask result, merge back */
                before = expand_binop(SImode, and_optab, scratch, mask,
                                      NULL_RTX, 1, OPTAB_LIB_WIDEN);
                scratch = expand_binop(SImode, xor_optab, scratch, before,
                                       scratch, 1, OPTAB_LIB_WIDEN);
                after = expand_simple_binop(SImode, code, before, val,
                                            NULL_RTX, 1, OPTAB_LIB_WIDEN);
                after = expand_binop(SImode, and_optab, after, mask,
                                     after, 1, OPTAB_LIB_WIDEN);
                scratch = expand_binop(SImode, ior_optab, scratch, after,
                                       scratch, 1, OPTAB_LIB_WIDEN);
                break;

        case MULT: /* NAND */
                after = expand_binop(SImode, and_optab, before, val,
                                     NULL_RTX, 1, OPTAB_LIB_WIDEN);
                after = expand_binop(SImode, xor_optab, after, mask,
                                     after, 1, OPTAB_LIB_WIDEN);
                scratch = after;
                break;

        default:
                gcc_unreachable();
        }

        /* Atomic store of the full word */
        oc32_emit_store_atomic(SImode, mem, scratch);

        /* Jump back if store failed (R3==0) */
        emit_jump_insn(gen_jump_eq_z(gen_rtx_REG(SImode, OC32_R3), XEXP(label, 0)));

        /* Return original and/or new values */
        if (orig_before)
                oc32_finish_atomic_subword(mode, orig_before, before, shift);
        if (orig_after)
                oc32_finish_atomic_subword(mode, orig_after, after, shift);
}

/* Worker for TARGET_ASM_OUTPUT_MI_THUNK.
   Output the assembler code for a thunk function.  THUNK_DECL is the
   declaration for the thunk function itself, FUNCTION is the decl for
   the target function.  DELTA is an immediate constant offset to be
   added to THIS.  If VCALL_OFFSET is nonzero, the word at address
   (*THIS + VCALL_OFFSET) should be additionally added to THIS.  */

static void
oc32_output_mi_thunk(FILE *file, tree thunk_fndecl,
                     HOST_WIDE_INT delta, HOST_WIDE_INT vcall_offset,
                     tree function)
{
        const char *fnname = IDENTIFIER_POINTER(DECL_ASSEMBLER_NAME(thunk_fndecl));
        rtx this_rtx, funexp;
        rtx_insn *insn;

        reload_completed = 1;
        epilogue_completed = 1;

        emit_note(NOTE_INSN_PROLOGUE_END);

        /* Find the "this" pointer.  Normally in r9, but if the function
           returns a structure, the structure return pointer is in r3 and
           the "this" pointer is in r10 instead.  */
        if (aggregate_value_p(TREE_TYPE(TREE_TYPE(function)), function))
                this_rtx = gen_rtx_REG(Pmode, OC32_R10);
        else
                this_rtx = gen_rtx_REG(Pmode, OC32_R9);

        /* Add DELTA.  When possible use a plain add, otherwise load it
           into a register first.  */
        if (delta)
        {
                rtx delta_rtx = GEN_INT(delta);

                if (!satisfies_constraint_I(delta_rtx))
                {
                        rtx scratch = gen_rtx_REG(Pmode, OC32_R6);
                        emit_move_insn(scratch, delta_rtx);
                        delta_rtx = scratch;
                }

                /* THIS_RTX += DELTA.  */
                emit_insn(gen_add2_insn(this_rtx, delta_rtx));
        }

        /* Add the word at address (*THIS_RTX + VCALL_OFFSET).  */
        if (vcall_offset)
        {
                rtx scratch = gen_rtx_REG(Pmode, OC32_R6);
                HOST_WIDE_INT lo = sext_hwi(vcall_offset, 16);
                HOST_WIDE_INT hi = vcall_offset - lo;
                rtx tmp;

                /* SCRATCH = *THIS_RTX.  */
                tmp = gen_rtx_MEM(Pmode, this_rtx);
                emit_move_insn(scratch, tmp);

                if (hi != 0)
                {
                        rtx scratch2 = gen_rtx_REG(Pmode, OC32_RV);
                        emit_move_insn(scratch2, GEN_INT(hi));
                        emit_insn(gen_add2_insn(scratch, scratch2));
                }

                /* SCRATCH = *(*THIS_RTX + VCALL_OFFSET).  */
                tmp = plus_constant(Pmode, scratch, lo);
                tmp = gen_rtx_MEM(Pmode, tmp);
                emit_move_insn(scratch, tmp);

                /* THIS_RTX += *(*THIS_RTX + VCALL_OFFSET).  */
                emit_insn(gen_add2_insn(this_rtx, scratch));
        }

        /* Generate a tail call to the target function.  */
        if (!TREE_USED(function))
        {
                assemble_external(function);
                TREE_USED(function) = 1;
        }
        funexp = XEXP(DECL_RTL(function), 0);

        /* The symbol will be a local alias and therefore always binds local.  */
        gcc_assert(SYMBOL_REF_LOCAL_P(funexp));

        funexp = gen_rtx_MEM(FUNCTION_MODE, funexp);
        insn = emit_call_insn(gen_sibcall(funexp, const0_rtx));
        SIBLING_CALL_P(insn) = 1;
        emit_barrier();

        /* Run just enough of rest_of_compilation to get the insns emitted.
           There's not really enough bulk here to make other passes such as
           instruction scheduling worth while.  */
        insn = get_insns();
        shorten_branches(insn);
        assemble_start_function(thunk_fndecl, fnname);
        final_start_function(insn, file, 1);
        final(insn, file, 1);
        final_end_function();
        assemble_end_function(thunk_fndecl, fnname);

        reload_completed = 0;
        epilogue_completed = 0;
}

static unsigned
oc32_libm_function_max_error(unsigned cfn, machine_mode mode,
                             bool boundary_p)
{
#ifdef OPTION_GLIBC
        bool glibc_p = OPTION_GLIBC;
#else
        bool glibc_p = false;
#endif
        if (glibc_p)
        {
                switch (cfn)
                {
                CASE_CFN_SIN:
                CASE_CFN_SIN_FN:
                        if (!boundary_p && mode == DFmode && flag_rounding_math)
                                return 7;
                        break;
                default:
                        break;
                }
                return glibc_linux_libm_function_max_error(cfn, mode, boundary_p);
        }
        return default_libm_function_max_error(cfn, mode, boundary_p);
}

#undef TARGET_ASM_OUTPUT_MI_THUNK
#define TARGET_ASM_OUTPUT_MI_THUNK oc32_output_mi_thunk
#undef TARGET_ASM_CAN_OUTPUT_MI_THUNK
#define TARGET_ASM_CAN_OUTPUT_MI_THUNK \
        hook_bool_const_tree_hwi_hwi_const_tree_true

#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE oc32_option_override

#undef TARGET_COMPUTE_FRAME_LAYOUT
#define TARGET_COMPUTE_FRAME_LAYOUT oc32_compute_frame_layout

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P oc32_legitimate_address_p

#ifdef HAVE_AS_TLS
#undef TARGET_HAVE_TLS
#define TARGET_HAVE_TLS true
#endif

#undef TARGET_HAVE_SPECULATION_SAFE_VALUE
#define TARGET_HAVE_SPECULATION_SAFE_VALUE speculation_safe_value_not_needed

#undef TARGET_LIBM_FUNCTION_MAX_ERROR
#define TARGET_LIBM_FUNCTION_MAX_ERROR oc32_libm_function_max_error

/* Calling Conventions.  */
#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE oc32_function_value
#undef TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE oc32_libcall_value
#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P oc32_function_value_regno_p
#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG oc32_function_arg
#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE oc32_function_arg_advance
#undef TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY oc32_return_in_memory
#undef TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE oc32_pass_by_reference
#undef TARGET_TRAMPOLINE_INIT
#define TARGET_TRAMPOLINE_INIT oc32_trampoline_init
#undef TARGET_FRAME_POINTER_REQUIRED
#define TARGET_FRAME_POINTER_REQUIRED oc32_frame_pointer_required

/*
#undef TARGET_CUSTOM_FUNCTION_DESCRIPTORS
#define TARGET_CUSTOM_FUNCTION_DESCRIPTORS 1
*/

#undef TARGET_CONSTANT_ALIGNMENT
#define TARGET_CONSTANT_ALIGNMENT constant_alignment_word_strings

/* Assembly generation.  */
#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND oc32_print_operand
#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS oc32_print_operand_address

/* Section anchor support.  */
#undef TARGET_MIN_ANCHOR_OFFSET
#define TARGET_MIN_ANCHOR_OFFSET -32768
#undef TARGET_MAX_ANCHOR_OFFSET
#define TARGET_MAX_ANCHOR_OFFSET 32767

struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-oc32.h"