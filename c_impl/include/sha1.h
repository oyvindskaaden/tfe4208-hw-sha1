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

#define big_endian(value) (((value & 0xff) << 24) | (((value & 0xff00)) << 8) | (((value & 0xff0000)) >> 8) | ((value & 0xff000000) >> 24))


void test(int a);

uint32_t    *SHA1(uint32_t *digest, uint8_t *data, uint64_t data_length);
int         digest(uint32_t *hash_words, uint8_t *data, uint64_t* data_len_bits, int num_data_bytes);
