#pragma once

#include "kernel.h"

#define SBI_CONSOLE_PUT_CHAR 0x01
#define SBI_SET_TIMER 0x54494D45

struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid);
void sbi_putchar(char ch);
void sbi_set_timer(uint64_t stime_value);
