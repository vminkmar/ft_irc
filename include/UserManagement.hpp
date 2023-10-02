#ifndef USERMANAGEMENT_HPP
#define USERMANAGEMENT_HPP
#include "enumPrivileges.hpp"
#include "user.hpp"
#include <map>
#include <string>
#include "responses.hpp"

class User;

class UserManagement {
private:
  std::map<int, User> m_users;

public:
  void addUser(int socket_fd, std::string nickname, std::string username,
               UserPrivilege privilege);
  void eraseUser(int socket_fd);
	void print();
	bool checkForUser(int socket);


	int getNumberUsers() const; //?
	std::string getNumberUsersAsString();
	std::string getNick(int socket);

	void setNick(int socket, std::string parameter);
	void setUser(int socket, std::string parameter);
};

#endif