/**
 * core/cpus.cpp
 * CPU Cores
 */

#include "cpus.h"
#include "../logging/logging.h"

CPU_core *cores;   // List of CPU cores

void init_cpus ( int n )
{
	logging::info << "Initializing " << n << " CPU(s)" << logging::log_endl;
	cores = new CPU_core[n];
	for ( int i = 0; i < n; ++i ){
		logging::debug << "Initializing CPU #" << i << logging::log_endl;
		cores[i].set_core_id ( i );
		cores[i].enable ();
		cores[i].enable_interrupt ();
	}
}

void destroy_cpus ()
{
	logging::debug << "Destorying CPUs" << logging::log_endl;

	delete[] cores;
}
