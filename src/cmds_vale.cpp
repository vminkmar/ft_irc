// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <iostream> // needed for std::cout, std::endl
#include <sstream>  // needed for std::stringstream

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> commands valentin */

void Server::CAP_RPL(int socket) {
  if (m_parameters[0] == "LS") {
    
    std::stringstream ss;
    ss << "CAP message send to Socket: " << socket << std::endl;
    log_out(ss.str());

    //std::cout << "CAP message send to Socket: " << socket << std::endl;

    std::string str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
  }
}

void Server::WELCOME_RPL(int socket) {
    log_out("Welcome message send to: " + um.getUsername(socket));
    //std::cout << "Welcome message send to: " << um.getUsername(socket)
    //         << std::endl;
    
    std::string str = "001 " + um.getNickname(socket) +
                    " :Welcome to the ft_irc network " +
                    um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + HOST + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::PING_RPL(int socket) {
    log_out("PONG message send to: " + um.getUsername(socket));

    //std::cout << "PONG message send to: " << um.getUsername(socket)
            //<< std::endl;
            //

    std::string str = " PONG :" + m_parameters[0] + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::QUIT_RPL(int socket) {
    log("QUIT command by: " + um.getUsername(socket));
    std::string str = um.getNickname(socket) + "!" +
                    um.getUsername(socket) + "@" + "localhost" +
                    " QUIT :Goodbye!\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::NICKCHANGE_RPL(int socket, std::string newNick){
    log(um.getUsername(socket)
        + " changed his Nickname from <" 
        + um.getNickname(socket) + "> to <" + newNick + ">");

	//std::cout << um.getNickname(socket) + " got changed to " + newNick << std::endl;
    std::string str = ":" + um.getNickname(socket)
                      + "!" + um.getUsername(socket)
                      + "@" + "localhost" + " " + "NICK"
                      + " :" + newNick + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

bool Server::checkUnallowedCharacters(std::string nickname){
	const std::string unallowedChars = " !@#$%^&*()[]{}<>:;,/";
	for(size_t i = 0; i < unallowedChars.length(); ++i){
		size_t find = nickname.find(unallowedChars[i]);
		if(find != std::string::npos){
			return true;
        }
	}
	return false;
}

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
