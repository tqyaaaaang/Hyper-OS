/**
 * main.cpp
 * Main entrance to the Operating System
 */

#include "init/init.h"
#include "destroy/destroy.h"
#include "debug/debug.h"

int main ()
{
	init ();
	debug ();
	destroy ();
}
