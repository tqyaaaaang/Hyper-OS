/**
 * interrupt/interrupt_id.cpp
 * Interrupt IDs
 */

#include "interrupt_id.h"

bool interrupt_id_is_signal ( interrupt_id_t id )
{
	return static_cast < int > ( id ) < 0;
}

bool interrupt_id_is_internal_exception ( interrupt_id_t id )
{
	return static_cast < int > ( id ) >= 0 && static_cast < int > ( id ) < 32;
}
