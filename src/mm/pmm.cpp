/* physical memory & pm manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 * read/write    : physical memory interface for mmu
 */

#include <iostream>
#include <list>
#include "pmm.h"
#include "pmem_info.h"
#include "page_dir.h"
#include "../logging/logging.h"

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
	logging::info << "Initialize physical memory manager" << logging::log_endl;
	pages = new page_frame[PAGE_NUM];
}

void destory_pmm ()
{
	logging::debug << "Destory physical memory manager" << logging::log_endl;
	delete[] pages;
}

/* memory thread */
static void memory_thread_main()
{
	
}

/* interface for mmu, read a byte
 * @paddr : physical address to read
 */
size_t read(size_t paddr)
{
	return 0;
}

/* interface for mmu, write a byte
 * @paddr : physical address to write
 * @data  : data to write
 */
void write(size_t paddr, char data)
{
	
}
