// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server replies */

void Server::RPL_CAP(int socket){
    log("CAP message prepared for socket#" + itostr(socket));
    t_str str = "CAP * LS :cap reply...\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_JOIN(int socketSender, int socketTarget, t_str_c& channelName){
    log("JOIN message for " + channelName 
        + " prepared for socket#" + itostr(socketTarget));
    t_str_c& username = um.getUsername(socketSender);
    t_str_c& nickname = um.getNickname(socketSender);
    t_str str  = ":" + nickname + "!"
                 + username + "@" + HOST + " JOIN "
                 + channelName + " * :" + username + "\r\n";
    um.appendToBuffer(socketTarget, str, OUTPUT);
}

void Server::RPL_NICKCHANGE(int socket, t_str_c& newNickname){
    t_str oldNickname = um.getNickname(socket);
    if (oldNickname.empty() == true){
        log("UNSET_NICKNAME got changed to " + newNickname);
    }
    else{
        log(oldNickname + " got changed to " + newNickname);
    }
    t_str str = ":" + oldNickname
                + "!" + um.getUsername(socket)
                + "@" + "localhost" + " " + "NICK"
                + " :" + newNickname + "\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_NOTOPIC(int socket, t_str_c& channelName){
    log("NOTOPIC message for " + channelName
        + " prepared for socket#" + itostr(socket));
    t_str str = "331 " + um.getNickname(socket) + " "
                + channelName + " :No topic is set\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_PING(int socket, t_str_c& serverName){
    log("PONG message prepared for socket#" + itostr(socket));
    t_str str = "PONG :" + serverName + "\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_QUIT(int socketSender, int socketTarget, t_str_c& message){
    log("QUIT message prepared for socket#" + itostr(socketTarget));
    t_str str = ":" + um.getNickname(socketSender) + "!" +
                um.getUsername(socketSender) + "@" + "localhost"
                + " QUIT :" + message + "\r\n";
    um.appendToBuffer(socketTarget, str, OUTPUT);
}

void Server::RPL_TOPIC(int socket, t_str_c& channelName, t_str_c& topic){
    log("TOPIC message for " + channelName
        + " prepared for socket#" + itostr(socket));
    t_str str = "332 " + um.getNickname(socket)
                + " " + channelName + " :"
                + topic + "\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_NAMREPLY(int socket, t_str_c& channelName, t_str_c& members){
    log("NAMREPLY message for " + channelName
        + " prepared for socket#" + itostr(socket));
    t_str str = "353 " + um.getNickname(socket)
                + " = " + channelName + " : " + members + "\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_WELCOME(int socket, t_str_c& username){
    log("Welcome message prepared to socket#" + itostr(socket));
    t_str str = ":" + SERVERNAME +
                " 001 " + um.getNickname(socket) +
                " :Welcome to the ft_irc network "
                + um.getNickname(socket) + "!"
                + username + "@" + HOST + "\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::RPL_PART(int socketSender, int socketTarget, t_str_c& channelName, t_str_c& message){
    log("Part message for " + channelName
        + " prepared for socket#" + itostr(socketTarget));
    t_str str = ":" + um.getNickname(socketSender) + "!"
                + um.getUsername(socketSender) + "@" + HOST + " PART "
                + channelName + " :" + message + "!" + "\r\n";
    um.appendToBuffer(socketTarget, str, OUTPUT);
}

void Server::RPL_IFTOPIC(int socket, t_str_c& channelName, t_str_c& topic){
    if (topic.empty() == true){
        RPL_NOTOPIC(socket, channelName);
    }
    else{
        RPL_TOPIC(socket, channelName, topic);
    }
}

void Server::RPL_PRIVMSG(int socketSender, t_str_c& target, t_str_c& message){
    log("Private message prepared for socket"
        + itostr(um.getSocket(target)) + "("
        + target + ")");
    t_str str = ":" + um.getNickname(socketSender) + "!"
                + um.getUsername(socketSender)
                + "@" + HOST " PRIVMSG " + target + " :" + message + "\r\n";
    um.appendToBuffer(um.getSocket(target), str, OUTPUT);
}

