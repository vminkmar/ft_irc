// -------------------------------------------------------------------------- //

#include "../include/Server.hpp" // needed for Server class

#include <sstream> // needed for std::stringstream

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
        /* @note return? */
    }

    if (m_parameters[0] == "0"){
        /* @note leave all channels */
        /* handle like PART command and reply accordingly */
    }
    else{
            
        std::vector<std::string>channelNames = split(m_parameters[0], ',');
        log_vector("channelNames", channelNames);
       
        std::vector<std::string>channelKeys;
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
                
                /* @note if no channel is found... */
                    /* ERR message */
                    /* add Channel */
                    /* add user to channel as operator */
                    /* RPL_NOTOPIC */
                    /* RPL_NAMRELPY */

                ERR_NOSUCHCHANNEL(socket, *it);
                um.addChannel(*it);
                um.addUserToChannel(socket, OPERATOR, *it);
                RPL_JOIN(socket, *it);
                RPL_NOTOPIC(socket, *it);
                RPL_NAMREPLY(socket, *it, um.getNickname(socket));
            }
            else{
                
                Channel const& channel = um.getChannel(*it);
                std::string const& topic = channel.getTopic();
                (void) topic;

                std::string passw;
                if (key != channelKeys.end()){
                    passw = *key;
                    ++key;
                }

                if (passw.empty() == false){

                    if (channel.isChannelKey() == false){
                        /* some kind of error! */
                    }
                    else{
                        if (channel.getPassword() != passw){
                            /* ERR_BADCHANNELKEY */
                        }
                        um.addUserToChannel(socket, USER, *it);
                    }
                }
                else{
                    um.addUserToChannel(socket, USER, *it);
                }
            }
        }
        //RPL_JOIN(socket, channelName, um.getUsername(socket));
    }
}

void Server::CMD_PART(int socket){

    /* PART */
    /* Channel, Channel, Channel */
    
    if (m_parameters.empty() == true){
        ERR_NEEDMOREPARAMS(socket, m_command);
    }


    std::vector<std::string> channels = split(m_parameters[0], ',');

    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it){
        if (um.checkForChannel(*it) == false){
            ERR_NOSUCHCHANNEL(socket, *it);
        }
        else{
            Channel const& channel = um.getChannel(*it);
            if (channel.isMember(socket) == false){
                ERR_NOTONCHANNEL(socket, *it);
            }
            um.eraseUserFromChannel(socket, *it);

            std::string partMessage;
            if (m_parameters.size() >= 2){

                std::stringstream ss;
                
                for (t_vec_str_cit it = m_parameters.begin();
                                   it != m_parameters.end();
                                   ++it){
                    ss << *it;
                }
                partMessage = ss.str();
            }
            else{
                partMessage = DEFMSG_PART;
            }
            RPL_PART(socket, *it, partMessage);
        }
    }
}

// -------------------------------------------------------------------------- //
