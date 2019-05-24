/**
 * dev/device_t.h
 * Devices base class
 */

#pragma once

#include <thread>
#include <string>
#include "../../src/status/status.h"

class motherboard_t;

class device_t
{
public:
	device_t ();
	device_t ( int __motherboard_id );   // initialize with motherboard ID
	device_t ( motherboard_t *_motherboard );   // initialize with a specific motherboard

	void set_id ( int _id );
	int get_id () const;

	void init_motherboard ();   // set motherboard using motherboard ID;

	void set_motherboard ( motherboard_t *_motherboard );
	motherboard_t * get_motherboard () const;

	virtual void init ();
	virtual void destroy ();

	virtual std::string to_string () const;

protected:
	void device_thread_entry ( status_t father_thread_status );   // device daemon thread entry
	virtual void device_thread_event_loop ();   // device daemon thread entry

	int id;   // device ID

	int _motherboard_id;
	motherboard_t *motherboard;   // motherboard on which the device is

	std::thread device_thread;   // device daemon thread
};
