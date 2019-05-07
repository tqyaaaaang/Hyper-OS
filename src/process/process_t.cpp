/**
 * process/process_t.cpp
 * basic information of process_t 
 */

#include "process_t.h"
#include "../utils/panic.h"

typedef process_t::state state;

process_t::process_t()
{
	prog = nullptr;
	name = "<default>";
	ptr_fa;
}

process_t::~process_t()
{
	if (prog != nullptr)
		delete prog;
}

void process_t::set_pid(size_t pid)
{
	this->pid = pid;
}

size_t process_t::get_pid() const
{
	return this->pid;
}

state process_t::get_state() const
{
	return pstat;
}

void process_t::set_state(state stat)
{
	pstat = stat;
}

void process_t::set_prog(program *prog)
{
	this->prog = prog;
}

void process_t::exec()
{
	if (prog != nullptr) {
		prog->main();
	}
}

