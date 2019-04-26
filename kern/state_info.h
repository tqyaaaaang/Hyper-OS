#pragma once

#include <cstdint>
#include <mutex>
#include "process.h"
#include "defs.h"
#include "page_dir.h"

extern std::mutex cpu_owner;         // cpu owner need to get this mutex
extern process *current_proc;        // current running proc / system service

extern size_t next_pid;              // next available proc pid 
extern page_dir *pgdir;              // current page dir
// extern user_stack * current_stack

void state_init();

