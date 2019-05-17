/*
 * physical memory manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 */

#include <iostream>
#include <list>
#include <cassert>
#include <mutex>
#include "pmm.h"
#include "page_table.h"
#include "pmem.h"
#include "../logging/logging.h"
#include "../env/env.h"
#include "../utils/bus/bus.h"
#include "../utils/allocator/allocator.h"
#include "../utils/allocator/ffma.h"

using std::list;
using logging::debug;
using logging::info;
using logging::log_endl;
using std::mutex;
using std::lock_guard;

typedef allocator::segment segment;

page_frame::page_frame()
{
	this->refer = 0;
	this->alloced = false;
}

void page_frame::ref()
{
	refer++;
}

void page_frame::free()
{
	assert(refer > 0);
	refer--;
}

bool page_frame::die()
{
	return refer == 0;
}

// page frames
page_frame *pages;

// page allocator 
static allocator *alloc;

// mutex for page allocation
static mutex alloc_mutex;

/**
 * get id from page_frame*
 */
int page2id(page_frame *pg)
{
	assert(pg >= pages && static_cast < size_t > ( pg - pages ) < PAGE_NUM);
	return pg - pages;
}

/**
 * get page_frame* from id
 */
page_frame* id2page(int id)
{
	assert(id >= 0 && static_cast < size_t > ( id ) < PAGE_NUM);
	return pages + id;
}

page_frame* pa2page(size_t pa)
{
	return id2page(pa / PAGE_SIZE);
}

/**
 * alloc n page frames
 * return page_frame pointer of first page frame(s)
 * @n : number of pages
 * TODO : fail handle
 */
page_frame* alloc_pages_nlock(int n)
{
	size_t start = alloc->malloc(n);
	start = alloc->malloc(n);
	if (start == (size_t)(-1)) {
		return nullptr;
	}
	page_frame *ret = pages + start;
	ret->alloced = true;
	ret->length = n;
	return ret;
}

page_frame* alloc_pages(int n)
{
	lock_guard<mutex> locker(alloc_mutex);
	return alloc_pages_nlock(n);
}

page_frame* alloc_page()
{ return alloc_pages(1); }

page_frame* alloc_page_nlock()
{ return alloc_pages_nlock(1); }

/* free allocted page frames
 * @pg : first page_frame of continuous pages
 * TODO : fail handle
 */
void free_pages(page_frame *pg)
{
	lock_guard<mutex> locker(alloc_mutex);
	assert(pg->alloced);
	assert(!pg->die());
	pg->free();
	if (pg->die()) { 
		alloc->free(page2id(pg), pg->length);
		pg->alloced = false;
		pg->length = 0;
	}
}

void free_page(page_frame *pg)
{ free_pages(pg); }

/* init physical memory and manager */
void init_pmm()
{
	info << "pmm init." << log_endl;
	init_pm();
	pages = new page_frame[PAGE_NUM];
	alloc = new ffma(PAGE_NUM);
	for (size_t i = 0; i < PAGE_NUM; i++)
		pages[i].paddr = i * PAGE_SIZE;
}

/* shutdown physical memory and manager */
void destroy_pmm()
{
	destroy_pm();
	delete alloc;
	delete[] pages;
}

void pmm_require_lock()
{
	alloc_mutex.lock();
}

void pmm_release_lock()
{
	alloc_mutex.unlock();
}

void swap_out_nlock(page_frame *pg)
{
	
}

void swap_out(page_frame *pg)
{
	lock_guard<mutex> lk(alloc_mutex);
	swap_out_nlock(pg);
}

void swap_in_nlock(pte_t *pte page_frame *pg)
{
	
}

void swap_in(pte_t *pte, page_frame *pg)
{
	lock_guard<mutex> lk(alloc_mutex);
	swap_in_nlock(pte, pg);
}

void pte_link_pf(pte_t *pte, page_frame *pf)
{
	pf->link.push_front(pte);
	pte->linker = pf->link.begin();
}
