#include "stdio.h"
#include "special_keys.h"
#include <vector>
#include "../../dev/device_list.h"
#include "../../dev/devices/output/output.h"
#include "../logging/logging.h"
#include "../schedule/signal.h"
#include <iostream>

std::vector<int> buffer;
using device_desc::standard_output;

void receive_keyboard(int data)
{
	switch(data) {
	case '\n':
		standard_output->write(data);
		for (auto i : buffer) {
			logging::debug << "buffer is sending signal : " << i << logging::log_endl;
			send_signal(signal_id::KEYBOARD, i);
		}
		send_signal(signal_id::KEYBOARD, '\n');
		buffer.clear();
		break;
	case '\b':
		standard_output->write(data);
		if (!buffer.empty()) {
			buffer.pop_back();
		} else {
			logging::debug << "backspace when the buffer is empty" << logging::log_endl;
		}
		break;
	default:
		standard_output->write(data);
		buffer.push_back(data);
		break;
	}
}
