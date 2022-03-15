#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "sha1.h"

#define MAX_CHARS_PRINTED 120
void test_sha1(uint8_t *data, size_t data_len, char *known_correct, char *name_of_test)
{
    printf("===== START TEST (%s) =====\n", name_of_test);
    uint32_t digest[5];
    char result_string[41];

    SHA1(digest, data, data_len);
    sha1_to_string(digest, result_string);

    printf("Data\t\t: ");
    if (data_len <= 120)
        printf("%s\n", data);
    else
        printf("%.*s... (%ld data bytes left)\n", MAX_CHARS_PRINTED, data, data_len - MAX_CHARS_PRINTED);

    printf("SHA1\t\t: %s\n", result_string);
    printf("Known SHA1\t: %s\n", known_correct);

    assert(!strcmp(result_string, known_correct));
    printf("===== TEST COMPLETE =====\n\n");
}

int main(int argc, char const *argv[])
{
    uint32_t digest[5];
    char result_string[41];

    char *data; //= "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef01234567";
    char *known_digest;
    char *name_of_test;
    // char data[] = "abc";
    // char data[] = "";

    // char *data = malloc(1000001 * sizeof(char)); memset(data, 'a', 1000000); data[1000000] = 0;

    // SHA1(digest, (uint8_t*)data, strlen(data));

    // printf("Data:\n%s\n", data);

    // sha1_to_string(digest, result_string);
    // printf("SHA1: %s\n", result_string);
    // print_sha(digest, true);

    /***** TESTS *****/

    // Test vectors from https://www.di-mgt.com.au/sha_testvectors.html

    // Empty data
    name_of_test    = "Empty data";
    data            = "";
    known_digest    = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    // Simple: 'abc'
    name_of_test    = "Simple: 'abc'";
    data            = "abc";
    known_digest    = "a9993e364706816aba3e25717850c26c9cd0d89d";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    // Fill the buffer
    name_of_test    = "Fill the buffer";
    data            = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    known_digest    = "84983e441c3bd26ebaae4aa1f95129e5e54670f1";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    // Two rounds with digest
    name_of_test    = "Two rounds with digest";    
    data            = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    known_digest    = "a49b2446a02c645bf419f995b67091253a04a259";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    // One millon 'a'
    name_of_test    = "One millon 'a'";
    data            = malloc(1000000 * sizeof(char)); memset(data, 'a', 1000000);
    known_digest    = "34aa973cd4c4daa4f61eeb2bdbad27316534016f";
    test_sha1(data, 1000000, known_digest, name_of_test);
    free(data);

    // Wikipedia test #1
    name_of_test    = "Wikipedia test #1";
    data            = "The quick brown fox jumps over the lazy dog";
    known_digest    = "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    // Wikipedia test #2
    name_of_test    = "Wikipedia test #2";
    data            = "The quick brown fox jumps over the lazy cog";
    known_digest    = "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3";
    test_sha1(data, strlen(data), known_digest, name_of_test);

    printf("All tests passed!\n");

    return 0;
}
