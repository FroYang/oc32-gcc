/* Copyright (C) 2015-2025 Free Software Foundation, Inc.

	This                      file is free software; you can redistribute it and/or modify it
	under                     the terms of the GNU General Public License as published by the
	Free                      Software Foundation; either version 3, or (at your option) any
	later                     version.

	This                      file is distributed in the hope that it will be useful, but
	WITHOUT                   ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY           or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General                   Public License for more details.

	Under                     Section 7 of GPL version 3, you are granted additional
	permissions               described in the GCC Runtime Library Exception, version
	3.1,                      as published by the Free Software Foundation.

	You                       should have received a copy of the GNU General Public License and
	a                         copy of the GCC Runtime Library Exception along with this program;
	see                       the files COPYING3 and COPYING.RUNTIME respectively. If not, see
<http:
	//www.gnu.org/licenses/>. */

/* An executable stack is *not* required for these functions. */
#ifdef __linux__
.section                  .note.GNU-stack,"",%progbits
.previous
#endif

/* OC32 Calling Convention:
- Arguments:
	R9                        (arg0), R10 (arg1), R11 (arg2), R12 (arg3), R13 (arg4), R14 (arg5)
- Return value:
	R15
- Return address:
	R7                        (LR)

This file provides fallback implementations for:
	-                         64-bit arithmetic and logical operations
	-                         Overflow-detecting arithmetic operations
	*/

/* 64-bit signed negative */
#ifdef L__negdi2
.balign                   4
.globl                    __negdi2
.type                     __negdi2, @function
__negdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	Return R16:R15 = -a (no overflow), abort on overflow */
	SUB 	R15,        R0,        R9
	SUBB	R16,        R0,        R10
	J   	[R7]

.size                     __negdi2, . - __negdi2
#endif

/* 32-bit signed addition with overflow detection */
#ifdef L__addvsi3
.balign                   4
.globl                    __addvsi3
.type                     __addvsi3, @function
__addvsi3:
/* R9 = a,        R10 = b
	Return R15 = a + b (no overflow), abort on overflow */
	ADD 	R15,        R9,        R10
/* R2 is overflow flag */
	JZ  	[R7],       R2
	J   	abort

.size                     __addvsi3, . - __addvsi3
#endif

/* 64-bit signed addition with overflow detection */
#ifdef L__addvdi3
.balign                   4
.globl                    __addvdi3
.type                     __addvdi3, @function
__addvdi3:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	R11 = b (low 32 bits),        R12 = b (high 32 bits)
	Return R16:R15 = a + b (no overflow), abort on overflow */
	ADD 	R15,        R9,        R11
	ADDC	R16,        R10,        R12
	JZ  	[R7],       R2
	J   	abort

.size                     __addvdi3, . - __addvdi3
#endif

/* 32-bit signed subtraction with overflow detection */
#ifdef L__subvsi3
.balign                   4
.globl                    __subvsi3
.type                     __subvsi3, @function
__subvsi3:
/* R9 = a,        R10 = b
	Return R15 = a - b (no overflow), abort on overflow */
	SUB 	R15,        R9,        R10
	JZ  	[R7],       R2
	J   	abort

.size                     __subvsi3, . - __subvsi3
#endif

/* 64-bit signed subtraction with overflow detection */
#ifdef L__subvdi3
.balign                   4
.globl                    __subvdi3
.type                     __subvdi3, @function
__subvdi3:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	R11 = b (low 32 bits),        R12 = b (high 32 bits)
	Return R16:R15 = a - b (no overflow), abort on overflow */
	SUB 	R15,        R9,        R11
	SUBB	R16,        R10,        R12
	JZ  	[R7],       R2
	J   	abort

.size                     __subvdi3, . - __subvdi3
#endif


