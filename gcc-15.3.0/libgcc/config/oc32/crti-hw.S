/* Hardware startup file for OC32
	Copyright (C) 2015-2025 Free Software Foundation, Inc.

	This file is part of GCC.

	GCC is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published
	by the Free Software Foundation; either version 3, or (at your
	option) any later version.

	GCC is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	OR  	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
	License for more details.

	You should have received a copy of the GNU General Public License
	along with GCC; see the file COPYING3.  If not see
	<http://www.gnu.org/licenses/>.  */

/* Memory Map (from linker script):
	fboot:  0x00000000, 4K    - Boot code
	ferr0-7: 0x10000000+0x100*n, 256 bytes each - System error handlers
	ssysc:  0x10000800, 256   - System call handlers
	strap:  0x10000900, 256   - System trap handlers
	shint:  0x10000A00, 1536  - Interrupt vectors
	fusr:   0x40000000, 98K   - User code
	susr:   after fusr, 18K   - User code
	sdata:  after susr, 4K    - Data section
	*/

#include "./CV32S6015SR.h"
/* ============================================================
	* Boot Section - Entry Point
* ============================================================ */
.section .boot, "ax"
.global _start, __FUSE_PID, __FUSE_LID, __FUSE_TSN
.global __FUSE_BPCON, __FUSE_IOSCCON, __FUSE_LVDCON, __FUSE_VTSCON, __FUSE_AMPCON
.global __FUSE_ABUFCON, __FUSE_CMP0CON, __FUSE_CMP1CON, __FUSE_CMP2CON, __FUSE_VTTABLE
.global __FUSE_REGSAVE
.type _start, @function
_start:
/* configure CPU mode 
bit0: user write SR permit
bit1: hardware interrupt stack
bit2: sr maps to memory space
bit3: save flags by R0
bit6~4: hardware interrupt entry offset 4/8/16/32/64/128/256/512
*/
    OR      R9,  R0,  0x0037
    WRSR    CMODE,  R9

/* Load Hardware Trim */
	MOVH	R9,  __FUSE_BPCON
	OR		R9,  R9,  __FUSE_BPCON
	LW		R10,  [R9]
	WRSR	BPCON,  R10

	MOVH	R9,  __FUSE_IOSCCON
	OR		R9,  R9,  __FUSE_IOSCCON
	LW		R10,  [R9]
	WRSR	IOSCCON,  R10

	MOVH	R9,  __FUSE_LVDCON
	OR		R9,  R9,  __FUSE_LVDCON
	LW		R10,  [R9]
	WRSR	LVDCON,  R10

	MOVH	R9,  __FUSE_VTSCON
	OR		R9,  R9,  __FUSE_VTSCON
	LW		R10,  [R9]
	WRSR	VTSCON,  R10

	MOVH	R9,  __FUSE_AMPCON
	OR		R9,  R9,  __FUSE_AMPCON
	LW		R10,  [R9]
	WRSR	AMPCON,  R10

	MOVH	R9,  __FUSE_ABUFCON
	OR		R9,  R9,  __FUSE_ABUFCON
	LW		R10,  [R9]
	WRSR	ABUFCON,  R10

	MOVH	R9,  __FUSE_CMP0CON
	OR		R9,  R9,  __FUSE_CMP0CON
	LW		R10,  [R9]
	WRSR	CMP0CON,  R10

	MOVH	R9,  __FUSE_CMP1CON
	OR		R9,  R9,  __FUSE_CMP1CON
	LW		R10,  [R9]
	WRSR	CMP1CON,  R10

	MOVH	R9,  __FUSE_CMP2CON
	OR		R9,  R9,  __FUSE_CMP2CON
	LW		R10,  [R9]
	WRSR	CMP2CON,  R10

/* Save __FUSE_REGSAVE value into interrupt section (SRAM) for fast read */
	MOVH	R9,  __FUSE_REGSAVE
	OR		R9,  R9,  __FUSE_REGSAVE
	LW		R10,  [R9]
	MOVH	R9,  _interrupt_fuse_save_reg
	OR		R9,  R9,  _interrupt_fuse_save_reg
	SW		[R9],  R10

/* Initialize stack pointer - use end of data region */
	MOVH	R8,  __stack
	OR  	R8,  R8,  __stack

