/**
 * core/cpus.h
 * CPU Cores
 */

#pragma once

#include "core.h"
#include <vector>

extern std::vector < CPU_core > cores;   // List of CPU cores

/**
 * init_cpus ( int n )
 * Arguments
 * @n: the number of cpus
 */
void init_cpus ( int n );
