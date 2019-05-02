/**
 * motherboard/motherboard.h
 * Motherboard
 */

#pragma once

#include "../apic/io_apic.h"

class motherboard_t
{
public:
	motherboard_t ();
	~motherboard_t ();

	void set_id ( int _id );
	int get_id () const;

	io_apic & get_io_apic ();

private:
	io_apic ioapic;
	int id;
};

extern motherboard_t *motherboards;   // List of motherboards

/**
 * init_motherboards ( int n )
 * Arguments
 * @n: the number of motherboards
 */
void init_motherboards ( int n );

/**
 * destroy_motherboards ()
 */
void destroy_motherboards ();
