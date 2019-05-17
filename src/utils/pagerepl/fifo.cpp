/**
 * pagerepl/fifo.h
 * fifo replacement algorithm
 */


#include "fifo.h"
#include <cassert>

fifo_repl::fifo_repl(size_t page_num)
{
	set_page_num(page_num);	
}

fifo_repl::~fifo_repl()
{
	
}

void fifo_repl::page_swap_in(size_t id)
{
	assert(!full());
	que.push(id);
}

size_t fifo_repl::page_swap_out()
{
	assert(!que.empty());
	size_t pg = que.front();
	que.pop();
	return pg;
}

