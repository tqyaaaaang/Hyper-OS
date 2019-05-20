/**
 * syscall/syscalls/sys_exec_prog.h
 * create_proc syscall
 */
#pragma once

#include "../../process/process.h"

class sys_exec_prog {
public:

	sys_exec_prog(size_t pid, program *prog);
    virtual int process();

private:
	size_t pid;
	program *prog;
};

