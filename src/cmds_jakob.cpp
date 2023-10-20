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

    if (checkUnallowedCharacters(newNickname) == true){
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

// -------------------------------------------------------------------------- //
