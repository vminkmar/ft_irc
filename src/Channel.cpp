// -------------------------------------------------------------------------- //

#include "../include/Channel.hpp" // needed for Channel class

#include <iostream>  // needed for std::cerr, std::endl

#ifndef DEBUG
# define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET  "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message){
    if (DEBUG){
        std::cerr << YELLOW << "Channel: " << message << RESET << std::endl;
    }
}

static inline void print_bool(std::ostream& os, bool is, bool nl){
    if (is == true){
		os << "true";
	}
    else{
		os << "false";
	}
    if (nl == true){
		os << "\n";
	}
}

static inline void print_stdstr(std::ostream& os, std::string str, bool nl){
    if (str.empty() == true){
		os << "not set";
	}
    else{
		os << str;
	}
    if (nl == true){
		os << "\n";
	}
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Channel::Channel(std::string const& name) : m_name(name),
                                            m_userLimit(USER_LIMIT_MAX),
                                            m_inviteOnly(false),
                                            m_topicEditable(true){
	print_log("name constructor called");
}

Channel::Channel(){
	print_log("default constructor called");
}

Channel::Channel(Channel const& src){
	print_log("copy constructor called");
	m_name = src.getName();
}

Channel::~Channel(){
    print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> setters */

void Channel::setName(std::string const& newName){
	m_name = newName;
}

void Channel::setTopic(std::string const& newTopic){
	m_topic = newTopic;
}

void Channel::setPassword(std::string const& newPassword){
	m_password = newPassword;
}

void Channel::setUserLimit(unsigned int newLimit){
	m_userLimit = newLimit;
}

void Channel::toggleInviteOnly(){
	m_inviteOnly = !m_inviteOnly;
}

void Channel::toggleTopicEditable(){
	m_topicEditable = !m_topicEditable;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> getters */

std::string const& Channel::getName() const{
	return m_name;
}

std::string const& Channel::getTopic() const{
	return m_topic;
}

std::string const& Channel::getPassword() const{
	return m_password;
}

unsigned int Channel::getUserLimit() const{
	return m_userLimit;
}

std::map<int, UserPrivilege> const& Channel::getUserMap() const{
    return m_users;
}

unsigned int Channel::getNumberOfUsers() const{
    return m_users.size();
}

int Channel::getFirstUserSocket() const{
    if (m_users.begin() != m_users.end()){
        return m_users.begin()->first;
    }
    throw std::runtime_error("getFirstUserSocket: No Users in Channel!");
}

bool Channel::isInviteOnly() const{
	return m_inviteOnly;
}

bool Channel::isTopicEditable() const{
	return m_topicEditable;
}

bool Channel::isChannelKey() const{
    return m_password.empty() == false;
}

bool Channel::isFull() const{
    return m_users.size() >= m_userLimit;
}

bool Channel::hasOperator() const{
    for (t_channel_users_cit it = m_users.begin(); it != m_users.end(); ++it){
        if (it->second == OPERATOR){
            return true;
        }
    }
    return false;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Channel::addUser(int socket, UserPrivilege up){
	m_users[socket] = up;
}

void Channel::eraseUser(int socket){
	m_users.erase(socket);
}

bool Channel::isMember(int socket) const{
    return m_users.find(socket) != m_users.end();
}

bool Channel::isOperator(int socket) const{
    /*@note might change this because user has to make sure this member is part
     * of the channel */
    return m_users.find(socket) != m_users.end() 
           && m_users.find(socket)->second == OPERATOR;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

Channel& Channel::operator=(Channel const& src){
	print_log("assignment operator called");
	if (this != &src)
	{
		m_name = src.getName();
		m_topic = src.getTopic();	
		m_password = src.getPassword();
		m_userLimit = src.getUserLimit();
		m_inviteOnly = src.isInviteOnly();
		m_topicEditable = src.isTopicEditable();
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, Channel const & c){
	os << "Channel:         " << c.getName() << "\n";
	
	os << "Topic:           ";
	print_stdstr(os, c.getTopic(), true);

	os << "Password:        ";
	print_stdstr(os, c.getPassword(), true);

	os << "User limit:      " << c.getUserLimit() << "\n";

	os << "isInviteOnly:    ";
	print_bool(os, c.isInviteOnly(), true);

	os << "isTopicEditable: ";
	print_bool(os, c.isTopicEditable(), true);

	os << "isChannelKey:    ";
	print_bool(os, c.isChannelKey(), true);

	return os;
}

// -------------------------------------------------------------------------- //
