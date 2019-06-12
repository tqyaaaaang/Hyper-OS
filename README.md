# Hyper-OS

Hyper-OS(HOS) is an Operating System Simulator for teaching and research use. It contains basic virtual hardwares (such as CPU, APIC, MMU, etc) and a tiny kernel. Different from other teaching OSes, Hyper-OS is not running on a real machine, so that it's easy enough for students without hardware knowledges. HOS can show the principle of interrupt, paging, memory allocation, process scheduling and symmetrical multi-processing by writing simple user-time programs. 

## Build

### Linux
#### Requirements
- `Python3`
- `python3-pip`
- `CMake >= 3.1`
- `GNU Make`

#### Get dependencies with python-pip
Install dependencies with the following command:
```
$ pip3 install -r requirements.txt
```

#### Compile hos kernel
Compile HOS using CMake and GNU make
```
$ cmake .
$ make
```

the binary file is located in `bin/hos`.

#### Run the kernel
Just run the `entry.py` script, and have fun:
```
$ python3 entry.py
```
