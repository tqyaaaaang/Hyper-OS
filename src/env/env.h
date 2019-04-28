/**
 * env.h
 * Environment Variables
 */
#pragma once

// CPU Information
extern const int CPU_CORES;   // Number of CPU cores

// Logging
extern const char *LOG_FILE_NAME;   // File name to write log, NULL if write to stdout

enum LOGGING_LEVEL   // Logging levels
{
	NONE,
	CRITICAL,
	ERROR,
	WARNING,
	INFO,
	DEBUG
};
extern const int LOG_LEVEL;   // Logging level to be used
