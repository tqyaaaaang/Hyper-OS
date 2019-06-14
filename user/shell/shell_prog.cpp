#include "shell.h"
#include <string>
using std::string;

void hyper_shell::pid_prog()
{
	if (argc > 2) {
		format_error();
	} else {
		if (argc == 1) {
			help_pid_title();
			help_pid_body();
		} else {
			string str = to_string(argv[1]);
			int result = 0;
			if (str == "parent") {
			    result = sys->parent();
			} else if (str == "current") {
				result = sys->pid();
			} else {
				argv_error(1);
				return;
			}
			hos_std->write_int(result);
			hos_std->putchar('\n');	
		}
	}
}

void hyper_shell::help_pid_body()
{
	hos_std->println("- Format  : pid [{which}]");
	hos_std->println("- {which} : parent, current");
}
