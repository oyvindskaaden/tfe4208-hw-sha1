#include "sha1.h"

void test(int a) 
{
    printf("Print from test: %d\n", a);
}

uint8_t*
set_digest(uint8_t *digest, uint32_t *result) 
{
    for (int i = 0; i < SHA1_LEN_BYTES; i++) {
        memcpy(digest + (4 - i) * sizeof(uint32_t), &result[i], sizeof(uint32_t));
    }
    return digest;
}

uint32_t* 
SHA1(uint32_t *digest, uint8_t *data, uint64_t data_length) 
{

    uint32_t h[5] = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    
    uint64_t data_length_bits = data_length * 8;

    while (digest(h, data, &data_length_bits, &data_length)) 
    {
        data_length -= 64;
        data += 64;
    }




    return digest;
}   

int 
digest(uint32_t *hash_words, uint8_t *data, uint64_t* data_len_bits, int *rem_data_bytes)
{    
    int ret = 1;
    uint32_t a, b, c, d, e;
    uint32_t f = 0, k = 0;
    uint32_t w[NO_WORK_WORDS];

    memset(w, 0, NO_WORK_WORDS * sizeof(uint32_t));

    memcpy(w, data, rem_data_bytes);

    // If there is room for a 
    if (rem_data_bytes < CHUNK_SIZE_BYTES)
    {
        *(w + *rem_data_bytes) = 0x80;
    }

    // Check if there is room for a 64 bit number at the end of the chunk
    if (*rem_data_bytes + 1 < (CHUNK_SIZE_BYTES - sizeof(uint64_t))) 
    {
        int i = sizeof(uint64_t);
        for (int i = 0; i < sizeof(uint64_t); i++)
        {
            *((uint8_t*)w + CHUNK_SIZE_BYTES - sizeof(uint64_t) + i) = *((uint8_t*)data_len_bits + (sizeof(uint64_t) - 1 - i));
        }
        ret = 0;
    }
    
    /* DO PROCESSING */


    *data               -= (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;

    *rem_data_bytes     -= (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    

}