#pragma once
#include "pmem_info.h"
#include "../env/env.h"
#include "page_dir.h"
#include <cstdint>
#include <list>
#include <cstddef>

struct page_frame {
	size_t paddr;    // start point of page
	int length;      // length of the page block
	bool alloced;    // if alloced
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
