/**
 * env.cpp
 * Environment Variables
 */

#include "env.h"
#include <cstdlib>

// CPU Information
const int CPU_CORES = 1;   // Number of CPU cores

// Memory information
const size_t MEMORY_SIZE = ( size_t ) 1 << 30;   // The size of the physical memory

// Logging
const char *LOG_FILE_NAME = NULL;   // File name to write log, NULL if write to stdout

const int LOG_LEVEL = LOGGING_LEVEL::DEBUG;   // Logging level to be used
