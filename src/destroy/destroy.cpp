/**
 * destroy/destroy.h
 * Call destructor
 */

#include "destroy.h"
#include "../mm/pmm.h"
#include "../core/cpus.h"

void destroy ()
{
	destroy_pmm ();
	destory_cpus ();
	// destory_shutdown ();
}
