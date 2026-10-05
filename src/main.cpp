//std
#include <cstdlib>
#include <stdexcept>

//Scissors
#include "Scissors/inc/scissors.hpp"

int main(void)
{
	try
	{
		scissors::unit();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	//return
	return EXIT_SUCCESS;
}