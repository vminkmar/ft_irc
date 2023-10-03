// -------------------------------------------------------------------------- //

#ifndef USER_HPP
#define USER_HPP

#include "UserPrivilege.hpp" // needed for ENUM
#include <string>            // needed for std::string

class User {
private:
  std::string m_nickname;
  std::string m_username;
	std::string m_inputBuffer;
	std::string m_outputBuffer;


public:
	//append
	//searchMessage
	


	void appendOutputBuffer(std::string message);
	void appendInputBuffer(std::string message);
	void searchMessage(std::string buffer);
  void setNickname(std::string const& nickname);
  void setUsername(std::string const& username);

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
