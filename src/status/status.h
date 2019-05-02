/**
 * status/status.h
 * Context of the current thread
 */

#pragma once

#include <string>
#include "../mm/page_table.h"

class CPU_core;

class status_t
{
public:
	status_t ();

	void set_core ( CPU_core *core );
	CPU_core * get_core () const;

	void set_name ( std::string _name );
	std::string get_name () const;

	void set_page_table( page_table *pg );
	page_table * get_page_table() const;
	
private:
	CPU_core *current_core;

	page_table *pgt;
	
	std::string name;
};

extern thread_local status_t status;

void init_status_phase_1 ();
void init_status_phase_2 ();
