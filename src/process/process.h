/**
 * process/process.h
 * operations to processes
 */

#pragma once

#include "process_t.h"

size_t create_process();
int exec_program(size_t pid, program *prog);

void init_proc();
void destroy_proc();
