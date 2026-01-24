#include "kernel.h"

#define SATP_SV32 (1u << 31)

#define PAGE_V	(1 << 0)
#define PAGE_R 	(1 << 1)
#define PAGE_W	(1 << 2)
#define PAGE_X	(1 << 3)
#define PAGE_U	(1 << 4)

paddr_t alloc_pages(uint32_t n);
void map_page(uint32_t *table1, uint32_t vaddr, paddr_t paddr, uint32_t flags);
void map_kernel_pages(uint32_t *page_table);
void map_user_pages(uint32_t *page_table, const void *image, size_t image_size);
