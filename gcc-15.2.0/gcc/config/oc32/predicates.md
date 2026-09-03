;; Predicate definitions for OC32
;; Copyright (C) 2015-2023 Free Software Foundation, Inc.
;; Contributed by FTDI <support@ftdi.com>

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; -------------------------------------------------------------------------
;; Predicates
;; -------------------------------------------------------------------------

;; Nonzero if OP can be source of a simple move operation.
;;
;; The CONST_INT could really be CONST if we were to fix
;; oc32_print_operand_address to format the address correctly.
;; It might require assembler/linker work as well to ensure
;; the right relocation is emitted.

(define_predicate "oc32_rsimm_operand"
  (ior (match_code "reg")
       (match_code "subreg")
       (and (match_code "const_int")
            (match_test "IN_RANGE (INTVAL (op), -32768, 32767)"))))

(define_predicate "reg_or_s16_operand"
  (if_then_else (match_code "const_int")
    (match_test "INTVAL (op) >= -32768 && INTVAL (op) <= 32767")
    (match_operand 0 "register_operand")))

(define_predicate "oc32_ruimm_operand"
  (ior (match_code "reg")
       (match_code "subreg")
       (and (match_code "const_int")
            (match_test "IN_RANGE (INTVAL (op), 0, 65535)"))))

(define_predicate "oc32_bit_operand"
  (ior (match_code "reg")
       (match_code "subreg")
       (and (match_code "const_int")
            (match_test "IN_RANGE (INTVAL (op), 0, 31)"))))

;; 16-bit unsigned immediate operand
(define_predicate "oc32_uimm_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 65535)")))

;; 16-bit signed immediate operand
(define_predicate "oc32_simm_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), -32768, 32767)")))

(define_predicate "symbol_label_operand"
  (match_code "symbol_ref,label_ref,const,unspec"))

;; Return true for relocations that must use MOVH+ADD
(define_predicate "losum_add_operand"
  (match_code "symbol_ref,label_ref,const,unspec"))

;; Return true for relocations that must use MOVH+OR
(define_predicate "losum_or_operand"
  (and (match_code "unspec")
       (match_test "XINT(op, 1) == UNSPEC_TLSGD")))

(define_predicate "virtual_frame_reg_operand"
  (match_code "reg")
{
  unsigned int regno = REGNO(op);
  return (regno != STACK_POINTER_REGNUM
          && regno != HARD_FRAME_POINTER_REGNUM
          && REGNO_PTR_FRAME_P(regno));
})

;; General input operand for most instructions
(define_predicate "input_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "memory_operand")
       (and (match_code "const_int")
	          (match_test "satisfies_constraint_I (op)
			        || satisfies_constraint_K (op)
              || satisfies_constraint_U (op)
			        || satisfies_constraint_J (op)"))))

;; Constant zero operand
(define_predicate "const0_operand"
  (and (match_code "const_int")
       (match_test "op == const0_rtx")))

;; Register or constant zero
(define_predicate "reg_or_0_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "const0_operand")))

;; Operand for call instructions
(define_predicate "call_insn_operand"
  (ior (match_code "symbol_ref")
       (match_operand 0 "register_operand")))

;; SR address：0x3FF00000 - 0x3FFFFFFF
(define_predicate "oc32_sr_address_operand"
  (and (match_code "mem")
       (match_test "oc32_sr_address_p (op)")))