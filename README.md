# Hyper-OS

Hyper-OS(HOS) is an Operating System Simulator for teaching and research use. It contains basic virtual hardwares (such as CPU, APIC, MMU, etc) and a tiny kernel. Different from other teaching OSes, Hyper-OS is not running on a real machine, so that it's easy enough for students without hardware knowledges. HOS can show the principle of interrupt, paging, memory allocation, process scheduling and symmetrical multi-processing by writing simple user-time programs. 

## Build

Compile HOS using CMake and GNU make

```
$ cmake .
$ make
```

the binary file is located in `bin/hos`.
