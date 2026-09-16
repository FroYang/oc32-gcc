/*
 * Test cases for OC32 EBF (extzvsi) and MBF (insvsi) instructions.
 * Compile: oc32-gcc -O2 -S test_bitfield.c
 *
 * EBF: zero_extract(dest, src, size, pos)
 *   dest = (src >> pos) & ((1 << size) - 1)
 *
 * MBF: zero_extract(dest, size, pos) = src
 *   inserts low 'size' bits of src into dest at bit position pos
 */

/* ===== EBF (extzvsi) tests ===== */

/* Extract 8 bits from position 4 → EBF r, x, 8, 4 */
unsigned int test_ebf_8_4(unsigned int x) {
    return (x >> 4) & 0xff;
}

/* Extract 1 bit from position 7 → EBF r, x, 1, 7 */
unsigned int test_ebf_1_7(unsigned int x) {
    return (x >> 7) & 1;
}

/* Extract 16 bits from position 8 → EBF r, x, 16, 8 */
unsigned int test_ebf_16_8(unsigned int x) {
    return (x >> 8) & 0xffff;
}

/* Extract 4 bits from position 0 → EBF r, x, 4, 0 */
unsigned int test_ebf_4_0(unsigned int x) {
    return x & 0xf;
}

/* Extract 4 bits from position 28 → EBF r, x, 4, 28 */
unsigned int test_ebf_4_28(unsigned int x) {
    return (x >> 28) & 0xf;
}

/* ===== MBF (insvsi) tests ===== */

struct bf { unsigned lo:28; unsigned hi:4; };

/* 内存目标：GCC 会生成 load + MBF + store（因为 insvsi 只接受寄存器目标）*/
void test_mbf_mem(struct bf *p, unsigned x) {
    p->hi = x;
}

/* 寄存器目标：SRA 后局部变量在寄存器中，expand 直接生成 MBF */
unsigned test_mbf_reg(unsigned x, unsigned y) {
    union { struct bf s; unsigned u; } u;
    u.u = y;
    u.s.hi = x;          /* 插入 4 位到 pos 28 */
    return u.u;
}
