/**
* dev/device.cpp
* Devices
*/

#include "device.h"
#include "device_list.h"
#include "../src/motherboard/motherboard.h"
#include "../src/logging/logging.h"

void init_devices ()
{
	logging::info << "Initializing " << device_list.size () << " device(s)" << logging::log_endl;
	for ( int i = 0; i < static_cast < int > ( device_list.size () ); ++i ) {
		logging::info << "Initializing device #" << i << " : " << device_list[i]->to_string () << ", on motherboard #" << device_list[i]->get_motherboard ()->get_id () << logging::log_endl;
		device_list[i]->init_motherboard ();
		device_list[i]->set_id ( i );
		device_list[i]->init ();
	}
}

void destroy_devices ()
{
	logging::info << "Destroying device(s)" << logging::log_endl;
	for ( int i = 0; i < static_cast < int > ( device_list.size () ); ++i ) {
		device_list[i]->destroy ();
		delete device_list[i];
	}
}
