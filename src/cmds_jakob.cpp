// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

void Server::CMD_CAP(int socket){
    if (m_parameters[0] == "LS"){
        RPL_CAP(socket);
    }
}

void Server::CMD_NICK(int socket){
    
    if (m_parameters.empty() == true){
        ERR_NONICKNAMEGIVEN(socket);
    }
    
    std::string const& newNickname= m_parameters[0];

    if (checkUnallowedCharacters(newNickname, UNALLOWED_NICK) == true){
        ERR_ERRONEUSNICKNAME(socket, newNickname);
    }
    else if (um.checkForNickname(newNickname) == true){
        ERR_NICKNAMEINUSE(socket, newNickname);
    }
    else{
        if (um.getNickname(socket).empty() == false){
            RPL_NICKCHANGE(socket, newNickname);
        }
        um.setNickname(socket, newNickname);
    }
}

void Server::CMD_USER(int socket){
    
    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
    }

    if (um.checkForUser(socket) == true
        && um.getUsername(socket).empty() == false){
        ERR_ALREADYREGISTRED(socket);
    }
    else{
        std::string const& username = m_parameters[0];
        RPL_WELCOME(socket, username);
        um.setUsername(socket, username);
    }
}

void Server::CMD_PING(int socket){
    std::string const& servername = m_parameters[0];
    RPL_PING(socket, servername);
}

void Server::CMD_QUIT(int socket){
    RPL_QUIT(socket);
}

void Server::CMD_JOIN(int socket){
    
    /* JOIN */
    /* channel */ /* , */ /* channel */
    /* key */ /* , */ /* key */

    
    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
    }

    if (m_parameters[0] == "0"){
        /* @note leave all channels */
        /* handle like PART command and reply accordingly */
    }
    else{
        
        // @note parse and test with test.cpp
        //
        // @note put all of this into ...
            // std::map<std::vector<std::string>, std::vector<std::string>>

        // @note loop through map and join/create channel with corresp. password
        
        std::string const& channelName = m_parameters[0];
        std::vector< std::pair<std::string, std::string> > channels;

        RPL_JOIN(socket, channelName, um.getUsername(socket));
    }
}

// -------------------------------------------------------------------------- //
