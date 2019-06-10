/**
 * program/program.cpp
 * abstract base class of all user-mode programs
 */
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <string>
#include <mutex>
#include <exception>
#include "program.h"
#include "../core/core.h"
#include "../process/process_t.h"
#include "../process/process.h"
#include "../status/status.h"
#include "../mm/pmem_info.h"
#include "../mm/pmem.h"
#include "../env/env.h"
#include "../utils/panic.h"
#include "../utils/check.h"
#include "../logging/logging.h"
#include "../utils/allocator/ffma.h"
#include "../interrupt/interrupt.h"
#include "../message/message.h"
#include "sys_t.h"
#include "lib.h"

using std::mutex;
using std::unique_lock;
using handle_type::type;
using std::string;
using logging::debug;
using logging::log_endl;
using logging::info;
using std::terminate;
using std::lock_guard;

template class handle<int>;
template class handle<char>;
template class handle<long long>;
template class handle<size_t>;
template class handle<double>;
template class handle< handle<int> >;

static void msg_mm(string str)
{
	message::memory(message::wrap_core_info("user program"))
		<< str
		<< message::msg_endl;
}

/**
 * tail check
 * if an interrupt occurs, trap into kernel mode
 */

void tail_check(program *prog)
{
	process_t *proc = prog->cur_proc;
	assert(proc != nullptr);
	assert(proc->get_core() == status.get_core());
	check_interrupt ();
}

template<typename T>
handle<T>::handle()
{
	prog = nullptr;
	addr = 0;
	this_type = handle_type::NIL;
}

template<typename T>
handle<T>::handle(program *prog)
{
	this->prog = prog;
	this_type = handle_type::NIL;
}

template<typename T>
handle<T>::handle(size_t addr, program *prog, type t)
{
	this->prog = prog;
	this->addr = addr;
	this->this_type = t;
}

template<typename T>
handle<T>::handle(const T &val)
{
   	process_t *proc = status.get_core()->get_current();
	alias(proc->get_prog()->alloc_stack<T>());
	*this = val;
}

template<typename T>
handle<T>::handle(const handle<T> &val)
{
	alias(val);
}

/**
 * STACK Variable : pop stack
 */
template<typename T>
handle<T>::~handle()
{
	if (prog == nullptr)
		return;
	if (prog->is_running() && this_type == type::STACK) {
		logging::debug << "handle of program : " << prog->get_name() << "poping" << logging::log_endl;
		prog->stack_pop(sizeof(T));
    }
	
}

/**
 * handle copy
 */
template<typename T>
handle<T>& handle<T>::operator = (const handle<T> &val)
{
	alias(val);
	if (prog != nullptr && prog->is_running()) {
		tail_check(this->prog);
	}
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
	if (this_type == handle_type::STACK) {
		msg_mm("write operation in stack");
	} else if (this_type == handle_type::HEAP) {
		msg_mm("write operation in heap");
	} else if (this_type == handle_type::STATIC) {
		msg_mm("write operation in .data");
	} else if (this_type == handle_type::BSS) {
		msg_mm("write operation in .bss");
	} else {
		msg_mm("write operation in unknown");
	}
	const char* buf = (const char*)(&val);
	prog->prog_write(addr, buf, buf + sizeof(T));
	tail_check(this->prog); 
	return *this;
}

/**
 * get T from handle
 * only in running time
 */
template<typename T>
handle<T>::operator T() const
{
	assert(prog != nullptr);
	if (!prog->is_running()) {
		logging::info << "handle of prog : " << prog->get_name() << " failed" << logging::log_endl;
	}
	assert(prog->is_running());
	if (this_type == handle_type::STACK) {
		msg_mm("read operation in stack");
	} else if (this_type == handle_type::HEAP) {
		msg_mm("read operation in heap");
	} else if (this_type == handle_type::STATIC) {
		msg_mm("read operation in .data");
	} else if (this_type == handle_type::BSS) {
		msg_mm("read operation in .bss");
	} else {
		msg_mm("read operation in unknown");
	}
	T buf;
	prog->prog_read((char*)(&buf), addr, addr + sizeof(T));
	tail_check(this->prog);
	return buf;
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
	sys = new sys_t(this);
	hos_std = new hos_std_t(this);
}

void program::reset_stdlib()
{
	sys = new sys_t(this);
	hos_std = new hos_std_t(this);
}

void program::build()
{
	debug << "BUILDING" << log_endl;
	compiling = true;
	data = nullptr;
	name = "<default>";
	text_size = data_size = bss_size = 0;
	running = false;
	debug << "STATIC INIT" << log_endl;
	this->static_init(); // init static info
	hos_std->init();
	info << "COMPILE" << log_endl;
	compile();           // simulate compile
	info << "BUILDING FINISH" << log_endl;
}

program::~program()
{
	info << "program " << get_name() << " destroy" << log_endl;
    if (data != nullptr) {
		free(data);
		data = nullptr;
	}
	if (sys != nullptr) {
		delete sys;
		sys = nullptr;
	}
	if (hos_std != nullptr) {
		delete hos_std;
		hos_std = nullptr;
	}
	this->running = false;
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

void program::compile()
{
	size_t tot_static = 0;
	round2page(text_size);
	round2page(bss_size);
	round2page(data_size);
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

void program::prog_read(char *buf, size_t la_begin, size_t la_end)
{
	assert(cur_proc != nullptr);
	return cur_proc->vm_read(buf, la_begin, la_end);
}

void program::prog_write(size_t addr, const char *buf_begin, const char *buf_end)
{
	assert(cur_proc != nullptr);
	cur_proc->vm_write(addr, buf_begin, buf_end);
}

void program::set_name(const string &name)
{
	this->name = name;
}

string program::get_name() const
{
	return name;
}
