/*
 * Memory Infomations
 */
#pragma once

#include <cstdint>
#include <cstddef>

const size_t PAGE_SIZE = 1 << 12;
const size_t MEMORY_SIZE = (size_t)1 << 32;
const size_t PAGE_NUM = MEMORY_SIZE / PAGE_SIZE;
