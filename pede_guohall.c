#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rw.h"

#define GET(i, j)  (i)*W+j

int H, W;

const int      offset[8] = {0, 64, 128, 192, 256, 320, 384, 448};
const uint64_t AM[8]     = {9857253686697263104ULL, 0ULL,
			    62205969853054976ULL,
			    23925373020405760ULL,
			    9861757284034543616ULL, 0ULL,
			    62205969853054976ULL,
			    23925373020471296ULL};
const uint64_t RAM[8]    = {3430940672ULL,
			    3456106496ULL,
			    3472883712ULL,
			    3472883712ULL, 0ULL, 0ULL,
			    216454257342152704ULL,
			    216454257342218240ULL};

int find_next(uint64_t n)
{
    if (n == 0)	return -1;
    return 63 - __builtin_clzll(n);
}

int get_king_neighbors(uint64_t *bmp, int i, int j, int k)
{
    int elem=0;
    if (k == 0) {
	elem |= ((bmp[GET(i-1,   j)] << 1) & 7) << 6;
	elem |= ((bmp[GET(i  ,   j)] << 1) & 7) << 3;
	elem |= ((bmp[GET(i+1,   j)] << 1) & 7);
	elem |=  (bmp[GET(i-1, j+1)] >> 63) << 6;
	elem |=  (bmp[GET(i  , j+1)] >> 63) << 3;
	elem |=  (bmp[GET(i+1, j+1)] >> 63);
    } else {
	elem |= ((bmp[GET(i-1, j)] >> (k-1)) & 7) << 6;
	elem |= ((bmp[GET(i,   j)] >> (k-1)) & 7) << 3;
	elem |= ((bmp[GET(i+1, j)] >> (k-1)) & 7);
	if (k == 63) {
	    elem |= (bmp[GET(i-1, j-1)] & 1ULL) << 8;
	    elem |= (bmp[GET(i  , j-1)] & 1ULL) << 5;
	    elem |= (bmp[GET(i+1, j-1)] & 1ULL) << 2;
	} 
    }
    return elem;
}

int in_automaton(const uint64_t *x, int el)
{
    int idx = el >> 6;
    return (x[idx] >> (el - offset[idx])) & 1ULL;
}

int guohall_iter(uint64_t *curr, uint64_t *copy, int count)
{
    uint64_t tmp;
    int i, j, k, elem;
    static int iter=1;
    const uint64_t *am = iter ? AM : RAM;
    memcpy(copy, curr, sizeof(uint64_t)*H*W);
    for (i=0; i<H; i++)
	for (j=0; j<W; j++) {
	    tmp = curr[GET(i, j)];
	    while ((k = find_next(tmp)) >= 0) {
		elem = get_king_neighbors(curr, i, j, k);
		if (in_automaton(am, elem)) {
		    copy[GET(i, j)] ^= 1ULL<<k;
		    count--;
		}
		tmp ^= 1ULL<<k;
	    }
	}
    iter ^= 1;
    return count;
}

void guohall(uint64_t *bmp, int cc)
{
    int cn;
    uint64_t *cpy = malloc(sizeof(uint64_t)*H*W);
    while (1) {
	cn = guohall_iter(bmp, cpy, cc);
	if (cc - cn == 0) break; cc = cn;
	bmp = memcpy(bmp, cpy, sizeof(uint64_t)*H*W);
    }
    free(cpy);
}

int main(int argc, char **argv)
{
    uint64_t *bmp;
    int count=0, ntimes=1;
    assert(argc == 3);
    ntimes = atoi(argv[2]);
    bmp = bwi_read(argv[1], &H, &W, &count);
    for (int i=0; i<ntimes; i++)
	guohall(bmp, count);
    bwi_write("tmp", bmp, H, W);
    free(bmp);
    return 0;
}
