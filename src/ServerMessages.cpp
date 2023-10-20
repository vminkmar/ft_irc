// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <sstream>  // needed for std::stringstream
#include <iostream> // needed for std::cout, std::endl

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server replies */

void Server::RPL_CAP(int socket) {
    std::stringstream ss;
    ss << socket;
    log_success("CAP message send to socket#" + ss.str());
    std::string str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_JOIN(int socket, std::string const& channelName, std::string const& username){
    std::stringstream ss;
    ss << socket;
    log_success("JOIN message send to socket#" + ss.str());
    std::string str  = um.getNickname(socket) + "!" +
                       username + "@" + HOST + " JOIN " +
                       channelName + " * :" + username + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NICKCHANGE(int socket, std::string const& newNickname){
    std::string oldNickname = um.getNickname(socket);
    if (oldNickname.empty() == true){
        log_success("UNSET_NICKNAME got changed to " + newNickname);
    }
    else{
        log_success(oldNickname + " got changed to " + newNickname);
    }
    std::string str = ":"   + oldNickname
                      + "!" + um.getUsername(socket)
                      + "@" + "localhost" + " " + "NICK"
                      + " :" + newNickname + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_QUIT(int socket){
    std::stringstream ss;
    ss << socket;
    log_success("QUIT message send to socket#" + ss.str());
    std::string str = um.getNickname(socket) + "!" +
                      um.getUsername(socket) + "@" + "localhost" +
                      " QUIT :Goodbye!\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_PING(int socket, std::string const& servername){
    std::stringstream ss;
    ss << socket;
    log_success("PONG message send to socket#" + ss.str());
    std::string str = "PONG :" + servername + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_WELCOME(int socket, std::string const& username){
    std::stringstream ss;
    ss << socket;
    log_success("Welcome message send to socket#" + ss.str());
    std::string str = ":" + SERVERNAME +
                      " 001 " + um.getNickname(socket) +
                      " :Welcome to the ft_irc network " +
                      um.getNickname(socket) + "!" +
                      username + "@" + HOST + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server errors */

void Server::ERR_NONICKNAMEGIVEN(int socket){
    log_err("No Nickname given");
    std::string str = "431 :No nickname given\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ERRONEUSNICKNAME(int socket, std::string const& nickname){
    log_err("Nickname has unallowed characters");
    std::string str = "432 " + nickname + " :Erroneous nickname\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NICKNAMEINUSE(int socket, std::string const& nickname){
	log_err("Nickname already in use");
	std::string str = "433 " + nickname + " :Nickname is already in use\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NEEDMOREPARAMS(int socket, std::string const& command){
    log_err("Not enough parameters");
    std::string str = "461 " + command + " :Not enough parameters\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ALREADYREGISTRED(int socket){
    log_err("Unauthorized command (already registered)");
    std::string str = "462 :Unauthorized command (already registered)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
