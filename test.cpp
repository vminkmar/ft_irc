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

void t_show_users(size_t no_users){
    log("LISTING USERS");
    for (size_t socket = 1; socket <= no_users; ++socket){
        std::cout << s.um.getNickname(socket)
                  << "(" << s.um.getUsername(socket) << ")";
        if (socket != no_users){
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
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
    a1->toggleChannelKey();
    
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

    int no_users = 0;

    t_connect("Dummy-User", "Dummy-Nick", ++no_users);
    t_connect("Test-User", "Test-Nick", ++no_users);
    t_connect("Foo-User", "Foo-Nick", ++no_users);
    t_show_users(no_users);

    t_populate_channel();
    t_show_channel();
   
    t_command("JOIN b2\r\n", 1);
    t_command("JOIN b2\r\n", 2);
    t_command("JOIN b2\r\n", 3);

    t_command("JOIN 0\r\n", 2);
    t_command("JOIN b2\r\n", 3);

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------- //
