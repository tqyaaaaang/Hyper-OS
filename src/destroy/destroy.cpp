/**
 * destroy/destroy.h
 * Call destructor
 */

#include "destroy.h"
#include "../mm/pmm.h"
#include "../core/cpus.h"
#include "../logging/logging.h"

void destroy ()
{
	destroy_pmm ();
	destroy_cpus ();

	destroy_logger ();
}
