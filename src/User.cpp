// -------------------------------------------------------------------------- //

#include "../include/User.hpp" // needed for User class, std::string, ENUM
#include <iostream>            // needed for std::ostream

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string const& message){
  if (DEBUG) {
    std::cerr << YELLOW << "User: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

User::User() : m_onlineStatus(true){
    print_log("default constructor called");
}

User::User(const User &copy) : m_nickname(copy.m_nickname),
                               m_username(copy.m_username),
                               m_inputBuffer(copy.m_inputBuffer),
                               m_outputBuffer(copy.m_outputBuffer){
  print_log("copy constructor called");
}

User::~User(){
    print_log("destructor called");
}


/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> setters */

void User::setNickname(std::string const& nickname){
    m_nickname = nickname;
}

void User::setUsername(std::string const& username){
    m_username = username;
}

void User::appendInputBuffer(std::string const& message){
	m_inputBuffer.append(message);
}

void User::appendOutputBuffer(std::string const& message){
	m_outputBuffer.append(message);
}

void User::eraseInputBuffer(int start, int end) {
  m_inputBuffer = m_inputBuffer.erase(start, end);
}
void User::eraseOutputBuffer(int start, int end) {
  m_outputBuffer = m_outputBuffer.erase(start, end);
}

void User::toggleOnlineStatus(){
    m_onlineStatus = !m_onlineStatus;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> getters */

std::string const& User::getNickname() const{
    return m_nickname;
}

std::string const& User::getUsername() const{
    return m_username;
}

std::string const& User::getInputBuffer() const{
    return m_inputBuffer;
}

std::string const& User::getOutputBuffer() const{
	return m_outputBuffer;
}

bool User::getOnlineStatus() const{
    return m_onlineStatus;
}


/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

std::ostream &operator<<(std::ostream &os, const User &user){
  os << "Nickname: "   << user.getNickname()
     << ", Username: " << user.getUsername();
  return os;
}

User& User::operator=(const User& src){
	print_log("assignment operator called");
	if (this != &src){
		m_nickname = src.m_nickname;
		m_username = src.m_username;
        m_inputBuffer = src.m_inputBuffer;
        m_outputBuffer = src.m_outputBuffer;
	}
	return *this;
}

// -------------------------------------------------------------------------- //