/* 32-bit signed negative with overflow detection */
#ifdef L__negvsi2
.balign                   4
.globl                    __negvsi2
.type                     __negvsi2, @function
__negvsi2:
/* R9 = a
	Return R15 = -a (no overflow), abort on overflow */
	SUB 	R15,        R0,        R9
	JZ  	[R7],       R2
	J   	abort

.size                     __negvsi2, . - __negvsi2
#endif

/* 64-bit signed negative with overflow detection */
#ifdef L__negvdi2
.balign                   4
.globl                    __negvdi2
.type                     __negvdi2, @function
__negvdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	Return R16:R15 = -a (no overflow), abort on overflow */
	SUB 	R15,        R0,        R9
	SUBB	R16,        R0,        R10
	JZ  	[R7],       R2
	J   	abort

.size                     __negvdi2, . - __negvdi2
#endif

/* 32-bit signed abs with overflow detection */
#ifdef L__absvsi2
.balign                   4
.globl                    __absvsi2
.type                     __absvsi2, @function
__absvsi2:
/* R9 = a
	Return R15 = |a| (no overflow), abort on overflow */
	SR		R4,			R9,			31
	MOVZ	R15,        R9,			R0
	JZ  	[R7],       R4
	SUB 	R15,        R0,			R9
	JZ  	[R7],       R2
	J   	abort

.size                     __absvsi2, . - __absvsi2
#endif

/* 64-bit signed abs with overflow detection */
#ifdef L__absvdi2
.balign                   4
.globl                    __absvdi2
.type                     __absvdi2, @function
__absvdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	Return R16:R15 = |a| (no overflow), abort on overflow */
	SR		R4,			R10,		31
	MOVZ	R15,        R9,			R0
	MOVZ	R16,        R10,        R0
	JZ  	[R7],       R4
	SUB 	R15,        R0,			R9
	SUBB	R16,        R0,        	R10
	JZ  	[R7],       R2
	J   	abort

.size                     __absvdi2, . - __absvdi2
#endif

/* 32-bit signed multiplication with overflow detection */
#ifdef L__mulvsi3
.balign                   4
.globl                    __mulvsi3
.type                     __mulvsi3, @function
__mulvsi3:
/* R9 = a,        R10 = b
	Return R15 = a * b (no overflow), abort on overflow */
	MUL 	R15,        R9,        R10
/* R6 is high part of product, 0x00000000 or 0xFFFFFFFF if no overflow */
	JZ  	[R7],       R6
	NOR 	R6,        R6,        R0
	JZ  	[R7],       R6
	J   	abort

.size                     __mulvsi3, . - __mulvsi3
#endif

/* 64-bit signed multiplication */
#ifdef L__muldi3
.balign                   4
.globl                    __muldi3
.type                     __muldi3, @function
__muldi3:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	R11 = b (low 32 bits),        R12 = B (high 32 bits)
Return R18:
	R17:                      R16: R15 = a * b (no overflow), abort on overflow */
	MUL 	R13,        R9,        R11
	OR  	R14,        R0,        R6 /* R14:R13=a*b */
	MUL 	R15,        R10,        R11
	OR  	R16,        R0,        R6 /* R16:R15=A*b */
	MUL 	R17,        R12,        R9
	OR  	R18,        R0,        R6 /* R18:R17=B*a */
	MUL 	R19,        R12,        R10 /* R6:R19=B*A */

	ADD 	R15,        R15,        R14
	ADDC	R16,        R16,        R0
	ADD 	R17,        R17,        R15
	ADDC	R16,        R18,        R16 /* output R16=Lh */
	ADD 	R17,        R19,        R16 /* output R17=Hl */
	ADDC	R18,        R6,        R0 /* output R18=Hh*/
	OR  	R15,        R13,        R0 /* output R15=Ll */

	J   	[R7]

.size                     __muldi3, . - __muldi3
#endif

