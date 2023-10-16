// -------------------------------------------------------------------------- //

#include <cstdlib>  // needed for MACROS
#include <iostream> // needed for std::cout, std::endl
#include <string.h> // needed for strdup()

#include "include/Server.hpp" // needed for Server class

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
	s.um.addUser(1);
	s.um.addUser(2);

	log("Testing Parser");
	std::string tmp = "NICK vminkmar\r\nUSER vminkmar bla";
	char *test = strdup(tmp.c_str());
	s.parseIncomingMessage(test, 1);
	std::string tmp1 = ":vminkmar\r\n";
	char *test1 = strdup(tmp1.c_str());
	s.parseIncomingMessage(test1, 1);

    log("Getting Usernames from server");
    std::cout << s.um.getUsernames() << std::endl;

    // log("Getting Usernames from server");
    // std::cout << um.getUsernames() << std::endl;

    // log("Appending some content to UserBuffer");
    // um.appendToBuffer("some content", 1, INPUT);
    // std::cout << um.getBuffer(1, INPUT) << std::endl;

    // log("Adding channels to server");
	// um.addChannel("Test");
	// um.addChannel("Hamburg");
    
    // log("Listing Channels");
	// um.listChannels();
	
    // log("Erasing and listing channel");
    // um.eraseChannel("Hamburg");
    // um.listChannels();

    // log("ChannelInfo");
    // um.printChannelInfo("Test");

    // log("Adding Users to channel");
	// um.addUserToChannel(1, USER, "Test");
	// um.addUserToChannel(2, OPERATOR, "Test");
    // um.printChannelInfo("Test");

    // log("Erasing User from Channel");
    // um.eraseUserFromChannel(1, "Test");
    // um.printChannelInfo("Test");

    // log("Erasing User from Server who is part of a Channel");
    // um.eraseUser(2);
    // um.printChannelInfo("Test");

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
