/**
 * syscall/syscall_id.h
 * System call IDs
 */

#pragma once

enum class syscall_id_t
{
	SYSCALL_TEST = 0,
		CREATE_PROC,
		EXEC_PROGRAM,
		YIELD,
		WAIT,
		EXIT,
		READ,
		WRITE,
		PID,
		CLOCK,
		PARENT
};
