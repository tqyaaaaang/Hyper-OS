#pragma once
#include "defs.h"
#include "page_dir.h"
#include <cstdint>
#include <list>

using std::list;

class page_frame {
public:
	uintptr_t paddr; // start point of page
	int length;      // length of the page block
	bool alloced;    // if alloced
} pages[page_num];

extern list<page_frame> free_list;

void pm_init();

page_frame* alloc_pages(int n);
page_frame* alloc_page();
void free_pages(page_frame *pg, int n);
void free_page(page_frame *pg);