/* 64-bit signed multiplication with overflow detection */
#ifdef L__mulvdi3
.balign                   4
.globl                    __mulvdi3
.type                     __mulvdi3, @function
__mulvdi3:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	R11 = b (low 32 bits),        R12 = B (high 32 bits)
	Return R18:R17:R16:R15 = a * b (no overflow), abort on overflow */
	MUL 	R13,        R9,        R11
	OR  	R14,        R0,        R6 /* R14:R13=a*b */
	MUL 	R15,        R10,        R11
	OR  	R16,        R0,        R6 /* R16:R15=A*b */
	MUL 	R17,        R12,        R9
	OR  	R18,        R0,        R6 /* R18:R17=B*a */
	MUL 	R19,        R12,        R10 /* R6:R19=B*A */

	ADD 	R15,        R15,        R14
	ADDC	R16,        R16,        R0
	ADD 	R17,        R17,        R15
	ADDC	R16,        R18,        R16 /* output R16=Lh */
	ADD 	R17,        R19,        R16 /* output R17=Hl */
	ADDC	R18,        R6,        R0 /* output R18=Hh*/
	OR  	R15,        R13,        R0 /* output R15=Ll */

	XOR 	R6,        R18,        R17
	JZ  	__mulvdi3_nov,        R6
	J   	abort

__mulvdi3_nov:
	JZ  	[R7],       R18
	NOR 	R18,        R18,        R0
	JZ  	[R7],       R18
	J   	abort

.size                     __mulvdi3, . - __mulvdi3
#endif


/* 64-bit logical shift right */
#ifdef L__lshrdi3
.balign                   4
.globl                    __lshrdi3
.type                     __lshrdi3, @function
__lshrdi3:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	R11 = b
	Return R16:R15 = a >> b */

/* if b=0 */
	JZ  	[R7],       R11


	CMPU	R11, 32
/* R4 is less flag */
	JNZ 	__lshrdi3_less32,        R4

/* if b>=32 */
	ADD 	R11,        R11,        -32
	OR  	R16,        R0,        R0
	SR  	R15,        R10,        R11
	J   	[R7]

/* if b<32 */
__lshrdi3_less32:
	OR  	R12,        R0,        32
	SR  	R15,        R9,        R11
	SUB 	R12,        R12,        R11
	SR  	R16,        R10,        R11
	SL  	R17,        R10,        R12
	OR  	R15,        R15,        R17
	J   	[R7]

.size                     __lshrdi3, . - __lshrdi3
#endif

/* 64-bit arithmetic shift left */
#ifdef L__ashldi3
.balign                   4
.globl                    __ashldi3
.type                     __ashldi3, @function
__ashldi3:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	R11 = b
	Return R16:R15 = a << b */

/* if b=0 */
	JZ  	[R7],       R11

	CMPU	R11, 32
/* R4 is less flag */
	JNZ 	__ashldi3_less32,        R4

/* if b>=32 */
	ADD 	R11,        R11,        -32
	SL  	R15,        R10,        R11
	OR  	R16,        R0,        R0 /* 高32位补0 */
	J   	[R7]

/* if b<32 */
__ashldi3_less32:
	OR  	R12,        R0,        32
	SL  	R15,        R9,        R11
	SUB 	R12,        R12,        R11
	SL  	R16,        R10,        R11
	SR  	R17,        R9,        R12
	OR  	R16,        R16,        R17
	J   	[R7]

.size                     __ashldi3, . - __ashldi3
#endif

/* 64-bit arithmetic shift right */
#ifdef L__ashrdi3
.balign                   4
.globl                    __ashrdi3
.type                     __ashrdi3, @function
__ashrdi3:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	R11 = b
	Return R16:R15 = a >> b (arithmetic) */

/* if b=0 */
	JZ  	[R7],       R11

	CMPU	R11,        32
/* R4 is less flag */
	JNZ 	__ashrdi3_less32,        R4

/* if b>=32 */
	ADD 	R11,        R11,        -32
	SA  	R15,        R10,        R11 /* 算术右移高32位 */
	SA  	R16,        R10,        31 /* 符号位扩展（全1或全0） */
	J   	[R7]

