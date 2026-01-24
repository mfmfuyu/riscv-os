#include "kernel.h"
#include "sched.h"
#include "string.h"
#include "mm.h"
#include "riscv.h"
#include "panic.h"

struct process procs[PROCS_MAX];

struct process *current_proc;
struct process *idle_proc;

__attribute__((naked))
void switch_context(uint32_t *prev_sp, uint32_t *next_sp)
{
	__asm__ __volatile__(
		"addi sp, sp, -13 * 4\n"
		"sw ra,  0	* 4(sp)\n"
		"sw s0,  1	* 4(sp)\n"
		"sw s1,  2	* 4(sp)\n"
		"sw s2,  3	* 4(sp)\n"
		"sw s3,  4	* 4(sp)\n"
		"sw s4,  5	* 4(sp)\n"
		"sw s5,  6	* 4(sp)\n"
		"sw s6,  7	* 4(sp)\n"
		"sw s7,  8	* 4(sp)\n"
		"sw s8,  9	* 4(sp)\n"
		"sw s9,  10 * 4(sp)\n"
		"sw s10, 11 * 4(sp)\n"
		"sw s11, 12 * 4(sp)\n"

		"sw sp, (a0)\n"
		"lw sp, (a1)\n"

		"lw ra,  0	* 4(sp)\n"
		"lw s0,  1	* 4(sp)\n"
		"lw s1,  2	* 4(sp)\n"
		"lw s2,  3	* 4(sp)\n"
		"lw s3,  4	* 4(sp)\n"
		"lw s4,  5	* 4(sp)\n"
		"lw s5,  6	* 4(sp)\n"
		"lw s6,  7	* 4(sp)\n"
		"lw s7,  8	* 4(sp)\n"
		"lw s8,  9	* 4(sp)\n"
		"lw s9,  10 * 4(sp)\n"
		"lw s10, 11 * 4(sp)\n"
		"lw s11, 12 * 4(sp)\n"
		"addi sp, sp, 13 * 4\n"
		"ret\n"
	);
}

__attribute__((naked))
void user_entry(void)
{
		__asm__ __volatile__(
						"csrw sepc, %[sepc]\n"

						"li   t0, 0\n"
						"ori  t0, t0, %[spie]\n"
						"csrw sstatus, t0\n"

						"csrw sie, %[sie]\n"
						"sret\n"
						:
						: [sepc] "r" (USER_BASE),
						  [spie] "i" (SSTATUS_SPIE),
						  [sie] "r" (SIE_STIE));
}

struct process *create_process(const void *image, size_t image_size)
{
		struct process *proc = NULL;
		int i;
		for (i = 0; i < PROCS_MAX; i++) {
			if (procs[i].state == PROC_UNUSED) {
				proc = &procs[i];
				break;
			}
		}

		if (!proc)
				panic("sched: no free process slots");

		uint32_t *sp = (uint32_t *) &proc->stack[sizeof(proc->stack)];
		*--sp = 0;				// s11
		*--sp = 0;				// s10
		*--sp = 0;				// s9
		*--sp = 0;				// s8
		*--sp = 0;				// s7
		*--sp = 0;				// s6
		*--sp = 0;				// s5
		*--sp = 0;				// s4
		*--sp = 0;				// s3
		*--sp = 0;				// s2
		*--sp = 0;				// s1
		*--sp = 0;				// s0
		*--sp = (uint32_t) user_entry;	// ra

		uint32_t *page_table = (uint32_t *) alloc_pages(1);
		// Map kernel pages.
		map_kernel_pages(page_table);
		// Map user pages.
		map_user_pages(page_table, image, image_size);

		proc->pid = i + 1;
		proc->state = PROC_RUNNABLE;
		proc->sp = (uint32_t) sp;
		proc->page_table = page_table;
		proc->sepc = USER_BASE;
		return proc;
}

void sched_init(void)
{
	idle_proc = create_process(NULL, 0);
	idle_proc->pid = 0; // idle
	current_proc = idle_proc;
}

void yield(void)
{
	struct process *next = idle_proc;
	for (int i = 0; i < PROCS_MAX; i++) {
		struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
		if (proc->state == PROC_RUNNABLE && proc->pid > 0) {
			next = proc;
			break;
		}
	}

	if (next == current_proc)
			return;

	struct process *prev = current_proc;
	current_proc = next;

	__asm__ __volatile__(
					"sfence.vma\n"
					"csrw satp, %[satp]\n"
					"sfence.vma\n"
					"csrw sscratch, %[sscratch]\n"
					:
					:	[satp] "r" (SATP_SV32 | ((uint32_t) next->page_table / PAGE_SIZE)),
						[sscratch] "r" ((uint32_t) &next->stack[sizeof(next->stack)]));

	switch_context(&prev->sp, &next->sp);
}

struct process *current(void)
{
	return current_proc;
}

void sched(void)
{
	struct process *next = idle_proc;
	for (int i = 0; i < PROCS_MAX; i++) {
		struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
		if (proc->state == PROC_RUNNABLE && proc->pid > 0) {
			next = proc;
			break;
		}
	}

	struct process *prev = current_proc;
	current_proc = next;

	__asm__ __volatile__(
					"sfence.vma\n"
					"csrw satp, %[satp]\n"
					"sfence.vma\n"
					"csrw sscratch, %[sscratch]\n"
					:
					:	[satp] "r" (SATP_SV32 | ((uint32_t) next->page_table / PAGE_SIZE)),
						[sscratch] "r" ((uint32_t) &next->stack[sizeof(next->stack)]));
}
