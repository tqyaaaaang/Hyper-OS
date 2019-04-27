cc = g++
prog = os
VPATH = user:kern/intr:kern/clock:kern/memory:kern/process:kern/defs:kern/application:kern

flags = -O2 -std=c++11 -lpthread -I./kern/intr -I./kern/clock -I./kern/memory -I./kern/process -I./kern/defs -I./kern
headers = defs.h intr.h page_dir.h process.h state_info.h pmm.h application.h vclock.h pmm.h

os: driver.o intr.o clock.o state.o pmm.o
	$(cc) driver.o intr.o clock.o state.o pmm.o -o os $(flags)

intr.o: intr.cpp $(headers)
	$(cc) -c ./kern/intr/intr.cpp -o intr.o $(flags)

clock.o: vclock.cpp $(headers)
	$(cc) -c ./kern/clock/vclock.cpp -o clock.o $(flags)

driver.o: driver.cpp $(headers)
	$(cc) -c ./kern/driver.cpp -o driver.o $(flags)

state.o: state_info.cpp $(headers)
	$(cc) -c ./kern/state_info.cpp -o state.o $(flags)

pmm.o: pmm.cpp $(headers)
	$(cc) -c ./kern/memory/pmm.cpp -o pmm.o $(flags)

clean:
	find . -name "*~" | xargs rm  -rf
	find . -name "*.aux" | xargs rm  -rf
	find . -name "*.log" | xargs rm  -rf
	find . -name "*.o" | xargs rm -rf
	find . -name "os" | xargs rm -rf
	find . -name "*.gch" | xargs rm -rf
