#ifndef USER_HPP
#define USER_HPP
#include "UserManagement.hpp"
#include "enumPriviledges.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "responses.hpp"

class User {
private:
  std::string m_nickname;
  std::string m_username;
  UserPriviledge m_priviledge;

public:
  User();
  User(std::string nickname, std::string username, UserPriviledge priviledge);
  User(const User &copy);
  ~User();

  void setNickname(std::string nickname);
  void setUsername(std::string username);

	std::string getNickname() const;
	std::string getUsername() const;
  UserPriviledge getPriviledge() const;
  };

  std::ostream &operator<<(std::ostream &os, const User &user);
#endif