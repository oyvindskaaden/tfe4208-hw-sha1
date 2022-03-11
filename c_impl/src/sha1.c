#include "sha1.h"

void test(int a) 
{
    printf("Print from test: %d\n", a);
}

uint32_t*
set_digest(uint32_t *digest, uint32_t *result) 
{
    // Flip the 32 bit words so that digest is a continious 160 bit digest.
    for (int i = 0; i < SHA1_LEN_WORDS; i++)
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

    // Find the padding needed to fill all the 64 byte chunks.
    DBGPRT("Number of bytes to process: %ld", data_length)
    int no_tail_bytes = CHUNK_SIZE_BYTES - (data_length % CHUNK_SIZE_BYTES);

    // If there is not enough room for the minimum 9 tail bytes, 
    if (no_tail_bytes < MINIMUM_TAIL_BYTES)
        no_tail_bytes += CHUNK_SIZE_BYTES;

    DBGPRT("Numer of tail bytes: %d", no_tail_bytes)

    // The longest datatail happens if there are space for 8 bytes to fill the 64. 
    // Therefore we need at max CHUNK_SIZE_BYTES (64) + 8 bytes to hold all the bits.
    uint8_t tail_bytes[CHUNK_SIZE_BYTES + 8];

    // Fill the datatail with the appropriate data.
    prepare_tailbytes(tail_bytes, data_length, no_tail_bytes);

    printf("Data tail: [");
    for (int i = 0; i < CHUNK_SIZE_BYTES + 8; i++)
        printf("%x, ", tail_bytes[i]);
    printf("%x]\n", tail_bytes[CHUNK_SIZE_BYTES + 8 - 1]);

    SHA1_Control_t sha1_ctrl = (SHA1_Control_t){
        .data_bytes     = data,
        .rem_data_bytes = data_length,
        .tail_bytes     = tail_bytes,
        .rem_tail_bytes = no_tail_bytes
    };


    DBGPRT("Data ptr:\t\t%p",       sha1_ctrl.data_bytes)
    DBGPRT("Rem data:\t\t%02ld",    sha1_ctrl.rem_data_bytes)
    DBGPRT("Tail ptr:\t\t%p",       sha1_ctrl.tail_bytes)
    DBGPRT("Rem tail:\t\t%02ld",    sha1_ctrl.rem_tail_bytes)


    // Digest chunk until there is no more data. The function returns true when it is finished
    while (!digest_chunk(h, &sha1_ctrl)) 
    {
        DBGPRT("Data ptr:\t\t%p",       sha1_ctrl.data_bytes)
        DBGPRT("Rem data:\t\t%02ld",    sha1_ctrl.rem_data_bytes)
        DBGPRT("Tail ptr:\t\t%p",       sha1_ctrl.tail_bytes)
        DBGPRT("Rem tail:\t\t%02ld",    sha1_ctrl.rem_tail_bytes)
    }

    digest = set_digest(digest, h);

    //hh = (h0 leftshift 128) or (h1 leftshift 96) or (h2 leftshift 64) or (h3 leftshift 32) or h4
    
    // Is done in set_digest;
    //hh = h[0] << 128 | h[1] << 96 | h[2] << 64 | h[3] << 32 | h[4];  
    return digest;
}   

uint8_t*
prepare_tailbytes(uint8_t tail_bytes[CHUNK_SIZE_BYTES + 8], uint64_t data_len, int no_tailbytes)
{
    memset(tail_bytes, 0, CHUNK_SIZE_BYTES + 8);
    // Insert 0x80 at the end because the message is a multiple of 8
    tail_bytes[0] = 0x80;

    uint64_t data_len_bits = data_len * 8;
    // Insert the length of bits at the end of the tail as big-endian. (flip the bytes)
    for (int i = 0; i < sizeof(uint64_t); i++)
        memcpy(tail_bytes + no_tailbytes - 1 - i, (uint8_t*)&data_len_bits + i, sizeof(uint8_t));
        //tail_bytes[no_tailbytes - 1 - i] = *((uint8_t*)&data_len_bits + i);

    return tail_bytes;
}

