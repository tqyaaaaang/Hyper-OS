/**
 * interrupt/interrupts/keyboard_interrupt.cpp
 * Interrupt classes
 * Keyboard interrupt
 */

#include "keyboard_interrupt.h"
#include "../../env/env.h"
#include "../../logging/logging.h"
#include "../../schedule/signal.h"
#include "../../io/special_keys.h"
#include "../../io/stdio.h"

keyboard_interrupt::keyboard_interrupt ( std::string _data )
	: external_interrupt_t ( interrupt_id_t::KEYBOARD_INTERRUPT )
	, data ( _data )
{
}

void keyboard_interrupt::process ()
{
	if ( !IO_DEVICE ) {
		logging::error << "Process keyboard interrupt with I/O Device directly connect to terminal" << logging::log_endl;
		return;
	}

	if ( data.size () == 1 ) {
		signal_data ( data[0] );
	} else {
		special_keys current_key = get_key_id ( data );
		int handle_flag = handle_special_key ( current_key );
		if ( handle_flag == -2 ) {
			signal_data ( -static_cast < int > ( current_key ) );
		} else if ( handle_flag >= 0 ) {
			signal_data ( handle_flag );
		}
	}
}

void keyboard_interrupt::signal_data ( int data )
{
	logging::debug << "Sending KEYBOARD signal with data " << data << " to process" << logging::log_endl;
	receive_keyboard ( data );
}
