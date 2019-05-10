/**
 * process/process_t.cpp
 * basic information of process_t 
 */

#include "process_t.h"
#include "../utils/panic.h"
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
	this->prog->main();
}

void process_t::add_chl(size_t pid)
{
	ptr_chl.insert(pid);
}

void process_t::set_par(size_t pid)
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
