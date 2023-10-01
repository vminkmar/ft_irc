// -------------------------------------------------------------------------- //

#include "../include/Channel.hpp" // needed for Channel class
								  //
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

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Channel::Channel(std::string const& name) : m_name(name),
                                            m_userLimit(10),
                                            m_inviteOnly(false)
{
	print_log("name constructor called");
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

std::string Channel::getUsers() const
{
	if (m_users.empty() == true)
	{
		return "No Users!";
	}

	std::stringstream ss;
	for (std::vector<User*>::const_iterator itr = m_users.begin();
                                            itr != m_users.end();
			                                ++itr)
	{
		ss << (*itr)->getNickname();
		if (itr + 1 != m_users.end())
		{
			ss << ", ";
		}
	}
	return ss.str();
}

void Channel::toggleInviteOnly()
{
	m_inviteOnly = !m_inviteOnly;
}

bool Channel::isInviteOnly() const
{
	return m_inviteOnly;
}

void Channel::addUser(User* u)
{
	m_users.push_back(u);
	return ;
}

void Channel::removeUser(User const& u)
{
	for (std::vector<User*>::iterator itr = m_users.begin();
		                              itr != m_users.end();)
	{
		if (&u == *itr)
		{
			itr = m_users.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

//void Channel::sendBroadcast(std::string& message) const
//{
//	(void) message;
//	return ;
//}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> operator overloads */

std::ostream& operator<<(std::ostream& os, Channel const & c)
{
	std::cout << "Channel:      " << c.getName() << "\n";
	
	std::cout << "Topic:        ";
	if (c.getTopic().empty() == true)
	{
		std::cout << "not set\n";
	}
	else
	{
		std::cout << c.getTopic() << "\n";
	}

	std::cout << "Password:     ";
	if (c.getPassword().empty() == true)
	{
		std::cout << "not set\n";
	}
	else
	{
		std::cout << c.getPassword() << "\n";
	}

	std::cout << "User limit:   " << c.getUserLimit() << "\n";

	std::cout << "isInviteOnly: ";
	if (c.isInviteOnly() == true)
	{
		std::cout << "true\n";
	}
	else
	{
		std::cout << "false\n";
	}

	std::cout << "Users:        " << c.getUsers();
	return os;
}

// -------------------------------------------------------------------------- //
