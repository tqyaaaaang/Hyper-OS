/**
 * schedule/schedule.cpp
 * schedule processes and cpus
 */

#include "schedule.h"
#include "sched_msg.h"
#include "../process/process_t.h"
#include "../core/cpus.h"
#include "../logging/logging.h"
#include "../status/status.h"
#include <list>
#include <mutex>
#include <cassert>
#include <map>
#include <unordered_map>
#include <vector>

using std::mutex;
using std::lock_guard;
using std::list;
using std::unordered_map;
using std::vector;

using logging::debug;
using logging::log_endl;
using logging::info;

extern unordered_map<int, process_t*> proc_table;
extern mutex table_mutex;

typedef process_t::state state;

struct state_list_t {
	list<process_t*> running;
	list<process_t*> sleeping;
	list<process_t*> uninit;
};

list<process_t*> uninit;
list<process_t*> zombie;

/**
 * state list array for cpus
 * state_list[i] is state list of CPU #i
 */ 
vector<state_list_t> state_list;

/**
 * waiting map & awake list
 * wait_map[i] = proc means proc is waiting for i
 * awake_list[i] is awake list of CPU #i
 */
unordered_map<int, list<process_t*> > wait_map;
unordered_map<int, process_t*> zombie_map;
vector< list<process_t*> > awake_list;

/**
 * schedule init
 */
void init_schedule()
{
	state_list.resize(get_core_num());
	awake_list.resize(get_core_num());
}

/**
 * schedule destroy
 */
void destroy_schedule()
{
	// delete[] state_list;
	// delete[] awake_list;
}

static mutex sched_mutex;
// for smp

/**
 * init process @proc
 */
void sched_init_proc(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	msg_proc("process "
			 + proc_info(proc)
			 + " is created, state : uninit");
	assert(proc != nullptr);
	uninit.push_front(proc);
	proc->linker = uninit.begin();
}

/**
 * set core for @proc
 * TODO : only CPU0
 */
void sched_set_core(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	int id = 0;
	msg_proc("process "
			 + proc_info(proc)
			 + " is assigned to CPU #"
			 + to_string(id));
	uninit.erase(proc->linker);
	proc->set_core(cores + id);
	state_list[id].uninit.push_front(proc);
	proc->linker = state_list[id].uninit.begin();
}

/**
 * set @proc to runable
 */

void sched_set_runable_nlock(process_t *proc)
{
	logging::info << "process " << proc->get_name() << " is set runable." << log_endl;
	assert(proc->get_state() != state::RUNABLE);
	msg_proc("process "
			 + proc_info(proc)
			 + " state change : "
			 + proc_state(proc)
			 + " --> runable");
	int id = proc->get_core()->get_core_id();
	switch(proc->get_state()) {
	case state::UNINIT:
		state_list[id].uninit.erase(proc->linker);
		state_list[id].running.push_front(proc);
		proc->linker = state_list[id].running.begin();
		break;
	case state::SLEEPING:
		state_list[id].sleeping.erase(proc->linker);
		state_list[id].running.push_front(proc);
		proc->linker = state_list[id].running.begin();
		break;
	case state::ZOMBIE:
	case state::RUNABLE:
		break;
	}
	proc->set_state(state::RUNABLE);
	proc->get_prog()->run();
}

void sched_set_runable(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	sched_set_runable_nlock(proc);
}

void sched_set_sleep_nlock(process_t *proc)
{
	logging::info << "process " << proc->get_name() << " " << proc->get_pid() << " need some sleep." << (proc->get_state() == state::SLEEPING) << log_endl;

	msg_proc("process "
			 + proc_info(proc)
			 + " state change : "
			 + proc_state(proc)
			 + " --> sleeping");

	int core_id = proc->get_core()->get_core_id();

	assert(proc->get_state() == state::RUNABLE);
	proc->set_state(state::SLEEPING);

	state_list[core_id].running.erase(proc->linker);
	state_list[core_id].sleeping.push_front(proc);
	proc->linker = state_list[core_id].sleeping.begin();

	status.get_core()->set_current(nullptr);
	logging::info << "process " << proc->get_name() << " " << proc->get_pid() << " slept." << log_endl;
}

void sched_set_sleep(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	sched_set_sleep_nlock(proc);
}

