#define PROCS_MAX 8

#define PROC_UNUSED		0x0
#define PROC_RUNNABLE 	0x1

struct process {
	int pid;
	int state;
	vaddr_t sp;
	uint32_t *page_table;
	uint8_t stack[8192];
};

void switch_context(uint32_t *prev_sp, uint32_t *next_sp);
struct process *create_process(uint32_t pc);
void create_idle_process(void);
void yield(void);
