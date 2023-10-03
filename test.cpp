// -------------------------------------------------------------------------- //

#include <vector>                     // needed for std::vector
#include <cstdlib>                    // needed for MACROS
#include <iostream>                   // needed for std::cout, std::endl

#include "include/UserManagement.hpp" // needed for UserManagement class
#include "include/Server.hpp"
#define GREEN "\033[32m"
#define RESET "\033[0m"

static inline void log(std::string const& message)
{
    std::cerr << GREEN << ">> " << message << " <<" << RESET << std::endl;
}

int main(void)
{
    log("Allocating Server on stack");
    Server s;

    log("Adding users to server");
	s.userManagement.addUser(1, "Hans", "Dieter");
	s.userManagement.addUser(2, "Walter", "Albert");

	log("Testing Parser");
	std::string tmp = "NICK vminkmar\r\nUSER vminkmar bla";
	char *test = strdup(tmp.c_str());
	s.parseIncomingMessage(test, 1);
	std::string tmp1 = ":vminkmar\r\n";
	char *test1 = strdup(tmp1.c_str());
	s.parseIncomingMessage(test1, 1);

    log("Getting Usernames from server");
    std::cout << s.userManagement.getUsernames() << std::endl;

//    log("Appending some content to UserBuffer");
//    s.userManagement.appendToBuffer("some content", 3, INPUT);
//    std::cout << s.userManagement.getBuffer(3, INPUT) << std::endl;
//
//    log("Adding channels to server");
//	s.userManagement.addChannel("Test");
//	s.userManagement.addChannel("Hamburg");
//    
//    log("Listing Channels");
//	s.userManagement.listChannels();
//
//    log("Erasing and listing channel");
//    s.userManagement.eraseChannel("Hamburg");
//    s.userManagement.listChannels();
//
//    log("Searching for erased Channel");
//    s.userManagement.getChannel("Hamburg");
//    s.userManagement.getChannel("some other");
//
//    log("ChannelInfo");
//    s.userManagement.printChannelInfo("Test");
//
//    log("Adding Users to channel");
//	s.userManagement.addUserToChannel(3, USER, "Test");
//	s.userManagement.addUserToChannel(4, OPERATOR, "Test");
//    s.userManagement.printChannelInfo("Test");
//
//    log("Erasing User from Channel");
//    s.userManagement.eraseUserFromChannel(3, "Test");
//    s.userManagement.printChannelInfo("Test");
//
//    log("Erasing User from Server who is part of a Channel");
//    s.userManagement.eraseUser(4);
//    s.userManagement.printChannelInfo("Test");
//
}

// -------------------------------------------------------------------------- //
