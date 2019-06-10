/**
 * io/special_keys.cpp
 * Special keys
 */

#include "special_keys.h"
#include "../logging/logging.h"

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



void handle_special_key ( special_keys key )
{
	switch ( key ) {
	default:
		logging::warning << "Processing special key <" << get_key_str ( key ) << "> not implemented" << logging::log_endl;
	}
}
