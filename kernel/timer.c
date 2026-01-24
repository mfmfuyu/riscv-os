#include "common.h"
#include "kernel.h"
#include "timer.h"
#include "riscv.h"
#include "sbi.h"

uint64_t rdtime()
{
   	uint32_t hi, lo, hi2;
	do {
		__asm__ __volatile__ ("rdtimeh %0" : "=r" (hi));
		__asm__ __volatile__ ("rdtime %0" : "=r" (lo));
		__asm__ __volatile__ ("rdtimeh %0" : "=r" (hi2));
	} while (hi != hi2);

	return (((uint64_t) hi) << 32) | lo;
}

void timer_init()
{
	WRITE_CSR(sstatus, READ_CSR(sstatus) | SSTATUS_SIE);
	WRITE_CSR(sie, READ_CSR(sie) | SIE_STIE);
}

void timer_next()
{
	sbi_set_timer(rdtime() + 1000000);
}
