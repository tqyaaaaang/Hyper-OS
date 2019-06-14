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

// Message service
const char *MESSAGE_FILE_NAME = "msg.log";   // File name to write messages, write to stdout if is NULL and IO_DEVICE is 0

// Unittest
const bool TEST = false;

// I/O Device
extern const int IO_DEVICE = 1;   // I/O Device method, 0 if directly to terminal, 1 if to module screen

// Tick interval of clock device ( ms )
const int CLOCK_INTERVAL = 20;
