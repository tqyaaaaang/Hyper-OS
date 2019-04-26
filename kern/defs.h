#pragma once

#include <cstddef>

/* system proc id */

const size_t null_proc = 0;
const size_t mmu_proc = 1;
const size_t clock_proc = 2;
const size_t intr_proc = 3;
const size_t user_proc_start = 32;

/* process */

const int process_sleep = 0;
const int process_runable = 1;
const int process_zombie = 2;

/* page */

const size_t page_size = 1 << 12;

/* virtual memory */

const size_t vm_size = 1u << 31;

/* interrupt */

const size_t clock_intr = 0;
const size_t page_fault = 1;

