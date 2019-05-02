/**
 * destroy/destroy.h
 * Call destructor
 */

#include "destroy.h"
#include "../mm/pmm.h"
#include "../core/cpus.h"
#include "../motherboard/motherboard.h"
#include "../logging/logging.h"
#include "../../dev/device.h"

void destroy ()
{
	destroy_pmm ();

	destroy_devices ();
	destroy_motherboards ();
	destroy_cpus ();

	destroy_logger ();
}
