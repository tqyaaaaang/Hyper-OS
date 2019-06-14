/**
 * dev/devices/output/output_terminal.h
 * Device output
 * Output directly to teminal
 */

#pragma once

#include "output.h"
#include "../../../src/status/status.h"
#include <string>

class dev_output_terminal : public dev_output
{
public:
	dev_output_terminal ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual void write ( int ch );

private:
};
