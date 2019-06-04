# Interface for User-Mode Programs

## Program Class

User-Mode Program is supported in Hyper OS in a special way. Every program is a inheritance class of abstract class `program`(see `/src/program/program.h`). You should implement two virtual function: `main()` and `static_init()`.

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

2. `void main()` is the main function of program. When the program is load into a process by system call `exec_program` (see section `System Call`) or kernel, the function is called after initialization. You can allocate space in heap or stack, do some works and exit using either `return` or system call `exit`.

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
- Memory of `.bss` is allocated during `static_init()` using `handle::alloc_bss`. When the program is loaded, kernel allocate pages for `.bss` and set to 0.
- `.stack` containing the stack and heap for program in order to support dynamic memory allocation. Stack starts at `0x7fffffff` and grows towards decreasing addresses; heap starts at `data_size + bss_size` and grows towards increasing addresses. Stack or heap overflow is undefined behaviour.

## Handle

**Handle** is a very important concept in Hyper OS program interface. In computer systems, external interrupt can break the process and trap into kernel. However in Hyper OS, since we can not break a running program (actually a running thread), we need to 'tell' the program to stop when there is an interrupt. Handle is the key to do this. 

`handle` is a template class. Objects of `handle<T>` is something like a pointer of type `T`, containing a linear address and some other information. As long as the program accesses the memory only by `handle`, we can easily stop the process when the interrupt flag is `true` after some memory access operation.

The functions and operators about `handle` is listed below.

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

After any memory access operation, the handle will do **tail check**. If the interrupt flag is `true`, in other words, there is an external interrupt, the program stop itself, release CPU access and notify the LAPIC atomicly. After that, ISR for external interrupt (actually a thread) starts processing.

```sequence
APIC -> process 1: set interrupt flag 
note over process 1: tail check
process 1 -> ISR : stop and release CPU
note over ISR: acquire CPU and start processing
```

