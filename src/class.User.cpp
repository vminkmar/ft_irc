// -------------------------------------------------------------------------- //

#include "../include/class.User.hpp" // needed for User class, t_str, ENUM

#include <iostream> // needed for std::ostream

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(t_str_c& message){
  if (DEBUG) {
    std::cerr << YELLOW << "User: " << message << RESET << std::endl;
  }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

User::User() : m_onlineStatus(true), m_welcomed(false){
    print_log("default constructor called");
}

User::User(const User &copy) : m_nickname(copy.m_nickname),
                               m_username(copy.m_username),
                               m_inputBuffer(copy.m_inputBuffer),
                               m_outputBuffer(copy.m_outputBuffer),
							   m_onlineStatus(copy.m_onlineStatus),
                               m_welcomed(copy.m_welcomed){
  print_log("copy constructor called");
}

User::~User(){
    print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> setters */

void User::setNickname(t_str_c& nickname){
    m_nickname = nickname;
}

void User::setUsername(t_str_c& username){
    m_username = username;
}

void User::setRegisteredStatus(bool flag){
	if(flag == true){
		m_registeredStatus = true;
	}
	else{
		m_registeredStatus = false;
	}
}

void User::appendInputBuffer(t_str_c& message){
	m_inputBuffer.append(message);
}

void User::appendOutputBuffer(t_str_c& message){
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

void User::toggleWelcomedStatus(){
    m_welcomed = !m_welcomed;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> getters */

t_str_c& User::getNickname() const{
    return m_nickname;
}

t_str_c& User::getUsername() const{
    return m_username;
}

t_str_c& User::getInputBuffer() const{
    return m_inputBuffer;
}

t_str_c& User::getOutputBuffer() const{
	return m_outputBuffer;
}

bool User::getOnlineStatus() const{
    return m_onlineStatus;
}

bool User::getWelcomedStatus() const{
    return m_welcomed;
}

bool User::getRegisteredStatus() const{
	return m_registeredStatus;
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
		m_onlineStatus = src.m_onlineStatus;
    m_welcomed = src.m_welcomed;
	}
	return *this;
}

// -------------------------------------------------------------------------- //
