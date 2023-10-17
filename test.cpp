// -------------------------------------------------------------------------- //

#include <cstdlib>  // needed for MACROS
#include <iostream> // needed for std::cout, std::endl

#include "include/Server.hpp" // needed for Server class

#define GREEN "\033[32m"
#define RESET "\033[0m"

Server s;

static inline void log(std::string const& message)
{
    std::cerr << GREEN << ">> " << message << " <<" << RESET << std::endl;
}

void t_incoming_message(std::string message, int socket){
    s.parseIncomingMessage(message.c_str(), socket);
}

void t_command(std::string command, int socket){
    log("Testing command");
    s.parseIncomingMessage(command, socket);
}

void t_setup(){
    log("Adding User/Client to server at socket #1");
    s.um.addUser(1);
    t_incoming_message("USER Dummy-User\r\n", 1);
}

int main(void)
{

    t_setup();
    t_command("NICK test\r\n", 1);

    std::cout << s.um.getUsernames() << std::endl;

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
