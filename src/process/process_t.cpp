/**
 * process/process_t.cpp
 * basic information of process_t 
 */

#include "process_t.h"
#include "../utils/panic.h"
#include "../core/core.h"
#include <string>
#include <cassert>
#include <mutex>

typedef process_t::state state;
using std::string;
using std::promise;
using std::unique_lock;
using std::mutex;

process_t::process_t()
{
	prog = nullptr;
	name = "<default>";
	ptr_par = 0;
	heap_allocator = nullptr;
}

process_t::~process_t()
{
	if (prog != nullptr)
		delete prog;
	if (heap_allocator != nullptr)
		delete heap_allocator;
}

void process_t::set_pid(int pid)
{
	this->pid = pid;
}

int process_t::get_pid() const
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
	prog->cur_proc = this;
 
}

program* process_t::get_prog() const
{
	return prog;
}

void process_t::set_name(const string &name)
{
	this->name = name;
}

string process_t::get_name() const
{
	return name;
}

void process_t::exec(promise<int> &fin_code)
{
	assert(prog != nullptr);
    unique_lock<mutex> lk(this->cond_mutex);
	fin_code.set_value(0);
	this->cond_var.wait(lk);
	lk.unlock();
	core->acquire();
	this->prog->main();
	prog->sys->exit();
	core->release();
}

void process_t::add_chl(int pid)
{
	ptr_chl.insert(pid);
}

void process_t::set_par(int pid)
{
	ptr_par = pid;
}

bool process_t::tick()
{
	return (slice--) == 0;
}

void process_t::set_slice(size_t slice)
{
	this->slice = slice;
}

void process_t::set_resched(bool resch)
{
	need_resched = resch;
}

bool process_t::get_resched() const
{
	return need_resched;
}

void process_t::set_core(CPU_core *core)
{
	this->core = core;
}

CPU_core* process_t::get_core() const
{
	return core;
}

context_t process_t::get_context() const
{
	return context;
}

void process_t::set_context(const context_t &context)
{
	this->context = context;
}

void process_t::vm_read(char *buf, size_t la_begin, size_t la_end)
{
	core->vm_read(buf, la_begin, la_end);
}

void process_t::vm_write(size_t addr, const char *buf_begin, const char *buf_end)
{
	core->vm_write(addr, buf_begin, buf_end);
}

