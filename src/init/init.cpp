/**
 * init/init.cpp
 * Initialize the system
 */

#include "init.h"

#include "../core/cpus.h"
#include "../status/status.h"
#include "../env/env.h"
#include "../logging/logging.h"
#include "../message/message.h"
#include "../motherboard/motherboard.h"
#include "../apic/io_apic.h"
#include "../mm/pmm.h"
#include "../../dev/device.h"
#include "../process/process.h"
#include "../program/program_manager.h"

void init ()
{
	init_logger ();

	init_message ();

	init_status_phase_1 ();

	init_cpus ( CPU_CORES );
	init_status_phase_2 ();
	init_motherboards ( MOTHERBOARD_NUM );
	init_io_apic ();

	init_devices ();

	init_pmm ( );

	init_program_manager( );

	init_proc();
}
