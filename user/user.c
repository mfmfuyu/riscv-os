#include "user.h"
#include "unistd.h"
#include "stdio.h"

extern char __stack_top[];

int syscall(int sysno, int arg0, int arg1, int arg2)
{
	register int a0 __asm__("a0") = arg0;
	register int a1 __asm__("a1") = arg1;
	register int a2 __asm__("a2") = arg2;
	register int a3 __asm__("a3") = sysno;

	__asm__ __volatile__("ecall"
					: "=r"(a0)
					: "r"(a0), "r"(a1), "r"(a2), "r"(a3)
					: "memory");
}

void write(int fd, const void *buf, int count)
{
	syscall(SYS_write, 0, 0, 0);
}

void printf(const char *fmt, ...)
{
	char buf[64];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	syscall(SYS_write, 0, (int) buf, 0);
}

__attribute__((noreturn))
void exit(void)
{
	for(;;);
}

__attribute__((section(".text.start")))
__attribute__((naked))
void start(void)
{
	__asm__ __volatile__(
		"mv sp, %[stack_top]\n"
		"call main\n"
		"call exit\n" ::[stack_top] "r"(__stack_top));
}
