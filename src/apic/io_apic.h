/**
 * apic/io_apic.h
 * I/O APIC
 */

#pragma once

/**
 * class io_apic
 * Each instance of this class simulates an I/O APIC
 * and monitor interrupts from a certain external device
 */
class io_apic
{
public:
	io_apic ();
	~io_apic ();

private:
};

void init_io_apic ();
