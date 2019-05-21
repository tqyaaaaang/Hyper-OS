/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */

class sys_t {
public:

	int create_process();
	int exec_program(size_t pid, const string &name);
	int yeild();
	int exit();
    
};
