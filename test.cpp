// -------------------------------------------------------------------------- //

#include <cstdlib>  // needed for MACROS
#include <iostream> // needed for std::cout, std::endl

#include "include/Server.hpp" // needed for Server class

#define GREEN "\033[32m"
#define RESET "\033[0m"

Server s;

static inline void log(std::string const& message)
{
    std::cerr << GREEN << "\n>> " << message << " <<" << RESET << std::endl;
}

void t_incoming_message(std::string message, int socket){
    s.parseIncomingMessage(message.c_str(), socket);
}

void t_command(std::string message, int socket){
    log("TESTING COMMAND");
    s.parseIncomingMessage(message.c_str(), socket);
    s.sendMessages(socket);
}

void t_show_users(){
    log("LISTING USERS");
    std::cout << "\nUsernames: " << s.um.getUsernames() << "\n"
              << "Nicknames: " << s.um.getNicknames() 
              << std::endl;
}

void t_setup(){
    log("ADDING USER/CLIENT TO SOCKET #1");
    s.um.addUser(1);
    t_incoming_message("USER Dummy-User\r\n", 1);
    s.sendMessages(1);
    t_incoming_message("NICK Dummy-Test\r\n", 1);
    s.sendMessages(1);
}

int main(void)
{
    /* USAGE: */
        /* Test commands with t_command(<full_message>, socket) */
            /* needs \r\n !!! */

    /* Setting up testing environment (Server + Client + User on socket #1) */
    t_setup();

    /* Show Users after basic setup */
    t_show_users();

    /* Test command NICK */
    t_command("NICK huhu\r\n", 1);
    t_command("NICK \r\n", 1);

    /* Test command QUIT */
    t_command("QUIT\r\n", 1);

    // log("Appending some content to UserBuffer");
    // log("Adding channels to server");
    // log("Listing Channels");
    // log("Erasing and listing channel");
    // log("ChannelInfo");
    // log("Adding Users to channel");
    // log("Erasing User from Channel");
    // log("Erasing User from Server who is part of a Channel");

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
