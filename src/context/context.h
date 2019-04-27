/**
 * context/context.h
 * Context of the current thread
 */

#pragma once

#include "../core/core.h"

class context_t
{
public:
	void set_core ( CPU_core *core );

	CPU_core * get_core () const;

private:
	CPU_core *current_core;
};

extern thread_local context_t context;
