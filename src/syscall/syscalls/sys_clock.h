/**
 * syscall/syscalls/sys_clock.h
 * clock()
 */

#pragma once

#include "../../process/process.h"
#include "syscall_t.h"

class sys_clock : public syscall_t {
public:

    sys_clock();
    virtual int process();

private:

};

