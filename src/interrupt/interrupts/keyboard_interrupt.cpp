/**
 * interrupt/interrupts/keyboard_interrupt.cpp
 * Interrupt classes
 * Keyboard interrupt
 */

#include "keyboard_interrupt.h"
#include "../../env/env.h"
#include "../../logging/logging.h"

keyboard_interrupt::keyboard_interrupt ()
	: external_interrupt_t ( interrupt_id_t::KEYBOARD_INTERRUPT )
{
}

void keyboard_interrupt::process ()
{
	if ( !IO_DEVICE ) {
		logging::error << "Process keyboard interrupt with I/O Device directly connect to terminal" << logging::log_endl;
		return;
	}
}
