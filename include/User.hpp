// -------------------------------------------------------------------------- //

#ifndef USER_HPP
#define USER_HPP

#include "UserPrivilege.hpp" // needed for ENUM
#include <string>            // needed for std::string

/* @note can prob remove the m_privilege + associated functions */

class User {
private:
  std::string m_nickname;
  std::string m_username;

public:
  void setNickname(std::string const &nickname);

  std::string const &getNickname() const;
  std::string const &getUsername() const;
  
  User(std::string const& nickname, std::string const& username);
  User();
  User& operator=(const User& src);
  User(const User &copy);
  ~User();
};

std::ostream& operator<<(std::ostream &os, const User &user);

#endif // USER_HPP

// -------------------------------------------------------------------------- //
