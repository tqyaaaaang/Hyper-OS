/*
 * Data Structures for Page Table
 */

#pragma once
#include <cstdint>
#include <cstddef>
#include "pmem_info.h"
#include "../env/env.h"
#include "pmm.h"

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
    pte_t();
};

class page_table {
public:
		
    page_table();
	~page_table();

	pte_t* get_pte(size_t la);
	pte_t* get_pte_try(size_t la);
	
private:

    pte_t *table;
	void free_pte(size_t id);
	
};

