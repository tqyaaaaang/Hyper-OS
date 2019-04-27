/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "core/cpus.h"
#include "env/env.h"
#include "logging/logging.h"

int main ()
{
	init_logger ();
	init_cpus ( CPU_CORES );
}
