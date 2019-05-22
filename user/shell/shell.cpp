/**
 * shell for hos
 * --------------------
 * - run prog by name
 */
#include "shell.h"
#include "../../src/program/program_manager.h"
#include <cstdlib>
#include <cassert>
#include <string>
#include "../../src/logging/logging.h"

using std::string;

static program* gen()
{ return new hyper_shell; }

void register_shell()
{
	register_program("shell", gen);
}

hyper_shell::hyper_shell()
{ }

hyper_shell::~hyper_shell()
{ }

void hyper_shell::static_init()
{
	set_name("shell");
	argc = alloc_bss<int>();
	for (size_t i = 0; i < 20; i++)
		argv[i] = alloc_bss<char>(30);
}

void hyper_shell::main()
{
	logging::info << "shell start." << logging::log_endl;
	hos_std->println("~hello~");
    string str; str = str + char(cur_proc->get_pid() + '0');
	while (1) {
		hos_std->print(str + "@hyper-shell:$ ");
		handle<int> res = (int)parse();
		if (res == 0) {
			logging::info << "operation success." << logging::log_endl; 
		} else if (res == -1) {
			break;
		} else if (res == -2) {
			logging::info << "operation failed." << logging::log_endl;
		}
	}
	hos_std->println("~see you~");
}

handle<int> hyper_shell::parse()
{
	handle<int> c;
	handle<int> top = 0;
	argc = 0;
	while (1) {
		c = get();
	    if (c == '\n') {
			argv[argc][top] = '\0';
			argc = argc + 1;
			break;
		} else if (c == ' ') {
			argv[argc][top] = '\0';
			argc = argc + 1;
			top = 0;
		} else {
			argv[argc][top] = c;
			top = top + 1;
		}
	}
	assert(argc > 0);
	return (int)servant(argv[0]);
}

handle<int> hyper_shell::get()
{
	handle<int> ch = alloc_heap<int>();
	ch = getchar();
	return ch;
}

handle<int> hyper_shell::servant(handle<char> cmd)
{
	string str = to_string(cmd);
    if (str == "exit") {
		return -1;
	} else if (is_program(to_string(cmd))) {
		exec();
	} else {
		error();
	}
	return 0;
}

string hyper_shell::to_string(handle<char> str)
{
	string tar;
	for (size_t i = 0; str[i] != '\0'; i++) {
		tar = tar + (char)(str[i]);
	}
	return tar;
}

void hyper_shell::error()
{
	hos_std->println("invalid program name");
}

void hyper_shell::exec()
{
	int pid = sys->create_process();
	logging::debug << "create process pid" << logging::log_endl;
	// program *prog = get_program(to_string(argv[0]));
	logging::debug << "get program : " << to_string(argv[0]) << " pid : " << pid << logging::log_endl;
	sys->exec_program(pid, argv[0]);
	logging::debug << "sub program start." << logging::log_endl;
	sys->wait(pid);
	logging::debug << "sub program exit." << logging::log_endl;
}
