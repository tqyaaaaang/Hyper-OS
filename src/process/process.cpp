/**
 * process/process.cpp
 * operation to processes
 */

#include "process.h"
#include "process_t.h"
#include "../core/core.h"
#include "../logging/logging.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <cassert>

using std::unordered_map;
using std::mutex;
using std::thread;

static unordered_map<size_t, process_t*> proc_table;
static size_t next_pid;

static void proc_main(process_t *proc)
{
	logging::info << "proc : " << proc->get_name() << " is on." << logging::log_endl;
}

void init_proc()
{
	next_pid = 0;
	logging::info << "proc init ok." << logging::log_endl;
}

void destroy_proc()
{
	for (auto i : proc_table) {
		delete i.second;
	}
}

size_t create_process()
{
	
}

void exec_program(size_t pid, program *prog)
{
	assert(prog != nullptr);
	if (!proc_table.count(pid)) {
		logging::info << "exec error. process " << pid << " not exists" << logging::log_endl;
	}
	process_t *proc = proc_table[pid];
	proc->set_prog(prog);
	// TODO
	// schedule to a cpu
	// set par
	// set state
	// set slice
	thread th(proc_main, proc);
	th.detach();
}

