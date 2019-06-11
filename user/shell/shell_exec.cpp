#include "shell.h"

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