/* if b<32 */
__ashrdi3_less32:
	OR  	R12,        R0,        32
	SR  	R15,        R9,        R11
	SUB 	R12,        R12,        R11
	SL  	R17,        R10,        R12
	OR  	R15,        R15,        R17
	SA  	R16,        R10,        R11 /* 算术右移高32位 */
	J   	[R7]

.size                     __ashrdi3, . - __ashrdi3
#endif

/* 64-bit byte swap */
#ifdef L__bswapdi2
.balign                   4
.globl                    __bswapdi2
.type                     __bswapdi2, @function
__bswapdi2:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	Return R16:R15 = byte-swapped a */

/* Reverse low 32 bits */
	RVB 	R16,        R9

/* Reverse high 32 bits */
	RVB 	R15,        R10

	J   	[R7]

.size                     __bswapdi2, . - __bswapdi2
#endif

/* Find first set bit (from LSB to MSB order) */
#ifdef L__ffssi2
.balign                   4
.globl                    __ffssi2
.type                     __ffssi2, @function
__ffssi2:
/* R9 = a 
	Return R15 = position of first set bit (1-32), 0 if no set bit */

/* Check low 32 bits */
	OR  	R15,        R0,        R0
	JZ  	[R7],       R9
	RV  	R9,        R9
	CLZ 	R15,        R9
	ADD 	R15,        R15, 1
	J   	[R7]

.size                     __ffssi2, . - __ffssi2
#endif



/* Find first set bit (from LSB to MSB order) in 64-bit integer */
#ifdef L__ffsdi2
.balign                   4
.globl                    __ffsdi2
.type                     __ffsdi2, @function
__ffsdi2:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	Return R15 = position of first set bit (1-64), 0 if no set bit */

/* Check low 32 bits */
	JZ  	__ffsdi2_high,        R9
	RV  	R9,        R9
	CLZ 	R15,        R9
	ADD 	R15,        R15,       1
	J   	[R7]

__ffsdi2_high:
	JZ  	__ffsdi2_zero,        R10
	OR  	R15,        R0,       33
	RV  	R10,        R10
	CLZ 	R11,        R10
	ADD 	R15,        R11,        R15
	J   	[R7]

__ffsdi2_zero:
	OR  	R15,        R0,        R0
	J   	[R7]

.size                     __ffsdi2, . - __ffsdi2
#endif

/* Count leading zero bits */
#ifdef L__clzsi2
.balign                   4
.globl                    __clzsi2
.type                     __clzsi2, @function
__clzsi2:
/* R9 = a
	Return R15 = leading zero bits */

	CLZ 	R15,        R9
	J   	[R7]

.size                     __clzsi2, . - __clzsi2
#endif

/* Count leading zero bits in 64-bit integer */
#ifdef L__clzdi2
.balign                   4
.globl                    __clzdi2
.type                     __clzdi2, @function
__clzdi2:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	Return R15 = leading zero bits */

	CLZ 	R15,        R10
	CMP 	R15,        32
/* R3 is equal flag */
	JNZ 	__clzdi2_low,        R3
	J   	[R7]

__clzdi2_low:
	CLZ 	R9,        R9
	ADD 	R15,        R15,        R9
	J   	[R7]

.size                     __clzdi2, . - __clzdi2
#endif

/* Count trailing zeros */
#ifdef L__ctzsi2
.balign                   4
.globl                    __ctzsi2
.type                     __ctzsi2, @function
__ctzsi2:
/* R9 = a
	Return R15 = trailing zero bits */
	RV  	R9,        R9
	CLZ 	R15,        R9
	J   	[R7]

.size                     __ctzsi2, . - __ctzsi2
#endif

