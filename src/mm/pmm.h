#pragma once
#include "pmem_info.h"
#include "../env/env.h"
#include "page_table.h"
#include <cstdint>
#include <list>
#include <cstddef>
#include <list>

class page_frame {
public:
		
	size_t paddr;    // start point of page
	int length;      // length of the page block
	bool alloced;    // if alloced
	page_frame();

	void ref();      // refer counter ++ 
	void free();     // refer counter --
	bool die();      // refer counter == 0

	std::list<pte_t*> link;
	// pte point to current page frame 
private:
	
	int refer;         // ref counter
} ;

extern page_frame *pages;

// init function
void init_pmm();
void destroy_pmm();

// interface for OS

/**
 * swap the page @pg out
 */
void swap_out(page_frame* pg);
void swap_out_nlock(page_frame* pg);

/**
 * swap the page @pg into memory
 */
void swap_in(page_frame *pg);

page_frame* alloc_pages(int n);
page_frame* alloc_page();
page_frame* alloc_page_nlock();
page_frame* alloc_pages_nlock(int n);

void free_pages(page_frame *pg);
void free_page(page_frame *pg);

int page2id(page_frame *pg);
page_frame* id2page(int id);
page_frame* pa2page(size_t pa);

void pmm_require_lock();
void pmm_release_lock();

void pte_link_pf(pte_t *pte, page_frame *pg);
