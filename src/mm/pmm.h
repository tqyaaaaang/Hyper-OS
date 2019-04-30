#pragma once
#include "pmem_info.h"
#include "../env/env.h"
#include "page_table.h"
#include <cstdint>
#include <list>
#include <cstddef>

class page_frame {
public:
		
	size_t paddr;    // start point of page
	int length;      // length of the page block
	bool alloced;    // if alloced
	page_frame();

	void ref();      // refer counter ++ 
	void free();     // refer counter --
	bool die();      // refer counter == 0
	
private:
	
	int refer;         // ref counter
} ;

extern page_frame *pages;

// init function
void init_pmm();
void debug_pmm();
void destroy_pmm();

// interface for OS 
page_frame* alloc_pages(int n);
page_frame* alloc_page();
void free_pages(page_frame *pg);
void free_page(page_frame *pg);
int page2id(page_frame *pg);
page_frame* id2page(int id);
