/**
 * syscall/syscalls/sys_wait.h
 * wait syscall
 */
#pragma once

#include "../../process/process.h"

class sys_wait : public syscall_t {
public:

    sys_wait(int pid);
    virtual int process();
private:

	int pid;
};

