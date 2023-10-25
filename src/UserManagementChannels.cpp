// -------------------------------------------------------------------------- //

#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <iostream>  // needed for std::cout, std::endl
#include <sstream>   // needed for std::stringstream
#include <stdexcept> // needed for std::runtime_error

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> channel map operations */

void UserManagement::addChannel(std::string const& channelName){
  m_channels[channelName] = Channel(channelName);
}

void UserManagement::eraseChannel(std::string const& channelName){
	
    for (t_um_channels_it it = m_channels.begin();
                          it != m_channels.end();
                          ++it){
		if (it->second.getName() == channelName){
			m_channels.erase(it);
            return ;
		}
	}
    throw std::runtime_error("eraseChannel(): Channel "
                             + channelName + " not found!");
}

bool UserManagement::checkForChannel(std::string const& channelName) const{
    return m_channels.find(channelName) != m_channels.end();
}

void UserManagement::printChannelInfo(std::string const& channelName) const{
    
    t_um_channels_cit it = m_channels.find(channelName);
    if (it != m_channels.end()){
        std::cout << it->second
                  << "List of Users:   "
                  << getChannelUsernames(channelName)
                  << std::endl;
        return ;
    }
    throw std::runtime_error("printChannelInfo(): Channel "
                             + channelName + " not found!");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> channel map setters */

void UserManagement::addUserToChannel(int socket,
                                      UserPrivilege up,
                                      std::string const& channelName){
	
    t_um_channels_it it = m_channels.find(channelName);
    if (it != m_channels.end()){
        it->second.addUser(socket, up);
        return ;
    }
    throw std::runtime_error("addUserToChannel(): Channel "
                             + channelName + " not found!");
}

void UserManagement::eraseUserFromChannel(int socket,
                                          std::string const& channelName){
    
    t_um_channels_it it = m_channels.find(channelName);
    if (it != m_channels.end()){
        it->second.eraseUser(socket);
        return ;
    }
    throw std::runtime_error("eraseUserFromChannel(): Channel "
                             + channelName + " not found!");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> channel map getters */

Channel * UserManagement::getChannel(std::string const& channelName){
    
    t_um_channels_it it = m_channels.find(channelName);
    if (it != m_channels.end()){
        return &it->second;
    }
    throw std::runtime_error("getChannel(): Channel " 
                             + channelName + " not found!");
}

Channel const* UserManagement::getChannel(std::string const& channelName) const{
	
    t_um_channels_cit it = m_channels.find(channelName);
    if (it != m_channels.end()){
        return &it->second;
    }
    throw std::runtime_error("getChannel(): Channel "
                             + channelName + " not found!");
}

std::string UserManagement::getChannelNames() const{ 
	
    std::stringstream ss;
    if (m_channels.empty()){
        return "";
    }
    t_um_channels_cit last = --m_channels.end();
	for (t_um_channels_cit it = m_channels.begin();	
                           it != m_channels.end();
                           ++it){
		ss << it->second.getName();
		if (it != last){
			ss << ",";
		}
	}
    return ss.str();
}

std::string UserManagement::getChannelUsernames
                                        (std::string const& channelName) const{
    
    std::stringstream ss;
    Channel::t_channel_users UserMap = getChannel(channelName)->getUserMap();
    
    for (Channel::t_channel_users_cit it = UserMap.begin();
                                      it != UserMap.end();
                                      ++it){
        t_um_users_cit usr = m_users.find(it->first);
        if (usr != m_users.end()){
            if (it != UserMap.begin()){
                ss << ", ";
            }
            ss << usr->second.getUsername();
            if (it->second == OPERATOR){
                ss << "(o)";
            }
        }
    }
    return ss.str();
}

std::string UserManagement::getChannelNicknames
                                        (std::string const& channelName) const{
    
    std::stringstream ss;
    Channel::t_channel_users UserMap = getChannel(channelName)->getUserMap();
    
    for (Channel::t_channel_users_cit it = UserMap.begin();
                                      it != UserMap.end();
                                      ++it){
        t_um_users_cit usr = m_users.find(it->first);
        if (usr != m_users.end()){
            if (it != UserMap.begin()){
                ss << ", ";
            }
            ss << usr->second.getNickname();
            if (it->second == OPERATOR){
                ss << "(o)";
            }
        }
    }
    return ss.str();
}

// -------------------------------------------------------------------------- //
