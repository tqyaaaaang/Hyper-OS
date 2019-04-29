/* physical memory manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 */

#include <iostream>
#include <list>
#include <cassert>
#include "pmm.h"
#include "page_dir.h"
#include "pmem.h"
#include "../logging/logging.h"
#include "../env/env.h"
#include "../tools/bus.h"

using std::cout;
using std::endl;
using std::list;
using logging::debug;
using logging::log_endl;

page_frame *pages;

/* alloce n page frames
 * return page_frame pointer of first page frame(s)
 * @n : number of pages 
 */
page_frame* alloc_pages(int n)
{
	return NULL;
}
 
inline page_frame* alloc_page()
{ return alloc_pages(1); }

/* free allocted page frames
 * @pg : first page_frame of continuous pages
 */
void free_pages(page_frame *pg)
{
	
}

inline void free_page(page_frame *pg)
{ free_pages(pg); }

/* init physical memory and manager */
void init_pmm()
{
	logging::info << "pmm init." << logging::log_endl;
	init_pm();
}

/* shutdown physical memory and manager */
void destroy_pmm()
{
	destroy_pm();
}

void debug_pmm()
{
	write(10, 1);
	assert(read(10) == 1);
	write(1 << 20, 127);
	write(1 << 29 | 1, 2);
	assert(read(1 << 20) == 127);
	assert(read(1 << 29 | 1) == 2);
	write(10, 0);
	write(1 << 20, 0);
	write(1 << 29 | 1, 0);
	debug << "pmm check ok." << log_endl;
}
