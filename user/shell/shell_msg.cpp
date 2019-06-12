#include "shell.h"
#include "../../src/message/message.h"

using std::string;
using message::set_interrupt_message;
using message::set_process_message;
using message::set_memory_message;
using message::set_test_message;

static void set_all(bool type)
{
	set_interrupt_message(type);
	set_process_message(type);
	set_memory_message(type);
	set_test_message(type);
}

void hyper_shell::msg_prog()
{
	if (argc == 1) {
		help_msg_title();
		help_msg_body();
	} else if (argc == 3) {
		string str = to_string(argv[1]), type = to_string(argv[2]);
	    int flag = -1;
		if (str == "enable") {
			flag = 1;
		} else if (str == "disable") {
			flag = 0;
		} else {
			argv_error(1);
		}
		if (flag != -1) {
			if (type == "all") {
				set_all(flag);
			} else if (type == "interrupt") {
			    set_interrupt_message(flag);
			} else if (type == "process") {
				set_process_message(flag);
			} else if (type == "memory") {
				set_memory_message(flag);
			} else if (type == "test") {
				set_test_message(flag);
			} else {
				argv_error(2);
			}
		}	
	} else {
		format_error();
	}
}

void hyper_shell::help_msg_body()
{
	hos_std->println("- Format : msg [enable/disable] {type}");
	hos_std->println("- {type} : interrupt, process, memory, test, all");
}

