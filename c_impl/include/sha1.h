#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define SHA1_LEN_WORDS      5                  /* 5 32 bit words in each digest */
#define SHA1_LEN_BYTES      SHA1_LEN_WORDS * 4 /* 160 bits */

#define CHUNK_SIZE_BYTES    64                  /* 512 bits */
#define NO_WORK_WORDS       80                  /* 80 words */

#define MINIMUM_TAIL_BYTES  9                   /* 1 byte for 0x80 and 8 bytes for the 64 bit data length in bits */

#define rol(value, bits)    (((value) << (bits)) | ((value) >> (32 - (bits))))
#define ror(value, bits)    (((value) >> (bits)) | ((value) << (32 - (bits))))

#define big_endian(value)   (((value & 0xff) << 24) | (((value & 0xff00)) << 8) | (((value & 0xff0000)) >> 8) | ((value & 0xff000000) >> 24))

//#define DEBUG
#ifdef DEBUG
#define DBGPRT(format, args...)                         {printf("[DEBUG] ");printf(format, ##args);}
#define DBGPRTARR(array_ptr, array_len, array_name)     {printf("[DEBUG] %s ", array_name);debug_print_array_bytes(array_ptr, array_len);}
#else
#define DBGPRT(format, args...)
#define DBGPRTARR(array_ptr, array_len, array_name)
#endif

/**
 * Struct to store all necessary pointers and values to do the digest loop
 * 
 */
typedef struct sha1_ctrl
{
    uint8_t *data_bytes;        /*!< Pointer to the data bytes array */
    uint8_t *tail_bytes;        /*!< Pointer to the tail bytes array */
    uint64_t rem_data_bytes;    /*!< Remaining data bytes */
    uint64_t rem_tail_bytes;    /*!< Remaining tail bytes */
} SHA1_Control_t;

/**
 * Calculates the SHA1 of the given datastream pointer
 *
 * @param digest        Pointer to a 5 long uint32 array to store the digested sha1 result
 * @param data          Pointer to the data bytes the SHA1 is to be calculated on
 * @param data_length   Length of the data in bytes
 * @return A pointer to the digest array
 */
uint32_t *SHA1(uint32_t *digest, uint8_t *data, uint64_t data_length);


/**
 *  Prepares the tail bytes given the length of data and number of tail bytes
 * 
 * @param tail_bytes    Pointer to the array containing the tail bytes
 * @param data_len      Total length of the data
 * @param no_tailbytes  Total number of tailbytes
 * @return A pointer to the tail bytes array
 */
uint8_t *prepare_tailbytes(uint8_t tail_bytes[CHUNK_SIZE_BYTES + 8], uint64_t data_len, int no_tailbytes);


/**
 *  Prepares the working words (512 bit) and does the calculations
 * 
 * @param hash_words    The starting hashwords. Changes as program propegates
 * @param sha1_ctrl     Pointer to a SHA1_Control_t struct containing the data pointer, data length, tail bytes and tail length
 * @return Returns true if it finished the calculations, false if there are more data in either data or tail bytes
 */
bool digest_chunk(uint32_t *hash_words, SHA1_Control_t *sha1_ctrl);

/**
 *  Prints the digest in hexadecimal to stdout
 * 
 * @param digest            Pointer to a 5 long uint32 array containing the digest
 * @param append_newline    Set to true if the print should end in a newline
 */
void print_sha(uint32_t *digest, bool append_newline);


/**
 *  Format digest into a 40 char string
 * 
 * @param digest        Pointer to a 5 long uint32 array containing the digest
 * @param sha1_string   Pointer to where to put the formatted string
 * @return char* 
 */
char *sha1_to_string(uint32_t *digest, char *sha1_string);

/**
 *  Debug function to print a data array
 * 
 * @param array_ptr Pointer to the array
 * @param array_len How many bytes to print
 */
void debug_print_array_bytes(uint8_t *array_ptr, uint64_t array_len);
