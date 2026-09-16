	.file	"test_bitfield.c"
# GNU C23 (GCC) version 15.3.0 (oc32-none-elf)
#	compiled by GNU C version 15.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mzfsf -Os
	.section	.text
	.align 2
	.global	test_ebf_8_4
	.type	test_ebf_8_4, @function
test_ebf_8_4:
# ./test_bitfield.c:17: }
		EBF		R15,  R9,8,4	#, x,,
		J		[R7]		#
	.size	test_ebf_8_4, .-test_ebf_8_4
	.align 2
	.global	test_ebf_1_7
	.type	test_ebf_1_7, @function
test_ebf_1_7:
# ./test_bitfield.c:22: }
		EBF		R15,  R9,1,7	#, x,,
		J		[R7]		#
	.size	test_ebf_1_7, .-test_ebf_1_7
	.align 2
	.global	test_ebf_16_8
	.type	test_ebf_16_8, @function
test_ebf_16_8:
# ./test_bitfield.c:27: }
		EBF		R15,  R9,16,8	#, x,,
		J		[R7]		#
	.size	test_ebf_16_8, .-test_ebf_16_8
	.align 2
	.global	test_ebf_4_0
	.type	test_ebf_4_0, @function
test_ebf_4_0:
# ./test_bitfield.c:32: }
		AND		R15,  R9,  15	#, x,
		J		[R7]		#
	.size	test_ebf_4_0, .-test_ebf_4_0
	.align 2
	.global	test_ebf_4_28
	.type	test_ebf_4_28, @function
test_ebf_4_28:
# ./test_bitfield.c:37: }
		SR		R15,  R9,  28	#, x,
		J		[R7]		#
	.size	test_ebf_4_28, .-test_ebf_4_28
	.align 2
	.global	test_mbf_mem
	.type	test_mbf_mem, @function
test_mbf_mem:
# ./test_bitfield.c:45:     p->hi = x;
		LW		R15,  [R9]	# p_4(D)->hi, p_4(D)->hi
		MBF		R15,4,28,  R10	# p_4(D)->hi,,, x
		SW		[R9],  R15	# p_4(D)->hi, p_4(D)->hi
# ./test_bitfield.c:46: }
		J		[R7]		#
	.size	test_mbf_mem, .-test_mbf_mem
	.align 2
	.global	test_mbf_reg
	.type	test_mbf_reg, @function
test_mbf_reg:
# ./test_bitfield.c:51:     u.u = y;
		OR		R15,  R0,  R10	# u, y
# ./test_bitfield.c:52:     u.s.hi = x;          /* 插入 4 位到 pos 28 */
		MBF		R15,4,28,  R9	# u,,, x
# ./test_bitfield.c:54: }
		J		[R7]		#
	.size	test_mbf_reg, .-test_mbf_reg
	.ident	"GCC: (GNU) 15.3.0"
