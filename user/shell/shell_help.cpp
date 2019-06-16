#include "shell.h"

void hyper_shell::help_prog()
{
	if (argc == 1) {
		general_help();
	} else {
		hos_std->println("not supported yet");
	}
}

void hyper_shell::general_help()
{
	hos_std->println("Hyper-OS Shell, Version 0.1");
	hos_std->println("--- Supported Commands  ---");
    help_exit_title();
	help_msg_title();
	help_pid_title();
	help_exec_title();
}

void hyper_shell::help_exit_title()
{
	hos_std->println("[exit] : exit the shell");
}

void hyper_shell::help_exec_title()
{
	hos_std->println("[program-name] : run program by name");
}

void hyper_shell::help_msg_title()
{
	hos_std->println("[msg] : enable or disable message");
}

void hyper_shell::help_pid_title()
{
	hos_std->println("[pid] : show pid of current or parent process");
}
