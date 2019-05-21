#include "test_program_manager.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/program/program_manager.h"
#include "../src/program/lib.h"
#include "../src/logging/logging.h"
#include <thread>

class prog_a : public program {
public:

	prog_a()
	{
		build();
	}

	virtual ~prog_a()
	{ }

	virtual void static_init()
	{
		set_name("prog_a");
	}
	
	virtual void main()
	{
	    hos_std->println("prog a start.");
		int pid = sys->create_process();
		logging::info << "pid : " << pid << logging::log_endl;
		sys->exec_program(pid, "prog b");
		sys->wait(pid);
		logging::info << "prog b finished." << logging::log_endl;
		hos_std->println("prog b finish.");
	    pid = sys->create_process();
		sys->exec_program(pid, "prog b");
		sys->wait(pid);
		hos_std->println("prog b finish.");
	}

};

class prog_b : public program {
public:

	prog_b()
	{
		build();
	}

	virtual ~prog_b()
	{ }

	virtual void static_init()
	{
		set_name("prog_b");
	}

	virtual void main()
	{
		logging::info << "prog b start." << logging::log_endl;
		hos_std->println("prog b start.");
	}
};

void test_program_manager()
{
	register_program("prog a", new prog_a, sizeof(prog_a));
	register_program("prog b", new prog_b, sizeof(prog_b));
	int id = proc_create_process();
	proc_exec_program(id, get_program("prog a"));
	while (1) {
		std::this_thread::yield();
	}
}
