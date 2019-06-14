#include "stdio.h"
#include "special_keys.h"
#include <vector>
#include "../../dev/device_list.h"
#include "../../dev/devices/output/output.h"
#include "../logging/logging.h"
#include "../schedule/signal.h"
#include "../message/message.h"
#include <iostream>
#include <cassert>

std::vector<int> buffer;
size_t ptr = 0;

using device_desc::standard_output;

static void new_line()
{
	logging::debug << "stdio receive new-line" << logging::log_endl;
	assert(ptr <= buffer.size());
	while (ptr < buffer.size()) {
		standard_output->write(-static_cast<int>(special_keys::RIGHT));
		ptr++;
	}
	standard_output->write('\n');
	for (int i : buffer) {
		logging::debug << "buffer is sending signal : " << i << logging::log_endl;
		send_signal(signal_id::KEYBOARD, i);
	}
	send_signal(signal_id::KEYBOARD, '\n');
	buffer.clear();
	ptr = 0;
}

static void back_space()
{
	logging::debug << "stdio receive back space" << logging::log_endl;
	if (ptr > 0) {
		standard_output->write('\b');
		ptr--;
		for (size_t i = ptr; i+1 != buffer.size(); i++) {
			buffer[i] = buffer[i + 1];
		}
		buffer.pop_back();
	} else {
		logging::debug << "backspace while the cursor is at the beginning of buffer" << logging::log_endl;
	}
}

static void normal_character(int data)
{
	standard_output->write(data);
	buffer.push_back(0);
	for (size_t i = buffer.size() - 1; i != ptr; i--) {
		buffer[i] = buffer[i-1];
	}
	buffer[ptr++] = data;
}

void move_left()
{
	if (ptr > 0) {
		ptr--;
		standard_output->write(-static_cast<int>(special_keys::LEFT));
	}
}

void move_right()
{
	if (ptr < buffer.size()) {
		ptr++;
		standard_output->write(-static_cast<int>(special_keys::RIGHT));
	}
}

void receive_keyboard(int data)
{
	switch(data) {
	case -static_cast<int>(special_keys::LEFT):
		move_left();
		break;
	case -static_cast<int>(special_keys::RIGHT):
		move_right();
		break;
	case '\n':
	    new_line();
		break;
	case '\b':
		back_space();
		break;
	default:
		normal_character(data);
		break;
	}
}
