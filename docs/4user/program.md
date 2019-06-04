# Interface for User-Mode Programs

## Program Class

User-Mode Program is supported in Hyper OS in a special way. Every program is a inheritance class of abstract class program(see `/src/program/program.h`). You should implement two virtual function: `main()` and `static_init()`.

1. `static_init()` is called when the program is 'compiled', and in Hyper OS it is called when it's cloned by `program manager`. In `static_init()` you can alloc memory in `.bss` or `.data`(see section `Program Memory Layout`), modify the data in `.data` using `handle<T>::modify_in_compile(T)`(see section `Handle`), or set name for program.

Example: `/user/shell/shell.cpp`

```c++
void hyper_shell::static_init()
{
	set_name("shell");
	argc = alloc_bss<int>();
	for (size_t i = 0; i < 20; i++)
		argv[i] = alloc_bss<char>(30);
}
```

2. 