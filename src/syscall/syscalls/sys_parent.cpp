/**
 * syscall/syscalls/sys_parent.cpp
 * get process id of parent
 */

#include "sys_parent.h"
#include "../../process/process.h"
#include "../../logging/logging.h"
#include "../../status/status.h"
#include "../../core/core.h"

sys_parent::sys_parent()
	:syscall_t(syscall_id_t::PARENT)
{
}

int sys_parent::process()
{
	int res = status.get_core()->get_current()->get_par();
	logging::debug << "get pid : " << res << logging::log_endl;
	return res;
}
