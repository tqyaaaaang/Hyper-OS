/**
 * process/process.cpp
 * operation to processes
 */

#include "process.h"
#include "process_t.h"
#include "../logging/logging.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <cassert>
#include <future>
#include "../schedule/schedule.h"
#include "../status/status.h"
#include "../core/core.h"

using std::promise;
using std::future;
using std::unordered_map;
using std::mutex;
using std::thread;
typedef process_t::state state;

static unordered_map<size_t, process_t*> proc_table;
static size_t next_pid;
static mutex pid_mutex;

static void proc_main(process_t *proc, promise<int> &fin_code)
{
	status.set_name(proc->get_name());
	status.set_core(proc->get_core());
	proc->exec(fin_code);
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
	process_t *proc = new process_t;
	proc->set_state(state::UNINIT);
	
	pid_mutex.lock();
	size_t id = ++next_pid;
	proc->set_pid(id);
	proc_table[id] = proc;
	pid_mutex.unlock();

	sched_init_proc(proc);
	
	return id;
}

/**
 * exec a program @prog on process @pid
 * @return : 0 ok.
 *          -1 process not exists.
 *          -2 program is invalid
 *        else error code of proc_main
 */
int exec_program(size_t pid, program *prog)
{
	if (prog == nullptr) {
		logging::info << "exec error. program is invalid" << logging::log_endl;
		return -2;
	}
	if (!proc_table.count(pid)) {
		logging::info << "exec error. process " << pid << " not exists" << logging::log_endl;
		return -1;
	}
	process_t *proc = proc_table[pid];
	
	proc->set_prog(prog);
	proc->set_name(prog->get_name());

	proc->init_context();
	proc->init_data();
	proc->init_bss();
	proc->init_dmm();

    sched_set_core(proc);
    sched_set_runable(proc);

	promise<int> fin_code;
	future<int> fut = fin_code.get_future();

	thread th(proc_main, proc, std::ref(fin_code));
	th.detach();

	int fcode = fut.get();
	
	proc->cond_mutex.lock();
	proc->cond_mutex.unlock();
	// wait for exec finished

	return fcode;
}

