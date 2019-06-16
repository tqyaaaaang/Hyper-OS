/**
 * schedule/signal.cpp
 * signals to wake up sleeping process
 */

#include "signal.h"
#include "../process/process_t.h"
#include "../logging/logging.h"
#include "../io/special_keys.h"
#include "schedule.h"
#include <unordered_map>
#include "../process/process.h"
#include <cassert>
#include "../status/status.h"
#include  "../core/core.h"

using std::mutex;
using std::lock_guard;
using std::unordered_map;

namespace signal_id {
	
	const int WAIT_EXIT = -1;
	const int KEYBOARD = -2;

	int signal_top = -128;
}

static unordered_map<int,signal_t *> signal_table;

signal_t::signal_t()
{ }

signal_t::signal_t(int id)
{
	this->signal_id = id;
}

signal_t::~signal_t()
{ }

/**
 * @return : false, not a keyboard signal or current is idle
 *           true, is a keyboard signal and is handled
 */
bool signal_t::check_keyboard_signal(int data, process_t *proc)
{
	logging::debug << "check keyboard signal : " << data << " " << this->signal_id << logging::log_endl;
	if (this->signal_id == signal_id::KEYBOARD) {
		bool return_value = false;
		switch (static_cast<special_keys>(-data)) {
		case special_keys::CTRL_C:
			assert(proc != nullptr);
			if (proc->get_prog() == nullptr) {
				// is idle
				return_value = false;
			} else {
				logging::debug << "process " << proc->get_name() << " exits because of <c-c>" << logging::log_endl;
				sched_set_exit(proc);
				return_value = true;
			}
			break;
		default:
			break;
		}
		return return_value;
	} else {
		return 0;
	}
}

void signal_t::notify(int data)
{
	lock_guard<mutex> lk(mut);
	if (!check_keyboard_signal(data, status.get_core()->get_current())) {
		if (!this->proc.empty()) {
			process_t *proc = this->proc.front();
			this->proc.pop();
			if (!check_keyboard_signal(data, proc)) {
				proc->set_signal_data(data);
				sched_set_runable(proc);
			}
		} else {
			que.push(data);
		}
	}
}

void signal_t::wait(process_t *proc)
{
	lock_guard<mutex> lk(mut);
	if (!que.empty()) {
		int current_data = que.front();
		que.pop();
		if (!check_keyboard_signal(current_data, proc)) {
			proc->set_signal_data(current_data);
			sched_set_runable(proc);
		}
	} else {
		this->proc.push(proc);
	}
}

void init_signal ()
{
	logging::debug << "Initializing process wait signals" << logging::log_endl;
	signal_table.insert ( std::make_pair ( signal_id::WAIT_EXIT, new signal_t (signal_id::WAIT_EXIT) ) );
	signal_table.insert ( std::make_pair ( signal_id::KEYBOARD, new signal_t (signal_id::KEYBOARD) ) );
}

void destroy_signal ()
{
	logging::debug << "Destroying process wait signals" << logging::log_endl;
	for ( auto &x : signal_table ) {
		delete x.second;
	}
}

int send_signal(int signal_id, int data)
{
	if (!signal_table.count(signal_id)) {
		logging::info << "signal " << signal_id << " not found" << logging::log_endl;
		return -1;
	}
	signal_table[signal_id]->notify(data);
	return 0;
}

int wait_signal(int signal_id, process_t *proc)
{
	if (!signal_table.count(signal_id)) {
		logging::info << "signal " << signal_id << " not found" << logging::log_endl;
		return -1;
	}
	signal_table[signal_id]->wait(proc);
	return 0;
}

int register_signal ()
{
	int signal = --signal_id::signal_top;
	signal_table.insert ( std::make_pair ( signal, new signal_t (signal) ) );
	return signal;
}

