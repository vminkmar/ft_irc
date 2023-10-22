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

    /* channelnames */
    /* must begin with "&, #, + or !" */
    /* up to 50 length */
    /* shall not contain: ' ', ^G, ',' */

    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
    }
    else if (m_parameters[0] == "0"){
        t_vec_str channels = split(um.getChannelNames(), ',');
        log_vector("channels", channels); 
        for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it){
            if (um.getChannel(*it).isMember(socket)){
                um.eraseUserFromChannel(socket, *it);
                RPL_PART(socket, *it, getPartMessage());
            }
        }
    }
    else{
        
        t_vec_str channelNames = split(m_parameters[0], ',');
        log_vector("channels", channelNames);
       
        t_vec_str channelKeys;
        if (m_parameters.size() >= 2){
            channelKeys = split(m_parameters[1], ',');
            log_vector("channelKeys", channelKeys);
        }

        t_vec_str_cit key;
        key = channelKeys.begin();

        for (t_vec_str_cit it = channelNames.begin();
                           it != channelNames.end();
                           ++it){
            if (um.checkForChannel(*it) == false){
                
                ERR_NOSUCHCHANNEL(socket, *it);
                um.addChannel(*it);
                log("Channel "+ *it + " created");
                um.addUserToChannel(socket, OPERATOR, *it);
                RPL_JOIN(socket, *it);
                RPL_NOTOPIC(socket, *it);
                RPL_NAMREPLY(socket, *it, um.getNickname(socket));
            }
            else{
                
                Channel const& channel = um.getChannel(*it);
                std::string const& topic = channel.getTopic();

                std::string passw;
                if (key != channelKeys.end()){
                    passw = *key;
                    ++key;
                }
                if (passw.empty() == false){
                    if (channel.isChannelKey() == false){
                        log_err("Received password for non_pw channel");
                    }
                    else{
                        if (channel.getPassword() != passw){
                            ERR_BADCHANNELKEY(socket, channel.getName());
                            return ;
                        }
                        um.addUserToChannel(socket, USER, *it);
                    }
                }
                else{
                    um.addUserToChannel(socket, USER, *it);
                }
                RPL_JOIN(socket, channel.getName());
                if (topic.empty() == true){
                    RPL_NOTOPIC(socket, channel.getName());
                }
                else{
                    RPL_TOPIC(socket, channel.getName(), topic);
                }
            }
        }
    }
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
        }
        else{
            Channel const& channel = um.getChannel(*it);
            if (channel.isMember(socket) == false){
                ERR_NOTONCHANNEL(socket, *it);
            }
            else{
                um.eraseUserFromChannel(socket, *it);
                RPL_PART(socket, *it, getPartMessage());
            }
        }
    }
}

// -------------------------------------------------------------------------- //
