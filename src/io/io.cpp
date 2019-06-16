/**
 * io/io.cpp
 * Handle input and output
 */

#include "io.h"
#include "special_keys.h"
#include "../logging/logging.h"

void init_io ()
{
	logging::debug << "Initializing I/O service" << logging::log_endl;

	init_special_keys ();
}
