/**
 * program/program.cpp
 * abstract base class of all user-mode programs
 */
#include <cstdlib>
#include <cassert>
#include "program.h"
#include "../mm/pmem_info.h"
#include "../mm/pmem.h"
#include "../env/env.h"
#include "../utils/panic.h"
#include "../utils/check.h"

using handle_type::type;

template class handle<int>;
template class handle<char>;

template<typename T>
handle<T>::handle()
{
	prog = nullptr;
	addr = 0;
}

template<typename T>
handle<T>::handle(program *prog)
{
	this->prog = prog;
}

template<typename T>
handle<T>::handle(size_t addr, program *prog, type t)
{
	this->prog = prog;
	this->addr = addr;
	this->this_type = t;
}

/**
 * STACK Variable : pop stack
 */
template<typename T>
handle<T>::~handle()
{
	if (prog == nullptr)
		return;
	/* if (this_type == type::STACK) {
		prog->stack_pop(sizeof(T));
		}*/
}

/**
 * set handle to val
 * copy memory of (T)val to this->addr
 * only in running time
 */
template<typename T>
handle<T>& handle<T>::operator = (const handle<T> &val)
{
	assert(prog->is_running());
	for (size_t i = 0; i < sizeof(T); i++)
		prog->prog_write(addr + i,
						 prog->prog_read(val.get_addr() + i));
	return *this;
}

/**
 * set handle to val
 * only in running time
 */
template<typename T>
handle<T>& handle<T>::operator = (const T &val)
{
	assert(prog->is_running());
	const char* buf = (const char*)(&val);
	for (size_t i = 0; i < sizeof(T); i++)
		prog->prog_write(addr + i, buf[i]);
	return *this;
}

/**
 * get T from handle
 * only in running time
 */
template<typename T>
handle<T>::operator T() const
{
	assert(prog->is_running());
	char *buf = new char[sizeof(T)];
	for (size_t i = 0; i < sizeof(T); i++)
		buf[i] = prog->prog_read(addr + i);
	delete[] buf;
	T *buf_T = (T*) buf;
	return *buf;
}

/**
 * get handle addr + sizeof(T) * id
 * for array access
 * can be used in compile & runnning time.
 */
template<typename T>
handle<T> handle<T>::operator [] (size_t id)
{
	return handle<T>(addr + sizeof(T) * id, prog, this_type);
}

template<typename T>
size_t handle<T>::get_addr() const
{
	return this->addr;
}

template<typename T>
program* handle<T>::get_prog() const
{
	return this->prog;
}

template<typename T>
size_t* handle<T>::get_addr_addr()
{
	return &addr;
}

program::program()
{
	compiling = true;
	data = nullptr;
	name = "<default>";
	text_size = data_size = bss_size = 0;
	running = false;
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

char program::get_data(size_t id) const
{
	return data[id];
}

void program::run()
{
	this->running = true;
}

bool program::is_running() const
{
	return running;
}

bool program::is_compiling() const
{
	return compiling;
}

static void round(size_t &size)
{
	size = (size / PAGE_SIZE
			+ (size % PAGE_SIZE != 0)) * PAGE_SIZE;
}

void program::compile()
{
	size_t tot_static = 0;
	round(text_size);
	round(bss_size);
	round(data_size);
	if (!add_check(text_size, bss_size)) {
		panic("compile failed : MLE");
	}
	tot_static = text_size + bss_size;
	if (!add_check(tot_static, data_size)) {
		panic("compile failed : MLE");
	}
	tot_static += data_size;
	if (tot_static >= VM_SIZE)
		panic("compile failed : MLE");
	this->stack_size = VM_SIZE - tot_static;
	do_redirect();
	compiling = false;
}

/**
 * do redirect for handle in .bss
 */
void program::do_redirect()
{
	for (auto addr : redr_table) {
		*addr += data_size;
	}
	redr_table.clear();
}

void program::static_init()
{
	
}

char program::prog_read(size_t addr)
{
	return pm::read(addr);
}

void program::prog_write(size_t addr, char data)
{
	return pm::write(addr, data);
}
