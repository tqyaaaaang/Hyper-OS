/**
 * process/process.h
 * operations to processes
 */

#pragma once

#include "process_t.h"

size_t create_process();
int exec_program(size_t pid, program *prog);
int proc_yield();
int proc_wait(size_t pid);
int proc_exit();

void init_proc();
void destroy_proc();
