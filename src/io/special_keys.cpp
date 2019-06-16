/**
 * io/special_keys.cpp
 * Special keys
 */

#include "special_keys.h"
#include "../schedule/signal.h"
#include "../logging/logging.h"
#include "../process/process.h"
#include "../message/message.h"

std::vector < std::string > special_keys_list = {
	"",
	"escape",
	"c-a",
	"c-b",
	"c-c",
	"c-d",
	"c-e",
	"c-f",
	"c-g",
	"c-h",
	"c-i",
	"c-j",
	"c-k",
	"c-l",
	"c-m",
	"c-n",
	"c-o",
	"c-p",
	"c-q",
	"c-r",
	"c-s",
	"c-t",
	"c-u",
	"c-v",
	"c-w",
	"c-x",
	"c-y",
	"c-z",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"f11",
	"f12",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"c-@",
	"c-\\",
	"c-]",
	"c-^",
	"c-_",
	"backspace",
	"up",
	"down",
	"right",
	"left",
	"s-up",
	"s-down",
	"s-right",
	"s-left",
	"home",
	"end",
	"delete",
	"s-delete",
	"c-delete",
	"pageup",
	"pagedown",
	"s-tab",
	"tab",
	"c-left",
	"c-right",
	"c-up",
	"c-down",
	"insert"
};

std::unordered_map < std::string, special_keys > special_keys_map;



void init_special_keys ()
{
	logging::debug << "Initializing special keys" << logging::log_endl;
	int key_num = special_keys_list.size () - 1;
	for ( int i = 1; i <= key_num; ++i ) {
		special_keys_map[special_keys_list[i]] = static_cast < special_keys > ( i );
	}
}



std::string get_key_str ( special_keys key )
{
	return special_keys_list[static_cast < int > ( key )];
}

special_keys get_key_id ( std::string str )
{
	return special_keys_map[str];
}



/**
* @return: whether to pass the key to waiting process
*          -2: pass as special_key ( like LEFT and RIGHT )
*          -1: not pass ( like CTRL_C )
*          >=0: pass as character ( like ENTER and BACKSPACE )
*/
int handle_special_key ( special_keys key )
{
	logging::debug << "Find special key <" << get_key_str ( key ) << ">, handling" << logging::log_endl;
	int return_value = false;
	switch ( key ) {
	case special_keys::LEFT:
	case special_keys::RIGHT:
		logging::debug << "Get <arrow-left> or <arrow-right>, direct pass the special key" << logging::log_endl;
		return_value = -2;
		break;

	case special_keys::CTRL_M:
		logging::debug << "Get <c-m>, transform to \\n" << logging::log_endl;
		return_value = '\n';
		break;

	case special_keys::BACKSPACE:   // Alias of <c-h>
	case special_keys::CTRL_H:
		logging::debug << "Get <backspace>, transform to \\b" << logging::log_endl;
		return_value = '\b';
		break;

	case special_keys::CTRL_C:
		logging::debug << "Get <c-c>, transform to -1" << logging::log_endl;
		message::process("kern kbd trap") << "<C-c> Received, try to exit current process" << message::msg_endl;
		send_signal(signal_id::KEYBOARD, -static_cast<int>(special_keys::CTRL_C));
		return_value = -1;
		break;
		
	default:
		logging::warning << "Processing special key <" << get_key_str ( key ) << "> not implemented" << logging::log_endl;
		return_value = -2;
	}
	return return_value;
}
