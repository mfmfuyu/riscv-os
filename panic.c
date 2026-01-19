#include "string.h"

void panic(char *s)
{
    do {
        printf("Kernel panic: %s\n", s);
        while (1);
    } while (0);
}