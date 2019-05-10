/**
 * schedule/schedule.cpp
 * schedule processes and cpus
 */

#include "schedule.h"
#include "../process/process_t.h"
#include <list>
#include <mutex>
#include <cassert>

using std::mutex;
using std::lock_guard;
using std::list;
typedef process_t::state state;
static list<process_t*> uninit, running, sleeping, zombie;
static mutex sched_mutex;
// for smp

/**
 * init process @proc
 */
void sched_init_proc(process_t *proc)
{	
	lock_guard<mutex> lk(sched_mutex);
	uninit.push_front(proc);
	proc->linker = uninit.begin();
}

/**
 * set core for @proc
 */
void sched_set_core(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
}

/**
 * set @proc to runable
 */
void sched_set_runable(process_t *proc)
{
	lock_guard<mutex> lk(sched_mutex);
	assert(proc->get_state() != state::RUNABLE);
	switch(proc->get_state()) {
	case state::UNINIT:
		uninit.erase(proc->linker);
		running.push_front(proc);
		proc->linker = running.begin();
		break;
	case state::SLEEPING:
		sleeping.erase(proc->linker);
		running.push_front(proc);
		proc->linker = running.begin();
		break;
	case state::ZOMBIE:
	case state::RUNABLE:
		break;
	}
}
