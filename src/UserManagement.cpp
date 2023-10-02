#include "UserManagement.hpp"

int m_numberUsers = 0;

// UserManagement::UserManagement() : m_numberUsers(0){}

// UserManagement::~UserManagement(){}

void UserManagement::addUser(int socket_fd, std::string nickname,
                             std::string username, UserPrivilege privilege) {
  User user(nickname, username, privilege);
  this->m_users[socket_fd] = user;
}

void UserManagement::eraseUser(int socket_fd) {
  std::map<int, User>::iterator it = this->m_users.find(socket_fd);
  this->m_users.erase(it);
}

void UserManagement::print() {
  for (std::map<int, User>::iterator it = this->m_users.begin(); it != this->m_users.end();
       it++)
    std::cout << it->first << "	" << it->second << std::endl;
}

int UserManagement::getNumberUsers() const { return (m_numberUsers); }

std::string UserManagement::getNumberUsersAsString() {
  std::string str = std::to_string(m_numberUsers);
  return (str);
}

std::string UserManagement::getNick(int socket) {
  for (std::map<int, User>::iterator it = this->m_users.begin(); it != this->m_users.end();
       it++) {
    if (it->first == socket)
      return it->second.getNickname();
  }
  return "";
}

void UserManagement::setNick(int socket, std::string parameter) {
  for (std::map<int, User>::iterator it = this->m_users.begin(); it != this->m_users.end();
       it++) {
    if (it->first == socket)
      it->second.setNickname(parameter);
  }
}

void UserManagement::setUser(int socket, std::string parameter) {
  for (std::map<int, User>::iterator it = this->m_users.begin(); it != this->m_users.end();
       it++) {
    if (it->first == socket)
      it->second.setUsername(parameter);
  }
}

bool UserManagement::checkForUser(int socket) {
  for (std::map<int, User>::iterator it = this->m_users.begin(); it != this->m_users.end();
       it++) {
    if (it->first == socket)
      return true;
  }
  return false;
}