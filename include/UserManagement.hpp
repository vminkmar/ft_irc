#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP

#include <map>     // needed for std::map
#include <string>  // needed for std::string

#include "EnumPrivileges.hpp"
#include "User.hpp"
#include "Responses.hpp"

class User;

class UserManagement {
private:
  std::map<int, User> m_users;

public:
  void addUser(int socket_fd, std::string nickname, std::string username,
               UserPrivilege privilege);
  void eraseUser(int socket_fd);
	void print();
	int getNumberUsers() const;



	std::string getNumberUsersAsString();
	std::string getNick(int socket);

	void setNick(int socket, std::string parameter);
};

#endif
