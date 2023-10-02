// -------------------------------------------------------------------------- //

#include "../include/User.hpp" // needed for User class, std::string, ENUM
#include <iostream>            // needed for std::ostream

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message) {
  if (DEBUG) {
    std::cerr << YELLOW << "User: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

User::User() : m_nickname(""), m_username("") {
  print_log("default constructor called");
}

User::User(std::string nickname, std::string username)
    : m_nickname(nickname), m_username(username) {
  print_log("main constructor called");
}

User::User(const User &copy) {
  print_log("copy constructor called");
  this->m_nickname = copy.m_nickname;
  this->m_username = copy.m_username;
}

User::~User() { print_log("destructor called"); }

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

std::ostream &operator<<(std::ostream &os, const User &user) {
  os << "Nickname: " << user.getNickname()
     << ", Username: " << user.getUsername();
  return os;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void User::setNickname(std::string const &nickname) { m_nickname = nickname; }

void User::setUsername(std::string const &username) { m_username = username; }

std::string const &User::getNickname() const { return m_nickname; }
std::string const &User::getUsername() const { return m_username; }

// -------------------------------------------------------------------------- //
