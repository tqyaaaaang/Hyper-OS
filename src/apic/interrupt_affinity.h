/**
 * apic/interrupt_affinity.h
 * Affinity of interrupts
 */

#pragma once

#include <map>
#include <mutex>
#include "../interrupt/interrupt_id.h"

namespace interrupt_affinity_namespace
{
	extern std::map < interrupt_id_t, int > interrupt_affinity;
	extern const int DEFAULT;
	extern std::mutex mut;
}

void init_interrupt_affinity ();

void set_interrupt_affinity ( interrupt_id_t interrupt_id, int core_id );
int get_interrupt_affinity ( interrupt_id_t interrupt_id );
