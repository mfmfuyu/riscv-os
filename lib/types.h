#pragma once

typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

#define true 1
#define false 0
#define NULL ((void *) 0)

#define va_list		__builtin_va_list
#define va_start	__builtin_va_start
#define va_end		__builtin_va_end
#define va_arg		__builtin_va_arg
