/**
 * schedule/signal.cpp
 * signals to wake up sleeping process
 */

#include "signal.h"
#include "../process/process_t.h"
#include "../logging/logging.h"
#include "schedule.h"
#include <unordered_map>

using std::mutex;
using std::lock_guard;
using std::unordered_map;

namespace signal_id {
	
	const int WAIT_EXIT = -1;
	const int KEYBOARD = -2;

	int signal_top = -128;
}

static unordered_map<int,signal_t> signal_table;

signal_t::signal_t()
{ }

signal_t::~signal_t()
{ }

void signal_t::notify(size_t data)
{
	lock_guard<mutex> lk (mut);
	que.push(data);
	if (!this->proc.empty()) {
		process_t *proc = this->proc.front();
		this->proc.pop();
		proc->set_signal_data(que.front());
		que.pop();
		sched_set_runable(proc);
	}
}

void signal_t::wait(process_t *proc)
{
	lock_guard<mutex> lk (mut);
    this->proc.push(proc);
}

int register_signal()
{
	int signal = --signal_id::signal_top;
	return signal;
}

int send_signal(int signal_id, size_t data)
{
	if (!signal_table.count(signal_id)) {
		logging::info << "signal " << signal_id << " not found" << logging::log_endl;
		return -1;
	}
	signal_table[signal_id].notify(data);
	return 0;
}

int wait_signal(int signal_id, process_t *proc)
{
	if (!signal_table.count(signal_id)) {
		logging::info << "signal " << signal_id << " not found" << logging::log_endl;
		return -1;
	}
	signal_table[signal_id].wait(proc);
	return 0;
}

