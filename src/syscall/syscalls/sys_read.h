/**
 * syscall/syscalls/sys_read.h
 * read from device syscall
 */

#pragma once

#include "../../process/process.h"
#include "syscall_t.h"
#include "../../../dev/device_list.h"
#include "../../../dev/devices/input/input.h"

class sys_read : public syscall_t {
public:

    sys_read(dev_input *device);
    virtual int process();

private:

	dev_input *device;
};

