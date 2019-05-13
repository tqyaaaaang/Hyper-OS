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
#include "../logging/logging.h"

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
	size_t data_size = prog->get_data_size();
	assert(data_size % PAGE_SIZE == 0);
	assert(prog != nullptr);
	size_t page_number = data_size / PAGE_SIZE;
	page_table *pt = context.get_page_table();
	assert(pt != nullptr);
	size_t id = 0;
	for (size_t i = 0; i < page_number; i++) {
		pte_t pte;
		page_frame *pf = alloc_page();
		assert(pf != nullptr);
		pte.present = true;
		pte.access = pte.dirty = false;
		pte.write = pte.user = true;
		pte.paddr = pf->paddr;
		pt->set_pte(i, pte);
		pf->ref();
		for (size_t j = 0; j < PAGE_SIZE && id < data_size; j++) {
			pm::write(pf->paddr + j, prog->get_data(id));
			id++;
		}
	}
}

/**
 * alloc pages for .bss and clear
 */
void process_t::init_bss()
{
	size_t bss_size = prog->get_bss_size();
	size_t start = prog->get_data_size() / PAGE_SIZE;
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
		pte.paddr = pf->paddr;
		pt->set_pte(start + i, pte);
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

