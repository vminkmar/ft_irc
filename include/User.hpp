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
	bool m_online;


public:
	//append
	//searchMessage
	

	void eraseOutputBuffer(int start, int end);
	void eraseInputBuffer(int start, int end);
	void appendOutputBuffer(std::string const& message);
	void appendInputBuffer(std::string const& message);
	void searchMessage(std::string buffer);
  void setNickname(std::string const& nickname);
  void setUsername(std::string const& username);
	bool getOnline() const;
	void setOnline(bool status);
  std::string const &getNickname() const;
  std::string const &getUsername() const;

  std::string const& getInputBuffer() const;
  std::string const& getOutputBuffer() const;
  
  User();
  // User(std::string const& nickname, std::string const& username);
  User& operator=(const User& src);
  User(const User &copy);
  ~User();

};

std::ostream& operator<<(std::ostream &os, const User &user);

#endif // USER_HPP

// -------------------------------------------------------------------------- //
