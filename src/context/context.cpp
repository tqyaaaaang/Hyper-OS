/**
 * context/context.h
 * Context of the current thread
 */

#include "context.h"
#include "../core/cpus.h"
#include "../logging/logging.h"

void context_t::set_core ( CPU_core * core )
{
	current_core = core;
}

CPU_core * context_t::get_core () const
{
	return current_core;
}

thread_local context_t context;

void init_context ()
{
	logging::info << "Set initial context" << logging::log_endl;
	context.set_core ( &cores[0] );
}
