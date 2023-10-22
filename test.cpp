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

int main(void)
{
    /* USAGE: */
        /* Test commands with t_command(<full_message>, socket) */
            /* needs \r\n !!! */

    t_connect("Dummy-User", "Dummy-Nick", 1);
    t_show_users();

    //t_command("JOIN channel,channel2,channel3 passw,passw1,passw2\r\n", 1);
    
    s.um.addChannel("b2");
    s.um.addChannel("c3");
    t_command("JOIN a1,b2\r\n", 1);

    std::cout << "a1: " << s.um.getChannelNicknames("a1") << std::endl;
    std::cout << "b2: " << s.um.getChannelNicknames("b2") << std::endl;
    std::cout << "c3: " << s.um.getChannelNicknames("c3") << std::endl;
    
    Channel const& a1 = s.um.getChannel("a1");
    t_bool(a1.isOperator(1), "Dummy User is a1 operator");
    
    Channel const& b2 = s.um.getChannel("b2");
    t_bool(b2.isOperator(1), "Dummy User is b2 operator");

    Channel const& c3 = s.um.getChannel("c3");
    t_bool(c3.isOperator(1), "Dummy User is c3 operator");

    t_command("JOIN 0\r\n", 1);

    return (EXIT_SUCCESS);
}


// -------------------------------------------------------------------------- //
