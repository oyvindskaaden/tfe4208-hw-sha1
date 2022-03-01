#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int test = 7;

    switch (test)
    {
    case 0 ... 5:
        printf("In [0:5]: %d\n", test);
        break;
    case 6 ... 10:
        printf("In [6:10]: %d\n", test);
        break;
    default:
        printf("Bigger than 10 or negative: %d\n", test);
        break;
    }
    return 0;
}
