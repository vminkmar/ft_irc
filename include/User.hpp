#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

//#include "UserManagement.hpp"

#include "UserPrivilege.hpp"
#include "Responses.hpp"

class User {
private:
  std::string m_nickname;
  std::string m_username;
  UserPrivilege m_privilege;

public:
  User();
  User(std::string nickname, std::string username, UserPrivilege privilege);
  User(const User &copy);
  ~User();

  void setNickname(std::string nickname);
  void setUsername(std::string username);

	std::string getNickname() const;
	std::string getUsername() const;
  UserPrivilege getPrivilege() const;
  };

  std::ostream &operator<<(std::ostream &os, const User &user);
#endif
