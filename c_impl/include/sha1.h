#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SHA1_LEN_BYTES      20  /* 160 bits */

#define CHUNK_SIZE_BYTES    64  /* 512 bits */
#define NO_WORK_WORDS       80  /* 80 words */

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
#define ror(value, bits) (((value) >> (bits)) | ((value) << (32 - (bits))))


void test(int a);

uint8_t *SHA1(uint8_t *digest, uint8_t *data, uint64_t data_length);