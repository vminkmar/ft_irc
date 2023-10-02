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

static inline void print_log(std::string message)
{
	if (DEBUG)
	{
		std::cerr << YELLOW << "User Management: "
			      << message << RESET << std::endl;
	}
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

UserManagement::UserManagement()
{
	print_log("default constructor called");
}

UserManagement::~UserManagement()
{
	print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

bool UserManagement::checkForUser(int socket) const{
  for (std::map<int, User>::const_iterator it = this->m_users.begin();
       it != this->m_users.end(); it++) {
    if (it->first == socket)
      return true;
  }
  return false;
}

bool UserManagement::checkForChannel(std::string name) const{
	for (std::map<std::string, Channel>::const_iterator itr = m_channels.begin();
			itr != m_channels.end(); ++itr)
	{
		if (itr->second.getName() == name)
		{
			return true;
		}
	}
	return false;
}

void UserManagement::setNick(int socket, std::string parameter) {
  for (std::map<int, User>::iterator it = m_users.begin(); it != m_users.end();
       ++it) {
    if (it->first == socket) {
      it->second.setNickname(parameter);
    }
  }
}

void UserManagement::setUser(int socket, std::string parameter){
  for (std::map<int, User>::iterator it = m_users.begin(); it != m_users.end();
       ++it) {
    if (it->first == socket) {
      it->second.setUsername(parameter);
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
	if (checkForChannel(name) == true)
	{
		/* @note impl error message */
		return ;
	}
	m_channels[name] = Channel(name);
}

void UserManagement::eraseChannel(std::string name)
{
	for (std::map<std::string, Channel>::iterator itr = m_channels.begin();
			itr != m_channels.end(); ++itr)
	{
		if (itr->second.getName() == name)
		{
			m_channels.erase(itr);
		}
	}
	/* @note impl error message */
}

Channel const& UserManagement::getChannel(std::string name)
{
	return m_channels[name];
}

void UserManagement::listChannels() const
{
	std::stringstream ss;
	for (std::map<std::string, Channel>::const_iterator itr = m_channels.begin();
			itr != m_channels.end(); ++itr)
	{
		ss << itr->second.getName();
		if (itr != --m_channels.end())
		{
			ss << ", ";
		}
	}
	std::cout << ss.str() << std::endl;
}

void UserManagement::addUsertoChannel(int socket,
		UserPrivilege up, std::string channelName)
{
	m_channels[channelName].addUser(socket, up);
}

void UserManagement::print() {
  for (std::map<int, User>::iterator it = this->m_users.begin();
       it != this->m_users.end(); it++)
    std::cout << it->first << "	" << it->second << std::endl;
}

// -------------------------------------------------------------------------- //
