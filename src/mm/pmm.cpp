/* physical memory manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 */

#include <iostream>
#include <list>
#include "pmm.h"
#include "page_dir.h"
#include "pmem.h"
#include "../logging/logging.h"
#include "../env/env.h"
#include "../tools/bus.h"

using std::cout;
using std::endl;
using std::list;

list<page_frame*> free_list;
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
	logging::info << "--- pmm debugging ---"  << logging::log_endl;
	write(10, 1);
	logging::info << "read 10 : " << read(10) << logging::log_endl;
	write(1 << 20, 2);
	write(1 << 29, 3);
	logging::info << "read 1 << 20 : " << read(1 << 20) << logging::log_endl;
	logging::info << "read 1 << 29 : " << read(1 << 29) << logging::log_endl;
}
