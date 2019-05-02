/**
 * init/init.cpp
 * Initialize the system
 */

#include "init.h"

#include "../core/cpus.h"
#include "../status/status.h"
#include "../env/env.h"
#include "../logging/logging.h"
#include "../apic/io_apic.h"
#include "../mm/pmm.h"

void init ()
{
	init_logger ();

	init_status_phase_1 ();

	init_cpus ( CPU_CORES );
	init_status_phase_2 ();
	init_io_apic ();

	init_pmm ( );
}
