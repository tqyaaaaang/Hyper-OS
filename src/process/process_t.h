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
#include "../context/context.h"
#include "../program/program.h"

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

	void set_pid(size_t pid);
	size_t get_pid() const;

	state get_state() const;
	void set_state(state stat);

	void set_name(const std::string &name);
	std::string get_name() const;

	void set_prog(program *prog);
	void exec(std::promise<int> &fin_code);

	context_t get_context() const;
	void set_context(const context_t &context);

	void add_chl(size_t pid);
	void set_par(size_t pid);

	bool tick();
	void set_slice(size_t slice);

	void set_core(CPU_core *core);
	CPU_core get_core() const;

	std::condition_variable cond_var;
	std::mutex cond_mutex;
	std::list<process_t*>::iterator linker;
	
	void init_context();
	void init_data();
	void init_bss();
	void init_dmm();
	
private:

	size_t pid;
	std::string name;
	program *prog;
	state pstat;
	context_t context;

	bool need_resched;
	size_t slice;
	
	size_t ptr_par;
	std::set<size_t> ptr_chl;

	CPU_core *core;
};
