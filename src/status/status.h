/**
 * status/status.h
 * Context of the current thread
 */

#pragma once

#include "../core/core.h"

class status_t
{
public:
	status_t ();

	void set_core ( CPU_core *core );

	CPU_core * get_core () const;

private:
	CPU_core *current_core;
};

extern thread_local status_t status;

void init_status ();
