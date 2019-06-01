/**
 * syscall/syscalls/sys_pid.cpp
 * get process id
 */

#include "sys_pid.h"
#include <unordered_map>
#include "../../process/process.h"
#include "../../logging/logging.h"
#include "../../status/status.h"
#include "../../core/core.h"

sys_pid::sys_pid()
	:syscall_t(syscall_id_t::PID)
{
}

int sys_pid::process()
{
	int res = status.get_core()->get_current()->get_pid();
	logging::debug << "get pid : " << res << logging::log_endl;
	return res;
}
