#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void bin_print(uint64_t x)
{
    int i;
    for (i=63; i>=0; i--)
	printf("%d", (x>>i) &1ULL);
}

void bwi_print(uint64_t *bmp, int H, int W)
{
    int i, j;
    for (i=0; i<H; i++) {
	for (j=0; j<W; j++) 
	    bin_print(bmp[i*W+j]);
	printf("\n");
    }
}

void bmp_print(uint8_t *bmp, int H, int W)
{
    int i, j;
    for (i=0; i<H; i++) {
	for (j=0; j<W; j++)
	    printf("%d ", bmp[i*W+j]);
	printf("\n");
    }
}

void bmp_write(char *dst, uint8_t *bmp, int H, int W)
{
    int i;
    FILE *pf = fopen(dst, "wb");
    assert(pf);
    fwrite(&H, sizeof(int), 1, pf);
    fwrite(&W, sizeof(int), 1, pf);
    for (i=0; i<H*W; i++)
	fwrite(bmp+i, sizeof(uint8_t), 1, pf);
    fclose(pf);
}

uint8_t *bmp_read(char *src, int *H, int *W, int *count)
{
    int i;
    uint8_t *bmp;
    FILE *pf = fopen(src, "rb");
    assert(pf);
    fread(H, sizeof(int), 1, pf);
    fread(W, sizeof(int), 1, pf);
    bmp = malloc(sizeof(uint8_t)*(*H)*(*W));
    for (i=0; i<(*W)*(*H); i++) {
	fread(bmp+i, sizeof(uint8_t), 1, pf);
	if (bmp[i] == 1) (*count)++;
    }
    fclose(pf);
    return bmp;
}

void bwi_write(char *file, uint64_t *bmp, int H, int W)
{
    int i, j, w = W*64;
    uint8_t c;
    FILE *pf = fopen(file, "wb");
    assert(pf);
    fwrite(&H, sizeof(int), 1, pf);
    fwrite(&w, sizeof(int), 1, pf);
    for (i=0; i<H*W; i++)
	for (j=63; j>=0; j--) {
	    c = (bmp[i] >> j) & 1;
	    fwrite(&c, sizeof(uint8_t), 1, pf);
	}
    fclose(pf);
}

int bit_count(uint64_t n)
{
    int count;
    for (count=0; n; count++)
	n &= n-1;
    return count;
}

uint64_t *bwi_read(char *file, int *H, int *W, int *count)
{
    int i=0, j, w, k;
    uint8_t c;
    uint64_t num, *bmp;
    FILE *pf = fopen(file, "rb");
    assert(pf);
    fread(H, sizeof(int), 1, pf);
    fread(&w, sizeof(int), 1, pf); *W = w/64;
    bmp = malloc(sizeof(uint64_t)*(*H)*(*W));
    for (i=0; i<(*H); i++) {
	fseek(pf, 8+i*w, SEEK_SET);
	for (j=0; j<(*W); j++) {
	    k=63; num=0;
	    while (k >= 0) {
		fread(&c, sizeof(uint8_t), 1, pf);
		if (c&1) num |= 1ULL << k;
		k--;
	    }
	    (*count) += bit_count(num);
	    bmp[i*(*W) + j] = num;
	}
    }
    fclose(pf);
    return bmp;
}
