/**
 * program/sys_t.h
 * syscall interface for user-mode programs
 */

class sys_t {
public:

	int create_process();
	int exec_program(int pid, const string &name);
	int yeild();
	int exit();
    int wait(int pid);
	int read();
	int write();
};