/* 64-bit Count trailing zeros */
#ifdef L__ctzdi2
.balign                   4
.globl                    __ctzdi2
.type                     __ctzdi2, @function
__ctzdi2:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	Return R15 = trailing zero bits */
	RV  	R9,        R9
	CLZ 	R15,        R9
	CMP 	R15,        32
/* R3 is equal flag */
	JNZ 	__ctzdi2_high,        R3
	J   	[R7]

__ctzdi2_high:
	RV		R10,        R10
	CLZ 	R10,        R10
	ADD 	R15,        R15,        R10
	J   	[R7]

.size                     __ctzdi2, . - __ctzdi2
#endif


/* Count leading sign bits */
#ifdef L__clrsbsi2
.balign                   4
.globl                    __clrsbsi2
.type                     __clrsbsi2, @function
__clrsbsi2:
/* R9 = a
	Return R15 = sign bits */
	CLZ		R15,        R9
	SR		R4,			R9,			31
	JZ		__clrsbsi2_pos,        R4
	CLO		R15,        R9

__clrsbsi2_pos:
	ADD		R15,        R15,        -1
	J   	[R7]

.size                     __clrsbsi2, . - __clrsbsi2
#endif

/* 64-bit Count leading sign bits */
#ifdef L__clrsbdi2
.balign                   4
.globl                    __clrsbdi2
.type                     __clrsbdi2, @function
__clrsbdi2:
/* R9 = a (low 32 bits),        R10 = A (high 32 bits)
	Return R15 = sign bits */
	CLZ		R15,        R10
	SR		R4,			R10,		31
	JZ		__clrsbdi2_pos,        	R4
	CLO		R15,        R10
	CMPU	R15,        32
	JZ		__clrsbdi2_ret,        	R3
	CLO		R9,        R9
	ADD		R15,        R15,        R9
	J		__clrsbdi2_ret

__clrsbdi2_pos:
	CMPU	R15,        32
	JZ		__clrsbdi2_ret,        R3
	CLZ		R9,        R9
	ADD		R15,        R15,        R9
__clrsbdi2_ret:
	ADD		R15,        R15, -1
	J   	[R7]

.size                     __clrsbdi2, . - __clrsbdi2
#endif


/* Population count (count set bits) - no hardware instruction */
#ifdef L__popcountsi2
.balign                   4
.globl                    __popcountsi2
.type                     __popcountsi2, @function
__popcountsi2:
/* R9 = input value,        Result in R15 */
	OR  	R15,        R0,        R0 /* count = 0 */

__popcountsi2_shift:
	JZ  	__popcountsi2_ret,        R9 /* if x == 0, done */
	AND 	R10,        R9,         1 /* x & 1 */
	ADD 	R15,        R15,        R10 /* count += x & 1 */
	SR  	R9,        R9,         1 /* x >>= 1 */
	J   	__popcountsi2_shift

__popcountsi2_ret:
	J   	[R7]

.size                     __popcountsi2, . - __popcountsi2
#endif



/* 64-bit population count */
#ifdef L__popcountdi2
.balign                   4
.globl                    __popcountdi2
.type                     __popcountdi2, @function
__popcountdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	Return R15 = number of set bits */
	OR  	R15,        R0,        R0 /* count = 0 */

__popcountdi2_Lshift:
	JZ  	__popcountdi2_Hshift,        R9 /* if x == 0, done */
	AND 	R11,        R9,        1 /* x & 1 */
	ADD 	R15,        R15,        R11 /* count += x & 1 */
	SR  	R9,        R9,        1 /* x >>= 1 */
	J   	__popcountdi2_Lshift

__popcountdi2_Hshift:
	JZ  	__popcountdi2_ret,        R10 /* if x == 0, done */
	AND 	R11,        R10,        1 /* x & 1 */
	ADD 	R15,        R15,        R11 /* count += x & 1 */
	SR  	R10,        R10,        1 /* x >>= 1 */
	J   	__popcountdi2_Hshift

__popcountdi2_ret:
	J   	[R7]

