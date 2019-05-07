/**
 * process/process_t.h
 * basic information of process_t 
 */

#pragma once

#include "process_t.h"

size_t create_process();
void exec_process(size_t pid, program *prog);

