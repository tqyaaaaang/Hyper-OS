/**
 * schedule/signal.h
 * signals to wake up sleeping process
 */
#include <queue>
#include <mutex>

class process_t;

class signal_t {

public:

	signal_t();
	~signal_t();
	void notify(size_t data);
	void wait(process_t *proc);
	
private:

	std::mutex mut;
	std::queue<process_t*> proc;
	std::queue<size_t> que;
};

namespace signal_id {
	extern const int WAIT_EXIT;
	extern const int KEYBOARD;
}

void init_signal ();
void destroy_signal ();

/**
 * send/wait on a signal
 * @return : 0 ok
 *          -1 not valid signal id
 */ 
int send_signal(int signal_id, size_t data);
int wait_signal(int signal_id, process_t *proc);

int register_signal();

