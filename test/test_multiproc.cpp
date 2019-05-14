#include "test_multiproc.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/logging/logging.h"
#include "../src/schedule/schedule.h"
#include "../src/status/status.h"
#include "../src/core/core.h"
#include <cassert>
#include <iostream>

using logging::info;
using logging::log_endl;
using std::cout;
using std::endl;

static int id = 0;

class tick_prog : public program {
private:
	handle<int> tick;
	int this_id;
	
public:

	tick_prog()
	{
		this_id = id++;
		info << "build " << id << log_endl;
		build();
	}
	
	virtual void static_init()
	{
		set_name("tick-prog");
		tick.alias(alloc_bss<int>());
	}

	virtual void main()
	{
		cout << this_id << " start." << endl;
		while (1) {
			tick = tick + 1;
			if (tick % 10000 == 0) {
				int tmp = tick;
				cout << "tick " << tmp << " from " << this_id << endl;
			}
		}
	}
};

void test_multiproc()
{
	info << "start multi process test" << log_endl;
	program *prox[10];
	for (int i = 0; i < 10; i++)
		prox[i] = new tick_prog;
	size_t x[10];
	for (int i = 0; i < 10; i++) {
		x[i] = create_process();
	}
	for (int i = 0; i < 10; i++)
		assert(prox[i]->get_data_size() == 0);
	for (int i = 0; i < 10; i++) {
		int ok = exec_program(x[i], prox[i]);
	}
	schedule(0);
	while (1) {
	    std::this_thread::yield();
	}
}
