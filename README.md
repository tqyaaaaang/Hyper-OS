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
mkdir build
cd build
cmake ..
make
cd ..
cp build/bin/hos bin/hos
```

the binary file is located in `bin/hos`.

#### Run the kernel
Just run the `entry.py` script, and have fun:
```
$ python3 entry.py
```

### Windows
#### Requirements
- `Python3`
- `python3-pip`
- `CMake >= 3.1`
- `Visual Studio 2017` or `Visual Studio 2019`

Notes:
- Remember to add `python` to `PATH`

#### Get dependencies with python-pip
Install dependencies with the following command:
```
$ pip install -r requirements.txt
```

#### Compile hos kernel
Generate the Visual Studio Solution using cmake in PowerShell:
```
mkdir build
cd build
cmake ..
```
Then you need to compile HOS using Visual Studio. You may need to copy the executable file generated to `bin/hos`.

#### Run the kernel
Just run the `entry.py` script, and have fun:
```
$ python entry.py
```
