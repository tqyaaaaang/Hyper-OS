/**
 * syscall/syscalls/sys_pid.h
 * get pid of current process
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_pid : public syscall_t {
public:

    sys_pid();
    virtual int process();
private:

};

