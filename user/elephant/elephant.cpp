#include "elephant.h"
#include "../../src/program/program_manager.h"
#include "elephant_boom.h"
#include <cstring>

static program* gen()
{
	return new elephant;
}

static program* gen_boom()
{
	return new elephant_boom;
}

void register_elephant()
{
	register_program("elephant", gen);
	register_program("elephant-boom", gen_boom);
}

elephant::elephant()
{ }

elephant::~elephant()
{ }

void elephant::static_init()
{
	set_name("elephant");
	const char str[] =
		"--------------------------------------------------------------------\n"
		"    //    / /                                      //   ) ) //   )  \n"
		"   //___ / /         ___      ___      __         //   / / ((       \n"        
		"  / ___   //   / / //   ) ) //___) ) //  ) )     //   / /    \\     \n"      
		" //    / ((___/ / //___/ / //       //          //   / /       ))   \n"   
	    "//    / /    / / //       ((____   //          ((___/ / ((___ //    \n"
		"--------------------------------------------------------------------\0";
	size_t size = strlen(str) + 1;
	graph = alloc_static<char>(size);
	for (size_t i = 0; i < size; i++)
		graph[i].modify_in_compile(str[i]);
    const char ele[] = "elephant\0";
	size = strlen(ele) + 1;
	elep = alloc_static<char>(size);
	for (size_t i = 0; i < size; i++)
		elep[i].modify_in_compile(ele[i]);
}

void elephant::main()
{
	hos_std->write_int(sys->pid());
	hos_std->println(" in.");
	hos_std->println(graph);
	handle<int> pid = sys->create_process();
	if (sys->pid() < 3) {
		sys->exec_program(pid, elep);
		sys->wait(pid);
	}
	hos_std->write_int(sys->pid());
	hos_std->println(" exit.");
}