void Server::RPL_INVITING(int socketSender,
                          int socketTarget,
                          t_str_c& channelName,
                          t_str_c& target){
    log(um.getUsername(socketSender) + " invited "
        + target + " to channel " + channelName);
    t_str str = ": 341 " + um.getNickname(socketSender) + " "
        + target + " " + channelName + "\r\n";
    um.appendToBuffer(socketTarget, str, OUTPUT);
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server errors */

void Server::ERR_NOSUCHCHANNEL(int socket, t_str_c& channelName){
    log_err(channelName + " No such channel!");
    t_str str = "403 " + channelName + " :No such channel\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NONICKNAMEGIVEN(int socket){
    log_err("No Nickname given!");
    t_str str = "431 :No nickname given\r\n";
	um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_ERRONEUSNICKNAME(int socket, t_str_c& nickname){
    log_err("Nickname has unallowed characters!");
    t_str str = "432 " + nickname + " :Erroneous nickname\r\n";
	um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NICKNAMEINUSE(int socket, t_str_c& nickname){
	log_err("Nickname already in use!");
	t_str str = "433 " + nickname + " :Nickname is already in use\r\n";
	um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NEEDMOREPARAMS(int socket, t_str_c& command){
    log_err("Not enough parameters!");
    t_str str = "461 " + command + " :Not enough parameters\r\n";
	um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_ALREADYREGISTRED(int socket){
    log_err("Unauthorized command (already registered)!");
    t_str str = "462 :Unauthorized command (already registered)\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NOTONCHANNEL(int socket, t_str_c& channelName){
    log_err("User not on channel " + channelName + "!");
    t_str str = "401 " + um.getNickname(socket)
                +  " " + channelName
                + " :You're not on that channel\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_BADCHANNELKEY(int socket, t_str_c& channelName){
    log_err("Bad channel key for " + channelName + "!");
    t_str str = "475 " + um.getNickname(socket)
                + " " + channelName
                + " :Cannot join channel (+k)\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_INVITEONLYCHAN(int socket, t_str_c& channelName){
    log_err("Tried to join invite-only channel " + channelName + "!");
    t_str str = "473 " + um.getNickname(socket)
                + " " + channelName
                + " :Cannot join channel (+j)\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_CHANNELISFULL(int socket, t_str_c& channelName){
    log_err("Tried to join full channel " + channelName + "!");
    t_str str =  "471 " + um.getNickname(socket)
                 + " " + channelName
                 + " :Cannot join channel (+l)\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_CHANOPRIVSNEEDED(int socket, t_str_c& channelName){
    log_err("Operator Privileges are needed in " + channelName + "!");
    t_str str = "482 " + um.getNickname(socket) + 
                + " " + channelName + " :You're not channel operator\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NOSUCHNICK(int socket, t_str_c& nicknameOrchannelName){
    log_err("No such nickname/channel on server ("
            + nicknameOrchannelName + ")!");
    t_str str = "401 " + um.getNickname(socket)
                + " " + nicknameOrchannelName + " :No such nick/channel\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_USERONCHANNEL(int socket, t_str_c& nickname, t_str_c& channelName){
    log_err("User " + nickname + " is already member of the channel " + channelName);
    /* @note nick or user name of target? */
    t_str str = "443 " + um.getNickname(socket) + " "
                + nickname + " " + channelName + " :is already on channel\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NORECIPIENT(int socket, t_str_c& command){
    log_err("No recipient was given with command " + command + "!");
    t_str str = "411 " + um.getNickname(socket)
                + " :No recipient given (" + command + ")\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

void Server::ERR_NOTEXTTOSEND(int socket){
    log_err("No text to send was given!");
    t_str str = "412 " + um.getNickname(socket)
                + " :No text to send\r\n";
    um.appendToBuffer(socket, str, OUTPUT);
}

// -------------------------------------------------------------------------- //
