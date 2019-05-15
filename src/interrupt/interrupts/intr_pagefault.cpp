/*
 * #PF : page fault.
 * trigger when:
 * - access a page outside memory
 * - write a read-only page
 * - access a super-user-only page
 */

#include "intr_pagefault.h"
#include "../interrupt_id.h"
#include "../../logging/logging.h"
#include <string>

using std::string;

typedef intr_pagefault_t::error_info error_info;

error_info::error_info()
{
	present = write = super = 0;
}

error_info::error_info(size_t la, bool present, bool write, bool super)
{
	this->la = la;
	this->present = present;
	this->write = write;
	this->super = super;
}

error_info::error_info(size_t la, int info)
{
	this->la = la;
	if (info & intr_pagefault_t::E_PRESENT)
		present = true;
	if (info & intr_pagefault_t::E_WRITE)
		write = true;
	if (info & intr_pagefault_t::E_SUPER)
		super = true;
}

string error_info::to_string()
{
	string str;
	str += present ? string("P") : string("-");
	str += write   ? string("W") : string("-");
	str += super   ? string("S") : string("-");
	return str;
}

intr_pagefault_t::intr_pagefault_t(const error_info &info)
	: internal_interrupt_t ( interrupt_id_t::PGFLT )
{
	this->info = info;
}

void intr_pagefault_t::process()
{
	logging::info << "ISR of #PF started" << logging::log_endl;
	
	logging::info << "ISR of #PF finished" << logging::log_endl;
}

string intr_pagefault_t::to_string()
{
	return "#PF : " + info.to_string();
}

