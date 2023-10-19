// -------------------------------------------------------------------------- //

#include <cstdlib>  // needed for MACROS
#include <sstream>  // needed for std::stringstream
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

void t_connect(std::string const& username,
               std::string const& nickname,
               int socket){
    std::stringstream ss;
    ss << socket;
    log("ADDING USER/CLIENT TO SOCKET #" + ss.str());
    s.um.addUser(socket);

    t_incoming_message("NICK " + nickname + "\r\n" + " USER " + username + "\r\n", socket);

    s.sendMessages(socket);
}

int main(void)
{
    /* USAGE: */
        /* Test commands with t_command(<full_message>, socket) */
            /* needs \r\n !!! */

    /* Setting up testing environment (Server + Client + User on socket #1) */
    t_connect("Dummy-User", "Dummy-Nick", 1);
    //t_connect("Hans", "Peter", 2);

    t_show_users();

    //t_command("USER Dummy-User\r\n", 1);

    t_command("QUIT\r\n", 1);

    t_show_users();

    return (EXIT_SUCCESS);
}

/* Past test cases */
    // log("Appending some content to UserBuffer");
    // log("Adding channels to server");
    // log("Listing Channels");
    // log("Erasing and listing channel");
    // log("ChannelInfo");
    // log("Adding Users to channel");
    // log("Erasing User from Channel");
    // log("Erasing User from Server who is part of a Channel");

// -------------------------------------------------------------------------- //
