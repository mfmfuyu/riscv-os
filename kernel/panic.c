#include "common.h"
#include "stdio.h"

void panic(const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);

	char buf[256];
	vsprintf(buf, fmt, vargs);
	va_end(vargs);

	printf("Kernel panic: %s\n", buf);
	for(;;);
}
