/**
 * apic/interrupt_affinity.cpp
 * Affinity of interrupts
 */

#include "interrupt_affinity.h"
#include "../logging/logging.h"

namespace interrupt_affinity_namespace
{
	std::map < interrupt_id_t, int > interrupt_affinity;
	const int DEFAULT = 0;
	std::mutex mut;
}

using namespace interrupt_affinity_namespace;

void init_interrupt_affinity ()
{
	logging::debug << "Initializing interrupt affinity information" << logging::log_endl;
}

void set_interrupt_affinity ( interrupt_id_t interrupt_id, int core_id )
{
	std::unique_lock < std::mutex > lock ( mut );
	logging::info << "Setting the affinity of interrupt " << static_cast < int > ( interrupt_id ) << " to CPU core #" << core_id << logging::log_endl;
	interrupt_affinity[interrupt_id] = core_id;
}

int get_interrupt_affinity ( interrupt_id_t interrupt_id )
{
	std::unique_lock < std::mutex > lock ( mut );
	logging::debug << "Trying to get interrupt affinity of id " << static_cast < int > ( interrupt_id ) << logging::log_endl;
	std::map < interrupt_id_t, int > ::iterator result = interrupt_affinity.find ( interrupt_id );
	if ( result != interrupt_affinity.end () ) {
		logging::debug << "Find result, assigned to CPU core #" << result->second << logging::log_endl;
		return result->second;
	} else {
		logging::debug << "Result not found, assigned to default CPU core #" << DEFAULT << logging::log_endl;
		return DEFAULT;
	}
}
