#include "elephant_boom.h"
#include <cstring>

void elephant_boom::static_init()
{
	set_name("elephant-boom");
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
    const char ele[] = "elephant-boom\0";
	size = strlen(ele) + 1;
	elep = alloc_static<char>(size);
	for (size_t i = 0; i < size; i++)
		elep[i].modify_in_compile(ele[i]);

}

void elephant_boom::main()
{
	handle<int> pid = sys->create_process();
	sys->exec_program(pid, elep);
	hos_std->println(graph);
	hos_std->write_int(sys->pid());
	hos_std->println("");
	while (1) {
		sys->yield();
		hos_std->write_int(sys->pid());
		hos_std->println(" tick.");
	}
}
