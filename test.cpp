// -------------------------------------------------------------------------- //

#include <vector>                     // needed for std::vector
#include <cstdlib>                    // needed for MACROS
#include <iostream>                   // needed for std::cout, std::endl

#include "include/Channel.hpp"        // needed for Channel class
#include "include/User.hpp"           // needed for User class
#include "include/UserManagement.hpp" // needed for UserManagement class

int main(void)
{
	
	UserManagement um;

	um.addChannel("Test");
	um.addChannel("Moin");
	um.addChannel("Hamburg");
	um.addChannel("Heilbronn");
	um.addChannel("Kuhdamm");
	um.listChannels();

	um.eraseChannel("Heilbronn");
	um.listChannels();
}

// -------------------------------------------------------------------------- //
