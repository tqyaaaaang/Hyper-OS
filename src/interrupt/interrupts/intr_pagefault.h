/*
 * #PF : page fault.
 * trigger when:
 * - access a page outside memory
 * - write a read-only page
 * - access a super-user-only page
 */

#pragma once

#include "interrupt_t.h"

class intr_pagefault_t : public interrupt_t {
public:

	struct error_info {
	    size_t la;
		bool present;
		bool write;
		bool super;
		error_info();
	    error_info(size_t, bool, bool, bool);
	    error_info(size_t, int);
	    std::string to_string();
	};

	intr_pagefault_t(const error_info&);
	virtual void process();
	virtual std::string to_string();

	static const int E_PRESENT = 1;
	static const int E_WRITE = 2;
	static const int E_SUPER = 4;
	
private:

	error_info info;
};