void sched_set_wait(process_t *proc, int pid)
{
	lock_guard<mutex> lk(sched_mutex);
	lock_guard<mutex> lkr(table_mutex);
	msg_proc("process " + proc_info(proc)
			 + " will wait process "
			 + to_string(pid));	
	info << "process " << proc->get_name() << " " << proc->get_pid() << " wait " << pid << log_endl;
	if (proc_table.count(pid)) {
		sched_set_sleep_nlock(proc);
		assert(proc->get_state() == state::SLEEPING);
		if (!wait_map.count(pid))
			wait_map[pid] = list<process_t*>();
		wait_map[pid].push_back(proc);		
	} else if (zombie_map.count(pid)) {
		logging::debug << "process " << pid << " has already finished." << logging::log_endl;
		process_t *chl = zombie_map[pid];
		zombie_map.erase(pid);
		chl->set_exit_flag();
		chl->cond_var.notify_one();
		chl->th->join();
		logging::debug << "deleting process " << pid << logging::log_endl;
		delete chl->th;
		delete chl;
    } else {
		logging::warning << "process " << pid << " not exists" << logging::log_endl;
	}
}

/**
 * set process @proc to zombie
 */
void sched_set_exit(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	lock_guard<mutex> lkr(table_mutex);
	logging::info << "process " << proc->get_name() << " is set exit." << log_endl;
	
	msg_proc("process " + proc_info(proc)
			 + " exit");
	
	assert(proc == status.get_core()->get_current());
	assert(proc->get_state() == state::RUNABLE);

	proc->set_state(state::ZOMBIE);
	
	int id = proc->get_core()->get_core_id();

	state_list[id].running.erase(proc->linker);
	zombie.push_front(proc);
	proc->linker = zombie.begin();
	
	status.get_core()->set_current(nullptr);

	logging::info << "process " << proc->get_name() << " " << proc->get_pid() << " exit." << log_endl;

	proc->clean();

	// logging::debug << "erase " << proc->get_pid() << logging::log_endl;
	
	proc_table.erase(proc->get_pid());
	zombie_map[proc->get_pid()] = proc;

	if (wait_map.count(proc->get_pid())) {
		for (process_t *i : wait_map[proc->get_pid()]) {
			msg_proc("process " + proc_info(proc)
					 + " exit, waiting"
					 + " process " + proc_info(i)
					 + " will be waken up");
			logging::info << "waking up : " << i->get_pid() << logging::log_endl;
			sched_set_runable_nlock(i);
		}
		wait_map.erase(proc->get_pid());
	}

    proc->set_exit_flag();
}

void schedule(int id)
{
	lock_guard<mutex> lk(sched_mutex);

	if ( cores[id].get_interrupt_depth () <= 1 ) {
		process_t *proc = cores[id].get_current ();

		if ( proc == nullptr ) {
			logging::info << "proc is null pointer" << logging::log_endl;
		}

		if ( proc == nullptr || proc->get_resched () ) {
			if ( proc != nullptr && proc->get_prog() != nullptr) {
				// not idle
				
				proc->set_resched ( 0 );
				proc->set_slice ( 1 );
				state_list[id].running.erase ( proc->linker );
				state_list[id].running.push_front ( proc );
				proc->linker = state_list[id].running.begin ();
			}

			// next proc
			if ( state_list[id].running.empty () )
				return;

			process_t *nxt_proc = state_list[id].running.back ();
			if ( nxt_proc != nullptr ) {
				cores[id].set_current ( nxt_proc );
				cores[id].set_context ( nxt_proc->get_context () );

				nxt_proc->cond_mutex.lock ();
				nxt_proc->cond_mutex.unlock ();
				// make sure nxt_proc sleep sucessfully

				// awake

				logging::info << "nodify process " << nxt_proc->get_name() << " " << nxt_proc->get_pid() << logging::log_endl;
				if (proc != nxt_proc) {
					msg_proc("time slice of process "
							 + proc_info(proc)
							 + " is running out, switch to process "
							 + proc_info(nxt_proc));
				}
				nxt_proc->cond_var.notify_one ();
			}
			if (nxt_proc != proc && nxt_proc != nullptr) {

				logging::info << "switch process : " << nxt_proc->get_name () << " " << nxt_proc->get_pid() << logging::log_endl;
			}
		} else {
			logging::info << "nodify process " << proc->get_name() << " " << proc->get_pid() << logging::log_endl;
			proc->cond_var.notify_one ();
		}
	} else {
		logging::debug << "Not running schedule because interrupt depth is greater than 1" << logging::log_endl;
	}

}
