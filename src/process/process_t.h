/**
 * process/process_t.h
 * basic information of process_t 
 */

#pragma once
#include <cstdint>
#include <cstddef>
#include <list>
#include <set>
#include <string>
#include <mutex>
#include <condition_variable>
#include <future>
#include <thread>
#include "../context/context.h"
#include "../program/program.h"
#include "../utils/allocator/allocator.h"

class CPU_core;

class process_t {
public:

	enum state {
		UNINIT,
		SLEEPING,
		RUNABLE,
		ZOMBIE
	};
	
	process_t();
	~process_t();

	// set/get process id
	void set_pid(int pid);
	int get_pid() const;

	// set/get process state
	state get_state() const;
	void set_state(state stat);

	// set/get process name
	void set_name(const std::string &name);
	std::string get_name() const;

	// set/get program of process
	void set_prog(program *prog);
	program* get_prog() const;

	// exec current process, return promise when start
	void exec(std::promise<int> &fin_code);

	// set/get context of process (page table, ...)
	context_t get_context() const;
	void set_context(const context_t &context);

	// set/get parent and children of process
	void add_chl(int pid);
	void set_par(int pid);
	bool is_chl(int pid) const;
	int get_par() const;

	// set time slice, or minus 1 (tick)
	bool tick();
	void set_slice(size_t slice);

	// set/get core
	void set_core(CPU_core *core);
	CPU_core* get_core() const;

	// condition variable to break control flow
	std::condition_variable cond_var;
	std::mutex cond_mutex;

	// linker to state list
	std::list<process_t*>::iterator linker;

	// init
	void init_context();
	void init_data();
	void init_bss();
	void init_dmm();

	// set/get need_resched	
	void set_resched(bool resch);
	bool get_resched() const;

	// vm read/write opertions
	void vm_read(char *buf, size_t la_begin, size_t la_end);
	void vm_write(size_t addr, const char *buf_begin, const char *buf_end);	

	// heap & stack operations	
	size_t heap_malloc(size_t len);
	void heap_free(size_t ptr);
	size_t stack_push(size_t size);
	size_t stack_pop(size_t size);

	// clean process when exit
	void clean();

	// thread of current process
	std::thread *th;

	// set/get exit flag
	void set_exit_flag();
	bool get_exit_flag() const;

	// set/get signal data (for wait/signal)
	void set_signal_data(size_t data);
	size_t get_signal_data() const;
	
private:

	int pid;
	std::string name;
	program *prog;
	state pstat;
	context_t context;

	bool need_resched;
	size_t slice;
	
	size_t ptr_par;
	std::set<size_t> ptr_chl;

	CPU_core *core;
	allocator *heap_allocator;

	bool exit_flag;
	size_t signal_data;
};
