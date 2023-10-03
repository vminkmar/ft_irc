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

static inline void print_log(std::string message) {
  if (DEBUG) {
    std::cerr << YELLOW << "User Management: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

UserManagement::UserManagement() { print_log("default constructor called"); }

UserManagement::~UserManagement() { print_log("destructor called"); }

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> private member functions */

bool UserManagement::checkForUser(int socket) const{
  for (t_um_users_cit it = m_users.begin();
                      it != m_users.end();
                      it++) {
    if (it->first == socket){
      return true;
    }
  }
  return false;
}

bool UserManagement::checkForChannel(std::string name) const{
	for (t_um_channels_cit it = m_channels.begin();
                           it != m_channels.end();
                           ++it){
		if (it->second.getName() == name){
			return true;
		}
	}
	return false;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> public member functions */

void UserManagement::setNick(int socket, std::string parameter) {
  for (t_um_users_it it = m_users.begin(); it != m_users.end(); ++it) {
    if (it->first == socket) {
      it->second.setNickname(parameter);
    }
  }
}

void UserManagement::setUser(int socket, std::string parameter){
  for (t_um_users_it it = m_users.begin(); it != m_users.end(); ++it) {
    if (it->first == socket) {
      it->second.setUsername(parameter);
    }
  }
}

std::string UserManagement::getNick(int socket) const{
  for (t_um_users_cit it = m_users.begin(); it != m_users.end(); it++){
    if (it->first == socket){
      return it->second.getNickname();
    }
  }
  return "";
}

std::string UserManagement::getNumberUsersAsString() const{
  std::stringstream ss;
  ss << m_numberUsers;
  return ss.str();
}

int UserManagement::getNumberUsers() const { return (m_numberUsers); }

std::string UserManagement::getUsernames() const{
    std::stringstream ss;
    for (t_um_users_cit itr = m_users.begin(); itr != m_users.end(); ++itr){
        ss << itr->second.getUsername();
        if (itr != --m_users.end()){
            ss << ", ";
        }
    }
    return ss.str();
}

void UserManagement::addUser(int socket,
                             std::string const &nickname,
                             std::string const &username){
  User user(nickname, username);
  m_users[socket] = user;
}

void UserManagement::eraseUser(int socket){
    for (t_um_channels_it itr = m_channels.begin();
                          itr != m_channels.end();
                          ++itr){
        eraseUserFromChannel(socket, itr->first);
    }
    m_users.erase(m_users.find(socket));
}

void UserManagement::appendToBuffer(std::string message, int socket, int flag) {
  for (t_um_users_it it = m_users.begin(); it != m_users.end(); ++it) {
    if (socket == it->first && flag == INPUT)
      it->second.appendInputBuffer(message);
    else if (socket == it->first && flag == OUTPUT)
      it->second.appendOutputBuffer(message);
  }
}

void UserManagement::addChannel(std::string name) {
  if (checkForChannel(name) == true) {
    /* @note impl error message */
    return;
  }
  m_channels[name] = Channel(name);
}

void UserManagement::eraseChannel(std::string name){
	for (t_um_channels_it it = m_channels.begin();
                          it != m_channels.end();
                          ++it){
		if (it->second.getName() == name){
			m_channels.erase(it);
		}
	}
}

Channel const& UserManagement::getChannel(std::string name) const{
	return m_channels.find(name)->second;
}

std::string UserManagement::getChannelNames() const{ 
	std::stringstream ss;
	for (t_um_channels_cit it = m_channels.begin();	
                           it != m_channels.end();
                           ++it){
		ss << it->second.getName();
		if (it != --m_channels.end()){
			ss << ", ";
		}
	}
    return ss.str();
}

std::string UserManagement::getChannelUsernames(std::string channelName) const {
    std::stringstream ss;
    Channel::t_channel_users UserMap = getChannel(channelName).getUserMap();
    for (t_um_users_cit itr = m_users.begin(); itr != m_users.end(); ++itr){
        for (Channel::t_channel_users_cit it = UserMap.begin();
                                          it != UserMap.end();
                                          ++it){
            if (itr->first == it->first){
                ss << itr->second.getUsername();
                if (it->second == OPERATOR){
                    ss << "(o)";
                }
                if (itr != --m_users.end()){
                    ss << ", ";
                }
            }
        }
    }
    return ss.str();
}

void UserManagement::printChannelInfo(std::string channelName) const{
    std::cout << m_channels.find(channelName)->second
              << "List of Users:   "
              << getChannelUsernames(channelName)
              << std::endl;
}

void UserManagement::listChannels() const{
	std::cout << getChannelNames() << std::endl;
}

void UserManagement::addUserToChannel(int socket,
                                      UserPrivilege up,
                                      std::string channelName){
	m_channels[channelName].addUser(socket, up);
}

void UserManagement::eraseUserFromChannel(int socket, std::string channelName){
    m_channels[channelName].eraseUser(socket);
}

// void UserManagement::print() {
//   for (std::map<int, User>::iterator it = this->m_users.begin();
//        it != this->m_users.end(); it++)
//     std::cout << it->first << "	" << it->second << std::endl;
// }

// -------------------------------------------------------------------------- //
