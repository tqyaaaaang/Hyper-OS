/**
 * process/process_load.cpp
 * init runtime environment
 */

#include "process_t.h"
#include "../mm/pmm.h"
#include "../mm/page_table.h"
#include "../context/context.h"
#include <cassert>
#include "../mm/pmem.h"

/**
 * alloc page table for @this
 */
void process_t::init_context()
{
	context.set_page_table(new page_table);
}

/**
 * alloc pages for .data and copy
 */
void process_t::init_data()
{
	assert(data_size % PAGE_SIZE == 0);
	assert(prog != nullptr);
	size_t page_number = data_size / PAGE_SIZE;
	page_table *pt = context.get_page_table();
	assert(pt != nullptr);
	char *dt = prog->data;
	char *ed = prog->data + prog->data_size;
	for (size_t i = 0; i < page_number; i++) {
		pte_t pte;
		page_frame *pf = alloc_page();
		assert(pf != nullptr);
		pte.present = true;
		pte.access = pte.dirty = false;
		pte.write = pte.user = true;
		pte.paddr = pt->paddr;
		pf->ref();
		for (size_t j = 0; j < PAGE_SIZE && dt < ed; j++) {
			pm::write(pf->paddr + j, *dt);
			dt++;
		}
	}
}

/**
 * alloc pages for .bss and clear
 */
void process_t::init_bss()
{
	assert(bss_size % PAGE_SIZE == 0);
	assert(prog != nullptr);
	size_t page_number = bss_size / PAGE_SIZE;
	page_table *pt = context.get_page_table();
	assert(pt != nullptr);
	for (size_t i = 0; i < page_number; i++) {
		pte_t pte;
		page_frame *pf = alloc_page();
		assert(pf != nullptr);
		pte.present = true;
		pte.access = pte.dirty = false;
		pte.write = pte.user = true;
		pte.paddr = pt->paddr;
		pf->ref();
		for (size_t j = 0; j < PAGE_SIZE; j++) {
			pm::write(pf->paddr + j, 0);
		}
	}
}

/**
 * init dynamic memory manager
 */
void process_t::init_dmm()
{
	
}

