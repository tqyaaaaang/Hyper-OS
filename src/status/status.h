/**
 * status/status.h
 * Context of the current thread
 */

#pragma once

#include <string>
#include "../core/core.h"

class status_t
{
public:
	status_t ();

	void set_core ( CPU_core *core );
	CPU_core * get_core () const;

	void set_name ( std::string _name );
	std::string get_name () const;

private:
	CPU_core *current_core;

	std::string name;
};

extern thread_local status_t status;

void init_status ();
