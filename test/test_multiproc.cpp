#include "test_multiproc.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/logging/logging.h"
#include "../src/schedule/schedule.h"
#include "../src/status/status.h"
#include "../src/core/core.h"
#include <cassert>

using logging::info;
using logging::log_endl;

static int id = 0;

class tick_prog : public program {
private:
	handle<int> tick;
	int this_id;
	
public:

	tick_prog()
	{
		this_id = id++;
		build();
	}
	
	virtual void static_init()
	{
		set_name("tick-prog");
		tick.alias(alloc_bss<int>());
	}

	virtual void main()
	{
		while (1) {
			tick = tick + 1;
			if (tick % 100000 == 0) {
				info << "tick " << tick << " from " << this_id << log_endl;
			}
		}
	}
};

void test_multiproc()
{
	info << "start multi process test" << log_endl;
	program *prox = new tick_prog;
	program *proy = new tick_prog;
	size_t x = create_process(), y = create_process();
	int ok = exec_program(x, prox);
	ok = exec_program(y, proy);
	schedule(0);
	while (1);
}
