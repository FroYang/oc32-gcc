	.file	"test_postinc.c"
# GNU C23 (GCC) version 15.3.0 (oc32-none-elf)
#	compiled by GNU C version 15.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mzfsf -Os
	.section	.text
	.align 2
	.global	copy1
	.type	copy1, @function
copy1:
# ./test_postinc.c:2:     int sum = 0;
		ADD		R15,  R0,  0	# <retval>,
.L2:
# ./test_postinc.c:3:     while (n--) 
		JNZ		.L3,  R11	#, n
# ./test_postinc.c:9: }
		J		[R7]		#
.L3:
# ./test_postinc.c:3:     while (n--) 
		ADD		R11,  R11,  -1	# n, n,
# ./test_postinc.c:5: 		sum += *s++;      /* LWI post_inc */
		LWI		R14,  [R10],  4	# MEM[(int *)s_11 + 4294967292B], s
		ADD		R15,  R15,  R14	# <retval>, <retval>, MEM[(int *)s_11 + 4294967292B]
# ./test_postinc.c:6:     		*d++ = sum;                    /* SWI post_inc */
		SWI		[R9],  R15,  4	# d, <retval>
		J		.L2		#
	.size	copy1, .-copy1
	.align 2
	.global	copy2
	.type	copy2, @function
copy2:
# ./test_postinc.c:14:     int sum = 0;
		ADD		R15,  R0,  0	# <retval>,
.L5:
# ./test_postinc.c:15:     while (n--) 
		JNZ		.L6,  R11	#, n
# ./test_postinc.c:23: }
		J		[R7]		#
.L6:
# ./test_postinc.c:15:     while (n--) 
		ADD		R11,  R11,  -1	# n, n,
# ./test_postinc.c:17: 		sum += *s;      /* LWI post_inc */
		LWI		R14,  [R10],  12	# MEM[(int *)s_3], s,
		ADD		R15,  R15,  R14	# <retval>, <retval>, MEM[(int *)s_3]
# ./test_postinc.c:19:     		*d = sum;                    /* SWI post_inc */
		SWI		[R9],  R15,  24	# d, <retval>,
		J		.L5		#
	.size	copy2, .-copy2
	.align 2
	.global	copy3
	.type	copy3, @function
copy3:
# ./test_postinc.c:32: 		s = s + m;
		SL		R12,  R12,  2	# _3, m,
# ./test_postinc.c:28:     int sum = 0;
		ADD		R15,  R0,  0	# <retval>,
.L8:
# ./test_postinc.c:29:     while (n--) 
		JNZ		.L9,  R11	#, n
# ./test_postinc.c:37: }
		J		[R7]		#
.L9:
# ./test_postinc.c:29:     while (n--) 
		ADD		R11,  R11,  -1	# n, n,
# ./test_postinc.c:31: 		sum += *s;      /* LWI post_inc */
		LWI		R14,  [R10],  R12	# *s_5, s, _3
		ADD		R15,  R15,  R14	# <retval>, <retval>, *s_5
# ./test_postinc.c:33:     		*d = sum;                    /* SWI post_inc */
		SWI		[R9],  R15,  R12	# d, <retval>, _3
		J		.L8		#
	.size	copy3, .-copy3
	.ident	"GCC: (GNU) 15.3.0"
