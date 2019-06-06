# Interface for User-Mode Programs

## Program Class

User-Mode Program is supported in Hyper OS in a special way. Each program is a inheritance class of abstract class `program`(see `/src/program/program.h`). You should implement two virtual function: `main()` and `static_init()`.

1. `void static_init()` is called when the program is 'compiled', and in Hyper OS it is called when it's cloned by `program manager`. In `static_init()` you can allocate memory in `.bss` or `.data`(see section `Process Memory Layout`), modify the data in `.data` using `handle<T>::modify_in_compile(T)`(see section `Handle`), or set name for program.

   ```c++
   // Example: /user/shell/shell.cpp
   void hyper_shell::static_init()
   {
   	set_name("shell");
       // set name for program
   	argc = alloc_bss<int>();
       // alloc a integar in .bss
   	for (size_t i = 0; i < 20; i++)
   		argv[i] = alloc_bss<char>(30);
       // alloc 20 string in .bss
   }
   ```
   It is not allowed to allocate space in stack/heap, because they don't exist until being loaded into process.

2. `void main()` is the main function of program. When the program is load into a process by system call `exec_program` (see section `System Call and Standard Library`) or kernel, the function is called after initialization. You can allocate space in heap or stack, do some works and exit using either `return` or system call `exit`.

   ```c++
   // Example: /user/elephant/elephant.h
   void elephant::main()
   {
   	hos_std->println(graph);
       // print a string using standard library function
   }
   ```

Just like real operating systems, Hyper OS provides process as runtime environment for user-mode programs, which makes it possible for multi programs to work together independently.

## Process Memory Layout 

A process of Hyper OS has its own page table and `2G` independent linear address space(the size of linear address space is defined in `src/mm/pmem_info.cpp`). To simplify the problem, program in Hyper OS use very simple memory layout. Linear address space are divided into four sections, `.text, .data, .bss, .stack`, from `0x0` to `0x7fffffff`. 

- As program code is not actually in `.text`, the size of `.text` is always 0. 
- Static data of program (allocated by `handle::alloc_static`) will stores in `.data`. Static data is saved in `char *program::data`, and the size of `.data` is computed after `static_init()` finished. When the program is loaded into process, kernel allocate pages for `.data` and copy data of `char *program::data` into memory. 
- Space in `.bss` is allocated during `static_init()` using `handle::alloc_bss`. When the program is loaded, kernel allocate pages for `.bss` and set to 0.
- `.stack` containing the **stack and heap** for program in order to support dynamic memory allocation. Stack starts at `0x7fffffff` and grows towards decreasing addresses; heap starts at `data_size + bss_size` and grows towards increasing addresses. Stack or heap overflow is undefined behaviour.

## Handle

**Handle** is a very important concept in Hyper OS program interface. In computer systems, external interrupt can break the process and trap into kernel. However in Hyper OS, since we can not break a running program (actually a running thread), we need to 'tell' the program to stop when there is an interrupt. Handle is the key to do this. 

`handle` is a template class. Objects of `handle<T>` is something like a pointer of type `T`, containing a linear address and some other information. As long as the program accesses the memory only by `handle`, we can easily stop the process when the interrupt flag is `true` after some memory access operation.

### Functions

The functions and operators about `handle` are listed below.

1. `handle<T>()`: default construction function, construct a null `handle<T>`.

2. `handle<T>(const T &val)`: construct a `handle<T>` on **stack** have value `@val`. Calling the function in compile time is undefined behaviour.

3. `handle<T>(const handle<T> &val)`: clone a handle `@val`. This function is totally different from the function above. 

   ```c++
   handle<int> new_handle = old_handle;
   ```

   means construct a new handle pointed to the same address of `old_handle`. If you want to allocate space in stack and clone the data in `old_handle`, use:
   ```c++
   handle<int> new_handle = (int)old_handle;
   ```

4. `handle<T> handle<T>::operator = (const handle<T> &val) `: clone a handle `@val`. 

5. `handle<T> handle<T> operator = (const T &val)`: copy the data `@val` to the address `@this` points to. 

   ```
   handle<int> a;
   a = 1;
   // wrong : a is null handle
   handle<int> b = 0;
   b = 1;
   // ok : b is allocated in stack
   handle<int> c;
   c = alloc_heap<int>();
   // ok : c is allocated in heap
   c = alloc_heap<int>();
   // bug : c is allocated again and the previous object in heap is lost 
   ```

6. `handle<T> handle<T>::operator [] (size_t id)`: if `@this` points to `p`, `@return` is a handle pointing to `p+sizeof(T)`. About continuous memory allocation, see section `Memory Allocation`. 

   ```
   handle<int> array = alloc_heap<int>(10);
   array[0] = 1;
   array[1] = 1;
   ```

7. `handle<T>::~handle<T>()`: destructor. Free the object if `@this` is owner of a **stack** object (definition of **own** is in section `Memory Allocation`).

### Tail Check

After any memory access operation, the handle will do **tail check**. If the interrupt flag is `true`, in other words, there is an external interrupt, the program will stop itself, release CPU access and notify the LAPIC atomicly. After that, ISR for external interrupt (actually a thread) starts processing.

```sequence
APIC -> process 1: set interrupt flag 
note over process 1: tail check
process 1 -> APIC : stop, release CPU and notify APIC
APIC -> ISR : create ISR thread
note over ISR: acquire CPU and start processing
```

