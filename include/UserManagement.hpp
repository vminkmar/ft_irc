#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP
#include "enumPriviledges.hpp"
#include "user.hpp"
#include <map>
#include <string>

class User;

class UserManagement {
private:
  std::map<int, User> users;

public:
  void addUser(int socket_fd, std::string nickname, std::string username,
               UserPriviledge priviledge);
  void eraseUser(int socket_fd);
	void print();
};

#endif