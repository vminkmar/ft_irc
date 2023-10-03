// -------------------------------------------------------------------------- //

#include "../include/User.hpp" // needed for User class, std::string, ENUM
#include <iostream>            // needed for std::ostream

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string const& message) {
  if (DEBUG) {
    std::cerr << YELLOW << "User: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

User::User(std::string const& nickname, std::string const& username)
    : m_nickname(nickname), m_username(username) {
  print_log("main constructor called");
}

User::User(){
  print_log("default constructor called");
}

User::User(const User &copy) : m_nickname(copy.m_nickname),
                               m_username(copy.m_username){
  print_log("copy constructor called");
}

User::~User() { print_log("destructor called"); }

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

std::ostream &operator<<(std::ostream &os, const User &user) {
  os << "Nickname: " << user.getNickname()
     << ", Username: " << user.getUsername();
  return os;
}

User& User::operator=(const User& src){
	print_log("assignment operator called");
	if (this != &src){
		this->m_nickname = src.getNickname();
		this->m_username = src.getUsername();
	}
	return *this;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void User::appendOutputBuffer(std::string message){
	m_outputBuffer.append(message);
}

void User::appendInputBuffer(std::string message){
	m_outputBuffer.append(message);
}




void User::setNickname(std::string const& nickname) { m_nickname = nickname; }
void User::setUsername(std::string const& username) { m_username = username; }

std::string const &User::getNickname() const { return m_nickname; }
std::string const &User::getUsername() const { return m_username; }

// -------------------------------------------------------------------------- //
