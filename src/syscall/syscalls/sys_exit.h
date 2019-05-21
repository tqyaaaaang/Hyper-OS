/**
 * syscall/syscalls/sys_exit.h
 * exit syscall
 */
#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_exit : public syscall_t {
public:

    sys_exit();
    virtual int process();

};

