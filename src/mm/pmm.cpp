/* physical memory & pm manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 * read/write    : physical memory interface for mmu
 */

#include <iostream>
#include <list>
#include "pmm.h"
#include "page_dir.h"

using std::cout;
using std::endl;
using std::list;

list<page_frame*> free_list;
page_frame pages[PAGE_NUM];

/* alloce n page frames
 * return page_frame pointer of first page frame(s)
 * @n : number of pages 
 */
page_frame* alloc_pages(int n)
{
	
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
void pm_init()
{
	cout << "pm init." << endl;
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
	
}

/* interface for mmu, write a byte
 * @paddr : physical address to write
 * @data  : data to write
 */
void write(size_t paddr, char data)
{
	
}
