/**
 * utils/pagerepl/pagerepl.cpp
 * page replacement algorithm's interface 
 */

#include "pagerepl.h"
#include <cassert>

page_repl::page_repl()
{
	cur_num = page_num = 0;
}

page_repl::~page_repl()
{
	
}

bool page_repl::full()
{
	return cur_num == page_num;
}

void page_repl::swap_in(size_t id)
{
	assert(!full());
	cur_num++;
	page_swap_in(id);
}

size_t page_repl::swap_out()
{
	size_t return_value = page_swap_out();
	cur_num--;
	return return_value;
}

size_t page_repl::get_page_num() const
{
	return page_num;
}

void page_repl::set_page_num(size_t page_num)
{
	this->page_num = page_num;
}

