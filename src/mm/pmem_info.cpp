/*
 * Memory Infomations
 */
#pragma once

#include "pmem_info.h"
#include "../env/env.h"

const size_t PAGE_SIZE = 1 << 12;
const size_t PAGE_NUM = MEMORY_SIZE / PAGE_SIZE;
