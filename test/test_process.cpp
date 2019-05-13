#include "test_process.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/logging/logging.h"
#include "../src/schedule/schedule.h"
#include "../src/status/status.h"
#include "../src/core/core.h"
#include <cassert>

using logging::info;
using logging::log_endl;

class hello_world : public program {
private:
	handle<char> str[12];
	handle<int> ptr;
	
public:

	hello_world()
	{
		build();
	}
	
	virtual void static_init()
	{
		set_name("hello, world");
		char s[13] = "hello, world";
		for (size_t i = 0; i < 12; i++) {
			str[i].alias(alloc_static<char>());
			str[i].modify_in_compile(s[i]);
		}
		ptr.alias(alloc_bss<int>());
		info << "STATIC FINISH " << get_name() << log_endl;
	}

	virtual void main()
	{
		info << "Running on Core : " << status.get_core()->get_core_id() << log_endl;
		for (size_t i = 0; i < 12; i++)
			info << (char)str[i];
		info << log_endl;
		assert(ptr == 0);
		info << (int)ptr << log_endl;
		ptr = 10;
		for (size_t i = 0; i < 12; i++)
			info << (char)str[i];
		info << log_endl;
		info << (int)ptr << log_endl;
		while (1) {
			ptr = ptr + 1;
		}
	}
	
};

void test_process()
{
	info << "start test process." << log_endl;
	program *prog = new hello_world;
	size_t pid = create_process();
	int ok = exec_program(pid, prog);
	// set to runable
	schedule(0);
	// run!
	info << "process " << pid << " may start." << log_endl;
	while (1);
}