.size                     __popcountdi2, . - __popcountdi2
#endif


/* Parity (XOR of all bits) */
#ifdef L__paritysi2
.balign                   4
.globl                    __paritysi2
.type                     __paritysi2, @function
__paritysi2:
/* R9 = input value,        Result in R15 */

/* XOR all 4 bytes */
	SR  	R15,        R9,        16
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        8
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        4
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        2
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        1
	XOR 	R9,        R9,        R15
	AND 	R15,        R9,        1 /* parity bit */
	J   	[R7]

.size                     __paritysi2, . - __paritysi2
#endif

/* 64-bit Parity (XOR of all bits) */
#ifdef L__paritydi2
.balign                   4
.globl                    __paritydi2
.type                     __paritydi2, @function
__paritydi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	Return R15 = number of set bits */

/* XOR all 4 bytes */
__paritydi2_low:
	SR  	R15,        R9,        16
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        8
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        4
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        2
	XOR 	R9,        R9,        R15
	SR  	R15,        R9,        1
	XOR 	R9,        R9,        R15

/* XOR all 4 bytes */
__paritydi2_high:
	SR  	R15,        R10,        16
	XOR 	R10,        R10,        R15
	SR  	R15,        R10,        8
	XOR 	R10,        R10,        R15
	SR  	R15,        R10,        4
	XOR 	R10,        R10,        R15
	SR  	R15,        R10,        2
	XOR 	R10,        R10,        R15
	SR  	R15,        R10,        1
	XOR 	R10,        R10,        R15

	XOR		R15,        R9,        R10
	AND 	R15,        R15,        1 /* parity bit */
	J   	[R7]

.size                     __paritydi2, . - __paritydi2
#endif

/* 64-bit signed comparison */
#ifdef L__cmpdi2
.balign                   4
.globl                    __cmpdi2
.type                     __cmpdi2, @function
__cmpdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	R11 = b (low 32 bits),        R12 = b (high 32 bits)
	Return R15 = 0 (a < b), 1 (a == b), 2 (a > b) */

	SUB 	R15,        R9,        R11
	SUBB	R13,        R10,        R12
	/* R2 is overflow flag, 0 is not overflow */
	JZ  	__cmpdi2_nov,        R2
	OR		R15,        R0,        R0
	J   	[R7]

__cmpdi2_nov:
	SR		R4,			R13,		31
	JZ		__cmpdi2_pos,        R4
	OR		R15,        R0,        R0
	J   	[R7]	

__cmpdi2_pos:
	JNZ		__cmpdi2_gt,        R15
	JNZ		__cmpdi2_gt,        R13
	OR		R15,        R0,       1
	J   	[R7]

__cmpdi2_gt:
	OR		R15,        R0,        2
	J   	[R7]

.size                     __cmpdi2, . - __cmpdi2
#endif

/* 64-bit unsigned comparison */
#ifdef L__ucmpdi2
.balign                   4
.globl                    __ucmpdi2
.type                     __ucmpdi2, @function
__ucmpdi2:
/* R9 = a (low 32 bits),        R10 = a (high 32 bits)
	R11 = b (low 32 bits),        R12 = b (high 32 bits)
	Return R15 = 0 (a < b), 1 (a == b), 2 (a > b) */
	SUB 	R15,        R9,        R11
	SUBB	R13,        R10,        R12
	/* R4 is borrow flag, 0 is borrow */
	JNZ  	__ucmpdi2_nbor,        R4
	OR		R15,        R0,        R0
	J		[R7]

__ucmpdi2_nbor:
	JNZ		__ucmpdi2_gt,        R15
	JNZ		__ucmpdi2_gt,        R13
	OR		R15,        R0,        1
	J   	[R7]

__ucmpdi2_gt:
	OR		R15,        R0,        2
	J   	[R7]


.size                     __ucmpdi2, . - __ucmpdi2
#endif