#include "user.h"

void delay(void) {
    for (int i = 0; i < 30000000; i++)
        __asm__ __volatile__("nop");
}

void main(void)
{
    while (1) {
        putchar('B');
        delay();
    }
	for(;;);
}
