/**
 * schedule/schedule.h
 * schedule processes and cpus
 */

#pragma once
#include "../process/process_t.h"

void sched_init_proc(process_t *proc);
void sched_set_core(process_t *proc);
void sched_set_runable(process_t *proc);
void sched_kill_process(process_t *proc);

void schedule(int core_id);

void init_schedule();
void destroy_schedule();
