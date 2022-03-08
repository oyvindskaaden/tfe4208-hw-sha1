#include <stdlib.h>
#include <stdio.h>

#include "sha1.h"

int main(int argc, char const *argv[])
{
    uint32_t digest[5];

    char data[] = "Hello, World!";

    SHA1(digest, (uint8_t*)data, strlen(data));
    
    printf("Data:\n%s\n", data);

    print_sha(digest, true);

    return 0;
}
