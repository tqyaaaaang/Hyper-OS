/**
 * env.cpp
 * Environment Variables
 */

#include "env.h"
#include <cstdlib>

// CPU Information
const int CPU_CORES = 1;   // Number of CPU cores

// Logging
const char *LOG_FILE_NAME = NULL;   // File name to write log, NULL if write to stdout

const int LOG_LEVEL = LOGGING_LEVEL::DEBUG;   // Logging level to be used
