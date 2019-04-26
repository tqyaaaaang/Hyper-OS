#pragma once

#include <cstdint>
#include "process.h"
#include "defs.h"

extern process *current_proc;        // current running proc / system service

extern size_t next_pid;              // next available proc pid 

void state_init();
