/**
 * dev/devices/input/input_terminal.h
 * Device input
 * Input directly from teminal
 */

#pragma once

#include "input.h"
#include "../../../src/status/status.h"
#include <string>

class dev_input_terminal : public dev_input
{
public:
	dev_input_terminal ( int __motherboard_id );

	virtual void init ();
	virtual void destroy ();

	virtual int read ();

private:
};
