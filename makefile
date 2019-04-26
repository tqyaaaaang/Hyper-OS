cc = g++
prog = os
VPATH = user:kern

flags = -O2 -std=c++11 -lpthread -I./
headers = defs.h intr.h page_dir.h process.h state_info.h

os: driver.o intr.o clock.o state.o
	$(cc) driver.o intr.o clock.o state.o -o os $(flags)

intr.o: intr.cpp $(headers)
	$(cc) -c ./kern/intr.cpp -o intr.o $(flags)

clock.o: vclock.cpp $(headers)
	$(cc) -c ./kern/vclock.cpp -o clock.o $(flags)

driver.o: driver.cpp $(headers)
	$(cc) -c ./kern/driver.cpp -o driver.o $(flags)

state.o: state_info.cpp $(headers)
	$(cc) -c ./kern/state_info.cpp -o state.o $(flags)
clean:
	find . -name "*~" | xargs rm  -rf
	find . -name "*.aux" | xargs rm  -rf
	find . -name "*.log" | xargs rm  -rf
	find . -name "*.o" | xargs rm -rf
	find . -name "os" | xargs rm -rf
	find . -name "*.gch" | xargs rm -rf
