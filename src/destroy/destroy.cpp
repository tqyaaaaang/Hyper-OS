/**
 * destroy/destroy.h
 * Call destructor
 */

#include "destroy.h"
#include "../mm/pmm.h"
#include "../core/cpus.h"
#include "../apic/io_apic.h"
#include "../logging/logging.h"

void destroy ()
{
	destroy_pmm ();
	destroy_io_apic ();
	destroy_cpus ();

	destroy_logger ();
}
