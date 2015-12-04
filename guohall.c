#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rw.h"

#define GET(i, j) (i)*(W)+(j)
#define MIN(x, y) (x)<(y) ? (x) : (y)

int W, H;

int guohall_iter(uint8_t *curr, uint8_t *copy, int count)
{
    int i, j;
    uint8_t p1, p2, p3, p4, p5, p6, p7, p8;
    int c, n1, n2, n, d;
    static int iter=0;
    memcpy(copy, curr, sizeof(uint8_t)*W*H);
    for (i=0; i<H; i++) {
    	for (j=0; j<W; j++) {
    	    if (curr[GET(i,j)]) {
    	    	p2 = curr[GET(i-1, j  )];
    	    	p3 = curr[GET(i-1, j+1)];
    	    	p4 = curr[GET(i  , j+1)];
    	    	p5 = curr[GET(i+1, j+1)];
   	    	p6 = curr[GET(i+1, j  )];
    	    	p7 = curr[GET(i+1, j-1)];
    	    	p8 = curr[GET(i  , j-1)];
		p1 = curr[GET(i-1, j-1)];
    	    	c = ((!p2)&&(p3||p4)) + ((!p4)&&(p5||p6)) + \
		    ((!p6)&&(p7||p8)) + ((!p8)&&(p1||p2));
    	    	n1 = (p1||p2) + (p3||p4) + (p5||p6) + (p7||p8);
    	    	n2 = (p2||p3) + (p4||p5) + (p6||p7) + (p8||p1);
    	    	n = MIN(n1, n2);
    	    	d = iter ? ((p2||p3||(!p5))&&p4) : ((p6||p7||(!p1))&&p8);
    	    	if ((c == 1) && (n >= 2 && n <= 3) && (d == 0)) {
    	    	    copy[GET(i, j)] = 0;
    	    	    count--;
		}
	    }
	}
    }
    iter ^= 1;
    return count;
}

void guohall(uint8_t *bmp, int cc)
{
    int cn;
    uint8_t *copy;
    copy = malloc(sizeof(uint8_t)*W*H);
    while (1) {
	cn = guohall_iter(bmp, copy, cc);
	if (cc - cn == 0) break;
	cn = cn;
	memcpy(bmp, copy, sizeof(uint8_t)*W*H);
    }
    free(copy);
}

int main(int argc, char **argv)
{
    int cc=0, ntimes=1;
    uint8_t *bmp;
    assert(argc == 3);
    ntimes = atoi(argv[2]);
    bmp = bmp_read(argv[1], &H, &W, &cc);
    for (int i=0; i<ntimes; i++)  
	guohall(bmp, cc);
    bmp_write("tmp", bmp, H, W);
    free(bmp);
    return 0;
}
