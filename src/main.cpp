/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "core/cpus.h"
#include "context/context.h"
#include "env/env.h"
#include "logging/logging.h"
#include "mm/pmm.h"

int main ()
{
	init_logger ();
	init_cpus ( CPU_CORES );
	init_context ();
	pmm_init ( );
	pmm_debug ( );
	pmm_shutdown ( );
}
