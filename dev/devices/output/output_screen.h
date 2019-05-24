/**
 * dev/devices/output/output_screen.h
 * Device output
 * Output to module screen
 */

#pragma once

#include "output.h"
#include "../../../src/status/status.h"
#include <string>

class dev_output_screen : public dev_output
{
public:
	dev_output_screen ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual void write ( char ch );

private:
};
