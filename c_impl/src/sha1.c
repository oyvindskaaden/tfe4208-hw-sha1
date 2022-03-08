#include "sha1.h"

void test(int a) 
{
    printf("Print from test: %d\n", a);
}

uint32_t*
set_digest(uint32_t *digest, uint32_t *result) 
{
    // Flip the 32 bit words so that digest is a continious 160 bit digest.
    for (int i = 0; i < SHA1_LEN_BYTES; i++)
        memcpy(&digest[4 - i], &result[i], sizeof(uint32_t));
    
    return digest;
}

uint32_t* 
SHA1(uint32_t *digest, uint8_t *data, uint64_t data_length) 
{
    // Initial hash values
    uint32_t h[5] = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    // Datatail is used to pad out the data stream so that the 
    // total datalenght is a multiple of 512 bits or 64 bytes
    int rem_tail_bytes = 9;

    // The longest datatail happens if there are space for 8 bytes to fill the 64. 
    // Therefore we need at max CHUNK_SIZE_BYTES (64) + 8 bytes to hold all the bits.
    uint8_t data_tail[CHUNK_SIZE_BYTES + 8];

    // Fill the datatail with the appropriate data.
    prepare_datatail(data_tail, data_length, rem_tail_bytes);

    uint64_t data_length_bits = data_length * 8;


    while (!digest_chunk(h, data, &data_length, data_tail, &rem_tail_bytes)) {}
    
    digest = set_digest(digest, h);

    return digest;
}   

uint8_t*
prepare_datatail(uint8_t data_tail[CHUNK_SIZE_BYTES + 8], uint64_t data_len, int no_tailbytes)
{
    // Insert 0x80 at the end because the message is a multiple of 8
    data_tail[0] = 0x80;

    uint64_t data_len_bits = data_len * 8;
    // Insert the length of bits at the end of the tail as big-endian. (flip the bytes)
    for (int i = 0; i < sizeof(uint64_t); i++)
        data_tail[no_tailbytes - 1 - i] = *((uint8_t*)&data_len_bits + i);
}

bool 
digest_chunk(uint32_t *hash_words, uint8_t *data, int *rem_data_bytes, uint8_t* data_tail, int* rem_tail_bytes)
{    
    bool is_complete = false;

    uint32_t a, b, c, d, e;
    uint32_t f = 0, k = 0;
    uint32_t w[NO_WORK_WORDS];

    memset(w, 0, NO_WORK_WORDS * sizeof(uint32_t));

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
        // Fill the remaining bytes with the generated tailbytes
        memcpy((uint8_t*)w + chunk_size, data_tail, rem_bytes_in_chunk);
    }
    if (rem_tail_bytes == rem_bytes_in_chunk)
        is_complete = false;
    
    // Move the pointer to the data_tail array,
    // and decrement the amount of tailbytes to append.
    data_tail       += rem_bytes_in_chunk;
    *rem_tail_bytes -= rem_bytes_in_chunk;

    // Move the pointer to the data bytes array,
    // and decrement the amount of remaining bytes.
    // It does not matter that the data pointer is outside range,
    // it is protected by the rem_data_bytes variable.
    data            += (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    *rem_data_bytes -= (*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    
    
    /* DO PROCESSING */
    

}

