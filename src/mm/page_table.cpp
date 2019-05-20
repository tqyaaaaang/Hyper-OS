/*
 * Data Structure For Page Table
 */

#include "page_table.h"
#include "../logging/logging.h"
#include "../utils/panic.h"
#include "../utils/pagerepl/fifo.h"
#include "pmm.h"
#include <cassert>

pte_t::pte_t()
{
	present = 0;
	access = 0;
	dirty = 0;
	write = user = 0;
	paddr = 0;
}

page_table::page_table()
{
	table = new pte_t[VPAGE_NUM];
	page_rp = new fifo_repl(PAGE_NUM);
}

page_table::~page_table()
{
	assert(table != nullptr);
	delete[] table;
	delete page_rp;
}

pte_t* page_table::get_pte(size_t la)
{
	size_t id = la / PAGE_SIZE;
	assert(id < VPAGE_NUM);
	if (table[id].present) {
		return table + id;
	} else {
		size_t pg;
		page_frame* page;
		pmm_require_lock();
		{
			if (page_rp->full()) {
				pg = page_rp->swap_out();
				swap_out_nlock(pte2page(table + pg));
				page = alloc_page_nlock();
			} else {
				page = alloc_page_nlock();
				pg = page2id(page);
			}
		}
		pmm_release_lock();
		assert(page != nullptr);
		page_rp->swap_in(pg);
		page->ref();
		table[id].present = 1;
		table[id].user = 1;
		if (table[id].paddr != 0) {
			swap_in(table + pg, page);
			table[id].paddr = page->paddr;
		} else {
			table[id].paddr = page->paddr;
			logging::info << "SET TABLE : id " << id << ", paddr = " << (page-pages)*PAGE_SIZE << "," << page->paddr << logging::log_endl;
		}
		return table + id;
	}
}

pte_t* page_table::get_pte_try(size_t la)
{
	size_t id = la / PAGE_SIZE;
	assert(id < VPAGE_NUM);
	return table + id;
}

void page_table::free_pte(size_t id)
{
	assert(id < VPAGE_NUM);
	pte_t *page = table + id;
	if (page->present) {
		free_page(id2page(page->paddr/PAGE_SIZE));
	} else if (page->paddr != 0) {
		// free page in swap
	}
}

void page_table::set_pte(size_t id, const pte_t &pte)
{
	assert(id < VPAGE_NUM);
	table[id] = pte;
}

page_frame* pte2page(pte_t *pte)
{
	assert(pte->present);
	return pa2page(pte->paddr);
}
