#ifndef RW_H
#define RW_H

#include <stdint.h>

void       bin_print   (uint64_t x);
void       bwi_print   (uint64_t *bmp, int H, int W);
void       bmp_print   (uint8_t *bmp);
void       bmp_write   (char *dst, uint8_t *bmp, int H, int W);
uint8_t   *bmp_read    (char *src, int *H, int *W, int *count);
void       bwi_write   (char *file, uint64_t *bmp, int H, int W);
uint64_t  *bwi_read    (char *file, int*H, int *W, int *count);

#endif /* RW_H */
