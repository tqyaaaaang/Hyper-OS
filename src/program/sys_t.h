/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */
#include <string>

class sys_t {
public:

	int create_process();
	int exec_program(int pid, const std::string &name);
	int yield();
	int exit();
    int wait(int pid);
	int read(int device);
	int write(int device, char data);
};
