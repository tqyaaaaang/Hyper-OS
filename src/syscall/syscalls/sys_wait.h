/**
 * syscall/syscalls/sys_wait.h
 * create_proc syscall
 */
#pragma once

#include "../../process/process.h"

class sys_wait {
public:

    sys_wait(size_t pid);
    virtual int process();
private:

	size_t pid;
};

