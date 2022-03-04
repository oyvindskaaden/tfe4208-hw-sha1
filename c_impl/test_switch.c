#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define NO_WORDS 9
#define big_endian(value) (((value & 0xff) << 24) | (((value & 0xff00)) << 8) | (((value & 0xff0000)) >> 8) | ((value & 0xff000000) >> 24))

int main(int argc, char const *argv[])
{
    uint64_t word = 0x1122334455667788;

    uint64_t drow = 0;


    uint64_t word2[NO_WORDS] = {
        0x1122334455667788,
        0x00aabbccddeeffaa,
        0x0022446688aaccee,
        0x1122334455667788,
        0x00aabbccddeeffaa,
        0x0022446688aaccee,
        0x1122334455667788,
        0x00aabbccddeeffaa,
        0x0022446688aaccee
    };
    uint64_t drow2[NO_WORDS];

    for (int i = 0; i < sizeof(uint64_t); i++) 
    {
        *((uint8_t*)&drow + i) = *((uint8_t*)&word + sizeof(uint64_t) - 1 - i);
    }

    for (int i = 0; i < NO_WORDS * sizeof(uint64_t); i++)
    {
        memcpy((uint8_t*)&drow2 + ((~0x7) & i ) + sizeof(uint64_t) - 1 - (i % sizeof(uint64_t)), (uint8_t*)word2 + i, 1);
    }




    printf("word:\t%016lx\ndrow1:\t%016lx\n", word, drow);


    for (int i = 0; i < NO_WORDS; i++)
        printf("word2[%01d] = %016lx | %016lx = [%01d]2drow\n", i, word2[i], drow2[i], i);

    return 0;
}
