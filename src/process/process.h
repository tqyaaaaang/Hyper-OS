/**
 * process/process.h
 * operations to processes
 */

#pragma once

#include "process_t.h"

int proc_create_process();
int proc_exec_program(int pid, program *prog);
int proc_yield();
int proc_wait(int pid);
int proc_exit();

void init_proc();
void destroy_proc();