/* Jump to initialization by RETE instruction inorder to change hardware to USER mode */
	MOVH	R9,  _init_hw
	OR  	R9,  R9,  _init_hw
	OR		R1,  R0,  R9
	RETE	

/* ============================================================
	* Pre-programmed data from Package Test Programmer (1K)
* ============================================================ */
.org	_start + 3072

/* Product ID (ASCII) - 16 bytes */
__FUSE_PID:
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000

/* LOT ID (ASCII) - 16 bytes */
__FUSE_LID:
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000

/* Test Serial Number - 16 bytes */
__FUSE_TSN:
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000

/* Hardware Trim Value */
__FUSE_BPCON:
.word	0x00000000

__FUSE_IOSCCON:
.word	0x00000000

__FUSE_LVDCON:
.word	0x00000000

__FUSE_VTSCON:
.word	0x00000000

__FUSE_AMPCON:
.word	0x00000000

__FUSE_ABUFCON:
.word	0x00000000

__FUSE_CMP0CON:
.word	0x00000000

__FUSE_CMP1CON:
.word	0x00000000

__FUSE_CMP2CON:
.word	0x00000000

/* Model options */
__FUSE_REGSAVE:
/* 0: this model supports R0~R7 hardware stacks for interrupt */
/* 1: this model supports R0~R31 hardware stacks for interrupt */
.word	0x00000000

/* VTS -> ADC -> Temperature Table(128 bytes/points)*/
__FUSE_VTTABLE:
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000
.word	0x00000000

.size _start, . - _start

/* ============================================================
	* System Error Handlers (8 handlers, 256 bytes each)
* ============================================================ */
.section .syserr0, "ax"
.global _syserr0
.type _syserr0, @function
_syserr0:
	J   	_syserr0 /* halt */
.size _syserr0, . - _syserr0

.section .syserr1, "ax"
.global _syserr1
.type _syserr1, @function
_syserr1:
	J   	_syserr1 /* halt */
.size _syserr1, . - _syserr1

.section .syserr2, "ax"
.global _syserr2
.type _syserr2, @function
_syserr2:
	J   	_syserr2 /* halt */
.size _syserr2, . - _syserr2

.section .syserr3, "ax"
.global _syserr3
.type _syserr3, @function
_syserr3:
	J   	_syserr3 /* halt */
.size _syserr3, . - _syserr3

.section .syserr4, "ax"
.global _syserr4
.type _syserr4, @function
_syserr4:
	J   	_syserr4 /* halt */
.size _syserr4, . - _syserr4

.section .syserr5, "ax"
.global _syserr5
.type _syserr5, @function
_syserr5:
	J   	_syserr5 /* halt */
.size _syserr5, . - _syserr5

.section .syserr6, "ax"
.global _syserr6
.type _syserr6, @function
_syserr6:
	J   	_syserr6 /* halt */
.size _syserr6, . - _syserr6

.section .syserr7, "ax"
.global _syserr7
.type _syserr7, @function
_syserr7:
	J   	_syserr7 /* halt */
.size _syserr7, . - _syserr7

/* ============================================================
	* System Call Handler
* ============================================================ */
.section .syscall, "ax"
.global __syscall_handler
.type __syscall_handler, @function
_syscall:
	SW  	[R8 - 4],  R3
	SW  	[R8 - 8],  R2
	SW  	[R8 - 12],  R4
	SW  	[R8 - 16],  R5
	SW  	[R8 - 20],  R6
	SW  	[R8 - 24],  R7
	SW  	[R8 - 28],  R9
	SW  	[R8 - 32],  R10
	SW  	[R8 - 36],  R11
	SW  	[R8 - 40],  R12
	SW  	[R8 - 44],  R13
	SW  	[R8 - 48],  R14
	SW  	[R8 - 52],  R15
	SW  	[R8 - 56],  R16
	SW  	[R8 - 60],  R17
	SW  	[R8 - 64],  R18
	SW  	[R8 - 68],  R19
	SW  	[R8 - 72],  R20
	SW  	[R8 - 76],  R21
	SW  	[R8 - 80],  R22
	SW  	[R8 - 84],  R23
	ADD 	R8,  R8,  -84
	
	/* prepare system call number */
	MOV		R9,  PSR
	OR		R9,  R9,  0xFFFF
	/* jump to handler */
	JL   	__syscall_handler
	J		_syscall_return
