// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <iostream> // needed for std::cout, std::endl

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server replies */

void Server::RPL_CAP(int socket) {
  if (m_parameters[0] == "LS") {
    
    //std::cout << "CAP message send to Socket: " << socket << std::endl;

    std::string str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
  }
}

void Server::RPL_WELCOME(int socket) {
    //std::cout << "Welcome message send to: " << um.getUsername(socket)
    //         << std::endl;
    
    std::string str = "001 " + um.getNickname(socket) +
                    " :Welcome to the ft_irc network " +
                    um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + HOST + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_PING(int socket) {

    //std::cout << "PONG message send to: " << um.getUsername(socket)
            //<< std::endl;
            //

    std::string str = " PONG :" + m_parameters[0] + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_QUIT(int socket) {
    std::string str = um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + "localhost" +
                    " QUIT :Goodbye!\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_JOIN(int socket, std::string name) {
    std::string str = um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + HOST + " JOIN " +
                    name + " * :" + um.getUsername(socket) + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NICKCHANGE(int socket, std::string newNick){
	//std::cout << um.getNickname(socket) + " got changed to " + newNick << std::endl;
    std::string str = ":" + um.getNickname(socket)
                      + "!" + um.getUsername(socket)
                      + "@" + "localhost" + " " + "NICK"
                      + " :" + newNick + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server errors */

void Server::ERR_NICKNAMEINUSE(int socket, std::string nick){
	log_err("Nickname already in use");
    //std::cout << "Error: Nickname already in use" << std::endl;

	std::string str = "433 " + nick + ":Nickname is already in use\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ERRONEUSNICKNAME(int socket, std::string nick){
    log_err("Nickname has unallowed characters");
    //std::cout << "Error: Nickname has unallowed characters" << std::endl;
	
    std::string str = "432 " + nick + ":Erroneous nickname\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NEEDMOREPARAMS(int socket, std::string command){
	log_err("Not enough parameters");
    std::string str = "461 " + command + " :Not enough parameters";
	std::cout << str << std::endl;
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NONICKNAMEGIVEN(int socket){
    
    /* @note correct message? */
    log_err("Username already in use");
    //std::cout << "Error: Username already in use" << std::endl;
	
    std::string str = "431 :No nickname given\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
