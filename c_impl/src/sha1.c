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

    // Datatail is used to pad out the data stream so that the 
    // total datalenght is a multiple of 512 bits or 64 bytes
    uint8_t datatail[2 * CHUNK_SIZE_BYTES];



    uint64_t data_length_bits = data_length * 8;

    while (digest(h, data, &data_length_bits, &data_length)) 
    {
        data_length -= 64;
        data += 64;
    }



    //hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
    
    
    hh = h[0] << 128 | h[1] << 96 | h[2] << 64 | h[3] << 32 | h[4];  
    return digest;
}   

uint8_t*
prepare_datatail(uint8_t data_tail[2 * CHUNK_SIZE_BYTES], uint64_t data_len, int no_tailbytes)
{
    // Insert 0x80 at the end because the message is a multiple of 8
    data_tail[0] = 0x80;

    uint64_t data_len_bits = data_len * 8;
    // Insert the length of bits at the end of the tail as big-endian. (flip the bytes)
    for (int i = 0; i < sizeof(uint64_t); i++)
        data_tail[no_tailbytes - 1 - i] = *((uint8_t*)&data_len_bits + i);
}

int 
digest(uint32_t *hash_words, uint8_t *data, int *rem_data_bytes, uint8_t* data_tail, int* rem_tail_bytes)
{    
    int ret = 1;
    uint32_t a, b, c, d, e;
    uint32_t f = 0, k = 0;
    uint32_t w[NO_WORK_WORDS];

    memset(w, 0, NO_WORK_WORDS * sizeof(uint32_t));

    //memcpy(w, data, rem_data_bytes);

    int chunk_size = (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;


    for (int i = 0; i < chunk_size; i++)
    {
        // Copy the data stream into the working words, 
        // also convert from little-endian to big-endian.
        memcpy((uint8_t*)w + ((~0x7) & i ) + sizeof(uint64_t) - 1 - (i % sizeof(uint64_t)), data + i, 1);
    }

    int rem_bytes_in_chunk = CHUNK_SIZE_BYTES - chunk_size;
    if (rem_bytes_in_chunk)
    {
        memcpy((uint8_t*)w + chunk_size, data_tail, rem_bytes_in_chunk);
    }
    
    /* DO PROCESSING */
    for( i=16; i<80; i++)
    {  
       // Note 3: SHA-0 differs by not having this leftrotate.
        w[i] = rol((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]) , 1);
    }


    a = h[0];
    b = h[1];
    c = h[2];
    d = h[3];
    e = h[4];

    for (int i = 0; i<80 ; i++)
    {
        switch (i)
        {
        case 0 ... 19:
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            break;
        
        case 20 ... 39:
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            break;

        case 40 ... 59:
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            break;        
        
        case 60 ... 79:
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            break;        

        default:
            break;

            temp = (rol(a,5))+f+e+k+w[i];
            e = d;
            d = c;
            c = rol(b,30);
            b = a;
            a = temp;
        }
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }

    *data               -= (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;

    *rem_data_bytes     -= (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    
}