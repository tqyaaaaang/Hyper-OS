/**
 * schedule/schedule.cpp
 * schedule processes and cpus
 */

#include "schedule.h"
#include "../process/process_t.h"
#include "../core/cpus.h"
#include "../logging/logging.h"
#include "../status/status.h"
#include <list>
#include <mutex>
#include <cassert>
#include <map>
#include <unordered_map>

using std::mutex;
using std::lock_guard;
using std::list;
using std::unordered_map;

using logging::debug;
using logging::log_endl;
using logging::info;

extern unordered_map<int, process_t*> proc_table;
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
state_list_t *state_list;

/**
 * waiting map & awake list
 * wait_map[i] = proc means proc is waiting for i
 * awake_list[i] is awake list of CPU #i
 */
unordered_map<int, list<process_t*> > wait_map;
list<process_t*> *awake_list;

/**
 * schedule init
 */
void init_schedule()
{
	state_list = new state_list_t[get_core_num()];
	awake_list = new list<process_t*>[get_core_num()];
}

/**
 * schedule destroy
 */
void destroy_schedule()
{
	delete[] state_list;
	delete[] awake_list;
}

static mutex sched_mutex;
// for smp

/**
 * init process @proc
 */
void sched_init_proc(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
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
	logging::info << "process " << proc->get_name() << " need some sleep." << (proc->get_state() == state::SLEEPING) << log_endl;
	int core_id = proc->get_core()->get_core_id();

	assert(proc->get_state() == state::RUNABLE);
	proc->set_state(state::SLEEPING);

	state_list[core_id].running.erase(proc->linker);
	state_list[core_id].sleeping.push_front(proc);
	proc->linker = state_list[core_id].sleeping.begin();

	status.get_core()->set_current(nullptr);
	logging::info << "process " << proc->get_name() << " slept." << log_endl;
}

void sched_set_sleep(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	sched_set_sleep_nlock(proc);
}

void sched_set_wait(process_t *proc, int pid)
{
	lock_guard<mutex> lk(sched_mutex);	
	sched_set_sleep_nlock(proc);
	assert(proc->get_state() == state::SLEEPING);
	if (proc_table[pid]->get_state() != state::ZOMBIE) {
		if (!wait_map.count(pid))
			wait_map[pid] = list<process_t*>();
		wait_map[pid].push_back(proc);		
	}
}

/**
 * set process @proc to zombie
 */
void sched_set_exit(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	logging::info << "process " << proc->get_name() << " is set exit." << log_endl;

	assert(proc == status.get_core()->get_current());
	assert(proc->get_state() == state::RUNABLE);

	proc->set_state(state::ZOMBIE);
	
	int id = proc->get_core()->get_core_id();

	state_list[id].running.erase(proc->linker);
	zombie.push_front(proc);
	proc->linker = zombie.begin();
	
	status.get_core()->set_current(nullptr);

	logging::info << "process " << proc->get_name() << " exit." << log_endl;

	if (wait_map.count(proc->get_pid())) {
		for (process_t *i : wait_map[proc->get_pid()]) {
			logging::info << "waking up : " << i->get_pid() << logging::log_endl;
			sched_set_runable_nlock(i);
		}
	}
}

void schedule(int id)
{
	if ( cores[id].get_interrupt_depth () == 1 ) {
		process_t *proc = cores[id].get_current ();

		if ( proc == nullptr ) {
			logging::info << "proc is null pointer" << logging::log_endl;
		}

		if ( proc == nullptr || proc->get_resched () ) {
			if ( proc != nullptr ) {
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
				nxt_proc->cond_var.notify_one ();
			}
			if ( nxt_proc != nullptr ) {

				logging::info << "switch process : " << nxt_proc->get_name () << logging::log_endl;
			}
		} else {
			proc->cond_var.notify_one ();
		}
	} else {
		logging::debug << "Not running schedule because interrupt depth is greater than 1" << logging::log_endl;
	}

}