### Add Type Support

Handle of type `int, char, long long, size_t, double and handle<int>` is supported. To support type `T`, you need to add

```c++
INST(T)
```

 in the end of `src/program/alloc.cpp` and add 

```c++
template class handle<T>;
```

at the beginning of `src/program/program.cpp`.

## Memory Allocation

We say a handle **own** an object if the handle points to it using

```c++
handle<T> this_handle = alloc_*();
```

In other words, the first handle to control the object gets the ownership, and other handles points to the same object (via `operator =` , `operator []`, etc) are only considered to be **alias** of this handle. A handle may own an object on static area (`.bss` or `.data`), stack or heap. Handles owning stack objects will pop the stack during destruction.

### Allocate Function

Four kinds of memory allocation is supported in Hyper OS. 

1. `alloc_static<T>()`: **during `static_init` only.** Allocate an object of type T in `.data` and return the handle. 

   `alloc_static<T>(int n)`: **during `static_init` only.** Allocate n objects sequentially in `.data` and return the handle of **first object**. You can get handles of the kth object using `handle<int>::operator [] (int n)` .

2. `alloc_bss<T>()`: **during `static_init` only.** Allocate an object of type T in `.bss` and return the handle. When the program is loaded, the data in `.bss` will be set to 0 by kernel.

   `alloc_bss<T>(int n)`: **during `static_init` only.** Allocate n objects sequentially in `.data` and return the handle of first object. 

3. `alloc_stack<T>()`: **during running only. **Allocate an object of type T in stack and return the handle.

4. `alloc_heap<T>()`: **during running only**. Allocate an object of type T in heap and return the handle.

   `alloc_heap<T>(int n)`: **during running only**. Allocate n objects sequentially in heap and return the handle of first object. The first handle becomes owner of the whole array.

   `free_heap<T>(const handle<T> &ptr)`: **during running only**. Free the memory that `@ptr` points to. If `ptr` is the owner of an array, free the whole array. Free handles of non-heap objects is undefined behaviour.

### Dynamic Memory Allocation

The implement of `malloc, free` is in `src/process/proc_dmm.cpp`. They allocate and free memory in heap at the command of `process_t::heap_allocator`. 

`utils/allocator` contains memory allocation algorithms. 

## Program Manager

In order to run a program in Hyper OS, you need to register the program in `program manager`. There are three steps.

1. Write a generate function like

   ```c++
   static program* gen()
   { return new hyper_shell; }
   ```

2. Include `src/program/program_manager.h` and write a register function like

   ```c++
   void register_shell()
   {
   	register_program("name", gen);
   }
   ```

3. Call register function in `init_program_manager` (in `src/program/program_mamnager.h`)

   ```c++
   void init_program_manager()
   {
   	register_shell();
   }
   ```

Program `lp` shows all the user-mode programs in Hyper OS.

```bash
hyper-shell:$ lp
shell lp elephant matrix demo-syscall demo-pf demo-proc
hyper-shell:$ 
```

## System Call and Standard Library

### System Call

In Hyper OS, user-mode programs can get service of kernel by system call. System call functions are encapsulated into `class sys_t` (see `src/program/sys_t.h` and `user/hyperstd.h`). User-mode programs call these functions via member object `sys_t *sys` of `class program`.  

Supported system calls are listed below.

1. `int create_process()`: create a process and return its process id. The new process becomes child process of current process. The new process is an `UNINIT` process until its father calls `exec_program`.
2. `int exec_program(int pid, handle<char> name)`: execute program `@name` in process `@pid`. Process `@pid` must be child of current process. `@name` should be a valid string, which means it is an array of `char` ended with `\0`. 
3. `int yield()`: yield CPU access. 
4. `int exit()`: exit process.
5. `int wait(int pid)`: block current process until child process `@pid` exit.
6. `int read(dev_input *device)`: read a byte from input device. User programs can get pointer of standard input device by `sys_t::std_input()`.
7. `int write(dev_output *device, char data)`: write a byte to output device. User programs can get pointer of standard output device by `sys_t::std_output()`.
8. `int pid()`: get process id of current process.

### Support New System Call

You need two steps to support a new system call.

1. Add new syscall id in `src/syscall/syscall_id.h`

2. Add new syscall in `src/syscall/syscalls/`, you need to create a inheritance class of `class syscall_t`,  initialize syscall id and create syscall process function. 

   ```c++
   // Example : pid() syscall
   
   sys_pid::sys_pid()
   	:syscall_t(syscall_id_t::PID)
   {
   }
   
   int sys_pid::process()
   {
   	int res = status.get_core()->get_current()->get_pid();
   	logging::debug << "get pid : " << res << logging::log_endl;
   	return res;
   }
   ```

3. Add syscall function in `sys_t`. Add declaration in `src/program/sys_t.h` and implement it in `src/program/sys_t.cpp`. You need to send an syscall interrupt to APIC using interface `syscall` or `sys_t::intr`. 

   ```c++
   // Example : pid() syscall
   int sys_t::pid()
   {
   	return intr(new sys_pid);
   }
   ```

### Standard Library

User mode programs of Hyper OS can also get service of standard library. Standard library functions are encapsulated into `class hos_std_t` (see `src/program/lib.h`). 

