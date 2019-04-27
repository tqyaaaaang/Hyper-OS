/**
 * core/cpus.cpp
 * CPU Cores
 */

#include "cpus.h"

std::vector < CPU_core > cores;   // List of CPU cores

void init_cpus ( int n )
{
	cores.resize ( n );
	for ( auto &x : cores ) {
		x.enable ();
		x.enable_interrupt ();
	}
}
