	.file	"test_cmov.c"
# GNU C23 (GCC) version 15.3.0 (oc32-none-elf)
#	compiled by GNU C version 15.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mzfsf -Os
	.section	.text
	.align 2
	.global	test_cmov0
	.type	test_cmov0, @function
test_cmov0:
# ./test_cmov.c:3:     return a ? 7 : 10;
		ADD		R15,  R0,  7	# _1,
		ADD		R14,  R0,  10	# tmp45,
		MOVZ	R15,  R14,  R9	# _1, tmp45, a
# ./test_cmov.c:4: }
		J		[R7]		#
	.size	test_cmov0, .-test_cmov0
	.align 2
	.global	test_cmov1
	.type	test_cmov1, @function
test_cmov1:
# ./test_cmov.c:9:     return a ? 10 : a;
		ADD		R15,  R0,  10	# tmp44,
		MOVZ	R15,  R9,  R9	# tmp44, a, a
# ./test_cmov.c:10: }
		J		[R7]		#
	.size	test_cmov1, .-test_cmov1
	.align 2
	.global	test_cmov2
	.type	test_cmov2, @function
test_cmov2:
# ./test_cmov.c:15:     return a ? a : b;
		OR		R15,  R0,  R9	# b, a
		MOVZ	R15,  R10,  R9	# b, b, a
# ./test_cmov.c:16: }
		J		[R7]		#
	.size	test_cmov2, .-test_cmov2
	.align 2
	.global	test_cmov3
	.type	test_cmov3, @function
test_cmov3:
		AND		R11,  R11,  0xFF	# b, b
# ./test_cmov.c:21:     c = s ? a : b;
		AND		R15,  R10,  0xFF	# a, a
		MOVZ	R15,  R11,  R9	# a, b, s
# ./test_cmov.c:23: }
		J		[R7]		#
	.size	test_cmov3, .-test_cmov3
	.ident	"GCC: (GNU) 15.3.0"
