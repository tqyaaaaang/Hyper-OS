/**
 * apic/io_apic.cpp
 * I/O APIC
 */

#include "io_apic.h"
#include "interrupt_affinity.h"
#include "../logging/logging.h"

io_apic::io_apic ()
{
}

io_apic::~io_apic ()
{
}



void init_io_apic ()
{
	logging::debug << "Initializing I/O APIC" << logging::log_endl;
	init_interrupt_affinity ();
}