bool 
digest_chunk(uint32_t* hash_words, SHA1_Control_t* sha1_ctrl)
{    
    DBGPRT("===== Into loop! =====")
    bool is_complete = false;

    uint32_t a, b, c, d, e;
    uint32_t f = 0, k = 0;
    uint32_t w[NO_WORK_WORDS];

    memset(w, 0, NO_WORK_WORDS * sizeof(uint32_t));

    // Get the chunk size, if more than 64, set it to 64.
    uint8_t chunk_size = (sha1_ctrl->rem_data_bytes > 64) ? 64 : sha1_ctrl->rem_data_bytes;


    for (int i = 0; i < chunk_size; i++)
    {
        // Copy the data stream into the working words, 
        // also convert from little-endian to big-endian.
        memcpy((uint8_t*)&w + ((~0x7) & i ) + sizeof(uint64_t) - 1 - (i % sizeof(uint64_t)), sha1_ctrl->data_bytes + i, sizeof(uint8_t));
        DBGPRT("Data being copied: %02x ", *(*data+i))
    }
    DBGPRTARR((uint8_t*)w, chunk_size)
    

    // If the chunk size is smaller than 64, return the remaining bytes and start copying in from tail bytes
    int rem_bytes_in_chunk = CHUNK_SIZE_BYTES - chunk_size;
    DBGPRT("Remaining bytes in data chunk: %d", rem_bytes_in_chunk)
    if (rem_bytes_in_chunk)
    {
        // Fill the remaining bytes with the generated tailbytes
        memcpy((uint8_t*)w + chunk_size, sha1_ctrl->tail_bytes, rem_bytes_in_chunk);
    }
    //DBGPRTARR((uint8_t*)w, CHUNK_SIZE_BYTES )
    //if (*rem_tail_bytes == rem_bytes_in_chunk)
    //    is_complete = true;
    
    // Move the pointer to the data_tail array,
    // and decrement the amount of tailbytes to append.
    sha1_ctrl->tail_bytes       += rem_bytes_in_chunk;
    sha1_ctrl->rem_tail_bytes   -= rem_bytes_in_chunk;

    DBGPRT("Remaining tail bytes inside loop: %ld", sha1_ctrl->rem_tail_bytes);
    if (!sha1_ctrl->rem_tail_bytes)
        is_complete = true;

    // Move the pointer to the data bytes array,
    // and decrement the amount of remaining bytes.
    // It does not matter that the data pointer is outside range,
    // it is protected by the rem_data_bytes variable.
    sha1_ctrl->data_bytes       += chunk_size;//(*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    sha1_ctrl->rem_data_bytes   -= chunk_size;//(*rem_data_bytes > 64) ? 64 : *rem_data_bytes;
    
    
    /* DO PROCESSING */
    for (int i = 16; i < 80; i++)
        w[i] = rol((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]) , 1);


    a = hash_words[0];
    b = hash_words[1];
    c = hash_words[2];
    d = hash_words[3];
    e = hash_words[4];

    for (int i = 0; i < 80; i++)
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

        }
        uint32_t temp = (rol(a,5)) + f + e + k + w[i];
        e = d;
        d = c;
        c = rol(b, 30);
        b = a;
        a = temp;

    }

    hash_words[0] += a;
    hash_words[1] += b;
    hash_words[2] += c;
    hash_words[3] += d;
    hash_words[4] += e;
    
    DBGPRT("===== Out of loop! =====")
    return is_complete;
}


void
print_sha(uint32_t *digest, bool append_newline)
{
    for (int i = 0; i < 5; i++)
        printf("%08x", digest[i]);
    if (append_newline) 
        putchar('\n');
}

void
debug_print_array_bytes(uint8_t* array_ptr, uint64_t array_len )
{
    printf("Array bytes: ");
    for (int i =0; i < array_len; i++) printf(" %0x", array_ptr[i] );
    printf("\n");
}