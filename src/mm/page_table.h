/*
 * Data Structures for Page Table
 */

#pragma once
#include <cstdint>
#include <cstddef>
#include <list>
#include "pmem_info.h"
#include "../env/env.h"
#include "../utils/pagerepl/pagerepl.h"

class page_frame;

/* type of page table entry */ 
struct pte_t {
	bool present;        // true if the page is inside memory 
	bool access;         // true if the page is accessed
	bool dirty;          // true if the page is modified
	bool write;          // true if the page is writable
	bool user;           // true if the page can be accessed in user mode
	size_t paddr;        // if (present) : physical address of the page
	                     // else         : physical address in swap
		                 //                if !present && paddr == 0 : uninited
	std::list<pte_t*>::iterator linker;
    pte_t();
};

class page_table {
public:
		
    page_table();
	~page_table();

	pte_t* get_pte(size_t la);     // get pte, if failed, try swap in
	pte_t* get_pte_try(size_t la); // get pte, if failed, return nullptr

	void set_pte(size_t id, const pte_t &pte);
private:

    pte_t *table;
	void free_pte(size_t id);

private:

	page_repl *page_rp;	
};

page_frame* pte2page(pte_t *pte);