.size _syscall, . - _syscall

/* ============================================================
	* System Trap Handler
* ============================================================ */
.section .systrap, "ax"
.global __systrap_handler
.type __systrap_handler, @function
_systrap:
	SW  	[R8 - 4],  R3
	SW  	[R8 - 8],  R2
	SW  	[R8 - 12],  R4
	SW  	[R8 - 16],  R5
	SW  	[R8 - 20],  R6
	SW  	[R8 - 24],  R7
	SW  	[R8 - 28],  R9
	SW  	[R8 - 32],  R10
	SW  	[R8 - 36],  R11
	SW  	[R8 - 40],  R12
	SW  	[R8 - 44],  R13
	SW  	[R8 - 48],  R14
	SW  	[R8 - 52],  R15
	SW  	[R8 - 56],  R16
	SW  	[R8 - 60],  R17
	SW  	[R8 - 64],  R18
	SW  	[R8 - 68],  R19
	SW  	[R8 - 72],  R20
	SW  	[R8 - 76],  R21
	SW  	[R8 - 80],  R22
	SW  	[R8 - 84],  R23
	ADD 	R8,  R8,  -84
	/* jump to handler */
	JL   	__systrap_handler
	J		_syscall_return
.size _systrap, . - _systrap

/* ============================================================
	* Interrupt Vector Table (32 vectors, 16 bytes each)
	* Hardware default is using 16 bytes offset for interrupt entry address
* ============================================================ */
.section .interrupt, "ax"

/* Default interrupt handlers - weak symbols that can be overridden */
.global __int0, __int1, __int2, __int3
.global __int4, __int5, __int6, __int7
.global __int8, __int9, __int10, __int11
.global __int12, __int13, __int14, __int15
.global __int16, __int17, __int18, __int19
.global __int20, __int21, __int22, __int23
.global __int24, __int25, __int26, __int27
.global __int28, __int29, __int30, __int31

/* Assembly interrupt handlers, for low latency application */
.global __asm_int0, __asm_int1, __asm_int2, __asm_int3
.global __asm_int4, __asm_int5, __asm_int6, __asm_int7
.global __asm_int8, __asm_int9, __asm_int10, __asm_int11
.global __asm_int12, __asm_int13, __asm_int14, __asm_int15
.global __asm_int16, __asm_int17, __asm_int18, __asm_int19
.global __asm_int20, __asm_int21, __asm_int22, __asm_int23
.global __asm_int24, __asm_int25, __asm_int26, __asm_int27
.global __asm_int28, __asm_int29, __asm_int30, __asm_int31

_interrupt_start:

/* Macro to generate interrupt entry(32 bytes - 8 instructions) */
.macro INT_ENTRY num
    /* jump to asm handler first for speed */
	MOVH	R6,  __asm_int\num
	OR  	R6,  R6,  __asm_int\num
    JL      [R6]
    /* jump to c handler */
	MOVH	R6,  __int\num
	OR  	R6,  R6,  __int\num
	J   	_interrupt_save_reg
	JNZ		[R0],  R0	/* it is a nop */
    JNZ		[R0],  R0	/* it is a nop */
.endm

	INT_ENTRY 0
	INT_ENTRY 1
	INT_ENTRY 2
	INT_ENTRY 3
	INT_ENTRY 4
	INT_ENTRY 5
	INT_ENTRY 6
	INT_ENTRY 7
	INT_ENTRY 8
	INT_ENTRY 9
	INT_ENTRY 10
	INT_ENTRY 11
	INT_ENTRY 12
	INT_ENTRY 13
	INT_ENTRY 14
	INT_ENTRY 15
	INT_ENTRY 16
	INT_ENTRY 17
	INT_ENTRY 18
	INT_ENTRY 19
	INT_ENTRY 20
	INT_ENTRY 21
	INT_ENTRY 22
	INT_ENTRY 23
	INT_ENTRY 24
	INT_ENTRY 25
	INT_ENTRY 26
	INT_ENTRY 27
	INT_ENTRY 28
	INT_ENTRY 29
	INT_ENTRY 30
	INT_ENTRY 31

