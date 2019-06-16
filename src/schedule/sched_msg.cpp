#include "sched_msg.h"
#include "../process/process_t.h"
#include "../utils/panic.h"
#include <cassert>

using std::string;
typedef process_t::state state;

void msg_proc(string str)
{
	message::process
		(message::wrap_core_info("kern scheduler"))
		<< str
		<< message::msg_endl;
}

string to_string(int dat)
{
	if (dat == 0)
		return "0";
	string str = "";
	while (dat) {
		str = char(dat % 10 + '0') + str;
		dat /= 10;
	}
	return str;
}

string proc_info(process_t *proc)
{
	if (proc == nullptr)
		return "<null>";
	return proc->get_name()
		+ "(pid = "
		+ to_string(proc->get_pid())
		+ ")"; 
}

string proc_state(process_t *proc)
{
	switch(proc->get_state()) {
	case state::RUNABLE:
		return "runable";
		break;
	case state::UNINIT:
		return "uninit";
		break;
	case state::SLEEPING:
		return "sleeping";
		break;
	case state::ZOMBIE:
		return "zombie";
		break;
	}
	panic ( "unknown process state" );
	return "";
}
