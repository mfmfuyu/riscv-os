#include "common.h"
#include "mm.h"
#include "string.h"
#include "panic.h"

extern char __kernel_base[];
extern char __free_ram[], __free_ram_end[];

paddr_t alloc_pages(uint32_t n) {
	static paddr_t next_paddr = (paddr_t) __free_ram;
	paddr_t paddr = next_paddr;
	next_paddr += n * PAGE_SIZE;

	if (next_paddr > (paddr_t) __free_ram_end)
			panic("mm: out of memory");

	memset((void *) paddr, 0, n * PAGE_SIZE);
	return paddr;
}

void map_page(uint32_t *table1, uint32_t vaddr, paddr_t paddr, uint32_t flags)
{
	if (!is_aligned(vaddr, PAGE_SIZE))
			panic("mm: unaligned vaddr %x", vaddr);

	if (!is_aligned(paddr, PAGE_SIZE))
			panic("mm: unaligned paddr %x", paddr);

	uint32_t vpn1 = (vaddr >> 22) & 0x3ff;
	if ((table1[vpn1] & PAGE_V) == 0)
	{
			uint32_t pt_paddr = alloc_pages(1);
			table1[vpn1] = ((pt_paddr / PAGE_SIZE) << 10) | PAGE_V;
	}

	uint32_t vpn0 = (vaddr >> 12) & 0x3ff;
	uint32_t *table0 = (uint32_t *) ((table1[vpn1] >> 10) * PAGE_SIZE);
	table0[vpn0] = ((paddr / PAGE_SIZE) << 10) | flags | PAGE_V;
}

void map_kernel_pages(uint32_t *page_table)
{
	for (paddr_t paddr = (paddr_t) __kernel_base;
					paddr < (paddr_t) __free_ram_end; paddr += PAGE_SIZE)
			map_page(page_table, paddr, paddr, PAGE_R | PAGE_W | PAGE_X);
}
