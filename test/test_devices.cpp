/**
 * test/test_devices.cpp
 * Test devices
 */

#include "test_devices.h"
#include <string>
#include "../src/logging/logging.h"
#include "../dev/device_list.h"

void test_devices ()
{
	logging::debug << "Testing devices" << logging::log_endl;

	test_output_device ();
}

void test_output_device ()
{
	logging::debug << "Testing output device" << logging::log_endl;

	std::string str = "output device ok.\n";
	for ( auto x : str ) {
		device_desc::standard_output->write ( x );
	}
	device_desc::standard_output->write ( str );

	device_desc::standard_output->write ( "Please input a character: " );
	char ch = device_desc::standard_input->read ();
	device_desc::standard_output->write ( "input read : " );
	device_desc::standard_output->write ( ch );
	device_desc::standard_output->write ( '\n' );

	logging::debug << "Test on output device succeeded" << logging::log_endl;
}