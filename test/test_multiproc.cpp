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
	handle<char> str;
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
		tick = alloc_bss<int>();
		str = alloc_static<char>(12);
		char s[] = "hello world";
		for (int i = 0; i < 11; i++)
			str[i].modify_in_compile(s[i]);
	}

	virtual void main()
	{
		cout << this_id << " start." << endl;
		for (int i = 0; i < 11; i++)
			cout << str[i];
		cout << endl;
		while (tick < 100000) {
			tick = tick + 1;
			if (tick % 10000 == 0) {
				handle<int> st = (int)tick;
				st = st + 1;
				// st is in stack
				int tmp = st;
				cout << "tick " << tmp << " from " << this_id << endl;
			}
		}
		cout << "tick " << this_id << " finished." << endl;
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
		info << "cr " << i << log_endl;
		x[i] = create_process();
	}
	for (int i = 0; i < 10; i++) {
		int ok = exec_program(x[i], prox[i]);
	}
	schedule(0);
	while (1) {
	    std::this_thread::yield();
	}
}
