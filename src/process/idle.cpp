#include "idle.h"
#include "process_t.h"
#include <mutex>
#include <thread>
#include "../core/cpus.h"
#include "../interrupt/interrupt.h"
#include "../status/status.h"

using std::mutex;
using std::unique_lock;
using std::lock_guard;
using std::thread;

/**
 * idle use this process
 * proc->get_proc == nullptr <==> proc is idle
 */
process_t proc;

static mutex flag_mut;

static void idle_main()
{
	status.set_core(proc.get_core());
	proc.get_core()->acquire();
	while (1) {
	    lock_guard<mutex> lk(flag_mut);
		if (proc.get_exit_flag())
			break;
		proc.get_core()->set_current(&proc);
		check_interrupt();
	}
	proc.get_core()->release();
}

void signal_idle()
{
	proc.get_core()->set_current(&proc);
	proc.cond_var.notify_one();
}

void idle_exit()
{
	if (1) {
		lock_guard<mutex> lk(flag_mut);
		proc.set_exit_flag();
	}
	proc.th->join();
	delete proc.th;
}

void init_idle()
{
    proc.set_name("idle");
	proc.set_core(cores + 0);
	proc.set_prog(nullptr);
	proc.set_pid(0);
	status.get_core()->set_current(&proc);
	proc.th = new thread(idle_main);
}

