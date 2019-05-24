/**
 * context/context.h
 * running context of cpu
 */
#pragma once
#include "../mm/page_table.h"

class context_t {
public:
		
	void set_page_table( page_table *pg );
	page_table * get_page_table() const;

	void free_all_pages();
	size_t esp;
	size_t brk;
	
private:
    page_table *pg;
};
