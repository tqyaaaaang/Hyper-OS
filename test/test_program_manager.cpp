#include "test_program_manager.h"
#include "../src/program/program.h"
#include "../src/process/process.h"
#include "../src/program/program_manager.h"
#include "../src/program/lib.h"
#include "../src/logging/logging.h"
#include "../src/program/sys_t.h"
#include <thread>
#include <string>

using std::string;

class prog_a : public program {
public:

	virtual ~prog_a()
	{ }

	virtual void static_init()
	{
		set_name("prog_a");
		name = alloc_static<char>(20);
		char str[] = "prog b";
	    for (size_t i = 0, size = 6; i < size; i++)
			name[i].modify_in_compile(str[i]);
	}

	string tostring(char c)
	{
		string str;
		str = str + c;
		return str;
	}
	
	virtual void main()
	{
	    hos_std->println("prog a start.");
		int pid = sys->create_process();
		logging::info << "pid : " << pid << logging::log_endl;
		for (size_t i = 0; i < 6; i++)
			hos_std->print(tostring(name[i]));
		hos_std->println("");
		sys->exec_program(pid, name);
		sys->wait(pid);
		logging::info << "prog b finished." << logging::log_endl;
		hos_std->println("prog b finish.");
	    pid = sys->create_process();
		sys->exec_program(pid, name);
		sys->wait(pid);
		hos_std->println("prog b finish.");
	}

private:

	handle<char> name;
};

class prog_b : public program {
public:

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

program* gen_a()
{ return new prog_a; }

program* gen_b()
{ return new prog_b; }

void test_program_manager()
{
	register_program("prog a", gen_a);
	register_program("prog b", gen_b);
	int id = proc_create_process();
	proc_exec_program(id, get_program("prog a"));
	while (1) {
		std::this_thread::yield();
	}
}
