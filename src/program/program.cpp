/**
 * program/program.cpp
 * abstract base class of all user-mode programs
 */
#include <cstdlib>
#include <cassert>
#include "program.h"
#include "../mm/pmem_info.h"
#include "../env/env.h"
#include "../utils/panic.h"

program::program()
{
	data = nullptr;
	this->static_init(); // init static info
	compile();           // simulate compile
}

program::~program()
{
	assert(data != nullptr);
	free(data);
}

size_t program::get_text_size() const
{
	return this->text_size;
}

size_t program::get_data_size() const
{
	return this->data_size;
}

size_t program::get_bss_size() const
{
	return this->bss_size;
}

size_t program::get_stack_size() const
{
	return this->stack_size;
}

void program::run()
{
	this->running = true;
}

bool program::is_running() const
{
	return this->running;
}

void program::compile()
{
	long long tot_static = this->text_size
	    + this->bss_size
		+ this->data_size;
	if (tot_static >= VM_SIZE)
		panic("compile failed.");
	this->stack_size = VM_SIZE - tot_static;
}

void program::static_init()
{
	
}
