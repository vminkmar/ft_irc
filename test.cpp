// -------------------------------------------------------------------------- //

#include <cstdlib>  // needed for MACROS
#include <sstream>  // needed for std::stringstream
#include <iostream> // needed for std::cout, std::endl

#include "include/Server.hpp" // needed for Server class

#define GREEN "\033[32m"
#define RESET "\033[0m"

/* Past test cases */
    // log("Appending some content to UserBuffer");
    // log("Adding channels to server");
    // log("Listing Channels");
    // log("Erasing and listing channel");
    // log("ChannelInfo");
    // log("Adding Users to channel");
    // log("Erasing User from Channel");
    // log("Erasing User from Server who is part of a Channel");

Server s;

static inline void log(std::string const& message)
{
    std::cerr << GREEN << "\n>> " << message << " <<" << RESET << std::endl;
}

void t_incoming_message(std::string const& message, int socket){
    s.parseIncomingMessage(message.c_str(), socket);
}

void t_command(std::string const& message, int socket){
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

    std::string msg = "NICK " + nickname + "\r\n";
    std::string msg2 = "USER " + username + "\r\n";
    t_incoming_message(msg.c_str(), socket);
    t_incoming_message(msg2.c_str(), socket);

    s.sendMessages(socket);
}

void t_bool(bool is, std::string const& message){

    if (is == true){
        std::cout << message << " is true!" << std::endl;
    }
    else{
        std::cout << message << " is false!" << std::endl;
    }
}

void t_populate_channel(){
    s.um.addChannel("a1");
    Channel * a1 = s.um.getChannel("a1");
    a1->setPassword("123");

    s.um.addChannel("b2");
    s.um.addChannel("c3");
}

void t_show_channel(){
    log("LISTING CHANNELS");
    std::cout << "\nChannelnames: " << s.um.getChannelNames() << std::endl;
}

int main(void)
{
    /* USAGE: */
        /* t_command(<full_message>, socket) <-- needs \r\n */

    int t_socket = 1;

    t_connect("Dummy-User", "Dummy-Nick", t_socket);
    t_show_users();

    t_populate_channel();
    t_show_channel();
   
    t_command("JOIN a1 passw\r\n", t_socket);
    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
