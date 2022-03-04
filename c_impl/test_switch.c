#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define big_endian(value) (((value & 0xff) << 24) | (((value & 0xff00)) << 8) | (((value & 0xff0000)) >> 8) | ((value & 0xff000000) >> 24))

int main(int argc, char const *argv[])
{
    uint64_t word = 0x1122334455667788;

    uint64_t drow = 0;

    for (int i = 0; i < sizeof(uint64_t); i++) 
    {
        *((uint8_t*)&drow + i) = *((uint8_t*)&word + sizeof(uint64_t) - 1 - i);
    }

    printf("Word: %0lx\ndroW: %0lx\n", word, drow);

    return 0;
}
