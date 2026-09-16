int copy1(int *d, int *s, int n) {
    int sum = 0;
    while (n--) 
	{
		sum += *s++;      /* LWI post_inc */
    		*d++ = sum;                    /* SWI post_inc */
	}
    return sum;
}



int copy2(int *d, int *s, int n) {
    int sum = 0;
    while (n--) 
	{
		sum += *s;      /* LWI post_inc */
		s = s + 3;
    		*d = sum;                    /* SWI post_inc */
		d = d + 6;
	}
    return sum;
}



int copy3(int *d, int *s, int n, int m) {
    int sum = 0;
    while (n--) 
	{
		sum += *s;      /* LWI post_inc */
		s = s + m;
    		*d = sum;                    /* SWI post_inc */
		d = d + m;
	}
    return sum;
}



