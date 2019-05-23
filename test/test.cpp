/**
 * test/test.cpp
 * Unittest entry
 */

#include "test.h"
#include "test_message.h"
#include "test_interrupt.h"
#include "test_syscall.h"
#include "test_devices.h"
#include "test_pmm.h"
#include "test_pm.h"
#include "test_ffma.h"
#include "test_process.h"
#include "test_multiproc.h"
#include "test_program_manager.h"
#include "test_heap.h"
#include "../src/mm/pmm.h"
#include "../src/logging/logging.h"
#include "../src/utils/allocator/ffma.h"
#include "../src/program/program_manager.h"

void test()
{
	logging::info << "Testing the system" << logging::log_endl;

	test_message ();

	test_interrupt ();
	test_syscall ();

	test_devices ();

	test_pm();
	test_pmm();
    test_ffma();
	test_program_manager();
}
