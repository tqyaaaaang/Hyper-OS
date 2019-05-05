/**
 * context/context.h
 * running context of cpu
 */
#include "context.h"

void context_t::set_page_table(page_table *pg)
{
	this->pg = pg;
}

page_table* context_t::get_page_table() const
{
	return this->pg;
}
