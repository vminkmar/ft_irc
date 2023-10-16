// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class
#include <iostream> // needed for std::cout, std::endl

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> commands valentin */

void Server::CAP_RPL(int socket) {
  if (m_parameters[0] == "LS") {
    std::cout << "CAP message send to Socket: " << socket << std::endl;
    std::string str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
  }
}

void Server::WELCOME_RPL(int socket) {
  std::cout << "Welcome message send to: " << um.getUser(socket)
            << std::endl;
  std::string str = "001 " + um.getNick(socket) +
                    " :Welcome to the ft_irc network " +
                    um.getNick(socket) + "!" +
                    um.getUser(socket) + "@" + HOST + "\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}

void Server::PING_RPL(int socket) {
  std::cout << "PONG message send to: " << um.getUser(socket)
            << std::endl;
  std::string str = " PONG :" + m_parameters[0] + "\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}

void Server::QUIT_RPL(int socket) {
  std::string str = um.getNick(socket) + "!" +
                    um.getUser(socket) + "@" + "localhost" +
                    " QUIT :Goodbye!\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}

void Server::NICKCHANGE_RPL(int socket, std::string newNick){
	std::cout << um.getNick(socket) + " got changed to " + newNick << std::endl;
	std::string str = ":" + um.getNick(socket) + "!" + um.getUser(socket) + "@" + "localhost" + " " + "NICK" + " :" + newNick + "\r\n";
  um.appendToBuffer(str, socket, OUTPUT);
}


void Server::ERR_NICKNAMEINUSE(int socket, std::string nick){
	std::cout << "Error: Username already in use" << std::endl;
	std::string str = "433 " + nick + ":Nickname is already in use\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
