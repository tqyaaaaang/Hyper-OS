/**
 * context/context.h
 * Context of the current thread
 */

#include "context.h"

void context_t::set_core ( CPU_core * core )
{
	current_core = core;
}

CPU_core * context_t::get_core () const
{
	return current_core;
}

thread_local context_t context;
