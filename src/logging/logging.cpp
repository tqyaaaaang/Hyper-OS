/**
 * logging/logging.cpp
 * Logging module
 */

#include <iostream>
#include <fstream>
#include "logging.h"
#include "../env/env.h"

namespace logging
{
	thread_local std::stringstream BUF;
	std::ostream *OUT;
	std::mutex output_lock;

	logger critical;
	logger error;
	logger warning;
	logger info;
	logger debug;

	log_endl_t log_endl;
}

logging::logger::logger ()
	: visible ( false )
{
}

logging::logger::logger ( bool _visible )
	: visible ( _visible )
{
}

logging::logger::~logger ()
{
}

void init_logger ()
{
	if ( LOG_FILE_NAME == NULL ) {
		logging::OUT = &std::cout;
	} else {
		logging::OUT = new std::ofstream ( LOG_FILE_NAME );
	}

	logging::critical = logging::logger ( LOGGING_LEVEL::CRITICAL <= LOG_LEVEL );
	logging::error = logging::logger ( LOGGING_LEVEL::ERROR <= LOG_LEVEL );
	logging::warning = logging::logger ( LOGGING_LEVEL::WARNING <= LOG_LEVEL );
	logging::info = logging::logger ( LOGGING_LEVEL::INFO <= LOG_LEVEL );
	logging::debug = logging::logger ( LOGGING_LEVEL::DEBUG <= LOG_LEVEL );
}
