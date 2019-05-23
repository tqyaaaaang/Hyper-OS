#include "elephant.h"
#include "../../src/program/program_manager.h"
#include <cstring>

static program* gen()
{
	return new elephant;
}

void register_elephant()
{
	register_program("elephant", gen);
}

elephant::elephant()
{ }

elephant::~elephant()
{ }

void elephant::static_init()
{
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
	hos_std->println(graph);
	/*handle<int> pid = sys->create_process();
	sys->exec_program(pid, elep);
	while (1) {
		sys->yield();
		hos_std->println(elep);
		}*/
}
