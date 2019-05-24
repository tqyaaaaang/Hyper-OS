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

// Motherboard information
const int MOTHERBOARD_NUM = 1;   // The number of motherboards

// Logging
const char *LOG_FILE_NAME = "hos.log";   // File name to write log, NULL if write to stdout

const int LOG_LEVEL = LOGGING_LEVEL::DEBUG;   // Logging level to be used

// Unittest
const bool TEST = false;

// I/O Device
extern const int IO_DEVICE = 1;   // I/O Device method, 0 if directly to terminal, 1 if to module screen
