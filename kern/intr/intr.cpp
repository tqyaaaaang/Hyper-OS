#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <cassert>
#include "state_info.h"
#include "intr.h"
#include "defs.h"

using std::condition_variable;
using std::mutex;
using std::thread;
using std::unique_lock;
using std::cerr;
using std::endl;
using std::cout;

mutex cpu_owner;              // mutex for cpu owner 

condition_variable do_intr;   // ISR sleep on it (if intr_enable = 1)
mutex do_intr_mut;

thread intr_admin;            // intr admin thread 
mutex mut_intr_enable;        // mut for setting current_info & intr_enable
static bool intr_enable;             // intr_enable tag
static intr_info *current_info;      // current interrupt info

static size_t ticks = 0;

void alltraps()
{
	mut_intr_enable.lock();
	// unenable interrupt
	switch(current_info->intr_id) {
	case clock_intr:
		ticks++;
		cout << "tick!" << endl;
		break;
	default:
		assert(false);
	}
	mut_intr_enable.unlock();
}

static void thread_main()
{
	while (intr_enabled()) {
		unique_lock<mutex> locker(do_intr_mut);
		// enable to service next interrupt
		do_intr.wait(locker);
		// waiting for a interrupt
		cpu_owner.lock();
		// waiting for current process yield
		// current process unlock cpu_owner
		// - and sleep
		alltraps();
		// handle the interrupt
		cpu_owner.unlock();
		// todo : schedule();
	}
}


bool intr_enabled()
{
	mut_intr_enable.lock();
	bool state = intr_enable;
	mut_intr_enable.unlock();
	return state;
}

void intr_on()
{
	assert(!intr_enabled());	
	intr_enable = true;
	intr_admin = thread(thread_main);
	intr_admin.detach();
}

void intr_off()
{
	assert(intr_enabled());
	mut_intr_enable.lock();
	intr_enable = false;
	mut_intr_enable.unlock();
}

void intr_init()
{
	cout << "intr on." << endl;
	intr_on();
}

mutex intr_list;

void intr_alert(intr_info *info)
{
	mut_intr_enable.lock();
	current_info = info;
	do_intr.notify_one();
	mut_intr_enable.unlock();
}
