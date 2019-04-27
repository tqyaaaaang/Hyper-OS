/**
 * apic/local_apic.h
 * Local APIC
 */

#pragma once

/**
 * class local_apic
 * Each instance of this class simulates an Local APIC
 * and send interrupts to a certain CPU core
 */
class local_apic
{
public:
	local_apic ();
	~local_apic ();

private:
};