// -------------------------------------------------------------------------- //

#include <vector>                     // needed for std::vector
#include <cstdlib>                    // needed for MACROS
#include <iostream>                   // needed for std::cout, std::endl

#include "include/UserManagement.hpp" // needed for UserManagement class

#define GREEN "\033[32m"
#define RESET "\033[0m"

static inline void log(std::string const& message)
{
    std::cerr << GREEN << ">> " << message << " <<" << RESET << std::endl;
}

int main(void)
{
    log("Testing UserManagement");
	
    UserManagement um;

    log("Adding users to server");
	um.addUser(1, "Hans", "Dieter");
	um.addUser(2, "Walter", "Albert");

    log("Getting Usernames from server");
    std::cout << um.getUsernames() << std::endl;

    log("Adding channels to server");
	um.addChannel("Test");
	um.addChannel("Hamburg");
    
    log("Listing Channels");
	um.listChannels();
	
    log("Erasing and listing channel");
    um.eraseChannel("Hamburg");
    um.listChannels();

    log("ChannelInfo");
    um.printChannelInfo("Test");

    log("Adding Users to channel");
	um.addUserToChannel(1, USER, "Test");
	um.addUserToChannel(2, OPERATOR, "Test");
    um.printChannelInfo("Test");

    log("Erasing User from Channel");
    um.eraseUserFromChannel(1, "Test");
    um.printChannelInfo("Test");

    log("Erasing User from Server who is part of a Channel");
    um.eraseUser(2);
    um.printChannelInfo("Test");
}

// -------------------------------------------------------------------------- //
