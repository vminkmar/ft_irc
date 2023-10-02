// -------------------------------------------------------------------------- //

#include <vector>              // needed for std::vector
#include <cstdlib>             // needed for MACROS
#include <iostream>            // needed for std::cout, std::endl

#include "include/Channel.hpp" // needed for Channel class
#include "include/User.hpp"    // needed for User class

int main(void)
{

	User u("Hans", "Dieter");
	User u2(u);

	std::cout << u << std::endl;
	std::cout << u2 << std::endl;

}

// -------------------------------------------------------------------------- //
