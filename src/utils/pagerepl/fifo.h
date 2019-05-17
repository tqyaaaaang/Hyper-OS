/**
 * pagerepl/fifo.h
 * fifo replacement algorithm
 */
#pragma once
#include "pagerepl.h"
#include <queue>

class fifo_repl : public page_repl {

public:

	fifo_repl(size_t page_num);
	virtual ~fifo_repl();
	virtual void page_swap_in(size_t id);
	virtual size_t page_swap_out();

private:
	
	std::queue<size_t> que;
};

