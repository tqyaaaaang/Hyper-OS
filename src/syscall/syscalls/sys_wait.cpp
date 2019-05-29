/**
 * syscall/syscalls/sys_wait.cpp
 * create_proc syscall
 */

#include "sys_wait.h"
#include <unordered_map>
#include "../../process/process.h"
#include "../../logging/logging.h"
#include "../../schedule/signal.h"
#include "../../status/status.h"

sys_wait::sys_wait(int pid)
	:syscall_t(syscall_id_t::WAIT)
{
	this->pid = pid;
}

int sys_wait::process()
{
	logging::debug << "sys_wait : " << pid << logging::log_endl;
	int res = proc_wait(pid);
	logging::debug << "sys_wait finish." << logging::log_endl;
	return res;
}
