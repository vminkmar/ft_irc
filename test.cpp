// -------------------------------------------------------------------------- //

#include <vector>              // needed for std::vector
#include <cstdlib>             // needed for MACROS

#include "include/Channel.hpp" // needed for Channel class
#include "include/User.hpp"    // needed for User class

int main(void)
{
	Channel d("Main");

	User u("Hans", "HansAlbert", USER);
	User u2("Dieter", "DieterKohl", USER);

	d.setName("Hamburg");
	d.setTopic("Moinsen allerseits");
	d.setPassword("moin");
	d.setUserLimit(3);
	d.toggleInviteOnly();

	d.addUser(&u);
	d.addUser(&u);
	d.addUser(&u2);
	
	d.removeUser(u2);

	std::cout << d << std::endl;

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
