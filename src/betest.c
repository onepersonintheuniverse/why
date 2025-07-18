#include <stdio.h>

int main() {
    int x = 23;
    char q = *(char *)(&x);
    if (q == 23) printf("Little endian\n");
    else printf("Big endian\n");
    return 0;
}

