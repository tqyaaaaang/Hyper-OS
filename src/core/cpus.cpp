/**
 * core/cpus.cpp
 * CPU Cores
 */

#include "cpus.h"
#include "../logging/logging.h"
#include <cassert>

CPU_core *cores;   // List of CPU cores

static int core_num = 0;

int get_core_num ()
{
	assert(core_num != 0);
	return core_num;
}

void init_cpus ( int n )
{
	logging::info << "Initializing " << n << " CPU(s)" << logging::log_endl;
	core_num = n;
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
