// -------------------------------------------------------------------------- //

#include "../include/UserManagement.hpp" // needed for UserManagement class

#include <sstream> // needed for std::stringstream

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user map operations */

void UserManagement::addUser(int socket){
  m_users[socket] = User(); /* c11 std::map.emplace */
	setOnlineStatus(socket, true);
}

void UserManagement::eraseUser(int socket){
    for (t_um_channels_it it = m_channels.begin();
                          it != m_channels.end();
                          ++it){
        eraseUserFromChannel(socket, it->first);
    }
    t_um_users_it user = m_users.find(socket);
    if (user != m_users.end()){
        m_users.erase(user);
    }
}

bool UserManagement::checkForUser(int socket) const{
    return m_users.find(socket) != m_users.end();
}

bool UserManagement::checkForUsername(std::string username) const{
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it){
        if(it->second.getUsername() == username){
            return true;
        }
    }
    return false;
}

bool UserManagement::checkForNickname(std::string nickname) const{
	for(t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it){
		if(it->second.getNickname() == nickname){
			return true;
        }
    }
	return false;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> setters */

void UserManagement::setUsername(int socket, std::string newUsername){
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end()){
        it->second.setUsername(newUsername);
    }
}

void UserManagement::setNickname(int socket, std::string newNickname){
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end()){
        it->second.setNickname(newNickname);
    }
}

void UserManagement::setOnlineStatus(int socket, bool flag){
  
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end()){
        if (it->second.getOnlineStatus() != flag){
            it->second.toggleOnlineStatus();
        }
    }
    /* @note throw exception! */
}

void UserManagement::appendToBuffer(std::string message, int socket, int flag){
    if (flag == INPUT){
        m_users.find(socket)->second.appendInputBuffer(message);
    }
    else if (flag == OUTPUT){
        m_users.find(socket)->second.appendOutputBuffer(message);
    }
}

void UserManagement::eraseBuffer(int socket, int flag, int start, int end){
	if (flag == INPUT)
    {
        m_users.find(socket)->second.eraseInputBuffer(start, end);
    }else if (flag == OUTPUT){
        m_users.find(socket)->second.eraseOutputBuffer(start, end);
    }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> getters */

std::string UserManagement::getNickname(int socket) const{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end()){
        return it->second.getNickname();
    }
    return "";
}

std::string UserManagement::getNicknames() const{
    if (m_users.empty()){
        return "";
    }
    std::stringstream ss;
    t_um_users_cit last = --m_users.end();
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it){
        ss << it->second.getNickname();
        if (it != last){
            ss << ", ";
        }
    }
    return ss.str();
}

std::string UserManagement::getUsername(int socket) const {
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end()){
        return it->second.getUsername();
    }
  return "";
}

std::string UserManagement::getUsernames() const{
    if (m_users.empty()){
        return "";
    }
    std::stringstream ss;
    t_um_users_cit last = --m_users.end();
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it){
        ss << it->second.getUsername();
        if (it != last){
            ss << ", ";
        }
    }
    return ss.str();
}

std::string UserManagement::getBuffer(int socket, int flag) const{
    if (flag == INPUT){
        return m_users.find(socket)->second.getInputBuffer();
    }
    else if (flag == OUTPUT){
        return m_users.find(socket)->second.getOutputBuffer();
    }
    return "";
}

bool UserManagement::getOnlineStatus(int socket) const{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end()){
        return it->second.getOnlineStatus();
    }
    return false; /* @note throw exception! */
}

// -------------------------------------------------------------------------- //
