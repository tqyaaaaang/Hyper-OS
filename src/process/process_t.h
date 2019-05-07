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
#include "../context/context.h"
#include "../core/core.h"
#include "../program/program.h"

class process_t {
public:

	enum state {
		SLEEPING,
		RUNABLE,
		RUNNING,
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
	void exec();

	context_t get_context() const;
	void set_context(const context_t &context);
	
private:

	size_t pid;
	std::string name;
	program *prog;
	state pstat;
	context_t context;
	bool need_resched;

	process_t* ptr_fa;
	std::set<process_t*> ptr_chl;

	bool wait_all;
	std::list<size_t> wait_list;

	
};
