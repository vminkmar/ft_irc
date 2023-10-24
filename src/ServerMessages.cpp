// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

//#include <sstream> // needed for std::stringstream
//#include <iostream> // @note debug

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

    if (checkUnallowedCharacters(newNickname, CHAR_UNALLOWED_NICK) == true){
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

    /* channelnames */
    /* must begin with "&, #, + or !" */
    /* up to 50 length */
    /* shall not contain: ' ', ^G, ',' */

    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
        return ;
    }
    
    if (m_parameters[0] == "0"){
        eraseUserFromAllChannels(socket);
        return ;
    }

    t_vec_str channelNames = split(m_parameters[0], ',');
    t_vec_str channelKeys;
    if (m_parameters.size() >= 2){
        channelKeys = split(m_parameters[1], ',');
    }

    addUserToChannels(socket, channelNames, channelKeys);

}

void Server::CMD_PART(int socket){ 
    
    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
        return ;
    }

    t_vec_str channels = split(m_parameters[0], ',');
    log_vector("channels", channels);

    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it){
        if (um.checkForChannel(*it) == false){
            ERR_NOSUCHCHANNEL(socket, *it);
            continue ;
        }
        Channel const* channel = um.getChannel(*it);
        if (channel->isMember(socket) == false){
            ERR_NOTONCHANNEL(socket, *it);
            continue ;
        }
        um.eraseUserFromChannel(socket, *it);
        RPL_PART(socket, *it, getPartMessage());
    }
}

// -------------------------------------------------------------------------- //
