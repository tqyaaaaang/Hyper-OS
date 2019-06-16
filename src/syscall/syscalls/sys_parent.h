/**
 * syscall/syscalls/sys_parent.h
 * get pid of parent process
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_parent : public syscall_t {
public:

    sys_parent();
    virtual int process();
private:

};

