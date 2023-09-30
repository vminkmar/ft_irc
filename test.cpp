
#include <vector>              // needed for std::vector
#include <cstdlib>             // needed for MACROS
#include "include/Channel.hpp" // needed for Channel class

int main(void)
{
	Channel d("Name");

	std::cout << d << std::endl;

    return (EXIT_SUCCESS);
}
