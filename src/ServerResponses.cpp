// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <sstream>  // needed for std::stringstream
#include <iostream> // needed for std::cout, std::endl

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server replies */

void Server::RPL_CAP(int socket) {
    std::stringstream ss;
    ss << socket;
    log("CAP message prepared for socket#" + ss.str());
    t_str str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_JOIN(int socket,
                      t_str_c& channelName){
    std::stringstream ss;
    ss << socket;
    log("JOIN message for " + channelName + " prepared for socket#" + ss.str());
    t_str_c& username = um.getUsername(socket);
    t_str_c& nickname = um.getNickname(socket);
    t_str str  = ":" + nickname + "!" +
                       username + "@" + HOST + " JOIN " +
                       channelName + " * :" + username + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NICKCHANGE(int socket, t_str_c& newNickname){
    t_str oldNickname = um.getNickname(socket);
    if (oldNickname.empty() == true){
        log("UNSET_NICKNAME got changed to " + newNickname);
    }
    else{
        log(oldNickname + " got changed to " + newNickname);
    }
    t_str str = ":"   + oldNickname
                      + "!" + um.getUsername(socket)
                      + "@" + "localhost" + " " + "NICK"
                      + " :" + newNickname + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NOTOPIC(int socket, t_str_c& channelName){
    std::stringstream ss;
    ss << socket;
    log("NOTOPIC message for " + channelName
        + " prepared for socket#" + ss.str());
    t_str str = "331 "
                      + um.getNickname(socket) + " "
                      + channelName + " :No topic is set\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_PING(int socket, t_str_c& serverName){
    std::stringstream ss;
    ss << socket;
    log("PONG message prepared for socket#" + ss.str());
    t_str str = "PONG :" + serverName + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_QUIT(int socket){
    std::stringstream ss;
    ss << socket;
    log("QUIT message prepared for socket#" + ss.str());
    t_str str = um.getNickname(socket) + "!" +
                      um.getUsername(socket) + "@" + "localhost" +
                      " QUIT :Goodbye!\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_TOPIC(int socket,
                       t_str_c& channelName,
                       t_str_c& topic){
    std::stringstream ss;
    ss << socket;
    log("TOPIC message for " + channelName + " prepared for socket#" + ss.str());
    t_str str = "332 " + um.getNickname(socket)
                      + " " + channelName + " :"
                      + topic + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_NAMREPLY(int socket,
                          t_str_c& channelName,
                          t_str_c& members){
    std::stringstream ss;
    ss << socket;
    log("NAMREPLY message for " + channelName + " prepared for socket#" + ss.str());
    t_str str = "353 " + um.getNickname(socket)
                      + " = " + channelName + " : " + members + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_WELCOME(int socket, t_str_c& username){
    std::stringstream ss;
    ss << socket;
    log("Welcome message prepared to socket#" + ss.str());
    t_str str = ":" + SERVERNAME +
                      " 001 " + um.getNickname(socket) +
                      " :Welcome to the ft_irc network " +
                      um.getNickname(socket) + "!" +
                      username + "@" + HOST + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_PART(int socket,
                      t_str_c& channelName,
                      t_str_c& message){
    std::stringstream ss;
    ss << socket;
    log("<part message> " + message);
    log("Part message for " + channelName + " prepared for socket#" + ss.str());
    t_str str = ":" + um.getNickname(socket) + "!" 
                      + um.getUsername(socket) + "@" + HOST + " PART "
                      + channelName + " :" + message + "!" + "\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::RPL_IFTOPIC(int socket,
                                  t_str_c& channelName,
                                  t_str_c& topic){

        if (topic.empty() == true){
            RPL_NOTOPIC(socket, channelName);
        }
        else{
            RPL_TOPIC(socket, channelName, topic);
        }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server errors */

void Server::ERR_NOSUCHCHANNEL(int socket, t_str_c& channelName){
    log_err(channelName + " No such channel!");
    t_str str = "403 " + channelName + " :No such channel\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NONICKNAMEGIVEN(int socket){
    log_err("No Nickname given!");
    t_str str = "431 :No nickname given\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ERRONEUSNICKNAME(int socket, t_str_c& nickname){
    log_err("Nickname has unallowed characters!");
    t_str str = "432 " + nickname + " :Erroneous nickname\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NICKNAMEINUSE(int socket, t_str_c& nickname){
	log_err("Nickname already in use!");
	t_str str = "433 " + nickname + " :Nickname is already in use\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NEEDMOREPARAMS(int socket, t_str_c& command){
    log_err("Not enough parameters!");
    t_str str = "461 " + command + " :Not enough parameters\r\n";
	um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_ALREADYREGISTRED(int socket){
    log_err("Unauthorized command (already registered)!");
    t_str str = "462 :Unauthorized command (already registered)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_NOTONCHANNEL(int socket, t_str_c& channelName){
    log_err("User not on channel " + channelName + " !");
    t_str str = "401 " + um.getNickname(socket)
                      +  " " + channelName
                      + " :You're not on that channel\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_BADCHANNELKEY(int socket, t_str_c& channelName){

    log_err("Bad channel key for " + channelName + "!");
    t_str str = "475 " + um.getNickname(socket)
                      + " " + channelName
                      + " :Cannot join channel (+k)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_INVITEONLYCHAN(int socket, t_str_c& channelName){

    log_err("Tried to join invite-only channel " + channelName + "!");
    t_str str = "473 " + um.getNickname(socket)
                      + " " + channelName
                      + " :Cannot join channel (+j)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

void Server::ERR_CHANNELISFULL(int socket, t_str_c& channelName){

    log_err("Tried to join full channel " + channelName + "!");
    t_str str =  "471 " + um.getNickname(socket)
                       + " " + channelName
                       + " :Cannot join channel (+l)\r\n";
    um.appendToBuffer(str, socket, OUTPUT);
}

// -------------------------------------------------------------------------- //
