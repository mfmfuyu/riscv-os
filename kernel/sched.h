#include "kernel.h"

#define PROCS_MAX 8

#define PROC_UNUSED		0x0
#define PROC_RUNNABLE 	0x1

struct process {
	int pid;
	int state;
	vaddr_t sp;
	uint32_t *page_table;
	uint8_t stack[8192];
	uint32_t sepc;
};

void switch_context(uint32_t *prev_sp, uint32_t *next_sp);
struct process *create_process(const void *image, size_t image_size);
void create_idle_process(void);
void yield(void);
struct process *current(void);
void sched(void);
