// -------------------------------------------------------------------------- //

#include <vector>              // needed for std::vector
#include <cstdlib>             // needed for MACROS
#include <iostream>            // needed for std::cout, std::endl

#include "include/Channel.hpp" // needed for Channel class
#include "include/User.hpp"    // needed for User class

int main(void)
{
	Channel d("Main");

	User u("Hans", "HansAlbert", USER);
	User u2("Dieter", "DieterKohl", USER);

	d.setName("Hamburg");
	d.setUserLimit(3);
	d.toggleInviteOnly();

	d.addUser(&u, OPERATOR);
	std::cout << d << std::endl;

	std::cout << std::endl;

	d.addUser(&u, USER);
	std::cout << d << std::endl;

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
