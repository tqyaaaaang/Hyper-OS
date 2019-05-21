/**
 * syscall/syscalls/sys_wait.h
 * wait syscall
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_wait : public syscall_t {
public:

    sys_wait(int pid);
    virtual int process();
private:

	int pid;
};

