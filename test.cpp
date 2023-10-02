// -------------------------------------------------------------------------- //

#include <vector>                     // needed for std::vector
#include <cstdlib>                    // needed for MACROS
#include <iostream>                   // needed for std::cout, std::endl

#include "include/Channel.hpp"        // needed for Channel class
#include "include/User.hpp"           // needed for User class
#include "include/UserManagement.hpp" // needed for UserManagement class

int main(void)
{
	Channel c("Test");

	std::cout << c;
	c.addUser(1, USER);
	c.addUser(2, OPERATOR);
}

// -------------------------------------------------------------------------- //
