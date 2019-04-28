/**
 * init/init.cpp
 * Initialize the system
 */

#include "init.h"

#include "../core/cpus.h"
#include "../status/status.h"
#include "../env/env.h"
#include "../logging/logging.h"
#include "../mm/pmm.h"

void init ()
{
	init_logger ();

	init_cpus ( CPU_CORES );
	init_status ();
	init_pmm ( );
}
