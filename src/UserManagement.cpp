// -------------------------------------------------------------------------- //

#include "../include/UserManagement.hpp" // needed for UserManagement class
#include <iostream>                      // needed for std::cout, std::endl
#include <sstream>                       // needed for stringstream

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> global variables */

int m_numberUsers = 0; /* could be a static int member of the class */

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message){
  if (DEBUG){ 
    std::cerr << YELLOW << "User Management: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

UserManagement::UserManagement(){
    print_log("default constructor called");
}

UserManagement::~UserManagement(){
    print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> private member functions */

bool UserManagement::checkForUser(int socket) const{
    return m_users.find(socket) != m_users.end();
}

bool UserManagement::checkForChannel(std::string channelName) const{
    return m_channels.find(channelName) != m_channels.end();
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user operations */

void UserManagement::setNick(int socket, std::string newNickname){
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end()){
        it->second.setNickname(newNickname);
    }
}

void UserManagement::setUser(int socket, std::string newUsername){
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end()){
        it->second.setUsername(newUsername);
    }
}

std::string UserManagement::getNick(int socket) const{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end()){
        return it->second.getNickname();
    }
    return "";
}

std::string UserManagement::getUser(int socket) const {
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end()){
        return it->second.getUsername();
    }
  return "";
}


std::string UserManagement::getNumberUsersAsString() const {
  std::stringstream ss;
  ss << m_numberUsers;
  return ss.str();
}

int UserManagement::getNumberUsers() const{
    return (m_numberUsers);
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

void UserManagement::addUser(int socket){
  m_users[socket] = User(); /* c11 std::map.emplace */
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

std::string UserManagement::getBuffer(int socket, int flag){
    if (flag == INPUT){
        return m_users.find(socket)->second.getInputBuffer();
    }
    else if (flag == OUTPUT){
        return m_users.find(socket)->second.getOutputBuffer();
    }
    return "";
}

int UserManagement::getSize() const{
	return m_users.size();
}

bool UserManagement::getOnlineStatus(int socket) const{
    t_um_users_cit it = m_users.find(socket);

    if (it != m_users.end()){
        return it->second.getOnlineStatus();
    }
    return false; /* @note throw exception! */
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

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> channel operations */

void UserManagement::addChannel(std::string channelName){
  m_channels[channelName] = Channel(channelName);
}

void UserManagement::eraseChannel(std::string channelName){
	for (t_um_channels_it it = m_channels.begin();
                          it != m_channels.end();
                          ++it){
		if (it->second.getName() == channelName){
			m_channels.erase(it);
            break ;
		}
	}
}

Channel const& UserManagement::getChannel(std::string channelName) const{
	
    t_um_channels_cit it = m_channels.find(channelName);
    if (it == m_channels.end()){
        std::cout << "Channel not found: " << channelName << std::endl;
    }
    return it->second;
    /* impl error handling! */
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
			ss << ", ";
		}
	}
    return ss.str();
}

std::string UserManagement::getChannelUsernames(std::string channelName) const{
    std::stringstream ss;
    Channel::t_channel_users UserMap = getChannel(channelName).getUserMap();
    
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

void UserManagement::printChannelInfo(std::string channelName) const{
    
    t_um_channels_cit it = m_channels.find(channelName);
    if (it != m_channels.end()){
        std::cout << it->second
                  << "List of Users:   "
                  << getChannelUsernames(channelName)
                  << std::endl;
    }
    else {
    std::cout << "Channel not found: " << channelName << std::endl;
    }
}

void UserManagement::listChannels() const {
  std::cout << getChannelNames() << std::endl;
}

void UserManagement::addUserToChannel(int socket,
                                      UserPrivilege up,
                                      std::string channelName){
	
    t_um_channels_it it = m_channels.find(channelName);
    if (it != m_channels.end()){
        it->second.addUser(socket, up);
    }
    else{
    std::cout << "Channel not found: " << channelName << std::endl;
    }
}

void UserManagement::eraseUserFromChannel(int socket, std::string channelName){
    
    t_um_channels_it it = m_channels.find(channelName);
    if (it != m_channels.end()){
        it->second.eraseUser(socket);
    }
    else{
    std::cout << "Channel not found: " << channelName << std::endl;
    }
}

// -------------------------------------------------------------------------- //
