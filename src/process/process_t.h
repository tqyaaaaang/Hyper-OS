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

	void set_pid(int pid);
	int get_pid() const;

	state get_state() const;
	void set_state(state stat);

	void set_name(const std::string &name);
	std::string get_name() const;

	void set_prog(program *prog);
	program* get_prog() const;
	
	void exec(std::promise<int> &fin_code);

	context_t get_context() const;
	void set_context(const context_t &context);

	void add_chl(int pid);
	void set_par(int pid);

	bool tick();
	void set_slice(size_t slice);

	void set_core(CPU_core *core);
	CPU_core* get_core() const;
	
	std::condition_variable cond_var;
	std::mutex cond_mutex;
	// condition variable to break control flow
	
	std::list<process_t*>::iterator linker;
	// linker to state list

	void init_context();
	void init_data();
	void init_bss();
	void init_dmm();
	// init
	
	void set_resched(bool resch);
	bool get_resched() const;
	// set/get need_resched
	
	void vm_read(char *buf, size_t la_begin, size_t la_end);
	void vm_write(size_t addr, const char *buf_begin, const char *buf_end);	
	// vm read/write opertions
	
	size_t heap_malloc(size_t len);
	void heap_free(size_t ptr);
	size_t stack_push(size_t size);
	size_t stack_pop(size_t size);
	// heap & stack operations
	
	void clean();
	// clean process when exit

	std::thread *th;

	void set_exit_flag();
	bool get_exit_flag() const;
	
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
};
