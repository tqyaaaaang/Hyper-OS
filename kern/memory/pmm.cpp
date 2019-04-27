/* physical memory manager 
 * alloc_page(s) : alloc new page frame(s)
 * free_page(s)  : free allocted page frame(s)
 * read/write    : physical memory interface
 */

#include <iostream>
#include <list>
#include "pmm.h"
#include "page_dir.h"

using std::cout;
using std::endl;
using std::list;

list<page_frame*> free_list;
page_frame pages[page_num];

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

void pm_init()
{
	cout << "pm init." << endl;
}

size_t read(size_t paddr)
{
	
}

size_t write(size_t paddr)
{
	
}
