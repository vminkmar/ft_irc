// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <sstream>  // needed for std::stringstream
#include <iostream> // needed for std::cout, std::endl

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server replies */

void Server::RPL_CAP(int socket) {
    std::stringstream ss;
    ss << socket;
    log("CAP message send to socket#" + ss.str());
    std::string str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_JOIN(int socket,
                      std::string const& channelName,
                      std::string const& username){
    std::stringstream ss;
    ss << socket;
    log("JOIN message send to socket#" + ss.str());
    std::string str  = ":" + um.getNickname(socket) + "!" +
                       username + "@" + HOST + " JOIN " +
                       channelName + " * :" + username + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NICKCHANGE(int socket, std::string const& newNickname){
    std::string oldNickname = um.getNickname(socket);
    if (oldNickname.empty() == true){
        log("UNSET_NICKNAME got changed to " + newNickname);
    }
    else{
        log(oldNickname + " got changed to " + newNickname);
    }
    std::string str = ":"   + oldNickname
                      + "!" + um.getUsername(socket)
                      + "@" + "localhost" + " " + "NICK"
                      + " :" + newNickname + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NOTOPIC(int socket, std::string const& channelName){
    std::stringstream ss;
    ss << socket;
    log("NOTOPIC message sent to socket#" + ss.str());
    std::string str = "331 "
                      + um.getNickname(socket) + " "
                      + channelName + " :No topic is set\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_PING(int socket, std::string const& serverName){
    std::stringstream ss;
    ss << socket;
    log("PONG message send to socket#" + ss.str());
    std::string str = "PONG :" + serverName + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_QUIT(int socket){
    std::stringstream ss;
    ss << socket;
    log("QUIT message send to socket#" + ss.str());
    std::string str = um.getNickname(socket) + "!" +
                      um.getUsername(socket) + "@" + "localhost" +
                      " QUIT :Goodbye!\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_TOPIC(int socket,
                       std::string const& channelName,
                       std::string const& channelTopic){
    std::stringstream ss;
    ss << socket;
    log("TOPIC message send to socket#" + ss.str());
    std::string str = "332 " + um.getNickname(socket)
                      + " " + channelName + " :"
                      + channelTopic + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NAMREPLY(int socket,
                          std::string const& channelName,
                          std::string const& members){
    std::stringstream ss;
    ss << socket;
    log("NAMREPLY message send to socket#" + ss.str());
    std::string str = "353 " + um.getNickname(socket)
                      + " = " + channelName + " : " + members + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_WELCOME(int socket, std::string const& username){
    std::stringstream ss;
    ss << socket;
    log("Welcome message send to socket#" + ss.str());
    std::string str = ":" + SERVERNAME +
                      " 001 " + um.getNickname(socket) +
                      " :Welcome to the ft_irc network " +
                      um.getNickname(socket) + "!" +
                      username + "@" + HOST + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server errors */

void Server::ERR_NOSUCHCHANNEL(int socket, std::string const& channelName){
    log_err("No such channel!");
    std::string str = "403 " + channelName + " :No such channel\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NONICKNAMEGIVEN(int socket){
    log_err("No Nickname given!");
    std::string str = "431 :No nickname given\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ERRONEUSNICKNAME(int socket, std::string const& nickname){
    log_err("Nickname has unallowed characters!");
    std::string str = "432 " + nickname + " :Erroneous nickname\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NICKNAMEINUSE(int socket, std::string const& nickname){
	log_err("Nickname already in use!");
	std::string str = "433 " + nickname + " :Nickname is already in use\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NEEDMOREPARAMS(int socket, std::string const& command){
    log_err("Not enough parameters!");
    std::string str = "461 " + command + " :Not enough parameters\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ALREADYREGISTRED(int socket){
    log_err("Unauthorized command (already registered)!");
    std::string str = "462 :Unauthorized command (already registered)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
