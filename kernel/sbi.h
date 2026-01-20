#pragma once

#define SBI_CONSOLE_PUT_CHAR 0x01

struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid);
void sbi_putchar(char ch);