.purgem INT_ENTRY

/* ============================================================
	* Interrupt save/restore code
* ============================================================ */
/* save R9~R23 for some models without full-registers-stack, then it can pretend to be a caller */
_interrupt_save_reg:
	MOVH	R7,  _interrupt_fuse_save_reg
	OR  	R7,  R7,  _interrupt_fuse_save_reg
	LW  	R7,  [R7]
/* zero: save registers */
	JZ  	_interrupt_caller_save,  R7

/* jump to handler */
	JL  	[R6]
	RETE	

_interrupt_caller_save:
/* save R9-R23 (caller saved)*/
	SW  	[R8 - 4],  R9
	SW  	[R8 - 8],  R10
	SW  	[R8 - 12],  R11
	SW  	[R8 - 16],  R12
	SW  	[R8 - 20],  R13
	SW  	[R8 - 24],  R14
	SW  	[R8 - 28],  R15
    SW  	[R8 - 32],  R16
    SW  	[R8 - 36],  R17
    SW  	[R8 - 40],  R18
    SW  	[R8 - 44],  R19
    SW  	[R8 - 48],  R20
    SW  	[R8 - 52],  R21
    SW  	[R8 - 56],  R22
    SW  	[R8 - 60],  R23
	ADD 	R8,  R8,  -60

/* jump to handler */
	JL  	[R6]

/* restore */
	ADD 	R8,  R8,  60
	LW  	R23,  [R8 - 60]
    LW  	R22,  [R8 - 56]
    LW  	R21,  [R8 - 52]
    LW  	R20,  [R8 - 48]
    LW  	R19,  [R8 - 44]
    LW  	R18,  [R8 - 40]
    LW  	R17,  [R8 - 36]
    LW  	R16,  [R8 - 32]
	LW  	R15,  [R8 - 28]
	LW  	R14,  [R8 - 24]
	LW  	R13,  [R8 - 20]
	LW  	R12,  [R8 - 16]
	LW  	R11,  [R8 - 12]
	LW  	R10,  [R8 - 8]
	LW  	R9,  [R8 - 4]
	RETE	

_syscall_return:
	ADD 	R8,  R8,  84
	LW  	R23,  [R8 - 84]
	LW  	R22,  [R8 - 80]
	LW  	R21,  [R8 - 76]
	LW  	R20,  [R8 - 72]
	LW  	R19,  [R8 - 68]
	LW  	R18,  [R8 - 64]
	LW  	R17,  [R8 - 60]
	LW  	R16,  [R8 - 56]
	LW  	R15,  [R8 - 52]
	LW  	R14,  [R8 - 48]
	LW  	R13,  [R8 - 44]
	LW  	R12,  [R8 - 40]
	LW  	R11,  [R8 - 36]
	LW  	R10,  [R8 - 32]
	LW  	R9,  [R8 - 28]
	LW  	R7,  [R8 - 24]
	LW  	R6,  [R8 - 20]
	LW  	R5,  [R8 - 16]
	LW  	R4,  [R8 - 12]
	LW  	R2,  [R8 - 8]  
	LW  	R3,  [R8 - 4]
	RETE

_interrupt_fuse_save_reg:
	.word	0x00000000

.size _interrupt_start, . - _interrupt_start

/* ============================================================
	* Hardware Initialization
* ============================================================ */
.section .text, "ax"
.global _init_hw
.type _init_hw, @function
_init_hw:
/* Note: _init_hw is jump via RETE from _start, not JL.
	it can not return in a usual way, but we never return here
	because _exit() is called at the end. */



/* Call main */
	JL  	main



/* Exit */
	OR  	R9,  R0,  R0 /* exit code 0 */
	JL  	_exit

/* Should not return, but if it does, halt */
_init_halt:
	J   	_init_halt

.size _init_hw, . - _init_hw

/* ============================================================
	* Exit function
* ============================================================ */
.section .text, "ax"
.global _exit
.type _exit, @function
_exit:
/* For simulator/debugger */
/* For now, infinite loop */
	J   	_exit
.size _exit, . - _exit

/* ============================================================
	* Stack pointer (defined by linker script)
* ============================================================ */
.global __stack
	/* Stack top at end of data region  */
	__stack = __stack_top
