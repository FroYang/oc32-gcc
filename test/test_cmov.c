
char test_cmov0 ( int a) {
    return a ? 7 : 10;
}


int test_cmov1 ( int a) {
    //return a ? a : 10;
    return a ? 10 : a;
}


int test_cmov2 ( int a, int b) {
    //return a ? b : a;
    return a ? a : b;
}


unsigned char test_cmov3 ( int s, unsigned char a, unsigned char b) {
    char c;
    c = s ? a : b;
    return c;
}
