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

/* @note not used right now */
// static inline void print_log(std::string message)
//{
//     if (DEBUG)
//     {
//         std::cerr << YELLOW << "Channel: "
//                   << message << RESET << std::endl;
//     }
// }

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

// UserManagement::UserManagement() : m_numberUsers(0){}
// UserManagement::~UserManagement(){}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void UserManagement::setNick(int socket, std::string parameter) {
  for (std::map<int, User>::iterator it = m_users.begin(); it != m_users.end();
       ++it) {
    if (it->first == socket) {
      it->second.setNickname(parameter);
    }
  }
}

std::string UserManagement::getNick(int socket) const {
  for (std::map<int, User>::const_iterator it = m_users.begin();
       it != m_users.end(); it++) {
    if (it->first == socket) {
      return it->second.getNickname();
    }
  }
  return "";
}

std::string UserManagement::getNumberUsersAsString() const {
  std::stringstream ss;

  ss << m_numberUsers;
  return ss.str();
}

int UserManagement::getNumberUsers() const { return (m_numberUsers); }

void UserManagement::addUser(int socket_fd, std::string const &nickname,
                             std::string const &username) {
  User user(nickname, username);
  this->m_users[socket_fd] = user;
}

void UserManagement::eraseUser(int socket_fd) {
  std::map<int, User>::iterator it = this->m_users.find(socket_fd);
  this->m_users.erase(it);
}

void UserManagement::addChannel(std::string name)
{
	m_channels.push_back(Channel(name));
}

void UserManagement::eraseChannel(std::string name)
{
	for (std::vector<Channel>::iterator itr = m_channels.begin();
			itr != m_channels.end(); ++itr)
	{
		if (itr->getName() == name)
		{
			m_channels.erase(itr);
		}
	}
}

//Channel const& UserManagement::getChannel(std::string const& name) const
//{
//	for (std::vector<Channel>::const_iterator itr = m_channels.begin();
//			itr != m_channels.end(); ++itr)
//	{
//
//	}
//	return NULL;
//}

void UserManagement::listChannels() const
{
	std::stringstream ss;
	for (std::vector<Channel>::const_iterator itr = m_channels.begin();
			itr != m_channels.end(); ++itr)
	{
		ss << itr->getName();
		if (itr != --m_channels.end())
		{
			ss << ", ";
		}
	}
	std::cout << ss.str() << std::endl;
}

void UserManagement::print() {
  for (std::map<int, User>::iterator it = this->m_users.begin();
       it != this->m_users.end(); it++)
    std::cout << it->first << "	" << it->second << std::endl;
}

bool UserManagement::checkForUser(int socket) {
  for (std::map<int, User>::iterator it = this->m_users.begin();
       it != this->m_users.end(); it++) {
    if (it->first == socket)
      return true;
  }
  return false;
}
