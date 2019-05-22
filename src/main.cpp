/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "init/init.h"
#include "destroy/destroy.h"
#include "../test/test.h"

int main ()
{
	init ();
	// test ();
	destroy ();
}
