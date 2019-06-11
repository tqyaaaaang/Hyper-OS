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
#include "../schedule/signal.h"
#include "../status/status.h"
#include "../core/core.h"
#include "../program/program_manager.h"
#include "../env/env.h"
#include "../core/cpus.h"
#include "idle.h"
#include <thread>

using std::promise;
using std::future;
using std::unordered_map;
using std::mutex;
using std::thread;
using std::lock_guard;
typedef process_t::state state;

unordered_map<int, process_t*> proc_table;
mutex table_mutex;

static int next_pid;
static mutex pid_mutex;

static void proc_main(process_t *proc, promise<int> &fin_code)
{
	try {
		status.set_name(proc->get_name());
		status.set_core(proc->get_core());
		proc->exec(fin_code);
	} catch(int id) {
		logging::info << "process exit with id : " << id << logging::log_endl;
	}
	logging::info << "process exit" << logging::log_endl;
}

bool proc_not_exit(int pid)
{
	lock_guard<mutex> lk(table_mutex);
	return proc_table.count(pid);
}

void init_proc()
{
	next_pid = 0;
	logging::info << "proc init ok." << logging::log_endl;
    if (!TEST) {
		init_idle();
		int pid = proc_create_process();
		proc_exec_program(pid, get_program("shell"));
		while (proc_not_exit(pid)) {
			std::this_thread::yield();
		}
		logging::info << "main shell exit, system shut down." << logging::log_endl;
		idle_exit();
		cores[0].disable_interrupt();
		logging::info << "main shell exit, system shut down." << logging::log_endl;
	}
}

void destroy_proc()
{
}

int proc_create_process()
{
	lock_guard<mutex> lkr(table_mutex);

	process_t *cur = status.get_core()->get_current();
	process_t *proc = new process_t;
	
	pid_mutex.lock();
	int id = ++next_pid;
	proc->set_pid(id);
	logging::debug << "proc table set : " << id << logging::log_endl;
	proc_table[id] = proc;
	pid_mutex.unlock();

	proc->set_state(state::UNINIT);
	if (cur != nullptr) {
		logging::debug << "set father for " << id << " to " << cur->get_pid() << logging::log_endl;
		proc->set_par(cur->get_pid());
		cur->add_chl(proc->get_pid());
	} else {
		proc->set_par(0);
	}
	
    sched_init_proc(proc);
	
	return id;
}

/**
 * exec a program @prog on process @pid
 * @return : 0 ok.
 *          -1 process not exists.
 *          -2 program is invalid
 *          -3 pid is not child process
 *        else error code of proc_main
 */
int proc_exec_program(int pid, program *prog)
{
	lock_guard<mutex> lkr(table_mutex);
	if (prog == nullptr) {
		logging::warning << "exec error. program is invalid" << logging::log_endl;
		return -2;
	}
	if (!proc_table.count(pid)) {
		logging::info << "exec error. process " << pid << " not exists" << logging::log_endl;
		return -1;
	}
	process_t *cur = status.get_core()->get_current();
	process_t *proc = proc_table[pid];
	if (cur != nullptr && proc->get_par() != cur->get_pid()) {
		logging::info << "exec error. process " << pid << " is not child of current process " << cur->get_pid() << logging::log_endl;
		logging::debug << "true father is " << proc->get_par() << logging::log_endl;
		return -3;
	}
	
	proc->set_prog(prog);
	proc->set_name(prog->get_name());

	logging::info << proc->get_name() << " " << prog->get_name() << logging::log_endl;
	
	proc->init_context();
	proc->init_data();
	proc->init_bss();
	proc->init_dmm();

    sched_set_core(proc);
    sched_set_runable(proc);

	promise<int> fin_code;
	future<int> fut = fin_code.get_future();

	proc->th = new thread(proc_main, proc, std::ref(fin_code));

	int fcode = fut.get();
	
	proc->cond_mutex.lock();
	proc->cond_mutex.unlock();
	// wait for exec finished

	return fcode;
}

/**
 * yield and give up cpu 
 */
int proc_yield()
{
	process_t *proc = status.get_core()->get_current();
	proc->set_resched(1);
	return 0;
}

/**
 * wait process or signal pid
 * @pid : signal/process id
 *        signal id is smaller than 0, process id is greater than 0
 * @return : 0   ok
 *           -1  pid is not children of current process
 *           -2  pid is not valid signal id
 */
int proc_wait(int pid)
{
	process_t *proc = status.get_core()->get_current();
	if (pid >= 0) {
		if (proc->is_chl(pid)) {
			sched_set_wait(proc, pid);
		} else {
			return -1;
		}
	} else {
		sched_set_sleep(proc);
		if (wait_signal(pid, proc) != 0)
			return -2;
	}
	return 0;
}

int proc_exit()
{
	process_t *proc = status.get_core()->get_current();
	sched_set_exit(proc);
	return 0;
}

int proc_sleep()
{
	process_t *proc = status.get_core()->get_current();
	sched_set_sleep(proc);
	return 0;
}
