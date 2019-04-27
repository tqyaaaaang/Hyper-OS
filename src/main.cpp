/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "core/cpus.h"
#include "env.h"

int main ()
{
	init_cpus ( CPU_CORES );
}
