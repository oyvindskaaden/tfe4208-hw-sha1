#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SHA1_LEN_WORDS      5   /* 5 32 bit words in each digest */
#define SHA1_LEN_BYTES      SHA1_LEN_WORDS * 4  /* 160 bits */

#define CHUNK_SIZE_BYTES    64  /* 512 bits */
#define NO_WORK_WORDS       80  /* 80 words */

#define MINIMUM_TAIL_BYTES  9   /* 1 byte for 0x80 and 8 bytes for the 64 bit data length in bits */

#define rol(value, bits)    (((value) << (bits)) | ((value) >> (32 - (bits))))
#define ror(value, bits)    (((value) >> (bits)) | ((value) << (32 - (bits))))

#define big_endian(value)   (((value & 0xff) << 24) | (((value & 0xff00)) << 8) | (((value & 0xff0000)) >> 8) | ((value & 0xff000000) >> 24))

//#define DEBUG
#ifdef DEBUG
#define DBGPRT(format, args...)                     {printf("[DEBUG] ");printf(format, ##args);}
#define DBGPRTARR(array_ptr, array_len, array_name) {printf("[DEBUG] %s ", array_name);debug_print_array_bytes(array_ptr, array_len);}
#else
#define DBGPRT(format, args...)       
#define DBGPRTARR(array_ptr, array_len, array_name)
#endif

typedef struct sha1
{
    uint8_t*    data_bytes;
    uint8_t*    tail_bytes;
    uint64_t    rem_data_bytes;
    uint64_t    rem_tail_bytes;
} SHA1_Control_t;


void test(int a);

uint32_t*   SHA1(uint32_t* digest, uint8_t* data, uint64_t data_length);
uint8_t*    prepare_tailbytes(uint8_t tail_bytes[CHUNK_SIZE_BYTES + 8], uint64_t data_len, int no_tailbytes);
bool        digest_chunk(uint32_t* hash_words, SHA1_Control_t* sha1_ctrl);

void        print_sha(uint32_t *digest, bool append_newline);

void        debug_print_array_bytes(uint8_t* array_ptr, uint64_t array_len);
