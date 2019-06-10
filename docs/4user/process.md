# Process Scheduling

## Process

**Process** is the abstract of a running program and its runtime context. In Hyper OS, process is simulated using a thread and a condition variable (`process_t::cond_var`). The program, runtime context and the thread of the process stores in an object of `process_t`. 

Runtime context is stored and restored during process switch. Runtime context (`class context_t`) contains the page table base address and stack pointer of the process (see `src/context/context.cpp` and `src/schedule/schedule.cpp`).

## Process State

In Hyper OS, a process can be in state `UNINIT, RUNABLE, SLEEPING, ZOMBIE` (see `src/process/process_t.h`, `process_t::state`). 

Processes of same state will store in a list. Each CPU core owns three list of process, containing all the processes on the core in `UNINIT`, `RUNABLE`, `SLEEPING` state. All the CPU shares an `UNINIT` list and a `ZOMBIE` list.  

1. `UNINIT`: a process created by `create_process` is in state `UNINIT` initially, and it is inserted into global `UNINIT` list. `UNINIT` process is not assigned to any CPU until loading a program by `exec_program`. 
2. `RUnABLE`: after loading a program by `exec_program`, the process is assigned to a core, switches to state `RUNABLE` and is inserted into `RUNABLE` list of corresponding core. When it needs to reschedule (time slice of current process runs out, current process yields, etc), the scheduler (see section `Scheduler`) runs the first process on `RUNABLE` list.
3. `SLEEPING`: a process called `wait` (see section `System Calls About Process/wait`) will switch to state `SLEEPING` until the expected event occurs. 
4. `ZOMBIE`: after a process exit, it is deleted from `RUNABLE`  list and inserted into global `ZOMBIE` list. A zombie process will be destroyed totally by its parent. 

## Process Switch

Each CPU core has an mutex to prevent more than one process on the core running together. The running process acquires the mutex using `CPU_core::acquire()` and releases it using `CPU_core::release` (see `src/core/core.h`). 

There are three situation of process state switch we must think about. 

1. **Start**. When a process is loaded with a program using `exec_program`, the thread of it will start to run `proc_main` (see `src/process/process.cpp`) and then `process_t::exec` (see `src/process/process_t.cpp`), block itself on condition variable and set a `std::promise` atomicly to tell kernel the finish of initialization.
2. **Wait(Sleep)**. When a process calls syscall `wait`, the kernel will call `proc_wait` (see `src/process/process.cpp`) and then `sched_set_wait` or `sched_set_sleep`, which will delete the process from `RUNABLE` list and insert it into `SLEEPING` list. For more information see section `System Calls About Process`. 
3. **Exit**. When a process exits using system call `exit`, kernel will clean the process information and process will throw an integer which will be caught by `proc_main` in order to finish the thread. (see `src/process/process.cpp` `proc_main` and `proc_exit`). 

## Process and Interrupt

A big challenge in designing Hyper OS is the interrupt and system call. We will figure out two problems and the solution to them which may give you a better view of Hyper OS.

#### Interrupt Priority

There are two ways for a process to trap into an interrupt. The first way is system call and faults (In Hyper OS, only page fault). In these situations the process will call `interrupt` or `syscall` (see `src/interrupt/interrupt.cpp, src/syscall/syscall.cpp`) which will block the process and start ISR. 

The second way is external interrupt. In this case, local APIC will set an interrupt flag (see `src/core/core.cpp` `CPU_core::interrupt_waiting_flag`). When a process find the notice this during tail checking, it will block itself actively, release CPU and **give a signal to APIC** which leads to the start of ISR (see `check_interrupt()` in `src/interrupt/interrupt.cpp` and  `local_apic::try_process_interrupt` in `src/apic/local_apic.cpp`). 

Now think about the situation. When a process send a syscall interrupt to APIC, at the same time a clock interrupt (external interrupt) comes, which interrupt should be processed first? The correct answer is the syscall interrupt. In other words, internal interrupt is always more important than external interrupt. That is because if the external interrupt, such as clock interrupt is processed before the syscall interrupt, it may leads to a process switch. In the view of kernel, previous syscall is sent by current process, which is totally wrong because of process switch. 

#### Synchronization of Process and ISR

An another important problem is about synchronization. The process (or user mode program) and the ISR are running on different thread. So before the start of ISR, we must ensure that the user mode program is blocked correctly. We use an interesting trick to do this (see `local_apic::try_process_interrupt` , `local_apic::interrupt` and `interrupt_trap_entry` in `src/interrupt/trap.cpp`). 

```c++
// process, see local_apic::try_process_interrupt
std::unique_lock < std::mutex > lck ( status.get_core ()->get_current ()->cond_mutex );
status.get_core ()->release ();
send_process_interrupt_signal ();
status.get_core ()->get_current ()->cond_var.wait ( lck );
if (proc->get_prog() != nullptr
	&& proc->get_exit_flag()) {
	throw 0;
}
status.get_core ()->acquire ();
```

```c++
// ISR, see interrupt_trap_entry
process_t *cur = status.get_core()->get_current();
	
status.get_core()->acquire();	
status.get_core ()->inc_interrupt_depth ();

if (cur != nullptr) {
	//make sure current proc slept successfully
	cur->cond_mutex.lock();
	cur->cond_mutex.unlock();	    
} 
current_interrupt->process ();
trap_exit();
```

```sequence
note over process: lock cond_mutex
note over process: release CPU
process -> ISR : start ISR
note over ISR : acquire CPU
note over ISR : try lock cond_mutex
note over process: wait on cond_var, unlock cond_mutex atomicly
process -> ISR : process slept
note over ISR : lock cond_mutex
note over ISR : unlock cond_mutex
note over ISR : ISR processing
ISR -> scheduler : reschedule
note over scheduler : choose a process to run
scheduler -> process : signal cond_var
note over process : acquire CPU
scheduler -> process : release CPU
note over process : process run again
note over scheduler : ISR finish 
```

## Scheduler

In the end of a trap, ISR determines whether process switching is needed. If so, the scheduler will choose a runnable process and switch to it. 

Hyper OS uses RR (Round Robin) algorithm to choose the next process to run. 

## System Calls About Process

#### wait

#### yield

## Example : Shell



