/**
 * test/test_message.h
 * Test message service
 */

#include "test_message.h"
#include "../src/message/message.h"
#include "../src/logging/logging.h"

void test_message ()
{
	logging::debug << "Testing message service" << logging::log_endl;

	message::test ( message::wrap_core_info ( "kern test" ) ) << "This is a test message" << message::msg_endl;
}
