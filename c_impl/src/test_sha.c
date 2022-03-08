#include <stdlib.h>
#include <stdio.h>

#include "sha1.h"

int main(int argc, char const *argv[])
{
    uint32_t digest[5];

    char data[] = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

    SHA1(digest, (uint8_t*)data, strlen(data));
    
    printf("Data:\n%s\n", data);

    printf("SHA1:\n");
    print_sha(digest, true);

    return 0;
}
