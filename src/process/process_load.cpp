/**
 * process/process_load.cpp
 * init runtime environment
 */

#include "process_t.h"
#include "../mm/pmm.h"
#include "../mm/page_table.h"
#include "../context/context.h"

void process_t::init_context()
{
	context.set_page_table(new page_table);
}

void process_t::init_data()
{
	
}

void process_t::init_bss()
{
	
}

void process_t::init_dmm()
{
	
}

