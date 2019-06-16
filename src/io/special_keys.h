/**
 * io/special_keys.h
 * Special keys
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

enum class special_keys
{
	ESCAPE = 1,
	CTRL_A,
	CTRL_B,
	CTRL_C,
	CTRL_D,
	CTRL_E,
	CTRL_F,
	CTRL_G,
	CTRL_H,
	CTRL_I,
	CTRL_J,
	CTRL_K,
	CTRL_L,
	CTRL_M,
	CTRL_N,
	CTRL_O,
	CTRL_P,
	CTRL_Q,
	CTRL_R,
	CTRL_S,
	CTRL_T,
	CTRL_U,
	CTRL_V,
	CTRL_W,
	CTRL_X,
	CTRL_Y,
	CTRL_Z,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	CTRL_SPACE,
	CTRL_BACKSLASH,
	CTRL_R_SQR_BRAC,
	CTRL_CARET,
	CTRL_UNDERSCORE,
	BACKSPACE,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	S_UP,
	S_DOWN,
	S_RIGHT,
	S_LEFT,
	HOME,
	END,
	DELETE,
	S_DELETE,
	CTRL_DELETE,
	PAGE_UP,
	PAGE_DOWN,
	S_TAB,
	TAB,
	CTRL_LEFT,
	CTRL_RIGHT,
	CTRL_UP,
	CTRL_DOWN,
	INSERT
};

extern std::vector < std::string > special_keys_list;
extern std::unordered_map < std::string, special_keys > special_keys_map;

void init_special_keys ();

std::string get_key_str ( special_keys key );
special_keys get_key_id ( std::string str );

/**
 * @return: whether to pass the key to waiting process
 *          -2: pass as special_key ( like LEFT and RIGHT )
 *          -1: not pass ( like CTRL_C )
 *          >=0: pass as character ( like ENTER and BACKSPACE )
 */
int handle_special_key ( special_keys key );
