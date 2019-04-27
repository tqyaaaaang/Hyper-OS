/**
 * core/cpus.cpp
 * CPU Cores
 */

#include "cpus.h"
#include "../logging/logging.h"

std::vector < CPU_core > cores;   // List of CPU cores

void init_cpus ( int n )
{
	logging::info << "Initialize " << n << " CPU(s)" << logging::log_endl;
	cores.resize ( n );
	for ( int i = 0; i < static_cast < int > ( cores.size () ); ++i ){
		logging::debug << "Initialize CPU #" << i << logging::log_endl;
		cores[i].enable ();
		cores[i].enable_interrupt ();
	}
}
