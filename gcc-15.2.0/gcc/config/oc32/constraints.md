;; Constraint definitions for OC32
;; Copyright (C) 2015-2023 Free Software Foundation, Inc.
;; Contributed by OC32 Project

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
;; Constraints
;; -------------------------------------------------------------------------


(define_register_constraint "c" "SIBCALL_REGS"
  "Registers which can hold a sibling call address")

(define_register_constraint "t" "GOT_REGS"
  "Registers which can be used to store the Global Offset Table (GOT) address.")

(define_constraint "b"
  "A constant for a bit(0..31)"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 31)")))

(define_memory_constraint "W"
  "An OC32 register indirect memory operand: [base] or [base+reg]."
  (and (match_code "mem")
       (ior (match_test "REG_P (XEXP (op, 0))")
            (and (match_test "GET_CODE (XEXP (op, 0)) == PLUS")
                 (match_test "REG_P (XEXP (XEXP (op, 0), 0))")
                 (match_test "REG_P (XEXP (XEXP (op, 0), 1))")))))

(define_memory_constraint "B"
  "An OC32 register indirect memory operand: [base]"
  (and (match_code "mem") (match_test "REG_P (XEXP (op, 0))")))

(define_memory_constraint "A"
  "An OC32 memory operand: [base], [base+reg], or [base+const_int].
   Excludes symbolic addresses (label, symbol, etc.) that require relocation."
  (and (match_code "mem")
       (ior (match_test "REG_P (XEXP (op, 0))")
            (and (match_test "GET_CODE (XEXP (op, 0)) == PLUS")
                 (match_test "REG_P (XEXP (XEXP (op, 0), 0))")
                 (ior (match_test "REG_P (XEXP (XEXP (op, 0), 1))")
                      (match_test "CONST_INT_P (XEXP (XEXP (op, 0), 1))"))))))

(define_constraint "O"
  "The constant zero"
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "I"
  "A 16-bit signed constant (-32768..32767)"
  (and (match_code "const_int")
       (match_test "ival >= -32768 && ival <= 32767")))

(define_constraint "J"
  "A 32-bit signed constant !(-32768..32767)"
  (and (match_code "const_int")
       (match_test "ival < -32768 || ival > 32767")))

(define_constraint "L"
  "An 18-bit signed constant, multiple of 4 (-131072..131071)"
  (and (match_code "const_int")
       (match_test "ival >= -131072 && ival <= 131071 && (ival & 3) == 0")))

(define_constraint "S"
  "A 20-bit signed constant, multiple of 4 (-524288..524287)"
    (and (match_code "const_int")
         (match_test "ival >= -524288 && ival <= 524287 && (ival & 3) == 0")))

(define_constraint "U"
  "A 16-bit unsigned constant (0..65535)"
  (and (match_code "const_int")
       (match_test "ival >= 0 && ival <= 65535")))

(define_constraint "K"
  "A 32-bit unsigned constant !(0..65535)"
  (and (match_code "const_int")
       (match_test "ival > 65535")))