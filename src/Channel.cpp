// -------------------------------------------------------------------------- //

#include "../include/Channel.hpp" // needed for Channel class

#include <iostream>  // needed for std::cerr, std::cout, std::endl
#include <sstream>   // needed for std::stringstream

#ifndef DEBUG
# define DEBUG 0
#endif // DEBUG

#define YELLOW "\033[33m"
#define RESET  "\033[0m"

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> non-class functions */

static inline void print_log(std::string message)
{
    if (DEBUG)
    {
        std::cerr << YELLOW << "Channel: "
                  << message << RESET << std::endl;
    }
}

static inline void print_bool(std::ostream& os, bool is, bool nl)
{
	if (is == true)
	{
		os << "true";
	}
	else
	{
		os << "false";
	}
	if (nl == true)
	{
		os << "\n";
	}
}

static inline void print_stdstr(std::ostream& os, std::string str, bool nl)
{
	if (str.empty() == true)
	{
		os << "not set";
	}
	else
	{
		os << str;
	}
	if (nl == true)
	{
		os << "\n";
	}
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Channel::Channel(std::string const& name) : m_name(name),
                                            m_userLimit(10),
                                            m_inviteOnly(false),
											m_topicEditable(true),
											m_channelKey(false)
																
{
	print_log("name constructor called");
}

Channel::Channel()
{
	print_log("default constructor called");
}

Channel::Channel(Channel const& src){
	print_log("copy constructor called");
	this->m_name = src.getName();
}

Channel::~Channel()
{
    print_log("destructor called");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Channel::setName(std::string const& newName) 
{
	m_name = newName;
}

void Channel::setTopic(std::string const& newTopic)
{
	m_topic = newTopic;
}

void Channel::setPassword  (std::string const& newPassword)
{
	m_password = newPassword;
}

void Channel::setUserLimit (unsigned int newLimit)
{
	m_userLimit = newLimit;
}

void Channel::toggleInviteOnly()
{
	m_inviteOnly = !m_inviteOnly;
}

void Channel::toggleTopicEditable()
{
	m_topicEditable = !m_topicEditable;
}

void Channel::toggleChannelKey()
{
	m_channelKey = !m_channelKey;
}

std::string const& Channel::getName() const
{
	return m_name;
}

std::string const& Channel::getTopic() const
{
	return m_topic;
}

std::string const& Channel::getPassword() const
{
	return m_password;
}

unsigned int Channel::getUserLimit() const
{
	return m_userLimit;
}

std::map<int, UserPrivilege> const& Channel::getUserMap() const
{
    return m_users;
}

bool Channel::isInviteOnly() const
{
	return m_inviteOnly;
}

bool Channel::isTopicEditable() const
{
	return m_topicEditable;
}

bool Channel::isChannelKey() const
{
	return m_channelKey;
}

void Channel::addUser(int socket, UserPrivilege up)
{
	m_users[socket] = up;
}

void Channel::removeUser(int socket)
{
	m_users.erase(socket);
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

Channel& Channel::operator=(Channel const& src)
{
	print_log("assignment operator called");
	if (this != &src)
	{
		m_name = src.getName();
		m_topic = src.getTopic();	
		m_password = src.getPassword();
		m_userLimit = src.getUserLimit();
		m_inviteOnly = src.isInviteOnly();
		m_topicEditable = src.isTopicEditable();
		m_channelKey = src.isChannelKey();
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, Channel const & c)
{
	std::cout << "Channel:         " << c.getName() << "\n";
	
	std::cout << "Topic:           ";
	print_stdstr(std::cout, c.getTopic(), true);

	std::cout << "Password:        ";
	print_stdstr(std::cout, c.getPassword(), true);

	std::cout << "User limit:      " << c.getUserLimit() << "\n";

	std::cout << "isInviteOnly:    ";
	print_bool(std::cout, c.isInviteOnly(), true);

	std::cout << "isTopicEditable: ";
	print_bool(std::cout, c.isTopicEditable(), true);

	std::cout << "isChannelKey:    ";
	print_bool(std::cout, c.isChannelKey(), true);

	return os;
}

// -------------------------------------------------------------------------- //
