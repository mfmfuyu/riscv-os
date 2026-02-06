#include "printk.h"
#include "stdio.h"
#include "sbi.h"

void printk(const char *fmt, ...)
{
	char buf[256];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	char *p = buf;
	while (*p) {
		sbi_putchar(*p);
		p++;
	}
}
