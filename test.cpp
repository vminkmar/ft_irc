// -------------------------------------------------------------------------- //

#include <vector>                     // needed for std::vector
#include <cstdlib>                    // needed for MACROS
#include <iostream>                   // needed for std::cout, std::endl

#include "include/UserManagement.hpp" // needed for UserManagement class

int main(void)
{
	UserManagement um;

	um.addUser(1, "Hans", "Dieter");
	um.addChannel("Test");
	um.addChannel("Hamburg");
	um.listChannels();
	um.addUsertoChannel(1, USER, "Test");

	//um.printChannel("Test");

}

// -------------------------------------------------------------------------- //
