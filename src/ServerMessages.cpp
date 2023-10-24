// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server messages */

void Server::CMD_CAP(int socket){
    if (m_parameters[0] == "LS"){
        RPL_CAP(socket);
    }
}

void Server::CMD_NICK(int socket){
    
    if (m_parameters.empty() == true){
        ERR_NONICKNAMEGIVEN(socket);
    }
    
    t_str_c& newNickname= m_parameters[0];

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
        t_str_c& username = m_parameters[0];
        RPL_WELCOME(socket, username);
        um.setUsername(socket, username);
    }
}

void Server::CMD_PING(int socket){
    t_str_c& servername = m_parameters[0];
    RPL_PING(socket, servername);
}

void Server::CMD_QUIT(int socket){
    RPL_QUIT(socket);
}

void Server::CMD_JOIN(int socket){
    
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

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server messages helpers */

void Server::createChannelBy(int socket,
                             t_str_c& channelName,
                             t_str_c& channelKey){

    /* not sure if this... */
    ERR_NOSUCHCHANNEL(socket, channelName);
    
    if (channelName.size() >= 50
        || channelName.find_first_of(CHAR_ALLOWED_CHANNEL) != 0
        || channelName.find_first_of(" ,\a") != t_str::npos){
        /* ... should go here instead */
        return ;
    }

    um.addChannel(channelName);
    um.addUserToChannel(socket, OPERATOR, channelName);

    if (channelKey.empty() == false){
        Channel *c = um.getChannel(channelName);
        c->setPassword(channelKey);
        c->toggleChannelKey();
        log("Channel "+ channelName
            + " created by " + um.getNickname(socket) +
            " (password-protected)");
    }
    else{
        log("Channel "+ channelName + " created by " + um.getNickname(socket));
    }

    RPL_JOIN(socket, channelName);
    RPL_NOTOPIC(socket, channelName);
    RPL_NAMREPLY(socket, channelName, um.getNickname(socket));

}

void Server::addUserToChannels(int socket,
                               t_vec_str_c& channelNames,
                               t_vec_str_c& channelKeys){

    t_vec_str_cit key = channelKeys.begin();

    for (t_vec_str_cit it = channelNames.begin();
                       it != channelNames.end();
                       ++it){
        
        /* @note possibly need to update this on an error */
        /* @note should be fine though if its here */
        t_str enteredKey;
        if (key != channelKeys.end()){
            enteredKey = *key++;
        }
    
        t_str_c& channelName = *it;
        if (um.checkForChannel(channelName) == false){
       
            createChannelBy(socket, channelName, enteredKey);
            continue ;

        }
        
        Channel const* channel = um.getChannel(channelName);
        if (channel->isInviteOnly() == true){
            
            ERR_INVITEONLYCHAN(socket, channelName);
            continue ;

        }

        if (channel->isFull() == true){
            
            ERR_CHANNELISFULL(socket, channelName);
            continue;

        }

        if (enteredKey.empty() == false && channel->isChannelKey() == false){
            log_err("Received password for non_pw channel");
        }
        else if (channel->isChannelKey() == true){
            if (channel->getPassword() != enteredKey){
                ERR_BADCHANNELKEY(socket, channel->getName());
                continue ;
            }
            um.addUserToChannel(socket, USER, channelName);
        }
        else{
            um.addUserToChannel(socket, USER, channelName);
        }
        RPL_JOIN(socket, channel->getName());
        RPL_IFTOPIC(socket, channel->getName(), channel->getTopic());
    }
}

void Server::eraseUserFromAllChannels(int socket){

    t_vec_str channels = split(um.getChannelNames(), ',');
    log_vector("all channels", channels); 
    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it){
        t_str_c& channelName = *it;
        if (um.getChannel(channelName)->isMember(socket) == true){
            um.eraseUserFromChannel(socket, channelName);
            RPL_PART(socket, channelName, getPartMessage());
        }
    }
}

// -------------------------------------------------------------------------- //
