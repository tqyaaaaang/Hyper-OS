/**
 * dev/devices/screen/screen
 * Device screen
 * Low-level interact methods with screen
 */

#pragma once

#include "../device_t.h"
#include "../../../src/status/status.h"
#include <string>
#include <mutex>

class dev_screen : public device_t
{
public:
	dev_screen ( int __motherboard_id );

	void init ();
	void destroy ();

	virtual std::string to_string () const;

	virtual int read ();
	virtual void write ( std::string type, std::string data );
	virtual void write ( std::string type, std::string config, std::string data );

	virtual void write_char ( char ch );
	virtual void backspace ();
	virtual void newline ();
	virtual void cursor_move_left ();
	virtual void cursor_move_right ();

private:
	virtual void device_thread_event_loop ();

	std::mutex write_lock;
};
