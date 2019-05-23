/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "init/init.h"
#include "destroy/destroy.h"
#include "../test/test.h"
#include "env/env.h"

int main ()
{
	init ();
	if (TEST) {
		test ();
	}
	destroy ();
}
