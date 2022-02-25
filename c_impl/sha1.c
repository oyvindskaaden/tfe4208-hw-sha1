#include "sha1.h"

void test(int a) {
    printf("Print from test: %d\n", a);
}

uint8_t *set_digest(uint8_t *digest, uint32_t *result) {
    for (int i = 0; i < 20; i++) {
        memcpy(digest + (4 - i) * sizeof(uint32_t), result[i], sizeof(uint32_t));
    }
    return digest;
}

uint8_t *SHA1(uint8_t *digest, uint8_t *data, uint64_t data_length) {

    uint32_t h[5] = {
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    };

    uint32_t a, b, c, d, e;
    uint32_t f = 0, k = 0;
    uint32_t w[80];
    uint64_t data_length_bits = data_length * 8;






    return digest;
}   